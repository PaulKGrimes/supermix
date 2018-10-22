// test_term
//
// test the terminators added to elements.h
// 8/21/98

#include "supermix.h"

int main()
{
  // get rid of tiny imaginary parts
  Complex::rel_tolerance = Complex::abs_tolerance = 1.0e-10;

  cout << endl;

  device::T = 300*Kelvin;

  device::f = 10*GHz;

  zterm t1;
  yterm t2;
  sdata s;  // hold get_data() results

  // make sure t1 and t2 are sized properly:
  cout << "The size of a terminator should be 1:" << endl;
  cout << "zterm: " << t1.size() << "\tyterm: " << t2.size() << endl << endl;

  // the default should be a short for t1, an open for t2:
  s = t1.get_data();
  cout << "The default short-circuit zterm:" << endl;
  cout << "S: \tC: \tB:\n" << s.S << '\t' << s.C << '\t' << s.B << endl << endl;
  s = t2.get_data();
  cout << "The default open-circuit yterm:" << endl;
  cout << "S: \tC: \tB:\n" << s.S << '\t' << s.C << '\t' << s.B << endl << endl;

  // now set both to Z0
  t1.Z = device::Z0; t2.Y = 1/device::Z0;
  s = t1.get_data();
  cout << "The Z0 zterm:" << endl;
  cout << "S: \tC: \tB:\n" << s.S << '\t' << s.C << '\t' << s.B << endl << endl;
  s = t2.get_data();
  cout << "The 1/Z0 yterm:" << endl;
  cout << "S: \tC: \tB:\n" << s.S << '\t' << s.C << '\t' << s.B << endl << endl;

}
