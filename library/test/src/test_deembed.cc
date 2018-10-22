// Test deembed by embedding a known circuit, then comparing its original
// sdata with the deembedded sdata.

#include "supermix.h"
#include "deembed.h"

int main()
{
  resistor r;
  r.R = 75. * Ohm;
  capacitor c;
  c.C = 20. * Pico * Farad;
  c.parallel();
  kukje k;

  circuit ckt;
  ckt.add_port(r, 1);
  ckt.connect(r, 2, k, 1);
  ckt.connect(k, 2, c, 1);
  ckt.add_port(c, 2);

  deembed d(r, c, ckt);

  device::f = 5.0 * GHz;

  cout << "The following two matrices should be the same." << endl << endl;

  cout << "S parameter of a bare transistor:";

  k.get_data().S.show();

  cout << endl;

  cout << "S parameter of the same transistor, deembedded:";

  d.get_data().S.show();
  
}
