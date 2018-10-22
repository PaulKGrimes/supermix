/*

Test class generator, as well as handling of B array in general.


9/30/98: changed connection objects to circuit objects
7/28/98: added <iostream.h> to includes, since SIScmplx.h doesn't

*/

#include <iostream>
#include "nport.h"
#include "elements.h"
#include "sources.h"
#include "circuit.h"
#include "units.h"
#include "error.h"

using namespace std;

int main(void)
{
  device::f = 5.0 * GHz;
  device::T = 100 * Kelvin;

  // Create a generator.
  generator ps1;
  ps1.R = 50.0 * Ohm;
  ps1.source_f = 5.0 * GHz;
  ps1.source_width = 1.0 * MHz;
  ps1.source_power = 1000.0 * uWatt;
  ps1.source_phase = Pi/4.0;
  generator ps2(ps1);

  // Create something simple to test connections.
  resistor r;
  r.R = 0.1 * Ohm;

  sdata sd1, sd2;
  circuit con1;
    con1.connect(r, 2, ps2, 1);
    con1.add_port(r, 1);

  for(double fr=4.999; fr<5.001; fr+=0.00025)
  {
    device::f = fr * GHz;

    sd1 = ps2.get_data();
    sd2 = con1.get_data();
    cout << fr << " GHz:" << endl;
    cout << "S11 = " << sd1.S.get(1,1) << endl;
    cout << "C11 = " << (sd1.C.get(1,1) / Kelvin) << " Kelvin" << endl;
    cout << "B1 = " << sd1.B.get(1)/mVolt << endl;
    cout << "S11 = " << sd2.S.get(1,1) << endl;
    cout << "C11 = " << (sd2.C.get(1,1) / Kelvin) << " Kelvin" << endl;
    cout << "B1 = " << sd2.B.get(1)/mVolt << endl;
    cout << endl;
  }

  device::f = 5.0 * GHz;

  branch b;
  resistor r2(r);

  con1 = circuit();
    con1.connect(r, 2, ps1, 1);
    con1.add_port(r, 1);

  circuit con2;
    con2.connect(r2, 2, ps2, 1);
    con2.add_port(r2, 1);

  circuit con3;
    con3.connect(b, 3, con1, 1);
    con3.add_port(b, 1);
    con3.add_port(b, 2);

  circuit con4;
    con4.connect(b, 2, con2, 1);
    con4.add_port(b, 1);
    con4.add_port(b, 3);

  sd1 = con4.get_data();
  cout << (device::f / GHz) << " GHz:" << endl;
  cout << "S11 = " << sd1.S.get(1,1) << endl;
  cout << "C11 = " << (sd1.C.get(1,1) / Kelvin) << " Kelvin" << endl;
  cout << "B1 = " << sd1.B.get(1)/mVolt << endl;
}  
