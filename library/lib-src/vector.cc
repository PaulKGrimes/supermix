// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
//
// vector.cc

#include "vector.h"
#include <iostream>
#include <iomanip>

using namespace std;

// Useful functions:

static inline int min(int a, int b)  { return (a < b)? a : b; }
static inline int max(int a, int b)  { return (a > b)? a : b; }
static inline int length(int a, int b) 
{ int n = b - a + 1; return (n < 0)? 0 : n; }

// i/o formatting static variable

static string out_separator_(" ");


// ********************************************************************
// real_vector definitions:


// vector output:

ostream & operator << (ostream & s, const real_vector & v)
{
  std::streamsize w = s.width();
  if (v.minindex() <= v.maxindex())  s << v[v.minindex()]; // v not empty
  for (int i = v.minindex() + 1; i <= v.maxindex(); ++i)
    s << out_separator_ << setw(w) << v[i];
  return s << setw(0);  // just in case no output was generated
}

real_vector & real_vector::show(ostream & s)
{
  s << endl << *this << endl;
  return *this;
}

real_vector & real_vector::show()
{
  return show(cout);
}

const real_vector & real_vector::show(ostream & s) const
{
  s << endl << *this << endl;
  return *this;
}

const real_vector & real_vector::show() const
{
  return show(cout);
}

string real_vector::out_separator()
{ return out_separator_; }

string real_vector::out_separator(const string & s)
{ string temp = out_separator_; out_separator_ = s; return temp; }

string real_vector::out_separator(const char * const s)
{ string temp = out_separator_; out_separator_ = s; return temp; }

string real_vector::out_separator(const char s)
{ string temp = out_separator_; out_separator_ = s; return temp; }

//-------------------------------------------------------------------------
// construct() do the real work of vector construction: 

void real_vector::construct(const int n, const v_index_mode t)
{
  internal_size = (n >= 0) ? n : 0;
  internal_mode = t;
  switch(t) {
  default:
  case Index_C: {
    if (internal_size) {    // != 0: must allocate memory
      data = delete_pointer = new double[internal_size];
      if (delete_pointer == 0) internal_size = 0; // out of memory
    }
    else {    // must not allocate memory
      data = delete_pointer = 0;
    }
    minindexvalue = 0; maxindexvalue = internal_size - 1;
    break;
  }
  case Index_1: {
    if (internal_size) {    // != 0: must allocate memory
      data = delete_pointer = new double[internal_size];
      if (delete_pointer == 0)
	internal_size = 0; // out of memory
      else
	--data;   // so data[1] is the first element
    }
    else {    // must not allocate memory
      data = delete_pointer = 0;
    }
    minindexvalue = 1; maxindexvalue = internal_size;
    break;
  }
  case Index_S: {
    // must always try to allocate memory, even if size == 0:
    delete_pointer = new double[2*internal_size + 1];
    if (delete_pointer == 0) {
      data = & trash; // out of memory, so send requests here
      internal_size = 0;
      maxindexvalue = -1;  // this is the "out of memory" flag
    }
    else {  // memory allocation succeeded
      data = delete_pointer + internal_size; //location of v[0]
      maxindexvalue = internal_size;
    }
    minindexvalue = -maxindexvalue;
    break;
  }
  } // switch
} // construct(const int n, const v_index_mode t = Index_C)

//---------------------------------------------------------------------
// constfill() fill the vector with a constant:

void real_vector::constfill(const double f)
{ for (int i = minindex(); i <= maxindex(); ++i) data[i] = f; }
 

//---------------------------------------------------------------------
// copy constructor:

real_vector::real_vector(const real_vector & v1)
  : size(internal_size), mode(internal_mode)
{
  construct(v1.size, v1.mode);
  // the following loop will work even if alloc for v1 failed and
  // v1.mode == Index_S.  In this case, the contents of trash will be
  // copied.
  for (int i = minindex(); i <= maxindex(); ++i)
    data[i] = v1[i];
  if(v1.is_empty())
    make_empty();
  else
    maxindex(v1.maxindex());
}

//---------------------------------------------------------------------
// alias constructor:

real_vector::real_vector(double *const ptr, const int n,
			 const v_index_mode t)
  : size(internal_size), mode(internal_mode)
{
  data = ptr;
  delete_pointer = 0; // don't destroy memory on destruction
  internal_mode = t;
  if (n >= 0) {
    // n is a useful value (could hold 1 element if t = Index_S)
    internal_size = n;
    maxindexvalue = (mode == Index_C) ? (size - 1) : size;
  }
  else {
    // n isn't useful; no memory available at ptr
    internal_size = 0;
    maxindexvalue = (mode == Index_1) ? 0 : -1;
  }
  switch (mode) {  // set minindexvalue
  case Index_C: { minindexvalue = 0; break; }
  case Index_1: { minindexvalue = 1; break; }
  case Index_S: { minindexvalue = -maxindexvalue; break; }
  }
}

//-------------------------------------------------------------------------
// reindex()  change the index modes

real_vector & real_vector::reindex(const v_index_mode M)
{
  // first check for do nothings: new mode same as old, or
  // vector is an alias
  if (M == mode) return *this;
  if (delete_pointer == 0 && data != 0 && data != &trash) return *this;

  // If here, reindexing necessary

  // calculate number of data elements
  int n = (mode == Index_S) ? 2*size + 1 : size;
  
  // new Index_S needs an odd number of elements
  if (M == Index_S && (n>>1)<<1 == n) {  // n even
    reallocate(size+1,mode);
    n = size; // the new size
  }

  // now handle the no allocated data case:
  if (delete_pointer == 0) { reallocate(0,M); return *this; }

  // set up new indexing:
  int ptr_offset; // offset into array for data[0]
  switch (M) {
  default:
    { return *this; } // unknown mode, so do nothing
  case Index_C:
    { ptr_offset = 0; break; }
  case Index_1:
    { ptr_offset = -1; break; }
  case Index_S:
    { ptr_offset = (n - 1)/2; break; }
  } // switch
  internal_size = (M == Index_S) ? ptr_offset : n;
  minindexvalue = -ptr_offset; 
  maxindexvalue = minindexvalue + n - 1;
  data = delete_pointer + ptr_offset;
  internal_mode = M;

  return *this;
}

//---------------------------------------------------------------------
// change maxindex(), make_empty():

int real_vector::maxindex(int n)
{
  if (size == 0 && mode != Index_S)
    // can't change the maxindex if no data elements
    return maxindex();
  else {
    switch (mode) {
    case Index_1: {
      if (n < 1) n = 1;
      maxindexvalue = (n > size) ? size : n;
      break;
    }
    case Index_S: {
      if (n < 0) n = 0;
      maxindexvalue = (n > size) ? size : n;
      minindexvalue = -maxindexvalue;
      break;
    }
    case Index_C: {
      if (n < 0) n = 0;
      maxindexvalue = (n >= size) ? (size - 1) : n;
      break;
    }
    default:
      ; // do nothing
    } //switch
    return maxindexvalue;
  } //else
}

real_vector & real_vector::make_empty()
{
  switch (mode) {
  case Index_C:
    maxindexvalue = -1;
    minindexvalue = 0;
    break;

  case Index_1:
    maxindexvalue = 0;
    minindexvalue = 1;
    break;

  case Index_S:
    maxindexvalue = -1;
    minindexvalue = 1;
    break;

  }
  
  return *this;
}

//---------------------------------------------------------------------
// clean up unused elements:

real_vector & real_vector::clean()
{
  int savemax = maxindex();
  int savemin = minindex();
  int i;
  maxindex(size); // make all memory visible
  for (i = savemax + 1; i <= maxindex(); ++i) data[i] = 0;
  for (i = savemin - 1; i >= minindex(); --i) data[i] = 0;
  maxindex(savemax);
  return *this;
}

//---------------------------------------------------------------------
// shrink index range to ignore tiny values:

real_vector & real_vector::shrink(double s)
{
  int i;       // the index
  double sum;  // accumulator of the sum of the magnitude squared's
  s *= s;      // magnitude squared for s: our tolerance value

  if (mode != Index_S)
    // Index_C and Index_1: just compare the sum at the positive indexes
    for (i = maxindexvalue, sum = 0; i > 0; --i) {
      sum += data[i]*data[i];
      if (sum >= s) break;  // found the limit
    }
  else {
    // Index_S must look at sum for negative indexes, too
    double nsum;  // accumulate negative index sum
    for (i = maxindexvalue, nsum = sum = 0; i > 0; --i) {
      sum += data[i]*data[i];
      nsum += data[-i]*data[-i];
      if ((sum >= s)||(nsum >= s)) break;
    }
  }

  maxindex(i);  // set the new index and return
  return *this;
}

//---------------------------------------------------------------------
// fill all elements, regardless of data subset control:

real_vector & real_vector::fillall(const double s)
{
  int savemax = maxindexvalue;
  int savemin = minindexvalue;
  maximize();
  constfill(s);
  maxindexvalue = savemax; minindexvalue = savemin;
  return *this;
}

//---------------------------------------------------------------------
// copy data from another vector:

real_vector & real_vector::copy(const real_vector & v1)
{
  // check for self copy: "v.copy(v)", and do something intelligent:
  if (data == v1.data) { clean(); return *this; }

  // otherwise, not a self copy, so:
  maxindex(size);  constfill(0);  // maximize the index range and clear
  // now set data subset on the result:
  if(v1.is_empty())
    make_empty();
  else {
    maxindex(v1.maxindex());
    // copy over the index range intersection:
    int minimum = max(minindexvalue, v1.minindex());
    int maximum = min(maxindexvalue, v1.maxindex());
    for (/*register*/ int i = minimum; i <= maximum; ++i)
      data[i] = v1[i];
  }
  return *this;
}

//---------------------------------------------------------------------
// reallocate memory:

real_vector & real_vector::reallocate(const int n, const v_index_mode t)
{
  // just return if reallocation is unnecessary:
  if ((size == n)&&(mode == t)) return *this;

  // if we're here, we have to reallocate:
  real_vector old(data, size, mode); // alias the current vector
  double * save_delete_pointer = delete_pointer;
  int newmax;  // will hold the new maxindexvalue
  newmax = (maxindexvalue < old.maxindex()) ? maxindexvalue : n ;

  construct(n,t);
  copy(old);
  maxindex(newmax);
  delete [] save_delete_pointer;
  return *this;
}

//---------------------------------------------------------------------
// resize: the ones which couldn't be defined in the header file

real_vector & real_vector::resize(const complex_vector & v)
{
  int Size = (v.mode == Index_C) ? v.maxindex()+1: v.maxindex();
  return reallocate(Size, v.mode);
}

//---------------------------------------------------------------------
// basic assignment operator:

real_vector & real_vector::operator = (const real_vector & v1)
{
  // check for self assignment: "v = v", and do something intelligent:
  if (data == v1.data) { clean(); return *this; }

  // otherwise, not a self assignment, so:
  maximize();
  if ((mode != v1.mode)||(maxindexvalue < v1.maxindex())) {
    // make the vector compatible with v1's data
    delete [] delete_pointer;
    int newsize = (v1.mode == Index_C) ? v1.maxindex()+1: v1.maxindex();
    construct(newsize,v1.mode);
  }
  copy(v1);
  return *this;
}

//---------------------------------------------------------------------
// other assignment operators (no memory reallocation for these):

real_vector & real_vector::operator += (const double s)
{
  // make local pointers to the first and last elements:
  /*register*/ double * d = data + minindexvalue;
  /*register*/ double * limit = data + maxindexvalue;
  // use the pointer to the first element as the loop variable:
  while (d <= limit) *(d++) += s;
  return *this;
}

real_vector & real_vector::operator -= (const double s)
{
  /*register*/ double * d = data + minindexvalue;
  /*register*/ double * limit = data + maxindexvalue;
  while (d <= limit) *(d++) -= s;
  return *this;
}

real_vector & real_vector::operator *= (const double s)
{
  /*register*/ double * d = data + minindexvalue;
  /*register*/ double * limit = data + maxindexvalue;
  while (d <= limit) *(d++) *= s;
  return *this;
}

real_vector & real_vector::operator /= (const double s)
{
  /*register*/ double * d = data + minindexvalue;
  /*register*/ double * limit = data + maxindexvalue;
  while (d <= limit) *(d++) /= s;
  return *this;
}

real_vector & real_vector::operator += (const real_vector & v1)
{
  clean();  // ensure nonvalid elements are set to 0
  // increase the target's valid range if necessary:
  if (maxindexvalue < v1.maxindex()) maxindex(v1.maxindex());
  // loop over the intersection of the vectors' valid ranges:
  int minimum = max(minindexvalue, v1.minindex());
  int maximum = min(maxindexvalue, v1.maxindex());
  for (/*register*/ int i = minimum; i <= maximum; ++i)
    data[i] += v1[i];
  return *this;
}

real_vector & real_vector::operator -= (const real_vector & v1)
{
  clean();
  if (maxindexvalue < v1.maxindex()) maxindex(v1.maxindex());
  int minimum = max(minindexvalue, v1.minindex());
  int maximum = min(maxindexvalue, v1.maxindex());
  for (/*register*/ int i = minimum; i <= maximum; ++i)
    data[i] -= v1[i];
  return *this;
}

//---------------------------------------------------------------------
// other miscellaneous funtions:

real_vector & real_vector::real(const complex_vector & v)
{
  resize(v);
  if(v.is_empty())
    make_empty();
  else {
    maxindex(v.maxindex());
    int i;
    for (i = minindex(); i <= maxindex(); ++i)
      data[i] = v[i].real;
  }
  return *this;
}

real_vector & real_vector::imaginary(const complex_vector & v)
{
  resize(v);
  if(v.is_empty())
    make_empty();
  else {
    maxindex(v.maxindex());
    int i;
    for (i = minindex(); i <= maxindex(); ++i)
      data[i] = v[i].imaginary;
  }
  return *this;
}

int real_vector::findmax() const
{
  if(maxindexvalue < minindexvalue) return 0;
  double norm;                // magnitude squared
  double max;                 // max norm so far 
  int index = minindexvalue;  // index to return

  norm = data[minindexvalue]; max = norm * norm;  // initialize
  for (int i = minindexvalue + 1; i <= maxindexvalue; ++i) {
    norm = data[i]; norm *= norm;
    if ( norm > max ) {
      max = norm; index = i;
    }
  }
  return index;
}

real_vector & real_vector::apply(double (*f)(double))
{
  // make local pointers to the first and last elements:
  /*register*/ double * d = data + minindexvalue;
  /*register*/ double * limit = data + maxindexvalue;
  // use the pointer to the first element as the loop variable:
  for ( ; d <= limit; ++d) *d = f(*d);
  return *this;
}

real_vector & real_vector::apply(double (*f)(const double &))
{
  // make local pointers to the first and last elements:
  /*register*/ double * d = data + minindexvalue;
  /*register*/ double * limit = data + maxindexvalue;
  // use the pointer to the first element as the loop variable:
  for ( ; d <= limit; ++d) *d = f(*d);
  return *this;
}

real_vector & real_vector::apply(double (*f)(double, int))
{
  /*register*/ int i = minindexvalue; 
  /*register*/ double * d = data;
  /*register*/ int limit = maxindexvalue;
  for ( d += i; i <= limit; ++i, ++d) *d = f(*d, i);
  return *this;
}

real_vector & real_vector::apply(double (*f)(const double &, int))
{
  /*register*/ int i = minindexvalue; 
  /*register*/ double * d = data;
  /*register*/ int limit = maxindexvalue;
  for ( d += i; i <= limit; ++i, ++d) *d = f(*d, i);
  return *this;
}

//---------------------------------------------------------------------
// STL compatibility:

void real_vector::reserve(int n)
{
  if(n <= size) return;
  bool e = empty(); int m = maxindexvalue;
  reallocate(n);
  if (e)
    make_empty();
  else
    maxindex(m);
}

void real_vector::push_back(double s)
{
  // adjust memory allocation, if necessary
  int i = maxindex()+1;
  if (maxindex(i) != i) {
    // then incrementing maxindex didn't work; must reallocate
    reallocate(2*(size+1));  // grow size by a factor of 2
    maxindex(i);
  }
  data[i] = s;
}

void real_vector::pop_back()
{
  if(empty()) return;
  int i = maxindex(maxindex()-1);
  if (maxindex() == i) {
    // then attempting to decrement maxindex didn't work;
    // vector is now empty
    make_empty();
  }
}

// end of real_vector definitions

// ********************************************************************
// complex_vector definitions:


// vector output:

ostream & operator << (ostream & s, const complex_vector & v)
{
  std::streamsize w = s.width();
  if (v.minindex() <= v.maxindex())  s << v[v.minindex()]; // v not empty
  for (int i = v.minindex() + 1; i <= v.maxindex(); ++i)
    s << out_separator_ << setw(w) << v[i];
  return s << setw(0);  // just in case no output was generated
}

complex_vector & complex_vector::show(ostream & s)
{
  s << endl << *this << endl;
  return *this;
}

complex_vector & complex_vector::show()
{
  return show(cout);
}

const complex_vector & complex_vector::show(ostream & s) const
{
  s << endl << *this << endl;
  return *this;
}

const complex_vector & complex_vector::show() const
{
  return show(cout);
}

string complex_vector::out_separator()
{ return out_separator_; }

string complex_vector::out_separator(const string & s)
{ string temp = out_separator_; out_separator_ = s; return temp; }

string complex_vector::out_separator(const char * const s)
{ string temp = out_separator_; out_separator_ = s; return temp; }

string complex_vector::out_separator(const char s)
{ string temp = out_separator_; out_separator_ = s; return temp; }

//-------------------------------------------------------------------------
// construct() do the real work of vector construction: 

void complex_vector::construct(const int n, const v_index_mode t)
{
  internal_size = (n >= 0) ? n : 0;
  internal_mode = t;
  switch(t) {
  default:
  case Index_C: {
    if (internal_size) {    // != 0: must allocate memory
      data = delete_pointer = new Complex[internal_size];
      if (delete_pointer == 0) internal_size = 0; // out of memory
    }
    else {    // must not allocate memory
      data = delete_pointer = 0;
    }
    minindexvalue = 0; maxindexvalue = internal_size - 1;
    break;
  }
  case Index_1: {
    if (internal_size) {    // != 0: must allocate memory
      data = delete_pointer = new Complex[internal_size];
      if (delete_pointer == 0)
	internal_size = 0; // out of memory
      else
	--data;   // so data[1] is the first element
    }
    else {    // must not allocate memory
      data = delete_pointer = 0;
    }
    minindexvalue = 1; maxindexvalue = internal_size;
    break;
  }
  case Index_S: {
    // must always try to allocate memory, even if size == 0:
    delete_pointer = new Complex[2*internal_size + 1];
    if (delete_pointer == 0) {
      data = & trash; // out of memory, so send requests here
      internal_size = 0;
      maxindexvalue = -1;  // this is the "out of memory" flag
    }
    else {  // memory allocation succeeded
      data = delete_pointer + internal_size; //location of v[0]
      maxindexvalue = internal_size;
    }
    minindexvalue = -maxindexvalue;
    break;
  }
  } // switch
} // construct(const int n, const v_index_mode t = Index_C)

//---------------------------------------------------------------------
// constfill() fill the vector with a constant:

void complex_vector::constfill(const Complex f)
{ for (int i = minindex(); i <= maxindex(); ++i) data[i] = f; }

//---------------------------------------------------------------------
// copy constructor:

complex_vector::complex_vector(const complex_vector & v1)
  : size(internal_size), mode(internal_mode)
{
  construct(v1.size, v1.mode);
  // the following loop will work even if alloc for v1 failed and
  // v1.mode == Index_S.  In this case, the contents of trash will be
  // copied.
  for (int i = minindex(); i <= maxindex(); ++i)
    data[i] = v1[i];
  if(v1.is_empty())
    make_empty();
  else
    maxindex(v1.maxindex());
}

complex_vector::complex_vector(const real_vector & v1)
  : size(internal_size), mode(internal_mode)
{
  construct(v1.size, v1.mode);
  for (int i = minindex(); i <= maxindex(); ++i)
    data[i] = v1[i];
  if(v1.is_empty())
    make_empty();
  else
    maxindex(v1.maxindex());
}

//---------------------------------------------------------------------
// alias constructor:

complex_vector::complex_vector(Complex *const ptr, const int n,
			 const v_index_mode t)
  : size(internal_size), mode(internal_mode)
{
  data = ptr;
  delete_pointer = 0; // don't destroy memory on destruction
  internal_mode = t;
  if (n >= 0) {
    // n is a useful value (could hold 1 element if t = Index_S)
    internal_size = n;
    maxindexvalue = (mode == Index_C) ? (size - 1) : size;
  }
  else {
    // n isn't useful; no memory available at ptr
    internal_size = 0;
    maxindexvalue = (mode == Index_1) ? 0 : -1;
  }
  switch (mode) {  // set minindexvalue
  case Index_C: { minindexvalue = 0; break; }
  case Index_1: { minindexvalue = 1; break; }
  case Index_S: { minindexvalue = -maxindexvalue; break; }
  }
}

//-------------------------------------------------------------------------
// reindex()  change the index modes

complex_vector & complex_vector::reindex(const v_index_mode M)
{
  // first check for do nothings: new mode same as old, or
  // vector is an alias
  if (M == mode) return *this;
  if (delete_pointer == 0 && data != 0 && data != &trash) return *this;

  // If here, reindexing necessary

  // calculate number of data elements
  int n = (mode == Index_S) ? 2*size + 1 : size;
  
  // new Index_S needs an odd number of elements
  if (M == Index_S && (n>>1)<<1 == n) {  // n even
    reallocate(size+1,mode);
    n = size; // the new size
  }

  // now handle the no allocated data case:
  if (delete_pointer == 0) { reallocate(0,M); return *this; }

  // set up new indexing:
  int ptr_offset; // offset into array for data[0]
  switch (M) {
  default:
    { return *this; } // unknown mode, so do nothing
  case Index_C:
    { ptr_offset = 0; break; }
  case Index_1:
    { ptr_offset = -1; break; }
  case Index_S:
    { ptr_offset = (n - 1)/2; break; }
  } // switch
  internal_size = (M == Index_S) ? ptr_offset : n;
  minindexvalue = -ptr_offset; 
  maxindexvalue = minindexvalue + n - 1;
  data = delete_pointer + ptr_offset;
  internal_mode = M;

  return *this;
}

//---------------------------------------------------------------------
// change maxindex(), make_empty():

int complex_vector::maxindex(int n)
{
  if (size == 0 && mode != Index_S)
    // can't change the maxindex if no data elements
    return maxindex();
  else {
    switch (mode) {
    case Index_1: {
      if (n < 1) n = 1;
      maxindexvalue = (n > size) ? size : n;
      break;
    }
    case Index_S: {
      if (n < 0) n = 0;
      maxindexvalue = (n > size) ? size : n;
      minindexvalue = -maxindexvalue;
      break;
    }
    case Index_C: {
      if (n < 0) n = 0;
      maxindexvalue = (n >= size) ? (size - 1) : n;
      break;
    }
    default:
      ; // do nothing
    } //switch
    return maxindexvalue;
  } //else
}

complex_vector & complex_vector::make_empty()
{
  switch (mode) {
  case Index_C:
    maxindexvalue = -1;
    minindexvalue = 0;
    break;

  case Index_1:
    maxindexvalue = 0;
    minindexvalue = 1;
    break;

  case Index_S:
    maxindexvalue = -1;
    minindexvalue = 1;
    break;

  }
  
  return *this;
}

//---------------------------------------------------------------------
// clean up unused elements:

complex_vector & complex_vector::clean()
{
  int savemax = maxindex();
  int savemin = minindex();
  int i;
  maxindex(size); // make all memory visible
  for (i = savemax + 1; i <= maxindex(); ++i) data[i] = 0;
  for (i = savemin - 1; i >= minindex(); --i) data[i] = 0;
  maxindex(savemax);
  return *this;
}

//---------------------------------------------------------------------
// shrink index range to ignore tiny values:

complex_vector & complex_vector::shrink(Complex s)
{
  int i;       // the index
  double sum;  // accumulator of the sum of the magnitude squared's
  double ss = norm(s);  // magnitude squared for s: our tolerance value

  if (mode != Index_S)
    // Index_C and Index_1: just compare the sum at the positive indexes
    for (i = maxindexvalue, sum = 0; i > 0; --i) {
      sum += norm(data[i]);
      if (sum >= ss) break;  // found the limit
    }
  else {
    // Index_S must look at sum for negative indexes, too
    double nsum;  // accumulate negative index sum
    for (i = maxindexvalue, nsum = sum = 0; i > 0; --i) {
      sum += norm(data[i]);
      nsum += norm(data[-i]);
      if ((sum >= ss)||(nsum >= ss)) break;
    }
  }

  maxindex(i);  // set the new index and return
  return *this;
}

//---------------------------------------------------------------------
// fill all elements, regardless of data subset control:

complex_vector & complex_vector::fillall(const Complex s)
{
  int savemax = maxindexvalue;
  int savemin = minindexvalue; 
  maximize();
  constfill(s);
  maxindexvalue = savemax; minindexvalue = savemin;
  return *this;
}

//---------------------------------------------------------------------
// copy data from another vector:

complex_vector & complex_vector::copy(const complex_vector & v1)
{
  // check for self copy: "v.copy(v)", and do something intelligent:
  if (data == v1.data) { clean(); return *this; }

  // otherwise, not a self copy, so:
  maxindex(size);  constfill(0);  // maximize the index range and clear
  // now set data subset on the result:
  if(v1.is_empty())
    make_empty();
  else {
    maxindex(v1.maxindex());
    // copy over the index range intersection:
    int minimum = max(minindexvalue, v1.minindex());
    int maximum = min(maxindexvalue, v1.maxindex());
    for (/*register*/ int i = minimum; i <= maximum; ++i)
      data[i] = v1[i];
  }
  return *this;
}

complex_vector & complex_vector::copy(const real_vector & v1)
{
  maxindex(size);  constfill(0);  // maximize the index range and clear
  // now set data subset on the result:
  if(v1.is_empty())
    make_empty();
  else {
    maxindex(v1.maxindex());
    // copy over the index range intersection:
    int minimum = max(minindexvalue, v1.minindex());
    int maximum = min(maxindexvalue, v1.maxindex());
    for (/*register*/ int i = minimum; i <= maximum; ++i)
      data[i] = v1[i];
  }
  return *this;
}

//---------------------------------------------------------------------
// reallocate memory:

complex_vector & complex_vector::reallocate(const int n, const v_index_mode t)
{
  // just return if reallocation is unnecessary:
  if ((size == n)&&(mode == t)) return *this;

  // if we're here, we have to reallocate:
  complex_vector old(data, size, mode); // alias the current vector
  Complex * save_delete_pointer = delete_pointer;
  int newmax;  // will hold the new maxindexvalue
  newmax = (maxindexvalue < old.maxindex()) ? maxindexvalue : n ;

  construct(n,t);
  copy(old);
  maxindex(newmax);
  delete [] save_delete_pointer;
  return *this;
}

//---------------------------------------------------------------------
// assignment operator:

complex_vector & complex_vector::operator = (const complex_vector & v1)
{
  // check for self assignment: "v = v", and do something intelligent:
  if (data == v1.data) { clean(); return *this; }

  // otherwise, not a self assignment, so:
  maximize();
  if ((mode != v1.mode)||(maxindexvalue < v1.maxindex())) {
    // make the vector compatible with v1's data
    delete [] delete_pointer;
    int newsize = (v1.mode == Index_C) ? v1.maxindex()+1: v1.maxindex();
    construct(newsize,v1.mode);
  }
  copy(v1);
  return *this;
}

complex_vector & complex_vector::operator = (const real_vector & v1)
{
  maximize();
  if ((mode != v1.mode)||(maxindexvalue < v1.maxindex())) {
    // make the vector compatible with v1's data
    delete [] delete_pointer;
    int newsize = (v1.mode == Index_C) ? v1.maxindex()+1: v1.maxindex();
    construct(newsize,v1.mode);
  }
  copy(v1);
  return *this;
}

//---------------------------------------------------------------------
// other assignment operators (no memory reallocation for these):

complex_vector & complex_vector::operator += (const Complex s)
{
  // make local pointers to the first and last elements:
  /*register*/ Complex * d = data + minindexvalue;
  /*register*/ Complex * limit = data + maxindexvalue;
  // use the pointer to the first element as the loop variable:
  while (d <= limit) *(d++) += s;
  return *this;
}

complex_vector & complex_vector::operator -= (const Complex s)
{
  /*register*/ Complex * d = data + minindexvalue;
  /*register*/ Complex * limit = data + maxindexvalue;
  while (d <= limit) *(d++) -= s;
  return *this;
}

complex_vector & complex_vector::operator *= (const Complex s)
{
  /*register*/ Complex * d = data + minindexvalue;
  /*register*/ Complex * limit = data + maxindexvalue;
  while (d <= limit) *(d++) *= s;
  return *this;
}

complex_vector & complex_vector::operator /= (const Complex s)
{
  /*register*/ Complex * d = data + minindexvalue;
  /*register*/ Complex * limit = data + maxindexvalue;
  while (d <= limit) *(d++) /= s;
  return *this;
}

complex_vector & complex_vector::operator += (const complex_vector & v1)
{
  clean();  // ensure nonvalid elements are set to 0
  // increase the target's valid range if necessary:
  if (maxindexvalue < v1.maxindex()) maxindex(v1.maxindex());
  // loop over the intersection of the vectors' valid ranges:
  int minimum = max(minindexvalue, v1.minindex());
  int maximum = min(maxindexvalue, v1.maxindex());
  for (/*register*/ int i = minimum; i <= maximum; ++i)
    data[i] += v1[i];
  return *this;
}

complex_vector & complex_vector::operator -= (const complex_vector & v1)
{
  clean();
  if (maxindexvalue < v1.maxindex()) maxindex(v1.maxindex());
  int minimum = max(minindexvalue, v1.minindex());
  int maximum = min(maxindexvalue, v1.maxindex());
  for (/*register*/ int i = minimum; i <= maximum; ++i)
    data[i] -= v1[i];
  return *this;
}

complex_vector & complex_vector::operator += (const real_vector & v1)
{
  clean();  // ensure nonvalid elements are set to 0
  // increase the target's valid range if necessary:
  if (maxindexvalue < v1.maxindex()) maxindex(v1.maxindex());
  // loop over the intersection of the vectors' valid ranges:
  int minimum = max(minindexvalue, v1.minindex());
  int maximum = min(maxindexvalue, v1.maxindex());
  for (/*register*/ int i = minimum; i <= maximum; ++i)
    data[i].real += v1[i];
  return *this;
}

complex_vector & complex_vector::operator -= (const real_vector & v1)
{
  clean();
  if (maxindexvalue < v1.maxindex()) maxindex(v1.maxindex());
  int minimum = max(minindexvalue, v1.minindex());
  int maximum = min(maxindexvalue, v1.maxindex());
  for (/*register*/ int i = minimum; i <= maximum; ++i)
    data[i].real -= v1[i];
  return *this;
}

//---------------------------------------------------------------------
// other miscellaneous funtions:

complex_vector & complex_vector::real(const complex_vector & v)
{
  if (data != v.data) {  // only resize if v is not this or an alias 
    resize(v);
    if(v.is_empty())
      make_empty();
    else
      maxindex(v.maxindex());
  }
  int i;
  for (i = minindex(); i <= maxindex(); ++i)
    data[i] = v[i].real;
  return *this;
}

complex_vector & complex_vector::imaginary(const complex_vector & v)
{
  if (data != v.data) {  // only resize if v is not this or an alias 
    resize(v);
    if(v.is_empty())
      make_empty();
    else
      maxindex(v.maxindex());
  }
  int i;
  for (i = minindex(); i <= maxindex(); ++i)
    data[i] = v[i].imaginary;
  return *this;
}

int complex_vector::findmax() const
{
  if(maxindexvalue < minindexvalue) return 0;
  double mag;                 // magnitude squared
  double max;                 // max mag so far 
  int index = minindexvalue;  // index to return

  max = norm(data[minindexvalue]);  // initialize
  for (int i = minindexvalue + 1; i <= maxindexvalue; ++i) {
    mag = norm(data[i]);
    if ( mag > max ) {
      max = mag; index = i;
    }
  }
  return index;
}

complex_vector & complex_vector::apply(Complex (*f)(Complex))
{
  // make local pointers to the first and last elements:
  /*register*/ Complex * d = data + minindexvalue;
  /*register*/ Complex * limit = data + maxindexvalue;
  // use the pointer to the first element as the loop variable:
  for ( ; d <= limit; ++d) *d = f(*d);
  return *this;
}

complex_vector & complex_vector::apply(Complex (*f)(const Complex &))
{
  // make local pointers to the first and last elements:
  /*register*/ Complex * d = data + minindexvalue;
  /*register*/ Complex * limit = data + maxindexvalue;
  // use the pointer to the first element as the loop variable:
  for ( ; d <= limit; ++d) *d = f(*d);
  return *this;
}

complex_vector & complex_vector::apply(Complex (*f)(Complex, int))
{
  /*register*/ int i = minindexvalue; 
  /*register*/ Complex * d = data;
  /*register*/ int limit = maxindexvalue;
  for ( d += i; i <= limit; ++i, ++d) *d = f(*d, i);
  return *this;
}

complex_vector & complex_vector::apply(Complex (*f)(const Complex &, int))
{
  /*register*/ int i = minindexvalue; 
  /*register*/ Complex * d = data;
  /*register*/ int limit = maxindexvalue;
  for ( d += i; i <= limit; ++i, ++d) *d = f(*d, i);
  return *this;
}

//---------------------------------------------------------------------
// STL compatibility:

void complex_vector::reserve(int n)
{
  if(n <= size) return;
  bool e = empty(); int m = maxindexvalue;
  reallocate(n);
  if (e)
    make_empty();
  else
    maxindex(m);
}

void complex_vector::push_back(Complex s)
{
  // adjust memory allocation, if necessary
  int i = maxindex()+1;
  if (maxindex(i) != i) {
    // then incrementing maxindex didn't work; must reallocate
    reallocate(2*(size+1));  // grow size by a factor of 2
    maxindex(i);
  }
  data[i] = s;
}

void complex_vector::pop_back()
{
  if(empty()) return;
  int i = maxindex(maxindex()-1);
  if (maxindex() == i) {
    // then attempting to decrement maxindex didn't work;
    // vector is now empty
    make_empty();
  }
}

// end of complex_vector definitions
