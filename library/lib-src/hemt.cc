// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
//
// hemt.cc

#include "hemt.h"

fhx13x::fhx13x() : fet()
{
  Gm.G = 73.1621 * mSiemens;
  Gm.Tau = 0.7 * Pico * Second;
  Cgs.C = 0.18305 * pFarad;
  Rds.R = 166.21 * Ohm;
  Cgd.C = 0.0242041 * pFarad;
  Cds.C = 0.0230908 * pFarad;
  Rgs.R = 1.47226 * Ohm;
  Rg.R = 0.382871 * Ohm;
  Rs.R = 1.90601 * Ohm;
  Rd.R = 5.0 * Ohm;
  Lg.L = 0.0676668 * nHenry;
  Ls.L = 0.0810802 * nHenry;
  Ld.L = 0.103801 * nHenry;
  Cpg.C = 0.00548683 * pFarad;
  Cpd.C = 0.0300508 * pFarad;

  td_min = 500.0 * Kelvin;
  td_297k = 4158.0 * Kelvin;
  slope = (td_297k - td_min) / (292.0 * Kelvin);
}

void fhx13x::recalc()
{
  // Set the drain resistor temperature.
  if(T<=5.0*Kelvin)
    Rds.Temp = td_min;
  else
    Rds.Temp = td_min + (T - 5.0*Kelvin)*slope;

  data_ptr = &fetckt.get_data();
}

fhr02x::fhr02x() : fet()
{
  Gm.G = 69.3646 * mSiemens;
  Gm.Tau = 0.895616 * Pico * Second;
  Cgs.C = 0.22726 * pFarad;
  Rds.R = 146.598 * Ohm;
  Cgd.C = 0.0204 * pFarad;
  Cds.C = 0.018567 * pFarad;
  Rgs.R = 0.4000 * Ohm;
  Rg.R = 0.15023 * Ohm;
  Rs.R = 4.67118 * Ohm;
  Rd.R = 0.0453676 * Ohm;
  Lg.L = 0.067903 * nHenry;
  Ls.L = 0.0867215 * nHenry;
  Ld.L = 0.0810266 * nHenry;
  Cpg.C = 0.0255152 * pFarad;
  Cpd.C = 0.0349169 * pFarad;

  td_min = 500.0 * Kelvin;
  td_297k = 2426.0 * Kelvin;
  slope = (td_297k - td_min) / (292.0 * Kelvin);
}

void fhr02x::recalc()
{
  // Set the drain resistor temperature.
  if(T<=5.0*Kelvin)
    Rds.Temp = td_min;
  else
    Rds.Temp = td_min + (T - 5.0*Kelvin)*slope;

  data_ptr = &fetckt.get_data();
}

kukje::kukje() : fet()
{
  Gm.G = 109.0 * mSiemens;
  Gm.Tau = 0.6 * Pico * Second;
  Cgs.C = 0.155 * pFarad;
  Rds.R = 112. * Ohm;
  Cgd.C = 0.031 * pFarad;
  Cds.C = 0.006 * pFarad;
  Rgs.R = 2.9 * Ohm;
  Rg.R = 1.2 * Ohm;
  Rs.R = 2.5 * Ohm;
  Rd.R = 3.6 * Ohm;
  Lg.L = 0.032 * nHenry;
  Ls.L = 0.008 * nHenry;
  Ld.L = 0.011 * nHenry;
  Cpg.C = 0.008 * pFarad;
  Cpd.C = 0.051 * pFarad;

  td_min = 500.0 * Kelvin;
  td_297k = 3000.0 * Kelvin;
  slope = (td_297k - td_min) / (292.0 * Kelvin);
}

void kukje::recalc()
{
  // Set the drain resistor temperature.
  if(T<=5.0*Kelvin)
    Rds.Temp = td_min;
  else
    Rds.Temp = td_min + (T - 5.0*Kelvin)*slope;

  data_ptr = &fetckt.get_data();
}

jpltrw160::jpltrw160() : fet()
{
  Gm.G = 111.7 * mSiemens;
  Gm.Tau = 0.680 * Pico * Second;
  Cgs.C = 0.126 * pFarad;
  Rds.R = 78.81 * Ohm;
  Cgd.C = 0.038 * pFarad;
  Cds.C = 0.056 * pFarad;
  Rgs.R = 2.0 * Ohm;
  Rg.R = 2.0 * Ohm;
  Rs.R = 1.0 * Ohm;
  Rd.R = 1.0 * Ohm;
  Lg.L = 0.189 * nHenry;
  Ls.L = 0.037 * nHenry;
  Ld.L = 0.158 * nHenry;
  Cpg.C = 0.000 * pFarad;
  Cpd.C = 0.000 * pFarad;

  td_min = 430.0 * Kelvin;
  td_297k = 2000.0 * Kelvin;
  slope = (td_297k - td_min) / (292.0 * Kelvin);
}

void jpltrw160::remove_bond_wires()
{
  Lg.L = 0.012 * nHenry;
  Ls.L = 0.008 * nHenry;
  Ld.L = 0.015 * nHenry;
}

void jpltrw160::recalc()
{
  // Set the drain resistor temperature.
  if(T<=12.5*Kelvin)
    Rds.Temp = td_min;
  else
    Rds.Temp = td_min + (T - 5.0*Kelvin)*slope;

  data_ptr = &fetckt.get_data();
}

jpltrw300::jpltrw300() : fet()
{
  Gm.G = 138.9 * mSiemens;
  Gm.Tau = 0.796 * Pico * Second;
  Cgs.C = 0.140 * pFarad;
  Rds.R = 43.21 * Ohm;
  Cgd.C = 0.063 * pFarad;
  Cds.C = 0.080 * pFarad;
  Rgs.R = 1.451 * Ohm;
  Rg.R = 3.0 * Ohm;
  Rs.R = 0.5 * Ohm;
  Rd.R = 2.0 * Ohm;
  Lg.L = 0.16 * nHenry;
  Ls.L = 0.034 * nHenry;
  Ld.L = 0.17 * nHenry;
  Cpg.C = 0.000 * pFarad;
  Cpd.C = 0.000 * pFarad;

  td_min = 430.0 * Kelvin;
  td_297k = 2000.0 * Kelvin;
  slope = (td_297k - td_min) / (292.0 * Kelvin);
}

void jpltrw300::remove_bond_wires()
{
  Lg.L = 0.012 * nHenry;
  Ls.L = 0.008 * nHenry;
  Ld.L = 0.015 * nHenry;
}

void jpltrw300::recalc()
{
  // Set the drain resistor temperature.
  if(T<=12.5*Kelvin)
    Rds.Temp = td_min;
  else
    Rds.Temp = td_min + (T - 5.0*Kelvin)*slope;

  data_ptr = &fetckt.get_data();
}
