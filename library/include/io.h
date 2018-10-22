// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ********************************************************************
// io.h
// classes and functions to aid in reading and writing formatted data
//
// F.R. 9/27/99
// ********************************************************************

#ifndef IO_H
#define IO_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "matmath.h"
#include "units.h"

// ********************************************************************
class data_parser
{
public:

  // Constructor can take a reference to the input stream.
  data_parser();
  data_parser(std::istream &);

  // Set or change the input stream.
  data_parser & input(std::istream & i) { pin = &i ; return *this ; }

  // Parse the next line of data; return the number of doubles found.
  // parse() has a voracious appetite for doubles but complete indifference
  // to anything else on a line. To qualify as a double, a token must look
  // like what you would think of as a double. It must be separated from
  // any previous token by some white space, but need not be separated from
  // any following token by white space. Consequently the string "-2Volts"
  // contains the double -2.0, while "Perfect-10.0" contains no doubles, since
  // "-10.0" is not preceded by white space. A comment character on the line
  // causes parse to stop looking for doubles following it. 
  unsigned parse();

  // Like parse, except first skip any lines without uncommented numbers;
  // returns 0 if end of file or error encountered.
  unsigned skip_parse();

  // Parse the string supplied as an argument as though it were input.
  unsigned parse(const std::string &);
  unsigned parse(const char * const);


  // A STL vector<double> containing the uncommented doubles found by
  // the latest call to parse().
  const std::vector<double> & data() { return data_ ; }

  // The latest input line read by parse(), parsed into a STL vector of C++
  // string tokens which were found separated by one or more whitespace char's
  // in the line. Whitespace chars are those identified as such by the C macro
  // isspace().
  const std::vector<std::string> & tokens();


  // Convert the data in data() into the supplied container.
  // Returns number of elements stored to the container (0 if an error).
  // Uses Complex::dtoz() to convert doubles to Complex. Optionally scales
  // converted values by multiplying by "units" following conversion.

  int convert(real_vector &, double units = 1.0);    // Starts with index = 1 in the
  int convert(complex_vector &, double units = 1.0); // receiving containers.


  // Convert the data in data() into the supplied double and container;
  // returns number of elements stored to the container (0 if an error).
  // Uses Complex::dtoz() to convert doubles to Complex. data()[0] (the first value
  // found on the line) goes into the double, the rest into the container. Optionally
  // scales converted values by multiplying by "u1" for the double, "u2" for the
  // values in the container following conversion. There must be at least two
  // doubles in data() for a conversion to be performed.

  int convert(double &, real_vector &, double u1 = 1.0, double u2 = 1.0);
  int convert(double &, complex_vector &, double u1 = 1.0, double u2 = 1.0);


  // String containing the entire line text found by parse().
  const std::string & line() { return line_ ; }

  // String containing the comment text found by parse(). The comment delimiter
  // is the first character of the string.
  const std::string & comment() { return comment_ ; }


  // String containing the comment delimiter chars recognized by parse().
  // Since the reference returned is not const, you can change this string,
  // changing the subsequent behavior of parse().
  std::string & delim() { return delim_ ; }

  // String constaining the default comment delimiter chars, '#' and '!'.
  // Each newly created data_parser object will use this string to initialize
  // delim(). You can change this string.
  static std::string & default_delim() { return def_delim_ ; }


private:
  std::istream * pin ;             // pointer to the input stream
  std::string line_ ;              // hold the line being parsed
  std::string comment_ ;           // hold the comment found in current line
  std::string delim_ ;             // hold a list of comment delimiter chars
  std::vector<std::string> tokens_ ; // hold the tokens found on the line
  std::vector<double> data_ ;      // hold the numbers found on the line
  static std::string def_delim_ ;  // hold the default comment delimiters

};


// ********************************************************************
// class touchstone_base
//
// Base class containing common elements of the touchstone file classes
// touchstone_read and touchstone_write

class touchstone_base
{
public:

  // the recognized matrix types
  enum types { S, Y, Z };

};


// ********************************************************************
// class touchstone_read
//
// This class can input frequency response data from most properly-formatted
// Touchstone files, and many "improperly" formatted ones.. It understands
// S, Z, and Y type matrix data in any of MA, DB, or RI number formats.
//
// It can interpret files which are minor generalizations of the strict
// Touchstone format: it ignores lines containing no doubles (as interpreted by
// data_parser::parse(), see above), even if they do not use a comment delimiter
// ('!'), and can handle numbers of ports exceeding 4 by looking for a
// generalization of the Touchstone format of 4-port data. The genereralized
// data format for an N-port (with N > 4) may be either of the following 2
// formats:
//
// Format 1 (all S parameters for a row on a single line):
//
//   freq1  S(1,1)  S(1,2)  S(1,3)  S(1,4)  ... S(1,N)
//          S(2,1)  S(2,2)  S(2,3)  S(2,4)  ... S(2,N)
//          S(3,1)  S(3,2)  S(3,3)  S(3,4)  ... S(3,N)
//           ...
//          S(N,1)  S(N,2)  S(N,3)  S(N,4)  ... S(N,N)
//
//   freq2  S(1,1)  S(1,2)  S(1,3)  S(1,4)  ... S(1,N)
//          S(2,1)  S(2,2)  S(2,3)  S(2,4)  ... S(2,N)
//           ...
//
// Format 2 (at least 4 S parameters per line; a single row's data wraps): 
//
//   freq1  S(1,1)  S(1,2)  S(1,3)  S(1,4)
//          S(1,5)  S(1,6)  S(1,7)  S(1,8)
//           ...  S(1,N-1)  S(1,N)
//          S(2,1)  S(2,2)  S(2,3)  S(2,4)
//          S(2,5)  S(2,6)  S(2,7)  S(2,8)
//           ...  S(2,N-1)  S(2,N)
//           ...
//          S(N,1)  S(N,2)  S(N,3)  S(N,4)
//          S(N,5)  S(N,6)  S(N,7)  S(N,8)
//           ...
//           ...  S(N,N-1)  S(N,N)
//
//   freq2  S(1,1)  S(1,2)  S(1,3)  S(1,4)
//          S(1,5)  S(1,6)  S(1,7)  S(1,8)
//           ...  S(1,N-1)  S(1,N)
//           ...
//
// Each freq value is a single number; the S matrix values are each a pair
// of numbers. Format 2 is output by programs such as HFSS and HPADS.

class touchstone_read : public touchstone_base
{
public:

  // -------------------
  // Create object and open a file:

  touchstone_read();  // the only constructor is the default.

  // Open the named file. Look for and interpret a specification line, if present.
  // Any previously-opened file is first closed. Returns false if unable to open or
  // can't find any numerical data in the file. f_scale gives the default scaling
  // for the file's frequency data; it will be overridden by the file's specification
  // line frequency scaling, if present. In the event a specification line is not
  // present (specification lines begin with "# "), uses the default format which would
  // have a specification line of:   "# <f_scale> S MA R 50" (f_scale frequency scale,
  // S parameter in Magnitude-Degree, 50 Ohm normalization). Note that white space must
  // separate the '#' and the rest of the specification line.
  bool open(const char * const name, int ports = 2, double f_scale = GHz);


  // -------------------
  // Get the matrix data:

  // Returns the type of matrix data in the file. Returns S if something went wrong.
  typedef touchstone_base::types types;
  types type() const { return type_ ; }

  // Fills in the frequency and Matrix data from the next data line of the file.
  // Returns false when no more Matrix data could be read from file, or data improperly
  // formatted, or no file has been specified with open(); f and M will not be
  // modified in this case. The data returned in the Matrix argument to value()
  // will be properly converted to supermix internal values using this impedance. In
  // the case of types Y and Z, the matrix elements are individually scaled using z()
  // to accomplish this conversion. In the case of type S, the matrix data is
  // renormalized to the normalizing impedance in the current value of device::Z0. 
  bool value(double & f, Matrix & M);

  // Svalue() is like value, except that if the file contains Y or Z data, it converts
  // the data to the equivalent S matrix normalized to device::Z0.
  bool Svalue(double & f, Matrix & S);

  // Yvalue() is like Svalue, except that it returns the equivalent Y matrix.
  bool Yvalue(double & f, Matrix & Y);

  // Zvalue() is like Svalue, except that it returns the equivalent Z matrix.
  bool Zvalue(double & f, Matrix & Z);


  // -------------------
  // Get the noise data:

  // Container for noise data read from a file
  struct noise {
    double  Fmin;   // min noise figure in dB
    complex Gopt;   // optimum source reflection coefficient (Gamma)
    double  Reff;   // effective noise resistance
  };

  // Returns true if noise data is detected in the file.
  bool has_noise() const { return (N == 2) && noise_ ; }

  // If the next lines in the file contain noise data (file must be for a 2-port),
  // fill in the members of N with that data. f will get the frequency of the data.
  // If there is no properly formatted noise data remaining, returns false. f and N will
  // not be modified in this case. N.Gopt will contain the source Gamma properly
  // renormalized to device::Z0; N.Reff will be a real (unnormalized) resistance.
  bool get_noise(double & f, noise & N);
  

  // -------------------
  // Other functions:

  // Returns false if data format was improper on latest call to open() or value().
  // If a call to value() returns false, but good() returns true, then the end of
  // the matrix data values has been reached. Returns false if open() has not been called.
  bool good() const { return (N != 0); }

  // Returns the normalizing impedance of the data in the file. The data
  // returned in the Matrix argument to value() will be properly converted to
  // supermix internal values using this impedance. In the case of types Y and Z,
  // the matrix elements are individually scaled using z() to accomplish this
  // conversion. In the case of type S, the matrix data is renormalized to the
  // normalizing impedance in the current value of device::Z0.
  double z() const { return z_ ; }

  // Verbosity. The default is for verbose behavior to be enabled. If verbose, then
  // warnings will be issued for problems encountered. If quiet, then no warnings will
  // be issued, so the return values of functions will give the only indications of
  // bad behavior.
  touchstone_read & verbose() { verbose_ = true;  return *this; }
  touchstone_read & quiet()   { verbose_ = false; return *this; }
  
  // -------------------
  // A static (global) function to determine the number of ports in a Touchstone file

  // Examine the named file. If it is a properly-formatted Touchstone file, return the
  // number of ports for the device described by the file. Return 0 if the file is not
  // a proper Touchstone file. This function only checks the specification line format
  // and examines the first data set in the file, so its checks are not at all
  // comprehensive. It should correctly interpret an "extended" Touchstone format for
  // files with more than 4 ports.
  static int ports(const char * const name);

private:

  touchstone_read(const touchstone_read &);  // no copy constructor!
  void operator = (const touchstone_read &); // no assignment operator!

  std::string name_; // the name of the file opened
  std::ifstream s;   // the stream for this file
  data_parser d;     // the data parser object used to parse the file
  int N;             // number of ports in the data (0 if something went wrong)
  bool end_;         // true when the end of the matrix data has been seen.
  bool noise_;       // true when noise data is available
  bool verbose_;     // warning behavior

  double f_units;    // units for file frequency data
  double z_;         // normalizing impedance of file data
  types type_;       // the type of matrix in the file
  double f_last;     // the latest frequency seen

  Matrix normalize(const Matrix &);  // use type_ and z_ to normalize data

  Complex::io_mode mode;     // the input mode (format) of the S data
  Complex::io_mode old_mode; // saves the io_mode at entry to value()

};

#endif /* IO_H */
