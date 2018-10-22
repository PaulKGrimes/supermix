// test_id.cc
//
// Make the id tag in class device is always set correctly for subclasses
// of device.  Check constructors, copy constructors, and assignment operators.
//
// John Ward, 11/23/98

#include "supermix.h"
 
int main(void) {
  // Set global frequency and temperature
  device::f = 5.0 * GHz;
  device::T = 100.0 * Kelvin;

  cout << "Test that device::id is always unique." << endl;
  cout << "The first number is the original id." << endl;
  cout << "The second number was made with a copy constructor." << endl;
  cout << "The third number was made with an assignment operator." << endl;
 
  resistor r1;
  resistor r2(r1);
  resistor r3;
  r3 = r1;
  cout << r1.id << ", "
       << r2.id << ", "
       << r3.id << endl;

  capacitor c1;
  capacitor c2(c1);
  capacitor c3;
  c3 = c1;
  cout << c1.id << ", "
       << c2.id << ", "
       << c3.id << endl;

  inductor l1;
  inductor l2(l1);
  inductor l3;
  l3 = l1;
  cout << l1.id << ", "
       << l2.id << ", "
       << l3.id << endl;
  
  branch b1;
  branch b2(b1);
  branch b3;
  b3=b1;
  cout << b1.id << ", "
       << b2.id << ", "
       << b3.id << endl;

  series_tee st1;
  series_tee st2(st1);
  series_tee st3;
  st3=st1;
  cout << st1.id << ", "
       << st2.id << ", "
       << st3.id << endl;

  voltage_source vs1;
  voltage_source vs2(vs1);
  voltage_source vs3;
  vs3=vs1;
  cout << vs1.id << ", "
       << vs2.id << ", "
       << vs3.id << endl;

  circuit ckt1;
  circuit ckt2(ckt1);
  circuit ckt3;
  ckt3=ckt1;
  cout << ckt1.id << ", "
       << ckt2.id << ", "
       << ckt3.id << endl;

  cascade cscd1;
  cascade cscd2(cscd1);
  cascade cscd3;
  cscd3=cscd1;
  cout << cscd1.id << ", "
       << cscd2.id << ", "
       << cscd3.id << endl;

  transconductance t1;
  transconductance t2(t1);
  transconductance t3;
  t3 = t1;
  cout << t1.id << ", "
       << t2.id << ", "
       << t3.id << endl;

  radial_stub rs1;
  radial_stub rs2(rs1);
  radial_stub rs3;
  rs3 = rs1;
  cout << rs1.id << ", "
       << rs2.id << ", "
       << rs3.id << endl;
}
