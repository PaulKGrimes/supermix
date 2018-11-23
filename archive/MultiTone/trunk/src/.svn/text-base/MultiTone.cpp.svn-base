// MultiTone.cpp
//
// Original code in "MultiFreq.cpp" and "HarmonicNewton.cpp" by Phichet Kittara
//
// Implementation of the main user callable MultiTone object.
//

/* Change Log:
*/

#include "MultiTone.h"

#include<iostream>


MultiTone::MultiTone()
{
    readyFlag = false;
    dataValidFlag = false;
    
    Max_Iterations = 100;
    epsilon = 1.0e-6;
    freq_tol = 1.0e-9;
    min_dX = 1.0e-18;
    step_dX = 1.0e-3;
    biasV = 0.75; // Default frequency is 0.5, so we bias to middle of photon step
}


void MultiTone::clearHarmonicBalance()
{
    OldX.clear();
    OldY.clear();
    NewX.clear();
    
    jacobian.unit();
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
            complex tempVs, tempZt;
            tempVs = multifreq.get_Vs(f, h);
            tempZt = multifreq.get_Zt(f, h);
            
            if (abs(tempVs) < freq_tol || abs(tempZt) < freq_tol)
            {
                continue;
            }
            else
            {
                validFreq val;
                val.f = f;
                val.h = h;
                validFreqs.push_back(val);
            }
        }
    }
}


void MultiTone::initHarmonicBalance()
{    
    setValidFreqs();
    
    OldX.resize(validFreqs.size()*2);
    OldY.resize(validFreqs.size()*2, 0.0);
    NewX.resize(validFreqs.size()*2);
    deltaX.resize(validFreqs.size()*2, 0.0);
    
    jacobian.resize(validFreqs.size()*2);
    jacobian.unit();
    
    // Fill Vn of each frequency with Vs as starting point
    int n;
    for (int i=0; i<NewX.size(); i+=2)
    {
        n = i/2;
        set_Vn(validFreqs[n].f, validFreqs[n].h, get_Vs(validFreqs[n].f, validFreqs[n].h) );
        OldX[i] = real(get_Vn(validFreqs[n].f, validFreqs[n].h));
        OldX[i+1] = imag(get_Vn(validFreqs[n].f, validFreqs[n].h));
        NewX[i] = OldX[i];
        NewX[i+1] = OldX[i+1];
    }
    multifreq.set_Ck();
    
    if (response.ready()) readyFlag = true;
}


void MultiTone::balance()
{
    if (!ready()) throw MultiTone_not_ready_error();
        
    dataValidFlag = false;
    
    bool totalCheck = true;
    long double dif;
    iterations = 0;
    
    // Fill Vn of each frequency with Vs as starting point
    int n;
    for (int i=0; i<NewX.size(); i+=2)
    {
        n = i/2;
        set_Vn(validFreqs[n].f, validFreqs[n].h, get_Vs(validFreqs[n].f, validFreqs[i/2].h) );
        OldX[i] = real(get_Vn(validFreqs[n].f, validFreqs[n].h));
        OldX[i+1] = imag(get_Vn(validFreqs[n].f, validFreqs[n].h));
        NewX[i] = OldX[i];
        NewX[i+1] = OldX[i+1];
    }
    
    multifreq.set_Ck();
        
    while( ( iterations < Max_Iterations) && totalCheck)
    {
        iterations++;
        OldX = NewX;
    
        functionEvaluation(OldX, OldY);
        
        setJacobian();
        try 
        {
            setNewX();
        }
        catch (matrix_singular_error)
        {
            cerr << "# MultiTone::balance() : Jacobian is singular at " << biasV << " bias, resetting\n"; 
            // One of the gradients is zero.  Offset everything by a small
            // and try again.
            for (int i=0; i<NewX.size(); i++)
            {
                OldX[i] += 0.1*epsilon*OldX[i];
            }
            setJacobian();
            setNewX();
        }
                
        // If any of the X values have changed by more than a certain fraction
        // continue iterating
        totalCheck = false;
        for( int i=0; i<validFreqs.size()*2; i++)
        {
            dif = abs( NewX[i] - OldX[i] );
            if ( dif > epsilon*abs(OldX[i]) )
            {
                totalCheck = true;
                break;
            }
        }
        
    }    
    
    dataValidFlag = true;
}


void MultiTone::setJacobian()
{    
    long double dX;
    vector<long double> tempX;
    vector<long double> tempY;
    long double dY;
    
    tempX.resize(validFreqs.size()*2, 0.0);
    tempY.resize(validFreqs.size()*2, 0.0);
    
    // Real part calculation
    for( int f=0; f<validFreqs.size()*2; f++ )
    {
        // Set start point and step size
        tempX = OldX;        
        
        dX = step_dX*tempX[f];
        if (dX < min_dX) dX = min_dX;
        
        // Step and calculate new Y values
        tempX[f] += dX;
        functionEvaluation(tempX, tempY);
        
        for( int g=0; g<validFreqs.size()*2; g++ )
        {
            dY = tempY[g] - OldY[g];
            jacobian(f, g) = dY/dX; 
        }
    }
}


void MultiTone::setNewX()
{
    // X(k+1) = X(k) + inverse(jacobian) x Y(k)
    jacobian.invert();
    
    deltaX = jacobian*OldY;
    
    // Unfortunately, binary vector arithmetic isn't as simple as binary matrix arithmetic
    for (int f=0; f<validFreqs.size()*2; f++) 
    {
        NewX[f] = OldX[f] - deltaX[f];
    }
}


complex MultiTone::get_Ip(long double freq)
{
    
    if (!multifreq.CkValid()) multifreq.set_Ck();
            
    complex Ires;
    complex Rs_plus = complex(0.0);
    complex Rs_minus = complex(0.0);
    long double Idc, Ikk, trialFreq;
    int ckSize = multifreq.get_no_Ck();
    
    for (int k=0; k<ckSize; k++)
    {
        for (int l=0; l<ckSize; l++)
        {
            trialFreq = multifreq.get_Ck_freq(l) - multifreq.get_Ck_freq(k);
            if ( abs(trialFreq - freq) < freq_tol )
            {
                Idc = response.get_Idc(biasV + multifreq.get_Ck_freq(k) );
                Ikk = response.get_Ikk(biasV + multifreq.get_Ck_freq(k) );
                Ires = complex( Ikk, Idc );
                Rs_plus += multifreq.get_Ck(k)*conj(multifreq.get_Ck(l))*Ires;
            }
            if ( abs(trialFreq + freq) < freq_tol )
            {
                Idc = response.get_Idc(biasV + multifreq.get_Ck_freq(k) );
                Ikk = response.get_Ikk(biasV + multifreq.get_Ck_freq(k) );
                Ires = complex( Ikk, Idc );
                Rs_minus += multifreq.get_Ck(k)*conj(multifreq.get_Ck(l))*Ires;
            }
        }
    }
    
    complex result = complex( imag(Rs_plus) + imag(Rs_minus), -real(Rs_plus)+real(Rs_minus) );
    
    // Correct DC amplitude
    if ( abs(freq) < 1.0e-9 ) result /= 2.0;
    return result;
}


void MultiTone::functionEvaluation(vector<long double> &inX, vector<long double> &outY)
{
    // Set Vn
    int n;
    for (int i=0; i<validFreqs.size()*2; i+=2)
    {
        n = i/2;
        multifreq.set_Vn(validFreqs[n].f, validFreqs[n].h, complex(inX[i], inX[i+1]));
    }
        
    multifreq.set_Ck();
    
    // Calculate delta = Vs - IZt - Vn
    for (int i=0; i<validFreqs.size()*2; i+=2)
    {
        n =i/2;
        int f = validFreqs[n].f;
        int h = validFreqs[n].h;
        complex tempY = multifreq.get_Vs(f, h) - multifreq.get_Zt(f, h)*get_Ip(get_freq(f, h)*(h+1)) - multifreq.get_Vn(f, h);
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


void MultiTone::set_bias(long double v)
{
    biasV = v;
    //set_Vs(0.0, complex(v, 0.0));
}


complex MultiTone::get_I(int f, int h)
{
    if (!ready()) 
        throw MultiTone_not_ready_error();
    if (!dataValid()) 
        balance();
    
    return get_Ip(get_freq(f, h));
}


complex MultiTone::get_I(long double V, int f, int h)
{
    if (abs(biasV - V) > freq_tol)
    {
        biasV = V;
        balance();
    }
    
    return get_I(f, h);
}


complex MultiTone::get_I(long double freq, int h)
{
    return get_Ip(freq*(h+1));
}

complex MultiTone::get_I(double freq, int h)
{
    return get_Ip(freq*(h+1));
}

complex MultiTone::get_I(long double freq)
{
    return get_Ip(freq);
}


complex MultiTone::get_I(long double V, long double freq, int h)
{
    return get_I(V, freq*(h+1));
}

complex MultiTone::get_I(long double V, double freq, int h)
{
    return get_I(V, freq*(h+1));
}

complex MultiTone::get_I(long double V, long double freq)
{
    return get_I(V, freq);
}

complex MultiTone::get_I(long double V, double freq)
{
    return get_I(V, freq);
}


// Multi_Frequency accessors
void MultiTone::add_frequency()
{
    multifreq.add_frequency();
    multifreq.set_Ck();
    dataValidFlag = false;
}

void MultiTone::add_frequency(long double freq)
{
    multifreq.add_frequency(freq);
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

void MultiTone::remove_frequency(long double freq)
{
    multifreq.remove_frequency(get_f(freq));
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

void MultiTone::add_harmonic(long double freq)
{
    multifreq.add_harmonic(multifreq.get_f(freq));
    multifreq.set_Ck();
    dataValidFlag = false;
}

void MultiTone::add_harmonic(double freq)
{
    multifreq.add_harmonic(multifreq.get_f(freq));
    multifreq.set_Ck();
    dataValidFlag = false;
}


void MultiTone::add_harmonic(int f, complex Vs, complex Zt)
{
    multifreq.add_harmonic(f, Vs, Zt);
    multifreq.set_Ck();
    dataValidFlag = false;
}

void MultiTone::add_harmonic(long double freq, complex Vs, complex Zt)
{
    multifreq.add_harmonic(multifreq.get_f(freq), Vs, Zt);
    multifreq.set_Ck();
    dataValidFlag = false;
}

void MultiTone::add_harmonic(double freq, complex Vs, complex Zt)
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

void MultiTone::remove_harmonic(long double freq)
{
    multifreq.remove_harmonic(multifreq.get_f(freq));
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

int MultiTone::get_no_harm(long double freq)
{
    return multifreq.get_no_harm(multifreq.get_f(freq));
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
long double MultiTone::get_freq(int f)
{
    return multifreq.get_freq(f);
}

long double MultiTone::get_freq(int f, int h)
{
    return multifreq.get_freq(f, h);
}

int MultiTone::get_f(long double freq)
{
    return multifreq.get_f(freq);
}

int MultiTone::get_h(long double freq)
{
    return multifreq.get_h(freq);
}

// Set Frequencies
void MultiTone::set_freq(int f, long double newFreq)
{
    multifreq.set_freq(f, newFreq);
    dataValidFlag = false;
}

void MultiTone::set_freq(long double oldFreq, long double newFreq)
{
    multifreq.set_freq(get_f(oldFreq), newFreq);
    dataValidFlag = false;
}

void MultiTone::set_freq(double oldFreq, long double newFreq)
{
    multifreq.set_freq(get_f(oldFreq), newFreq);
    dataValidFlag = false;
}


// Normalising frequencies
void MultiTone::set_F0(long double newF0)
{
    multifreq.set_F0(newF0);
    dataValidFlag = false;
}

long double MultiTone::get_F0()
{
    return multifreq.get_F0();
}


// Spectrum calculation control
long double MultiTone::get_min_Ck()
{
    return multifreq.get_min_Ck();
}

void MultiTone::set_min_Ck(long double c)
{
    multifreq.set_min_Ck(c);
    // This doesn't invalidate previous data
}


// Embedding parameter accessors
complex MultiTone::get_Vn(int f, int h)
{
    return multifreq.get_Vn(f, h);
}

complex MultiTone::get_Vn(double freq, int h)
{
    return multifreq.get_Vn(get_f(freq), h);
}

complex MultiTone::get_Vn(long double freq, int h)
{
    return multifreq.get_Vn(get_f(freq), h);
}

complex MultiTone::get_Vn(double freq)
{
    return multifreq.get_Vn(get_f(freq), get_h(freq));
}

complex MultiTone::get_Vn(long double freq)
{
    return multifreq.get_Vn(get_f(freq), get_h(freq));
}


// Embedding parameter setting
void MultiTone::set_Vn(int f, int h, complex V)
{
    multifreq.set_Vn(f, h, V);
    dataValidFlag = false;
}

void MultiTone::set_Vn(long double freq, int h, complex V)
{
    multifreq.set_Vn(get_f(freq), h, V);
    dataValidFlag = false;
}

void MultiTone::set_Vn(double freq, int h, complex V)
{
    multifreq.set_Vn(get_f(freq), h, V);
    dataValidFlag = false;
}

void MultiTone::set_Vn(long double freq, complex V)
{
    multifreq.set_Vn(get_f(freq), get_h(freq), V);
    dataValidFlag = false;
}

void MultiTone::set_Vn(double freq, complex V)
{
    multifreq.set_Vn(get_f(freq), get_h(freq), V);
    dataValidFlag = false;
}


// Embedding parameter accessors
complex MultiTone::get_Zt(int f, int h)
{
    return multifreq.get_Zt(f, h);
}

complex MultiTone::get_Zt(double freq, int h)
{
    return multifreq.get_Zt(get_f(freq), h);
}

complex MultiTone::get_Zt(long double freq, int h)
{
    return multifreq.get_Zt(get_f(freq), h);
}

complex MultiTone::get_Zt(double freq)
{
    return multifreq.get_Zt(get_f(freq), get_h(freq));
}

complex MultiTone::get_Zt(long double freq)
{
    return multifreq.get_Zt(get_f(freq), get_h(freq));
}


// Embedding parameter setting
void MultiTone::set_Zt(int f, int h, complex Z)
{
    multifreq.set_Zt(f, h, Z);
    dataValidFlag = false;
}

void MultiTone::set_Zt(long double freq, int h, complex Z)
{
    multifreq.set_Zt(get_f(freq), h, Z);
    dataValidFlag = false;
}

void MultiTone::set_Zt(double freq, int h, complex Z)
{
    multifreq.set_Zt(get_f(freq), h, Z);
    dataValidFlag = false;
}

void MultiTone::set_Zt(long double freq, complex Z)
{
    multifreq.set_Zt(get_f(freq), get_h(freq), Z);
    dataValidFlag = false;
}

void MultiTone::set_Zt(double freq, complex Z)
{
    multifreq.set_Zt(get_f(freq), get_h(freq), Z);
    dataValidFlag = false;
}


// Embedding parameter accessors
complex MultiTone::get_Vs(int f, int h)
{
    return multifreq.get_Vs(f, h);
}

complex MultiTone::get_Vs(double freq, int h)
{
    return multifreq.get_Vs(get_f(freq), h);
}

complex MultiTone::get_Vs(long double freq, int h)
{
    return multifreq.get_Vs(get_f(freq), h);
}

complex MultiTone::get_Vs(double freq)
{
    return multifreq.get_Vs(get_f(freq), get_h(freq));
}

complex MultiTone::get_Vs(long double freq)
{
    return multifreq.get_Vs(get_f(freq), get_h(freq));
}


// Embedding parameter setting
void MultiTone::set_Vs(int f, int h, complex V)
{
    multifreq.set_Vs(f, h, V);
    dataValidFlag = false;
}

void MultiTone::set_Vs(long double freq, int h, complex V)
{
    multifreq.set_Vs(get_f(freq), h, V);
    dataValidFlag = false;
}

void MultiTone::set_Vs(double freq, int h, complex V)
{
    multifreq.set_Vs(get_f(freq), h, V);
    dataValidFlag = false;
}

void MultiTone::set_Vs(long double freq, complex V)
{    
    multifreq.set_Vs(get_f(freq), get_h(freq), V);
    dataValidFlag = false;
}

void MultiTone::set_Vs(double freq, complex V)
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


void MultiTone::set_vGap(long double V)
{
    response.set_vGap(V);
    dataValidFlag = false;
}

long double MultiTone::get_vGap()
{
    return response.get_vGap();
}


void MultiTone::set_iGap(long double I)
{
    response.set_iGap(I);
    dataValidFlag = false;
}

long double MultiTone::get_iGap()
{
    return response.get_iGap();
}


void MultiTone::set_Rn(long double R)
{
    response.set_Rn(R);
    dataValidFlag = false;
}

long double MultiTone::get_Rn()
{
    return response.get_Rn();
}


void MultiTone::set_vMax(long double V)
{
    response.set_vMax(V);
    // Data is still valid
}

long double MultiTone::get_vMax()
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


long double MultiTone::get_unpumped_Idc(long double V)
{
    return response.get_Idc(V);
}

long double MultiTone::get_unpumped_Ikk(long double V)
{
    return response.get_Ikk(V);
}
