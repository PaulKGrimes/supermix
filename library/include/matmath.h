// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ************************************************************************
// matmath.h
// math routines and the numeric vector and table classes
//
// Defines the following macros:
//   DEFAULT_VECTOR_SIZE 0
//   DEFAULT_TABLE_SIZE  0
//
// Defines the following typedefs:
//
//   Matrix    same as complex_table
//   Vector    same as complex_vector
//  
// F. Rice 11/8/97
// ************************************************************************
//
//			   The mathmath Routines
//
// This header file defines many math routines for the vector and matrix
// classes which make these classes truly useful for the SuperMix program.
//
// In addition, the default matrix and vector sizes are set to 0, and there
// are typedefs which alias the complex_matrix and complex_vector types to
// Matrix and Vector, respectively. Because the default index mode was
// changed to Index_1 with the versions of these classes later than
// 11/8/97, declaring objects like:
//
//	Matrix A;  Matrix B(n);
//      Vector v;  Vector y(n);
//
// will provide objects similar to John Ward's matrix and complexArray
// types, i.e. with initial size 0 or n and indexing starting at 1.
//
// ************************************************************************
//
//			  Routine Quick Reference
//
// Vector operations:
// -----------------
// (x, y: real_vectors;  u, v: complex_vectors;  s: double; z: complex)
//           (a, b: either real_vectors or complex_vectors)
//   (the return type is denoted like: <type> at the end of each line)
//
// The following return Rvalue vector objects:
//
//  conj(u);  <complex_vector>
//  conj(x);  <real_vector>
//  real(u);  imaginary(u);  <real_vector>
//
//  -x;     +x;     -u;     +u;
//
//  x + y;  x - y;  <real_vector>
//  a + u;  a - u;  u + a;  u - a;                  <complex_vector>
//
//  x + s;  x - s;  x * s;  x / s;  s + x;  s * x;  <real_vector>
//  a + z;  a - z;  a * z;  a / z;  z + a;  z * a;  <complex_vector>
//
//  scalemult(x,y); scalediv(x,y);   <real_vector>
//  scalemult(u,a); scalemult(a,u);  <complex_vector>
//  scalediv(u,a);  scalediv(a,u);   <complex_vector>
//
//  scalemult() and scalediv() return a vector whose elements are the products
//  or ratios, respectively, of the corresponding elements of the argument
//  vectors. These functions can be used to simutaneously apply custom scaling
//  to indiviual elements of a vector and return the result. 
//  
//
// The following return Rvalue scalar objects:
//
//  x * y;  <double>
//  x * u;  u * x;  u * v;  <Complex>
//
//  dot(x,y);  <double>  (this is equivalent to: x * y)
//  dot(x,v);  <Complex> (this is equivalent to: x * v)
//  dot(u,v);  <Complex> (this is equivalent to: conj(u)*v)
//  dot(u,x);  <Complex> (this is equivalent to: conj(u)*x)
//
//  norm(x); norm(u); <double>  returns dot(x,x) or dot(u,u)
//  max_norm(x); max_norm(u); <double> examines the magnitude
//   squared of each element and returns the largest found
//
//
// Matrix operations:
// ----------------
// (X, Y: real_matrices;  U, V: complex_matrices;  s: double; z: complex)
// (the return type is denoted like: <type> at the end of each line)
//
// The following return Rvalue matrix objects:
//
//  identity_matrix(n); identity_matrix(n,mode);  <real_matrix>
//    (square identity matrix)
//    (n is an int size, mode is a v_index_mode value; default is Index_1)
//  identity_matrix(X); identity_matrix(U);       <real_matrix>
//    (return identity matrix with same size and mode as argument)
//
//  conj(U);  transpose(U);  dagger(U);  <complex_matrix>
//  conj(X);  transpose(X);  dagger(X);  <real_matrix>
//  real(U);  imaginary(U);  <real_matrix>
//
//  -X;     +X;     -U;     +U;
//
//  X + Y;  X - Y;  X * Y;  <real_matrix>
//  X + U;  X - U;  U + X;  U - X;  U + V;  U - V;  <complex_matrix>
//  X * U;  U * X;  U * V;  <complex_matrix>                 
//
//  X + s;  X - s;  X * s;  X / s;  s + X;  s * X;  <real_matrix>
//  X + z;  X - z;  X * z;  X / z;  z + X;  z * X;  <complex_matrix>
//  U + z;  U - z;  U * z;  U / z;  z + U;  z * U;  <complex_matrix>
//
//  solve(X,Y);  inverse(X);  <real_matrix>
//  solve(U,V);  inverse(U);  <complex_matrix>
//  ( solve(A,B) returns the solution X to: A*X == B )
//  ( the returned matrix will have 0 size if a solution is impossible )
//  ( inverse() uses solve() to invert the argument matrix )
//
// The following return a scalar (double) Rvalue:
//
//  norm(X); norm(U); <double>
//    returns the sum of the magnitude squared's of all elements in the valid
//    index range of the matrix argument.
//
//  max_norm(X); max_norm(U); <double>
//    calculates the magnitude squared for each element in the matrix and
//    returns the largest one.
//
// The following returns a scalar index:
//
//  lookup(s,X,n);       lookup(s,U,n);   <int>
//    returns the index for the largest element in row n of X or U whose
//    real part is less than or equal to s. Returns Rminindex() if all
//    elements have real parts larger than s or if the matrix is empty.
//    Row n must be sorted from smallest to largest real part, or returns
//    garbage. Ignores the imaginary part of row n if using a
//    complex_matrix. s is double. If row n doesn't exist for the matrix,
//    lookup treats it as a row full of zeroes.
//
// The following returns a single interpolated value (scalar):
//
//  linterp(s,X,nx,ny) <double>;  linterp(s,U,nx,ny) <complex>;
//    returns y(s), where y is calculated by linear interpolation into matrix
//    X or U, where sorted s values are in row nx, with their corresponding
//    y values in row ny.  Performs linear extrapolation if s is beyond the
//    value limits in row nx. Uses lookup(). Only considers the real part of
//    row nx when doing the lookup. s is double. Returns garbage if the
//    matrix is empty, not sorted, or doesn't contain row nx or ny.
//
// The following returns a vector of interpolated values:
//
//  linterp(s,X,n) <real_vector>;  linterp(s,U,n) <complex_vector>;
//    returns y(s), where y is a vector calculated by linear interpolation
//    in matrix X or U, where sorted s values are in row n, with their
//    corresponding y values in the other rows.  Performs linear extrapolation
//    if s is beyond the matrix limits. Uses lookup(). Only considers the
//    real part of row n when doing the lookup. s is double. The returned
//    vector will have size and index mode determined by the left index of the
//    matrix; it will contain s in element n, and interpolated values in all
//    other elements from the corresponding rows of the matrix.  Returns
//    garbage if the matrix is empty, not sorted, or doesn't contain row n. 
//
// The following changes the matrix argument:
//
//  scalerow(n,X,y) <real_matrix>; scalerow(n,U,v) <complex_matrix>;
//    multiply each element of row n of the matrix (X or U) by the scalar
//    value (y or v). The source matrix is actually modified; returns a
//    reference to the matrix.
//
//
// Mixed Vector and Matrix operations:
// ---------------------------------
// (x, y: real_vectors;  u, v: complex_vectors;  X, Y: real_matrices;
//  U, V: complex_matrices;  s: double;  z: complex;  n: int)
// (the return type is denoted like: <type> at the end of each line)
//
//  row(n,X);  col(n,X);  <real_vector>
//  row(n,U);  col(n,U);  <real_vector>
//  (note: for the above, the index mode of the resulting vector will be
//   the same as that of the matrix argument in the direction selected, ie:
//   row: Rmode, col: Lmode)
//
//  rowmatrix(x);  columnmatrix(x);  <real_matrix>
//  rowmatrix(u);  columnmatrix(u);  <complex_matrix>
//  (note: for the above, the index mode of the resulting matrix will be
//   the same as that of the vector argument in its direction, and Index_1
//   in the other direction, with size 1 in that direction.)
//
//  X * y;  <real_vector> ( equivalent to: col(1, X * columnmatrix(y)) )
//  y * X;  <real_vector> ( equivalent to: row(1, rowmatrix(y) * X) )
//
//  U * y;  X * v;  U * v;  y * U;  v * X;  v * U;  <complex_vector>
//
//  solve(X,y);  <real_vector> ( equiv: col(1, solve(X, columnmatrix(y))) )
//  solve(U,v);  <complex_matrix>
//
//
// Special Fast Square Matrix Operations:
// -------------------------------------
// These routines assume that all argument matrices are square and are
// already allocated to have the same index ranges and index modes.
// If these requirements are not met, the results are undefined (and
// probably catastrophic).
//
// IplusM(U, s, V);   U = s*I + V after the call. I is the identity.
// IminusM(U, s, V);  U = s*I - V after the call. I is the identity.
//
// MMdagger(U, V);     U = V * dagger(V)  after the call.
// MAMdagger(U, V, W); U = V * W * dagger(V)
//
// ************************************************************************

#ifndef MATMATH_H
#define MATMATH_H

#ifndef DEFAULT_VECTOR_SIZE
#define DEFAULT_VECTOR_SIZE   0
#endif  /* DEFAULT_VECTOR_SIZE */

#ifndef DEFAULT_MATRIX_SIZE
#define DEFAULT_MATRIX_SIZE   0
#endif  /* DEFAULT_MATRIX_SIZE */

#include "vector.h"
#include "table.h"


typedef complex_vector Vector;
typedef complex_matrix  Matrix;

// ************************************************************************
// Vector Routines:

// Complex conjugation:
inline complex_vector conj(const complex_vector &u)
{ return complex_vector(u).apply(conj); }
inline real_vector conj(const real_vector &x)
{ return x; }

// Real or complex part:
inline real_vector real(const complex_vector &u)
{ return real_vector().real(u); }
inline real_vector imaginary(const complex_vector &u)
{ return real_vector().imaginary(u); }

// Unary +,-:
inline complex_vector operator +(const complex_vector & u) { return u; }
inline    real_vector operator +(const    real_vector & x) { return x; }
inline complex_vector operator -(const complex_vector & u)
{ return complex_vector(u) *= -1; }
inline    real_vector operator -(const    real_vector & x)
{ return real_vector(x) *= -1; }

// Vector addition, subtraction:
complex_vector operator +(const complex_vector &, const complex_vector &);
complex_vector operator +(const    real_vector &, const complex_vector &);
   real_vector operator +(const    real_vector &, const    real_vector &);
inline complex_vector operator +(const complex_vector &u, const real_vector &x)
{ return x + u; }

complex_vector operator -(const complex_vector &, const complex_vector &);
complex_vector operator -(const    real_vector &, const complex_vector &);
complex_vector operator -(const complex_vector &, const    real_vector &);
   real_vector operator -(const    real_vector &, const    real_vector &);

// scalemult(), scalediv():
complex_vector scalemult(const complex_vector &, const complex_vector &);
complex_vector scalemult(const    real_vector &, const complex_vector &);
   real_vector scalemult(const    real_vector &, const    real_vector &);
inline complex_vector scalemult(const complex_vector &u, const real_vector &x)
{ return scalemult(x,u); }

complex_vector scalediv(const complex_vector &, const complex_vector &);
complex_vector scalediv(const    real_vector &, const complex_vector &);
complex_vector scalediv(const complex_vector &, const    real_vector &);
   real_vector scalediv(const    real_vector &, const    real_vector &);
 
// Inner Product, Dot Product:
Complex operator *(const complex_vector &, const complex_vector &);
Complex operator *(const    real_vector &, const complex_vector &);
 double operator *(const    real_vector &, const    real_vector &);
inline Complex operator *(const complex_vector &u, const real_vector &x)
{ return x * u; }
inline Complex dot(const complex_vector &u, const complex_vector &v)
{ return conj(u) * v; }
inline Complex dot(const complex_vector &u, const real_vector &x)
{ return x * conj(u); }
inline Complex dot(const real_vector &x, const complex_vector &v)
{ return x * v; }
inline double dot(const real_vector &x, const real_vector &y)
{ return x * y; }

// Operate a Vector with a Scalar:
complex_vector operator +(const complex_vector &, const Complex);
complex_vector operator +(const    real_vector &, const Complex);
   real_vector operator +(const    real_vector &, const  double);
inline complex_vector operator +(const complex_vector & u, const double s)
{ return u + Complex(s); }
inline complex_vector operator +(const Complex s, const complex_vector &v)
{ return v + s; }
inline complex_vector operator +(const double s, const complex_vector &v)
{ return v + Complex(s); }
inline complex_vector operator +(const Complex s, const real_vector &v)
{ return v + s; }
inline    real_vector operator +(const  double s, const real_vector &v)
{ return v + s; }

complex_vector operator -(const complex_vector &, const Complex);
complex_vector operator -(const    real_vector &, const Complex);
   real_vector operator -(const    real_vector &, const  double);
inline complex_vector operator -(const complex_vector & u, const double s)
{ return u - Complex(s); }

complex_vector operator *(const complex_vector &, const Complex);
complex_vector operator *(const    real_vector &, const Complex);
   real_vector operator *(const    real_vector &, const  double);
inline complex_vector operator *(const complex_vector & u, const double s)
{ return u * Complex(s); }
inline complex_vector operator *(const Complex s, const complex_vector &v)
{ return v * s; }
inline complex_vector operator *(const double s, const complex_vector &v)
{ return v * Complex(s); }
inline complex_vector operator *(const Complex s, const real_vector &v)
{ return v * s; }
inline    real_vector operator *(const  double s, const real_vector &v)
{ return v * s; }

complex_vector operator /(const complex_vector &, const Complex);
complex_vector operator /(const    real_vector &, const Complex);
   real_vector operator /(const    real_vector &, const  double);
inline complex_vector operator /(const complex_vector & u, const double s)
{ return u / Complex(s); }

// norms:
inline double norm(const real_vector & x)
{ return x*x; }
inline double norm(const complex_vector & u)
{ return dot(u,u).real; }
double max_norm(const real_vector &);
double max_norm(const complex_vector &);


// ************************************************************************
// Matrix Routines:

// Identity matrix:
inline real_matrix identity_matrix(const int n, const v_index_mode t = Index_1)
{ return real_matrix(n,t).diagonal(1.0); }
inline real_matrix identity_matrix(const real_matrix & M)
{ return real_matrix(0).resize(M).diagonal(1.0); }
inline real_matrix identity_matrix(const complex_matrix & M)
{ return real_matrix(0).resize(M).diagonal(1.0); }

// Transpose and Complex Conjugation:
inline complex_matrix conj(const complex_matrix &U)
{ return complex_matrix(U).apply(conj); }
inline real_matrix conj(const real_matrix &X)
{ return X; }
complex_matrix transpose(const complex_matrix &U);
real_matrix transpose(const real_matrix &X);
inline complex_matrix dagger(const complex_matrix &U)
{ return transpose(conj(U)); }
inline real_matrix dagger(const real_matrix &X)
{ return transpose(X); }

// Real or complex part:
inline real_matrix real(const complex_matrix &U)
{ return real_matrix().real(U); }
inline real_matrix imaginary(const complex_matrix &U)
{ return real_matrix().imaginary(U); }

// Unary +,-:
inline complex_matrix operator +(const complex_matrix & U) { return U; }
inline    real_matrix operator +(const    real_matrix & X) { return X; }
inline complex_matrix operator -(const complex_matrix & U)
{ return complex_matrix(U) *= -1; }
inline    real_matrix operator -(const    real_matrix & X)
{ return real_matrix(X) *= -1; }

// Matrix Addition, Subtraction, Multiplication:
complex_matrix operator +(const complex_matrix &, const complex_matrix &);
complex_matrix operator +(const    real_matrix &, const complex_matrix &);
complex_matrix operator +(const complex_matrix &, const    real_matrix &);
   real_matrix operator +(const    real_matrix &, const    real_matrix &);
complex_matrix operator -(const complex_matrix &, const complex_matrix &);
complex_matrix operator -(const    real_matrix &, const complex_matrix &);
complex_matrix operator -(const complex_matrix &, const    real_matrix &);
   real_matrix operator -(const    real_matrix &, const    real_matrix &);
complex_matrix operator *(const complex_matrix &, const complex_matrix &);
complex_matrix operator *(const    real_matrix &, const complex_matrix &);
complex_matrix operator *(const complex_matrix &, const    real_matrix &);
   real_matrix operator *(const    real_matrix &, const    real_matrix &);

// Operate a Matrix with a Scalar:
complex_matrix operator +(const complex_matrix &, const Complex);
complex_matrix operator +(const    real_matrix &, const Complex);
   real_matrix operator +(const    real_matrix &, const  double);
inline complex_matrix operator +(const complex_matrix & A, const double s)
{ return A + Complex(s); }
inline complex_matrix operator +(const Complex s, const complex_matrix &A)
{ return A + s; }
inline complex_matrix operator +(const double s, const complex_matrix &A)
{ return A + Complex(s); }
inline complex_matrix operator +(const Complex s, const real_matrix &A)
{ return A + s; }
inline    real_matrix operator +(const  double s, const real_matrix &A)
{ return A + s; }

complex_matrix operator -(const complex_matrix &, const Complex);
complex_matrix operator -(const    real_matrix &, const Complex);
   real_matrix operator -(const    real_matrix &, const  double);
inline complex_matrix operator -(const complex_matrix & A, const double s)
{ return A - Complex(s); }

complex_matrix operator *(const complex_matrix &, const Complex);
complex_matrix operator *(const    real_matrix &, const Complex);
   real_matrix operator *(const    real_matrix &, const  double);
inline complex_matrix operator *(const complex_matrix & A, const double s)
{ return A * Complex(s); }
inline complex_matrix operator *(const Complex s, const complex_matrix &A)
{ return A * s; }
inline complex_matrix operator *(const double s, const complex_matrix &A)
{ return A * Complex(s); }
inline complex_matrix operator *(const Complex s, const real_matrix &A)
{ return A * s; }
inline    real_matrix operator *(const  double s, const real_matrix &A)
{ return A * s; }

complex_matrix operator /(const complex_matrix &, const Complex);
complex_matrix operator /(const    real_matrix &, const Complex);
   real_matrix operator /(const    real_matrix &, const  double);
inline complex_matrix operator /(const complex_matrix & A, const double s)
{ return A / Complex(s); }

// Lookup and interpolation:
int lookup(const double, const real_matrix &, const int);
int lookup(const double, const complex_matrix &, const int, const int);
double linterp(const double, const real_matrix &, const int, const int); 
complex linterp(const double, const complex_matrix &, const int, const int); 
real_vector linterp(const double, const real_matrix &, const int);
complex_vector linterp(const double, const complex_matrix &, const int);

// norms:
double norm(const real_matrix &);
double norm(const complex_matrix &);
double max_norm(const real_matrix &);
double max_norm(const complex_matrix &);

// scalerow:
real_matrix & scalerow(const int, real_matrix &, const double);
complex_matrix & scalerow(const int, complex_matrix &, const Complex);

// ************************************************************************
// Matrix Solvers:

// Solves AX == B and returns X.  If it's unable to solve the system,
// then X will be empty.

   real_matrix solve(const    real_matrix & A, const    real_matrix & B);
complex_matrix solve(const complex_matrix & A, const complex_matrix & B);


// Uses solve() to find the inverse of a matrix. If the inverse doesn't
// exist, then the returned matrix will be empty. Note: the index modes and
// valid index ranges of A must be the same in both dimensions for an
// inverse to exist (A is square).

   real_matrix inverse(const    real_matrix & A);
complex_matrix inverse(const complex_matrix & A);


// Solvers with a vector RHS:

   real_vector solve(const    real_matrix & A, const    real_vector & b);
complex_vector solve(const complex_matrix & A, const complex_vector & b);

// ************************************************************************
// Mixed Vector and Matrix Routines:


real_vector row(const int n, const real_matrix & X);
real_vector col(const int n, const real_matrix & X);
complex_vector row(const int n, const complex_matrix & U);
complex_vector col(const int n, const complex_matrix & U);

inline real_matrix columnmatrix(const real_vector & x)
{ return real_matrix(x); }
inline real_matrix rowmatrix(const real_vector & x)
{ return transpose(real_matrix(x)); }
inline complex_matrix columnmatrix(const complex_vector & x)
{ return complex_matrix(x); }
inline complex_matrix rowmatrix(const complex_vector & x)
{ return transpose(complex_matrix(x)); }

real_vector operator *(const real_matrix &, const real_vector &);
inline real_vector operator *(const real_vector & x, const real_matrix & Y)
{ return transpose(Y) * x; }

complex_vector operator *(const complex_matrix &, const complex_vector &);
complex_vector operator *(const complex_matrix &, const real_vector &);
complex_vector operator *(const real_matrix &, const complex_vector &);
inline complex_vector operator *
(const complex_vector & u, const complex_matrix & V)
{ return transpose(V) * u; }
inline complex_vector operator *
(const real_vector & x, const complex_matrix & V)
{ return transpose(V) * x; }
inline complex_vector operator *
(const complex_vector & u, const real_matrix & Y)
{ return transpose(Y) * u; }

// ************************************************************************
// Special Fast Square Matrix Operations:

void IplusM(Matrix & U, double s, const Matrix & V);
void IminusM(Matrix & U, double s, const Matrix & V);
void MMdagger(Matrix & U, const Matrix & V);
void MAMdagger(Matrix & U, const Matrix & V, const Matrix & W);

// ************************************************************************
#endif  /* MATMATH_H */
