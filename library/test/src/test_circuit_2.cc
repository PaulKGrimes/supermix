// test_circuit_2
// 8/21/98: more testing of class circuit


#include "supermix.h"

inline void show_sdata(const sdata & A, ostream & s = cout)
{
  streamsize w = s.width();  // the output field width
  A.S.show(s); s.width(w); A.C.show(s); s.width(w); A.B.show(s);
}


int main()
{
  // Make a simple circuit:
  resistor r; r.parallel(); r.R = 100*Ohm;
  capacitor c; c.parallel(); c.C = 10*pFarad;
  circuit ckt;
  ckt.add_port(r,1);
  ckt.connect(r,2,c,1);
  ckt.add_port(c,2);

  cout << "\nThe 2-port circuit:" << endl;
  cout << "size: " << ckt.size() << endl;
  show_sdata(ckt.get_data());
  cout << endl;

  // Now delete the last port added, and connect a Z0 terminator
  port check = ckt.pop_port();
  if (port(c.id, 2) == check)
    cout << "circuit::pop() got the correct port." << endl << endl;
  else
    cout << "The popped port is incorrect!!" << endl << endl;

  zterm t; t.Z = device::Z0;
  ckt.connect(c,2,t,1);

  cout << "\nThe 1-port circuit:" << endl;
  cout << "size: " << ckt.size() << endl;
  show_sdata(ckt.get_data());
  cout << endl;

  // now try the same thing using a cascade
  cascade ckt2;
  cout << "\nThe default (empty) cascade:" << endl;
  cout << "size: " << ckt2.size() << endl;
  show_sdata(ckt2.get_data());
  cout << endl;

  ckt2.a(r).a(c);
  cout << "\nThe 2-port circuit, using a cascade:" << endl;
  cout << "size: " << ckt2.size() << endl;
  show_sdata(ckt2.get_data());
  cout << endl;

  ckt2.a(t);
  cout << "\nThe 1-port circuit, using a cascade:" << endl;
  cout << "size: " << ckt2.size() << endl;
  show_sdata(ckt2.get_data());
  cout << endl;

  // now play with the temperature in various ways:
  device::T = 100*Kelvin;
  cout << "\nChange the global temp to 100K" << endl;
  show_sdata(ckt2.get_data());
  cout << endl;
  ckt2.Temp = 300*Kelvin;
  cout << "\nLocal circuit temp back to 300K" << endl;
  show_sdata(ckt2.get_data());
  cout << endl;

  // check copy constructor and operator =:
  cascade ckt3(ckt2), ckt4;
  cout << "\ncascade copy constructor:" << endl;
  cout << "size: " << ckt3.size() << endl;
  show_sdata(ckt3.get_data());
  cout << endl;
  ckt4 = ckt2;
  cout << "\ncascade operator =:" << endl;
  cout << "size: " << ckt4.size() << endl;
  show_sdata(ckt4.get_data());
  cout << endl;


  // Restore ckt2 to global temp, do some checks
  ckt2.Temp = &device::T;
  cout << "\nBack to shadowing global 100K" << endl;
  show_sdata(ckt2.get_data());
  cout << endl;
  cout << "\nBut the copy should still be at 300K" << endl;
  show_sdata(ckt3.get_data());
  cout << endl;
  cout << "\nSame for operator = copy" << endl;
  show_sdata(ckt4.get_data());
  cout << endl;

}
