// MultiTone.cpp
//
// Original code in "MultiFreq.cpp" and "HarmonicNewton.cpp" by Phichet Kittara
//
// Implementation of the main user callable MultiTone object.
//

/* Change Log:
*   05/07/04 : PKG : Removed all checks on properties for validFreqs as 
*               they are not required.
*/

#include "MultiTone.h"

#include<iostream>


MultiTone::MultiTone()
{
    readyFlag = false;
    dataValidFlag = false;
    
    xlast.reindex(Index_C);
    fval.reindex(Index_C);
    Jacobian.reindex(Index_C, Index_C);
    
    maxstep = 10;
    
    iterations = 0;
    max_iter = 100;
    f_tol = 1.0e-8;
    F_tol = 1.0e-10;
    freq_tol = 1.0e-9;
    step_dX = 1.0e-3;
    min_dX = 1.0e-6;
    dx_tol = 1.0e-9;
    rate_factor = 1.0e-4;
    biasV = 0.75; // Default frequency is 0.5, so we bias to middle of photon step
}


void MultiTone::setValidFreqs()
{
    validFreqs.clear();
    
    for (int f=0; f<multifreq.get_no_freq(); f++)
    {
        // Exclude DC
        if (multifreq.get_freq(f) < freq_tol) continue;
            
        for (int h=0; h<multifreq.get_no_harm(f); h++)
        {
            validFreq val;
            val.f = f;
            val.h = h;
            validFreqs.push_back(val);
        }
    }
}


void MultiTone::initHarmonicBalance()
{    
    setValidFreqs();
    
    xlast.resize(validFreqs.size()*2-1);
    fval.resize(validFreqs.size()*2-1);
    
    Jacobian.resize(validFreqs.size()*2-1);
    Jacobian.diagonal(1.0);
    
    // Fill Vn of each frequency with Vs as starting point
    int n;
    for (int i=0; i<xlast.size; i+=2)
    {
        n = i/2;
        set_Vn(validFreqs[n].f, validFreqs[n].h, get_Vs(validFreqs[n].f, validFreqs[n].h) );
        xlast[i] = real(get_Vn(validFreqs[n].f, validFreqs[n].h));
        xlast[i+1] = imag(get_Vn(validFreqs[n].f, validFreqs[n].h));
    }
    multifreq.set_Ck();
    
    if (response.ready()) readyFlag = true;
}


void MultiTone::balance()
{
    if (!ready()) throw MultiTone_not_ready_error();
        
    dataValidFlag = false;
    
    iterations = 0;
    
    //std::cout << "# Calling newton::solve()...\n";
    
    solve();
    
    //std::cout << "# newton::solve() completed\n";
    
    if (!no_solution()) dataValidFlag = true;
}


void MultiTone::calc()
{
    //std::cout << "# In MultiTone::calc():\n#\tCalling MultiTone::functionEvaluation()...\n";
    functionEvaluation(xlast, fval);

    //std::cout << xlast;
    //std::cout << " " << fval << std::endl;
    
    //std::cout << "#\tCalling setJacobian()...\n";
    
    setJacobian();
    
    //std::cout << "# Jacobian : \n" << Jacobian << std::endl;
    
    //std::cout << "#\t Incrementing iterations...\n#calc() call complete\n";
    iterations++;
}


void MultiTone::setJacobian()
{    
    double dX;
    double dY;
        
    real_vector tempX(Index_C);
    real_vector tempY(Index_C);

    tempX.resize(validFreqs.size()*2-1);
    tempY.resize(validFreqs.size()*2-1);

    
    // Real part calculation
    for( int f=0; f<validFreqs.size()*2; f++ )
    {
        // Set start point and step size
        tempX = xlast;        
        
        dX = step_dX*tempX[f];
        if (dX < min_dX) 
        {
            dX = min_dX;
            //std::cout << "# setting min_dX\n";
        }
        
        // Step and calculate new Y values
        tempX[f] += dX;
        functionEvaluation(tempX, tempY);
        
        for( int g=0; g<validFreqs.size()*2; g++ )
        {
            dY = tempY[g] - fval[g];
            Jacobian[g][f] = dY/dX; 
        }
    }
}


complex MultiTone::get_Ip(double freq)
{
 
    //std::cout << "#\t\tIn get_Ip() : checking vaildity of Ck values\n";
    if (!multifreq.CkValid()) multifreq.set_Ck();
            
    Complex Ires;
    Complex Rs_plus = Complex(0.0);
    Complex Rs_minus = Complex(0.0);
    double Idc, Ikk, trialFreq;
    int ckSize = multifreq.get_no_Ck();
    
    //std::cout << "#\t\t\tLooping over Ck values...\n";
    for (int k=0; k<ckSize; k++)
    {
        for (int l=0; l<ckSize; l++)
        {
            trialFreq = multifreq.get_Ck_freq(l) - multifreq.get_Ck_freq(k);
            if ( abs(trialFreq - freq) < freq_tol )
            {
                Idc = response.get_Idc(biasV + multifreq.get_Ck_freq(k) );
                Ikk = response.get_Ikk(biasV + multifreq.get_Ck_freq(k) );
                Ires = Complex( Ikk, Idc );
                Rs_plus += multifreq.get_Ck(k)*conj(multifreq.get_Ck(l))*Ires;
            }
            if ( abs(trialFreq + freq) < freq_tol )
            {
                Idc = response.get_Idc(biasV + multifreq.get_Ck_freq(k) );
                Ikk = response.get_Ikk(biasV + multifreq.get_Ck_freq(k) );
                Ires = Complex( Ikk, Idc );
                Rs_minus += multifreq.get_Ck(k)*conj(multifreq.get_Ck(l))*Ires;
            }
        }
    }

    //std::cout << "#\t\t\tLoop complete, calculating result...\n";
    Complex result = Complex( imag(Rs_plus) + imag(Rs_minus), -real(Rs_plus)+real(Rs_minus) );
    
    //std::cout << "#\t\t\tResult calculated, correcting DC amplitude...\n";
    // Correct DC amplitude
    if ( abs(freq) < 1.0e-9 ) result /= 2.0;

    return result;
}


void MultiTone::functionEvaluation(real_vector &inX, real_vector &outY)
{
    // inX and outY are assumed to by Index_C SuperMix real_vectors
    
    // Set Vn
    int n;
    
    //std::cout << "#\tIn functionEvaluation() : inX.size: " << inX.size << " outY.size:" << outY.size << "\n#\t\t\tSetting Vn...\n"; 
    for (int i=0; i<validFreqs.size()*2; i+=2)
    {
        n = i/2;
        multifreq.set_Vn(validFreqs[n].f, validFreqs[n].h, Complex(inX[i], inX[i+1]));
    }
    
    //std::cout << "#\t\tSetting Ck values...\n";
    multifreq.set_Ck();
    
    //std::cout << "#\t\tVn are set, calculating delta...\n";
    
    // Calculate delta = Vs - IZt - Vn
    for (int i=0; i<validFreqs.size()*2; i+=2)
    {
        n = i/2;
        int f = validFreqs[n].f;
        int h = validFreqs[n].h;
        Complex tempY = multifreq.get_Vs(f, h) - multifreq.get_Zt(f, h)*get_Ip(get_freq(f, h)) - multifreq.get_Vn(f, h);
        outY[i] = real(tempY);
        outY[i+1] = imag(tempY);
    }
}


bool MultiTone::ready()
{
    return readyFlag;
}


bool MultiTone::dataValid()
{
    return dataValidFlag;
}


void MultiTone::set_bias(double v)
{
    biasV = v;
}


complex MultiTone::get_I(int f, int h)
{
    if (!ready()) 
        throw MultiTone_not_ready_error();
    if (!dataValid()) 
        balance();
    
    return get_Ip(get_freq(f, h));
}


complex MultiTone::get_I(double V, int f, int h)
{
    if (abs(biasV - V) > freq_tol)
    {
        biasV = V;
        balance();
    }
    
    return get_I(f, h);
}


complex MultiTone::get_I(double freq, int h)
{
    return get_I(freq*(h+1));
}


complex MultiTone::get_I(double freq)
{
    if (!ready()) 
        throw MultiTone_not_ready_error();
    if (!dataValid()) 
        balance();
    
    return get_Ip(freq);
}


complex MultiTone::get_I(double V, double freq, int h)
{
    return get_I(V, freq*(h+1));
}


complex MultiTone::get_I(double V, double freq)
{
    if (abs(biasV - V) > freq_tol)
    {
        biasV = V;
        balance();
    }
    
    return get_I(freq);
}


// Multi_Frequency accessors
void MultiTone::add_frequency()
{
    multifreq.add_frequency();
    multifreq.set_Ck();
    dataValidFlag = false;
}


void MultiTone::add_frequency(double freq)
{
    multifreq.add_frequency(freq);
    multifreq.set_Ck();
    dataValidFlag = false;
}


void MultiTone::remove_frequency(int f)
{
    multifreq.remove_frequency(f);
    multifreq.set_Ck();
    dataValidFlag = false;
}


void MultiTone::remove_frequency(double freq)
{
    multifreq.remove_frequency(get_f(freq));
    multifreq.set_Ck();
    dataValidFlag = false;
}


// Harmonic control
void MultiTone::add_harmonic(int f)
{
    multifreq.add_harmonic(f);
    multifreq.set_Ck();
    dataValidFlag = false;
}


void MultiTone::add_harmonic(double freq)
{
    multifreq.add_harmonic(multifreq.get_f(freq));
    multifreq.set_Ck();
    dataValidFlag = false;
}


void MultiTone::add_harmonic(int f, Complex Vs, Complex Zt)
{
    multifreq.add_harmonic(f, Vs, Zt);
    multifreq.set_Ck();
    dataValidFlag = false;
}


void MultiTone::add_harmonic(double freq, Complex Vs, Complex Zt)
{
    multifreq.add_harmonic(multifreq.get_f(freq), Vs, Zt);
    multifreq.set_Ck();
    dataValidFlag = false;
}


void MultiTone::remove_harmonic(int f)
{
    multifreq.remove_harmonic(f);
    multifreq.set_Ck();
    dataValidFlag = false;
}


void MultiTone::remove_harmonic(double freq)
{
    multifreq.remove_harmonic(multifreq.get_f(freq));
    multifreq.set_Ck();
    dataValidFlag = false;
}


// Functions for finding limits of loops
int MultiTone::get_no_freq()
{
    return multifreq.get_no_freq();
}


int MultiTone::get_no_harm(int f)
{
    return multifreq.get_no_harm(f);
}


int MultiTone::get_no_harm(double freq)
{
    return multifreq.get_no_harm(multifreq.get_f(freq));
}


int MultiTone::get_no_Ck()
{
    return multifreq.get_no_Ck();
}


// Get data on frequencies etc.
double MultiTone::get_freq(int f)
{
    return multifreq.get_freq(f);
}

double MultiTone::get_freq(int f, int h)
{
    return multifreq.get_freq(f, h);
}

int MultiTone::get_f(double freq)
{
    return multifreq.get_f(freq);
}

int MultiTone::get_h(double freq)
{
    return multifreq.get_h(freq);
}

// Set Frequencies
void MultiTone::set_freq(int f, double newFreq)
{
    multifreq.set_freq(f, newFreq);
    dataValidFlag = false;
}


void MultiTone::set_freq(double oldFreq, double newFreq)
{
    multifreq.set_freq(get_f(oldFreq), newFreq);
    dataValidFlag = false;
}


// Normalising frequencies
void MultiTone::set_F0(double newF0)
{
    multifreq.set_F0(newF0);
    dataValidFlag = false;
}

double MultiTone::get_F0()
{
    return multifreq.get_F0();
}


// Spectrum calculation control
double MultiTone::get_min_Ck()
{
    return multifreq.get_min_Ck();
}

void MultiTone::set_min_Ck(double c)
{
    multifreq.set_min_Ck(c);
    // This doesn't invalidate previous data
}


// Embedding parameter accessors
Complex MultiTone::get_Vn(int f, int h)
{
    return multifreq.get_Vn(f, h);
}


Complex MultiTone::get_Vn(double freq, int h)
{
    return multifreq.get_Vn(get_f(freq), h);
}


Complex MultiTone::get_Vn(double freq)
{
    return multifreq.get_Vn(get_f(freq), get_h(freq));
}


// Embedding parameter setting
void MultiTone::set_Vn(int f, int h, Complex V)
{
    multifreq.set_Vn(f, h, V);
    dataValidFlag = false;
}


void MultiTone::set_Vn(double freq, int h, Complex V)
{
    multifreq.set_Vn(get_f(freq), h, V);
    dataValidFlag = false;
}


void MultiTone::set_Vn(double freq, Complex V)
{
    multifreq.set_Vn(get_f(freq), get_h(freq), V);
    dataValidFlag = false;
}


// Embedding parameter accessors
Complex MultiTone::get_Zt(int f, int h)
{
    return multifreq.get_Zt(f, h);
}


Complex MultiTone::get_Zt(double freq, int h)
{
    return multifreq.get_Zt(get_f(freq), h);
}


Complex MultiTone::get_Zt(double freq)
{
    return multifreq.get_Zt(get_f(freq), get_h(freq));
}


// Embedding parameter setting
void MultiTone::set_Zt(int f, int h, Complex Z)
{
    multifreq.set_Zt(f, h, Z);
    dataValidFlag = false;
}


void MultiTone::set_Zt(double freq, int h, Complex Z)
{
    multifreq.set_Zt(get_f(freq), h, Z);
    dataValidFlag = false;
}


void MultiTone::set_Zt(double freq, Complex Z)
{
    multifreq.set_Zt(get_f(freq), get_h(freq), Z);
    dataValidFlag = false;
}


// Embedding parameter accessors
Complex MultiTone::get_Vs(int f, int h)
{
    return multifreq.get_Vs(f, h);
}


Complex MultiTone::get_Vs(double freq, int h)
{
    return multifreq.get_Vs(get_f(freq), h);
}


Complex MultiTone::get_Vs(double freq)
{
    return multifreq.get_Vs(get_f(freq), get_h(freq));
}


// Embedding parameter setting
void MultiTone::set_Vs(int f, int h, Complex V)
{
    multifreq.set_Vs(f, h, V);
    dataValidFlag = false;
}


void MultiTone::set_Vs(double freq, int h, Complex V)
{
    multifreq.set_Vs(get_f(freq), h, V);
    dataValidFlag = false;
}


void MultiTone::set_Vs(double freq, Complex V)
{    
    multifreq.set_Vs(get_f(freq), get_h(freq), V);
    dataValidFlag = false;
}


// Response Function accessors
bool MultiTone::responseFnReady()
{
    return response.ready();
}

void MultiTone::kennedy(int n, int noPoints)
{
    response.kennedy(n, noPoints);
    dataValidFlag = false;
}

void MultiTone::data(std::string dcFile)
{
    response.data(dcFile);
    dataValidFlag = false;
}


void MultiTone::data(std::string dcFile, std::string kkFile)
{
    response.data(dcFile, kkFile);
    dataValidFlag = false;
}


void MultiTone::set_vGap(double V)
{
    response.set_vGap(V);
    dataValidFlag = false;
}

double MultiTone::get_vGap()
{
    return response.get_vGap();
}


void MultiTone::set_iGap(double I)
{
    response.set_iGap(I);
    dataValidFlag = false;
}

double MultiTone::get_iGap()
{
    return response.get_iGap();
}


void MultiTone::set_Rn(double R)
{
    response.set_Rn(R);
    dataValidFlag = false;
}

double MultiTone::get_Rn()
{
    return response.get_Rn();
}


void MultiTone::set_vMax(double V)
{
    response.set_vMax(V);
    // Data is still valid
}

double MultiTone::get_vMax()
{
    return response.get_vMax();
}


std::string MultiTone::get_idcFile()
{
    return response.get_idcFile();
}

int MultiTone::get_no_points()
{
    return response.get_no_points();
}


double MultiTone::get_unpumped_Idc(double V)
{
    return response.get_Idc(V);
}

double MultiTone::get_unpumped_Ikk(double V)
{
    return response.get_Ikk(V);
}
