// One_Harmonic.cpp
//
// Original code in "Complex_One_Harmonic.cpp" by Phichet Kittara
//
// Implementation of an object representing a single harmonic.
//
/* Changes
*   03/05/04 : PKG : Altered file to operate in the same way as Piew's final code
*                   Changed name of get_totalJ() and set_totalJ(int) to <g|s>et_no_Ck(<|int)
*/

#include "One_Harmonic.h"
#include <iostream>


// Simple Constructor
// Object is uninitialised after running this
One_Harmonic::One_Harmonic( )
{
    // Initalise members to sensible values
    totalJ = 15;
    p = 1;
    Vn = std::complex<long double>(1.0,0.0);
    Fp = 0.5;
    F0 = 1.0;
    
    // Setup the An vector
    An.resize(totalJ*2+1,0);
    set_An();
}


// Longer Constructor
One_Harmonic::One_Harmonic(int TotalSize,int P,std::complex<long double> V,long double fp,long double f0)
{
    totalJ = TotalSize;
    p = P;
    Vn = V;
    Fp = fp;
    F0 = f0;

    An.resize(totalJ*2+1,0);
    set_An();
}


// Calculates entire Anp spectrum
void One_Harmonic::set_An()
{
    // Get magnitude of voltage
    long double absV = abs(Vn);
    
    // Calculate alpha (junction drive level)
    long double Alpha_p = absV*F0/(p*Fp);
    
    // Calculate exp(i*n*phase theta_p)
    std::complex<long double> unitV;
    if(absV > 0)
        unitV = Vn/absV;
    else
        unitV = std::complex<long double>(1.0,0.0);
    
    std::complex<long double> conj_unitV = conj(unitV);
    
    std::complex<long double> phase = pow(unitV, (std::complex<long double>)totalJ);
    
    std::vector<long double> modAnp;
    BJ.setArrayJnx(totalJ, Alpha_p, modAnp);
    
    // Offset phase by unitV
    phase *= unitV;
            
    // Set negative Anp's
    // Set Anp(-totalJ:0) into Anp[0:totalJ]
    long double sign;
    for (int i=totalJ; i>=0;i--)
    {
        if ( i%2 == 0) sign = 1.0;
        else sign = -1.0;
        
        phase *=conj_unitV; // more negative as i increases
        An[totalJ-i] = sign*modAnp[i]*phase;
    }
    
    // set positive Anp's
    for (int i=1; i<=totalJ; i++)
    {
        phase *=conj_unitV;
        An[totalJ+i] = modAnp[i]*phase;
    }
}


// Returns nth element of Anp spectrum
std::complex<long double> One_Harmonic::get_An(int n)
{
    if( n < -(totalJ) || n > totalJ) {
        //std::cerr << "# Warning : One_Harmonic::get_An(int n=" << n << ") : n outside of range of Anp vector\n";
        return 0.0;
    }
    else {
        return An[n+totalJ];
    }
}


// Set Frequency of this harmonic
void One_Harmonic::set_Fp(long double F)
{
    Fp = F;
    set_An();
}


// Set Normalising frequency of this harmonic
void One_Harmonic::set_F0(long double F)
{
    F0 = F;
    set_An();
}


// Set Order of this harmonic
void One_Harmonic::set_p(int P)
{
    p = P;
    set_An();
}


// Set maximum Bessel order of this harmonic
void One_Harmonic::set_no_Ck(int J)
{
    totalJ = J;
    An.resize(totalJ*2+1,0);
    
    set_An();
}


// Set drive voltage of this harmonic
void One_Harmonic::set_Vn(std::complex<long double> V)
{
    Vn = V;
    set_An();
}


// The following two accessors do not need to call set_An() as they are
// for data storage only.  The values are not used in any other One_Harmonic 
// methods

// Set source voltage
void One_Harmonic::set_Vs(std::complex<long double> V)
{
    Vs = V;
}


// Set source embedding impedance
void One_Harmonic::set_Zt(std::complex<long double> Z)
{
    Zt = Z;
}

