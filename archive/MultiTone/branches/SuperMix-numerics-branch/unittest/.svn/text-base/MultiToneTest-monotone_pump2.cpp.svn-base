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
    mt.set_Vs(0, 0, Complex(1.0, 0.0));
    mt.set_Vn(0, 0, Complex(1.0, 0.0));
    mt.set_Zt(0, 0, Complex(1.0, 0.0));
    
    cout << "# Setting bias\n";
    
    mt.set_bias(0.85);
    
    cout << "# Pump frequency set up\n";
    
    // Set up the response function
    mt.set_vMax(3.0);
    std::string dcFile = "Ideal_30.idc";
    std::string kkFile = "Ideal_30_SM.ikk";
    mt.data(dcFile, kkFile);
    
    cout << "# Response function set\n";
    
    cout << "# Current calculation test:\n";
    cout << "# V: 0.85\t1.0 + 0.0i\n";
    cout << "# I: " << mt.get_Ip(0.0) << "\t" << mt.get_Ip(1.7).real << " + " << mt.get_Ip(1.7).imaginary << "i\n# \n";
    
    cout << "# Ck Values:\n";
    for (int i=0; i<mt.get_no_Ck(); i++)
    {   
        std::cout << "# " << mt.get_Ck_freq(i) << "\t" << mt.get_Ck(i).real << "\t" << mt.get_Ck(i).imaginary << endl;
    }
        

    cout << "# Initialising harmonic balancer\n";
    mt.initHarmonicBalance();

    cout << "# Balancing device\n";
    //mt.set_Max_Iterations( 25 );
    mt.balance();
    
    cout << "# Device balanced in " << mt.get_iterations() << " iterations\n# \n";
    
    cout << "# Balanced data\n";
    cout << "# Unpump I: " << mt.get_unpumped_Idc(0.85) << endl;
    cout << "# Unpump K: " << mt.get_unpumped_Ikk(0.85) << endl;
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
        cout << "\t" << real(mt.get_I(0,0));
        cout << "\t" << imag(mt.get_I(0,0));
        cout << "\t" << real(mt.get_Vn(0,0));
        cout << "\t" << imag(mt.get_Vn(0,0));
        cout << "\t" << mt.get_iterations() << endl;
    }
    
    return 0;
}

