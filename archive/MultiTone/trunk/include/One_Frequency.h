// One_Frequency.h
//
// Original code in "One_Frequency.h" by Phichet Kittara
//
// Object representing a frequency made up of a number of harmonics.
//
/* Change Log:
*     15/04/04 : PKG : Removed Vn, Zt and Vs vectors, as these are now stored
*                      within each harmonic.
*                      Altered accessors to Vn, Zt and Vs to work with this
*     03/05/04 : PKG : Merge changes from PK's final version:
*                     Remove C2k vector and related functions.
*                     Slight alterations to accuracy checking code.
*                     Remove #define of complex, and replace all instances with std::complex<long double>
*/

#ifndef One_FrequencyH
#define One_FrequencyH

#include "One_Harmonic.h"
#include <vector>
#include <complex>

class One_Frequency
{
private:
    std::vector<One_Harmonic> harmonics; // Vector list of harmonics

    std::vector<std::complex<long double> > Cjk;            // Vector of Cjk factors
    
    long double F;                       // Fundamental frequency
    long double F0;                      // Normalising frequency
    
    long double delta_Ck, delta_Ck_0;    // Actual and target accuracy of Ck values
    
    void set_Cjk();
    void set_An();
    
public:
    One_Frequency();
    
    void add_harmonic();
    void add_harmonic(std::complex<long double>, std::complex<long double>);
    void remove_harmonic();
    void resetToZero();
    
    // Data return
    std::complex<long double> get_Cjk(int, int);
    std::complex<long double> get_Cjk(int);
     
    int get_no_harm() { return harmonics.size(); };

    // Read Accessors
    long double get_F() { return F; };
    long double get_F0() { return F0; };
    long double get_delta_Ck() { return delta_Ck; };
    long double get_delta_Ck0() { return delta_Ck_0; };
    std::complex<long double> get_Vn(int p) { return harmonics.at(p).get_Vn(); };
    std::complex<long double> get_Zt(int p) { return harmonics.at(p).get_Zt(); };
    std::complex<long double> get_Vs(int p) { return harmonics.at(p).get_Vs(); };
    int get_no_Ck() { return harmonics[0].get_no_Ck(); };
    
    // Write Accessors
    void set_F(long double);
    void set_F0(long double);
    void set_Vn(int, std::complex<long double>);
    void set_Zt(int, std::complex<long double>);
    void set_Vs(int, std::complex<long double>);
    void set_delta_Ck0(long double); // Sets delta_Ck_0, the target accuracy
};

#endif

