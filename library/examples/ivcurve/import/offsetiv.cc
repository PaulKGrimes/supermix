// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ********************************************************************
char * title =
 "offsetiv.cc:                                                        \n"
 "filter an SIS DC IV curve read from a file or the standard input    \n"
 "stream, removing any zero offset errors in current and voltage. The \n"
 "offsets are calculated by matching the -V to the +V portions of the \n"
 "data. Output the offset-corrected IV data to the standard output.   \n"
 "\n"
 "The IV data file format should consist of lines of V I pairs.  The  \n"
 "data should be nearly symmetrical about 0 volts. If -Vmin and Vmax  \n"
 "differ by more than 10%, offsetiv will fail with an error message.  \n"
 "Comment lines in the input data (delimited by a leading \'#\' or    \n"
 "\'!\') are passed on to the output. An additional comment block     \n"
 "containing the offset values is inserted into the file. Offsets are \n"
 "subtracted from the input data.                                     \n";

#include "supermix.h"
#include "extras/commentstream.h"
#include "extras/cmd_line.h"
#include <vector>

commentstream comment (cout, "# ");
commentstream ecomment(cerr, "# ");
vector<string> filecomments;  // hold the initial comment block of the file

command_line_string
  fname ("IV file", "input IV data file. Use \"-\" for standard input");

interpolation mdata, pdata;
parameter voff, ioff;
parameter v;
vector<double> voltages;
vector<double> currents;

// these error terms calculate the difference between the + and - segments
// of the IV curve, given the offset corrections

struct offset_error_class : public error_term_mode {
  double get(state_tag)
    { 
      // we add the offsets to the mdata, subtract from the pdata
      double e = pdata(fabs(v)+voff)-mdata(fabs(v)-voff) - 2.0*ioff;
      return checkval(e);
    }
} offset_error;

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
  // If a data line, copy the voltage into voltages, and store point
  // in mdata or pdata, as appropriate.

  do {
    if( ndata == 0 )
      filecomments.push_back(iv.line());
    else if ( ndata != 2 )
      error::fatal("Invalid IV data line: ", iv.line().c_str());
    else {
      voltages.push_back(iv.data()[0]);
      currents.push_back(iv.data()[1]);
      if (iv.data()[0] < 0.0 )
	mdata.add(-iv.data()[0],-iv.data()[1]); // reflect the data
      else
	pdata.add(iv.data()[0],iv.data()[1]);
    }
    ndata = iv.parse();
  } while (*ps);

  // set up interpolators
  error::stream(ecomment);
  mdata.quiet().linear().build();
  pdata.quiet().linear().build();
  if(!mdata.ready())
    error::fatal("offsetiv: no IV data with V < 0!");
  if(!pdata.ready())
    error::fatal("offsetiv: no IV data with V >= 0!");

  // Compare Vmin and Vmax:
  double minV = mdata.x(mdata.size()-1), maxV = pdata.x(pdata.size()-1);
  if(fabs((maxV-minV))/(maxV+minV) > 0.05)
    error::fatal("offsetiv: IV data too asymmetrical about 0!");

  // sweeper
  sweeper vsweep;
  vsweep.sweep(v, voltages);

  // set up error function and optimize
  offset_error.match(0.0);

  error_func ef;
  voff = ef.vary( -minV/10, 0, maxV/10);
  ioff = ef.vary( -mdata[mdata.size()-1]/10 , 0, pdata[pdata.size()-1]/10);
  ef.add_term(1.0, offset_error, vsweep);
  powell opt(ef);
  opt.quiet();
  opt.minimize();

  // Output a header with calculated offset errors
  char* delim = "-----\n";
  state_display::command_line(comment,argc,argv,"offsetiv.cc :");
  comment << "Offsets (subtracted from input data):\n"
	  << " v: " << voff << " , i: " << ioff << endl;
  comment << delim;

  // Now output the rest of the file comments saved previously
  for (unsigned i = 0 ; i < filecomments.size() ; ++i)
    cout << filecomments[i] << endl;

  // Now for the data
  cout << fixed << setprecision(6);
  for (unsigned i = 0; i < voltages.size(); ++i) 
    cout << voltages[i]-voff << "\t" << currents[i]-ioff << endl;
}

