// Multi_Frequency.cpp
//
// Original code in "MultiFreq.cpp" by Phichet Kittara
//
// Implementation of object representing a collection of frequencies.
//
/* Change Log:
*       03/05/04 : PKG : Compared with final PK version, no changes.
*           Replaced DOUBLEPRECISION with FREQ_TOL and accessors
*           Replaced #define'd complex with explicit std::complex<long double>
*/

#include "Multi_Frequency.h"
#include <iostream>

Multi_Frequency::Multi_Frequency()
{
    FREQ_TOL = 1.0e-9;
    
    add_frequency();    

    F0 = 1.0;
    min_Ck = 1.0e-18;
    
    set_Ck();
}


void Multi_Frequency::set_Ck()
{
    CkSpectrum.clear();
    
    std::vector<Ck> tempCkSpectrum;
    Ck tempCk;
    
    // 1st Frequency
    int besselOrder = Frequencies[0].get_no_Ck();
    
    for (int j=-besselOrder; j<=besselOrder; j++)
    {
        tempCk.Freq = j*Frequencies[0].get_F();
        tempCk.Ckf = Frequencies[0].get_Cjk(j);
        CkSpectrum.push_back(tempCk);
    }
    
    // Subsequent frequencies
    int CkSize = CkSpectrum.size();
    bool Added;
    for (int f=1; f<get_no_freq(); f++)
    {
        besselOrder = Frequencies[f].get_no_Ck();
        tempCkSpectrum.clear();
        
        for (int j=-besselOrder; j<=besselOrder; j++)
        {
            for (int k=0; k<CkSize; k++)
            {
                tempCk.Ckf = CkSpectrum[k].Ckf*(Frequencies[f].get_Cjk(j));
                
                // test whether this value is big enough to include
                if ( abs(tempCk.Ckf) > min_Ck )
                {
                    Added = false;
                    tempCk.Freq = CkSpectrum[k].Freq + (j*Frequencies[f].get_F());
                    
                    // test whether this frequency has already been inserted
                    for (int i=0; i<tempCkSpectrum.size(); i++)
                    {
                        // This tolerance must be larger than double precision
                        // something around 1e-16 works for 2 freqs and 2 harmonics
                        // so we set it much higher, but still smaller than any
                        // likely frequency difference
                        if ( fabs(tempCkSpectrum[i].Freq - tempCk.Freq) < FREQ_TOL )
                        {  
                            // If does exist, add new value
                            tempCkSpectrum[i].Ckf += tempCk.Ckf;
                            Added = true;
                            break;
                        }
                    }
                    
                    if ( !Added ) // If it doesn't exist, add a new frequency
                    {
                        tempCkSpectrum.push_back(tempCk);
                    }
                } // Amplitude test
            } // Loop over CkSpectrum size
        } // Frequency spectrum
        CkSpectrum = tempCkSpectrum;
        CkSize = CkSpectrum.size();
    } // Frequencies 

    CkValidFlag = true;
}
    

std::complex<long double>  Multi_Frequency::get_Ck(long double freq)
{
    int besselOrder = get_no_Ck();
    std::complex<long double>  result = std::complex<long double> (0.0,0.0);
    
    for (int k=0; k < besselOrder; k++)
    {
        if ((CkSpectrum[k].Freq - freq) <= FREQ_TOL)
        {
            result = CkSpectrum[k].Ckf;
            break;
        }
    }
    
    return result;
}


std::complex<long double>  Multi_Frequency::get_Ck(int f, int h)
{
    long double freq = get_freq(f, h);
    return get_Ck(freq);
}


long double Multi_Frequency::get_min_Ck()
{
    return min_Ck;
}


void Multi_Frequency::set_min_Ck(long double min)
{
    min_Ck = min;
    
    set_Ck();
}
    


void Multi_Frequency::add_frequency()
{
    One_Frequency newFreq;
    
    Frequencies.push_back(newFreq);
    
    set_Ck();
}


void Multi_Frequency::add_frequency(long double freq)
{
    One_Frequency newFreq;
    newFreq.set_F(freq);
        
    Frequencies.push_back(newFreq);
    
    set_Ck();
}


void Multi_Frequency::remove_frequency(int f)
{
    Frequencies.erase(Frequencies.begin()+f);
    
    set_Ck();
}


void Multi_Frequency::add_harmonic(int f)
{
    Frequencies.at(f).add_harmonic();
    
    set_Ck();
}


void Multi_Frequency::add_harmonic(int f, std::complex<long double>  Vs, std::complex<long double>  Zt)
{
    Frequencies.at(f).add_harmonic(Vs, Zt);
    
    set_Ck();
}


void Multi_Frequency::remove_harmonic(int f)
{
    Frequencies.at(f).remove_harmonic();
    
    set_Ck();
}


int Multi_Frequency::get_no_harm(int f)
{
    return Frequencies.at(f).get_no_harm();
}


long double Multi_Frequency::get_freq(int f)
{
    return Frequencies.at(f).get_F();
}


long double Multi_Frequency::get_freq(int f, int h)
{
    return Frequencies.at(f).get_F()*(h+1);
}


int Multi_Frequency::get_f(long double freq)
{
    // Check fundamental frequencies
    int f = -1;
    for (int i=0; i < get_no_freq(); i++)
    {
        if (fabs(Frequencies[i].get_F() - freq) < FREQ_TOL)
        {
            f = i;
            break;
        }
    }
    
    // Check harmonics
    long double test;
    long double fund;
    if (f < 0)
    {            
        for (int i=0; i< get_no_freq(); i++)
        {
            fund = Frequencies[i].get_F();
            for (int h=1; h<=get_no_harm(i); h++)
            {
                test = fund*(h+1);                
                if (fabs(test - freq) < (FREQ_TOL*(h+1)))
                {
                    f = i;
                    break;
                }
            }
            if (f>0) break;
        }
    }
    
    return f;
}


int Multi_Frequency::get_h(long double freq)
{
    int h=0;
    
    long double test;
    long double fund;
    for (int i=0; i< get_no_freq(); i++)
    {
        fund = Frequencies[i].get_F();
        for (int p=0; p<get_no_harm(i); p++)
        {
            test = fund*(p+1);                
            if (fabs(test - freq) < (FREQ_TOL*(p+1)))
            {
                h = p;
                break;
            }
        }
        if (h>0) break;
    }
    
    return h;
}



void Multi_Frequency::set_freq(int f, long double newFreq)
{
    Frequencies.at(f).set_F(newFreq);
    
    set_Ck();
}


void Multi_Frequency::set_F0(long double newF0)
{
    for (int f=0; f<get_no_freq(); f++)
    {
        Frequencies[f].set_F0(newF0);
    }
    F0 = newF0;
    
    set_Ck();
}


void Multi_Frequency::set_Vn(int f, int h, std::complex<long double>  V)
{
    Frequencies.at(f).set_Vn(h, V);
    CkValidFlag = false;
}


void Multi_Frequency::set_Vs(int f, int h, std::complex<long double>  V)
{
    Frequencies.at(f).set_Vs(h, V);
}


void Multi_Frequency::set_Zt(int f, int h, std::complex<long double>  Z)
{
    Frequencies.at(f).set_Zt(h, Z);
}
