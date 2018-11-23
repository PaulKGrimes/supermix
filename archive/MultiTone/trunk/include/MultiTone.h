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
*   02/05/04 : PKG : Changed all vectors and matrices in harmonic balance calculation into
*                   long doubles from complex, as otherwise the jacobian is wrong.  each of these
*                   must be mapped from complex[i] to double[i*2]
*
*/

#ifndef MultiToneH
#define MultiToneH

#include "MultiTone_exceptions.h"
#include "Multi_Frequency.h"
#include "ResponseFn.h"
#include "matrix.h"



class MultiTone
{
private:
    // The objects that do a lot of the work
    Multi_Frequency multifreq; 
    ResponseFn response;

    // Matrices for harmonic balance calculations
    matrix<long double> jacobian;
    vector<long double> OldX;
    vector<long double> NewX;
    vector<long double> OldY;
    vector<long double> deltaX;

    // Bias at which last harmonic balance was carried out
    long double biasV; 
    
    // Struct to hold frequency and harmonic indexs for harmonics to be included
    struct validFreq { int f; int h; };
    // Vector holding the indices of frequencies and harmonics that should be included
    vector<validFreq> validFreqs;
    void setValidFreqs();
    
    // Functions for carrying out harmonic balance
    void clearHarmonicBalance();
    void setJacobian();
    void setNewX();
    void functionEvaluation(vector<long double> & inX, vector<long double> &outY);
    
    // Numerical values affecting the harmonic balance
    int Max_Iterations;     // Largest number of iterations
    long double epsilon; // Conversion constraint
    long double freq_tol; // Difference in frequencies under which frequencies are considered to be equal
    long double min_dX;  // minimum step in calculation of Jacobian
    long double step_dX; // stepfactor in calculation of Jacobian
    
    // Some internal states
    bool readyFlag;
    bool dataValidFlag;
    int iterations;
    
public:
    MultiTone();

    // Temporarily public: for debug purposes
    complex get_Ip(long double freq);  // Used for balancing the mixer
    
    // Methods that do the real work (and change state)
    void initHarmonicBalance();
    bool ready();
    void balance();
    bool dataValid();
    void set_bias(long double V);
    int get_iterations() { return iterations; }

    complex get_I(long double V, int f, int h);
    complex get_I(long double V, long double freq, int h);
    complex get_I(long double V, double freq, int h);
    complex get_I(long double V, long double freq);
    complex get_I(long double V, double freq);

    complex get_I(int f, int h);
    complex get_I(long double freq, int h);
    complex get_I(double freq, int h);
    complex get_I(long double freq);
    
    // Harmonic balance control accessors
    void set_Max_Iterations(int i) { Max_Iterations = i; }
    int get_Max_Iterations() { return Max_Iterations; }
    void set_epsilon(long double e) { epsilon = e; }
    long double get_epsilon( long double e) { return epsilon; }
    void set_freq_tol(long double df) { freq_tol = df; multifreq.set_freq_tol(df);}
    long double get_freq_tol() { return freq_tol; }
    void set_min_dX(long double dx) { min_dX = dx; }
    long double get_min_dX() { return min_dX; }
    void set_step_dX(long double dx) { step_dX = dx; }
    long double get_step_dX() { return step_dX; }
    
    
    // Multi_Freq accessors
    void add_frequency();
    void add_frequency(long double freq);    
    void add_frequency(double freq);         
    void remove_frequency(int f);            
    void remove_frequency(long double freq); 
    void remove_frequency(double freq);      
    
    void add_harmonic(int f);               
    void add_harmonic(long double freq);    
    void add_harmonic(double freq);         
    void add_harmonic(int f, complex Vs, complex Zt);
    void add_harmonic(long double freq, complex Vs, complex Zt);
    void add_harmonic(double freq, complex Vs, complex Zt);     
    void remove_harmonic(int f);           
    void remove_harmonic(long double freq);
    void remove_harmonic(double freq);     
    
    int get_no_freq();                      
    int get_no_harm(int f);                 
    int get_no_harm(long double freq);      
    int get_no_harm(double freq);           
    int get_no_Ck();                        
    
    long double get_freq(int f);             /*!< Return frequency of f'th frequency */
    long double get_freq(int f, int h);      /*!< Return frequency of h'th harmonic of f */
    void set_freq(int f, long double newFreq); /*!< Set fundamental frequency of f'th frequency */
    void set_freq(long double oldFreq, long double newFreq); /*!< Change fundamental frequency of frequency at oldFreq */
    void set_freq(double oldFreq, long double newFreq); /*!< Change fundamental frequency of frequency at oldFreq */
    int get_f(long double freq);             /*!< Return index of frequency containing harmonic at freq */
    int get_h(long double freq);             /*!< Return order of harmonic at frequency freq */
    
    long double get_F0();                    /*!< Return normalising frequency */
    void set_F0(long double newF0);          /*!< Set normalising frequency */
    
    long double get_min_Ck();                /*!< Get minimum Ck amplitude kept in Ck spectrum */
    void set_min_Ck(long double c);                       /*!< Set minimum Ck amplitude to be kept in Ck spectrum */
    
    complex get_Vn(int f, int h);
    complex get_Vn(long double freq, int h);
    complex get_Vn(double freq, int h);
    complex get_Vn(long double freq);
    complex get_Vn(double freq);
    
    void set_Vn(int f, int h, complex V);
    void set_Vn(long double freq, int h, complex V);
    void set_Vn(double freq, int h, complex V);
    void set_Vn(long double freq, complex V);
    void set_Vn(double freq, complex V);

    complex get_Zt(int f, int h);
    complex get_Zt(long double freq, int h);
    complex get_Zt(double freq, int h);
    complex get_Zt(long double freq);
    complex get_Zt(double freq);
    
    void set_Zt(int f, int h, complex Z);
    void set_Zt(long double freq, int h, complex Z);
    void set_Zt(double freq, int h, complex Z);
    void set_Zt(long double freq, complex Z);
    void set_Zt(double freq, complex Z);

    complex get_Vs(int f, int h);
    complex get_Vs(long double freq, int h);
    complex get_Vs(double freq, int h);
    complex get_Vs(long double freq);
    complex get_Vs(double freq);
    
    void set_Vs(int f, int h, complex V);
    void set_Vs(long double freq, int h, complex V);
    void set_Vs(double freq, int h, complex V);
    void set_Vs(long double freq, complex V);
    void set_Vs(double freq, complex V);
     
    // ResponseFn accessors
    bool responseFnReady();
    void kennedy(int n, int noPoints);
    void data(std::string dcFile);
    void data(std::string dcFile, std::string kkFile);
    
    void set_vGap(long double V);
    long double get_vGap();
    void set_iGap(long double I);
    long double get_iGap();
    void set_Rn(long double R);
    long double get_Rn();
    void set_vMax(long double V);
    long double get_vMax();
    
    std::string get_idcFile();
    int get_no_points();
    
    long double get_unpumped_Idc(long double V);
    long double get_unpumped_Ikk(long double V);
};


#endif
