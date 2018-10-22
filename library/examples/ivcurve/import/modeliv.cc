// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ********************************************************************
char * title =
 "modeliv.cc:                                                         \n"
 "Read a measured SIS DC IV curve from a file or the standard input   \n"
 "and attempt to fit the data using a simple model of an SIS IV (as   \n"
 "used in supermix/examples/ivcurve/fakeiv.cc). Output a normalized   \n"
 "DC IV suitable for input to the supermix ivcurve class.             \n"
 "                                                                    \n"
 "The IV data file format should consist of lines of V I pairs.       \n"
 "Comment lines in the input data (delimited by a leading \'#\' or    \n"
 "\'!\') are passed on to the output. An additional comment block     \n"
 "is inserted containing model IV parameters, as well as values for   \n"
 "the SIS gap voltage and the normal resistance. The quoted Rn will   \n"
 "be in ohms if the supplied V data are in millivolts and the I in    \n"
 "microamps.                                                          \n";

#include "supermix.h"
#include "numerical.h"
#include "extras/commentstream.h"
#include "extras/cmd_line.h"

commentstream comment (cout, "# ");
commentstream ecomment(cerr, "# ");

command_line_string
  fname ("IV file", "input IV data file. Use \"-\" for standard input");

// normalization parameters for the IV curve:
command_line_parameter
  Vgap(1,"Vgap estimate", "SIS gap voltage"),
  Rn(1,"Rn estimate", "normal resistance (ohms if V in mV and I in uA)");

// ---------------------------------------------------------------------------
// Functions used to simulate a simple SIS dc IV response:

// parameters of the IV model:
parameter
  G_subgap,      // Subgap/Normal conductance ratio  (0.0 - 1.0)
  Gap_width,     // Relative width of the gap        (0.0 - 1.0)
  I_leakage,     // Leakage current/(Vgap/Rn)        (0.0 - 1.0)
  I_defect;      // Current defect above gap         (0.0 - 1.0)

// output a straight line fit: y = mx+b:
inline double line(double x, double m, double b) { return m*x + b; }

// a hyperbolic tangent step function with some parameter checking
double sym_step(double x, double w)
{
  if (w > 0.0) return tanh(x/w);
  else {
    if (x  < 0.0) return -1.0;
    else if (x == 0.0) return  0.0;
    else return  1.0;
  }
}

// output a finite-width generalization of the Heaviside step function,
// using function sym_step():
inline double step(double x, double w) { return 0.5*(sym_step(x,w)+1.0); }

// The full simulation of the iv curve, using the curve parameters.
// It uses sym_step() at the origin to fade in the subgap behavior, and
// step() at the gap to transition from subgap to above-gap behavior.
double f(double v)
{
  double av     = fabs(v);
  double i_sg   = line(av, G_subgap, I_leakage) * sym_step(v, sqrt(Gap_width));
  double i_ag   = line(av, 1.0, -I_defect) * sym_step(v, Gap_width);
  double weight = step(av - 1.0, Gap_width);
  return weight*i_ag + (1.0-weight)*i_sg;
}

// ---------------------------------------------------------------------------
// Objects used to store the input file data and compare to the model

vector<string> filecomments;  // hold the initial comment block of the file
interpolation data;
parameter v;

// this error term calculates the difference between the model and the data
struct model_error_class : public error_term_mode {
  double get(state_tag)
    {
      // calculate the I(v) from the normalized model:
      double model = f(v/Vgap)*Vgap/Rn*1000; 
      // the difference from the real data is the error
      return checkval(data(v)-model);
    }
} model_error;


// ---------------------------------------------------------------------------
// Here is the main program:

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
  // If a comment line, store in filecomments.
  // If a data line, store point in data

  do {
    if( ndata == 0 )
      filecomments.push_back(iv.line());
    else if ( ndata != 2 )
      error::fatal("modeliv: Invalid IV data line: ", iv.line().c_str());
    else
      data.add(iv.data()[0],iv.data()[1]);
    ndata = iv.parse();
  } while (*ps);

  // set up interpolators
  error::stream(ecomment);
  data.quiet().linear().build();
  if(!data.ready())
    error::fatal("modeliv: not enough IV data!");

  // set up error function and optimizer
  sweeper vsweep;
  vsweep.sweep(v, data);
  model_error.match(0.0);

  error_func ef;
  ef.add_term(1.0, model_error, vsweep);

  G_subgap  = ef.vary( 0,       0.1,  0.2    );
  Gap_width = ef.vary( 0,       0.1,   0.3    );
  I_leakage = ef.vary( 0,       0.01,  0.1    );
  I_defect  = ef.vary( 0,       0.1,  0.2    );

  Vgap      = ef.vary( Vgap/2,  Vgap,  2*Vgap );
  Rn        = ef.vary( Rn/10,   Rn,    Rn*10  );

  powell opt(ef);
  opt.quiet();

  // optimize the model and output the values
  opt.minimize();
  char* delim = "-----\n";
  state_display::command_line(comment,argc,argv,"modeliv.cc :");
  state_display::info(comment);
  comment << " Additional model parameters:\n"
	  << "  G_subgap : " << G_subgap << "\n"
	  << "  Gap_width: " << Gap_width << "\n"
	  << "  I_leakage: " << I_leakage << "\n"
	  << "  I_defect:  " << I_defect  << endl;
  comment << delim;

  // Now output the rest of the file comments saved previously
  for (unsigned i = 0 ; i < filecomments.size() ; ++i)
    cout << filecomments[i] << endl;

  // Adaptively fill an interpolator with simulated IV curve points.
  interpolation idc;
  adaptive_fill(idc, f, 0, 2, .0001, .01);


  // Now output the (V,I) points in the interpolator.
  comment << "\n";
  comment << " NORMALIZED " << "\t" << "NORMALIZED" << endl;
  comment << " V:         " << "\t" << "I:" << endl;
  cout << fixed << setprecision(10);
  for(unsigned i = 0; i < idc.size(); ++i)
    cout << idc.x(i) << "\t" << idc[i] << endl;

}
