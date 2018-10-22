#include <iostream>
#include "supermix.h"
#include "transformer.h"

int main() 
{
  Complex::rel_tolerance = Complex::abs_tolerance = 1.0e-14;  // get rid of tiny parts

  device::T = 100.0 * Kelvin;
  device::f = 5.0 * GHz;

  transformer t1;
  t1.Z1 = 50.*Ohm ;
  t1.Z2 = (140.+I*100.) *Ohm ;

  sdata sd = t1.get_data() ;
  cout << "50 to 140 + j100 Ohm ideal transformer" << endl;
  cout << "S matrix: " << endl ;
  sd.S.show() ;
  complex S22 = sd.S.get(2,2) ;
  complex Zout = device::Z0 * (1.+S22)/(1.-S22) ;
  cout << "Output impedance is:  " << Zout << endl ;
  cout << "C matrix: " << endl ;
  sd.C.show() ;
  cout << endl ;

  transformer t2(t1) ;
  t2.Z1 = (140.-I*100)*Ohm ;
  t2.Z2 = 50*Ohm ;
  sd = t2.get_data() ;
  complex S11 = sd.S.get(1,1) ;
  complex Zin = device::Z0 * (1.+S11)/(1.-S11) ;
  cout << "140-j100 to 50 ohm transformer:" << endl ;
  cout << "Input impedance is:  " << Zin << endl ;

  circuit ckt1 ;
  ckt1.connect(t1, 2, t2, 1) ;  // cascade the transformers
  ckt1.add_port(t1, 1) ;
  ckt1.add_port(t2, 2) ;
  
  sd = ckt1.get_data() ;
  cout << "50 to 140+j100 Ohm and 140-j100 to 50 Ohm transformer cascade" ;
  cout << endl ;
  cout << "S matrix: " << endl ;
  sd.S.show() ;
  cout << "C matrix: " << endl ;
  sd.C.show() ;
  cout << endl ;
}


