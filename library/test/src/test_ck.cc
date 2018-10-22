// test_ck
//
// Test the ckdata class and its calc() function
// usage: test_ck flo vlo phase
//   flo: L.O. freq in units of gap frequency
//   vlo: L.O. rms voltage in units of gap voltage
//   phase: L.O. phase in degrees (0 for real vlo)
//
// FR 1/9/98
//
// 1/12/00:  Changed argument list to ckdata::calc()
// 11/11/98: Changed table access to new syntax
// 7/28/98: added <iostream.h>, since SIScmplx.h doesn't any more

#include "global.h"
#include "junction.h"
#include "units.h"
#include <cstdlib>
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
  Complex::rel_tolerance = 1.0e-14;  // get rid of tiny imaginary parts

  if (argc != 4) {
    cout << "usage: test_ck flo vlo phase" << endl
         << "   flo: L.O. freq in units of gap frequency" << endl
         << "   vlo: L.O. rms voltage in units of gap voltage" << endl
         << "   phase: L.O. phase in degrees (0 for real vlo)" << endl;
    return 0;
  }

  double f = atof(argv[1]);
  double Vgap = 3.0 * mVolt;
  Vector V(1); V[1] = polar(atof(argv[2]), atof(argv[3])*Degree);
  ckdata C;
 
  // Now renormalize the input to real values using Vgap

  f *= Vgap * VoltToFreq;  // convert f to a real freq
  V *= Vgap;               // convert to real RMS voltage

  // Call C.calc() with these values and display the results

  C.calc(f,V);

  for (int i = C.Ck.minindex(); i <= C.Ck.maxindex(); ++i)
    cout << i << " :\t" << C[i] << endl;

  return 0;
}
