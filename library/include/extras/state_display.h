// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ********************************************************************
// state_display.h
// class to manage the display of program and parameter state
// information
//
// includes class:
//   state_display (abstract class)
//
// F. Rice 2/28/04
// ********************************************************************
//                The state_display Abstract Class
//
// Users of the SuperMix library typically will generate many output
// files containing the results of optimizations and simulations of
// complex circuits. The simulation output depends on the values of
// various parameters controlling the circuit model behavior; some 
// parameter values often are varied from run to run, generating
// different results.  Keeping track of the parameter values used to
// generate the data of a given program run can be tedious and
// error-prone without some sort of automatic parameter value
// recording scheme.
//
// The abstact class "state_display" can be used to help automate this
// parameter record-keeping system. The idea is that classes derived
// from state_display contain references to the values of variables
// which need to be recorded along with the simulation data
// output. Each derived class implements a display_state() member
// function which class objects use to output their individual state
// data. The state_display abstract class keeps track of all derived
// class objects, so that simply calling state_display::info() causes
// all objects to emit their individual state information in turn.
//
//
// USAGE EXAMPLE:
//
// Assume that your program output depends on two real_parameter
// variables which may be varied from run to run of the program (maybe
// they are set via command line arguments, for example). You want to
// output the values used in the simulation as part of a header block
// at the start of the output (which we assume is sent to std::cout
// for this example). Let's further assume you want to prefix each
// line of the header with a comment delimiter character "#", so the
// header is not confused with the actual simulation data output.
//
// We derive a class from state_display which can refer to a parameter
// value and output it along with a string describing the variable:
//
// class parameter_state : public state_display {
//   const abstract_real_parameter & p;
//   std::string description;
// public:
//   // the constructor takes a reference to the variable to monitor
//   // and a string with a description of it:
//   parameter_state(const abstract_real_parameter & pp, 
//                   const std::string & s)
//     : p(pp), description(s) { }
//
//   // this is the state display function we must implement to have
//   // a concrete state_display derived class:
//   void display_state(std::ostream & s)
//   {
//     // begin the line with the comment delimiter string
//     s << state_display::comment
//       << description << ": " << p.get() << std::endl;
//   }
// };
//
// Here's how we could use our class parameter_state in our program:
//
// int main()
// {
//   parameter a, b;  // these are the parameters we need to output
//
//   // we create a class object to refer to each of the variables
//   parameter_state a_state(a, "parameter a");
//   parameter_state b_state(b, "parameter b");
//
//   // ...we set up the parameter values and prepare the simulation
//
//   // now we output the values of a and b using state_display:
//   state_display::comment = "# ";  // our header comment delimiter
//   state_display::info(std::cout); // output state to std::cout
//
//   // ...now we generate and output the results of the simulation
// }
//
// If, for example, parameters a and b had values of 3.2 and -10.45,
// the call to state_display::info() would output the following lines
// to cout:
//
// # parameter a: 3.2
// # parameter b: -10.45
//
//
// OUTPUT THE COMMAND LINE
//
// Since the command line used to invoke a program run may contain
// important information defining the conditions of a simulation,
// state_display contains a static member function to output it, along
// with an optional title. Here's how to use it:
//
// int main(int argc, char **argv)  // must use command line arguments
// {
//   char * title = "My program to demonstrate state_display";
//   state_display::comment = "# ";  // our header comment delimiter
//   state_display::command_line(cout, argc, argv, title);
// }
//
// If the program containing the above code were named "foo", and were
// invoked using: "foo 1.2 -13 7 bar 23", the call to command_line
// would output to cout:
//
// # My program to demonstrate state_display
// # Invoked with the command line:
// #  foo 1.2 -13 7 bar 23
//
// ********************************************************************

#ifndef STATE_DISPLAY_H
#define STATE_DISPLAY_H

#include <iostream>
#include <list>

// ===================================
// state_display
// an abstract class which supports display of state information

class state_display {

public:
  // A comment delimiter string can be used as a prefix to
  // all state information output. Default is a null string.
  static const char * comment;


  // Display the command line used to invoke the program, optionally
  // preceded by a title line.

  static void command_line(std::ostream & s, int argc, const char ** argv, 
			   const char * title = 0);

  static void command_line(std::ostream & s, int argc, char ** argv, 
			   const char * title = 0)
  { command_line(s, argc, (const char **)(argv), title); }


  // Display the status information for all derived objects
  static void info(std::ostream & s);


  // Each derived class must define this function, which outputs
  // the state data for that class object, with each line prefixed
  // by the state_display::comment string
  virtual void display_state(std::ostream &) = 0;



  // ---------------------------------------------------------------------
  // Implementation details:

  // the container of the objects who have state information
  typedef std::list<state_display *> objects;
  typedef objects::iterator pobj;

protected:
  static objects display_list;

  // this iterator is stepped through display_list during state display
  static pobj display_list_pos;

public:
  pobj my_display_list_pos; // so each object knows where it is in display_list


  // The constructor maintains the list of pointers to objects
  // with state display info:
  state_display(pobj position = display_list.end())
  { my_display_list_pos = display_list.insert(position, this); }

  // The copy constructor inserts the object just before the source
  state_display(const state_display & s)
  { 
    my_display_list_pos = 
      display_list.insert(s.my_display_list_pos, this);
  }

private:
  // hide operator =
  state_display & operator = (const state_display &);

public:
  virtual ~state_display()
  { display_list.erase(my_display_list_pos); }

};

#endif
