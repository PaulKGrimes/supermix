/* OneHarmonicTest.cpp
*
* Driver program to test OneHarmonic code
*
* Written by Paul Grimes
*
* CHANGELOG:
*    01/07/04 : PKG : Changed class to use SuperMix based storage, as part of 
*               program to replace all storage with SuperMix classes, prior to
*               extension to multiple junctions.
*/

#include "One_Harmonic.h"
#include <iostream>
#include "supermix.h"


int main()
{
    One_Harmonic OH(10, 1, Complex(1.0,0.0), 0.5, 1.0);
    
    Complex test = OH.get_An(1);
    std::cout << "# An(1) : " << real(test) << " + " << imag(test) << " i\n";

    
    std::cout << "# \n# Testing Accessors...\n";
    
    OH.set_p(1);
    OH.set_no_Ck(5);
    OH.set_Vn(complex(0.56375,-0.0553926));
    OH.set_Fp(1.7);
    OH.set_F0(1.0);
    
    std::cout << "# Current Properties:\n";
    std::cout << "#    p     : " << OH.get_p() << std::endl;
    std::cout << "#    Fp    : " << OH.get_Fp() << std::endl;
    std::cout << "#    F0    : " << OH.get_F0() << std::endl; 
    std::cout << "#  totalJ  : " << OH.get_no_Ck() << std::endl;
    std::cout << "#    Vn   : " << real( OH.get_Vn() ) << " + " << imag( OH.get_Vn() ) << "i\n# \n"; 
    
    test = OH.get_An(1);
    std::cout << "# An(1) : " << real(test) << " + " << imag(test) << " i\n";
    
    for (int i=-(OH.get_no_Ck()+5); i<=OH.get_no_Ck()+5; i++)
    {
        test = OH.get_An(i);
        std::cout << i*OH.get_Fp() << "\t" << test.real << "\t" << test.imaginary << std::endl;
    }
       
    return 0; 
}
