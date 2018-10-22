// ************************************************************************
// test_linterp.cc
// Tests datafile reading, table searching and interpolation
//
// Needs a data file name on the command line
//
// F. Rice 2/12/98
//
// 11/11/98: Changed table access to new syntax
// 7/28/98: added <iostream.h>, since SIScmplx.h doesn't any more
// ************************************************************************


#include <iostream>
#include "global.h"
#include "datafile.h"

using namespace std;

const int points = 100;   // output table size
const double Max = 1.5;   // what multiple of input max is output max

int main(int argc, char **argv) 
{
  if(argc < 2) {
    cout << "Need to specify input file name on command line\n" ;
    return 1;
  }

  int i;
  real_table A = datafile(argv[1]);

  // build table to hold interpolated output results
  double scale = Max * A.read(1,A.Rmaxindex()) / (points-1);
  double x;
  real_table B(2,points);
  for (i = 0; i < B.Rmaxindex(); ++i) {
    B[1][i+1] = x = i * scale;  // independent variable
    B[2][i+1] = linterp(x, A, 1, 2);
  }

  // Now show the results
  transpose(B).show();

  return 0;
}

