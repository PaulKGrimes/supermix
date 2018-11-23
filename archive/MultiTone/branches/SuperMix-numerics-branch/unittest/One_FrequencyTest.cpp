/* One_FrequencyTest.cpp
*
* Driver program to test One_Frequency code
*
* Written by Paul Grimes
*/

#include <iostream>
#include "One_Frequency.h"

int main()
{
    // Create frequency
    std::cout << "# Creating One_Frequency object\n";
    One_Frequency OF;
    
    std::cout << "# Testing Accessors...\n# \n";
    // Test Accessors
    OF.set_F(0.5);
    OF.set_F0(1.0);
    
    OF.set_Vn(0, Complex(1.0,0.0));
    OF.set_Zt(0, Complex(1.0,0.0));
    OF.set_Vs(0, Complex(1.0,0.0));
    
    OF.set_delta_Ck0(1e-13);
    
    std::cout << "#     No of harmonics : " << OF.get_no_harm() << std::endl;
    std::cout << "#     F               : " << OF.get_F() << std::endl;
    std::cout << "#     F0              : " << OF.get_F0() << std::endl;
    std::cout << "#     delta_Ck        : " << OF.get_delta_Ck() << std::endl;
    std::cout << "#     delta_Ck_0      : " << OF.get_delta_Ck0() << std::endl;
    std::cout << "#     totalJ          : " << OF.get_no_Ck() << std::endl;
    std::cout << "#     Vn              : " << real(OF.get_Vn(0)) << " + " << imag(OF.get_Vn(0)) << " i" << std::endl;
    std::cout << "#     Vs              : " << real(OF.get_Vs(0)) << " + " << imag(OF.get_Vs(0)) << " i" << std::endl;
    std::cout << "#     Zt              : " << real(OF.get_Zt(0)) << " + " << imag(OF.get_Zt(0)) << " i" << std::endl;
    std::cout << "# \n# Testing data return...\n# \n";
    std::cout << "#     get_Cjk(0,1)    : " << real(OF.get_Cjk(0,1)) << " + " << imag(OF.get_Cjk(0,1)) << " i\n";
    std::cout << "# Calling get_Cjk(0)...\n";
    Complex res = OF.get_Cjk(0);
    std::cout << "# Done\n";
    std::cout << "#     get_Cjk(1)      : " << real(res) << " + " << imag(res) << " i\n# \n";
    
    std::cout << "# Testing add_harmonic()...\n";
    OF.add_harmonic();
    std::cout << "#     No of harmonics : " << OF.get_no_harm() << std::endl;
    OF.set_Vn(1, complex(1.0,0.0));
    std::cout << "#     get_Cjk(1,2)    : " << real(OF.get_Cjk(1,2)) << " + " << imag(OF.get_Cjk(1,2)) << " i\n# \n";
    std::cout << "# \n# Testing add_harmonic(complex, complex)...\n";
    OF.add_harmonic(Complex(1.0,0.0), Complex(1.0,0.0));
    OF.set_Vn(2, Complex(1.0,0.0));
    std::cout << "#     No of harmonics : " << OF.get_no_harm() << std::endl;
    OF.set_Vn(1, Complex(1.0,0.0));
    std::cout << "#     get_Cjk(1,1)    : " << real(OF.get_Cjk(1,1)) << " + " << imag(OF.get_Cjk(1,1)) << " i\n# \n";
    std::cout << "#     get_Cjk(2,1)    : " << real(OF.get_Cjk(2,1)) << " + " << imag(OF.get_Cjk(2,1)) << " i\n# \n";
    std::cout << "# Testing remove_harmonic()...\n";
    OF.remove_harmonic();
    OF.remove_harmonic();
    std::cout << "#     No of harmonics : " << OF.get_no_harm() << std::endl;
    std::cout << "# \n# Testing remove_harmonic() exception throw (should terminate)\n";
    OF.remove_harmonic();
    
    return 0;
}

