/* MultiToneTest.cpp
*
* Driver program to test MultiTone code
*
* Written by Paul Grimes
*/

#include <iostream>
#include "MultiTone.h"

int main(int argc, char *argv[])
{
    double freq, volt, Zreal, Zimag;
    int n;
    
    // Read command line values
    if (argc < 4)
    {   cerr << "Not enough arguments\n";
        cerr << "Usage: $ ./monotone_pump <n> <freq> <voltage> <real Z> <imag Z>\n";
        return 1;
    }
    else if (argc == 4)
    {   cerr << "No embedding impedance given, using unity\n";
        n = atoi(argv[1]);
        freq = atof(argv[2]);
        volt = atof(argv[3]);
        Zreal = 1.0;
        Zimag = 0.0;
    }
    else if (argc == 5)
    {   cerr << "Only real part of embedding impedance given, assuming impedance is purely real\n";
        n= atoi(argv[1]);
        freq = atof(argv[2]);
        volt = atof(argv[3]);
        Zreal = atof(argv[4]);
        Zimag = 0.0;
    }
    else if (argc == 6)
    {   
        n = atoi(argv[1]);
        freq = atof(argv[2]);
        volt = atof(argv[3]);
        Zreal = atof(argv[4]);
        Zimag = atof(argv[5]);
    }
    else
    {   cerr << "Too many arguments, ignoring extras\n";
        n = atoi(argv[1]);
        freq = atof(argv[2]);
        volt = atof(argv[3]);
        Zreal = atof(argv[4]);
        Zimag = atof(argv[5]);
    }        
        
    cout << "# monotone_pump output\n";
    cout << "# Using polynomial quotient approximation, order n=" << n << endl;
    cout << "# Pump signal f=" << freq << " V=" << volt << " Z=" << Zreal << "+" << Zimag << "i\n# \n";
    
    
    // Create multitone object
    MultiTone mt;
    
    mt.set_freq(0, freq);
    mt.set_Vs(0, 0, Complex(volt, 0.0));
    mt.set_Zt(0, 0, Complex(Zreal, Zimag));
    
    mt.set_bias(0.90);
    
    // Set up the response function
    mt.set_vMax(3.0);
    std::string dcFile = "31Jul03_unpump.IV";
    std::string kkFile = "31Jul03_unpump.KK";
    mt.kennedy(n, 201);
    
    mt.initHarmonicBalance();

    //mt.set_Max_Iterations( 25 );
    mt.balance();
    
    cout << "# Signal Volts | Unpumped DC | KK | DC Current | Pump Current | Dynamic Z | Iterations\n";
    
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
        
        mt.set_bias(V-0.001);
        mt.balance();
        double i_low = real(mt.get_I(0.0));
        mt.set_bias(V+0.001);
        mt.balance();
        double i_high = real(mt.get_I(0.0));
        
        cout << "\t" << (i_high - i_low)/0.002;
        cout << "\t" << mt.get_iterations() << endl;
    }
    
    return 0;
}

