#include "supermix.h"


int main(int argc, char ** argv)
{
  if(argc < 2) {
    error::fatal(argv[0],
		 " : Need to specify 2-port Touchstone input file name on command line") ;
  }

  sdata_interp d;
  d.touchstone(argv[1]);

  // output status of d
  cout << endl << "File S data from " << argv[0] << endl;
  cout << "ready(): " << d.ready() << " ; active_noise(): " << d.active_noise() << endl;
  cout << "internal Znorm: " << d.interpolator().Znorm()/Ohm << " Ohm" << endl;

  device::Z0 = d.interpolator().Znorm();

  // output the interpolator S matrix points:
  complex::out_degree(); complex::out_delimited();
  cout << endl << "The S matrix interpolator points:" << endl;
  for(unsigned i = 0; i < d.interpolator().S_interpolator().size(); ++i) {
    cout << d.interpolator().S_interpolator().x(i)/GHz << " GHz: " ;
    d.interpolator().S_interpolator()[i].show();
  }

  // output the interpolator C matrix points:
  if(d.active_noise()) {
    cout << endl << "The C matrix interpolator points: (Kelvin)" << endl;
    for(unsigned i = 0; i < d.interpolator().C_interpolator().size(); ++i) {
      cout << d.interpolator().C_interpolator().x(i)/GHz << " GHz: " ;
      (d.interpolator().C_interpolator()[i]/Kelvin).show();
    }

    // output the noise data converted back to the form in the file
    cout << endl << "The C interpolator points converted back to amplifier params:" << endl;
    for(unsigned i = 0; i < d.interpolator().C_interpolator().size(); ++i) {
      device::f = d.interpolator().C_interpolator().x(i);
      ampdata a = d.get_data();
      cout << device::f/GHz << " GHz;  Znorm: " << a.get_znorm()/Ohm << " Ohm" << endl;
      cout << "Noise: " << a.F_min() << "  " << a.gamma_opt() << "  " << a.Rn() << endl<<endl;
    }
  }


}
