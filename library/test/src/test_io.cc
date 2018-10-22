// test io.h functions

#include "io.h"
#include <bits/stl_pair.h>

using namespace std;

double u1 = 2, u2 = .5;

int main(int argc, char **argv) 
{
  data_parser d;
  istream * pin;
  ifstream infile;
  
  if (argc < 2) {
    d.input(cin);
    pin = & cin;
  }
  else {
    infile.open(argv[1]);
    d.input(infile);
    pin = & infile;
  }

  cout << "Data file " << ((argc < 2)? "std input" : argv[1]) << endl << endl;

  unsigned j = 0;
  while(*pin && !(pin->eof())) {
    unsigned n = d.parse();
    cout << "Line " << ++j << ":" << d.line() << endl;
    cout << " Number of values: " << n ;
    cout << ", comment string: " << d.comment() << endl;
    cout << " Tokens";
    for(unsigned i = 0; i < d.tokens().size(); ++i) cout << " : " << d.tokens()[i];
    cout << endl;
    for(unsigned i = 0; i < n; ++i) cout << "  " << d.data()[i];
    cout << endl;
    cout << "Convert to a double, Vector pair; scale factors: "
	 << u1 << ", " << u2 << "  :" << endl;
    pair<double,Vector> p;
    n = unsigned(d.convert(p.first, p.second, u1, u2));
    if(n) 
      cout << p.first << " : " << p.second << endl;
    else
      cout << "  Conversion not performed." << endl;
  }
}

