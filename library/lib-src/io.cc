// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
//
// io.h

#include "io.h"
#include "error.h"
#include "units.h"
#include "nport.h"
#include <cctype>             // for isspace()
#include <cstring>            // for strchr()
#include <cstdlib>            // for strtod()

using namespace std;

// fuctions to skip over whitespace or nonwhitespace chars
// of a null-terminated string:
static void skipwhite(const char * & c)
{ while (*c && isspace(*c)) c++ ; return ; }
static void skipnonwhite(const char * & c)
{ while (*c && !isspace(*c)) c++ ; return ; }

// function to convert a string to uppercase:
static void up(std::string & s)
{ for(unsigned i = 0; i < s.length(); ++i) s[i] = toupper(s[i]); }


// ********************************************************************
// data_parser:

std::string data_parser::def_delim_ = "#!" ;

data_parser::data_parser() : delim_(def_delim_) { }
data_parser::data_parser(istream & f) : pin(&f), delim_(def_delim_) { }

unsigned data_parser::parse()
{
  string temp = "" ;
  if(!pin)
    error::warning("data_parser: no input stream assigned.");

  else if (!(*pin) && !(pin->eof()))
    error::warning("data_parser: some error occurred in input stream.");

  else if (*pin)
    getline(*pin, temp) ;

  // temp contains input line to parse, if any
  return parse(temp) ;
}

unsigned data_parser::skip_parse()
{
  unsigned n = 0;
  while ((*pin) && 0 == (n = parse())) ;
  return n ;
}

unsigned data_parser::parse(const std::string & inp)
{
  // clear out previous line results; copy in new line
  comment_ = "" ;
  data_.erase(data_.begin(),data_.end()) ;
  line_ = inp ;

  const char * c = line_.c_str();

  // here is the line parser for uncommented doubles:
  while (*c) {
    skipwhite(c);  // skip to next token

    // a comment char effectively terminates the line
    if ( strchr(delim_.c_str(), *c) ) {
      // *c is a comment delimiter
      comment_ = c ;  // comment_ gets the remainder of the line
      break ;
    }

    // try to convert this token to a double
    char * next ;
    double value = strtod(c, & next);
    if ( next == c ) {
      // then it's not a double, so skip it
      skipnonwhite(c);
      continue;
      }

    // found a double, contained in value
    data_.push_back(value);
    c = next; // skip over the parsed characters
  }

  return data_.size() ;
}

unsigned data_parser::parse(const char * const s)
{
  string temp = (!s) ? s : "" ;  // use a null string if a null pointer
  return parse(temp) ;
}

const std::vector<string> & data_parser::tokens()
{
  tokens_.erase(tokens_.begin(),tokens_.end()) ;

  // here is the line parser for more general tokens:
  const char * c = line_.c_str();  // beginning of line
  while (*c) {
    skipwhite(c);  // skip to next token
    const char * next = c;
    skipnonwhite(next);  // c and next bracket the next token, if any
    if(next == c) break; // no more tokens
    string tok(c, next-c);
    tokens_.push_back(tok);
    c = next;
  }

  return tokens_ ;
}

int data_parser::convert(real_vector & v, double u)
{
  int m = int(data_.size()) ;
  if (m <= 0) return 0 ;

  if (v.maxindex(m) < m) v.resize(m);
  for (int i = 0; i < m; ++i) v.get(i+1) = u * data_[unsigned(i)];
  return m;
}

int data_parser::convert(complex_vector & v, double u)
{
  bool pushed = false;
  if(data_.size() % 2 == 1) {
    data_.push_back(0.0);  // need an even number of values
    pushed = true;
  }

  int m = int(data_.size()/2) ;
  if (m <= 0) return 0 ;

  if (v.maxindex(m) < m) v.resize(m);
  for (int i = 0; i < m; ++i) 
    v.get(i+1) = u * dtoz(data_[2*unsigned(i)],data_[2*unsigned(i)+1]);

  if (pushed) data_.pop_back() ; // remove the added 0.0
  return m;
}

int data_parser::convert(double & x, real_vector & v, double u1, double u2)
{
  int m = int(data_.size()) - 1 ;
  if (m <= 0) return 0 ;

  x = u1 * data_[0];
  if (v.maxindex(m) < m) v.resize(m);
  for (int i = 1; i <= m; ++i) v.get(i) = u2 * data_[unsigned(i)];
  return m;
}

int data_parser::convert(double & x, complex_vector & v, double u1, double u2)
{
  int m = int(data_.size()) - 1 ;
  if (m <= 0) return 0 ;

  bool pushed = false;
  x = u1 * data_[0];
  if (m % 2 == 1) {
    data_.push_back(0.0);  // need an even number of values here
    ++m;
    pushed = true;
  }

  m /= 2;
  if (v.maxindex(m) < m) v.resize(m);
  for (int i = 1; i <= m; ++i)
    v.get(i) = u2 * dtoz(data_[2*unsigned(i)-1],data_[2*unsigned(i)]);

  if (pushed) data_.pop_back() ; // remove the added 0.0
  return m;
}

// ********************************************************************
// touchstone_read:

touchstone_read::touchstone_read()
  : N(0), verbose_(true), 
    f_units(GHz), z_(50*Ohm), type_(S), f_last(-1.0), mode(Complex::degree)
{
  d.delim() = "!" ;
}

bool touchstone_read::open(const char * const str, int ports, double f_scale)
{
  // reset private variables
  if(s.is_open()) s.close();
  N = 0; f_units = GHz; z_ = 50*Ohm;
  type_ = S; f_last = -1.0; mode = Complex::degree;
  end_ = false; noise_ = false;
  name_ = str;

  // use the provided frequency scaling, if ok
  if(f_scale > 0.0)
    f_units = f_scale;
  else
    if(verbose_)
      error::warning("touchstone_read::open(): improper frequency scaling argument. Using GHz.");

  // is the number of ports provided ok?
  if(ports <= 0) {
    if(verbose_)
      error::warning("touchstone_read::open(): must call with ports > 0");
    return false;
  }

  // can we open the file?
  s.open(str);
  if(!s) {
    if(verbose_)
      error::warning("touchstone_read::open(): could not open" , name_);
    return false;
  }

  // ok, let's look for some data or a specification line...
  d.input(s);
  unsigned n;
  std::vector<string> token;

  // parse lines until we find a number, a spec line, or reach the end:
  do {
    n = d.parse();
    token = d.tokens();
  } while(s && n == 0 && (token.size() == 0 || token[0] != "#"));

  if(!s) {
    // file has no data!
    if(verbose_)
      error::warning("touchstone_read::open(): found no data in file", name_ );
    end_ = true;
    return false;
  }

  // The line just parsed may be the "specification line" of the file. If present,
  // the line should be like: "# [GHZ/MHZ/KHZ/HZ] [S/Y/Z] [MA/DB/RI] [R n]".

  if(token[0] != "#") {
    // it's not a specification line, so we'll use default values already set
    N = ports;
    return true;
  }

  // Found a specification line with at least one entry. Try to parse it...
  unsigned i = 1;

  if(i >= token.size()) goto cleanup; // no more entries
  up(token[i]);

  // first is the frequency unit, if present
  if(token[i] == "GHZ") {
    f_units = GHz;
    ++i;
  }
  else if(token[i] == "MHZ") {
    f_units = MHz;
    ++i;
  }
  else if(token[i] == "KHZ") {
    f_units = Kilo*Hertz;
    ++i;
  }
  else if(token[i] == "HZ") {
    f_units = Hertz;
    ++i;
  }

  if(i >= token.size()) goto cleanup; // no more entries
  up(token[i]);

  // next is the type of the data matrix
  if(token[i] == "S") {
    type_ = S;
    ++i;
  }
  else if(token[i] == "Y") {
    type_ = Y;
    ++i;
  }
  else if(token[i] == "Z") {
    type_ = Z;
    ++i;
  }

  if(i >= token.size()) goto cleanup; // no more entries
  up(token[i]);

  // next is the complex number format
  if(token[i] == "MA") {
    mode = complex::degree;
    ++i;
  }
  else if(token[i] == "DB") {
    mode = complex::db;
    ++i;
  }
  else if(token[i] == "RI") {
    mode = complex::cartesian;
    ++i;
  }

  if(i >= token.size()) goto cleanup; // no more entries
  up(token[i]);

  // finally the normalizing impedance
  if(token[i] == "R" && n == 1) {
    z_ = d.data()[0]*Ohm;
    if(z_ > 0.0) 
      i += 2;  // skip over impedance entry only if it is proper
    else 
      z_ = 50*Ohm;
  }

  if(i < token.size()) {
    // there are some unrecognized entries still in spec line
    if(verbose_)
      error::warning("touchstone_read::open(): spec line has unrecognized syntax in", name_);
    return false;
  }

 cleanup:
  N = ports;
  d.skip_parse();  // skip to the data lines
  return true;
}


bool touchstone_read::value(double & freq, Matrix & M)
{
  // On entry, d will already have parsed the next data line using d.skip_parse()

  // ready to rumble?
  if((N == 0) || end_) return false;
  if(!s && !s.eof()) {
    // something very wrong
    if(verbose_)
      error::warning("touchstone_read::value(): something failed while reading", name_);
    N = 0; s.close(); return false;
  }
  if(!s || (d.data().size() == 0)) {
    end_ = true; s.close(); return false;
  }

  // Have we reached "noise data"?
  if(d.data()[0]*f_units <= f_last) {
    end_ = true; noise_ = (N==2); return false;
  }

  // We are ready to go...
  double f;                // the frequency read from file
  Vector L;                // the matrix data from a single line of file
  Matrix T(N, Index_1);    // temporary matrix constructed from file data

  old_mode = Complex::in_mode(mode);  // set the complex format, saving previous

  switch (N) {
  case 1:
  case 2: {
    // in these cases, all data is on one line
    if((d.convert(f,L,f_units) != N*N) || (f < 0.0)) goto bad; // something's wrong
    int k = 1;
    for(int i = 1; i <= N; ++i) for(int j = 1; j <= N; ++j) T[j][i] = L[k++];
    break;
  }

  case 3:
  case 4: {
    // N == 3 or N == 4, data on multiple lines, one row per line
    if((d.convert(f,L,f_units) != N) || (f < 0.0)) goto bad; // something's wrong
    // first row
    for(int j = 1; j <= N; ++j) T[1][j] = L[j];
    // other rows
    for(int i = 2; i <= N; ++i) {
      d.skip_parse();
      if(d.convert(L) != N) goto bad;
      for(int j = 1; j <= N; ++j) T[i][j] = L[j];
    }
    break;
  }

  default: {
    // N > 4, data on multiple lines, but not sure how many points/line

    // Interpret this first line, saving the count of the S entries
    int P = d.convert(f,L,f_units);
    if((P < 4) || (P > N) || (f < 0.0)) goto bad; // something's wrong

    // O.K., now we have the first P values of the first row of S
    int Pmax = P;      // max entries per subsequent line

    // Build T by reading in the rows
    for (int R = 1; R <= N; ++R) {
      // At the start of each iteration, the next input line is already parsed
      // Copy entries from this and subsequent lines into T to complete a row
      for (int C = 0; C < N; ) {
	// Note that we start with C = 0, not C = 1
	// Copy the parsed line values into T:
	for(int j = 1; j <= P; ++j) T[R][++C] = L[j];
	if(C == N) break;  // finished the row

	// The row's not finished. Read the next line:
	d.skip_parse();
	P = d.convert(L);
	// Check for an unexpected number of entries:
	if((P == 0) || (P > Pmax) || (C+P > N)) goto bad;
      }

      // Finished this row. Is it the last?
      if(R == N) break;

      // More rows to go... Get the next line
      d.skip_parse();
      P = d.convert(L);
      // Check for an unexpected number of entries:
      if(P == 0 || P > Pmax) goto bad;
    }
    break;
  }
  } // switch

  // normal exit, everything ok
  d.skip_parse();
  Complex::in_mode(old_mode);
  freq = f_last = f;
  M = normalize(T);
  return true;

  // abnormal exit, improper line format
 bad:
  if(verbose_) {
    error::warning("touchstone_read::value(): unexpected number of entries"
		   " in a line of file", name_);
    error::stream() << "Number of Ports expected: " << N << endl
		    << "Data line:" << endl << d.line() << endl;
  }
  Complex::in_mode(old_mode);
  s.close();
  N = 0;
  return false;
}


bool touchstone_read::Svalue(double & freq, Matrix & M)
{
  // if file contains S data, value() is all we need.
  if(type_ == S) return value(freq,M);

  // get freq and M, if file is still belching data; if not, return false
  if (!value(freq,M)) return false;

  // freq is correct, but M is a Z or Y instead of S; we must fix
  Matrix i = identity_matrix(N);
  i *= (type_ == Z) ? double(device::Z0) : 1.0/device::Z0 ;
  M = solve((i + M), (type_ == Z) ? (M - i) : (i - M));
  return true;
}


bool touchstone_read::Yvalue(double & freq, Matrix & M)
{
  // if file contains Y data, value() is all we need.
  if(type_ == Y) return value(freq,M);

  // get freq and M, if file is still belching data; if not, return false
  if (!value(freq,M)) return false;

  // freq is correct, but M is a Z or S instead of Y; we must fix
  Matrix i = identity_matrix(N);
  if (type_ == Z)
    M = solve(M, i);  // just invert for Z -> Y
  else
    M = (1/device::Z0) * solve((i + M), (i - M));  // type_ is S
  return true;
}


bool touchstone_read::Zvalue(double & freq, Matrix & M)
{
  // if file contains Z data, value() is all we need.
  if(type_ == Z) return value(freq,M);

  // get freq and M, if file is still belching data; if not, return false
  if (!value(freq,M)) return false;

  // freq is correct, but M is a Y or S instead of Z; we must fix
  Matrix i = identity_matrix(N);
  if (type_ == Y)
    M = solve(M, i);  // just invert for Y -> Z
  else
    M = device::Z0 * solve((i - M), (i + M));  // type_ is S
  return true;
}  


Matrix touchstone_read::normalize(const Matrix & T)
{
  switch(type_) {

  case Z:
    return z_ * T ; break ;

  case Y:
    return (1/z_) * T ; break ;

  default:
  case S:
    if(z_ == device::Z0)
      return T;
    else {
      // assumes T has size N with indexing Index_1
      double r = (z_ - device::Z0)/(z_ + device::Z0);
      Matrix i = identity_matrix(N);
      return solve((i + r*T), (r*i + T));
    }
    break ;

  }
}


bool touchstone_read::get_noise(double & f, noise & ND)
{
  // On entry, d will already have parsed the next data line using d.skip_parse()
  if(!has_noise()) return false;
  if(!s && !s.eof()) {
    // something very wrong
    if(verbose_)
      error::warning("touchstone_read::get_noise(): something failed while reading", name_);
    N = 0; s.close(); return false;
  }
  if(!s || (d.data().size() == 0)) {
    noise_ = false; s.close(); return false;
  }

  // Is format of noise data correct?
  if(d.data().size() != 5) {
    if(verbose_)
      error::warning("touchstone_read::get_noise(): couldn't interpret noise data in", name_);
    N = 0; s.close(); return false;
  }

  // ok, fill in the data
  f = d.data()[0]*f_units ;
  ND.Fmin = d.data()[1] ;
  ND.Reff = d.data()[4]*z_ ;

  // when we read Gopt, we must renormalize it to device::Z0
  old_mode = Complex::in_degree();
  ND.Gopt = dtoz(d.data()[2],d.data()[3]);
  double r = (z_ - device::Z0)/(z_ + device::Z0);
  ND.Gopt = (r + ND.Gopt)/(1 + r*ND.Gopt);
  Complex::in_mode(old_mode);

  // read the next line and exit
  d.skip_parse();
  return true;
}


int touchstone_read::ports(const char * const name)
{
  touchstone_read f;

  // First check for more than 4 ports, since these may look like 4-ports
  // to a naive check:
  if (!f.open(name, 5)) // we don't actually expect 5 ports
    // Couldn't open or interpret the spec line; a warning will be issued.
    // Exit function returning 0.
    return 0;

  // Now the data parser in f has moved to the first line of data
  double freq;
  Vector L;
  int max = int(f.d.data().size());  // the number of doubles on this line.

  // Count the number of elements; stop if we get to the next freq point
  int N_sq = f.d.convert(freq,L);
  while (true) {
    int n = f.d.skip_parse();
    if(n == 0 || n >= max) break;    // finished this frequency
    N_sq += f.d.convert(L);          // add the elements to the count
  }

  // Now N_sq has the square of the number of ports.
  if (N_sq >= 5*5) {
    // we know we have to have at least a 5-port
    int N = int(sqrt(double(N_sq)) + .5);  // round the sqrt to nearest int
    if (N*N == N_sq)
      return N;  // success!
    else {
      error::warning("touchstone_read::ports(): number of ports could not be "
		     "determined in file", name);
      return 0;
    }
  }
  
  // Now we test for 4 or fewer ports
  for(int i = 4; i > 0; --i) {

    // attempt to open file with <i> ports
    if (!f.open(name, i))
      // couldn't open or interpret the spec line; a warning will be issued
      // exit function returning 0
      return 0;

    // issue no warnings for the next check, since we don't know the number of ports
    f.quiet();
    double freq;
    Matrix M;
    if (f.value(freq, M))
      // successfully read the data for an <i>-port; exit function returning i
      return i;
    f.verbose();
  }

  // hmm... if we get here, then we don't have a clue.
  error::warning("touchstone_read::ports(): number of ports could not be "
		 "determined in file", name);
  return 0;
}
