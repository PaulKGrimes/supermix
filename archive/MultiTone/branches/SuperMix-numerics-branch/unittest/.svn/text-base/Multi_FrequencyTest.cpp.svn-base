/* Multi_FrequencyTest.cpp
*
* Driver program to test One_Frequency code
*
* Written by Paul Grimes
*/

#include <iostream>
#include "Multi_Frequency.h"
#include "supermix.h"

int main()
{
    // Create frequency
    Multi_Frequency MF;
    
    // Test Accessors
    MF.set_freq(0, 0.5);
    MF.set_F0(1.0);
    
    MF.set_Vn(0, 0, Complex(1.0,0.0));
    MF.set_Zt(0, 0, Complex(1.0,0.0));
    MF.set_Vs(0, 0, Complex(1.0,0.0));
    
    MF.set_min_Ck(1e-5);
    
    std::cout << "# Testing Accessors...\n# \n";
    std::cout << "#   No of frequencies : " << MF.get_no_freq() << std::endl;
    std::cout << "#    No of harmonics  : " << MF.get_no_harm(0) << std::endl;
    std::cout << "#     F(0)            : " << MF.get_freq(0) << std::endl;
    std::cout << "#    test get_f(freq) : " << MF.get_f(MF.get_freq(0)) << std::endl;
    std::cout << "#     F0              : " << MF.get_F0() << std::endl;
    std::cout << "#     min_Ck          : " << MF.get_min_Ck() << std::endl;
    std::cout << "#     totalJ          : " << MF.get_no_Ck() << std::endl;
    std::cout << "#     Vn              : " << real(MF.get_Vn(0,0)) << " + " << imag(MF.get_Vn(0,0)) << " i" << std::endl;
    std::cout << "#     Vs              : " << real(MF.get_Vs(0,0)) << " + " << imag(MF.get_Vs(0,0)) << " i" << std::endl;
    std::cout << "#     Zt              : " << real(MF.get_Zt(0,0)) << " + " << imag(MF.get_Zt(0,0)) << " i" << std::endl;
    std::cout << "# \n# Testing data return...\n";
    std::cout << "#     get_Ck(1)    : " << real(MF.get_Ck(1)) << " + " << imag(MF.get_Ck(1)) << " i\n";
    
    std::cout << "# \n# Testing add_harmonic(0)...\n";
    MF.add_harmonic(0);
    std::cout << "#     No of harmonics : " << MF.get_no_harm(0) << std::endl;
    MF.set_Vn(0, 1, Complex(1.0,0.5));
    std::cout << "#     get_Ck(1)    : " << real(MF.get_Ck(1)) << " + " << imag(MF.get_Ck(1)) << " i\n# \n";
    std::cout << "# \n# Testing add_frequency(0.51)\n";
    MF.add_frequency(0.51);
    MF.set_Vn(1, 0, Complex(1.0,0.0));
    std::cout << "#     No of Frequencies : " << MF.get_no_freq() << std::endl;
    std::cout << "#     get_Ck(1)    : " << real(MF.get_Ck(1)) << " + " << imag(MF.get_Ck(1)) << " i\n# \n";
    std::cout << "# \n# Testing add_harmonic(1)...\n";
    MF.add_harmonic(1);
    std::cout << "#     No of harmonics : " << MF.get_no_harm(1) << std::endl;
    MF.set_Vn(1, 1, Complex(1.0,0.0));
    std::cout << "#     get_no_Ck()  : " << MF.get_no_Ck() << std::endl;
    std::cout << "#     get_Ck(1)    : " << real(MF.get_Ck(1)) << " + " << imag(MF.get_Ck(1)) << " i\n# \n";
    std::cout << "#     Test get_f(freq) : " << MF.get_f(1.4) << std::endl;
    std::cout << "#     Test get_freq(f, h) : " << MF.get_freq(1, 1) << std::endl;
    /*std::cout << "# \n# Testing remove_harmonic(1)...\n";
    MF.remove_harmonic(1);
    std::cout << "#     No of harmonics : " << MF.get_no_harm(1) << std::endl;
    std::cout << "#     get_no_Ck() : " << MF.get_no_Ck() << std::endl;
    std::cout << "#     get_Ck(1)    : " << real(MF.get_Ck(1)) << " + " << imag(MF.get_Ck(1)) << " i\n# \n";
    std::cout << "# \n# Testing remove_frequency(1)...\n";
    MF.remove_frequency(1);
    std::cout << "#     No of freqs  : " << MF.get_no_freq() << std::endl;
    std::cout << "#     get_no_Ck() : " << MF.get_no_Ck() << std::endl;
    std::cout << "#     get_Ck(1)    : " << real(MF.get_Ck(1)) << " + " << imag(MF.get_Ck(1)) << " i\n# \n";*/

    for (int i=0; i<MF.get_no_Ck(); i++)
    {
        std::cout << MF.get_Ck_freq(i) << "\t" << real(MF.get_Ck(i)) << "\t" << imag(MF.get_Ck(i)) << std::endl;
    }   
        
    return 0;
}

