// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ********************************************************************
char * title =
 "pruneiv.cc:                                                         \n"
 "filter an SIS DC IV curve read from a file or the standard input    \n"
 "stream, removing data lines with voltages whose absolute values are \n"
 "either too large or too close to zero. Output to standard output.   \n"
 "\n"
 "The IV data file format should consist of lines of V I pairs.       \n"
 "Comment lines in the input data (delimited by a leading \'#\' or    \n"
 "\'!\') are passed on to the output. An additional comment block is  \n"
 "inserted into the file giving the arguments used by pruneiv.        \n"; 

#include "supermix.h"
#include "extras/cmd_line.h" 
#include "extras/commentstream.h"

commentstream comment(cout, "# ");

command_line_string
  fname ("IV file", "input IV data file. Use \"-\" for standard input");

command_line_parameter 
  dend  (1,"max V", "prune points whose |voltage| exceeds this value"),
  dzero (1,"zero delta", "prune points within this voltage of zero");

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

  // Now output a comment block with the parameters used by pruneiv:
  char* delim = "-----\n";
  state_display::command_line(comment,argc,argv,"pruneiv.cc :");
  state_display::info(comment);
  comment << delim;

  //this is the pruning loop
  do {
    if( ndata == 0 ||   // ndata == 0 means no data, so line is a comment
	(fabs(iv.data()[0]) <= dend && fabs(iv.data()[0]) >= dzero) )
      cout << iv.line() << endl;
    ndata = iv.parse();
  } while (*ps);
}

