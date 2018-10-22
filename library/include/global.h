// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ********************************************************************
/**
 * @file global.h
 *
 * include file for the SIS circuit group
 *
 *      *** All SOURCE FILES SHOULD INCLUDE THIS FILE FIRST ***
 *
 * @author Frank Rice
 * @date July 31, 1997
 *
 * @see units.h
 */
// ********************************************************************

#ifndef GLOBAL_H
#define GLOBAL_H

#include "SIScmplx.h"
#include "matmath.h"

// ====================================================================
// Miscellaneous constants and limits:

/** A small number, to limit calculations. */  
#define ZEROTOL (1.0e-6)

// standard truth values
#ifndef FALSE
#define FALSE   0
#endif

#ifndef TRUE
#define TRUE  ( ! FALSE )
#endif

const complex I(0.0,1.0);  // Useful complex number

const long double PiLong = 3.1415926535897932384626433832795028841973L;
const double Pi = PiLong;

const double Tiny = 1.0E-100;

const long double DegreeLong = 0.01745329251994329576923690768488612713443L;
const double Degree = DegreeLong;
          // Degree == Pi/180

const long double RmsToPeakLong = 1.414213562373095048801688724209698078570L;
const double RmsToPeak = RmsToPeakLong;
          // RmsToPeak == sqrt(2.0), for converting RMS to peak values


// ====================================================================
// Master Units:
//       All internally passed parameters should be normalized to
//       multiples of the following.


const double  mVolt  = 1.0;           //  millivolt RMS

const double  Ohm    = 1.0;    

const double  GHz    = 0.01;          //  100*Ghz == 1.0

const double  Micron = 1.0;

const double  Kelvin = 1.0;

#include "units.h"

// ====================================================================

#endif /* GLOBAL_H */
