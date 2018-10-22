// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// **************************************************************************
// mstrip.h

#ifndef MSTRIP_H
#define MSTRIP_H

#define MSTRIP_H_VERYBIG 1.e20
#define MSTRIP_H_VERYSMALL 1.e-20

void mstrip(double h, double w, double t1, double t2, 
            double esub, double eup, double Qsub, double Qup, double freq,
            double &Z0, double &epeff, double &tandel, 
            double &g2strip, double &g2ground) ;
//
//    Lengths are converted to be in microns and frequencies in GHz.
//
//    Input parameters:
//
//    h - substrate thickness
//    w - width of microstrip
//    t1 - conductor thickness (strip)
//    t2 - conductor thickness (ground)
//    esub - dielectric constant of substrate
//    eup - dielectric constant superstrate (usually = 1)
//    freq - frequency
//    Qsub - dielectric quality factor of substrate (1/tan(delta))
//    delta is defined as the phase angle of the complex
//    dielectric constant of the substrate
//    Qup - dielectric quality factor of superstrate
//
//
//    Output parameters:
//
//    Z0 - characteristic impedance
//    epeff - effective dielectric constant
//         (wavelength = free space wavelength / sqrt(epeff)
//    tandel - effective loss tangent of line. Defined so that
//         beta = (2*Pi/wavelength)*(1 + i*tandel)
//         where beta is the complex propagation factor; i = sqrt(-1).
//    g2strip - geometrical factor for loss calculation in strip conductor
//    g2ground - geometrical factor for loss calculation in ground conductor
//

#endif /* MSTRIP_H */

