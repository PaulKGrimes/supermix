// One_Harmonic.h
//
// Original code in "Complex_One_Harmonic.h" by Phichet Kittara
//
// Object representing a single harmonic.
//
// Accessor functions automatically recalculate An vector when values are altered
//

/* Change Log:
//    15/04/04 : PKG : Added parameters (with accessors) to store Vs, Zt, and Vn
//                     locally.  Removed Cv_p and accessors, and altered set_An()
//                     to deal with this change.
//    03/05/04 : PKG : Changed names of <g|s>et_totalJ(<|int>) to <g|s>et_no_Ck(<|int>)
//                      Removed #define complex, replaced all complex with std::complex<long double>
*/

#ifndef One_HarmonicH
#define One_HarmonicH

#include <vector>
#include <complex>

#include "BesselFn.h"

class One_Harmonic {

public:
    One_Harmonic();
    One_Harmonic(int,int,std::complex<long double>,long double,long double);

    std::complex<long double> get_An(int);   // Return a An value
    void set_An();         // Recalculate An vector
        
    // Accessors
    long double get_Fp() { return Fp; }; // Return frequency of harmonic
    long double get_F0() { return F0; }; // Return normalising frequency
    int get_p() { return p; };           // Return order of harmonic
    int get_no_Ck() { return totalJ; };  // Return maximum order of An
    std::complex<long double> get_Vs() { return Vs; };     // Return source voltage
    std::complex<long double> get_Zt() { return Zt; };     // Return source embedding impedance
    std::complex<long double> get_Vn() { return Vn; };     // Return nonlinear voltage
    
    void set_Fp(long double F);          // Set frequency of harmonic
    void set_F0(long double F);          // Set normalising frequency
    void set_p(int P);                   // Set order of harmonic
    void set_no_Ck(int J);               // Set number of An coefficients
    void set_Vs(std::complex<long double> V);   // Set source voltage
    void set_Zt(std::complex<long double> Z);   // Set source embedding impedance
    void set_Vn(std::complex<long double> V);   // Set nonlinear voltage

private:
    BesselFn BJ;

    int totalJ, p;
    long double Fp, F0;
    std::complex<long double> Vs, Zt, Vn;

    std::vector< std::complex<long double> > An;

};
#endif


