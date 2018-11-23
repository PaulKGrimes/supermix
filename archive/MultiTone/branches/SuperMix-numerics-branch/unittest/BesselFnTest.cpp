/* BesselFnTest.cpp
*
* Driver program to test BesselFn code
*
* Written by Paul Grimes
*/

#include "BesselFn.h"
#include <iostream>

int main(int argc, char *argv[])
{
    int n;
    double x;
    if (argc != 3)
    {   std::cerr << "Not enough arguments, requires 2\n";
        return 1;
    }
    else 
    {   n = atoi(argv[1]);
        x = atof(argv[2]);
    }
    
    BesselFn BJ;
    
    long double test = BJ.Jnx(n, x);
    
    std::cout << "Jnx(1, 0.5) : " <<  test << std::endl;
    
    test = BJ.IntegratedJnx(n, x);
    
    std::cout << "Int(Jnx(1, 0.5)) : " << test << std::endl;
   
    return 0; 
}

