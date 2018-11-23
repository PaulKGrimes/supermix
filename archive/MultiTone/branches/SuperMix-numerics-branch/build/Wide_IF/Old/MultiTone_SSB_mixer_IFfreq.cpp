/* MultiTone_SSB_mixer_IFfreq.cpp
*
* MultiTone program to scan IF freq for a SSB (USB only) mixer
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
    
	// Set up LO
    mt.set_freq(0, 0.95);
    mt.set_Vs(0, 0, Complex(1.0, 0.0));
    mt.set_Zt(0, 0, Complex(1.0, 0.0));
    
    cout << "# LO frequency set up\n";
    
    cout << "# Setting bias\n";
    
    mt.set_bias(0.5);
        
    // Set up IF frequency
    mt.add_frequency(0.001);
    mt.set_Vs(1, 0, Complex(0.0, 0.0));
    mt.set_Zt(1, 0, Complex(2.5, 0.0));
    
    cout << "# IF frequency set up\n";
   
    // Set up signal frequency
    mt.add_frequency(mt.get_freq(0,0) + mt.get_freq(1,0));
    mt.set_Vs(2, 0, Complex(1e-4, 0.0));
    mt.set_Zt(2, 0, Complex(1.0, 0.0));
    
    cout << "# USB frequency set up\n";
	
    // Set up image frequency
    mt.add_frequency(mt.get_freq(0,0) - mt.get_freq(1,0));
    mt.set_Vs(3, 0, Complex(0.0, 0.0));
    mt.set_Zt(3, 0, Complex(1.0, 0.0));
    
    cout << "# LSB frequency set up\n";
    
    // Set up the response function
    mt.set_vMax(3.0);
    std::string dcFile = "unpump.idc";
    std::string kkFile = "unpump.ikk";
    mt.data(dcFile, kkFile);
	//mt.kennedy(30,301);
    
    cout << "# Response function set\n";

    //cout << "# Initialising harmonic balancer\n";
    mt.initHarmonicBalance();

    //cout << "# Balancing device\n";
    //mt.set_Max_Iterations( 25 );
    mt.balance();
    
    /*cout << "# Device balanced in " << mt.get_iterations() << " iterations\n# \n";
    
    cout << "# Balanced data\n";
    cout << "# Unpump I: " << mt.get_unpumped_Idc(0.9) << endl;
    cout << "# Unpump K: " << mt.get_unpumped_Ikk(0.9) << endl;
    cout << "# DC Ip   : " << mt.get_I(0.0)  << endl;
    cout << "# No freqs     : " << mt.get_no_freq() << endl;
    cout << "# No harmonics : " << mt.get_no_harm(0) << endl;
    cout << "# No Ck coeffs : " << mt.get_no_Ck() << endl;*/
    cout << "# \n# Mixer currents curve\n";
    cout << "# IF Freq | Unpumped DC | KK | DC Current | Pump Current | IF Current | Signal Current | Image Current | DC dynamic impedance | Iterations\n";
    
    for (int f=1; f<=200; f++)
    {
        long double F = f/1000.0; 
        cout << F;
        mt.set_freq(1, F);
		mt.set_freq(2, mt.get_freq(0,0)+F);
		mt.set_freq(3, mt.get_freq(0,0)-F);
        mt.balance();
        cout << "\t" << mt.get_unpumped_Idc(0.5);
        cout << "\t" << mt.get_unpumped_Ikk(0.5);
        cout << "\t" << real(mt.get_I(0.0));
        cout << "\t" << abs(mt.get_I(0,0));
        cout << "\t" << abs(mt.get_I(1,0));
        cout << "\t" << abs(mt.get_I(2,0));
		cout << "\t" << abs(mt.get_I(3,0));
		
        cout << "\t" << mt.get_iterations() << endl;
    }
    
    return 0;
}

