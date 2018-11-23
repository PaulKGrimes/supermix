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
    
    mt.set_freq(0, 0.20);
    mt.set_Vs(0, 0, Complex(1.28, 0.0));
    mt.set_Zt(0, 0, Complex(1.0, -0.5));
    
    cout << "# Pump frequency set up\n";
    
    cout << "# Setting bias\n";
    
    mt.set_bias(0.90);
        
    // Set up IF frequency
    mt.add_frequency(0.003);
    mt.set_Vs(1, 0, Complex(0.0, 0.0));
    mt.set_Zt(1, 0, Complex(1.0, 0.0));
    
    cout << "# IF frequency set up\n";
   
    // Set up signal frequency
    mt.add_frequency(mt.get_freq(0,0) + mt.get_freq(1,0));
    mt.set_Vs(2, 0, Complex(1e-4, 0.0));
    mt.set_Zt(2, 0, Complex(1.0, -0.5));
    
    cout << "# USB frequency set up\n";
   
    // Set up image frequency
    mt.add_frequency(mt.get_freq(0,0) - mt.get_freq(1,0));
    mt.set_Vs(3, 0, Complex(0.0, 0.0));
    mt.set_Zt(3, 0, Complex(1.0, -0.5));
    
    cout << "# LSB frequency set up\n";
    
    // Set up the response function
    mt.set_vMax(3.0);
    std::string dcFile = "31Jul03_unpump.IV";
    std::string kkFile = "31Jul03_unpump.KK";
    mt.kennedy(30, 201);
    
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
    cout << "# Bias | Unpumped DC | KK | DC Current | Pump Current | IF Current | Signal Current | Image Current | DC dynamic impedance | Iterations\n";
    
    for (int v=0; v<=400; v++)
    {
        long double V = v/100.0; 
        cout << V;
        mt.set_bias(V);
        mt.balance();
        cout << "\t" << mt.get_unpumped_Idc(V);
        cout << "\t" << mt.get_unpumped_Ikk(V);
        cout << "\t" << real(mt.get_I(0.0));
        cout << "\t" << abs(mt.get_I(0,0));
        cout << "\t" << abs(mt.get_I(1,0));
        cout << "\t" << abs(mt.get_I(2,0));
        cout << "\t" << abs(mt.get_I(3,0));

        mt.set_bias(V-0.001);
        mt.balance();
        double i_low = real(mt.get_I(0.0));
        mt.set_bias(V+0.001);
        mt.balance();
        double i_high = real(mt.get_I(0.0));
        
        cout << "\t" << (i_high-i_low)/0.002;
        
        cout << "\t" << mt.get_iterations() << endl;
    }
    
    return 0;
}

