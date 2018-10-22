// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
//
// SIScmplx.cc

#include "SIScmplx.h"
#include "global.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cmath>

using namespace std;

//---------------------------------------------------------------------
// i/o formatting static variables

static Complex::io_mode out_mode_ , in_mode_ ;

static int in_form_ ;

static string out_prefix_ , out_suffix_ , out_separator_ ;

double Complex::rel_tolerance = 0.0;
double Complex::abs_tolerance = 0.0;


//---------------------------------------------------------------------
// set output formatting

Complex::io_mode Complex::out_mode()
{ return out_mode_; } 

Complex::io_mode Complex::out_mode(Complex::io_mode m)
{ io_mode temp = out_mode_; out_mode_ = m; return temp; } 

string Complex::out_prefix()
{ return out_prefix_; }

string Complex::out_prefix(const string & s)
{ string temp = out_prefix_; out_prefix_ = s; return temp; }

string Complex::out_prefix(const char * const s)
{ string temp = out_prefix_; out_prefix_ = s; return temp; }

string Complex::out_prefix(const char s)
{ string temp = out_prefix_; out_prefix_ = s; return temp; }

string Complex::out_separator()
{ return out_separator_; }

string Complex::out_separator(const string & s)
{ string temp = out_separator_; out_separator_ = s; return temp; }

string Complex::out_separator(const char * const s)
{ string temp = out_separator_; out_separator_ = s; return temp; }

string Complex::out_separator(const char s)
{ string temp = out_separator_; out_separator_ = s; return temp; }

string Complex::out_suffix()
{ return out_suffix_; }

string Complex::out_suffix(const string & s)
{ string temp = out_suffix_; out_suffix_ = s; return temp; }

string Complex::out_suffix(const char * const s)
{ string temp = out_suffix_; out_suffix_ = s; return temp; }

string Complex::out_suffix(const char s)
{ string temp = out_suffix_; out_suffix_ = s; return temp; }

void Complex::out_default()
{ out_prefix_ = out_separator_ = out_suffix_ = ""; }

void Complex::out_space()
{ out_prefix_ = out_suffix_ = ""; out_separator_ = " "; }

void Complex::out_delimited()
{ out_prefix_ = "("; out_separator_ = ","; out_suffix_ = ")"; }


//---------------------------------------------------------------------
// set input formatting

Complex::io_mode Complex::in_mode()
{ return in_mode_; } 

Complex::io_mode Complex::in_mode(Complex::io_mode m)
{ io_mode temp = in_mode_; in_mode_ = m; return temp; } 

int Complex::in_form()
{ return in_form_; }

int Complex::in_form(int f)
{ int temp = in_form_; in_form_ = (f != 0); return temp; }


//---------------------------------------------------------------------
// i/o functions

ostream & operator <<(ostream & out_file, Complex z)
{
  ostringstream zout;
  zout.setf(out_file.flags());
  zout.precision(out_file.precision());
  zout.fill(out_file.fill());

  string separator(out_separator_);

  // Here's where we check for rounding to zero:
  double ar = abs(z.real), ai = abs(z.imaginary);
  if( (ai < abs(Complex::abs_tolerance)) ||
      (ai < ar*abs(Complex::rel_tolerance)) )
    z.imaginary = 0.0;

  if( (ar < abs(Complex::abs_tolerance)) ||
      (ar < ai*abs(Complex::rel_tolerance)) )
    z.real = 0.0;

  // Get rid of "-0" outputs
  if (z.imaginary == 0.0) z.imaginary = 0.0;

  // Now format the number for output
  switch ( out_mode_) {

  default:
  case Complex::cartesian: {
    if (separator == "") {
      if (z.imaginary < 0.0) {
	separator = "-i";
	z.imaginary = -z.imaginary;
      }
      else
	separator = "+i";
    }

    zout << out_prefix_ << z.real << separator
	 << z.imaginary << out_suffix_;
    break;
  }

  case Complex::polar: {
    if (separator == "") separator = " ";

    zout << out_prefix_ << abs(z) << separator
	 << arg(z) << out_suffix_;
    break;
  }

  case Complex::degree: {
    if (separator == "") separator = " ";

    zout << out_prefix_ << abs(z) << separator
	 << arg(z)/Degree << out_suffix_;
    break;
  }

  case Complex::db: {
    if (separator == "") separator = " ";

    zout << out_prefix_ << 20.0 * std::log10(abs(z)) << separator
	 << arg(z)/Degree << out_suffix_;
    break;
  }

  } // switch


  return out_file << zout.str();
}


istream & operator >>(istream & in_file, Complex & z)
{
  double re = 0, im = 0;

  if (in_form_ == 0) {  // the default behavior
    in_file >> re >> im;
  }
  else {  // delimited input form
    char c = 0;
    in_file >> c;
    if (c == '(') {
      in_file >> re >> c;
      if (c == ',')
	in_file >> im >> c;
      if (c != ')')
	in_file.clear(ios::badbit);  // invalid - set state
    }
    else {
      in_file.putback(c);
      in_file >> re;
    }
  }

  if (in_file) z = dtoz(re,im);  // everything went well
  return in_file;
}

Complex dtoz(double re, double im)
{
  Complex z;

  switch (in_mode_) {

  default:
  case Complex::cartesian: {
    z = Complex(re,im);
    break;
  }

  case Complex::db: {
    re = std::pow(10.0, re/20.0); // convert decibel magnitude
    // fall through to case Complex::degree
  }

  case Complex::degree: {
    im *= Degree;  // convert phase to radians
    // fall through to case Complex::polar
  }

  case Complex::polar: {
    if (im == 0.0)
      z = re;
    else
      z = polar(re,im);
    break;
  }

  } // switch

  return z;
}


//---------------------------------------------------------------------
// functions which use <cmath>:

// absolute value (magnitude):

double zabs( const Complex & z )
{
  return std::sqrt( z.real*z.real + z.imaginary*z.imaginary );
}

double zarg( const Complex & z )
{
  return std::atan2( z.imaginary, z.real );
}

Complex log( const Complex & z )
{
  return Complex (std::log(zabs(z)), zarg(z));
}

Complex exp( const Complex & z )
{
  return polar(std::exp(z.real), z.imaginary);
}

Complex log10( const Complex & z )
{
  return log(z)/std::log(10.0);
}

Complex pow( const Complex & z, const double d )
{
  return polar(std::pow(zabs(z),d), d * zarg(z));
}

Complex pow( Complex z1, const Complex & z2 )
{
  z1 = Complex( std::log(zabs(z1)), zarg(z1) ); // use z1 as temp variable
  return polar( std::exp(z1.real * z2.real - z1.imaginary * z2.imaginary),
		z1.real * z2.imaginary + z1.imaginary * z2.real );
}

Complex cos( const Complex & z )
{
  return Complex( std::cos(z.real)*std::cosh(z.imaginary), 
		  -std::sin(z.real)*std::sinh(z.imaginary) );
}

Complex sin( const Complex & z )
{
  return Complex( std::sin(z.real)*std::cosh(z.imaginary),
		  std::cos(z.real)*std::sinh(z.imaginary) );
}

Complex tan( Complex z )
{
  z *= 2;  // use z as a temp variable
  return Complex( std::sin(z.real), std::sinh(z.imaginary) )/
    ( std::cos(z.real) + std::cosh(z.imaginary) );
}

Complex cosh( const Complex & z )
{
  return Complex( std::cosh(z.real)*std::cos(z.imaginary), 
		  +std::sinh(z.real)*std::sin(z.imaginary) );
}

Complex sinh( const Complex & z )
{
  return Complex( std::sinh(z.real)*std::cos(z.imaginary),
		  std::cosh(z.real)*std::sin(z.imaginary) );
}

Complex tanh( Complex z )
{
  z *= 2;  // use z as a temp variable
  return Complex( std::sinh(z.real), std::sin(z.imaginary) )/
    ( std::cosh(z.real) + std::cos(z.imaginary) );
}

Complex asin( const Complex & z )
{
  return -I*log(I*z+sqrt(1-z*z));
}

Complex acos( const Complex & z )
{
  return -I*log(z+I*sqrt(1-z*z));
}

Complex atan( Complex z )
{
  z *= I;
  return log((1+z)/(1-z))/(2*I);
}

Complex asinh( const Complex & z )
{
  return log(z+sqrt(z*z+1));
}

Complex acosh( const Complex & z )
{
  return log(z+sqrt(z*z-1));
}

Complex atanh( Complex z )
{
  return 0.5*log((1+z)/(1-z));
}
