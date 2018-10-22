// Test optimizer.
//
// This test program does not test all features of the optimizer.
//
#include "supermix.h"

int main()
{
  device::T = 100.0 * Kelvin;

  // define some resistors and capacitors
  resistor r1;
  r1.series();
  r1.R = 40.0 * Ohm;
  resistor r2(r1) ;
  r2.R = 80.0 * Ohm;

  capacitor c1 ;
  c1.parallel() ;
  c1.C = 100.*pFarad ;
  capacitor c2(c1) ;
  c2.C = 200*pFarad ;

  // connect them as parallel RC circuits
  circuit ckt1, ckt2 ;
  ckt1.connect(r1, 2, c1, 1) ;
  ckt1.add_port(r1, 1) ;
  ckt1.add_port(c1, 2) ;
  ckt2.connect(r2, 2, c2, 1) ;
  ckt2.add_port(r2, 1) ;
  ckt2.add_port(c2, 2) ;

  // define the optimization band
  sweeper band1;
  band1.sweep(device::f, 1.*GHz, 10.*GHz, 0.1*GHz) ;

  // define the error function to match circuit 1 to circuit 2
  error_func ef1 ;
  two_match tm1(ckt1, ckt2) ;
  ef1.add_term(1.0, band1, tm1);
  
  // define which parameter(s) are optimized
  r2.R = ef1.vary(1.*Ohm, 100.*Ohm, 1000.*Ohm) ; 
  c2.C = ef1.vary(1.*pFarad, 500.*pFarad, 10000.*pFarad) ; 

  // we will use Powell algorithm for minimizing
  powell pow1(ef1) ;
  pow1.verbose() ; // see the iterations as they occur
  //pow1.set_target(1.e-2) ;  // optimizer will quit when error function
                            // drops below this target value

  // let's try minimizing !
  double answer = pow1.minimize();
  cout << "Final error function value is: " << answer << endl ;
  cout << "Final parameters are: " ;
  ef1.get_parms().show() ;
  cout << "Double-check the parameter values: " 
       << r2.R 
       << "  " 
       << c2.C 
       << endl ;
  cout << "Parameters with units: "
       << r2.R/Ohm << " ohms," 
       << "  " 
       << c2.C/pFarad << " pF" 
       << endl ;
}

