// 10/1/99:  modified for new real_interp and complex_interp implementations
// 7/7/99:   modified for new complex_interp implementation
// 9/15/98:  turned on error messages.

#include <iostream>
#include "datafile.h"
#include "units.h"
#include "real_interp.h"
#include "complex_interp.h"
#include "parameter/real_parameter.h"
#include "parameter/complex_parameter.h"
#include "error.h"

using namespace std;

int main(int argc, char **argv) 
{
  error::messages = 1;
  if(argc < 2) {
    cerr << "Need to specify input file name on command line\n" ;
  }
  else {
    real_parameter x;
    datafile df(argv[1]) ;           // read data file
    //    real_interp Ra(df, 1, 2, x) ;    // and create interpolators
    real_interp Ra(x, argv[1]) ;
    //    real_interp Xa(df, 1, 3, x) ;
    real_interp Xa(x, argv[1], 1.0, 1.0, 2) ;  // the imaginary part
    complex_interp Za1(df, 1, 2, 3, x) ;
    complex_interp Za2;
    Za2.parameter(x).file(argv[1]);
    //    Za2.table(df, 1, 2, 3, complex_interp::CARTESIAN) ;

    //    real_interp phse(df, 1, 3, x, real_interp::PHASE) ;

    real_parameter Ra_ref = &Ra;     // see if we can reference them
    real_parameter Xa_ref = &Xa;

    // a shorter syntax
    // real_parameter test = new real_interp(df, 1, 2, x) ;

    complex_parameter ctst(Ra, Xa, complex_parameter::CARTESIAN) ;

    // other allowed forms:
    // complex_parameter ctst(Ra, Xa) ;   // defaults to cartesian
    // complex_parameter ctst(mag, phs, complex_parameter::POLAR) ;
    // and also the modes similar to real_parameters:
    // complex_parameter ctst(Complex &, abstract_complex_parameter *, etc) ;

    double xmin = df.read(1,1) ;
    double xmax = df.read(1, df.numlines()) ;

    int n=1000 ;
    double dx = (xmax-xmin)/n ;
    x = xmin ;

    for(int i=0; i<n; i++) {
      cout << x
	   << "   "
	   << Ra_ref
	   << "   "
	   << Xa_ref
	   << "   "
	   << real(Za1) << "  " << imag(Za1) << "  "
	   << real(Za2) << "  " << imag(Za2) << "  "
	   << real(ctst) << "  " << imag(ctst)
	   << endl ;
      x += dx ;
    }
  }
}

