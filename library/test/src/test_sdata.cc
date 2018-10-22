// test various conversions declared in sdata.h
// 7/8/98

#include "supermix.h"

int main()
{
  // get rid of tiny imaginary parts
  Complex::rel_tolerance = Complex::abs_tolerance = 1.0e-10;

  resistor R1; R1.R = 100 * Ohm; R1.parallel();
  resistor R2; R2.R = 100 * Ohm; R2.series();
  resistor R3; R3.R = 100 * Ohm; R3.series();

  circuit c;
  c.add_port(R2,1);
  c.add_port(R3,2);
  c.connect(R1,1,R2,2);
  c.connect(R1,2,R3,1);

  sdata s(c.get_data());

  zdata z(s);
  ydata y(s);
  ydata_ptr py(&y.Y, &y.C, &y.Is);

  cout << "\nThe 100 Ohm Tee\n" << endl;

  cout << "S:"; s.S.show();
  cout << "Y:"; y.Y.show();
  cout << "Z:"; z.Z.show();
  cout << "Y*Z:"; (y.Y * z.Z).show();

  cout << "S - sdata(Y):"; (s.S - sdata(y).S).show(); 
  cout << "S - sdata(Z):"; (s.S - sdata(z).S).show(); 
  cout << "Y - ydata(Z):"; (y.Y - ydata(z).Y).show(); 
  cout << "Z - zdata(Y):"; (z.Z - zdata(y).Z).show(); 
  cout << "S - sdata(Yptr):"; (s.S - sdata(py).S).show(); 
  
  sdata s2; s2 = py;
  cout << "S - S2:"; (s.S - s2.S).show();

  ydata_ptr py2(&y.Y);
  s2 = py2;
  cout << "S - S2: (another way)"; (s.S - s2.S).show();

  // now look at noise matrices:
  cout << "Noise Matrix Comparisons:\n" << endl;
  cout << "S - sdata(Y):"; (s.C - sdata(y).C).show(); 
  cout << "S - sdata(Z):"; (s.C - sdata(z).C).show(); 
  cout << "Y - ydata(Z):"; (y.C - ydata(z).C).show(); 
  cout << "Z - zdata(Y):"; (z.C - zdata(y).C).show(); 
  cout << "S - sdata(Yptr):"; (s.C - sdata(py).C).show(); 
  cout << "C"; s.C.show();
  cout << "s2.C (should be a matrix of zeroes)"; s2.C.show();
}
