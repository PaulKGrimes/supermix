// test_ck_prompt
//
// Test the ckdata class and its calc() function
// interactive; prompts for user input
//
// FR 1/9/98
//
// 1/12/00:  Changed argument list to ckdata::calc()
// 11/11/98: Changed table access to new syntax
// 7/28/98: added <iostream.h>, since SIScmplx.h doesn't any more
// 7/2/98:  updated #include file
// 6/29/98: added autocorrelation check

#include <iostream>
#include "global.h"
#include "junction.h"
#include "units.h"

using namespace std;

Vector V;   // hold harmonic voltages

int main()
{

double f;
double Vgap = 3.0 * mVolt;
ckdata C;
 
  while (TRUE) {  // loop until break

    // Input values for fLO and harmonic voltages:

    cout << "\nTest the ckdata class\n";
    cout << "\nInput a value for Local Oscillator Frequency\n"
	 << "as a fraction of the SIS gap frequency (0 to quit)\n ";
    cin >> f;
    if (f == 0.0) break;  // Quit if input 0.0

    cout << "\nInput the harmonic voltages ( real  imaginary )\n"
	 << "Enter the PEAK values, normalized to the SIS Vgap = 3mV\n";
    cout << "(999 999  to terminate voltage input)\n";
    int j = 1;
    while (TRUE) {  // loop until 999 999 is entered
      Complex Vin;
      cout << "harmonic " << j << " : ";
      cin >> Vin.real >> Vin.imaginary;
      if (Vin == Complex(999.0,999.0)) break;
      V.resize(j)[j] = Vin;
      ++j;
    }

    // Now renormalize the input to real values using Vgap

    f *= Vgap * VoltToFreq;  // convert f to a real freq
    V *= Vgap / RmsToPeak;   // convert to real RMS voltages

    // Call C.calc() with these values and display the results

    C.calc(f,V);

    cout << "\nTolerance: " << C.Tol << endl;
    cout << "\nThe Voltages:\n";
    for (int i = V.minindex(); i <= V.maxindex(); ++i)
      cout << "\t" << i << " :\t" << V[i] << endl;

    cout << "\nThe Ck:\n";
    for (int i = C.Ck.minindex(); i <= C.Ck.maxindex(); ++i)
      cout << "\t" << i << " :\t" << C[i] << endl;

    cout << "\nThe Autocorrelation Checks (should be 0 except for index = 0):\n";
    complex autocorrelate(const Vector &, int);
    for (int i = C.Ck.minindex(); i <= C.Ck.maxindex(); ++i)
      cout << "\t" << i << " :\t" << autocorrelate(C.Ck, i) << endl;

    // Now clear V and loop

    V.resize(0);
  }

  return 0;
}

complex autocorrelate(const Vector & ck, int p)
{
  // calculates the sum of ck[i]*conj(ck[i+p])
  complex sum = 0.0;
  for (int i = ck.minindex(); i <= ck.maxindex(); ++i)
    sum += ck[i]*conj(ck.read(i+p));
  return sum;
}
