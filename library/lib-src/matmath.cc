// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004, 2009 California Institute of Technology.
// All rights reserved.
//
// matmath.cc
// 5/8/09: commented out unused ResultModeMin()

#include "matmath.h"
#include "Amath.h"

// ************************************************************************
// Helper functions:

inline int min(int a, int b)  { return (a < b)? a : b; }
inline int max(int a, int b)  { return (a > b)? a : b; }
inline int length(int a, int b) { int n = b - a + 1; return (n < 0)? 0 : n; }

// Determine the result index mode which maximizes number of elements:
static v_index_mode ResultModeMax(const v_index_mode a, const v_index_mode b)
{
  if((a == Index_S)||(b == Index_S)) return Index_S;
  else if ((a == Index_C)||(b == Index_C)) return Index_C;
  else return Index_1;
}

// Determine the result index mode which minimizes number of elements:
// static v_index_mode ResultModeMin(const v_index_mode a, const v_index_mode b)
// {
//   if((a == Index_1)||(b == Index_1)) return Index_1;
//   else if ((a == Index_C)||(b == Index_C)) return Index_C;
//   else return Index_S;
// }

// Determine the maxindex which maximizes the number of elements:
inline int ResultIndexMax(const int i1, const int i2)
{ return max(i1,i2); }

// Determine the maxindex which minimizes the number of elements:
inline int ResultIndexMin(const int i1, const int i2)
{ return min(i1,i2); }


// ************************************************************************
// Vector and Matrix norms:

static double norm_accumulator;  // static, so confined to this file

static inline void norm_add(double x)
{ norm_accumulator += x*x; }

static inline void norm_add(const Complex & x)
{ norm_accumulator += norm(x); }

static inline void norm_bigger(double x)
{ x *= x; if (x > norm_accumulator) norm_accumulator = x; }

static inline void norm_bigger(const Complex & x)
{ double u = norm(x); if (u > norm_accumulator) norm_accumulator = u; }

double max_norm(const real_vector & x)
{
  /*register*/ int i;
  /*register*/ int m = x.maxindex();
  norm_accumulator = 0;
  for (i = x.minindex(); i <= m; ++i)
    norm_bigger(x[i]);
  return norm_accumulator;
}
  
double max_norm(const complex_vector & x)
{
  /*register*/ int i;
  /*register*/ int m = x.maxindex();
  norm_accumulator = 0;
  for (i = x.minindex(); i <= m; ++i)
    norm_bigger(x[i]);
  return norm_accumulator;
}

double norm(const real_matrix & x)
{
  /*register*/ int i;
  /*register*/ int mR = x.Rmaxindex();
  /*register*/ int j;
  /*register*/ int mL = x.Lmaxindex();
  norm_accumulator = 0;
  for (j = x.Lminindex(); j <= mL; ++j)
    for (i = x.Rminindex(); i <= mR; ++i)
      norm_add(x[j][i]);
  return norm_accumulator;
}
  
double norm(const complex_matrix & x)
{
  /*register*/ int i;
  /*register*/ int mR = x.Rmaxindex();
  /*register*/ int j;
  /*register*/ int mL = x.Lmaxindex();
  norm_accumulator = 0;
  for (j = x.Lminindex(); j <= mL; ++j)
    for (i = x.Rminindex(); i <= mR; ++i)
      norm_add(x[j][i]);
  return norm_accumulator;
}

double max_norm(const real_matrix & x)
{
  /*register*/ int i;
  /*register*/ int mR = x.Rmaxindex();
  /*register*/ int j;
  /*register*/ int mL = x.Lmaxindex();
  norm_accumulator = 0;
  for (j = x.Lminindex(); j <= mL; ++j)
    for (i = x.Rminindex(); i <= mR; ++i)
      norm_bigger(x[j][i]);
  return norm_accumulator;
}
  
double max_norm(const complex_matrix & x)
{
  /*register*/ int i;
  /*register*/ int mR = x.Rmaxindex();
  /*register*/ int j;
  /*register*/ int mL = x.Lmaxindex();
  norm_accumulator = 0;
  for (j = x.Lminindex(); j <= mL; ++j)
    for (i = x.Rminindex(); i <= mR; ++i)
      norm_bigger(x[j][i]);
  return norm_accumulator;
}



// ************************************************************************
// Vector Functions: Overloaded Operators for two Vectors, scalemult(), scalediv()

real_vector operator +(const real_vector & x, const real_vector & y)
{
  // determine answer mode and size, then define it:
  v_index_mode mode = ResultModeMax(x.mode,y.mode);
  int size = ResultIndexMax(x.maxindex(),y.maxindex());
  if (mode == Index_C) ++size;  // need to fix size for Index_C only
  real_vector ans(size,mode);   // construction initializes to all 0's

  // now calculate answer:
  ans += x; ans += y;
  return ans;
}

real_vector operator -(const real_vector & x, const real_vector & y)
{
  // determine answer mode and size, then define it:
  v_index_mode mode = ResultModeMax(x.mode,y.mode);
  int size = ResultIndexMax(x.maxindex(),y.maxindex());
  if (mode == Index_C) ++size;  // need to fix size for Index_C only
  real_vector ans(size,mode);   // construction initializes to all 0's

  // now calculate answer:
  ans += x; ans -= y;
  return ans;
}

double operator *(const real_vector & x, const real_vector & y)
{
  double sum = 0;
  /*register*/ int     i = max(x.minindex(), y.minindex());
  /*register*/ int limit = min(x.maxindex(), y.maxindex());
  for ( ; i <= limit; ++i)
    sum += (x[i] * y[i]);
  return sum;
}

complex_vector operator +(const complex_vector & x, const complex_vector & y)
{
  // determine answer mode and size, then define it:
  v_index_mode mode = ResultModeMax(x.mode,y.mode);
  int size = ResultIndexMax(x.maxindex(),y.maxindex());
  if (mode == Index_C) ++size;  // need to fix size for Index_C only
  complex_vector ans(size,mode);   // construction initializes to all 0's

  // now calculate answer:
  ans += x; ans += y;
  return ans;
}

complex_vector operator -(const complex_vector & x, const complex_vector & y)
{
  // determine answer mode and size, then define it:
  v_index_mode mode = ResultModeMax(x.mode,y.mode);
  int size = ResultIndexMax(x.maxindex(),y.maxindex());
  if (mode == Index_C) ++size;  // need to fix size for Index_C only
  complex_vector ans(size,mode);   // construction initializes to all 0's

  // now calculate answer:
  ans += x; ans -= y;
  return ans;
}

Complex operator *(const complex_vector & x, const complex_vector & y)
{
  Complex sum = 0;
  /*register*/ int     i = max(x.minindex(), y.minindex());
  /*register*/ int limit = min(x.maxindex(), y.maxindex());
  for ( ; i <= limit; ++i)
    sum += (x[i] * y[i]);
  return sum;
}

complex_vector operator +(const real_vector & x, const complex_vector & y)
{
  // determine answer mode and size, then define it:
  v_index_mode mode = ResultModeMax(x.mode,y.mode);
  int size = ResultIndexMax(x.maxindex(),y.maxindex());
  if (mode == Index_C) ++size;  // need to fix size for Index_C only
  complex_vector ans(size,mode);   // construction initializes to all 0's

  // now calculate answer:
  ans += x; ans += y;
  return ans;
}

complex_vector operator -(const real_vector & x, const complex_vector & y)
{
  // determine answer mode and size, then define it:
  v_index_mode mode = ResultModeMax(x.mode,y.mode);
  int size = ResultIndexMax(x.maxindex(),y.maxindex());
  if (mode == Index_C) ++size;  // need to fix size for Index_C only
  complex_vector ans(size,mode);   // construction initializes to all 0's

  // now calculate answer:
  ans += x; ans -= y;
  return ans;
}

complex_vector operator -(const complex_vector & x, const real_vector & y)
{
  // determine answer mode and size, then define it:
  v_index_mode mode = ResultModeMax(x.mode,y.mode);
  int size = ResultIndexMax(x.maxindex(),y.maxindex());
  if (mode == Index_C) ++size;  // need to fix size for Index_C only
  complex_vector ans(size,mode);   // construction initializes to all 0's

  // now calculate answer:
  ans += x; ans -= y;
  return ans;
}

Complex operator *(const real_vector & x, const complex_vector & y)
{
  Complex sum = 0;
  /*register*/ int     i = max(x.minindex(), y.minindex());
  /*register*/ int limit = min(x.maxindex(), y.maxindex());
  for ( ; i <= limit; ++i)
    sum += (x[i] * y[i]);
  return sum;
}

real_vector scalemult(const real_vector & x, const real_vector & y)
{
  // determine answer mode and size, then define it:
  v_index_mode mode = ResultModeMax(x.mode,y.mode);
  int size = ResultIndexMax(x.maxindex(),y.maxindex());
  if (mode == Index_C) ++size;  // need to fix size for Index_C only
  real_vector ans(size,mode);   // construction initializes to all 0's

  // now calculate answer:
  for(int i = ans.minindex(); i <= ans.maxindex(); ++i)
    ans[i] = x.read(i)*y.read(i);
  return ans;
}

real_vector scalediv(const real_vector & x, const real_vector & y)
{
  // determine answer mode and size, then define it:
  v_index_mode mode = ResultModeMax(x.mode,y.mode);
  int size = ResultIndexMax(x.maxindex(),y.maxindex());
  if (mode == Index_C) ++size;  // need to fix size for Index_C only
  real_vector ans(size,mode);   // construction initializes to all 0's

  // now calculate answer:
  for(int i = ans.minindex(); i <= ans.maxindex(); ++i)
    ans[i] = x.read(i)/y.read(i);
  return ans;
}

complex_vector scalemult(const complex_vector & x, const complex_vector & y)
{
  // determine answer mode and size, then define it:
  v_index_mode mode = ResultModeMax(x.mode,y.mode);
  int size = ResultIndexMax(x.maxindex(),y.maxindex());
  if (mode == Index_C) ++size;  // need to fix size for Index_C only
  complex_vector ans(size,mode);   // construction initializes to all 0's

  // now calculate answer:
  for(int i = ans.minindex(); i <= ans.maxindex(); ++i)
    ans[i] = x.read(i)*y.read(i);
  return ans;
}

complex_vector scalediv(const complex_vector & x, const complex_vector & y)
{
  // determine answer mode and size, then define it:
  v_index_mode mode = ResultModeMax(x.mode,y.mode);
  int size = ResultIndexMax(x.maxindex(),y.maxindex());
  if (mode == Index_C) ++size;  // need to fix size for Index_C only
  complex_vector ans(size,mode);   // construction initializes to all 0's

  // now calculate answer:
  for(int i = ans.minindex(); i <= ans.maxindex(); ++i)
    ans[i] = x.read(i)/y.read(i);
  return ans;
}

complex_vector scalemult(const real_vector & x, const complex_vector & y)
{
  // determine answer mode and size, then define it:
  v_index_mode mode = ResultModeMax(x.mode,y.mode);
  int size = ResultIndexMax(x.maxindex(),y.maxindex());
  if (mode == Index_C) ++size;  // need to fix size for Index_C only
  complex_vector ans(size,mode);   // construction initializes to all 0's

  // now calculate answer:
  for(int i = ans.minindex(); i <= ans.maxindex(); ++i)
    ans[i] = x.read(i)*y.read(i);
  return ans;
}

complex_vector scalediv(const real_vector & x, const complex_vector & y)
{
  // determine answer mode and size, then define it:
  v_index_mode mode = ResultModeMax(x.mode,y.mode);
  int size = ResultIndexMax(x.maxindex(),y.maxindex());
  if (mode == Index_C) ++size;  // need to fix size for Index_C only
  complex_vector ans(size,mode);   // construction initializes to all 0's

  // now calculate answer:
  for(int i = ans.minindex(); i <= ans.maxindex(); ++i)
    ans[i] = x.read(i)/y.read(i);
  return ans;
}

complex_vector scalediv(const complex_vector & x, const real_vector & y)
{
  // determine answer mode and size, then define it:
  v_index_mode mode = ResultModeMax(x.mode,y.mode);
  int size = ResultIndexMax(x.maxindex(),y.maxindex());
  if (mode == Index_C) ++size;  // need to fix size for Index_C only
  complex_vector ans(size,mode);   // construction initializes to all 0's

  // now calculate answer:
  for(int i = ans.minindex(); i <= ans.maxindex(); ++i)
    ans[i] = x.read(i)/y.read(i);
  return ans;
}


// ************************************************************************
// Vector Functions: Overloaded Operators for Vector and Scalar

real_vector operator +(const real_vector & v, const double s)
{ real_vector ans; ans = v; ans += s; return ans; }

real_vector operator -(const real_vector & v, const double s)
{ real_vector ans; ans = v; ans -= s; return ans; }

real_vector operator *(const real_vector & v, const double s)
{ real_vector ans; ans = v; ans *= s; return ans; }

real_vector operator /(const real_vector & v, const double s)
{ real_vector ans; ans = v; ans /= s; return ans; }

complex_vector operator +(const complex_vector & v, const Complex s)
{ complex_vector ans; ans = v; ans += s; return ans; }

complex_vector operator -(const complex_vector & v, const Complex s)
{ complex_vector ans; ans = v; ans -= s; return ans; }

complex_vector operator *(const complex_vector & v, const Complex s)
{ complex_vector ans; ans = v; ans *= s; return ans; }

complex_vector operator /(const complex_vector & v, const Complex s)
{ complex_vector ans; ans = v; ans /= s; return ans; }

complex_vector operator +(const real_vector & v, const Complex s)
{ complex_vector ans; ans = v; ans += s; return ans; }

complex_vector operator -(const real_vector & v, const Complex s)
{ complex_vector ans; ans = v; ans -= s; return ans; }

complex_vector operator *(const real_vector & v, const Complex s)
{ complex_vector ans; ans = v; ans *= s; return ans; }

complex_vector operator /(const real_vector & v, const Complex s)
{ complex_vector ans; ans = v; ans /= s; return ans; }


// ************************************************************************
// Matrix Functions: Unary Operations

complex_matrix transpose(const complex_matrix &U)
{
  // Make V with transposed modes and sizes:
  complex_matrix V(0, 0, U.Rmode, U.Lmode);
  V.resize(U.Rmaxindex(), U.Lmaxindex());
  // Now copy the elements:
  for (int i = V.Lminindex(); i <= V.Lmaxindex(); ++i) {
    /*register*/ int limit = V.Rmaxindex();
    /*register*/ int j;
    for (j = V.Rminindex(); j <= limit; ++j)
      V[i][j] = U[j][i];
  }
  return V;
}

real_matrix transpose(const real_matrix &X)
{
  // Make Y with transposed modes and sizes:
  real_matrix Y(0, 0, X.Rmode, X.Lmode);
  Y.resize(X.Rmaxindex(), X.Lmaxindex());
  // Now copy the elements:
  for (int i = Y.Lminindex(); i <= Y.Lmaxindex(); ++i) {
    /*register*/ int limit = Y.Rmaxindex();
    /*register*/ int j;
    for (j = Y.Rminindex(); j <= limit; ++j)
      Y[i][j] = X[j][i];
  }
  return Y;
}


// ************************************************************************
// Matrix Functions: Overloaded Operators for two Matrices

complex_matrix operator +(const complex_matrix & B, const complex_matrix & C)
{ complex_matrix A(0); return A.add(B,C); }

complex_matrix operator +(const    real_matrix & B, const complex_matrix & C)
{ complex_matrix A(0); return A.add(B,C); }

complex_matrix operator +(const complex_matrix & B, const    real_matrix & C)
{ complex_matrix A(0); return A.add(B,C); }

real_matrix operator +(const    real_matrix & B, const    real_matrix & C)
{ real_matrix A(0); return A.add(B,C); }

complex_matrix operator -(const complex_matrix & B, const complex_matrix & C)
{ complex_matrix A(0); return A.sub(B,C); }

complex_matrix operator -(const    real_matrix & B, const complex_matrix & C)
{ complex_matrix A(0); return A.sub(B,C); }

complex_matrix operator -(const complex_matrix & B, const    real_matrix & C)
{ complex_matrix A(0); return A.sub(B,C); }

real_matrix operator -(const    real_matrix & B, const    real_matrix & C)
{ real_matrix A(0); return A.sub(B,C); }

complex_matrix operator *(const complex_matrix & B, const complex_matrix & C)
{
  // make the result look like B in the left index, C in the right index:
  complex_matrix A(0, 0, B.Lmode, C.Rmode);
  A.resize(B.Lmaxindex(), C.Rmaxindex());
  // now fill the result:
  int i, j;
  int low = max(B.Rminindex(), C.Lminindex());
  int hi  = min(B.Rmaxindex(), C.Lmaxindex());
  Complex sum;
  /*register*/ int k;
  for(i = A.Lminindex(); i <= A.Lmaxindex(); ++i)
    for(j = A.Rminindex(); j <= A.Rmaxindex(); ++j) {
      for (sum = 0, k = low; k <= hi; ++k)
	sum += B[i][k] * C[k][j];
      A[i][j] = sum;
    }
  return A;
}

complex_matrix operator *(const real_matrix & B, const complex_matrix & C)
{
  // make the result look like B in the left index, C in the right index:
  complex_matrix A(0, 0, B.Lmode, C.Rmode);
  A.resize(B.Lmaxindex(), C.Rmaxindex());
  // now fill the result:
  int i, j;
  int low = max(B.Rminindex(), C.Lminindex());
  int hi  = min(B.Rmaxindex(), C.Lmaxindex());
  Complex sum;
  /*register*/ int k;
  for(i = A.Lminindex(); i <= A.Lmaxindex(); ++i)
    for(j = A.Rminindex(); j <= A.Rmaxindex(); ++j) {
      for (sum = 0, k = low; k <= hi; ++k)
	sum += B[i][k] * C[k][j];
      A[i][j] = sum;
    }
  return A;
}

complex_matrix operator *(const complex_matrix & B, const real_matrix & C)
{
  // make the result look like B in the left index, C in the right index:
  complex_matrix A(0, 0, B.Lmode, C.Rmode);
  A.resize(B.Lmaxindex(), C.Rmaxindex());
  // now fill the result:
  int i, j;
  int low = max(B.Rminindex(), C.Lminindex());
  int hi  = min(B.Rmaxindex(), C.Lmaxindex());
  Complex sum;
  /*register*/ int k;
  for(i = A.Lminindex(); i <= A.Lmaxindex(); ++i)
    for(j = A.Rminindex(); j <= A.Rmaxindex(); ++j) {
      for (sum = 0, k = low; k <= hi; ++k)
	sum += B[i][k] * C[k][j];
      A[i][j] = sum;
    }
  return A;
}

real_matrix operator *(const real_matrix & B, const real_matrix & C)
{
  // make the result look like B in the left index, C in the right index:
  real_matrix A(0, 0, B.Lmode, C.Rmode);
  A.resize(B.Lmaxindex(), C.Rmaxindex());
  // now fill the result:
  int i, j;
  int low = max(B.Rminindex(), C.Lminindex());
  int hi  = min(B.Rmaxindex(), C.Lmaxindex());
  double sum;
  /*register*/ int k;
  for(i = A.Lminindex(); i <= A.Lmaxindex(); ++i)
    for(j = A.Rminindex(); j <= A.Rmaxindex(); ++j) {
      for (sum = 0, k = low; k <= hi; ++k)
	sum += B[i][k] * C[k][j];
      A[i][j] = sum;
    }
  return A;
}


// ************************************************************************
// Matrix Functions: Overloaded Operators for Matrix and Scalar

real_matrix operator +(const real_matrix & A, const double s)
{ real_matrix ans; ans = A; ans += s; return ans; }

real_matrix operator -(const real_matrix & A, const double s)
{ real_matrix ans; ans = A; ans -= s; return ans; }

real_matrix operator *(const real_matrix & A, const double s)
{ real_matrix ans; ans = A; ans *= s; return ans; }

real_matrix operator /(const real_matrix & A, const double s)
{ real_matrix ans; ans = A; ans /= s; return ans; }

complex_matrix operator +(const complex_matrix & A, const Complex s)
{ complex_matrix ans; ans = A; ans += s; return ans; }

complex_matrix operator -(const complex_matrix & A, const Complex s)
{ complex_matrix ans; ans = A; ans -= s; return ans; }

complex_matrix operator *(const complex_matrix & A, const Complex s)
{ complex_matrix ans; ans = A; ans *= s; return ans; }

complex_matrix operator /(const complex_matrix & A, const Complex s)
{ complex_matrix ans; ans = A; ans /= s; return ans; }

complex_matrix operator +(const real_matrix & A, const Complex s)
{ complex_matrix ans; ans = A; ans += s; return ans; }

complex_matrix operator -(const real_matrix & A, const Complex s)
{ complex_matrix ans; ans = A; ans -= s; return ans; }

complex_matrix operator *(const real_matrix & A, const Complex s)
{ complex_matrix ans; ans = A; ans *= s; return ans; }

complex_matrix operator /(const real_matrix & A, const Complex s)
{ complex_matrix ans; ans = A; ans /= s; return ans; }


// ************************************************************************
// Matrix Functions: lookup and linterp

// Refer to FR's bsearch.cc for a fully commented description of the
// algorithm; also notebook, pg 39.

int lookup(const double x, const real_matrix & A, const int row)
{
  int b, t, test;  // b,t index limits, test index in the middle

  b = A.Rminindex(); t = A.Rmaxindex();
  if (t < b) return b;  // empty matrix

  while (b + 1 < t) {
    test = (b + t) / 2;  // we know that: b < test < t here
    if ( A.read(row,test) <= x )
      b = test;
    else
      t = test;
  }

  return (A.read(row,t) <= x) ? t : b;
}

int lookup(const double x, const complex_matrix & A, const int row)
{
  int b, t, test;  // b,t index limits, test index in the middle

  b = A.Rminindex(); t = A.Rmaxindex();
  if (t < b) return b;  // empty matrix

  while (b + 1 < t) {
    test = (b + t) / 2;  // we know that: b < test < t here
    if ( A.read(row,test).real <= x )
      b = test;
    else
      t = test;
  }

  return (A.read(row,t).real <= x) ? t : b;
}

// Refer to FR notebook, pp 27-28,46 for details.

double linterp(const double x, const real_matrix & A,
	       const int n1, const int n2)
{
  int i = lookup(x, A, n1);  // lower index (maybe)

  int j = (i < A.Rmaxindex()) ? i+1 : i-1; // garbage if A empty


  double m;  //slope
  double xi = A.read(n1,i); double xj = A.read(n1,j);
  double yi = A.read(n2,i); double yj = A.read(n2,j);

  if (xi != xj) {
    m = (yi - yj)/(xi - xj);  // m = Dy/Dx
    return yi + m*(x - xi);  // an extrapolation if x outside matrix
  }
  else  //slope is vertical, method fails, return 0.0 (why not?)
    return 0.0;
}

Complex linterp(const double x, const complex_matrix & A,
	       const int n1, const int n2)
{
  int i = lookup(x, A, n1);  // lower index (maybe)

  int j = (i < A.Rmaxindex()) ? i+1 : i-1; // garbage if A empty


  Complex m;  //slope
  double  xi = A.read(n1,i).real;  double xj = A.read(n1,j).real;
  Complex yi = A.read(n2,i);       Complex yj = A.read(n2,j);

  if (xi != xj) {
    m = (yi - yj)/(xi - xj);  // m = Dy/Dx
    return yi + m*(x - xi);  // an extrapolation if x outside matrix
  }
  else  //slope is vertical, method fails, return 0.0 (why not?)
    return Complex(0.0);
}

real_vector linterp(const double x, const real_matrix & A,
	       const int n)
{
  int i = lookup(x, A, n);  // lower index (maybe)
  int j = (i < A.Rmaxindex()) ? i+1 : i-1; // garbage if A empty
  double xi = A.read(n,i); double xj = A.read(n,j);
  if(xi == xj) return real_vector(0);  // vertical slope; empty return vector
  
  double f = 1/(xi - xj);  // factor for calculating slope
  real_vector result(A.Lmode); result.resize(A.Lmaxindex());
  for(int row = result.minindex(); row <= result.maxindex(); ++row) {
    double yi = A.read(row,i); double yj = A.read(row,j);
    double m = f*(yi - yj);  // slope
    result[row] = yi + m*(x - xi);  // an extrapolation if x outside matrix
  }
  result.get(n) = x;  // the independent variable
  return result;
}

complex_vector linterp(const double x, const complex_matrix & A,
	       const int n)
{
  int i = lookup(x, A, n);  // lower index (maybe)
  int j = (i < A.Rmaxindex()) ? i+1 : i-1; // garbage if A empty
  double xi = A.read(n,i).real; double xj = A.read(n,j).real;
  if(xi == xj) return complex_vector(0);  // vertical slope; empty return vector

  double f = 1/(xi - xj);  // factor for calculating slope
  complex_vector result(A.Lmode); result.resize(A.Lmaxindex());
  for(int row = result.minindex(); row <= result.maxindex(); ++row) {
    Complex yi = A.read(row,i); Complex yj = A.read(row,j);
    Complex m = f*(yi - yj);  // slope
    result[row] = yi + m*(x - xi);  // an extrapolation if x outside matrix
  }
  result.get(n) = x;  // the independent variable (imaginary part = 0)
  return result;
}


// ************************************************************************
// Matrix Functions: scalerow

real_matrix & scalerow(const int n, real_matrix & X, const double y)
{
  if ((n < X.Lminindex())||(n > X.Lmaxindex()))
    return X;  // no action required for n out of range

  int len = length(X.Rminindex(), X.Rmaxindex());
  Ascale(&X[n][X.Rminindex()], y, len);
  return X;
}

complex_matrix & scalerow(const int n, complex_matrix & U, const Complex v)
{
  if ((n < U.Lminindex())||(n > U.Lmaxindex()))
    return U;  // no action required for n out of range

  int len = length(U.Rminindex(), U.Rmaxindex());
  Ascale(&U[n][U.Rminindex()], v, len);
  return U;
}


// ************************************************************************
// Mixed Matrix and Vector Routines: Get a Row or Column of a Matrix

real_vector row(const int n, const real_matrix & X)
{
  real_vector v(0, X.Rmode);
  v.resize(X.Rmaxindex());
  for(int i = v.minindex(); i <= v.maxindex(); ++i)
    v[i] = X.read(n,i);
  return v;
}

real_vector col(const int n, const real_matrix & X)
{
  real_vector v(0, X.Lmode);
  v.resize(X.Lmaxindex());
  for(int i = v.minindex(); i <= v.maxindex(); ++i)
    v[i] = X.read(i,n);
  return v;
}

complex_vector row(const int n, const complex_matrix & X)
{
  complex_vector v(0, X.Rmode);
  v.resize(X.Rmaxindex());
  for(int i = v.minindex(); i <= v.maxindex(); ++i)
    v[i] = X.read(n,i);
  return v;
}

complex_vector col(const int n, const complex_matrix & X)
{
  complex_vector v(0, X.Lmode);
  v.resize(X.Lmaxindex());
  for(int i = v.minindex(); i <= v.maxindex(); ++i)
    v[i] = X.read(i,n);
  return v;
}

// ************************************************************************
// Mixed Matrix and Vector Routines: Overloaded Operator *

real_vector operator *(const real_matrix & X, const real_vector & y)
{
  return col(1, X * real_matrix(y));
}

complex_vector operator *(const complex_matrix & U, const complex_vector & v)
{
  return col(1, U * complex_matrix(v));
}

complex_vector operator *(const real_matrix & X, const complex_vector & v)
{
  return col(1, X * complex_matrix(v));
}

complex_vector operator *(const complex_matrix & U, const real_vector & y)
{
  return col(1, U * real_matrix(y));
}


// ************************************************************************
// The Matrix Solver, and helper routines:

// Real Version:

// Solves Ax == b and returns x.  If it's unable to solve the system,
// then x will be empty.
real_vector solve(const real_matrix & A, const real_vector & b)
{ 
  return col(1, solve(A, real_matrix(b)));
}

// checks sizes of A and B in AX == B for solvability,
// and puts lengths of X into n and m
static int sizecheck(
		     const real_matrix & A, // The coefficient matrix
		     const real_matrix & B, // The RHS matrix
		     int & n,                 // The column length of X
		     int & m                  // The row    length of X
		     );  // Returns 0 if not solvable, 1 if solvable

// copies A and B into the previously-sized C so that C is A|B.
// C must be properly sized and have Index_C in both dimensions
static void augment(real_matrix & C,
		    const real_matrix & A,
		    const real_matrix & B
		    );

// scale the rows of C if necessary in preparation for solving
static void scale(real_matrix & C);

// performs gaussian elimination of the augmented matrix C, with
// partial pivoting. Does not back-substitute. Returns 0 if C
// represents a singular system, otherwise returns 1
static int triangle(real_matrix & C);

// performs back-substitution in the solvable system C, writing the
// results into the former locations of the RHS elements
static void backsub(real_matrix & C);

// transcribes the results in C into the proper locations in X
static void copyresult(real_matrix & X, const real_matrix & C);

// pivot row i of C with the rows following it. Returns 0 if a pivot
// != 0 cannot be found (so C is singular), else returns 1.
// (used by triangle())
static int pivot(int i, real_matrix & C);
 
// ------------------------------------------------------------------------
// the solver definitions:

// Solves AX == B and returns X.  If it's unable to solve the system,
// then X will be empty.
real_matrix solve(const real_matrix & A, const real_matrix & B)
{
  real_matrix X(0,0,Index_1,Index_1);  // the solution (empty for now)
  int n, m;  // the column length and row length of X

  // Calculate n and m and check compatibility of A and B:
  if ( ! sizecheck(A, B, n, m) )
    // sizecheck returns 0 if the geometries of A and B aren't o.k.
    return X;  // failure: returns the empty X

  // Passed sizecheck(). Make the augmented matrix C from A and B:
  real_matrix C(n, n+m, Index_C, Index_C);
  augment(C, A, B);
  scale(C);  // individually scales each row to improve solver accuracy

  // Perform the Gaussian Elimination, with X values replacing B in C:
  if ( ! triangle(C))
    // triangle() returns 0 if A is singular, so system can't be solved
    return X;  // failure: returns the empty X
  backsub(C);
  
  // Now resize X and copy the results into it, then return:
  X.reallocate(A.Rsize, B.Rsize, A.Rmode, B.Rmode);
  X.Lmaxindex(A.Rmaxindex());
  X.Rmaxindex(B.Rmaxindex());
  copyresult(X, C);
  return X;
}

// Uses solve() to find the inverse of a matrix. If the inverse doesn't
// exist, then the returned matrix will be empty. Note: the index modes and
// valid index ranges of A must be the same in both dimensions for an
// inverse to exist (A is square).
real_matrix inverse(const real_matrix & A)
{
  real_matrix B(0);   // will become the RHS for solve()

  if ((A.Rmode == A.Lmode)&&(A.Rmaxindex() == A.Lmaxindex())) {
    // then A may be invertible, so set up the RHS for solve()
    B.resize(A);
    B.diagonal(1.0);
  }
  // if the above if failed, then the RHS is empty, and so will be the
  // returned matrix from solve()
  return solve(A,B);
}

// checks sizes of A and B in AX == B for solvability,
// and puts lengths of X into n and m
static int sizecheck(
		     const real_matrix & A, // The coefficient matrix
		     const real_matrix & B, // The RHS matrix
		     int & n,                 // The column length of X
		     int & m                  // The row    length of X
		     )   // Returns 0 if not solvable, 1 if solvable
{
  n = A.Rmaxindex() - A.Rminindex();
  m = B.Rmaxindex() - B.Rminindex();

  if((A.Lmaxindex() - A.Lminindex()) != n)
    // Oops! then A is not square
    return 0;

  else if((n < 0)||(m < 0))
    // Oops! then either A or B is empty
    return 0;

  else if ((B.Lminindex() != A.Lminindex())||(B.Lmaxindex() != A.Lmaxindex()))
    // Oops! then A and B are not compatible
    return 0;

  else {
    // all tests pass; so far so good
    n += 1; m += 1; // so they are proper lengths, rather than differences
    return 1;
  }
}

// copies A and B into the previously-sized C so that C is A|B.
// C must be properly sized and have Index_C in both dimensions
static void augment(real_matrix & C,
		    const real_matrix & A,
		    const real_matrix & B
		    )
{
  int n = C.Lsize;
  int m = C.Rsize - n;
  int ALmin = A.Lminindex();
  int ARmin = A.Rminindex();
  int BLmin = ALmin;
  int BRmin = B.Rminindex();

  for (int i = 0; i < n; ++i) {
    Acopy(C[i],   A[i+ALmin]+ARmin, n);
    Acopy(C[i]+n, B[i+BLmin]+BRmin, m);
  }
}

// scale the rows of C if necessary in preparation for solving.
// right now (11/16) scale() always scales each row of C by the sum
// of the magnitude squareds of the elements of A in that row.
static void scale(real_matrix & C)
{
  int n = C.Lsize;
  int m = C.Rsize;
  int i,j;
  double norm;
  double *d;

  for(i = 0; i < n; ++i) {
    d = C[i];
    for(j = 0, norm = 0; j < n; ++j) norm += ::norm(d[j]);
    if (norm > 0) Ascale(d, 1/norm, m);
  }
}

// performs gaussian elimination of the augmented matrix C, with
// partial pivoting. Does not back-substitute. Returns 0 if C
// represents a singular system, otherwise returns 1
static int triangle(real_matrix & C)
{
  int n = C.Lsize;  // number of equations
  int m = C.Rsize;  // number of variables + number of RHS's
  int i, j, len;    // i: current equation; j: indexes eqn's below i
  double *d, *f;    // *d: diagonal element in eqn i; *f: elements below it

  for (i = 0; i < n; ++i) {
    if ( ! pivot(i,C)) return 0; // pivot; return 0 if C is singular
    len = m - i - 1;          // count of elements to be manipulated
    d = C[i]+i;          // points to C[i][i]
    Ascale(d+1, 1/(*d), len); // put a 1 on the diagonal at [i][i]
    for (j = i+1; j < n; ++j) {
      f = C[j]+i;        // points to C[j][i]
      if ( (*f) != 0.0 )      // don't bother if it's already a 0
	Ascalesub(f+1, d+1, (*f), len);  // eliminate C[j][i]
    }
  }

  return 1;
}

// pivot row i of C with the rows following it. Returns 0 if a pivot
// != 0 cannot be found (so C is singular), else returns 1.
// (used by triangle())
static int pivot(int i, real_matrix & C)
{
  int n = C.Lsize;
  double val, test;  // val: largest norm seen so far in column i
  int j, pivot;      // j: index; pivot: row # with the largest val

  for (j = i, val = 0, pivot = -1; j < n; j++) {
    test = norm(C[j][i]);
    if (test > val) {
      val = test; pivot = j;
    }
  }
  
  if (pivot == -1)
    return 0;  // no nonzero pivots, so C is singular
  else if (pivot != i) {
    C.rowswap(i,pivot);
    return 1;
  }
  else return 1;
}

// performs back-substitution in the solvable system C, writing the
// results into the former locations of the RHS elements
static void backsub(real_matrix & C)
{
  int n = C.Lsize;      // size of coefficient matrix
  int m = C.Rsize - n;  // number of RHS vectors
  int i, j;             // i: current equation; j: following equations
  double *xi, *xj;      // xi: RHS's of eqn i; xj: RHS's of eqn j

  for (i = n-2; i >= 0; --i) {
    xi = C[i]+n;
    for(j = i+1; j < n; ++j) {
      xj = C[j]+n;
      Ascalesub(xi, xj, C[i][j], m);
    }
  }
}

// transcribes the results in C into the proper locations in X
static void copyresult(real_matrix & X, const real_matrix & C)
{
  int n = C.Lsize;            // number of rows in X, and offset in C
  int m = C.Rsize - n;        // number of columns in X
  int XLmin = X.Lminindex();  // offset from 0 to first row index of X
  int XRmin = X.Rminindex();  // offset from 0 to first column index of X

  for (int i = 0; i < n; ++i)
    Acopy(X[i+XLmin]+XRmin, C[i]+n, m);
}

// ========================================================================
// Complex Version:

// Solves Ax == b and returns x.  If it's unable to solve the system,
// then x will be empty.
complex_vector solve(const complex_matrix & A, const complex_vector & b)
{ 
  return col(1, solve(A, complex_matrix(b)));
}

// checks sizes of A and B in AX == B for solvability,
// and puts lengths of X into n and m
static int sizecheck(
		     const complex_matrix & A, // The coefficient matrix
		     const complex_matrix & B, // The RHS matrix
		     int & n,                 // The column length of X
		     int & m                  // The row    length of X
		     );  // Returns 0 if not solvable, 1 if solvable

// copies A and B into the previously-sized C so that C is A|B.
// C must be properly sized and have Index_C in both dimensions
static void augment(complex_matrix & C,
		    const complex_matrix & A,
		    const complex_matrix & B
		    );

// scale the rows of C if necessary in preparation for solving
static void scale(complex_matrix & C);

// performs gaussian elimination of the augmented matrix C, with
// partial pivoting. Does not back-substitute. Returns 0 if C
// represents a singular system, otherwise returns 1
static int triangle(complex_matrix & C);

// performs back-substitution in the solvable system C, writing the
// results into the former locations of the RHS elements
static void backsub(complex_matrix & C);

// transcribes the results in C into the proper locations in X
static void copyresult(complex_matrix & X, const complex_matrix & C);

// pivot row i of C with the rows following it. Returns 0 if a pivot
// != 0 cannot be found (so C is singular), else returns 1.
// (used by triangle())
static int pivot(int i, complex_matrix & C);
 
// ------------------------------------------------------------------------
// the solver definitions:

// Solves AX == B and returns X.  If it's unable to solve the system,
// then X will be empty.
complex_matrix solve(const complex_matrix & A, const complex_matrix & B)
{
  complex_matrix X(0,0,Index_1,Index_1);  // the solution (empty for now)
  int n, m;  // the column length and row length of X

  // Calculate n and m and check compatibility of A and B:
  if ( ! sizecheck(A, B, n, m) )
    // sizecheck returns 0 if the geometries of A and B aren't o.k.
    return X;  // failure: returns the empty X

  // Passed sizecheck(). Make the augmented matrix C from A and B:
  complex_matrix C(n, n+m, Index_C, Index_C);
  augment(C, A, B);
  scale(C);  // individually scales each row to improve solver accuracy

  // Perform the Gaussian Elimination, with X values replacing B in C:
  if ( ! triangle(C))
    // triangle() returns 0 if A is singular, so system can't be solved
    return X;  // failure: returns the empty X
  backsub(C);
  
  // Now resize X and copy the results into it, then return:
  X.reallocate(A.Rsize, B.Rsize, A.Rmode, B.Rmode);
  X.Lmaxindex(A.Rmaxindex());
  X.Rmaxindex(B.Rmaxindex());
  copyresult(X, C);
  return X;
}

// Uses solve() to find the inverse of a matrix. If the inverse doesn't
// exist, then the returned matrix will be empty. Note: the index modes and
// valid index ranges of A must be the same in both dimensions for an
// inverse to exist (A is square).
complex_matrix inverse(const complex_matrix & A)
{
  complex_matrix B(0);   // will become the RHS for solve()

  if ((A.Rmode == A.Lmode)&&(A.Rmaxindex() == A.Lmaxindex())) {
    // then A may be invertible, so set up the RHS for solve()
    B.resize(A);
    B.diagonal(1.0);
  }
  // if the above if failed, then the RHS is empty, and so will be the
  // returned matrix from solve()
  return solve(A,B);
}

// checks sizes of A and B in AX == B for solvability,
// and puts lengths of X into n and m
static int sizecheck(
		     const complex_matrix & A, // The coefficient matrix
		     const complex_matrix & B, // The RHS matrix
		     int & n,                 // The column length of X
		     int & m                  // The row    length of X
		     )   // Returns 0 if not solvable, 1 if solvable
{
  n = A.Rmaxindex() - A.Rminindex();
  m = B.Rmaxindex() - B.Rminindex();

  if((A.Lmaxindex() - A.Lminindex()) != n)
    // Oops! then A is not square
    return 0;

  else if((n < 0)||(m < 0))
    // Oops! then either A or B is empty
    return 0;

  else if ((B.Lminindex() != A.Lminindex())||(B.Lmaxindex() != A.Lmaxindex()))
    // Oops! then A and B are not compatible
    return 0;

  else {
    // all tests pass; so far so good
    n += 1; m += 1; // so they are proper lengths, rather than differences
    return 1;
  }
}

// copies A and B into the previously-sized C so that C is A|B.
// C must be properly sized and have Index_C in both dimensions
static void augment(complex_matrix & C,
		    const complex_matrix & A,
		    const complex_matrix & B
		    )
{
  int n = C.Lsize;
  int m = C.Rsize - n;
  int ALmin = A.Lminindex();
  int ARmin = A.Rminindex();
  int BLmin = ALmin;
  int BRmin = B.Rminindex();

  for (int i = 0; i < n; ++i) {
    Acopy(C[i],   A[i+ALmin]+ARmin, n);
    Acopy(C[i]+n, B[i+BLmin]+BRmin, m);
  }
}

// scale the rows of C if necessary in preparation for solving.
// right now (11/16) scale() always scales each row of C by the sum
// of the magnitude squareds of the elements of A in that row.
static void scale(complex_matrix & C)
{
  int n = C.Lsize;
  int m = C.Rsize;
  int i,j;
  double norm;
  Complex *d;

  for(i = 0; i < n; ++i) {
    d = C[i];
    for(j = 0, norm = 0; j < n; ++j) norm += ::norm(d[j]);
    if (norm > 0) Ascale(d, 1/norm, m);
  }
}

// performs gaussian elimination of the augmented matrix C, with
// partial pivoting. Does not back-substitute. Returns 0 if C
// represents a singular system, otherwise returns 1
static int triangle(complex_matrix & C)
{
  int n = C.Lsize;  // number of equations
  int m = C.Rsize;  // number of variables + number of RHS's
  int i, j, len;    // i: current equation; j: indexes eqn's below i
  Complex *d, *f;   // *d: diagonal element in eqn i; *f: elements below it

  for (i = 0; i < n; ++i) {
    if ( ! pivot(i,C)) return 0; // pivot; return 0 if C is singular
    len = m - i - 1;          // count of elements to be manipulated
    d = C[i]+i;          // points to C[i][i]
    Ascale(d+1, 1/(*d), len); // put a 1 on the diagonal at [i][i]
    for (j = i+1; j < n; ++j) {
      f = C[j]+i;        // points to C[j][i]
      if ( (*f) != 0.0 )      // don't bother if it's already a 0
	Ascalesub(f+1, d+1, (*f), len);  // eliminate C[j][i]
    }
  }

  return 1;
}

// pivot row i of C with the rows following it. Returns 0 if a pivot
// != 0 cannot be found (so C is singular), else returns 1.
// (used by triangle())
static int pivot(int i, complex_matrix & C)
{
  int n = C.Lsize;
  double val, test;  // val: largest norm seen so far in column i
  int j, pivot;      // j: index; pivot: row # with the largest val

  for (j = i, val = 0, pivot = -1; j < n; j++) {
    test = norm(C[j][i]);
    if (test > val) {
      val = test; pivot = j;
    }
  }
  
  if (pivot == -1)
    return 0;  // no nonzero pivots, so C is singular
  else if (pivot != i) {
    C.rowswap(i,pivot);
    return 1;
  }
  else return 1;
}

// performs back-substitution in the solvable system C, writing the
// results into the former locations of the RHS elements
static void backsub(complex_matrix & C)
{
  int n = C.Lsize;      // size of coefficient matrix
  int m = C.Rsize - n;  // number of RHS vectors
  int i, j;             // i: current equation; j: following equations
  Complex *xi, *xj;     // xi: RHS's of eqn i; xj: RHS's of eqn j

  for (i = n-2; i >= 0; --i) {
    xi = C[i]+n;
    for(j = i+1; j < n; ++j) {
      xj = C[j]+n;
      Ascalesub(xi, xj, C[i][j], m);
    }
  }
}

// transcribes the results in C into the proper locations in X
static void copyresult(complex_matrix & X, const complex_matrix & C)
{
  int n = C.Lsize;            // number of rows in X, and offset in C
  int m = C.Rsize - n;        // number of columns in X
  int XLmin = X.Lminindex();  // offset from 0 to first row index of X
  int XRmin = X.Rminindex();  // offset from 0 to first column index of X

  for (int i = 0; i < n; ++i)
    Acopy(X[i+XLmin]+XRmin, C[i]+n, m);
}

// ************************************************************************
// Special Fast Square Matrix Operations:

void IplusM(Matrix & U, double s, const Matrix & V)
{
  /*register*/ int i,j;
  /*register*/ int max = V.Lmaxindex();
  for (i = V.Lminindex(); i <= max; ++i) {
    U[i][i] = s + V[i][i];
    for (j = i+1; j <= max; ++j) {
      U[i][j] = V[i][j];
      U[j][i] = V[j][i];
    }
  }
}

void IminusM(Matrix & U, double s, const Matrix & V)
{
  /*register*/ int i,j;
  /*register*/ int max = V.Lmaxindex();
  for (i = V.Lminindex(); i <= max; ++i) {
    U[i][i] = s - V[i][i];
    for (j = i+1; j <= max; ++j) {
      U[i][j] = -V[i][j];
      U[j][i] = -V[j][i];
    }
  }
}

void MMdagger(Matrix & U, const Matrix & V)
{
  int min = V.Lminindex(), max = V.Lmaxindex();
  int len = max - min + 1; len = (len < 0) ? 0 : len;   // length of a row
  const complex *ai, *aj;
  for (int i = min; i <= max; ++i) {
    ai = & V[i][min];
    U[i][i] = Adot(ai,ai,len);
    for (int j = i + 1; j <= max; ++j) {
      aj = & V[j][min];
      U[j][i] = conj( U[i][j] = Adot(aj,ai,len) );
    }
  }
}
  
void MAMdagger(Matrix & U, const Matrix & V, const Matrix & W)
{
  int i, j, k;
  int min = V.Lminindex(), max = V.Lmaxindex();
  int len = max - min + 1; len = (len < 0) ? 0 : len;   // length of a row
  const complex *pVi, *pVj;
  complex sum;
  for (i = min; i <= max; ++i) {
    pVi = & V[i][min];
    for (j = min; j <= max; ++j) {
      pVj = (& V[j][min]) - 1;
      sum = 0.0;
      for (k = min; k <= max; ++k)
	sum += (*(++pVj))*Adot(pVi, &W[k][min], len);
      U[j][i] = sum;
    }
  }
}
