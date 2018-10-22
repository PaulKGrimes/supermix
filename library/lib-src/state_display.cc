// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
//
// state_display.cc
//
// Implementation of the classes in extras/state_display.h and
// extras/cmd_line.h

#include "extras/state_display.h"
#include "extras/cmd_line.h"

using namespace std;

// =======================================================================
// state_display

// The static member objects of state_display:
state_display::objects state_display::display_list;
state_display::pobj    state_display::display_list_pos = display_list.end();
const char * state_display::comment = "";


// Function definitions for class state_display:

void state_display::command_line(ostream & s, int argc, const char ** argv, 
					const char * title)
{
  if(title) s << comment << title << endl;
  s << comment << "Invoked with the command line:" << endl << comment;
  for (int i = 0; i < argc; ++i)
    s << " " << argv[i];
  s << endl;
}

void state_display::info(ostream & s)
{
  s << comment << "State Data:" << endl;
  pobj & i = display_list_pos;
  for (i = display_list.begin(); i != display_list.end(); ++i)
    (*i)->display_state(s);
}

// =======================================================================
// command_line_parser

// the static member objects of command_line_parser:
command_line_parser::objects command_line_parser::cmd_list;
command_line_parser::pobj    command_line_parser::cmd_list_pos = cmd_list.end();


// Function definitions for class command_line_parser:

int command_line_parser::total_params()
{
  pobj i;
  int n = 0;
  for (i = cmd_list.begin(); i != cmd_list.end(); ++i)
    n += (*i)->params();
  return n;
}

void command_line_parser::short_usage(const char * prog_name,
				      const char * prompt)
{
  if (prompt) error::stream() << prompt << endl;
  error::stream() << prog_name;
  pobj i;
  for (i = cmd_list.begin(); i != cmd_list.end(); ++i)
    (*i)->help_1(error::stream());
  error::stream() << endl;
}

void command_line_parser::usage(const char * prog_name,
				const char * prompt)
{
  if (prompt) error::stream() << prompt << endl;
  error::stream() << "Usage: " << prog_name;
  pobj i;
  for (i = cmd_list.begin(); i != cmd_list.end(); ++i)
    (*i)->help_1(error::stream());
  error::stream() << endl;
  error::stream() << "\nWhere:\n";
  for (i = cmd_list.begin(); i != cmd_list.end(); ++i)
    (*i)->help_2(error::stream());
  error::stream() << endl;
}

void command_line_parser::get_commands(int argc, const char ** argv,
				       const char * prompt)
{
  const char * const alert = "= = = = = = = = = = = = = = = ="
    " = = = = = = =\n";
  if (argc <= command_line_parser::total_params()) {
    // not enough command line arguments
    if (argc > 1)
      error::stream() << alert
		      << "Unexpected end to command line argument list!"
		      << endl << alert;
    usage(argv[0],prompt);
    exit(1);
  }
  
  // parse command line parameters
  int n = 0;  // this is the index into the argv[] array
  pobj & i = cmd_list_pos;
  for (i = cmd_list.begin(); i != cmd_list.end(); ) {
    if( !((*(i++))->parse(n, argc, argv)) ) {
      // some error occured when parsing
      if(n >= argc) {
	// ran out of command line arguments
	error::stream() << alert
			<< "Unexpected end to command line argument list!"
			<< endl << alert;
	usage(argv[0],prompt);
      }
      else {
	// an error interpreting an argument
	error::stream() << alert
			<< "I don't understand command line argument " << n 
			<< ": " << argv[n] << endl << alert;
	usage(argv[0],prompt);
      }
      exit(1);
    }
  } // for()
  if(n < argc-1) {
    error::stream() << alert << "Too many command line arguments!\n";
    if(n == argc-2)
      error::stream() << "Last argument (\"" << argv[argc-1]
		      << "\") doesn't seem to be needed."
		      << endl << alert;
    else
      error::stream()  << "Last " << argc-1-n
		       << " arguments don't seem to be needed."
		       << endl << alert;
    usage(argv[0],prompt);
    exit(1);
  }
}

