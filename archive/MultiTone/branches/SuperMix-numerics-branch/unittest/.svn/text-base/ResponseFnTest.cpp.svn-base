/* ResponseFnTest.cpp
*
* Driver program to test ResponseFn code
*
* Written by Paul Grimes
*/

#include <iostream>
#include <string>
#include "ResponseFn.h"

int main()
{
    std::cout << "# Creating ResponseFn object\n";
    
    ResponseFn Rfn;
    
    std::cout << "# Calling data(\"testIV.dat\", \"testVK.dat\")\n";
    
    std::string dcFile = "testIV.dat";
    std::string kkFile = "testVK.dat";
    
    Rfn.set_vMax(3.0);
    
    Rfn.data(dcFile, kkFile);
    //Rfn.kennedy(20, 200);
    
    std::cout << "# Outputting data\n";
    
    for (int i=-300; i<=300; i++)
    {
        std::cout << i/100.0 << "\t" << Rfn.get_Idc((double)i/100.0) << "\t" << Rfn.get_Ikk((double)i/101.0) << std::endl;
    }
    
    
    return 0; 
}

