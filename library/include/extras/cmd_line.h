// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ********************************************************************
// cmd_line.h
// classes to provide a primitive but useful way to parse the command
// line for arguments, and/or generate a usage prompt.
//
// includes classes:
//   command_line_parser (abstract class)
//   command_line_parameter
//   command_line_range
//   command_line_int
//
// and functions:
//   get_command_line(argc, argv)
//   command_line_parameters_required()
//
// F. Rice 2/28/04
// ********************************************************************
//                The command_line_parser Abstract Class
//
// SuperMix users often will write programs that perform various
// simulations or optimizations of a generic circuit design which
// requires the specification of several circuit parameters before the
// design is fully defined. One way to provide the values of these
// parameters is to include them on the command line used to invoke
// the program. The code required to fetch and interpret the command
// line arguments is tedious to write and include in each program,
// especially if you wish to include parameter validity checks and to
// provide a usage prompt to the user describing in detail the various
// command line arguments required.
//
// The command_line_parser abstract class and the concrete classes
// derived from it included in this header file provide a primitive
// but straightforward way to implement command line argument parsing
// which is powerful enough to handle many situations. It also
// provides for the automatic generation of a usage prompt (ie, a
// terse "help" facility). The concrete classes included here also are
// derived from class state_display (see extras/state_display.h), so
// they provide for inclusion of the argument values in a header block
// in your simulation output, so you data will include an unambiguoius
// record of the values used during a particular program run.
//
//
// USAGE:
//
// The command_line_parser abstract class maintains a list of all
// objects derived from it so that it can properly parse command line
// arguments and generate usage information. It declares several
// member functions which provide the interface to objects derived
// from it. See the concrete classes declared in this header file for
// examples of how to implement a concrete instance of the class. What
// follows is a description of the member functions defining the
// interface to objects of the class.
//
// 




#ifndef CMD_LINE_H
#define CMD_LINE_H

#include "extras/state_display.h"
#include "parameter.h"
#include "error.h"
#include <cstdlib>

// =======================================================================
// class cmd_line_parser
// an abstract class which supports command line parsing and help

class command_line_parser {
  public:
  // ---------------------------------------------------------------------
  // Define the following functions in derived classes:

  // The MINIMUM number of command line parameters which will be needed.
  virtual int params() = 0;

  // Parse the command line arguments needed, starting with argv[i+1]:
  // on exit, i should be the index of the last parsed argument.
  // Returns true if parsing was successful. Returns false if an
  // argument is not what was expected; in this case i holds the index
  // of the offending argument. Also returns false if there aren't
  // enough command line arguments, in which case i == argc.
  virtual bool parse(int & i, int argc, const char ** argv) = 0;

  // Output a string containing a space-separated list of command line
  // parameters, beginning with a space, eg: " <arg1> <arg2>". Do not
  // include any line terminators or comment delimiters
  virtual void help_1(std::ostream &) = 0;

  // Output a number of lines which explain the command line parameters
  // used, including units, limits, etc, eg:
  //   << " Lengths in Microns." << endl;
  virtual void help_2(std::ostream &) = 0;


  // ---------------------------------------------------------------------
  // These static functions manage the command line arguments

  // the container of the objects requiring command line arguments
  typedef std::list<command_line_parser *> objects;
  typedef objects::iterator pobj;

protected:
  static objects cmd_list;

  // this iterator is stepped through cmd_list during argument parsing
  static pobj cmd_list_pos;

public:
  pobj my_cmd_list_pos;  // so each object knows where it is in cmd_list

  // This function returns the minimum total number of command
  // line parameters required (doesn't count argv[0])
  static int total_params();

  // This function prints a short usage prompt to error::stream(),
  // using only the help_1() strings:
  static void short_usage(const char * prog_name, const char * prompt = 0);

  // This function prints a consolidated usage prompt to error::stream():
  static void usage(const char * prog_name, const char * prompt = 0);

  // This function parses the command line for the correct number of
  // entries, using the information from all objects derived from this
  // class. If the command line is not formatted correctly, prints a
  // usage message to error::stream() and calls exit(1).  prompt is an
  // optional program description prompt printed along with the usage
  // message. It uses the static iterator cmd_list_pos so that derived
  // objects may adjust it as necessary.
  static void get_commands(int argc, const char ** argv,
			   const char * prompt = 0);

  // The constructor maintains the list of pointers to objects needing
  // command line parameters:
  command_line_parser(pobj position = cmd_list.end())
  { my_cmd_list_pos = cmd_list.insert(position, this); }

  // The copy constructor inserts the object just before the source
  command_line_parser(const command_line_parser & s)
  { 
    my_cmd_list_pos = 
      cmd_list.insert(s.my_cmd_list_pos, this);
  }

private:
  // hide operator =
  command_line_parser & operator = (const command_line_parser &);

public:
  virtual ~command_line_parser()
  { cmd_list.erase(my_cmd_list_pos); }

};  // class command_line_parser


// -----------------------------------------------------------------------
// Global functions which access command_line_parser:

inline void get_command_line(int argc, const char **argv, const char *prompt = 0)
{ command_line_parser::get_commands(argc,argv,prompt); }

inline void get_command_line(int argc, char **argv, const char *prompt = 0)
{ command_line_parser::get_commands(argc,(const char **)(argv),prompt); }

inline int command_line_parameters_required()
{ return command_line_parser::total_params(); }


// =======================================================================
// class command_line_parameter : read a double from the command line and allow
// access to its value as an abstract_real_parameter. Includes associated help
// functions.

struct command_line_parameter
  : public command_line_parser, public state_display, public real_parameter {

  double    scale;         // the units scaling factor
  const char *prompt_1, *prompt_2;

  // constructor: prompt_string_1 is printed inside angle brackets
  // in the help_1() prompt string; prompt_string_2 is the explanation
  // string printed by help_2().

  command_line_parameter(double units, 
	       const char * prompt_string_1, const char * prompt_string_2 = 0) :
    real_parameter(0.0), scale(units),
    prompt_1(prompt_string_1), prompt_2(prompt_string_2)
  { }


  // This constructor sets an existing real_parameter to shadow this
  // newly-created object 

  command_line_parameter(real_parameter & target, double units, 
	       const char * prompt_string_1, const char * prompt_string_2 = 0) :
    real_parameter(0.0), scale(units),
    prompt_1(prompt_string_1), prompt_2(prompt_string_2)
  { target = this; }


// Command Line parameter input:

  // number of command line parameters
  int params() { return 1; }

  // parse command line starting with entry i+1:
  bool parse(int & i, int argc, const char ** argv)
  { 
    if(++i < argc) {
      real_parameter::set(atof(argv[i])*scale);
      return true;
    }
    else
      return false;
  }

  // display the command line entries for a usage prompt
  void help_1(std::ostream & s)
  { s << " <" << prompt_1 << ">"; }

  // display notes regarding the usage prompt
  void help_2(std::ostream & s)
  {
    if(prompt_2) {
      s << " *";
      help_1(s);
      s << ": " << prompt_2 << std::endl;
    }
  }

// Display state info:
  void display_state(std::ostream & s)
  { 
    s << comment 
      << ((prompt_2)? prompt_2 : prompt_1)
      << " : " << real_parameter::get()/scale << std::endl; }

// The operator = acts as though this is a real_parameter
  template <class T>
  command_line_parameter & operator = (T v)
  { (*this).real_parameter::operator =(v); return *this; }

};  // class command_line_parameter


// =======================================================================
// command_line_range: a container of functions for reading in a
// range from the command line, with associated help functions.

struct command_line_range : public command_line_parser {

  parameter low, high, step;  // starting, ending, and step values
  double scale;               // the units scaling factor
  const int args;             // how many arguments (2 or 3)
  const char * prompt1, * prompt2;

  // constructor: the last argument value tells how many
  // command line arguments to read (default 3); if set to:
  //  3 : read in low, high, and step values.
  //  2 : read in low and high values only; step must be
  //      set by the program.

  command_line_range(const char * prompt_string, double units = 1.0, int n = 3)
    : scale(units), args(n), prompt1(0), prompt2(prompt_string)
  {
    if (args != 2 && args != 3) {
      error::fatal("n = 2 or 3 in range constructor only!");
    }
  }

  command_line_range(const char * prompt_string2, const char * prompt_string1,
		     double units = 1.0, int n = 3)
    : scale(units), args(n), prompt1(prompt_string1), prompt2(prompt_string2)
  {
    if (args != 2 && args != 3) {
      error::fatal("n = 2 or 3 in range_type constructor only!");
    }
  }


// Command Line parameter input:

  // number of command line parameters
  int params() { return args; }

  // parse command line starting with entry i+1:
  bool parse(int & i, int argc, const char ** argv)
  {
    if(i+args >= argc) {
      i = argc;
      return false;
    }
    else {
      low  = atof(argv[++i])*scale;
      high = atof(argv[++i])*scale;
      if (args == 3)
	step = atof(argv[++i])*scale;
      return true;
    }
  }

  // display the command line entries for a usage prompt
  void help_1(std::ostream & s)
  {
    if(prompt1) {
      s << " <" << prompt1 << " Low> <" << prompt1 << " High>";
      if (args == 3)
	s << " <" << prompt1 << " Step>";
    }
    else {
      s << " <Low> <High>";
      if (args == 3)
	s << " <Step>";
    }
  }

  // display notes regarding the usage prompt
  void help_2(std::ostream & s)
  {
    if(prompt2) {
      s << " *";
      if(prompt1) {
	s << " <" << prompt1 << " Low> <" << prompt1 << " High>";
	if (args == 3)
	  s << " <" << prompt1 << " Step>";
      }
      else {
	s << " <Low> <High>";
	if (args == 3)
	  s << " <Step>";
      }
      s << ": enter range for " << prompt2 << std::endl; 
    }
  }
};  // class command_line_range


// =======================================================================
// class command_line_int : read a number from the command line and allow
// access to its value by casting to an int. Includes associated help
// functions. If the number read is not an integer, or is not within an
// optionally specified range, then parse() returns false.

struct command_line_int
  : public command_line_parser, public state_display {

  int value, min, max;
  bool limits;  // true if should check against min and max.
  const char *prompt_1, *prompt_2;

  // constructor: prompt_string_1 is printed inside angle brackets
  // in the help_1() prompt string; prompt_string_2 is the explanation
  // string printed by help_2().

  command_line_int(const char * prompt_string_1, const char * prompt_string_2 = 0,
		   bool lim = false, int minimum = 0, int maximum = 1)
    : value(0), min(minimum), max(maximum), limits(lim),
      prompt_1(prompt_string_1), prompt_2(prompt_string_2)
  { }


// Command Line parameter input:

  // number of command line parameters
  int params() { return 1; }

  // parse command line starting with entry i+1:
  bool parse(int & i, int argc, const char ** argv)
  { 
    if(++i < argc) {
      double n = atof(argv[i]);
      value = int(n);
      if(double(value) != n || (limits && (value < min || value > max)))
	return false;
      else
	return true;
    }
    else
      return false;
  }

  // display the command line entries for a usage prompt
  void help_1(std::ostream & s)
  { s << " <" << prompt_1 << ">"; }

  // display notes regarding the usage prompt
  void help_2(std::ostream & s)
  {
    if(prompt_2 || limits) {
      s << " *";
      help_1(s);
      s << ": ";
      if(prompt_2) s << prompt_2;
      if(limits) s << " Must be between " << min << " and " << max << " inclusive.";
      s << std::endl;
    }
  }

// Display state info:
  void display_state(std::ostream & s)
  {
    s << comment
      << ((prompt_2)? prompt_2 : prompt_1)
      << " : " << value << std::endl;
  }

// Get and set the value:
  operator int() const { return value; }
  command_line_int & operator = (const int & i) { value = i; return *this; }

};  // class command_line_int


// =======================================================================
// class command_line_string : read a string token from the command line.
// Copies the argument into a string member variable.

struct command_line_string
  : public command_line_parser, public state_display {

  std::string arg;
  const char *prompt_1, *prompt_2;

  // constructor: prompt_string_1 is printed inside angle brackets
  // in the help_1() prompt string; prompt_string_2 is the explanation
  // string printed by help_2().

  command_line_string(const char * prompt_string_1, const char * prompt_string_2 = 0)
    : prompt_1(prompt_string_1), prompt_2(prompt_string_2)
  { }


// Command Line parameter input:

  // number of command line parameters
  int params() { return 1; }

  // parse command line starting with entry i+1:
  bool parse(int & i, int argc, const char ** argv)
  { 
    if(++i < argc) {
      arg = argv[i];
      return true;
    }
    else
      return false;
  }

  // display the command line entries for a usage prompt
  void help_1(std::ostream & s)
  { s << " <" << prompt_1 << ">"; }

  // display notes regarding the usage prompt
  void help_2(std::ostream & s)
  {
    if(prompt_2) {
      s << " *";
      help_1(s);
      s << ": ";
      s << prompt_2;
      s << std::endl;
    }
  }

// Display state info:
  void display_state(std::ostream & s)
  {
    s << comment
      << ((prompt_2)? prompt_2 : prompt_1)
      << " : " << arg << std::endl;
  }

// Get and set the value:
  operator std::string() const { return arg; }
  command_line_string & operator = (const std::string & i) { arg = i; return *this; }

};  // class command_line_string

#endif
