// ********************************************************************
// test_formatting
//
// test various ways of formatting table and vector output
//
// 7/31/98
//
// ********************************************************************

#include "supermix.h"

// this function is defined following main()
void show_sdata(const sdata & A, ostream & s = cout);


int main()
{
  Complex::rel_tolerance = 1.0e-14;  // get rid of tiny imaginary parts

  // create a simple circuit for testing

  device::f = 100*GHz;
  device::T = 4*Kelvin;

  voltage_source v1,v2,v3;

  v1.source_voltage = 10*mVolt;
  v1.source_phase = Pi/4;
  v1.R = 20*Ohm;
  v1.source_f = 100*GHz;

  v2.source_voltage = 500*mVolt;
  v2.source_phase = Pi/2;
  v2.R = 500*Ohm;
  v2.source_f = 100*GHz;

  v3.source_voltage = 1*mVolt;
  v3.source_phase = 0;
  v3.R = 100*Ohm;
  v3.source_f = 100*GHz;

  resistor r1;
  r1.R = 100*Ohm;
  r1.series();

  branch b1;

  circuit c1;
  c1.connect(v3,1, b1,2);
  c1.connect(r1,1, b1,3);
  c1.add_port(b1,1);
  c1.add_port(r1,2);

  circuit c2;
  c2.add_port(v1,1);
  c2.add_port(v2,1);
  c2.add_port(c1,1);
  c2.add_port(c1,2);

  // now get the sdata object for c2, which has 4 ports:
  sdata s = c2.get_data();

  // display this data using default complex, Vector, and Matrix formats:
  cout << "\nThe sdata for a simple 4-port, using all default formats,"
    " and .show()" << endl;
  s.S.show(); s.C.show(); s.B.show();

  // change active left index range on s.S to check that we put out
  // matrices properly:
  s.S.Lmaxindex(3);
  cout << "\nA matrix with 3 rows, 4 columns, all default formats:" << endl;
  s.S.show();
  s.S.maximize();  // restore S

  // now fix a width of 15
  cout << "\nThe sdata for a simple 4-port, using setw(15), default formats"
    " and .show()" << endl;
  cout << setw(15); s.S.show();
  cout << setw(15); s.C.show();
  cout << setw(15); s.B.show();
  
  // set complex output mode to polar degrees:
  cout << "\nSame, except use complex::out_degree():" << endl;
  complex::out_degree();
  cout << setw(15); s.S.show();
  cout << setw(15); s.C.show();
  cout << setw(15); s.B.show();
  
  // set complex output mode to polar degrees:
  cout << "\nSame, except use complex::out_degree():" << endl;
  complex::out_degree();
  cout << setw(15); s.S.show();
  cout << setw(15); s.C.show();
  cout << setw(15); s.B.show();

  // set table separator to two newlines, use delimited format
  string old_t = Matrix::out_separator("\n\n");
  cout << "\nTwo newlines between rows, delimited complex format:" << endl;
  complex::out_delimited();
  cout << setw(15); s.S.show();
  cout << setw(15); s.C.show();
  cout << setw(15); s.B.show();

  // now set vector separator to null, and leave width 0 
  string old_v = Vector::out_separator("");
  cout << "\nNow no vector separator, field width 0:" << endl;
  s.S.show();
  s.C.show();
  s.B.show();

  // with no table separator, and use << to ouput
  Matrix::out_separator("");
  cout << "\nNow no table separator, field width 0, using <<:" << endl;
  cout << endl << s.S << s.C << s.B << endl;

  // use tabs, with complex::out_space()
  complex::out_cartesian();
  complex::out_space();
  Matrix::out_separator("\t\t");
  Vector::out_separator("\t");
  cout << "\nNow use complex::out_space, tabs to separate components:" << endl
       << s.S << "\t\t\t" << s.C << "\t\t\t" << s.B << endl;

  // everything back to defaults, should duplicate first output
  complex::out_default();
  Matrix::out_separator(old_t);
  Vector::out_separator(old_v);
  cout << "\nBack to using all default formats,"
    " and .show()" << endl;
  s.S.show(); s.C.show(); s.B.show();

  // Use the function defined after main to output the sdata
  cout <<"\nUse the simple show_sdata() function, width(15):" << endl;
  cout.width(15); show_sdata(s);

  // Display the source vector with complicated format
  cout << "\nDisplay the source vector vertically in volts, polar:"
       << endl << endl;

  string save1 = complex::out_prefix("");
  string save2 = complex::out_separator(" Volts, Phase = ");
  string save3 = complex::out_suffix(" deg");
  string save4 = Vector::out_separator('\n');
  complex::io_mode save5 = complex::out_degree();

  cout << setprecision(3) << fixed << zdata(s).Vs / Volt << endl;

  complex::out_prefix(save1);
  complex::out_separator(save2);
  complex::out_suffix(save3);
  Vector::out_separator(save4);
  complex::out_mode(save5);

  cout << endl;

} //main

void show_sdata(const sdata & A, ostream & s)
{
  streamsize w = s.width();  // the output field width
  A.S.show(s); s.width(w); A.C.show(s); s.width(w); A.B.show(s);
}
