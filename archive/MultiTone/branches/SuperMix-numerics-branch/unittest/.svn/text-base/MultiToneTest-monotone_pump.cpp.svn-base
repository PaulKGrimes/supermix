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
    
    cout << "# Initial no freqs: " << mt.get_no_freq() << endl;
    cout << "# Initial no harmonics: " << mt.get_no_harm(0) << endl;
    
    mt.set_freq(0, 1.7);
    mt.set_Vs(0, 0, Complex(2.5, 0.0));
    mt.set_Zt(0, 0, Complex(1.0, 0.0));
    
    cout << "# Setting bias\n";
    
    mt.set_bias(0.90);
    
    cout << "# Pump frequency set up\n";
    
    // Set up the response function
    mt.set_vMax(3.0);
    std::string dcFile = "31Jul03_unpump.IV";
    std::string kkFile = "31Jul03_unpump.KK";
    mt.kennedy(50, 201);
    
    cout << "# Response function set\n";

    cout << "# Initialising harmonic balancer\n";
    mt.initHarmonicBalance();

    cout << "# Balancing device\n";
    //mt.set_Max_Iterations( 25 );
    mt.balance();
    
    cout << "# Device balanced in " << mt.get_iterations() << " iterations\n# \n";
    
    cout << "# Balanced data\n";
    cout << "# Unpump I: " << mt.get_unpumped_Idc(0.9) << endl;
    cout << "# Unpump K: " << mt.get_unpumped_Ikk(0.9) << endl;
    cout << "# DC Ip   : " << mt.get_I(0.0)  << endl;
    cout << "# No freqs     : " << mt.get_no_freq() << endl;
    cout << "# No harmonics : " << mt.get_no_harm(0) << endl;
    cout << "# No Ck coeffs : " << mt.get_no_Ck() << endl;
    cout << "# \n# Mixer currents curve\n";
    cout << "# Signal Volts | Unpumped DC | KK | DC Current | Pump Current | IF Current | Signal Current | Image Current | Iterations\n";
    
    for (int v=0; v<=200; v++)
    {
        long double V = v/100.0; 
        cout << V;
        mt.set_bias(V);
        mt.balance();
        cout << "\t" << mt.get_unpumped_Idc(V);
        cout << "\t" << mt.get_unpumped_Ikk(V);
        cout << "\t" << real(mt.get_I(0.0));
        cout << "\t" << abs(mt.get_I(0,0));
        cout << "\t" << mt.get_iterations() << endl;
    }
    
    return 0;
}

