// arbitrary_impedance.h
//
// Copyright (c) 2004 Paul Grimes.
// All rights reserved.
//
// Developed from code contained in SuperMix version 1.3 elements.h
//
// Original code (c) 1999 California Institute Of Technology.
// All rights reserved, and distributed under the following licence
//
// Redistribution and use in source and binary forms for noncommercial
// purposes are permitted provided that the above copyright notice and
// this paragraph are duplicated in all such forms and that any
// documentation and other materials related to such distribution and
// use acknowledge that the software was developed by California
// Institute of Technology. Redistribution and/or use in source or
// binary forms is not permitted for any commercial purpose. Use of
// this software does not include a permitted use of the Institute's
// name or trademark for any purpose.
//
// DISCLAIMER:
// THIS SOFTWARE AND/OR RELATED MATERIALS ARE PROVIDED "AS-IS" WITHOUT
// WARRANTY OF ANY KIND INCLUDING ANY WARRANTIES OF PERFORMANCE OR
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE OR PURPOSE (AS SET
// FORTH IN UCC 23212-2313) OR FOR ANY PURPOSE WHATSOEVER, FOR THE
// LICENSED PRODUCT, HOWEVER USED.  IN NO EVENT SHALL CALTECH/JPL BE
// LIABLE FOR ANY DAMAGES AND/OR COSTS, INCLUDING BUT NOT LIMITED TO
// INCIDENTAL OR CONSEQUENTIAL DAMAGES OF ANY KIND, INCLUDING ECONOMIC
// DAMAGE OR INJURY TO PROPERTY AND LOST PROFITS, REGARDLESS OF
// WHETHER CALTECH/JPL SHALL BE ADVISED, HAVE REASON TO KNOW, OR IN
// FACT SHALL KNOW OF THE POSSIBILITY.  THE USER BEARS ALL RISK
// RELATING TO QUALITY AND PERFORMANCE OF THE SOFTWARE AND/OR RELATED
// MATERIALS.
//

/**
 * @file arbitrary_impedance.h
 *
 * Defines arbitrary_impedance child of spimp
 *
 * @author Paul Grimes
 * @date January 5th, 2004
 */

// ***************************************************************************
//
// 05/01/2004 File created
//
// ***************************************************************************

#ifndef ARB_IMPEDANCE_H
#define ARB_IMPEDANCE_H 1

#include "elements.h"

class arbitrary_impedance : public spimp
{
public:
  complex_parameter Z;

  parameter Temp;

  explicit arbitrary_impedance(Complex z = 0.0)
    : spimp(), Z(z), Temp(&T) {  }

  explicit arbitrary_impedance(const abstract_real_parameter * z)
    : spimp(), Z(z), Temp(&T) {  }

  arbitrary_impedance & set(Complex z)
  { Z = z; return *this; }

  arbitrary_impedance & set(abstract_complex_parameter * z)
  { Z = z; return *this; }

  arbitrary_impedance & set_T(double t)
  { Temp = t; return *this; }

  arbitrary_impedance & set_T(abstract_real_parameter * t)
  { Temp = t; return *this; }

  const nport::data_info & get_data_info()
  { info.active = (Temp != device::T); return info; }

private:
  // The calculation is done here.
  void recalc()   { calcZ(Complex(Z)-1.0); data.passive_noise(f, Temp); }
  void recalc_S() { calcZ(Complex(Z)-1.0); }
};

#endif
