// test_cpw.cc
//
// Test the coplanar waveguide class (class cpw)


#include "supermix.h"

// Function to calculate a cpw and print out its properties.
void calculate(cpw &wg);

int main() 
{
  // First, define a metal films

  super_film nb;

  // Standard parameters for Niobium

  nb.Vgap.set(2.9*mVolt) ;
  nb.Tc.set(9.2*Kelvin) ;
  double cm = 1.e4 * Micron;
  double Micro = 1.e-6;
  nb.rho_normal.set(5.*Micro*Ohm*cm) ;

  // Set niobium film thickness
  nb.Thick.set(2000.*Angstrom) ;

  //
  // Define dielectrics
  //

  const_diel gaas ;

  gaas.eps.set(12.9) ;	// dielectric constant
  gaas.tand.set(0.) ;   // loss tangent

  //
  // Now make a cpw line
  //

  cpw wg1;

  wg1.top_strip(nb) ;               // use Nb film for top strip
  wg1.substrate(gaas) ;             // GaAs insulator
  wg1.sub_thick.set(500.*Micron) ;  // GaAs thickness

  wg1.length.set(1.0*Centi*Meter) ; // cpw length
  wg1.width.set(15.0*Micron) ;      // cpw trace width
  wg1.space.set(5.0*Micron) ;       // cpw gap width


  //
  // Calculate the wave guide.
  //
  cout << "width = " << wg1.width / Micron << " space = "
       << wg1.space / Micron << endl;
  calculate(wg1) ;

  cout << endl ;

  wg1.width.set(5.0*Micron) ;    // cpw trace width
  wg1.space.set(10.0*Micron) ;    // cpw gap width
  cout << "width = " << wg1.width / Micron << " space = "
       << wg1.space / Micron << endl;
  calculate(wg1) ;
  cout << endl ;
}


void calculate(cpw &wg)
{
  double Temp = 4.2*Kelvin ;
  complex zch, prop ;
  double wfree, wavelen, cbar, alpha, percent ;

  double freq;
  for(freq = 1.*GHz ; freq <= 10.*GHz; freq += 1.*GHz) {
    zch = wg.Zchar(freq, Temp) ;
    prop = wg.Kprop(freq, Temp) ;
    wavelen = 2.*Pi/imag(prop) ;
    wfree = cLight/freq ;
    cbar = wavelen/wfree ;
    alpha = real(prop) ;
    percent = 100.*(1. - exp(-2.*alpha*wavelen)) ;
    
/*
    cout << freq/GHz
         << "   "
         << real(zch)
         << "   "
         << imag(zch)
         << "   "
         << wavelen/Micron
         << "   "
         << cbar
         << "   "
         << alpha*Micron
         << "   "
         << percent
         << endl ;
*/
    device::f = freq;
    sdata sd = wg.get_data();
    sd.S = transpose(sd.S);
    complex::out_degree();
    cout << fixed << setw(7) << setprecision(3) << freq/GHz << "  "
         << general << setprecision(6) << showpoint
         << sd.S
         << endl;

  }
  return ;
}
