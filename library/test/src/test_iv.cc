// ************************************************************************
// test_iv.cc
// Test the ivcurve object
//
// usage: test_iv <idc_file> <ikk_file> <voltage>
// returns: <idc(voltage)>   <ikk(voltage)>
//
// FR 6/29/98
//
// 7/2/98:   Updated #include file
// ************************************************************************

#include "junction.h"
#include <iostream>
#include <cstdlib>  // for atof()

using namespace std;

int main(int argc, char **argv) 
{
  if(argc < 4) {
    cout << "Usage: test_iv <idc_file> <ikk_file> <voltage>" << endl;
    return 1;
  }

  ivcurve iv(argv[1],argv[2]);
  complex z = iv.I(atof(argv[3]));

  cout << z.imaginary <<"\t"<< z.real << endl;
}
