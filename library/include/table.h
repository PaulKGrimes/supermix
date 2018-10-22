// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ************************************************************************
// table.h
// numeric matrix classes
//
// Needs:
//   a #defined constant macro DEFAULT_MATRIX_SIZE
//      (creates its own otherwise)
//
// F. Rice 9/24/97
// ************************************************************************
//
//		 The real_matrix and complex_matrix Classes
//
// General:
// -------
// The *_matrix classes provide for manipulation of 2-dimensional array
// data. A matrix object's data is allocated and referenced as a set of 1-D
// data "rows", each in its own contiguous chunk of memory.  A 1-D array
// contains pointers to each of the rows, providing for the row indexing.
// Each row is in effect a numeric vector "record", whose location is
// maintained in the pointer array. The real_matrix can store type double
// data elements, the complex_matrix can store type Complex data elements.
//
// Even though implemented as a list of rows, the matrix classes require
// that all rows have the same data capacity and indexing mode.  When
// created, a matrix allocates a single, contiguous block of memory for the
// data elements, which it divides up into the individual row arrays.  Upon
// destruction, the entire block of memory is deleted.
//
//
// Indexing and Capacity:
// ---------------------
// Indexing of the data elements in a matrix is accomplished using a pair of
// int indexes.  There are three different indexing modes available, and
// the row indexing and column indexing modes may differ in a given matrix
// object.  The three indexing modes available are called Index_C, Index_1,
// and Index_S, and are identical to the modes used for *_vector indexing,
// described in vectors.h.
//
// Both controlled and uncontrolled (direct) indexed access to the data
// elements are provided.  If "A" is a matrix object, then:
//
//
// A.get(i,j);      Provides controlled, safe read and write access to
//                  the data elements.  The indexes i and j may have any
//                  integer values; i selects the row, and j selects the
//                  element within the row (column). If (i,j) selects an
//                  "element" beyond the limits of A's memory capacity,
//                  A.get(i,j) returns a modifiable Lvalue containing 0.
//                  The program can select a subset of the physically
//                  allocated memory for controlled access -- see "Data
//                  subset control" below.  The default is for all
//                  allocated memory to be accessible. get returns an
//                  Lvalue, so it can be used to modify a data element,
//                  e.g.:   A.get(2,3) = 5.0;
//                  If the matrix is declared const, returns a const
//                  (read-only) Lvalue.
//
// A.read(i,j);     Like A.get(i,j), except returns an Rvalue, so it is
//                  only useful for reading data values, not writing to
//                  them.
//
// A[i][j];         Provides uncontrolled, direct Lvalue access to the data
//                  elements. [i] selects a row pointer, [j] selects an
//                  offset from element 0 within the row. If [i][j] addresses
//                  a nonexistant element of A, a program crash is likely. If
//                  a is declared const, then the Lvalue will be const.
//
// A[i];            Returns a const Lvalue pointer to the data in row i of matrix
//                  A. Actually points to A[i][0], which is not necessarily
//                  the first (lowest address) data element in row i. In
//                  fact, if the right index mode is Index_1, then A[i]
//                  does not directly point to any valid data element in
//                  row i. See the section "Manipulating Matrix Rows as
//                  Vector Objects" for a quite useful application of this
//                  construct. If A is declared const, then the pointer
//                  returned will be to const data.
//
// The controlled access modes simulate an "infinite" matrix, whose
// elements become identically zero if an index gets close enough to +/-
// infinity. How close is "close" depends on A's data capacity and the
// indexing modes it uses for row and column indexing.
//
// Referring to the following indexing modes and limits matrix: 
//
//             Indexing Modes, Index Limits, and Capacities
//
//             n : size of an index (a nonnegative integer)
//
//          Index Mode        Index Range         Data Capacity
//          ----------        -----------         -------------
//           Index_C          { 0 .. n-1 }              n
//
//           Index_1           { 1 .. n }               n
//
//           Index_S         {-n .. 0 .. n }         2*n + 1
//
// If, for example, the left index (row) mode is Index_1 and its size is n,
// while the right index (within a row) mode is Index_S, size m, then the
// total data capacity of the matrix is (referring to the matrix above)
// n * (2*m + 1). The sizes for the left and right indexes are specified at
// construction or reallocation and are kept as read-only member variables. 
// For matrix object A:
//
//
// A.Lsize;         Current value of the size of the left index range
//
// A.Rsize;         Current value of the size of the right index range
//
//
// To easily determine the valid index range for a given matrix object A,
// the following member functions are provided:
//
//
// A.Lmaxindex();   Returns the int value of the max valid left index
// 
// A.Rmaxindex();   Returns the int value of the max valid right index
//
// A.Lminindex();   Returns the int value of the min valid left index
//
// A.Rminindex();   Returns the int value of the min valid right index
//
//
// The left index picks the row, the right index picks the element in the
// row (or column).
//
// Many times a square matrix using the same addressing mode for each index
// is required. the following member functions are particularly useful in
// this case, though they may be called regardless of the matrix topology:
//
//
// A.maxindex();    Returns the minimum of A.Lmaxindex() and A.Rmaxindex()
//
// A.minindex();    Returns the maximum of A.Lminindex() and A.Rminindex()
//
//
// For a square matrix, these functions return the proper limits for both
// row and column indexing. For other matrices, they would identify a valid
// subset of the full matrix indexing range.
//
// As for the vector classes, a maxindex is guaranteed to be smaller than
// its corresponding minindex if the data capacity of the matrix is 0.
// Therefore, loop constructs such as:
//
//      for (int i = A.Lminindex(); i <= A.Lmaxindex(); ++i)
//        for (int j = A.Rminindex(); j <= A.Rmaxindex(); ++j)
//          A[i][j];
//
// will properly loop over the valid index values of the matrix A,
// regardless of its index modes or data capacity.
//
// The following function will quickly tell you if a matrix has no valid
// data elements:
//
// A.is_empty();    Returns 1 if there are no valid elements of A,
//                  otherwise returns 0.
//
//
// Specifying the Index Modes:
// --------------------------
// The default index mode for both the left and right indexes is Index_1.
// This may be overridden by including one or two index mode values at the
// end of a constructor's argument list, or at the end of the reallocate
// member function's argument list.  If only one index mode value is
// supplied, it will be used for both indexes.  If two values are supplied,
// the first will be used for the left index, the other for the right
// index. The index mode names are enumerations specified in an enumerated
// type defined in this header file, called "v_index_mode".
//
// The index mode values for the left and right indexes for a matrix object
// are maintained in read-only member variables.  For matrix object A:
//
//
// A.Lmode;    current indexing mode value for the left index
//
// A.Rmode;    current indexing mode value for the right index
//
// The indexing mode may also be changed:
//
//
// A.reindex(Lmode, Rmode);   change the index modes for this matrix
//             without moving data or reallocating memeory (mostly). Turns
//             off any data subset control, then changes internal pointers
//             so that the allocated memory for the matrix is now indexed
//             using the specified v_index_modes. The memory contents are
//             not moved, so:   A[A.Lminindex()][A.Rminindex()]
//             addresses the same data element before and after the mode
//             change (assuming no data subset control before reindex() is
//             executed). Note that changing to Index_S requires an odd
//             number of allocated elements along that index direction;
//             if the number is even, the matrix is first resized, adding
//             an additional element.
//
//
// Data Subset Control:
// -------------------
// At creation (L/R)minindex() and (L/R)maxindex() are set so that all
// physically allocated matrix elements are available using controlled
// access (except for the copy constructor, in which case they match the
// values in the source matrix). The default behavior is therefore to allow
// controlled access to all data elements. If only a subset of the matrix
// near data[0][0] contains useful values, however, it may be wise to
// temporarily limit access, so that looping over the relevant data will
// not also include memory locations not currently needed, speeding up loop
// execution. To implement this capability, both Lmaxindex() and
// Rmaxindex() may be adjusted under program control; Lminindex() and
// Rminindex() are determined by the index modes and the value of the
// respective maxindex().
//
// Note that all data elements are always accessible using A[i][j].  For
// any matrix object A:
//
//
// A.Lmaxindex(i);  Set the maximum valid left index to i if possible. If
//                  the result would be less than the resulting
//                  Lminindex(), then set Lmaxindex such that Lmaxindex()
//                  == Lminindex() (0 for Index_C and Index_S, 1 for
//                  Index_1). If the result would exceed the allocated
//                  memory, set Lmaxindex so that all allocated memory is
//                  available (A.Lsize for Index_1 and Index_S, A.Lsize-1
//                  for Index_C). If the size of the left index (A.Lsize)
//                  is zero, A.Lmaxindex(i) will have no effect, regardless
//                  of the value of i. Returns the resulting Lmaxindex()
//                  value. To maximize Lmaxindex(), use:
//                  A.Lmaxindex(A.Lsize);
//
//
// A.Rmaxindex(i);  Set the maximum valid right index to i if possible.
//                  Same notes as for Lmaxindex(i). To maximize
//                  Rmaxindex(), use: A.Rmaxindex(A.Rsize);
//
//
// A.maxindex(i);   Set the maximum vaild index for both the left and right
//                  indexes to i if possible. Individually limit the
//                  indexes to valid ranges as described for Lmaxindex(i)
//                  and Rmaxindex(i); this may result in a nonsquare index
//                  range if A.Lsize != A.Rsize or A.Lmode != A.Rmode.
//                  Returns the resulting A.maxindex(). To simultaneously
//                  maximize both index ranges, use:
//
// A.maximize();    Set Lmaxindex() and Rmaxindex() to the largest values
//                  allowed by physically allocated memory and their modes.
//                  Equivalent to:  A.maxindex(A.Lsize + A.Rsize);
//
// A.make_empty();  Set Lmaxindex() and Rmaxindex() and Lminindex() and  
//                  Rminindex() to the values they would have were there to
//                  be no memory allocated for A, so that each maxindex()
//                  will be less than its corresponding minindex().
//
//
// Construction, Memory Allocation and Reallocation:
// ------------------------------------------------ 
// Upon creation a matrix object's data memory will be allocated from the
// heap. The sizes of each dimension (left index and right index) for the
// matrix object are specifiable as constructor parameters. If no values for
// the sizes are given, a square matrix is constructed using the value
// defined by the macro DEFAULT_MATRIX_SIZE for both dimensions. This macro
// will be defined by this header file if it is not already defined. The
// constant member variables Lsize and Rsize are set to the matrix's sizes;
// both will equal 0 if memory allocation fails.
//
// If required, the memory allocation and/or indexing modes for an
// individual matrix object may be adjusted following creation, so a matrix
// object may be created with initial sizes of 0, and then later
// reallocated once the required sizes are known.
//
// Upon destruction, a matrix object will return its memory to the heap.
//
// Note for mode Index_S and (L/R)size: size == 0 should mean that one
// valid index value is available in mode Index_S, namely 0. However, in
// the event of a memory allocation failure, this one index value will
// actually access the same location as that used for index out of range
// operations, and so will not be very useful.  Note that in this case the
// maxindex() value will be -1, and the minindex() value will be +1, so a
// looping construct as described previously will access no values, which
// is the correct action.
//
// A list of the basic constructor forms follows. In this list, *_ means a
// prefix of real_ or complex_. Additionally: n and m are nonnegative int
// sizes for the left and right indexes; t, tl, and tr are index mode
// values of enumerated type v_index_mode and specify the index modes for
// both indexes, the left index, and the right index respectively.
//
//
// *_matrix A;           Construct a square matrix with size given by the
//                       value of DEFAULT_MATRIX_SIZE and index mode Index_1
//                       in each index.  Initialize all data elements to 0.
//
// *_matrix A(n);        Construct a matrix with left and right index sizes
//                       both equal n, and use Index_1 mode for both
//                       indexes. Initialize all data elements to 0.
//
// *_matrix A(n,t);      Construct a matrix with left and right index sizes
//                       both equal n, and use index mode t for both
//                       indexes. Initialize all data elements to 0.
//
// *_matrix A(n,m);      Construct a matrix with left and right index sizes
//                       of n and m, respectively. Use Index_1 for both
//                       indexes. Initialize all data elements to 0.
//
// *_matrix A(n,m,t);    Construct a matrix with left and right index sizes
//                       of n and m, respectively. Use index mode t for
//                       both indexes. Initialize all data elements to 0.
//
// *_matrix A(n,m,tl,tr); Construct a matrix with left and right index sizes
//                       of n and m, respectively. Use index mode tl for
//                       the left index, tr for the right index. Initialize
//                       all data elements to 0.
//
// For all of the above constructors, the valid index range is initialized
// to include all allocated data elements.
//
//
// *_matrix A(B);        Construct a copy of the matrix B, with the same
//                       sizes and index modes as B, and with the same
//                       valid index ranges (maxindex and minindex). Copy
//                       all data from B, even any elements beyond the
//                       valid index ranges.  If A is a complex_matrix, then
//                       B may be either a complex_matrix or a real_matrix
//                       object. If A is a real_matrix, then B must also be
//                       a real_matrix.
//
// *_matrix A(v);        Construct a matrix which is a copy of the vector v.
//                       The matrix will have one column, ie: Rsize == 1,
//                       and will have Rmode == Index_1. The Left indexing
//                       will match the indexing of the vector v, and all
//                       physical elements of v will be copied. Finally,
//                       set Lmaxindex() == v.maxindex(). If A is complex,
//                       v may be either a complex_vector or a real_vector
//                       object. If A is a real_matrix, then v must be a
//                       real_vector.
//
// real_matrix A(D);     Construct a real_matrix which is a copy of the
//                       data matrix in the datafile class object D.
//
// To reallocate the memory or change the indexing modes of an existing
// matrix object, the resize and reallocate member functions are
// provided. When a matrix is reallocated, it copies the maximum amount of
// data from the old A into the new A, given the new index sizes and modes
// of A, and such that each data element keeps its same index locations. If
// data subset control is being used, then the new valid index range will
// match as closely as possible the old valid index range, but all physical
// data will be copied, not just the data in the valid index range. If data
// subset control is not being used (the maxindex and minindex values
// include all allocated memory), then the new maxindex and minindex values
// will include all of the newly allocated memory. The resize() member
// function is useful if you know the resulting index range you want. The
// reallocate() function is a more primitive construct which is called by
// resize(). If A is a matrix object, then:
//
// A.resize(n,m);        Reallocate the memory for A so that its size will
//                       just fit left and right maximum index values of n
//                       and m, respectively.  Keep the current indexing
//                       modes.
//
// A.resize(n);          Reallocate the memory for A so that its size will
//                       just fit left and right maximum index values of n.
//                       Keep the current indexing modes.
//
// A.resize(B);          Reallocate the memory for A so that its index
//                       modes match those of matrix B, and its size will
//                       just fit the valid index ranges of B, as
//                       determined by B's data subset control.
//
// A.reallocate(n);      Reallocate the memory for A to change it into a
//                       matrix with size n for each index.  Keep the
//                       current indexing modes.
//
// A.reallocate(n,m);    Reallocate the memory for A to change it into a
//                       matrix with left and right index sizes n and m,
//                       respectively. Keep the current indexing modes.
//
// A.reallocate(n,m,tl,tr);   Reallocate the memory for A to change its
//                       left index size and mode to n and tl, and the
//                       right index size and mode to m and tr.
//
// In addition to the reallocate member function, the assignment operator
// '=' may result in memory reallocation.  This behavior is described in
// the next section.
//
//
// Assignment and Copying Data:
// ---------------------------
// The following member functions are provided for bulk data transfer from
// one matrix object into another existing object: = and copy.  Both
// functions attempt to preserve the source matrix's data subset control,
// and both set unused elements in the destination matrix to 0.  There is a
// major difference, however, between the two operations: = may reallocate
// memory for the receiving matrix, while copy leaves the sizes and modes of
// the receiving matrix unchanged.  Note that self assignment ( A = A ) and
// self copy ( A.copy(A) ) are not "do nothing" operations.
//
//
// A = B;          If A has insufficient memory to hold all of B's valid
//                 (data subset control) data, or if the index modes of A
//                 differ from those of B, reallocate the memory for A so
//                 that it has the same index modes as B, with sizes just
//                 sufficient to hold all of the valid data elements of B.
//                 Copy all of B's valid data elements into A, and set any
//                 remaining elements of A to 0. Set the data subset control
//                 for A so that its valid index ranges match the valid
//                 index ranges for B.  When finished, A will be a (possibly
//                 larger) copy of the valid data elements of B, with the
//                 same valid index ranges and index modes. Note that = only
//                 copies data within B's currently valid index range, so
//                 data subset control on B will reduce the number of
//                 elements copied.  If A and B are the SAME matrix, just
//                 perform A.clean().
//
// A = D;          Perform the above operator = using the real_matrix data
//                 in datafile object D. A must be a real_matrix. 
//
// A.copy(B);      Copy as many of the valid data elements of B into the
//                 corresponding elements of A as the memory allocation and
//                 indexing modes of A will allow.  In other words, copy
//                 the data elements with indexes in the intersection of
//                 the indexing ranges of A and B.  Only copies data within
//                 B's currently valid index range, so data subset control
//                 on B will reduce the number of elements copied. Ignores
//                 any data subset control previously set for A. Sets the
//                 remaining elements of A to 0, and sets A data subset
//                 control to match as closely as possible B's valid index
//                 range.  If A and B are the SAME matrix, just perform
//                 A.clean().
//
// As with the copy constructor, B may be either a real_matrix or a
// complex_matrix if A is of type complex_matrix; B must be of type
// real_matrix if A is of type real_matrix.  D is of type datafile (defined in
// datafile.h); for A = D, A must be of type real_matrix.
//
//
// Other Member Functions:
// ----------------------
// The remaining member functions defined for matrix objects are:
//
//
// A.fill(s);      Fill all elements in the valid index range of A with the
//                 scalar value s. The fill is subject to data subset
//                 control.
//
// A = s;          For scalar s, the same as A.fill(s). s may not be
//                 Complex if A is of type real_matrix.
//
// A.fillall(s);   Fill all physically allocated elements of A with the
//                 scalar value s. Ignores data subset control.
//
// A.fillrow(n,v); Fill row n of A using the contents of vector v. Only
//                 copies data into the valid index range of A from the
//                 corresponding elements in the valid index range of v;
//                 any valid elements of A without a corresponding valid
//                 element in v are set to 0.  If A is real, so must be v.
//
// A.fillcol(n,v); Fill column n of A using the contents of vector v. Only
//                 copies data into the valid index range of A from the
//                 corresponding elements in the valid index range of v;
//                 any valid elements of A without a corresponding valid
//                 element in v are set to 0.  If A is real, so must be v.
//
// A.clean();      Reset to 0 all elements of A beyond the valid index
//                 range as determined by data subset control.
//
// A.diagonal(s);  Set A.get(i,i) = s; for all valid diagonal elements of A.
//                 Sets all other elements of A to 0. Uses data subset
//                 control to determine the valid diagonal elements of A.
//
// A.rowswap(n,m); Swap rows n and m of A. Actually performs the swap by
//                 exchanging the values of the pointers stored in A[n]
//                 and A[m], so the execution is very fast.  If either
//                 index is outside the valid left index range, then fills
//                 the other row with zeroes.  If both are outside the
//                 valid left index range, then has no effect.
//
// A.apply(f);     Apply the scalar function f to each valid element of
//                 A. f() will be called once for each valid element of A.
//                 f must be a function taking either:
//                  (1) a single numeric argument of the same type as the
//                      data elements of A: f(double x) or f(Complex x).
//                  (2) a numeric argument and and two int arguments:
//                      f(double x,int i,int j) or f(Complex x,int i,int j)
//                 f() must return a single numeric value of the same type
//                 as the data elements of A. As apply() iterates through
//                 the vector elements, it calls f() with the value of each
//                 element and, if f() is of type (2) above, the value of
//                 the left index and the right index of the element.
//                 Equivalent to:
//
//                 for (int i = A.Lminindex(); i <= A.Lmaxindex(); ++i)
//                   for (int j = A.Rminindex(); j <= A.Rmaxindex(); ++j)
//                     A[i][j] = f(A[i][j]);
//                       /* OR */
//                     A[i][j] = f(A[i][j], i, j);
//
//
// Matrix Arithmetic Member Functions:
// ----------------------------------
//
//
// A += s;  A -= s;  A *= s;  A /= s;
//                 Perform the indicated assignment operator operation to
//                 each element of A within its valid index range, using
//                 data subset control.  The scalar s may not be Complex if
//                 A is a real_matrix.
//
// A += B; A -= B;  First perform A.clean(), then perform the indicated
//                 assignment operation as a vector addition or subtraction
//                 using only valid elements of B (data subset control on
//                 B).  Increases the data subset range of A if necessary
//                 so that its valid index range includes the valid index
//                 range of B as a subset, unless limited by the physical
//                 capacity of A. Does not reallocate any memory for A or
//                 change A's index modes. B must be a real_matrix if A is
//                 a real_matrix.
//
// A.add(B,C);     sets A = B + C (matrix sum), where zeroes are padded to
//                 rows and or columns of B and C to make them have equal
//                 index ranges for the purposes of the sum (B and C are
//                 not actually modified).  Only includes elements in the
//                 valid index ranges of B and C.  If A is not originally
//                 large enough to hold all elements of the result, it is
//                 reallocated. B and C must both be real_matrix if A is a
//                 real_matrix.
//
// A.sub(B,C);     sets A = B - C (matrix), otherwise like A.add(B,C).
//
//
// A.real(B);      sets each element of A equal to the real part of the
//                 corresponding element of complex_matrix B. Performs
//                 A.resize(B) first, so that it is just big enough to
//                 hold the valid elements of B.  A may be either a
//                 real_matrix or a complex_matrix; B must be a complex_
//                 matrix. (The resize is not performed if B is the same
//                 matrix as A)
//
// A.imaginary(B); sets each element of A equal to the imaginary part of
//                 the corresponding element of complex_matrix B. Performs
//                 A.resize(B) first, so that it is just big enough to
//                 hold the valid elements of B.  A may be either a
//                 real_matrix or a complex_matrix; B must be a complex_
//                 matrix. (The resize is not performed if B is the same
//                 matrix as A)
//
//
// Manipulating Matrix Rows as Vector Objects
// -----------------------------------------
// Because the *_vector classes include an alias constructor, and because
// each row of a matrix object is stored as a contiguous block of memory, it
// is easy to manipulate any row of a matrix as a true vector object.  Some
// examples follow (you must use the same data type for both the matrix and
// vector objects, ie: both of type real or type complex):
//
//
// complex_matrix A(10,7,Index_S,Index_1);
//           // each row of A is like a complex_vector(7,Index_1)
//
// complex_vector y(complex_vector(A[3], A.Rsize, A.Rmode));
//           // y is an independent copy of the data in row 3 of A
//
// complex_vector x(A[3], A.Rsize, A.Rmode);
//           // x aliases row 3 of A, with the same size and index mode.
//           // any changes to elements of x will result in changes to
//           // A, such as:
//
// x.copy(v);   // copy elements of v into x (and row 3 of A)
//
// x.copy(y);   // restores x (and row 3 of A) to its original values
//
//            
//
// Matrix Output/Display Functions:
// ------------------------------
//
// ostream << A;   send the contents of matrix A to the output stream ostream,
//                 using "ostream << v" to output each row in the valid
//                 index range of A as a vector v.  Multiple valid rows of
//                 A are separated by the C++ string whose value is controlled
//                 by the static member function out_separator(); the default
//                 value is a single newline character '\n'.  The value of
//                 ostream.width() is used to set the field width for each
//                 row output; it is not used for the field width of the row
//                 separator string. Note that << is not a member function of
//                 any class. See vector.h for details on how each row of
//                 output is formatted.
//
// A.show(ostream); output equivalent to:  ostream << endl << A << endl;
//
// A.show();       equivalent to A.show(cout);
//
// real_matrix::out_separator(s); or: complex_matrix::out_separator(s);
//                 either function sets the output separator string for
//                 both real_matrix and complex_matrix output. s may be a
//                 C++ string, a C-style string, or a single char. The
//                 function returns the previous value of the separator as
//                 a C++ string. If no argument is specified, then the
//                 function simply returns the current separator value. The
//                 default value is a single newline. If no output separator
//                 character is desired, use a null string "" for s.
//
// ************************************************************************

#ifndef MATRIX_H
#define MATRIX_H

#ifndef DEFAULT_MATRIX_SIZE
#define DEFAULT_MATRIX_SIZE   16   /* a default matrix size */
#endif  /* DEFAULT_MATRIX_SIZE */

#ifndef V_INDEX_MODE_DEFINED
#define  V_INDEX_MODE_DEFINED
enum v_index_mode { Index_C, Index_1, Index_S }; // same as in vectors.h
#endif /* V_INDEX_MODE_DEFINED */

#include "SIScmplx.h"
#include <iosfwd>
#include <string>

class real_vector;
class complex_vector;
class complex_matrix;
class datafile;

//-------------------------------------------------------------------------
class real_matrix {

public:

  // data (all are read-only aliases of private data members):

  const int     &Lsize;             // the size of the left index
  const int     &Rsize;             // the size of the right index
  const v_index_mode   &Lmode;      // the indexing mode for the left index
  const v_index_mode   &Rmode;      // the indexing mode for the right index


  // constructors:

  explicit real_matrix(const int n = DEFAULT_MATRIX_SIZE)
    : Lsize(internal_Lsize), Rsize(internal_Rsize), 
      Lmode(internal_Lmode), Rmode(internal_Rmode)
  { construct(n, n, Index_1, Index_1); constfill(0.0); }

  real_matrix(const int n, const v_index_mode t)
    : Lsize(internal_Lsize), Rsize(internal_Rsize), 
      Lmode(internal_Lmode), Rmode(internal_Rmode)
  { construct(n, n, t, t); constfill(0.0); }

  real_matrix(const int n, const int m)
    : Lsize(internal_Lsize), Rsize(internal_Rsize), 
      Lmode(internal_Lmode), Rmode(internal_Rmode)
  { construct(n, m, Index_1, Index_1); constfill(0.0); }

  real_matrix(const int n, const int m, const v_index_mode t)
    : Lsize(internal_Lsize), Rsize(internal_Rsize), 
      Lmode(internal_Lmode), Rmode(internal_Rmode)
  { construct(n, m, t, t); constfill(0.0); }

  real_matrix(const int n, const int m, 
	     const v_index_mode tl, const v_index_mode tr)
    : Lsize(internal_Lsize), Rsize(internal_Rsize), 
      Lmode(internal_Lmode), Rmode(internal_Rmode)
  { construct(n, m, tl, tr); constfill(0.0); }

  real_matrix(const real_matrix & B);
  real_matrix(const real_vector & v);
  real_matrix(const datafile & D);


  // destructor:

  ~real_matrix(void)
  { delete [] delete_pointer_data; delete [] delete_pointer_rows; }


  // data subset contol:

  inline int Lminindex(void) const  { return Lminindexvalue; }
  inline int Lmaxindex(void) const  { return Lmaxindexvalue; }
  int Lmaxindex(const int n);
  inline int Rminindex(void) const  { return Rminindexvalue; }
  inline int Rmaxindex(void) const  { return Rmaxindexvalue; }
  int Rmaxindex(const int n);
  inline int minindex(void) const
  {
    return (Lminindexvalue > Rminindexvalue) ?
      Lminindexvalue : Rminindexvalue;
  }
  inline int maxindex(void) const
  {
    return (Lmaxindexvalue < Rmaxindexvalue) ?
      Lmaxindexvalue : Rmaxindexvalue;
  }
  inline int maxindex(int n)
  { Lmaxindex(n); Rmaxindex(n); return maxindex(); }
  inline int maximize(void)
  { return maxindex(Lsize+Rsize); }
  inline int is_empty() const
  { return (Lmaxindex() < Lminindex())||(Rmaxindex() < Rminindex());}
  real_matrix & make_empty();


  // data access:

  inline double read(const int i, const int j) const
  { 
    if((i > Lmaxindex())||(j > Rmaxindex())||
       (i < Lminindex())||(j < Rminindex())  )
      return 0.0;
    else 
      return data[i][j];
  }

  inline double & get(const int i, const int j)
  { 
    if((i > Lmaxindex())||(j > Rmaxindex())||
       (i < Lminindex())||(j < Rminindex())  )
      return (trash = 0.0);
    else 
      return data[i][j];
  }

  inline const double & get(const int i, const int j) const
  { 
    if((i > Lmaxindex())||(j > Rmaxindex())||
       (i < Lminindex())||(j < Rminindex())  )
      return (trash = 0.0);
    else 
      return data[i][j];
  }

  inline double *const & operator [] (const int i)
  { return data[i]; }

  inline const double *const & operator [] (const int i) const
  {
    return *( static_cast<double const * const *>(data+i) );
  }


  // change the index modes:

  real_matrix & reindex(const v_index_mode, const v_index_mode);


  // reallocation:

  real_matrix & reallocate(const int n, const int m,
			  const v_index_mode tl, const v_index_mode tr);
  inline real_matrix & reallocate(const int n)  
  { return reallocate(n, n, Lmode, Rmode); }
  inline real_matrix & reallocate(const int n, const int m)
  { return reallocate(n, m, Lmode, Rmode); }
  inline real_matrix & resize(const int Lmax, const int Rmax)
  {
    int newLsize = (Lmode == Index_C) ? Lmax+1: Lmax;
    int newRsize = (Rmode == Index_C) ? Rmax+1: Rmax;
    return reallocate(newLsize, newRsize);
  }
  inline real_matrix & resize(const int Max) { return resize(Max, Max); }
  inline real_matrix & resize(const real_matrix & B)
  {
    int newLsize = (B.Lmode == Index_C) ? B.Lmaxindex()+1: B.Lmaxindex();
    int newRsize = (B.Rmode == Index_C) ? B.Rmaxindex()+1: B.Rmaxindex();
    return reallocate(newLsize, newRsize, B.Lmode, B.Rmode);
  }
  real_matrix & resize(const complex_matrix & B);


  // copy and assignment:

  real_matrix & copy(const real_matrix & B);
  real_matrix & operator = (const real_matrix & B);
  real_matrix & operator = (const datafile & D);


  // other member functions:

  real_matrix & clean(void);
  inline real_matrix & fill(const double s) { constfill(s); return *this; }
  inline real_matrix & operator = (const double s) { return fill(s); }
  real_matrix & fillall(const double s);
  real_matrix & fillrow(const int n, const real_vector & v);
  real_matrix & fillcol(const int n, const real_vector & v);
  real_matrix & diagonal(const double s);
  real_matrix & rowswap(const int n, const int m);
  real_matrix & apply(double (*f)(double));
  real_matrix & apply(double (*f)(const double &));
  real_matrix & apply(double (*f)(double, int, int));
  real_matrix & apply(double (*f)(const double &, int, int));

  // matrix arithmetic member functions:

  real_matrix & operator += (const double s);
  real_matrix & operator -= (const double s);
  real_matrix & operator *= (const double s);
  real_matrix & operator /= (const double s);
  real_matrix & operator += (const real_matrix & B);
  real_matrix & operator -= (const real_matrix & B);

  real_matrix & add(const real_matrix & B, const real_matrix & C);
  real_matrix & sub(const real_matrix & B, const real_matrix & C);
  real_matrix & real(const complex_matrix & B);
  real_matrix & imaginary(const complex_matrix & B);

  // matrix output member functions:

  real_matrix & show(std::ostream &);
  real_matrix & show(void);
  const real_matrix & show(std::ostream &) const;
  const real_matrix & show(void) const;
  static std::string out_separator(); // return the current output separator
  static std::string out_separator(const std::string &);
  static std::string out_separator(const char * const);
  static std::string out_separator(const char);


private:

  int            internal_Lsize;    // aliased by Lsize
  int            internal_Rsize;    // aliased by Rsize
  v_index_mode   internal_Lmode;    // aliased by Lmode
  v_index_mode   internal_Rmode;    // aliased by Rmode
  int            Lmaxindexvalue;    // the left index upper limit
  int            Lminindexvalue;    // the left index lower limit
  int            Rmaxindexvalue;    // the right index upper limit
  int            Rminindexvalue;    // the right index lower limit
  double     * * data;              // points to pointer to [0][0]
  double     * * delete_pointer_data; // used by destructor to delete memory
  double       * delete_pointer_rows; // used by destructor to delete memory
  mutable double trash;             // Where index-out-of-bounds wind up
  double       * trashptr;          // Will point to trash ico alloc failure

  // these functions are called by other member functions:
  void construct(const int n, const int m,
		 const v_index_mode tl,
		 const v_index_mode tr);  //allocate memory
  void constfill(const double f); // fill with a constant value

};  // class real_matrix 

std::ostream & operator << (std::ostream &, const real_matrix &);

//-------------------------------------------------------------------------

class complex_matrix {

public:

  // data (all are read-only aliases of private data members):

  const int     &Lsize;             // the size of the left index
  const int     &Rsize;             // the size of the right index
  const v_index_mode   &Lmode;      // the indexing mode for the left index
  const v_index_mode   &Rmode;      // the indexing mode for the right index


  // constructors:

  explicit complex_matrix(const int n = DEFAULT_MATRIX_SIZE)
    : Lsize(internal_Lsize), Rsize(internal_Rsize), 
      Lmode(internal_Lmode), Rmode(internal_Rmode)
  { construct(n, n, Index_1, Index_1); constfill(0.0); }

  complex_matrix(const int n, const v_index_mode t)
    : Lsize(internal_Lsize), Rsize(internal_Rsize), 
      Lmode(internal_Lmode), Rmode(internal_Rmode)
  { construct(n, n, t, t); constfill(0.0); }

  complex_matrix(const int n, const int m)
    : Lsize(internal_Lsize), Rsize(internal_Rsize), 
      Lmode(internal_Lmode), Rmode(internal_Rmode)
  { construct(n, m, Index_1, Index_1); constfill(0.0); }

  complex_matrix(const int n, const int m, const v_index_mode t)
    : Lsize(internal_Lsize), Rsize(internal_Rsize), 
      Lmode(internal_Lmode), Rmode(internal_Rmode)
  { construct(n, m, t, t); constfill(0.0); }

  complex_matrix(const int n, const int m, 
	     const v_index_mode tl, const v_index_mode tr)
    : Lsize(internal_Lsize), Rsize(internal_Rsize), 
      Lmode(internal_Lmode), Rmode(internal_Rmode)
  { construct(n, m, tl, tr); constfill(0.0); }

  complex_matrix(const complex_matrix & B);
  complex_matrix(const real_matrix & B);
  complex_matrix(const real_vector & v);
  complex_matrix(const complex_vector & v);

  // destructor:

  ~complex_matrix(void)
  { delete [] delete_pointer_data; delete [] delete_pointer_rows; }


  // data subset contol:

  inline int Lminindex(void) const  { return Lminindexvalue; }
  inline int Lmaxindex(void) const  { return Lmaxindexvalue; }
  int Lmaxindex(const int n);
  inline int Rminindex(void) const  { return Rminindexvalue; }
  inline int Rmaxindex(void) const  { return Rmaxindexvalue; }
  int Rmaxindex(const int n);
  inline int minindex(void) const
  {
    return (Lminindexvalue > Rminindexvalue) ?
      Lminindexvalue : Rminindexvalue;
  }
  inline int maxindex(void) const
  {
    return (Lmaxindexvalue < Rmaxindexvalue) ?
      Lmaxindexvalue : Rmaxindexvalue;
  }
  inline int maxindex(int n)
  { Lmaxindex(n); Rmaxindex(n); return maxindex(); }
  inline int maximize(void)
  { return maxindex(Lsize+Rsize); }
  inline int is_empty() const
  { return (Lmaxindex() < Lminindex())||(Rmaxindex() < Rminindex());}
  complex_matrix & make_empty();


  // data access:

  inline Complex read(const int i, const int j) const
  { 
    if((i > Lmaxindex())||(j > Rmaxindex())||
       (i < Lminindex())||(j < Rminindex())  )
      return 0.0;
    else 
      return data[i][j];
  }

  inline Complex & get(const int i, const int j)
  { 
    if((i > Lmaxindex())||(j > Rmaxindex())||
       (i < Lminindex())||(j < Rminindex())  )
      return (trash = 0.0);
    else 
      return data[i][j];
  }

  inline const Complex & get(const int i, const int j) const
  { 
    if((i > Lmaxindex())||(j > Rmaxindex())||
       (i < Lminindex())||(j < Rminindex())  )
      return (trash = 0.0);
    else 
      return data[i][j];
  }

  inline Complex *const & operator [] (const int i)
  { return data[i]; }

  inline const Complex *const & operator [] (const int i) const
  {
    // These extra steps are necessary to work around a compiler bug
    // in gcc version 2.96 20000731 (Red Hat Linux 7.1 2.96-81)
    // return data[i];
    Complex const * const * p = data + i;
    return *p;
  }


  // change the index modes:

  complex_matrix & reindex(const v_index_mode, const v_index_mode);


  // reallocation:

  complex_matrix & reallocate(const int n, const int m,
			     const v_index_mode tl, const v_index_mode tr);
  inline complex_matrix & reallocate(const int n)  
  { return reallocate(n, n, Lmode, Rmode); }
  inline complex_matrix & reallocate(const int n, const int m)
  { return reallocate(n, m, Lmode, Rmode); }
  inline complex_matrix & resize(const int Lmax, const int Rmax)
  {
    int newLsize = (Lmode == Index_C) ? Lmax+1: Lmax;
    int newRsize = (Rmode == Index_C) ? Rmax+1: Rmax;
    return reallocate(newLsize, newRsize);
  }
  inline complex_matrix & resize(const int Max) 
  { return resize(Max, Max); }
  inline complex_matrix & resize(const complex_matrix & B)
  {
    int newLsize = (B.Lmode == Index_C) ? B.Lmaxindex()+1: B.Lmaxindex();
    int newRsize = (B.Rmode == Index_C) ? B.Rmaxindex()+1: B.Rmaxindex();
    return reallocate(newLsize, newRsize, B.Lmode, B.Rmode);
  }
  inline complex_matrix & resize(const real_matrix & B)
  {
    int newLsize = (B.Lmode == Index_C) ? B.Lmaxindex()+1: B.Lmaxindex();
    int newRsize = (B.Rmode == Index_C) ? B.Rmaxindex()+1: B.Rmaxindex();
    return reallocate(newLsize, newRsize, B.Lmode, B.Rmode);
  }

  // copy and assignment:

  complex_matrix & copy(const complex_matrix & B);
  complex_matrix & copy(const real_matrix & B);
  complex_matrix & operator = (const complex_matrix & B);
  complex_matrix & operator = (const real_matrix & B);


  // other member functions:

  complex_matrix & clean(void);
  inline complex_matrix & fill(const Complex s) { constfill(s); return *this; }
  inline complex_matrix & operator = (const Complex s) { return fill(s); }
  inline complex_matrix & operator = (const double s) 
  { return fill(Complex(s)); }
  complex_matrix & fillall(const Complex s);
  complex_matrix & fillrow(const int n, const real_vector & v);
  complex_matrix & fillrow(const int n, const complex_vector & v);
  complex_matrix & fillcol(const int n, const real_vector & v);
  complex_matrix & fillcol(const int n, const complex_vector & v);
  complex_matrix & diagonal(const Complex s);
  complex_matrix & rowswap(const int n, const int m);
  complex_matrix & apply(Complex (*f)(Complex));
  complex_matrix & apply(Complex (*f)(const Complex &));
  complex_matrix & apply(Complex (*f)(Complex, int, int));
  complex_matrix & apply(Complex (*f)(const Complex &, int, int));

  // matrix arithmetic member functions:

  complex_matrix & operator += (const Complex s);
  complex_matrix & operator -= (const Complex s);
  complex_matrix & operator *= (const Complex s);
  complex_matrix & operator /= (const Complex s);
  complex_matrix & operator += (const complex_matrix & B);
  complex_matrix & operator -= (const complex_matrix & B);
  complex_matrix & operator += (const real_matrix & B);
  complex_matrix & operator -= (const real_matrix & B);

  complex_matrix & add(const complex_matrix & B, const complex_matrix & C);
  complex_matrix & add(const complex_matrix & B, const real_matrix & C);
  inline complex_matrix & add(const real_matrix & B, const complex_matrix & C)
  { return add(C,B); }
  complex_matrix & add(const real_matrix & B, const real_matrix & C);
  complex_matrix & sub(const complex_matrix & B, const complex_matrix & C);
  complex_matrix & sub(const complex_matrix & B, const real_matrix & C);
  complex_matrix & sub(const real_matrix & B, const complex_matrix & C);
  complex_matrix & sub(const real_matrix & B, const real_matrix & C);
  complex_matrix & real(const complex_matrix & B);
  complex_matrix & imaginary(const complex_matrix & B);

  // matrix output member functions:

  complex_matrix & show(std::ostream &);
  complex_matrix & show(void);
  const complex_matrix & show(std::ostream &) const;
  const complex_matrix & show(void) const;
  static std::string out_separator(); // return the current output separator
  static std::string out_separator(const std::string &);
  static std::string out_separator(const char * const);
  static std::string out_separator(const char);


private:

  int            internal_Lsize;     // aliased by Lsize
  int            internal_Rsize;     // aliased by Rsize
  v_index_mode   internal_Lmode;     // aliased by Lmode
  v_index_mode   internal_Rmode;     // aliased by Rmode
  int            Lmaxindexvalue;     // the left index upper limit
  int            Lminindexvalue;     // the left index lower limit
  int            Rmaxindexvalue;     // the right index upper limit
  int            Rminindexvalue;     // the right index lower limit
  Complex     * * data;              // points to pointer to [0][0]
  Complex     * * delete_pointer_data; // used by destructor to delete memory
  Complex       * delete_pointer_rows; // used by destructor to delete memory
  mutable Complex trash;             // Where index-out-of-bounds wind up
  Complex       * trashptr;          // Will point to trash ico alloc failure

  // these functions are called by other member functions:
  void construct(const int n, const int m,
		 const v_index_mode tl,
		 const v_index_mode tr);  //allocate memory
  void constfill(const Complex f); // fill with a constant value

};  // class complex_matrix 

std::ostream & operator << (std::ostream &, const complex_matrix &);

//-------------------------------------------------------------------------

typedef real_matrix real_table;
typedef complex_matrix complex_table;

#endif  /* MATRIX_H */
