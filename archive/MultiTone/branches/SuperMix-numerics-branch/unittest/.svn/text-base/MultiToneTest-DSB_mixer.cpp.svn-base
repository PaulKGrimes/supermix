/* MultiToneTest-DSB_mixer.cpp
*
* Program that calculates the mixer gain as a function of the signal voltage 
* in both RF sidebands
*
* Written by Paul Grimes - 25/08/04
*/

#include <iostream>
#include "MultiTone.h"

int main(int argc, char* argv[])
{
    // Parse command line options
    double LO_freq, IF_freq, Bias, LO_volt;
    if (argc != 5)
    {
        cerr << "usage: ./MultiToneTest-DSB_sat <LO Freq> <IF Freq> <Bias> <LO Voltage>\n";
        return 1;        
    } 
    else
    {
        LO_freq = atof(argv[1]);
        IF_freq = atof(argv[2]);
        Bias    = atof(argv[3]);
        LO_volt = atof(argv[4]);
    }
    
    cout << "# MultiToneTest-DSB_sat output\n# \n";
    cout << "# LO Frequency : " << LO_freq << endl;
    cout << "# IF Frequency : " << IF_freq << endl;
    cout << "# Bias         : " << Bias    << endl;
    cout << "# LO Voltage   : " << LO_volt << endl;
    
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
    mt.set_Vs(2, 0, Complex(1e-4, 0.0));
    mt.set_Zt(2, 0, Complex(1.0, 0.0));
    
    // Set up image frequency
    mt.add_frequency(LO_freq-IF_freq);
    mt.set_Vs(3, 0, Complex(1e-4, 0.0));
    mt.set_Zt(3, 0, Complex(1.0, 0.0));
    

    // Set up the response function
    mt.set_vMax(3.0);
    std::string dcFile = "31Jul03_unpump.IV";
    std::string kkFile = "31Jul03_unpump.KK";
    mt.kennedy(30, 201);
    
    mt.initHarmonicBalance();

    //mt.set_Max_Iterations( 25 );
    mt.balance();
    
    cout << "# \n# Mixer currents curve\n";
    cout << "# Signal Volts | DC Current | LO Current | IF Current | LSB Current | USB Current | Iterations\n";
    
    double DC, LO, IF, USB, LSB;
    int n;
    
    for (int v=0; v<=40; v++)
    {
        long double V = pow(10.0, -5.0+v/10.0); 
        cout << V;
        
        n = 0;
        DC = 0.0;
        LO = 0.0;
        IF = 0.0;
        USB = 0.0;
        LSB = 0.0;

        cerr << V << "\t" ;
       
        for (int phi=0; phi <360; phi+=12)
        {

            for (int phi2=0; phi2 <360; phi2+=12)
            {

                Complex phasor = Complex(cos(phi/180*Pi), sin(phi/180*Pi));
                Complex phasor2 = Complex(cos(phi2/180*Pi), sin(phi2/180*Pi));
                mt.set_Vs(2, 0, Complex(V)*phasor);
                mt.set_Vs(3, 0, Complex(V)*phasor2);
                mt.balance();
                
                n++;
                DC += norm(mt.get_I(0.0));
                LO += norm(mt.get_I(0,0));
                IF += norm(mt.get_I(1,0));
                USB+= norm(mt.get_I(2,0));
                LSB+= norm(mt.get_I(3,0));
            }
            cerr << "#";
        }
        
        cerr << "\n";
        
        cout << "\t" << mt.get_unpumped_Idc(Bias);
        cout << "\t" << mt.get_unpumped_Ikk(Bias);
        
        cout << "\t" << sqrt(DC/n);
        cout << "\t" << sqrt(LO/n);
        cout << "\t" << sqrt(IF/n);
        cout << "\t" << sqrt(USB/n);
        cout << "\t" << sqrt(LSB/n) << endl;
    }
    
    return 0;
}
