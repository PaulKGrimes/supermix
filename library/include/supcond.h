// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ******************************************************************
// supcond.h
//
// Function declaration for supcond()
// (defined in supcond.cc)
//
// J. Zmuidzinas  9/16/97
// ******************************************************************

#ifndef SUPCOND_H
#define SUPCOND_H

#include "SIScmplx.h"

Complex supcond(double freq, double T, double Vgap, double Tc);

// ******************************************************************
//     calculate complex conductivity of a superconductor in the local limit
//
//     Input variables:
//        freq - frequency
//        T - temperature 
//        Vgap - energy gap voltage of superconductor at T=0
//        Tc - critical temperature
//     These variables use standard units as defined in units.h
//
//     Return value: Superconductor complex conductivity 
//     normalized to normal-state conductivity
//     This is dimensionless so units don't matter
//
// ******************************************************************
//     The sign of the imaginary part is chosen to be consistent
//     with the standard EE convention of exp(+j omega t) for
//     harmonic time dependence
// ******************************************************************

#endif  /* SUPCOND_H */



