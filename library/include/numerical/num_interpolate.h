// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ********************************************************************
// Definitions of functions in class interpolator<>
//
// F. Rice 4/9/99
// ********************************************************************

#include "error.h"

// --------------------------------------------------------------------
// constructors; clear(); operator =

template < class Y_type > inline 
interpolator<Y_type>::interpolator()
  : ready_(false), no_warn_(false), type_(SPLINE), user_ls(false), user_rs(false)
{ }


template < class Y_type > inline 
interpolator<Y_type>::interpolator(const interpolator<Y_type> & f)
  : data(f.data), aux(f.aux), table(),
    ready_(false), no_warn_(f.no_warn_), type_(f.type_),
    user_ls(f.user_ls), user_rs(f.user_rs)
{ 
  if (user_ls) lslope = f.lslope;
  if (user_rs) rslope = f.rslope;
  if (f.ready_) build(); 
}


template < class Y_type > inline 
interpolator<Y_type> & interpolator<Y_type>::clear()
{
  // conditions on exit from clear():
  //   (1) data and aux are empty
  //   (2) table is empty
  //   (3) ready() == false, user_ls == false, user_rs == false, type_ = SPLINE

  data.erase(data.begin(), data.end());
  aux.erase(aux.begin(), aux.end());
  table.erase(table.begin(), table.end());
  ready_ = user_ls = user_rs = false;
  type_ = SPLINE;

  return *this;
}


template < class Y_type > inline 
interpolator<Y_type> & interpolator<Y_type>::operator =
(const interpolator<Y_type> & f)
{
  data = f.data; aux = f.aux; table.erase(table.begin(),table.end());
  ready_ = false; no_warn_ = f.no_warn_; type_ = f.type_;
  user_ls = f.user_ls; user_rs = f.user_rs;
  if (user_ls) lslope = f.lslope;
  if (user_rs) rslope = f.rslope;
  if (f.ready_) build(); 
  return *this;
}


// --------------------------------------------------------------------
// set-up routines: add(), type(), left_slope(), right_slope()

template < class Y_type > inline 
interpolator<Y_type> & interpolator<Y_type>::add(const double x, const Y_type & y)
{
  static data_type temp;             // declared static so allocation occurs once
  temp.first = x, temp.second = y;

  // invariants on entry to and exit from add():
  //   (1) data is sorted by x values, lowest x first, all x's unique
  //   (2) aux has the same size as data

  if ( data.size() == 0 || data.back().first < x ) {
    // new data just goes to the back
    data.push_back(temp);   // just add new data to back
  }
  else {
    // need to enter data somewhere else
    data_iter j = data.begin();
    while((j->first) < x) j++;
    if ((j->first) == x)  // a duplicate x value
      error::fatal("x values input to interpolator are not all unique.");
    data.insert(j, temp); // maintain invariant (1)
  }

  aux.push_front(temp); // now restore invariant (2), using temp as junk

  return *this;
}

template < class Y_type > inline 
interpolator<Y_type> & interpolator<Y_type>::add(const interpolator<Y_type> & Z)
{
  data_type temp;
  double & x = temp.first;
  Y_type & y = temp.second;

  // loop through the supplied interpolator's data list
  for(typename interpolator<Y_type>::const_data_iter i = Z.data.begin(); i != Z.data.end(); ++i) {
    x = i->first; y = i->second;

    // invariants on entry to and exit from add():
    //   (1) data is sorted by x values, lowest x first, all x's unique
    //   (2) aux has the same size as data

    if ( data.size() == 0 || data.back().first < x ) {
      // new data just goes to the back
      data.push_back(temp);   // just add new data to back
    }
    else {
      // need to enter data somewhere else
    data_iter j = data.begin();
    while((j->first) < x) j++;
      if ((j->first) != x)  // not a duplicate x value
	data.insert(j, temp); // maintain invariant (1)
    }

    aux.push_front(temp); // now restore invariant (2), using temp as junk
  }

  return *this;
}


template < class Y_type > inline 
interpolator<Y_type> & interpolator<Y_type>::type(int t)
{
  ready_ = false;     // must rebuild before using
  switch(t) {
  default:
  case LINEAR: { type_ = LINEAR; break; }
  case SPLINE: { type_ = SPLINE; break; }
  }
  return *this;
}


template < class Y_type > inline 
interpolator<Y_type> & interpolator<Y_type>::left_slope(const Y_type & y)
{ lslope = y; user_ls = true; ready_ = false; return *this; }


template < class Y_type > inline 
interpolator<Y_type> & interpolator<Y_type>::right_slope(const Y_type & y)
{ rslope = y; user_rs = true; ready_ = false; return *this; }


// --------------------------------------------------------------------
// build routines: build(), build_linear(), build_spline()


template < class Y_type > inline 
interpolator<Y_type> & interpolator<Y_type>::build()
{
  // assumed conditions on entry to build()
  //   (1) data.size() == aux.size()
  //   (2) data is sorted by x with all x's unique
  //
  // conditions on entry for build() to do anything useful
  //   (1) data and aux each have size() > 1
  //
  // conditions on successful exit from build():
  //   (1) table, data and aux all have the same size() > 1
  //   (2) ready() == 1
  //   and for all i (except the last i for (6))
  //   (3) table[i].first == data[i].first == x[i]
  //   (4) table[i].second.first == data.begin()+i => (x[i],y[i])
  //   (5) table[i].second.second == aux.begin()+i => (dx[i],yp[i])
  //   (6) (aux.begin()+i)->first
  //          == (data.begin()+i+1)->first - (data.begin()+i)->first
  //          == dx[i] = x[i+1]-x[i]

  typename data_list::size_type size = data.size();

  if (size != aux.size())
    error::fatal("Internal error in interpolator: "
		 "internal tables inconsistent");

  if (size <= 1) {
    error::warning("Not enough data to interpolate in interpolator. "
		   "Add more data and build again.");
    return *this;
  }

  // passed the tests, so here goes:

  table.erase(table.begin(), table.end());  // clear out old table
  table.reserve(size);                      // and get enough space for new

  data_iter i_data = data.begin(), i_aux = aux.begin();
  typename data_list::size_type i = 0;  // counts elements we've processed
  double x;
  entry  t;
  while ( ++i < size ) {  // loop over all but the final entry
    x = i_data->first;    // x[i] value here
    t.first = x; t.second.first = i_data; t.second.second = i_aux;
    table.push_back(t);

    // The next line computes aux[i].first = x[i+1] - x[i], incrementing iterators. 
    (i_aux++)->first = (++i_data)->first - x;

  } // while

  // now i_data and i_aux point to the final entries
  t.first = i_data->first; t.second.first = i_data; t.second.second = i_aux;
  table.push_back(t);

  // processing of the aux Y data here
  switch (type_) {
  case LINEAR: { build_linear(); break; }
  case SPLINE: { build_spline(); break; }
  default: 
    { error::fatal("Unknown interpolation type in interpolator."); }
  }

  ready_ = true;
  return *this;
}


template < class Y_type > inline 
void interpolator<Y_type>::build_linear()
{ 
  // aux Y values contain Y' to next point, to speed up interpolations
  // Endpoint slopes are calculated and stored in lslope, rslope if required.

  // convenient readablility macros:
  #define Y(i)  ((i)->second.first->second)
  #define Yp(i) ((i)->second.second->second)
  #define Dx(i) ((i)->second.second->first)

  table_iter i, last = table.end() - 1;
  for (i = table.begin(); i != last; ++i) {
    Yp(i) = Y(i+1);  Yp(i) -= Y(i); Yp(i) *= 1.0/Dx(i);
  }

  // left extrapolation slope value
  if (!user_ls) lslope = Yp(table.begin());

  // right extrapolation slope value
  if (!user_rs) rslope = Yp(last-1);

  #undef Y
  #undef Yp
  #undef Dx
}


template < class Y_type > inline 
void interpolator<Y_type>::build_spline()
{ 
  // the aux Y[i] will hold Y''(x[i]) for the cubic spline interpolation.
  // boundary conditions: if an endpoint slope is specified, use it; otherwise
  // set Y'' at the endpoint to 0. The Y(x[i]) and the continuity of Y'' now
  // give a tridiagonal set of equations for the Y''[i] like:
  //    a[i]*Y''[i-1] + b[i]*Y''[i] + c[i]*Y''[i+1] == r[i]
  // Note that the a,b,c are scalars (double); the r are Y_type
  //
  // This algorithm is a highly modified form of that in Numerical Recipes.

  static std::vector<double> c;    // holds the -c[i]; static avoids reallocation,
                                   // so multiple builds are faster
  c.empty(); c.reserve(table.size());

  unsigned long i;
  unsigned long last = table.size()-1;  // the last data element

  // convenient readablility macros:
  #define Y(i)  (table[(i)].second.first->second)
  #define Yp(i) (table[(i)].second.second->second)
  #define Dx(i) (table[(i)].second.second->first)
  #define C(i)  (c[(i)])
  #define R(i)  (Yp(i))

  // The equations relating the various y''[i] form the following tridiagonal system,
  // for 0 < i < table.size() (first eqn (i == 0) comes from boundary condition):
  //                a y''[i-1] + b y''[i] + c y''[i+1] = r
  // where:
  //   a = (x[i]-x[i-1])/(x[i+1]-x[i-1])
  //   b = 2
  //   c = (x[i+1]-x[i])/(x[i+1]-x[i-1])
  //   r = 6((y[i+1]-y[i])/(x[i+1]-x[i]) - (y[i]-y[i-1])/(x[i]-x[i-1]))/(x[i+1]-x[i-1])

  // The first step: diagonalize the system using Gaussian elimination:
  // invariants in the elimination loop, 0 <= j < i < table.size()-1
  //  (1) a[j]  == 0
  //  (2) b[j]  == 1
  //  (3) C(j)  == -c[j]       ( use vector c to temporarily hold the -c[j] )
  //  (4) Yp(j) == r[j]        ( use the aux Y list to temporarily hold the r[j] )
  //  (5) b[i]  == 2  (a literal constant) at start of each iteration i
  //
  // Also recall that the Dx(i) == x[i+1]-x[i], stored in aux X values.

  // set up the equation for i == 0; (b[0] == 1, c[0] == -C(0), r[0] == R(0)):
  if (user_ls) {
    C(0) = -0.5;
    R(0) = Y(1); R(0) -= Y(0); R(0) *= (1.0/Dx(0));  // r = linear slope between points
    R(0) -= lslope;       // subtract off user-specified slope
    R(0) *= (3.0/Dx(0));  // this is the final r[0]
  }
  else {
    R(0) *= (C(0) = 0.0); // the natural spline: Y''[0] == 0  ( b y''[0] == 0; b == 1 )
  }

  double b, a;
  Y_type m1, m2;
  m1 = Y(1); m1 -= Y(0); m1 *= 6.0/Dx(0);      // 6 * slope from point 0 to 1
  for (i = 1; i < last; ++i) {
    // the initial a, b, c, r for row i:
    a = 1.0/(Dx(i)+Dx(i-1));                   // temporarily == 1/(x[i+1]-x[i-1])
    m2 = Y(i+1); m2 -= Y(i); m2 *= 6.0/Dx(i);  // 6 * slope from i to i+1
    R(i) = m2; R(i) -= m1; R(i) *= a;          // initial r (m1=m2 from the last iter)
    m1 = m2;                                   // for the next iteration
    a *= Dx(i-1);                              // == (x[i]-x[i-1])/(x[i+1]-x[i-1])
    b = 2.0;
    C(i) = a - 1.0;                            // == -(x[i+1]-x[i])/(x[i+1]-x[i-1])

    // now the Gaussian elimiation step (b[i-1] == 1 from the loop invariants):
    b += a*C(i-1);                  // b[i] = b[i] - a[i]*c[i-1]; now a[i] == 0
    m2 = R(i-1); m2 *= -a;          // avoids a Y_type temporary allocation in next line
    R(i) += m2;                     // r[i] = r[i] - a[i]*r[i-1]
                                    // c[i] = c[i] - a[i]*0 (tridiagonal), so no change
    b = 1.0/b;                      // to multiply instead of divide below
    C(i) *= b;                      // normalize so b[i] == 1
    R(i) *= b;                      // normalize so b[i] == 1
  }

  // Gaussian elimination and back substitution of the last row, where there is no C[i]
  if (user_rs) {
    Yp(last) = rslope; Yp(last) *= 6.0;
    Yp(last) -= m1;  // m1 == 6*slope from previous point
    Yp(last) *= 1.0/Dx(last-1);
    Yp(last) -= R(last-1);
    Yp(last) *= 1.0/(2.0 + C(last-1));
  }
  else {
    Yp(last) *= 0.0;  // the natural spline: Y''[last] == 0
  }

  // back substitution of the remaining rows
  for (i = last; i > 0; --i) {
    m2 = Yp(i); m2 *= C(i-1);
    Yp(i-1) += m2;             // remember, C(j) == -c for jth row
  }

  // now set lslope and rslope if necessary
  if (!user_ls) {
    lslope = Y(0); lslope *= -1.0; lslope += Y(1); lslope *= 1.0/Dx(0); // slope fm 0 to 1
    m2 = Yp(0); m2 *= 2.0; m2 += Yp(1); m2 *= -Dx(0)/6.0;
    lslope += m2;
  }
  if (!user_rs) {
    rslope = Y(last-1); rslope *= -1.0; 
    rslope += Y(last); rslope *= 1.0/Dx(last-1); // slope fm last-1 to last
    m2 = Yp(last); m2 *= 2.0; m2 += Yp(last-1); m2 *= Dx(last-1)/6.0;
    rslope += m2;
  }

  #undef Y
  #undef Yp
  #undef Dx
  #undef C
  #undef R
}


// --------------------------------------------------------------------
// interpolation routines: bsearch(), (), linear(), spline(), lextrapolate(), 
//         rextrapolate(), prime(), val_prime(), prime_linear(), prime_spline()

// this function is like the stl "lower_bound" algorithm, except
// specialized for our application, and using indexes rather than
// iterators into table. Returns the index of the first element of
// table whose x is not less than the supplied argument, or table.size()
// if all elements are less than x.

template < typename Y_type > inline 
unsigned long interpolator<Y_type>::bsearch(double x) const
{
  unsigned long lo = 0;
  unsigned long len = table.size();
  unsigned long test, h;
  while( len > 0) {
    h = len >> 1;
    test = lo + h;
    if( table[test].first < x ) {
      lo = test + 1;
      len -= h + 1;
    }
    else
      len = h;
  }
  return lo;
}
    

template < class Y_type > inline 
Y_type interpolator<Y_type>::operator ()(double x) const
{
  if (!ready_)
    error::fatal("Must build interpolator before use.");

  // attempt to find where in table to interpolate:
  unsigned long j = bsearch(x); // table[j] >= x;

  // check if extrapolation is needed, else perform the interpolation
  if (j == 0) {
    lextrapolate(x);
    if (!no_warn_ && x < table[j].first)
      error::warning("Interpolator extrapolating beyond range of data points.");
  }
  else if (j == table.size()) {
    rextrapolate(x);
    if (!no_warn_)
      error::warning("Interpolator extrapolating beyond range of data points.");
  }
  else {
    --j;  // now j is such that: x[j] < x <= x[j+1]
    switch (type_) {
    case LINEAR: { linear(j,x); break; }
    case SPLINE: { spline(j,x); break; }
    default: 
      { error::fatal("Unknown interpolation type in interpolator."); }
    }
  }

  return result;
}


template < class Y_type > inline 
void interpolator<Y_type>::linear
(unsigned long i, double x, Y_type & r) const
{ 
  r = table[i].second.second->second;  // Y'[i]
  r *= x - table[i].first;             // Y'[i]*(x - x[i])
  r += table[i].second.first->second;  // Y[i] + Y'[i]*(x - x[i])
}


template < class Y_type > inline 
void interpolator<Y_type>::spline
(unsigned long i, double x, Y_type & r) const
{
  double D  = x - table[i].first;                // D  == x - x[i]
  double Do = table[i].second.second->first;     // Do == x[i+1] - x[i]
  double A = D/Do, B = 1.0 - A;

  Y_type & T1 = r;   // T1 aliases r, and will hold result
  T1 = table[i].second.second->second;           // T1 == Y''[i]
  Y_type T2(table[i+1].second.second->second);   // T2 == Y''[i+1]

  T1 *= -Do*Do*A*(B+1)/6.0; T2 *= -Do*Do*B*(A+1)/6.0;
  T1 += table[i].second.first->second; T1 *= B;
  T2 += table[i+1].second.first->second; T2 *= A;
  T1 += T2;   // T1 has the result
}


template < class Y_type > inline 
void interpolator<Y_type>::lextrapolate(double x, Y_type & r) const
{
  r = lslope;
  r *= (x - table[0].first);
  r += table[0].second.first->second;
}


template < class Y_type > inline 
void interpolator<Y_type>::rextrapolate(double x, Y_type & r) const
{
  r = rslope;
  unsigned long last = table.size() - 1;
  r *= (x - table[last].first);
  r += table[last].second.first->second;
}


template < class Y_type > inline 
Y_type interpolator<Y_type>::prime(double x) const
{
  if (!ready_)
    error::fatal("Must build interpolator before use.");

  // attempt to find where in table to interpolate:
  unsigned long j = bsearch(x); // table[j] >= x;

  // check if extrapolation is needed, else perform the interpolation
  if (j == 0) {
    result = lslope;   // linear extrapolation has a constant slope
    if (!no_warn_ && x < table[j].first)
      error::warning("Interpolator extrapolating beyond range of data points.");
  }
  else if (j == table.size()) {
    result = rslope;
    if (!no_warn_)
      error::warning("Interpolator extrapolating beyond range of data points.");
  }
  else {
    --j;  // now j is such that: x[j] < x <= x[j+1]
    switch (type_) {
    case LINEAR: { prime_linear(j,x); break; }
    case SPLINE: { prime_spline(j,x); break; }
    default: 
      { error::fatal("Unknown interpolation type in interpolator."); }
    }
  }

  return result;
}


template < class Y_type > inline 
void interpolator<Y_type>::prime_linear
(unsigned long i, double x, Y_type & r) const
{
  // convenient readablility macros:
  #define Y(i)  (table[(i)].second.first->second)
  #define Yp(i) (table[(i)].second.second->second)
  #define Dx(i) (table[(i)].second.second->first)

  // calculate slopes at points i and i+1:
  Y_type yp1;
  Y_type & yp2 = r;  // alias r with yp2;
  if (i == 0) {    // i is left endpoint
    yp1 = lslope;
  }
  else {
    yp1 = Yp(i-1); yp1 *= Dx(i)/Dx(i-1); yp1 += Yp(i);
    yp1 *= Dx(i-1)/(Dx(i)+Dx(i-1));
  }
  if (i == table.size()-2) {   // i+1 is right endpoint
    yp2 = rslope;
  }
  else {
    yp2 = Yp(i+1); yp2 *= Dx(i)/Dx(i+1); yp2 += Yp(i);
    yp2 *= Dx(i+1)/(Dx(i)+Dx(i+1));
  }

  // now do a linear interpolation between yp1 and yp2:
  double A = (x-table[i].first)/Dx(i);
  yp2 -= yp1; yp2 *= A; yp2 += yp1;    // yp2 is the result

  #undef Y
  #undef Yp
  #undef Dx
}


template < class Y_type > inline 
void interpolator<Y_type>::prime_spline
(unsigned long i, double x, Y_type & r) const
{ 
  // convenient readablility macros:
  #define Y(i)  (table[(i)].second.first->second)
  #define Yp(i) (table[(i)].second.second->second)
  #define Dx(i) (table[(i)].second.second->first)

  Y_type yp(Y(i+1)); yp -= Y(i); yp *= 1.0/Dx(i);  // the average slope from i to i+1
  double B = (x-table[i].first)/Dx(i), A = 1.0-B;  // B == (x-x[i])/(x[i+1]-x[i])

  // using the yp, A and B defined above, the formula for Y'(x) is:
  // Y'(x) = yp - (3 A^2 - 1)/6 Dx(i) Y''(i) + (3 B^2 - 1)/6 Dx(i) Y''(i+1)
 
  A = (3.0*A*A-1.0)*Dx(i)/6.0; B = (3.0*B*B-1.0)*Dx(i)/6.0;

  r = Yp(i+1); r *= B/A; r -= Yp(i); r *= A;
  r += yp;

  #undef Y
  #undef Yp
  #undef Dx
}


template < class Y_type > inline 
int interpolator<Y_type>::val_prime
(double x, Y_type & y, Y_type & y_prime) const
{
  if (!ready_)
    error::fatal("Must build interpolator before use.");
  int flag = 0;  // will hold the returned extrapolation flag value

  // attempt to find where in table to interpolate:
  unsigned long j = bsearch(x); // table[j] >= x;

  // check if extrapolation is needed, else perform the interpolation
  if (j == 0) {
    lextrapolate(x,y);
    y_prime = lslope;   // linear extrapolation has a constant slope
    if ( x < table[j].first) {
      flag = -1;
      if (!no_warn_)
	error::warning("Interpolator extrapolating beyond range of data points.");
    }
  }
  else if (j == table.size()) {
    rextrapolate(x,y);
    y_prime = rslope;
    flag = 1;
    if (!no_warn_)
      error::warning("Interpolator extrapolating beyond range of data points.");
  }
  else {
    --j;  // now j is such that: x[j] < x <= x[j+1]
    switch (type_) {
    case LINEAR: { linear(j,x,y); prime_linear(j,x,y_prime); break; }
    case SPLINE: { spline(j,x,y); prime_spline(j,x,y_prime); break; }
    default: 
      { error::fatal("Unknown interpolation type in interpolator."); }
    }  
  }
  return flag;
}


// ********************************************************************
// debug functions for interpolator class

template < class Y_type > inline 
void interpolator<Y_type>::list_data_list() const
{
  for (typename interpolator<Y_type>::const_data_iter i = data.begin(); i != data.end(); ++i)
    std::cout << i->first <<" , " << i->second << std::endl;
} 

template < class Y_type > inline 
void interpolator<Y_type>::list_lists() const
{
  for (unsigned i = 0; i < table.size(); ++i) {
    const entry & t = table[i];
    std::cout << t.first 
	      << "\t|\t" << t.second.first->first << " , " << t.second.first->second
	      << "\t|\t" << t.second.second->first << " , " << t.second.second->second
	      << std::endl;
  }
}

// end num_interpolate.h
