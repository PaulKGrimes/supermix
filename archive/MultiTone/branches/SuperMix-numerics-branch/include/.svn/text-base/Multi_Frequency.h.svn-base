
// Multi_Frequency.h
//
// Original code in "MultiFreq.h" by Phichet Kittara
//
// Object representing a collection of frequencies. 
//
/* Change Log:
*    22/04/04 : PKG : Added int get_h(long double freq) method to provide
*               access to order of harmonic at frequency freq.
*    03/05/04 : PKG : Replaced DOUBLEPRECISION with FREQ_TOL, and provided accessors,
*               Replaced #define'd complex with explicit std::complex<long double>
*               Found no changes between PK versions of MultiFreq.cpp that apply to this file.
*    01/07/04 : PKG : Changed class to use SuperMix based storage, as part of 
*               program to replace all storage with SuperMix classes, prior to
*               extension to multiple junctions.
*/

#ifndef MultiFrequencyH
#define MultiFrequencyH

#include <vector>
#include "supermix.h"

#include "One_Frequency.h"

class Multi_Frequency
{
private:
    std::vector<One_Frequency> Frequencies; /*!< Vector of One_Frequency objects */
    
    struct Ck { double Freq; Complex Ckf; }; /*!< Struct for correlating frequencies and Ck values */

    std::vector<Ck> CkSpectrum; /*!< Vector of Ck structs holding results */
    
    double F0; /*!< Normalising frequency */
    double min_Ck; /*!< Amplitude of smallest Ck to keep */
    double FREQ_TOL; /*!< Smallest difference betwen frequencies regarded as different frequencies*/
    bool CkValidFlag;
    
public:
    Multi_Frequency();    /*!< Constructor, initialises a single frequency with one harmonic */
    
    void add_frequency();                     /*!< Add a new One_Frequency to the frequency vector */
    void add_frequency(double freq);     /*!< Add a new One_Frequency of frequency freq */
    void remove_frequency(int f);             /*!< Remove fth One_Frequency */

    void add_harmonic(int f);                 /*!< Add a harmonic to the fth frequency */
    void add_harmonic(int f, Complex Vs, Complex Zt); /*!< Add a harmonic to the fth frequency */
    void remove_harmonic(int f);              /*!< Remove a harmonic from the fth freqeuncy */
        
    int get_no_freq() { return Frequencies.size(); };  /*!< Returns the no of frequencies */
    int get_no_harm(int f);                            /*!< Return the no of harmonics at fth frequency */
    int get_no_Ck() { return CkSpectrum.size(); };    /*!< Return the besselOrder used in Ck */
    
    double get_freq_tol() { return FREQ_TOL; }
    void set_freq_tol(double tol) { FREQ_TOL = tol; }
    
    double get_freq(int f);                 /*!< Return frequency of fth One_Frequency object */
    double get_freq(int f, int h);          /*!< Return frequency of hth harmonic of fth frequency */
    int get_f(double freq);                 /*!< Return index of One_Frequency containing harmonic at freq */
    int get_h(double freq);                 /*!< Return order of harmonic at freq */
    void set_freq(int f, double newFreq);   /*!< Set frequency of fth One_Frequency */
    
    double get_F0() { return F0; };         /*!< Return the normalising frequency */
    void set_F0(double newF0);              /*!< Set the normalising frequency */
    
    double get_min_Ck();                    /*!< Get the minimum Ck amplitude that will be kept */
    void set_min_Ck(double min);            /*!< Set the minimum Ck amplitude to keep */
    
    Complex get_Vn(int f, int h) { return Frequencies.at(f).get_Vn(h); }; /*!< Return the Nonlinear voltage associated with the hth harmonic of the fth frequency */
                
    void set_Vn(int f, int h, Complex V); /*!< Set the Nonlinear voltage associated with the hth harmonic of the fth frequency */
    
    void set_Ck();  /*!< Function that calculates the CkSpectrum */
    bool CkValid() { return CkValidFlag; }
    
    Complex get_Vs(int f, int h) { return Frequencies.at(f).get_Vs(h); }; /*!< Return the source voltage associated with the hth harmonic of the fth frequency */
                
    void set_Vs(int f, int h, Complex V); /*!< Set the source voltage associated with the hth harmonic of the fth frequency */
                
    Complex get_Zt(int f, int h) { return Frequencies.at(f).get_Zt(h); };  /*!< Return the embedding impedance of the hth harmonic of the fth frequency */
                
    void set_Zt(int f, int h, Complex Z);  /*!< Set the embedding impedance of the hth harmonic of the fth frequency */
    
    Complex get_Ck(int k) { return CkSpectrum[k].Ckf; };  /*!< Return the kth Ck value */
    Complex get_Ck(double freq); /*!< Return the Ck value at frequency freq */
    Complex get_Ck(int f, int h);      /*!< Return the CK value at frequency of hth harmonic of fth frequency */
    double get_Ck_freq( int k ) { return CkSpectrum[k].Freq; }; /*!< Return the frequency associated with the kth Ck value */
            
};

#endif


