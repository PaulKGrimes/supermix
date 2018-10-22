// filter.cc:
//
// The following code builds a 2 GHz maximally flat low-pass filter.
// See Pozar, page 489.

#include "supermix.h"

int main()
{
  device::T = 295. * Kelvin;
  device::Z0 = 50. * Ohm;

  capacitor c1;
  c1.parallel();
  c1.C = 0.984 * pFarad;

  inductor L2(6.438*nHenry);
  L2.series();

  capacitor c3;
  c3.parallel();
  c3.C = 3.183 * pFarad;

  inductor L4 = L2;
  capacitor c5(c1);

  circuit filter;
  int input = filter.add_port(c1, 1);
  int output = filter.add_port(c5, 2);
  filter.connect(c1, 2, L2, 1);
  filter.connect(L2, 2, c3, 1);
  filter.connect(c3, 2, L4, 1);
  filter.connect(L4, 2, c5, 1);

  cout << fixed << setprecision(3);
  cout << "2 GHz low-pass filter" << endl << endl;
  cout << "F(GHz)" << "\t" << "S21" << "\t" << "Phase" << endl;

  complex::out_degree();
  complex::out_separator("\t");
  
  for(double f=0.25; f<5.0; f+=0.25)  {
    device::f = f*GHz;
    sdata response = filter.get_data();
    complex S21 = response.S[output][input];
    cout << device::f/GHz << "\t" << S21 << endl;
  }

}
