// test touchstone_read::ports()

#include "supermix.h"
#include <cstdlib>

int main(int argc, char ** argv)
{
  if(argc != 2) {
    error::fatal(argv[0],
		 " : Need to specify input file name on command line") ;
  }

  int ports = touchstone_read::ports(argv[1]);
  
  cout << "Attempted to open file " << argv[1] << " ," << endl
       << "touchstone_read::ports() returned " << ports << endl;
}

  
