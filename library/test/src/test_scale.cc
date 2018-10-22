// test scalerow() in matmath

#include "supermix.h"
#include "complex_interp.h"
#include "parameter/scaled_real_parameter.h"

int main()
{
  real_table dt(datafile("Zslot.750"));
  datafile df("Zslot.750");
  scaled_real_parameter freq_GHz(1/GHz,device::f);	// Used by Za (below)
  complex_interp Za(df,1,2,3,freq_GHz);
  complex_interp Zb(dt,1,2,3,device::f);

  transpose(dt).show();

  cout << endl << "GHz =" << GHz;
  cout << endl << "Ohm =" << Ohm << endl << endl;

  scalerow(1,dt,GHz);
  scalerow(2,dt,Ohm);
  scalerow(3,dt,Ohm);

  transpose(dt).show();

  device::f = 150*GHz;
  cout << "f = " << device::f << ", freq_GHz = " << freq_GHz << endl;
  cout << "Zb = " << Zb << ", Za = " << Za << endl;
  
}
