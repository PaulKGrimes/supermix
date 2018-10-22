// test the touchstone reader

#include "supermix.h"
#include <cstdlib>

int main(int argc, char ** argv)
{
  if(argc < 3) {
    error::fatal(argv[0],
		 " : Need to specify input file name and number of ports on command line") ;
  }

  touchstone_read d;
  bool ok = d.open(argv[1],atoi(argv[2]));
  
  cout << "Attempted to open file " << argv[1] << " , open() returned " << ok << endl;
  cout << "  good() returns " << d.good() << endl << endl;
  cout << "Normalizing Z: " << d.z()/Ohm << " Ohm" << endl;
  cout << "Type of matrix: " ;
  switch (d.type()) {
  case touchstone_read::S : cout << "S" ; break;
  case touchstone_read::Z : cout << "Z" ; break;
  case touchstone_read::Y : cout << "Y" ; break;
  }
  cout << endl << endl;

  cout << "Here's the data as an S matrix:" << endl << "(mag,degrees)" << endl;
  complex::out_degree(); complex::out_delimited();

  double f;
  Matrix M;

  while (d.Svalue(f,M)) { 
    cout << f/GHz << " GHz:";
    M.show();
  }

  if(d.has_noise()) {
    cout << endl << "Here's the raw noise data:" << endl;
    touchstone_read::noise N;
    while (d.get_noise(f,N)) {
      cout << f/GHz << " GHz: " << N.Fmin << "  " << N.Gopt << "  " << N.Reff/d.z() << endl;
    }
  } 
}

  
