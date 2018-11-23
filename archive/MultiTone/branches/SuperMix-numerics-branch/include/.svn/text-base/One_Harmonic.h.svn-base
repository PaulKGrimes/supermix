// One_Harmonic.h
//
// Original code in "Complex_One_Harmonic.h" by Phichet Kittara
//
// Object representing a single harmonic.
//
// Accessor functions automatically recalculate An vector when values are altered
//

/* Change Log:
*    15/04/04 : PKG : Added parameters (with accessors) to store Vs, Zt, and Vn
*               locally.  Removed Cv_p and accessors, and altered set_An() to 
*               deal with this change.
*    03/05/04 : PKG : Changed names of <g|s>et_totalJ(<|int>) to 
*               <g|s>et_no_Ck(<|int>)
*               Removed #define complex, replaced all complex with 
*               std::complex<long double>
*    01/07/04 : PKG : Changed class to use SuperMix based storage, as part of 
*               program to replace all storage with SuperMix classes, prior to
*               extension to multiple junctions.
*/

#ifndef One_HarmonicH
#define One_HarmonicH

#include "supermix.h"

#include "BesselFn.h"

class One_Harmonic {

public:
    One_Harmonic();
    One_Harmonic(int,int,Complex,double,double);
    Complex get_An(int);   // Return an An value
    void set_An();         // Recalculate An vector
        
    // Accessors
    double get_Fp() { return Fp; }; // Return frequency of harmonic
    double get_F0() { return F0; }; // Return normalising frequency
    int get_p() { return p; };           // Return order of harmonic
    int get_no_Ck() { return totalJ; };  // Return maximum order of An
    Complex get_Vs() { return Vs; };     // Return source voltage    
    Complex get_Zt() { return Zt; };     // Return source embedding impedance
    Complex get_Vn() { return Vn; };     // Return nonlinear voltage    
    void set_Fp(double F);          // Set frequency of harmonic
    void set_F0(double F);          // Set normalising frequency
    void set_p(int P);              // Set order of harmonic
    void set_no_Ck(int J);          // Set number of An coefficients
    void set_Vs(Complex V);         // Set source voltage
    void set_Zt(Complex Z);         // Set source embedding impedance
    void set_Vn(Complex V);         // Set nonlinear voltage

private:
    BesselFn BJ;

    int totalJ, p;
    double Fp, F0;
    Complex Vs, Zt, Vn;

    complex_vector An;
};
#endif


