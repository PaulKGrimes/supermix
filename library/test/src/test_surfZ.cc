#include <iostream>
#include <cmath>
#include "SIScmplx.h"
#include "units.h"
#include "surfaceZ.h"

using namespace std;


int main() 
{

   super_film nb1;

   // Standard parameters for Niobium
   //
   nb1.Vgap.set(2.9*mVolt) ;
   nb1.Tc.set(9.2*Kelvin) ;
   //
   double cm = 1.e4 * Micron;
   double Micro = 1.e-6;
   nb1.rho_normal.set(5.*Micro*Ohm*cm) ;
   //
   // Set thickness
   nb1.Thick.set(1000.*Angstrom) ;

   // Copying works
   super_film nb2 = nb1;

   nb2.Thick.set(2000.*Angstrom) ;
 
   layerList bilayer;         /* See if two 1000 Angstrom films */
   bilayer.addlayer(nb1) ;   /* end up the same as 2000 A film */
   bilayer.addlayer(nb1) ;

   // Check thicknesses
   cout << nb1.thickness()/Angstrom << "   " 
        << nb2.thickness()/Angstrom << "   "
        << bilayer.thickness()/Angstrom << "   "
        << endl ;


   // Calculate surface impedance
   //
   double T = 4.2*Kelvin ;

   complex sig1, sig2, sig3;
   double freq;
   for(freq = 100.*GHz ; freq <= 1000.*GHz; freq += 100.*GHz) {
      sig1 = nb1.Zsurf(freq, T) ;
      sig2 = nb2.Zsurf(freq, T) ;
      sig3 = bilayer.Zsurf(freq, T);
      cout << freq/GHz 
           << "   " 
           << real(sig1)/Ohm 
           << "   " 
           << real(sig2)/Ohm 
           << "   " 
           << real(sig3)/Ohm 
           << "   " 
           << imag(sig1)/Ohm 
           << "   " 
           << imag(sig2)/Ohm 
           << "   " 
           << imag(sig3)/Ohm 
           << "\n" ; 
   }
   
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
   nb1.Thick.set(100.*Angstrom) ;

   // now make bilayer
   bilayer.remove() ; /* first get rid of previous definition */
   bilayer.remove() ; 

   bilayer.addlayer(nb1) ;   /* Thin niobium    */
   bilayer.addlayer(nbtin) ; /* on top of NbTiN */


   cout << endl << endl ;
   // Check thicknesses
   cout << nb1.thickness()/Angstrom << "   " 
        << nbtin.thickness()/Angstrom << "   "
        << bilayer.thickness()/Angstrom << "   "
        << endl ;


   for(freq = 100.*GHz ; freq <= 1000.*GHz; freq += 100.*GHz) {
      sig1 = nb1.Zsurf(freq, T) ;
      sig2 = nbtin.Zsurf(freq, T) ;
      sig3 = bilayer.Zsurf(freq, T);
      cout << freq/GHz 
           << "   " 
           << real(sig1)/Ohm 
           << "   " 
           << real(sig2)/Ohm 
           << "   " 
           << real(sig3)/Ohm 
           << "   " 
           << imag(sig1)/Ohm 
           << "   " 
           << imag(sig2)/Ohm 
           << "   " 
           << imag(sig3)/Ohm 
           << "\n" ; 
   }


}


