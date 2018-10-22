// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ********************************************************************
char * title =
 "topositive.cc:                                                      \n"
 "Convert SIS DC IV curve data read from a file or the standard input \n"
 "stream into smoothed, nonnegative data points starting with V=0,    \n"
 "I=0. The data is calculated by adaptively filling a cubic spline    \n"
 "interpolator representing the smoothed data.                        \n"
 "\n"
 "The IV data file format should consist of lines of V I pairs which  \n"
 "form a single sweep from low(-) to high(+) voltage. If the data     \n"
 "includes both + and - voltages, then the data should be nearly      \n"
 "symmetrical about 0 volts. If -Vmin and Vmax differ by more than 10%\n"
 "the program will only use that side of 0 covering the greater range \n"
 "of voltage.                                                         \n"
 "\n"
 "Comment lines in the input data (delimited by a leading \'#\' or    \n"
 "\'!\') are passed on to the output. An additional comment block is  \n"
 "inserted into the file.                                             \n";

#include "supermix.h"
#include "adaptive.h"
#include "extras/commentstream.h"
#include "extras/cmd_line.h"
#include <algorithm>

commentstream comment(cout, "# ");

command_line_string
  fname ("IV file", "input IV data file. Use \"-\" for standard input");

command_line_parameter 
  sm (1,"smoothing factor", "how agressive to be with smoothing - try 1.0");

vector<double> voltages;
vector<double> currents;

interpolation mdata, pdata, outdata;
// these booleans let use know which interpolators have useful data:
bool use_mdata, use_pdata, use_both;

// this function averages the estimates from the +V and -V data
double f(double v)
{
  double result;
  if(use_both)
    result =0.5*(mdata(v)+pdata(v));
  else if(use_mdata)
    result = mdata(v);
  else
    result = pdata(v);
  return result;
}



int main(int argc, char **argv)
{
  get_command_line(argc,argv,title);

  // set up the parser for the input file specified on the command line
  data_parser iv;
  istream *ps;
  ifstream s;
  if(fname.arg.size()==1 && fname.arg[0]=='-')
    ps = &cin;
  else {
    s.open(fname.arg.c_str());
    ps = &s;
  }
  iv.input(*ps);

  // get the first line. If it is a comment line (contains no doubles),
  // then copy it to cout.

  unsigned ndata = iv.parse();
  if(ndata == 0) {
    // the line doesn't contain data
    cout << iv.line() << endl;
    ndata = iv.parse(); // fetch the next line;
  }

  // Now output a comment block with the parameters used by topositive:
  char* delim = "-----\n";
  state_display::command_line(comment,argc,argv,"topositive.cc :");
  comment << delim;

  // read lines from the file.
  // If a comment line, pass on to cout.
  // If a data line, store point.

  do {
    if( ndata == 0 )
      cout << iv.line() << endl;
    else if ( ndata != 2 )
      error::fatal("topositive: invalid IV data line: ", iv.line().c_str());
    else {
      voltages.push_back(iv.data()[0]);
      currents.push_back(iv.data()[1]);
    }
    ndata = iv.parse();
  } while (*ps);

  // Sort voltages into ascending order while leaving currents alone.
  // This should avoid gross errors in the interpolations near the gap
  sort(voltages.begin(), voltages.end());

  // set up input data interpolators
  for (unsigned i = 0; i < voltages.size(); i++) {
    if(voltages[i] < 0.0)
      mdata.add(-voltages[i],-currents[i]);
    else
      pdata.add(voltages[i],currents[i]);
  }
  mdata.quiet().linear().build();
  pdata.quiet().linear().build();
  use_mdata = mdata.ready(); use_pdata = pdata.ready();
  use_both = use_mdata && use_pdata;
  if(!use_mdata && !use_pdata)
    error::fatal("topositive: insufficient IV input data!");

  // get limits of the data sets:
  double mVmin, mVmax, pVmin, pVmax, Vmin, Vmax;
  if(use_mdata) {
    mVmin = mdata.x(0); mVmax = mdata.x(mdata.size()-1);
  }
  if(use_pdata) {
    pVmin = pdata.x(0); pVmax = pdata.x(pdata.size()-1);
  }
  if(use_both) {
    double delta = (pVmax-mVmax)/(pVmax+mVmax);
    if(delta > 0.05)
      use_both = use_mdata = false;
    if(delta < -0.05)
      use_both = use_pdata = false;
  }
  if(use_both) {
    Vmin = (mVmin > pVmin) ? mVmin : pVmin;
    Vmax = (mVmax < pVmax) ? mVmax : pVmax;
  }
  else {
    Vmin = (use_mdata) ? mVmin : pVmin;
    Vmax = (use_mdata) ? mVmax : pVmax;
  } 

  // build the output data interpolator:
  outdata.add(0.0,0.0);
  adaptive_fill(outdata, f, Vmin, Vmax, f(Vmax)*(1.0e-4)*sm*sm, .01*sm);

  // Now output the (V,I) points in the interpolator.
  cout << fixed << setprecision(6);
  for(unsigned i = 0; i < outdata.size(); ++i)
    cout << outdata.x(i) << "\t" << outdata[i] << endl;

}
