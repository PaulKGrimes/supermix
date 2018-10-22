// parameters2.cc

// Illustrates how we can have a parameter maintain a value which
// is the result of a complicated calculation.

#include "supermix.h"

// The shadowing capability of the parameter classes provides a
// powerful and flexible functionality.
//
// Suppose the program defines some parameter which we must set.
// Unfortunately, the value must be calculated from some other
// values which may be changing under control of an optimizer or
// read in from a file, etc. How do we ensure that the parameter
// dynamically maintains the proper calculated value during
// program execution?
//
// Solution: we can have the parameter shadow an object which
// performs the calculation whenever it is asked to provide its
// value. Here's a simple example:
//   we have a parameter which must be kept equal to the normal
//   resistance (Rn) of an SIS junction, but we must calculate
//   Rn from two variables we actually control: the SIS junction
//   area (A) and normal resistance-area product (RnA), so that
//   Rn = RnA/A.
//
// We derive a class from abstract_real_parameter which holds
// pointers to two parameter values (RnA and Area) and can perform
// the required calculation. We decide to call this new class
// "function":

class function : public abstract_real_parameter
{
public:  // DON'T FORGET THE "public:" LABEL !!!

  // We decide that we want pointers to the argument variables, which are
  // to be held in parameters:
  abstract_real_parameter *p1;
  abstract_real_parameter *p2;

  // We must put the required calulation in the body of a member function
  // named get() as follows (ensure you declare get() EXACTLY as shown): 
  double get() const
    { return (*p1) / (*p2); }  // (*p1 and *p2 behave like doubles)

  // To make things really clever, we also define a constructor which
  // simply initializes our pointers from two provided arguments:
  function(abstract_real_parameter * num, abstract_real_parameter * denom)
    : p1(num), p2(denom)
    { }

}; // DON'T FORGET THE SEMICOLON AFTER THE CLOSING BRACE !!!


// Now we can use our new class as follows:

int main()
{
  parameter Rn;         // here's the parameter we must calculate

  parameter RnA, Area;  // the values we actually control

  // Now we create an object of our new class. The constructor we
  // defined requires that two pointer arguments be provided when
  // we create an object of class "function". We name our object "f":
  function f(& RnA, & Area);

  // And have Rn shadow this new object
  Rn = & f;

  // Here's what happens when we assign values to RnA and Area. Refer to
  // "using_units.cc" for a desription of physical units like we use here:
  RnA  = 20 * Ohm*Micron*Micron;
  Area = 1.4*Micron * 1.4*Micron;

  cout << "RnA = "    << RnA/(Ohm*Micron*Micron) << " Ohm uM^2"
       << ", Area = " << Area/(Micron*Micron) << " uM^2"
       << endl;

  // Rn gets its value from f, which uses the values in RnA and Area:
  cout << "Rn = " << Rn/Ohm << " Ohm" << endl;
  
  // Changing RnA automatically changes Rn
  RnA = 10*Ohm;
  cout << endl
       << "RnA = "    << RnA/(Ohm*Micron*Micron) << " Ohm uM^2"
       << ", Area = " << Area/(Micron*Micron) << " uM^2"
       << endl;
  cout << "Rn = " << Rn/Ohm << " Ohm" << endl;
  
  // Ditto for changes to Area
  Area /= 2;
  cout << endl
       << "RnA = "    << RnA/(Ohm*Micron*Micron) << " Ohm uM^2"
       << ", Area = " << Area/(Micron*Micron) << " uM^2"
       << endl;
  cout << "Rn = " << Rn/Ohm << " Ohm" << endl;

}

/* Here's the program output:

RnA = 20 Ohm uM^2, Area = 1.96 uM^2
Rn = 10.2041 Ohm

RnA = 10 Ohm uM^2, Area = 1.96 uM^2
Rn = 5.10204 Ohm

RnA = 10 Ohm uM^2, Area = 0.98 uM^2
Rn = 10.2041 Ohm

*/
