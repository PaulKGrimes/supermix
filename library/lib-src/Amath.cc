// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
//
// Amath.cc

#include "Amath.h"

using namespace std;

void Aset( Complex dest[], Complex s, int len )
{
  /*register*/ Complex * d = dest + len;
  while (d != dest) *(--d) = s;
}

void Aset( double dest[], double s, int len )
{
  /*register*/ double * d = dest + len;
  while (d != dest) *(--d) = s;
}

void Ascale( Complex dest[], double s, int len )
{
  /*register*/ Complex * d = dest + len;
  while (d != dest) *(--d) *= s;
}

void Ascale( Complex dest[], Complex s, int len )
{
  /*register*/ Complex * d = dest + len;
  while (d != dest) *(--d) *= s;
}

void Ascale( double dest[], double s, int len )
{
  /*register*/ double * d = dest + len;
  while (d != dest) *(--d) *= s;
}

void Aadd( Complex *dest, const Complex *source, int len )
{
  /*register*/ Complex * d = dest + len;
  /*register*/ const Complex * s = source + len;

  while (d != dest) *(--d) += *(--s);
}

void Aadd( double *dest, const double *source, int len )
{
  /*register*/ double * d = dest + len;
  /*register*/ const double * s = source + len;

  while (d != dest) *(--d) += *(--s);
}

void Aadd( Complex *dest, const double *source, int len )
{
  /*register*/ Complex * d = dest + len;
  /*register*/ const double * s = source + len;

  while (d != dest) *(--d) += *(--s);
}

void Asub( Complex *dest, const Complex *source, int len )
{
  /*register*/ Complex * d = dest + len;
  /*register*/ const Complex * s = source + len;

  while (d != dest) *(--d) -= *(--s);
}

void Asub( double *dest, const double *source, int len )
{
  /*register*/ double * d = dest + len;
  /*register*/ const double * s = source + len;

  while (d != dest) *(--d) -= *(--s);
}

void Asub( Complex *dest, const double *source, int len )
{
  /*register*/ Complex * d = dest + len;
  /*register*/ const double * s = source + len;

  while (d != dest) *(--d) -= *(--s);
}

void Ascalesub( Complex *dest, const Complex *source, Complex scale, int len )
{
  /*register*/ Complex * d = dest + len;
  /*register*/ const Complex * s = source + len;

  while (d != dest) *(--d) -= ((*(--s)) * scale);
}

void Ascalesub( double *dest, const double *source, double scale, int len )
{
  /*register*/ double * d = dest + len;
  /*register*/ const double * s = source + len;
  /*register*/ double rs = scale;

  while (d != dest) *(--d) -= ((*(--s)) * rs);
}

Complex Adot( const Complex *a1, const Complex *a2, int len )
{
  /*register*/ const Complex * ra1 = a1 + len;
  /*register*/ const Complex * ra2 = a2 + len;
  Complex sum(0.0);

  while (ra1 != a1) sum += (conj(*(--ra1)) * (*(--ra2)));
  return sum;
}

double Adot( const double *a1, const double *a2, int len )
{
  /*register*/ const double * ra1 = a1 + len;
  /*register*/ const double * ra2 = a2 + len;
  /*register*/ double sum = 0.0;;

  while (ra1 != a1) sum += (*(--ra1)) * (*(--ra2));
  return sum;
}

void Aapply( Complex a[], Complex (* f)(Complex), int len)
{
  /*register*/ Complex * d = a + len;
  while (d-- != a) *d = f(*d);
}

void Aapply( Complex a[], Complex (* f)(Complex &), int len)
{
  /*register*/ Complex * d = a + len;
  while (d-- != a) *d = f(*d);
}

void Aapply( Complex a[], Complex (* f)(const Complex &), int len)
{
  /*register*/ Complex * d = a + len;
  while (d-- != a) *d = f(*d);
}

void Aapply( double a[], double (* f)(double), int len)
{
  /*register*/ double * d = a + len;
  while (d-- != a) *d = f(*d);
}

void Aapply( double a[], double (* f)(double &), int len)
{
  /*register*/ double * d = a + len;
  while (d-- != a) *d = f(*d);
}

void Aapply( double a[], double (* f)(const double &), int len)
{
  /*register*/ double * d = a + len;
  while (d-- != a) *d = f(*d);
}


