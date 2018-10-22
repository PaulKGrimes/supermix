// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ********************************************************************
char * title =
 "normalizeiv.cc:                                                     \n"
 "filter an SIS DC IV curve read from a file or the standard input    \n"
 "stream, normalizing the gap voltage and the normal resistance to 1.0\n"
 "so that the output data is suitable for use by the supermix ivcurve \n"
 "class.                                                              \n"
 "\n"
 "The IV data file format should consist of lines of V I pairs.       \n"
 "Comment lines in the input data (delimited by a leading \'#\' or    \n"
 "\'!\') are passed on to the output. An additional comment block is  \n"
 "inserted into the file giving the normalization values.             \n"; 

#include "supermix.h"
#include "extras/cmd_line.h" 
#include "extras/commentstream.h"
#include <vector>

commentstream comment(cout, "# ");
vector<string> filecomments;  // hold the initial comment block of the file

command_line_string
  fname ("IV file", "input IV data file. Use \"-\" for standard input");

interpolation data;

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

  // read lines from the file.
  // If a comment line, save in filecomments.
  // If a data line, store point in data interpolator.
  do {
    if( ndata == 0 )
      filecomments.push_back(iv.line());
    else if ( ndata != 2 )
      error::fatal("normalizeiv: invalid IV data line: ", iv.line().c_str());
    else
      data.add(iv.data()[0],iv.data()[1]);
    ndata = iv.parse();
  } while (*ps);

  // set up interpolator
  data.quiet().spline().build();
  if(!data.ready())
    error::fatal("normalizeiv: insufficient IV data!");

  // determine the normalization values
  double endslope = data.prime(data.x(data.size()-1));
  double maxslope = 0.0, vgap = 0.0;
  for(unsigned i = 1; i < data.size(); ++i) {
    double v = data.x(i); 
    if(data.prime(v) >= maxslope) {
      vgap = v;
      maxslope = data.prime(v);
    }
  }
  vgap = fabs(vgap);
  double rn = 1/endslope;

  // output status info, including normalization values:
  char* delim = "-----\n";
  state_display::command_line(comment,argc,argv,"normalizeiv.cc :");
  comment << " Vgap = " << vgap << endl;
  comment << " Rn   = " << rn << " , 1000*Rn = " << rn*1000 << endl;
  comment << delim;

  // Now output the rest of the file comments saved previously
  for (unsigned i = 0 ; i < filecomments.size() ; ++i)
    cout << filecomments[i] << endl;

  // Now for the normalized data
  cout << fixed << setprecision(6);
  for (unsigned i = 0; i < data.size(); ++i) 
    cout << data.x(i)/vgap << "\t" << data[i]*rn/vgap << endl;
}
