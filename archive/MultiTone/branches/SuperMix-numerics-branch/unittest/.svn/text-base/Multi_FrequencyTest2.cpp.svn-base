/* Multi_FrequencyTest.cpp
*
* Driver program to test One_Frequency code
*
* Written by Paul Grimes
*/

#include <iostream>
#include "Multi_Frequency.h"

int main()
{
    // Create frequency
    Multi_Frequency MF;
    
    // Test Accessors
    MF.set_freq(0, 0.5);
    MF.set_F0(1.0);
    
    MF.set_Vn(0, 0, Complex(0.6,0.0));
    MF.set_Zt(0, 0, Complex(1.0,0.0));
    MF.set_Vs(0, 0, Complex(0.6,0.0));
    
    
    MF.add_frequency(0.55);
    MF.set_Vn(1, 0, Complex(6.6e-4, 0.0));
    MF.set_Zt(1, 0, Complex(1.0, 0.0));
    MF.set_Vs(1, 0, Complex(6.6e-4, 0.0));
    
    MF.set_min_Ck(1e-40);
    
    MF.set_Ck();

    std::cout << "# " << MF.get_no_Ck() << " Anp in each harmonic\n";
    
    for (int i=0; i<MF.get_no_Ck(); i++)
    {
        std::cout << MF.get_Ck_freq(i) << "\t" << real(MF.get_Ck(i)) << "\t" << imag(MF.get_Ck(i)) << std::endl;
    }   
        
    return 0;
}

