// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ************************************************************************
// vector.h
// numeric vector classes
//
// Needs:
//   a #defined constant macro DEFAULT_VECTOR_SIZE
//      (creates its own otherwise)
//
// F. Rice 8/15/97
// ************************************************************************
//
//              The real_vector and complex_vector Classes
//
// General:
// -------
// The *_vector classes provide for manipulation of 1-dimensional array
// data. A vector object's data is stored in a contiguous chunk of memory,
// whose data elements may be indexed in various ways. The real_vector type
// gives a vector of doubles and complex_vector gives a vector containing
// Complex.
//
//
// Indexing and Capacity:
// ---------------------
// Indexing of the data elements in a vector is accomplished using an int
// index. There are three different indexing modes available, called
// Index_C, Index_1, and Index_S.  They will be described in detail below.
//
// Both controlled and uncontrolled (raw) indexed access to the data
// elements are provided.  If v is a vector object, then:
//
//          
// v.get(i);    Provides controlled, safe read and write access to the data
//              elements. The index i may be any integer value; if i
//              addresses an "element" beyond the limits of v's memory
//              capacity, this construct returns a modifiable Lvalue
//              containing 0. The program can select a subset of the
//              physically allocated memory for controlled access -- see
//              "Data subset control" below. The default is for all
//              actually allocated memory to be accessible. If the vector
//              is declared const, returns a const (read-only) Lvalue.
//
// v.read(i);   Like v.get(i), except always returns an Rvalue, so it is only
//              useful for reading data values, not writing to them.
//
// v[i];        Provides uncontrolled, direct Lvalue access to the data
//              elements.  If i addresses an "element" beyond the limits of
//              v's memory allocation, a program crash is likely. If v is
//              declared const, then the Lvalue will be const.
//
// The controlled access mode simulates an "infinite" vector, whose
// elements become identically zero if you get close enough to +/-
// infinity.  How close is "close" depends on v's capacity and indexing
// mode, and will be discussed next.
//
// The limits on i for accessing valid data members depends on the "size"
// of v and v's indexing mode. The size of v, along with the indexing mode,
// also determine v's capacity: the total number of data elements it can
// store.  Upon creation or reallocation, both the size and indexing mode
// may be specified (default values for each are also provided). The
// indexing limits and data capacities are described in the following
// table:
//
//             Indexing Modes, Index Limits, and Capacities
//
//    n : size of vector v (a nonnegative integer)
//
//      Index Mode        Index Range(*)      Data Capacity(**)
//      ----------        --------------      -----------------
//       Index_C           { 0 .. n-1 }              n
//
//       Index_1            { 1 .. n }               n
//
//       Index_S        {-n .. 0 .. n }           2*n + 1
//
// (*) Note on index range: { 0 .. n-1 } means that the index goes from
//     0 to n-1.  If the stated limits are exceeded using direct access
//     (v[i]), a runtime program crash is likely; if exceeded using
//     controlled access (v[i] or v.read(i)), a 0 will be returned, and no
//     error will result.
//
// (**)Data Capacity: the total number of data elements allocated.
//
// So the data capacity equals the size for all but the Index_S mode.  For
// an Index_S mode vector, even a size of 0 implies that one data element
// is allocated: v[0]. The Index_C mode provides tradional C-style
// indexing. The Index_1 mode (that's a "one", not an "el") starts indexing
// at 1 vice 0 and is the default mode if none is specified at creation.
// The Index_S mode provides for symmetric indexing around 0, v[0] being
// the middle element.  To determine the size of a vector object (the value
// of n used in the table above), simply examine:
//
//
// v.size;         current size of the vector v
//
//
// To easily determine the available index range for any vector object, the
// following member functions are provided (for a vector v):
//
//
// v.maxindex();   returns the int value of the maximum valid index
//
// v.minindex();   returns the int value of the minimum valid index
//
//
// If the data capacity of the vector is 0, v.maxindex is guaranteed to be
// less than v.minindex for all index modes, so a loop construct such as:
//
//   for( int i = v.minindex(); i <= v.maxindex(); ++i)
//     v[i];
//
// will always loop i over the valid index range of the vector v,
// regardless of v's size or index mode. The index range may be optionally
// adjusted to less than that determined by vector memory capacity -- refer
// to "Data subset control" for details.
//
// The following function will quickly tell you if a vector has no valid
// data elements:
//
// v.is_empty();    Returns 1 if there are no valid elements of v,
//                  otherwise returns 0.
//
//
// Specifying the Index Mode:
// -------------------------
// The default index mode is Index_1. This may be overridden by including
// an index mode value at the end of a constructor's argument list, or at
// the end of the reallocate member function's argument list. The index
// mode names are enumerations specified in an enumerated type defined in
// this header file, called "v_index_mode".
// 
// The indexing mode for a given vector object may be determined by
// examining a read-only member variable, of type v_index_mode:
//
//
// v.mode;          current indexing mode for vector v
//
//
// The indexing mode may also be changed:
//
//
// v.reindex(mode); change the index mode for this vector without moving
//                  data or reallocating memeory (mostly). Turns off any
//                  data subset control, then changes internal pointers
//                  so that the allocated memory for the vector is now
//                  indexed using the specified v_index_mode. The
//                  memory contents are not moved, so v[v.minindex()]
//                  addresses the same data element before and after the
//                  mode change (assuming no data subset control before
//                  reindex() is executed). Note that changing to Index_S
//                  requires an odd number of allocated elements; if the
//                  number is even, the vector is first resized, adding an
//                  additional element. This function has no effect if the
//                  vector aliases a preexisting memory block; such a
//                  vector's index mode cannot be changed with this fcn.
//
// Data Subset Control:
// -------------------
// At creation minindex() and maxindex() are set so that all physically
// allocated vector elements are available using controlled access (except
// for the copy constructor, in which case they match the values in the
// source vector). The default behavior is therefore to allow controlled
// access to all data elements. If only a subset of the vector near data[0]
// contains useful values, however, it may be wise to temporarily limit
// access, so that looping over the relevant data will not also include
// memory locations not currently needed, speeding up loop execution. To
// implement this capability, maxindex() may be adjusted under program
// control; minindex()is determined by the index mode and the value of
// maxindex(), as in the previous table regarding indexing modes.
//
// Note that all data elements are always accessible using data[i]. For any
// vector object v:
//
//
// v.maxindex(i);  Set the maximum valid index to i if possible. If the
//                 result would be less than the resulting minindex(), then
//                 set maxindex such that maxindex() == minindex() (0 for
//                 Index_C and Index_S, 1 for Index_1). If the result would
//                 exceed the allocated memory, set maxindex() so that all
//                 allocated memory is available (v.size for Index_1 and
//                 Index_S, v.size-1 for Index_C). If the size of the
//                 vector (v.size) is zero, maxindex(i) will have no
//                 effect, regardless of the value of i. Returns the
//                 resulting maxindex() value. To maximize maxindex(), use:
//
// v.maximize();   Adjusts maxindex() and minindex() so that all physically
//                 allocated data elements are accessible using controlled
//                 access; equivalent to:
//
//                        v.maxindex(v.size);
//
//
// v.shrink(s);    Shrinks the data subset range of v to ignore tiny
//                 values. Uses the scalar s as a tolerance defining
//                 "tiny". Starts at v[maxindex()]; accumulates the sum of
//                 the magnitude squared's of the elements going down
//                 towards v[0], and stops when this sum exceeds the square
//                 magnitude of s. Sets the new maxindex to the index at
//                 which this sum exceeds the threshold set by s.  If v is
//                 mode Index_S, separately accumulates sums for indexes
//                 near maxindex() and for indexes near minindex(); stops
//                 when either sum exceeds the square magnitude of s.
//
// v.make_empty();  Set maxindex() and minindex() to the values they would have
//                  were there to be no memory allocated for v, so that
//                  maxindex() will be less than minindex().
//
//
// Construction, Memory Allocation and Reallocation:
// ------------------------------------------------
// Upon creation a vector object's data memory will be allocated from the
// heap or, optionally, may use an already existing chunk of memory. The
// size of the vector object is specifiable as a constructor parameter.  If
// no value for size is given, the value defined by the macro
// DEFAULT_VECTOR_SIZE is used.  This macro will be defined by this header
// file if it is not already defined. The constant member variable v.size
// is set to the vector's size; it will equal 0 if memory allocation fails.
//
// If required, the memory allocation and/or indexing mode for a vector
// object may be adjusted following creation, so a vector object may be
// created with an initial size of 0, and then later reallocated once the
// required size is known.
//
// Upon destruction, a vector object will return memory to the heap only if
// it was allocated by the object, so the constructor which uses an already
// existing memory area is safe.  Using this feature, for example, allows
// functions to create local vector objects to manipulate data identified
// by a pointer argument without copying, or to create member vector
// objects which use memory allocated by the parent class object.
//
// Note for mode Index_S and v.size: v.size == 0 should mean that one data
// element is available in mode Index_S, namely v[0]. However, in the
// event of a memory allocation failure, this one element will be the same
// as that used for index out of range operations, and so will not be very
// useful. Note that in this case the maxindex() value will be -1, and the
// minindex() value will be +1, so a looping construct as described
// previously will access no values, which is the correct action.
//
// A list of the basic constructor forms follows. In this list, *_ means a
// prefix of real_ or complex_. Additionally: n is a nonnegative size; t is
// an index mode value of enumerated type v_index_mode; ptr is a pointer to
// data of the same type as the vector data type (e.g.: double ico a
// real_vector).
//
//
// *_vector v;             Create a vector with the size specified in
//                         DEFAULT_VECTOR_SIZE.  Use mode
//                         Index_1. Initializes all data elements to 0.
//
// *_vector v(t);          Another default size constructor, but with index
//                         mode specified by t. Initializes all data
//                         elements to 0.
//
// *_vector v(n);          Create a vector of size n, index mode Index_1. 
//                         (if n < 0, creates a default-sized vector)
//                         v[i] are all set to 0.
//
// *_vector v(n,t);        Create a vector of size n, index mode t. (if n <
//                         0, creates a default-sized vector) v[i] are
//                         all set to 0.
//
// *_vector v(ptr,n,t);    Use a memory block around the element pointed
//                         to by ptr for the vector data, with v[0] = 
//                         *ptr (in other words, set v.data = ptr). set
//                         v.size = n, v.mode = t. Do not initialize any
//                         data values, do not delete the block on
//                         destruction (No data memory is allocated from
//                         the heap). BE CAREFUL WITH THIS ONE! If t !=
//                         Index_C, ptr does not point to the beginning of
//                         the memory block! There is no default index mode,
//                         one must be specified. If n <= 0, set v.size = 0,
//                         treat as a memory allocation failure.
//
// For all of the above constructors, the valid index range is initialized
// to include all allocated data elements.
//
//
// *_vector v(v1);         Create a copy of vector v1: v.mode = v1.mode;
//                         v.size = v1.size. Copy all data elements, even
//                         those beyond the valid index range if data
//                         subset control is in use on vector v1. Set data
//                         subset control on v to the same limits in use on
//                         v1. If creating a real_vector, v1 must also be a
//                         real_vector.  If creating a complex_vector, v1
//                         may be of types real_vector or complex_vector.
//
// To reallocate the memory or change the indexing modes of an existing
// vector object, the resize and reallocate member functions are
// provided. When a vector is reallocated, it copies the maximum amount of
// data from the old v into the new v, given the new index size and mode
// of v, and such that each data element keeps its same index location. If
// data subset control is being used, then the new valid index range will
// match as closely as possible the old valid index range, but all physical
// data will be copied, not just the data in the valid index range. If data
// subset control is not being used (the maxindex and minindex values
// include all allocated memory), then the new maxindex and minindex values
// will include all of the newly allocated memory. The resize() member
// function is useful if you know the resulting index range you want. The
// reallocate() function is a more primitive construct which is called by
// resize(). If v is a vector object, then:
//
//
// v.resize(n);          Reallocate the memory for v so that its size will
//                       just fit a  maximum index value of n. Keep the
//                       current indexing mode.
//
// v.resize(v1);         Reallocate the memory for v so that its index
//                       mode matches that of vector v1, and its size will
//                       just fit the valid index range of v1, as
//                       determined by v1's data subset control.
//
// v.reallocate(n);      Reallocate the memory for v to change it into a
//                       vector with size n.  Keep the current indexing
//                       mode.
//
// v.reallocate(n,t);    Reallocate the memory for v to change its size
//                       and mode to n and t.
//
// In addition to the reallocate member function, the assignment operator
// '=' may result in memory reallocation.  This behavior is described in
// the next section.
//
//
// Assignment and Copying Data:
// ---------------------------
// The following member functions are provided for bulk data transfer from
// one vector object into another existing object: = and copy.  Both
// functions attempt to preserve the source vector's data subset control,
// and both set unused elements in the destination vector to 0.  There is a
// major difference, however, between the two operations: = may reallocate
// memory for the receiving vector, while copy leaves the size and mode of
// the receiving vector unchanged.  Note that self assignment ( v = v ) and
// self copy ( v.copy(v) ) are not "do nothing" operations.
//
//
// v = v1;         If v has insufficient memory to hold all of v1's valid
//                 (data subset control) data, or if the index mode of v
//                 differs from that of v1, reallocate the memory for v so
//                 that it has the same index mode as v1, with a size just
//                 sufficient to hold all of the valid data elements of v1.
//                 Copy all of v1's valid data elements into v, and set any
//                 remaining elements of v to 0. Set the data subset control
//                 for v so that its valid index range matches the valid
//                 index range for v1.  When finished, v will be a (possibly
//                 larger) copy of the valid data elements of v1, with the
//                 same valid index range and index mode.  If v1 and v are
//                 the SAME vector (even if one is an alias of the other),
//                 just perform v.clean().
//
// v.copy(v1);     Copy as many of the valid data elements of v1 into the
//                 corresponding elements of v as the memory allocation and
//                 indexing mode of v will allow.  In other words, copy the
//                 data elements with indexes in the intersection of the
//                 indexing ranges of v and v1.  Only copies data within
//                 v1's currently valid index range, so data subset control
//                 on v1 may reduce the number of elements copied. Ignores
//                 any data subset control previously set for v. Sets the
//                 remaining elements of v to 0, and sets v data subset
//                 control to match as closely as possible v1's valid index
//                 range.  If v1 and v are the SAME vector (even if one is
//                 an alias of the other), just perform v.clean().
//
// As with the copy constructor, v1 may be a real_vector or a
// complex_vector if v is of type complex_vector; v1 must be of type
// real_vector if v is of type real_vector.
//
//
// Some Other Basic Vector Member Functions:
// ----------------------------------------
//
//
// v.fill(s);      Fill all elements in the valid index range of v with the
//                 scalar value s. The fill is subject to data subset
//                 control.
//
// v = s;          For scalar s, the same as v.fill(s). s may not be
//                 Complex if v is of type real_vector.
//
// v.fillall(s);   Fill all physically allocated elements of v with the
//                 scalar value s. Ignores data subset control.
//
// v.clean();      Resets to 0 all allocated data elements beyond the
//                 boundaries set by v.minindex() and v.maxindex()
//
// v.unit(n);      Set v[n] = 1, if n is a valid index for v. Set all other
//                 elements of v to 0. Uses data subset to determine if n
//                 is a valid index; if n isn't valid, all elements are set
//                 to zero.
//
// v.findmax();    Return the int index of the element of v with the
//                 largest magnitude within v's valid index range (data
//                 subset control), or 0 if v contains no elements.
//
// v.swap(n,m);    Swap the contents of v[n] and v[m]. Use 0 as the value
//                 of a data element for an index outside the valid index
//                 range (data subset control).
//
// v.apply(f);     Apply the scalar function f to each valid element of
//                 v. f() will be called once for each valid element of v.
//                 f must be a function taking either:
//                  (1) a single numeric argument of the same type as the
//                      data elements of v: f(double x) or f(Complex x).
//                  (2) a numeric argument and an int argument:
//                      f(double x, int i) or f(Complex x, int i)
//                 f() must return a single numeric value of the same type
//                 as the data elements of v. As apply() iterates through
//                 the vector elements, it calls f() with the value of each
//                 element and, if f() is of type (2) above, the value of
//                 the index of the element.  Equivalent to:
//
//                   for (int i = v.minindex(); i <= v.maxindex(); ++i)
//                     v[i] = f(v[i]); /* OR */ v[i] = f(v[i],i);
//
//
// Vector Arithmetic Member Functions:
// ----------------------------------
//
//
// v += s;  v -= s;  v *= s;  v /= s;
//                 Perform the indicated assignment operator operation to
//                 each element of v within its valid index range, using
//                 data subset control.  The scalar s may not be Complex if
//                 v is a real_vector.
//
// v += v1; v -= v1;  First perform v.clean(), then perform the indicated
//                 assignment operation as a vector addition or subtraction
//                 using only valid elements of v1 (data subset control on
//                 v1).  Increases the data subset range of v if necessary
//                 so that its valid index range includes the valid index
//                 range of v1 as a subset, unless limited by the physical
//                 capacity of v. Does not reallocate any memory for v or
//                 change v's index mode.
//
// v.real(v1);     sets each element of v equal to the real part of the
//                 corresponding element of complex_vector v1. Performs
//                 v.resize(v1) first, so that it is just big enough to
//                 hold the valid elements of v1.  v may be either a
//                 real_vector or a complex_vector; v1 must be a complex_
//                 vector. (The resize is not performed if v1 is the same
//                 vector as v)
//
// v.imaginary(v1); sets each element of v equal to the imaginary part of
//                 the corresponding element of complex_vector v1. Performs
//                 v.resize(v1) first, so that it is just big enough to
//                 hold the valid elements of v1.  v may be either a
//                 real_vector or a complex_vector; v1 must be a complex_
//                 vector. (The resize is not performed if v is the same
//                 vector as v1)
//
//
// STL Compatibility:
// -----------------
//
// The following types and functions are provided to mimic as closely as
// possible the behavior of the similarly-named objects which operate on the
// Standar Template Library vector<double> and vector<Complex> classes. They
// may be used with STL generic algorithms, for example.
//
// v.push_back(s); Add scalar s just beyond the last element in the valid
//                 index range of v, increasing v.maxindex() by 1. If v is
//                 not large enough to hold this additional element, resize
//                 v so that it will hold about twice as many elements as 
//                 before.
//
// v.pop_back();   Attempt to reduce maxindex() by 1. If maxindex()<=minindex(),
//                 then make v empty using make_empty().
//
// v.capacity();   Just return v.size.
//
// v.reserve(n);   Similar to v.reallocate(n), unless the size of v already
//                 exceeds n, in which case do nothing. Unlike reallocate(),
//                 v.maxindex() is always unaffected by reserve().
//
// v.empty();      Like is_empty(), except returns a bool rather than int.
//
// v.front();      Returns a reference to v[v.minindex()];
//
// v.back();       Returns a reference to v[v.maxindex()];
//
//
// Types:
//
// real_vector::value_type;         double
// complex_vector::value_type;      Complex
//
// real_vector::reference;          double &
// complex_vector::reference;       Complex &
//
// real_vector::const_reference;    const double &
// complex_vector::const_reference; const Complex &
//
// real_vector::iterator;           double*
// complex_vector::iterator;        Complex*
//
// real_vector::const_iterator;     const double*
// complex_vector::const_iterator;  const Complex*
//
// real_vector::difference_type;    int  (ditto for complex_vector)
// real_vector::size_type;          int  (since indexes can be negative)
//
// The iterators are all STL "random access" iterators, with all the requisite
// operations defined for them. reverse_iterator types may be added in a future
// release.
//
// Given the above types, including iterators, we also have the functions:
//
// v.begin();      Iterator pointing to v[v.minindex()]
//
// v.end();        Iterator pointing to v[v.maxindex()+1]
//
// Note that as with STL vector<> class, any operation which may result in
// reallocation of the vector's memory will invalidate all iterators to that
// vector. 
//
//
// Vector Output/Display Functions:
// -------------------------------
//
// ostream << v;   send the contents of v to the output stream ostream. Note
//                 that << is not a member function of any class.  The
//                 elements of v in its valid index range are sent to ostream
//                 in index order. The elements of v are separated by the
//                 C++ string whose value is controlled by the static member
//                 function out_separator(); the default value is a single
//                 space character ' '.  The output of each element is into a
//                 field of width determined by ostream.width(); the separator
//                 string is not affected by the ostream.width() value. Note
//                 that << is used to output each individual element.
//
// v.show(ostream); output equivalent to:  ostream << endl << v << endl;
//
// v.show();       equivalent to v.show(cout);
//
// real_vector::out_separator(s); or: complex_vector::out_separator(s);
//                 either function sets the output separator string for
//                 both real_vector and complex_vector output. s may be a
//                 C++ string, a C-style string, or a single char. The
//                 function returns the previous value of the separator as
//                 a C++ string. If no argument is specified, then the
//                 function simply returns the current separator value. The
//                 default value is a single space. If no output separator
//                 character is desired, use a null string "" for s.
//
// ************************************************************************

#ifndef VECTOR_H
#define VECTOR_H

#ifndef DEFAULT_VECTOR_SIZE
#define DEFAULT_VECTOR_SIZE   16   /* a default vector size */
#endif  /* DEFAULT_VECTOR_SIZE */

#ifndef V_INDEX_MODE_DEFINED
#define  V_INDEX_MODE_DEFINED
enum v_index_mode { Index_C, Index_1, Index_S };
#endif /* V_INDEX_MODE_DEFINED */

#include "SIScmplx.h"
#include <iosfwd>
#include <string>

class real_vector;
class complex_vector;

//---------------------------------------------------------------------

class real_vector {

public:

  // data:

  const int     &size;             // the size of v
  const v_index_mode   &mode;      // the indexing mode for v


  // constructors:

  explicit real_vector(const v_index_mode t = Index_1)
    : size(internal_size), mode(internal_mode)
  { construct(DEFAULT_VECTOR_SIZE, t); constfill(0.0); }

  explicit real_vector(const int n, const v_index_mode t = Index_1)
    : size(internal_size), mode(internal_mode)
  { construct(n, t); constfill(0.0); }

  real_vector(const real_vector & v1);

  real_vector(double *const ptr, const int n, const v_index_mode t);

  // destructor:

  ~real_vector(void) { delete [] delete_pointer; }

  // data subset contol:

  int minindex(void) const  { return minindexvalue; }
  int maxindex(void) const  { return maxindexvalue; }
  int maxindex(int n);
  int maximize(void) { return maxindex(size); }
  real_vector & shrink(double s);
  int is_empty() const  { return maxindex() < minindex(); }
  real_vector & make_empty();

  // change index mode:

  real_vector & reindex(const v_index_mode);

  // data access:
  
  double read (const int i) const
  { return ((i > maxindex())||(i < minindex())) ? 0.0 : data[i]; }

  double & get (const int i)
  { return ((i > maxindex())||(i < minindex())) ? (trash = 0.0) : data[i]; }

  const double & get (const int i) const
  { return ((i > maxindex())||(i < minindex())) ? (trash = 0.0) : data[i]; }

  double & operator [] (const int i) 
  { return data[i]; }

  const double & operator [] (const int i) const
  { return data[i]; }

  // data copying, assignment, and reallocation:

  real_vector & copy(const real_vector & v1);
  real_vector & operator = (const real_vector & v1);
  real_vector & reallocate(const int n, const v_index_mode t);
  real_vector & reallocate(const int n)  { return reallocate(n,mode); }
  real_vector & resize(const int Max)
  { int Size = (mode == Index_C) ? Max+1: Max; return reallocate(Size); }
  real_vector & resize(const real_vector & v)
  {
    int Size = (v.mode == Index_C) ? v.maxindex()+1: v.maxindex();
    return reallocate(Size, v.mode);
  }
  real_vector & resize(const complex_vector & v);

  // other basic vector member functions:

  real_vector & clean(void);
  real_vector & fill(const double s) { constfill(s); return *this; }
  real_vector & operator = (const double s) { return fill(s); }
  real_vector & fillall(const double s);
  real_vector & unit(int n) { fillall(0); get(n) = 1; return *this; }
  int findmax() const;
  real_vector & swap(const int n, const int m)
  { 
    if (n != m) { double temp = get(n); get(n) = get(m); get(m) = temp; }
    return *this;
  }
  real_vector & apply(double (*f)(double));
  real_vector & apply(double (*f)(const double & x));
  real_vector & apply(double (*f)(double, int));
  real_vector & apply(double (*f)(const double &, int));

  // vector arithmetic member functions:

  real_vector & operator += (const double s);
  real_vector & operator -= (const double s);
  real_vector & operator *= (const double s);
  real_vector & operator /= (const double s);
  real_vector & operator += (const real_vector & v1);
  real_vector & operator -= (const real_vector & v1);
  real_vector & real(const complex_vector & v);
  real_vector & imaginary(const complex_vector & v);
  
  // vector output member functions:

  real_vector & show(std::ostream &);
  real_vector & show(void);
  const real_vector & show(std::ostream &) const;
  const real_vector & show(void) const;
  static std::string out_separator(); // return the current output separator
  static std::string out_separator(const std::string &);
  static std::string out_separator(const char * const);
  static std::string out_separator(const char);


  // STL Types:

  typedef   double                 value_type;
  typedef   value_type &           reference;
  typedef   const reference        const_reference;
  typedef   value_type *           iterator;
  typedef   const value_type *     const_iterator;
  typedef   int                    difference_type;
  typedef   int                    size_type;

  // STL compatibility declarations:

  bool empty() const  { return bool(is_empty()); }
  int capacity() const  { return size; }
  void reserve(int n);
  double & front() { return data[minindexvalue]; }
  const double & front() const  { return data[minindexvalue]; }
  double & back() { return data[maxindexvalue]; }
  const double & back() const  { return data[maxindexvalue]; }
  void push_back(double s);
  void pop_back();
  iterator begin() { return & data[minindexvalue]; }
  iterator end()   { return & data[maxindexvalue+1]; }
  const_iterator begin() const { return & data[minindexvalue]; }
  const_iterator end()   const { return & data[maxindexvalue+1]; }

private:

  int            internal_size;    // aliased by size
  v_index_mode   internal_mode;    // aliased by mode
  int            maxindexvalue;    // the index upper limit
  int            minindexvalue;    // the index lower limit
  double        *data;             // The vector raw data pointer
  double        *delete_pointer;   // used by destructor to delete memory
  mutable double trash;            // Where index-out-of-bounds wind up

  // these functions are called by the constructors:
  void construct(const int n, const v_index_mode t); //allocate memory
  void constfill(const double f); // fill with a constant value

};  // class real_vector

std::ostream & operator << (std::ostream &, const real_vector &);

//---------------------------------------------------------------------

class complex_vector {

public:

  // data:

  const int     &size;             // the size of v
  const v_index_mode   &mode;      // the indexing mode for v


  // constructors:

  explicit complex_vector(const v_index_mode t = Index_1)
    : size(internal_size), mode(internal_mode)
  { construct(DEFAULT_VECTOR_SIZE, t); constfill(0.0); }

  explicit complex_vector(const int n, const v_index_mode t = Index_1)
    : size(internal_size), mode(internal_mode)
  { construct(n, t); constfill(0.0); }

  complex_vector(const complex_vector & v1);
  complex_vector(const real_vector & v1);

  complex_vector(complex *const ptr, const int n, const v_index_mode t);


  // destructor:

  ~complex_vector(void) { delete [] delete_pointer; }

  // data subset contol:

  int minindex(void) const  { return minindexvalue; }
  int maxindex(void) const  { return maxindexvalue; }
  int maxindex(int n);
  int maximize(void) { return maxindex(size); }
  complex_vector & shrink(Complex s);
  int is_empty() const  { return maxindex() < minindex(); }
  complex_vector & make_empty();

  // change index mode:

  complex_vector & reindex(const v_index_mode);

  // data access:
  
  Complex read (const int i) const
  { return ((i > maxindex())||(i < minindex())) ? 0.0 : data[i]; }

  Complex & get (const int i)
  { return ((i > maxindex())||(i < minindex())) ? (trash = 0.0) : data[i]; }

  const Complex & get (const int i) const
  { return ((i > maxindex())||(i < minindex())) ? (trash = 0.0) : data[i]; }

  Complex & operator [] (const int i) 
  { return data[i]; }

  const Complex & operator [] (const int i) const
  { return data[i]; }

  // data copying and reallocation:

  complex_vector & copy(const complex_vector & v1);
  complex_vector & copy(const real_vector & v1);
  complex_vector & operator = (const complex_vector & v1);
  complex_vector & operator = (const real_vector & v1);
  complex_vector & reallocate(const int n, const v_index_mode t);
  complex_vector & reallocate(const int n)
  { return reallocate(n,mode); }
  complex_vector & resize(const int Max)
  { int Size = (mode == Index_C) ? Max+1: Max; return reallocate(Size); }
  complex_vector & resize(const real_vector & v)
  {
    int Size = (v.mode == Index_C) ? v.maxindex()+1: v.maxindex();
    return reallocate(Size, v.mode);
  }
  complex_vector & resize(const complex_vector & v)
  {
    int Size = (v.mode == Index_C) ? v.maxindex()+1: v.maxindex();
    return reallocate(Size, v.mode);
  }

  // other basic vector member functions:

  complex_vector & clean(void);
  complex_vector & fill(const Complex s) { constfill(s); return *this; }
  complex_vector & operator = (const Complex s) { return fill(s); }
  complex_vector & operator = (const double s) 
  { return fill(Complex(s)); }
  complex_vector & fillall(const Complex s);
  complex_vector & unit(int n) { fillall(0); get(n) = 1; return *this; }
  int findmax() const;
  complex_vector & swap(const int n, const int m)
  {
    if (n != m) { Complex temp = get(n); get(n) = get(m); get(m) = temp; }
    return *this;
  }
  complex_vector & apply(Complex (*f)(Complex));
  complex_vector & apply(Complex (*f)(const Complex &));
  complex_vector & apply(Complex (*f)(Complex, int));
  complex_vector & apply(Complex (*f)(const Complex &, int));

  // vector arithmetic member functions:

  complex_vector & operator += (const Complex s);
  complex_vector & operator -= (const Complex s);
  complex_vector & operator *= (const Complex s);
  complex_vector & operator /= (const Complex s);
  complex_vector & operator += (const complex_vector & v1);
  complex_vector & operator -= (const complex_vector & v1);
  complex_vector & operator += (const real_vector & v1);
  complex_vector & operator -= (const real_vector & v1);
  complex_vector & real(const complex_vector & v);
  complex_vector & imaginary(const complex_vector & v);

  // vector output member functions:

  complex_vector & show(std::ostream &);
  complex_vector & show(void);
  const complex_vector & show(std::ostream &) const;
  const complex_vector & show(void) const;
  static std::string out_separator(); // return the current output separator
  static std::string out_separator(const std::string &);
  static std::string out_separator(const char * const);
  static std::string out_separator(const char);


  // STL Types:

  typedef   Complex                value_type;
  typedef   value_type &           reference;
  typedef   const reference        const_reference;
  typedef   value_type *           iterator;
  typedef   const value_type *     const_iterator;
  typedef   int                    difference_type;
  typedef   int                    size_type;

  // STL compatibility declarations:

  bool empty() const  { return bool(is_empty()); }
  int capacity() const  { return size; }
  void reserve(int n);
  Complex & front() { return data[minindexvalue]; }
  const Complex & front() const  { return data[minindexvalue]; }
  Complex & back() { return data[maxindexvalue]; }
  const Complex & back() const  { return data[maxindexvalue]; }
  void push_back(Complex s);
  void pop_back();
  iterator begin() { return & data[minindexvalue]; }
  iterator end()   { return & data[maxindexvalue+1]; }
  const_iterator begin() const { return & data[minindexvalue]; }
  const_iterator end()   const { return & data[maxindexvalue+1]; }


private:

  int            internal_size;    // aliased by size
  v_index_mode   internal_mode;    // aliased by mode
  int            maxindexvalue;    // the index upper limit
  int            minindexvalue;    // the index lower limit
  Complex       *data;             // The vector raw data pointer
  Complex       *delete_pointer;   // used by destructor to delete memory
  mutable Complex trash;           // Where index-out-of-bounds wind up

  // these functions are called by the constructors:
  void construct(const int n, const v_index_mode t); //allocate memory
  void constfill(const Complex f); // fill with a constant value

};  // class complex_vector 

std::ostream & operator << (std::ostream &, const complex_vector &);

//---------------------------------------------------------------------

#endif  /* VECTOR_H */
