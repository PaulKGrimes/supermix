// test_datafile

#include <iostream>
#include "datafile.h"

using namespace std;

int main(int argc, char **argv) 
{
  if(argc < 2) {
    cout << "Need to specify input file name on command line\n" ;
  }
  else {
    datafile df(argv[1]) ;   // read data file
    cout << "Data file "     // spit it out
	 << argv[1]
	 << " :"
	 << endl ;
    cout << "Number of lines = "
	 << df.numlines() 
	 << endl ;
    cout << "Number of columns = "
	 << df.numcolumns() 
	 << endl
	 << endl ;
    for(int i=1; i<=df.numlines(); i++) {
      cout << "Line "
	   << i
	   << ": " ;
      for(int j=1; j<=df.numcolumns(); j++) {
	cout << df.read(j, i)
	     << "  " ;
      }
      cout << endl ;
    }
  }
}

