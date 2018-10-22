// test_microstrip.cc
// 9/16/98: changed to support new member names

#include "supermix.h"

void calculate(microstrip &ms);  /* calculate and print out microstrip props */

int main() 
{
  //
  // First define metal films(s)
  //

  super_film nb1;

  // Standard parameters for Niobium

  nb1.Vgap.set(2.9*mVolt) ;
  nb1.Tc.set(9.2*Kelvin) ;
  //
  double cm = 1.e4 * Micron;
  double Micro = 1.e-6;
  nb1.rho_normal.set(5.*Micro*Ohm*cm) ;

  // Set thickness
  nb1.Thick.set(2000.*Angstrom) ;

  //
  // Now define dielectrics
  //

  const_diel sio ;
  const_diel vacuum ;

  vacuum.eps.set(1.0) ;
  vacuum.tand.set(0.0) ;

  sio.eps.set(5.6) ;   /* dielectric constant */
  sio.tand.set(0.) ;   /* loss tangent */

  //
  // Now make a microstrip line
  //

  microstrip ms1;

  ms1.top_strip(nb1) ;                 /* use Nb film for top strip */
  ms1.ground_plane(nb1) ;             /* same for ground plane     */
  ms1.superstrate(vacuum) ;           /* nothing above */
  ms1.substrate(sio) ;                /* SiO insulator */
  ms1.sub_thick.set(2000.*Angstrom) ;  /* SiO thickness */

  ms1.length.set(100.0*Micron) ; /* microstrip length */
  ms1.width.set(5.0*Micron) ;    /* microstrip width */


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
  nbtin.Vgap.set(5.2*mVolt) ;
  nbtin.Tc.set(15.2*Kelvin) ;
  //
  nbtin.rho_normal.set(60.*Micro*Ohm*cm) ;
  //
  // Set thickness
  nbtin.Thick.set(2000.*Angstrom) ;

  // Thin niobium layer
  nb1.Thick.set(10.*Angstrom) ;

  layerList bilayer ;

  bilayer.addlayer(nb1) ;   /* 100 Angstrom niobium film     */
  bilayer.addlayer(nbtin) ; /* on top of 2000 Angstrom NbTiN */

  // Use these in a microstrip line
  ms1.ground_plane(bilayer) ;
  ms1.top_strip(nbtin) ;

  calculate(ms1) ;
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
  return ;
}
