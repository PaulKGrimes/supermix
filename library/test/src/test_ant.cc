// test the objects in antenna.h

#include "supermix.h"


int main(int argc, char ** argv)
{
  // get rid of tiny imaginary parts
  Complex::rel_tolerance = Complex::abs_tolerance = 1.0e-10;

  if(argc != 2) {
    error::fatal(argv[0],
		 " : Need to specify input file name on command line") ;
  }

  // make a twin-slot from two single-slot antennas
  complex_interp ant_interp(device::f);
  ant_interp.file(argv[1], GHz, Ohm);
  slot_antenna ant_1, ant_2;
  ant_1.Z = & ant_interp;
  ant_2.Z = & ant_interp;
  power_divider split;
  circuit a;
  a.add_port(split,1);
  a.connect(split,2,ant_1,1);
  a.connect(split,3,ant_2,1);
  a.add_port(ant_1,2);
  a.add_port(ant_1,3);
  a.add_port(ant_2,2);
  a.add_port(ant_2,3);


  // make the twin slot directly
  sdata_interp ant_s(1);
  ant_s.touchstone(argv[1],GHz);
  twin_slot_antenna b(ant_s);

  // output the S and C data
  device::T = 4*Kelvin;
  for(device::f = 675*GHz; device::f <= 825*GHz; device::f += 50*GHz) {
    sdata s;
    s = a.get_data();
    complex::out_polar();
    complex::out_delimited();
    s = a.get_data();
    cout<< fixed << setprecision(3) << setw(16); s.S.show();
    s = b.get_data();
    cout<< fixed << setprecision(3) << setw(16); s.S.show();
    s = a.get_data();
    cout<< fixed << setprecision(3) << setw(16); s.C.show();
    s = b.get_data();
    cout<< fixed << setprecision(3) << setw(16); s.C.show();
  }
}
