// ************************************************************************
// test_iv_slope.cc
// Test the ivcurve object, including slope
//
// usage: test_iv_slope <idc_file> <ikk_file> <voltage>
// returns: <idc(voltage)>   <ikk(voltage)>  <idc'(voltage)>   <ikk'(voltage)>
//
// FR 7/10/98, 4/15/99
//
// ************************************************************************

#include "junction.h"
#include <iostream>
#include <cstdlib>  // for atof()

using namespace std;

int main(int argc, char **argv) 
{
  if(argc < 4) {
    cout << "Usage: test_iv_slope <idc_file> <ikk_file> <voltage>" << endl;
    return 1;
  }

  ivcurve iv(argv[1],argv[2]);
  pair<Complex,Complex> z;
  iv.Iprime(atof(argv[3]), z.first, z.second);

  cout << z.first.imaginary <<"\t"<< z.first.real <<"\t"
       << z.second.imaginary <<"\t"<< z.second.real << endl;
}
