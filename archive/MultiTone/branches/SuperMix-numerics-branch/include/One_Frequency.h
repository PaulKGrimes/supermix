// One_Frequency.h
//
// Original code in "One_Frequency.h" by Phichet Kittara
//
// Object representing a frequency made up of a number of harmonics.
//
/* Change Log:
*    15/04/04 : PKG : Removed Vn, Zt and Vs vectors, as these are now stored
*               within each harmonic.
*               Altered accessors to Vn, Zt and Vs to work with this
*    03/05/04 : PKG : Merge changes from PK's final version:
*               Remove C2k vector and related functions.
*               Slight alterations to accuracy checking code.
*               Remove #define of complex, and replace all instances with
*               std::complex<long double>
*    01/07/04 : PKG : Changed class to use SuperMix based storage, as part of 
*               program to replace all storage with SuperMix classes, prior to
*               extension to multiple junctions.
*/

#ifndef One_FrequencyH
#define One_FrequencyH

#include "One_Harmonic.h"
#include "supermix.h"
#include <vector>

class One_Frequency
{
private:
    std::vector<One_Harmonic> harmonics; // Vector list of harmonics

    complex_vector Cjk;                  // Vector of Cjk factors

    double F;                       // Fundamental frequency
    double F0;                      // Normalising frequency
    
    double delta_Ck, delta_Ck_0;    // Actual and target accuracy of Ck values

    void set_Cjk();
    void set_An();
    
public:
    One_Frequency();
    
    void add_harmonic();
    void add_harmonic(Complex, Complex);
    void remove_harmonic();
    void resetToZero();
    
    // Data return
    Complex get_Cjk(int, int);
    Complex get_Cjk(int);
     
    // Read Accessors
    int get_no_harm() { return harmonics.size(); };

    double get_F()        { return F; };
    double get_F0()       { return F0; };
    double get_delta_Ck() { return delta_Ck; };
    double get_delta_Ck0(){ return delta_Ck_0; };
    Complex get_Vn(int p) { return harmonics.at(p).get_Vn(); };
    Complex get_Zt(int p) { return harmonics.at(p).get_Zt(); };
    Complex get_Vs(int p) { return harmonics.at(p).get_Vs(); };
    int get_no_Ck()       { return harmonics[0].get_no_Ck(); };
    
    // Write Accessors
    void set_F(double);
    void set_F0(double);
    void set_Vn(int, Complex);
    void set_Zt(int, Complex);
    void set_Vs(int, Complex);
    void set_delta_Ck0(double); // Sets delta_Ck_0, the target accuracy
};

#endif

