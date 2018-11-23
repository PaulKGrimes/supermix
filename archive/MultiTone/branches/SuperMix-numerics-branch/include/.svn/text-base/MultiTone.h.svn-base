/* MultiTone.h
*
* Written by Paul Grimes, derived from code by Phichet Kittara
*
* Definition of an overall multitone analysis class, containing a MultiFreq 
* phasor calculator object and a ResponseFn object.  The object carries out
* the harmonic balance and current calculations.
*
* Frequencies are referred to by index.  The index of a Frequency at freq can
* be found by calling get_f(freq) 
*/

/* Changes
*    02/05/04 : PKG : Changed all vectors and matrices in harmonic balance calculation into
*               long doubles from complex, as otherwise the jacobian is wrong.  each of these
*               must be mapped from complex[i] to double[i*2]
*    01/07/04 : PKG : Massive changes to use SuperMix based matrix and vector storage 
*               classes.  MultiTone is now derived from SuperMix's newton class 
*               (implementation of a Newton Raphson algorithm).  This change requires that
*               MultiTone supplies an implementation of the virtual function calc() that
*               sets F(X) and the Jacobian; and that the class sets some initial value of
*               the X vector.
*               Significantly simplified by removing long double options.
*               This MultiTone class then acts as a single junction idealised mixer with
*               embedding circuits represented by the embedding impedances.  
*               This is quite a monolithic class at the moment, but it should be relatively
*               simple to extend this method to multiple junctions and the scattering matrix
*               formulation of SuperMix's mixer class.
*              
*/

#ifndef MultiToneH
#define MultiToneH

#include "MultiTone_exceptions.h"
#include "Multi_Frequency.h"
#include "ResponseFn.h"
#include "supermix.h"


class MultiTone : private newton
{
private:
    // The objects that do a lot of the work
    Multi_Frequency multifreq; 
    ResponseFn response;

    // Bias at which last harmonic balance was carried out
    double biasV; 

    // Struct to hold frequency and harmonic indexs for harmonics to be included
    struct validFreq { int f; int h; };
    // Vector holding the indices of frequencies and harmonics that should be included
    std::vector<validFreq> validFreqs;
    void setValidFreqs();
    
    // Functions for carrying out harmonic balance
    void calc();
    void setJacobian();
    void functionEvaluation(real_vector &X, real_vector &Y);
    double step_dX;  // Step size used in calculating Jacobian
    double min_dX;   // Minimum step size to be used in calculating Jacobian
    double freq_tol; // Difference below which two frequencies are assumed to be the same
    int iterations;  // Number of iterations
    
    // Some internal states
    bool readyFlag;
    bool dataValidFlag;
    
public:
    MultiTone();

    // Temporarily public: for debug purposes
    Complex get_Ip(double freq);  // Used for balancing the mixer
    real_vector get_x() { return newton::get_x(); }
    real_vector get_fval() { return newton::get_fval(); }
    real_matrix get_Jacobian() { return newton::get_Jacobian(); }
    
    Complex get_Ck(int k) { return multifreq.get_Ck(k); }
    Complex get_Ck(double f) { return multifreq.get_Ck(f); }
    Complex get_Ck_freq(int k) { return multifreq.get_Ck_freq(k); }

    
    // Methods that do the real work (and change state)
    void initHarmonicBalance();
    bool ready();
    void balance();
    bool dataValid();
    void set_bias(double V);
    int get_iterations() { return iterations; }

    Complex get_I(double V, int f, int h);
    Complex get_I(double V, double freq, int h);
    Complex get_I(double V, double freq);

    Complex get_I(int f, int h);
    Complex get_I(double freq, int h);
    Complex get_I(double freq);
        
    // Multi_Freq accessors
    void add_frequency();
    void add_frequency(double freq);         
    void remove_frequency(int f);            
    void remove_frequency(double freq);      
    
    void add_harmonic(int f);               
    void add_harmonic(double freq);         
    void add_harmonic(int f, Complex Vs, Complex Zt);
    void add_harmonic(double freq, Complex Vs, Complex Zt);     
    void remove_harmonic(int f);           
    void remove_harmonic(double freq);     
    
    int get_no_freq();                      
    int get_no_harm(int f);                 
    int get_no_harm(double freq);           
    int get_no_Ck();                        
    
    double get_freq(int f);             /*!< Return frequency of f'th frequency */
    double get_freq(int f, int h);      /*!< Return frequency of h'th harmonic of f */
    void set_freq(int f, double newFreq); /*!< Set fundamental frequency of f'th frequency */
    void set_freq(double oldFreq, double newFreq); /*!< Change fundamental frequency of frequency at oldFreq */
    int get_f(double freq);             /*!< Return index of frequency containing harmonic at freq */
    int get_h(double freq);             /*!< Return order of harmonic at frequency freq */
    
    double get_F0();                    /*!< Return normalising frequency */
    void set_F0(double newF0);          /*!< Set normalising frequency */
    
    double get_min_Ck();                /*!< Get minimum Ck amplitude kept in Ck spectrum */
    void set_min_Ck(double c);                       /*!< Set minimum Ck amplitude to be kept in Ck spectrum */
    
    Complex get_Vn(int f, int h);
    Complex get_Vn(double freq, int h);
    Complex get_Vn(double freq);
    
    void set_Vn(int f, int h, Complex V);
    void set_Vn(double freq, int h, Complex V);
    void set_Vn(double freq, Complex V);

    Complex get_Zt(int f, int h);
    Complex get_Zt(double freq, int h);
    Complex get_Zt(double freq);
    
    void set_Zt(int f, int h, Complex Z);
    void set_Zt(double freq, int h, Complex Z);
    void set_Zt(double freq, Complex Z);

    Complex get_Vs(int f, int h);
    Complex get_Vs(double freq, int h);
    Complex get_Vs(double freq);
    
    void set_Vs(int f, int h, Complex V);
    void set_Vs(double freq, int h, Complex V);
    void set_Vs(double freq, Complex V);
     
    // ResponseFn accessors
    bool responseFnReady();
    void kennedy(int n, int noPoints);
    void data(std::string dcFile);
    void data(std::string dcFile, std::string kkFile);
    
    void set_vGap(double V);
    double get_vGap();
    void set_iGap(double I);
    double get_iGap();
    void set_Rn(double R);
    double get_Rn();
    void set_vMax(double V);
    double get_vMax();
    
    std::string get_idcFile();
    int get_no_points();
    
    double get_unpumped_Idc(double V);
    double get_unpumped_Ikk(double V);
};


#endif
