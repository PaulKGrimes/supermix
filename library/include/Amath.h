// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ********************************************************************
// Amath.h - fast c-style array routines for Complex and double
// ********************************************************************
//                         The Amath Routines
//
// These routines provide fast manipulation of c-style arrays for use
// in numerical routines.  They require that a length of the arrays be
// included as a parameter, and assume that the source and destination
// arrays are each at least this long.
//
// Cautions:
//  The source and destination arrays must not overlap.
//  The length must be nonnegative.
//
// The Amath routines do no validity checks, so violating either of
// the above restrictions will result in unpredictable behavior.
// The array addresses must be that of the first (ie, lowest address
// value) elements of the arrays.
//
// The result of each routine is overwritten into the first array
// argument (except for Adot).
//
// ********************************************************************
// In the following list:
//  c denotes type Complex, d denotes type double
//  D is the destination array, S is the source array, F is a scalar
//  L is an integer length (must be nonnegative!)
//
// Acopy(cD, cS, L)          copy S onto D
// Acopy(dD, dS, L)
//
// Aset(cD, cF, L)           set all elements of D to F
// Aset(dD, dF, L)
//
// Ascale(cD, dF, L)         multiply each element of D by F
// Ascale(cD, cF, L)
// Ascale(dD, dF, L)
//
// Aadd(cD, cS, L)           add elements of S into elements of D
// Aadd(cD, dS, L)
// Aadd(dD, dS, L)
//
// Asub(cD, cS, L)           subtract elements of S from elements of D
// Asub(cD, dS, L)
// Asub(dD, dS, L)
//
// Ascalesub(cD, cS, cF, L)  subtract elemnts of S, each multiplied by
// Ascalesub(dD, dS, dF, L)  F first, from elements of D (S unmodified)
//
// c Adot(cS1, cS2, L)       return dot product of S1 and S2: <S1|S2>
// d Adot(dS1, dS2, L)
//
// Aapply(cD, Func, L)       replace each element e of D with the result
// Aapply(dD, Func, L)       F(e).  F is called L times.
//
// Note for Aapply: the function Func must have the declaration:
//  Complex Func(Complex);   for applying to a Complex array
//  double Func(double);     for applying to a double array
// or its result must be automatically castable to the required type.
// Additionally Func may accept types <type>& and const <type>&, where
// type is Complex or double, as appropriate.
//
// ********************************************************************

#ifndef A_MATH_H
#define A_MATH_H

// following allows you to test routines using standard complex class
// #include <complex>
// typedef std::complex<double> Complex;

#include "SIScmplx.h"
#include <cstring>  /* for memcpy(), size_t; in Acopy() */

inline void Acopy( Complex *dest, const Complex *source, int len )
{
  std::memcpy(static_cast<void*>(dest), static_cast<const void*>(source), 
	      static_cast<std::size_t>(len * sizeof(Complex)));
}
inline void Acopy( double *dest, const double *source, int len )
{
  std::memcpy(static_cast<void*>(dest), static_cast<const void*>(source), 
	      static_cast<std::size_t>(len * sizeof(double)));
}

void Aset( Complex dest[], Complex s, int len );
void Aset( double dest[], double s, int len );

void Ascale( Complex dest[], double s, int len );
void Ascale( Complex dest[], Complex s, int len );
void Ascale( double dest[], double s, int len );

void Aadd( Complex *dest, const Complex *source, int len );
void Aadd( double *dest, const double *source, int len );
void Aadd( Complex *dest, const double *source, int len );

void Asub( Complex *dest, const Complex *source, int len );
void Asub( double *dest, const double *source, int len );
void Asub( Complex *dest, const double *source, int len );

void Ascalesub( Complex *dest, const Complex *source, Complex scale, int len );
void Ascalesub( double *dest, const double *source, double scale, int len );

Complex Adot( const Complex *a1, const Complex *a2, int len );
// inline Complex Adot( Complex *a1, Complex *a2, int len )
// { return Adot( (const Complex *)a1, (const Complex *)a2, len); }
double Adot( const double *a1, const double *a2, int len );
// inline double Adot( double *a1, double *a2, int len )
// { return Adot( (const double *)a1, (const double *)a2, len); }

void Aapply( Complex a[], Complex (* f)(Complex), int len);
void Aapply( Complex a[], Complex (* f)(Complex &), int len);
void Aapply( Complex a[], Complex (* f)(const Complex &), int len);
void Aapply( double a[], double (* f)(double), int len);
void Aapply( double a[], double (* f)(double &), int len);
void Aapply( double a[], double (* f)(const double &), int len);

#endif /* A_MATH_H */
