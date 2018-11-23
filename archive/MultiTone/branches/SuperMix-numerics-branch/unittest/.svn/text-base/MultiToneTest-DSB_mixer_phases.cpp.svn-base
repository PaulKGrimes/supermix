/* MultiToneTest-DSB_mixer_phases.cpp
*
* Program that calculates the mixer gain as a function of the signal voltage 
* phase in both RF sidebands.  The phases are calculated as difference
* between the centre of the two sidebands and the LO, and as the 
* difference between the two sidebands
*
* Written by Paul Grimes - 25/08/04
*/

#include <iostream>
#include "MultiTone.h"

int main(int argc, char* argv[])
{
    // Parse command line options
    double LO_freq, IF_freq, Bias, LO_volt, SF_volt;
    if (argc != 6)
    {
        cerr << "usage: ./MultiToneTest-DSB_mixer_phases <LO Freq> <IF Freq> <Bias> <LO Voltage> <SF Voltage>\n";
        return 1;        
    } 
    else
    {
        LO_freq = atof(argv[1]);
        IF_freq = atof(argv[2]);
        Bias    = atof(argv[3]);
        LO_volt = atof(argv[4]);
        SF_volt = atof(argv[5]);
    }
    
    cout << "# MultiToneTest-DSB_mixer_phases output\n# \n";
    cout << "# LO Frequency : " << LO_freq << endl;
    cout << "# IF Frequency : " << IF_freq << endl;
    cout << "# Bias         : " << Bias    << endl;
    cout << "# LO Voltage   : " << LO_volt << endl;
    cout << "# SF Voltage   : " << SF_volt << endl;
    
    // Create multitone object
    MultiTone mt;

    mt.set_bias(Bias);
    
    mt.set_freq(0, LO_freq);
    mt.set_Vs(0, 0, Complex(LO_volt, 0.0));
    mt.set_Zt(0, 0, Complex(1.0, 0.0));
        
    // Set up IF frequency
    mt.add_frequency(IF_freq);
    mt.set_Vs(1, 0, Complex(0.0, 0.0));
    mt.set_Zt(1, 0, Complex(1.0, 0.0));
    
    // Set up signal frequency
    mt.add_frequency(LO_freq+IF_freq);
    mt.set_Vs(2, 0, Complex(SF_volt, 0.0));
    mt.set_Zt(2, 0, Complex(1.0, 0.0));
    
    // Set up image frequency
    mt.add_frequency(LO_freq-IF_freq);
    mt.set_Vs(3, 0, Complex(SF_volt, 0.0));
    mt.set_Zt(3, 0, Complex(1.0, 0.0));
    

    // Set up the response function
    mt.set_vMax(3.0);
    std::string dcFile = "31Jul03_unpump.IV";
    std::string kkFile = "31Jul03_unpump.KK";
    mt.kennedy(30, 201);
    
    cout << "# Initialising mixer\n";
    mt.initHarmonicBalance();

    cout << "# Balancing mixer\n";
    //mt.set_Max_Iterations( 25 );
    mt.balance();
    
    cout << "# \n# Mixer phase surface\n# \n";
    cout << "# LO phase | Sideband phase diff | DC Current | LO Current | IF Current | LSB Current | USB Current\n";
        
      
    for (int phi=0; phi <360; phi+=6)
    {

        for (int phi2=0; phi2 <360; phi2+=6)
        {

            Complex phasor = Complex(cos((phi-phi2)/180.*Pi), sin((phi-phi2)/180.*Pi));
            Complex phasor2 = Complex(cos((phi+phi2)/180.*Pi), sin((phi+phi2)/180.*Pi));
            mt.set_Vs(2, 0, Complex(SF_volt)*phasor);
            mt.set_Vs(3, 0, Complex(SF_volt)*phasor2);
            mt.balance();
            
            cout << phi;
            cout << "\t" << phi2;
            cout << "\t" << abs(mt.get_I(0.0));
            cout << "\t" << abs(mt.get_I(0,0));
            cout << "\t" << abs(mt.get_I(1,0));
            cout << "\t" << abs(mt.get_I(2,0));
            cout << "\t" << abs(mt.get_I(3,0)) << std::endl;
        }
    }
    
    return 0;
}
