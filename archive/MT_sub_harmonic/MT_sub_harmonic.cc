#include <iostream>

#include "MultiTone.h"

int main(char argc, char *argv[])
{
	// Read and convert command line arguments
	parameter rf_freq, rf_volt, if_freq, bias;
	int harmonic;
	std::string iv_data, kk_data;
	
	if (argc != 7)
	{
		cerr << "Usage: ./MT_sub_harmonic <ivcurve> <kkcurve> <rf_freq> <rf_volt> <if_freq> <harmonic>\n";
		return 1;
	}
	else
	{
		iv_data = argv[1];
		kk_data = argv[2];
		rf_freq = atof(argv[3]);
		rf_volt = atof(argv[4]);
		if_freq = atof(argv[5]);
		harmonic = atoi(argv[6]);
	}
	
	cout << "# MT_sub_harmonic output\n# \n";
	cout << "# ivcurve : " << iv_data << endl;
	cout << "# kkcurve: " << kk_data << endl;
	cout << "# rf_freq: " << rf_freq/1.0 << endl;
	cout << "# rf_volt: " << rf_volt/1.0 << endl;
	cout << "# if_freq: " << if_freq/1.0 <<endl;
	cout << "# harmonic: " << harmonic << endl;
	cout << "# \n";
	
	MultiTone mt;
	
	mt.set_bias(0.75);
	mt.set_freq(0, rf_freq);
	
	mt.set_Zt(0, 0, Complex (1.0,0.0));
	mt.set_Vs(0, 0, Complex (rf_volt, 0.0));
	
	if (harmonic>1)
	{
		mt.add_frequency(harmonic*rf_freq);
	}
	
	mt.add_frequency(harmonic*rf_freq+if_freq);
	mt.set_Zt(1, 0, Complex(1.0, 0.0));
		
	mt.add_frequency(harmonic*rf_freq-if_freq);
	
	mt.add_frequency(if_freq);

	mt.add_frequency(harmonic*rf_freq+2*if_freq);
	
	mt.data(iv_data, kk_data);
	
	mt.initHarmonicBalance();
	
	mt.balance();
	
	for (int b=0; b<=50; b++)
	{
		bias = b/25.0;
		mt.set_bias(bias);
		
		mt.balance();
		
		cout << bias << "\t" << abs(mt.get_I(0.0)) << "\t" << abs(mt.get_I(0,0)) << "\t" << abs(mt.get_I(0,harmonic-1)) << endl;
	}
}

