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
//
#include <stdexcept>
#include <iostream>
#include "One_Frequency.h"


One_Frequency::One_Frequency( )
{
    // Initialise simple values
    delta_Ck = 1.0e-13;
    delta_Ck_0 = 1.0e-13;
    
    F0 = 1.0;
    F = 0.5;
    
    Cjk.reindex(Index_C);
    
    //std::cout << "Creating One_Harmonic...\n";
    One_Harmonic harm(10, 1, Complex(0.0,0.0), F, F0);

    //std::cout << "Adding One_Harmonic to harmonics...\n";    
    harmonics.push_back(harm);
    
    //std::cout << "Calling set_An()...\n";
    set_An();
    
    //std::cout << "Calling set_Cjk()...\n";
    set_Cjk();    
}
    

void One_Frequency::add_harmonic()
{
    Cjk.reindex(Index_C);
    
    
    // Create new harmonic with appropriate values
    One_Harmonic new_harm(harmonics[0].get_no_Ck(), get_no_harm(), Complex(1.0,0.0), F, F0);
    new_harm.set_Zt(Complex(0.0,0.0));
    new_harm.set_Vs(Complex(0.0,0.0));
    
    harmonics.push_back(new_harm);
    
    set_An();
    set_Cjk();    
}


void One_Frequency::add_harmonic(Complex Vs, Complex Zt)
{
    One_Harmonic new_harm(harmonics[0].get_no_Ck(), harmonics.size()+1, Complex(0.0,0.0), F, F0);
    new_harm.set_Zt(Zt);
    new_harm.set_Vs(Vs);
    
    harmonics.push_back(new_harm);
    
    set_An();
    set_Cjk();
}


void One_Frequency::remove_harmonic()
{
    // We must have at least one harmonic, and we must avoid underflow
    if (harmonics.size() > 1)
    {
        harmonics.pop_back();
    }
    else
    {
        throw std::out_of_range("MultiTone :: Attempted Removal of fundamental harmonic");
    }
    
    set_An();
    set_Cjk();
}


void One_Frequency::resetToZero()
{
    for ( int i=0; i < harmonics.size(); i++)
    {
        harmonics[i].set_Vn(Complex(0.0,0.0));
        harmonics[i].set_Zt(Complex(0.0,0.0));
        harmonics[i].set_Vs(Complex(0.0,0.0));    
    }
}


// Force recalculation of all harmonics
void One_Frequency::set_An()
{
    for (int i=0; i < harmonics.size(); i++)
    {
        harmonics[i].set_An();
    }
}


void One_Frequency::set_Cjk()
{
    int besselOrder = get_no_Ck();

    Cjk.resize(besselOrder*2+1);
    
    // Find Cjk
    for (int k=-besselOrder; k<=besselOrder; k++)
    {
        //std::cout << k << "\t" << besselOrder << "\t" << get_no_harm() << std::endl;
        Cjk[k+besselOrder] = get_Cjk(get_no_harm()-1, k);
    }
    
    // Check accuracy
    Complex sum1 = Complex(0.0,0.0);
    Complex sum2 = Complex(0.0,0.0);
    Complex temp;
    for( int k = -besselOrder; k <= besselOrder; k++)
    {
        temp = (Cjk[ k+besselOrder ] * conj(Cjk[ k+besselOrder ]) );
        temp+= (conj(Cjk[ k+besselOrder ]) * Cjk[ k+besselOrder ] );
        sum1+= temp;
        if (abs(k) < besselOrder-5) sum2 += temp;
    }
    sum1 *= 0.5;
    sum2 *= 0.5;
    
    delta_Ck = abs(sum1-(double)1.0);
    double delta_Ck_2 = abs(sum2-(double)1.0);
        
    // Add or remove Bessel coefficients, recursing until required accuracy is
    // acheived.  Adds faster than it removes, to prevent oscillation.
    if ( delta_Ck > delta_Ck_0 && besselOrder < 50 )
    {
        for ( int i=0; i < harmonics.size(); i++ )
        {
            harmonics[i].set_no_Ck(besselOrder + 5);
        }
        set_Cjk();
    }
    else if ( delta_Ck_2 < delta_Ck_0 && besselOrder > 5 )
    {
        for ( int i=0; i < harmonics.size(); i++ )
        {
            harmonics[i].set_no_Ck(besselOrder - 3);
        }
        set_Cjk();
    }
}


Complex One_Frequency::get_Cjk(int j, int k)
{
    Complex result = Complex(0.0,0.0);
    int besselOrder = get_no_Ck();
    
    if ( j == 0 )
    {
        result = harmonics[j].get_An(k);
    }
    else // Calculate Cjk by recursion
    { 
        for ( int m = -besselOrder; m <= besselOrder; m++ )
        {
            result += get_Cjk( j-1, k-(j+1)*m )*harmonics[j].get_An(m);
        }
    }
    
    return result;
}


Complex One_Frequency::get_Cjk(int k)
{
    Complex result = Complex(0.0,0.0);
    int besselOrder = harmonics[0].get_no_Ck();
    
    if ( abs(k) <= besselOrder )
        result = Cjk[k+besselOrder];
    
    return result;
}


/*************/
// Accessors //
/*************/

void One_Frequency::set_F(double f)
{
    F = f;
    
    // f for each harmonic, and recalculate.
    for ( int i=0; i < harmonics.size(); i++)
    {
        harmonics[i].set_Fp(F);
    }
    set_An();
    set_Cjk();
}


void One_Frequency::set_F0(double f)
{
    F0 = f;
    
    // f0 for each harmonic, and recalculate.
    for ( int i=0; i < harmonics.size(); i++)
    {
        harmonics[i].set_F0(F0);
    }
    set_An();
    set_Cjk();
}


void One_Frequency::set_Vn(int i, Complex V)
{   
    // Use .at(i) to do bounds checking for us
    harmonics.at(i).set_Vn(V);
    
    set_An();
    set_Cjk();
}


void One_Frequency::set_Zt(int i, Complex Z)
{
    // Use .at(i) to do bounds checking for us
    harmonics.at(i).set_Zt(Z);
}


void One_Frequency::set_Vs(int i, Complex V)
{
    // Use .at(i) to do bounds checking for us
    harmonics.at(i).set_Vs(V);
}


void One_Frequency::set_delta_Ck0(double d)
{
    delta_Ck_0 = d;
    
    set_Cjk();
}    

