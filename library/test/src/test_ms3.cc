// test_ms3.cc
// 9/22/00: fixed Nb Tc. Now Tc = 9.2 Kelvin.
// 9/16/98: changed to support new member names
// 9/15/98: added line to turn on error::messages to support new surfaceZ

#include "supermix.h"

void calculate(microstrip &ms);  /* calculate and print out microstrip props */

int main() 
{
  Complex::rel_tolerance = 1.0e-14;  // get rid of tiny imaginary parts

  error::messages = 1;

  // default temp
  device::T = 4.2*Kelvin ;
  device::Z0 = 50.*Ohm ;

  //
  // First define metal films(s)
  //

  super_film nb1;

  // Standard parameters for Niobium

  nb1.Vgap = 2.9*mVolt ;
  nb1.Tc = 9.2*Kelvin ;

  double cm = 1.e4 * Micron;
  nb1.rho_normal = 5.*Micro*Ohm*cm ;

  // Set thickness
  nb1.Thick = 2000.*Angstrom ;

  //
  // Now define dielectrics
  //

  const_diel sio ;
  const_diel vacuum ;

  vacuum.eps = 1.0 ;
  vacuum.tand = 0.0 ;

  sio.eps = 5.6 ;   /* dielectric constant */
  sio.tand = 0. ;   /* loss tangent */

  //
  // Now make a microstrip line
  //

  microstrip ms1;

  ms1.top_strip(nb1) ;                 /* use Nb film for top strip */
  ms1.ground_plane(nb1) ;             /* same for ground plane     */
  ms1.superstrate(vacuum) ;           /* nothing above */
  ms1.substrate(sio) ;                /* SiO insulator */
  ms1.sub_thick = 2000.*Angstrom ;  /* SiO thickness */

  ms1.length = 100.0*Micron ; /* microstrip length */
  ms1.width = 5.0*Micron ;    /* microstrip width */


  //
  // Let's calculate
  //
  calculate(ms1) ;

  cout << endl ;

  //
  // Now check out how Nb/NbTiN bilayer works
  //

  // Standard parameters for NbTiN
  //
  super_film nbtin ;
  nbtin.Vgap = 5.2*mVolt ;
  nbtin.Tc = 15.2*Kelvin ;
  //
  nbtin.rho_normal = 60.*Micro*Ohm*cm ;
  //
  // Set thickness
  nbtin.Thick = 2000.*Angstrom ;

  // Thin niobium layer
  nb1.Thick = 10.*Angstrom ;

  layerList bilayer ;

  bilayer.addlayer(nb1) ;   /* 100 Angstrom niobium film     */
  bilayer.addlayer(nbtin) ; /* on top of 2000 Angstrom NbTiN */

  // Use these in a microstrip line
  ms1.ground_plane(bilayer) ;
  ms1.top_strip(nbtin) ;

  calculate(ms1) ;

  // 1995 Twin-slot SIS

  super_film nbi ;  // make interpolator
  nbi.Vgap = nb1.Vgap ;
  nbi.Tc = nb1.Tc ;
  nbi.rho_normal = nb1.rho_normal ;
  nbi.tol = 1.e-3 ; // desired tolerance
  nbi.Thick = 2000.*Angstrom ;
  
  //super_film nbi = nb1 ;  // uncomment if you don't want to use interpolation

  ms1.width = 2.*Micron ;
  ms1.length = 45.9*Micron ;
  
  ms1.ground_plane(nbi) ;
  ms1.top_strip(nbi) ;
  ms1.sub_thick = 4000.*Angstrom ;

  microstrip ms2(ms1) ;               // use copy constructor to duplicate
  ms2.length = 50.1*Micron ;          // change width, length
  ms2.width = 4.7*Micron ;

  microstrip ms3(ms2) ;
  ms3.sub_thick = 2000.*Angstrom  ;    /* Change SiO thickness */
  ms3.length = 2.5*Micron ;
  ms3.width = 5.*Micron ;

  microstrip ms4(ms3) ;
  ms4.length = 7.5*Micron ;
  ms4.width = 5.*Micron ;

  resistor R1 ;
  R1.parallel() ;
  R1.R = 11.8*Ohm ;

  capacitor C1 ;
  C1.parallel() ;
  C1.C = 144.*fFarad ;

  resistor shrt ;
  shrt.parallel() ;
  shrt.R = 0.000001*Ohm ;

  circuit ckt1 ;

  ckt1.connect(ms1, 2, ms2, 1) ;
  ckt1.connect(ms2, 2, ms3, 1) ;
  ckt1.connect(ms3, 2, R1, 1) ;
  ckt1.connect(R1, 2, C1, 1) ;
  ckt1.connect(C1, 2, ms4, 1) ;
  //  ckt1.connect(ms4, 2, shrt, 1) ;

  ckt1.add_port(ms1, 1) ;
  ckt1.add_port(ms4, 2) ;
  //ckt1.add_port(shrt, 2) ;

  double freq;
  complex S11, S21 ;
  for(freq = 100. ; freq <= 1000.; freq += 10) {
    device::f = freq*GHz ;
    //S11 = ms2.S(1,1) ;
    //S21 = ms2.S(2,1) ;
    S11 = ckt1.S(1,1) ;
    S21 = ckt1.S(2,1) ;
    cout << freq << "   " 
	 << zabs(S11) << "   " 
	 << arg(S11)/Degree << "   "
	 << zabs(S21) << "   " 
	 << arg(S21)/Degree << "   "
	 << endl ;
  }
}


void calculate(microstrip &ms)
{
  double Temp = 4.2*Kelvin ;
  complex zch, prop ;
  double wfree, wavelen, cbar, alpha, percent ;

  double freq;
  for(freq = 100.*GHz ; freq <= 1000.*GHz; freq += 100.*GHz) {
    zch = ms.Zchar(freq, Temp) ;
    prop = ms.Kprop(freq, Temp) ;
    wavelen = 2.*Pi/imag(prop) ;
    wfree = cLight/freq ;
    cbar = wavelen/wfree ;
    alpha = real(prop) ;
    percent = 100.*(1. - exp(-2.*alpha*wavelen)) ;
    
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
  }
  cout << endl ;
  
  device::f = 500.*GHz ;
  sdata sd = ms.get_data() ;
  sd.S.show() ;
  (sd.C / Kelvin).show() ;

  cout << endl ;
  cout << endl ;

  return ;
}


