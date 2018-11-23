/* MultiToneTest.cpp
*
* Driver program to test MultiTone code
*
* Written by Paul Grimes
*/

#include <iostream>
#include "MultiTone.h"

int main()
{
    // Create multitone object
    MultiTone mt;
       
    mt.set_freq(0, 0.5);
    mt.set_Zt(0, 0, Complex(0.5,0.0));
    mt.set_Vs(0, 0, Complex(0.6,0.0));

    // Add 2nd Frequency
    mt.add_frequency(0.3);
    mt.set_Zt(1, 0, Complex(0.5,0.0));
    mt.set_Vs(1, 0, Complex(1.0,0.0));
        
    cout << "# Initial no freqs: " << mt.get_no_freq() << endl;
    cout << "# Initial no harmonics: " << mt.get_no_harm(0) << endl;
 
    cout << "# Setting bias\n";
    
    mt.set_bias(0.75);
    
    cout << "# Frequencies set up\n";
    
    // Set up the response function
    mt.set_vMax(3.0);
    std::string dcFile = "31Jul03_unpump.norm.IV";
    std::string kkFile = "31Jul03_unpump.KK";
    //mt.data(dcFile, kkFile);
    mt.kennedy(50, 101);
    
    cout << "# Response function set\n";

    cout << "# Initialising harmonic balancer\n";
    mt.initHarmonicBalance();

    cout << "# Balancing device\n";
    mt.balance();
    
    cout << "# Device balanced in " << mt.get_iterations() << " iterations\n# \n";
    
    cout << "# Balanced data\n";
    cout << "# Unpump I: " << mt.get_unpumped_Idc(0.75) << endl;
    cout << "# Unpump K: " << mt.get_unpumped_Ikk(0.75) << endl;
    cout << "# DC Ip   : " << mt.get_I(0.0)  << endl;
    cout << "# Pump Vs : " << mt.get_Vs(0.5) << endl;
    cout << "# Pump Zt : " << mt.get_Zt(0.5) << endl;
    cout << "# Pump Vn : " << mt.get_Vn(0.5) << endl;
    cout << "# Pump Ip : " << mt.get_I(0.5) << endl;
    cout << "# xlast   : " << mt.get_x() << endl;
    cout << "# Fval    : " << mt.get_fval() << endl;
    //cout << "# Jacobian: \n" << mt.get_Jacobian() << endl;
    cout << "# No freqs     : " << mt.get_no_freq() << endl;
    cout << "# No harmonics : " << mt.get_no_harm(0) << endl;
    cout << "# No Ck coeffs : " << mt.get_no_Ck() << endl;
    cout << "# \n# Pumped IV Curve:\n" ;
    
    for (int v=1; v<=100; v++)
    {
        long double V = v/50.0; 
        cout << V << "\t";
        mt.set_bias(V);
        mt.balance();
        cout << mt.get_unpumped_Idc(V) << "\t" << mt.get_unpumped_Ikk(V);
        cout << "\t" << real(mt.get_I(0.0)) << "\t" << abs(mt.get_I(0,0));
        cout << "\t" << abs(mt.get_I(1,0));
        //cout << "\t" << abs(mt.get_I(0,2));
        //cout << "\t" << abs(mt.get_I(0,3));
        cout << "\t" << mt.get_iterations() << endl;
        
    }
    
    return 0;
}

