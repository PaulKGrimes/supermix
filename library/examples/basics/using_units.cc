// using_units.cc

// UNITS MUST BE PROPERLY USED TO GET THE RIGHT ANSWERS OUT
// OF SUPERMIX !!!

// How to use units with numerical values in SuperMix:

#include "supermix.h"

// Numerical values with units must be stored and manipulated
// in a consistent way by the many numerical routines in
// SuperMix. To ensure that consistent results are obtained
// no matter what units the user desires (e.g., Volts vs.
// milliVolts), SuperMix uses the following conventions:
//
// (1) Electromagnetic formulas are expressed in their SI
//     form.
//
// (2) For power and other nonlinear calculations, amplitudes
//     for sinusoids are RMS, not peak.
//
// (3) Phases are in radians.
//
// (4) Program results should not be dependent upon a user
//     knowing the actual numerical magnitude stored in a
//     variable by SuperMix for some physical quantity; it
//     should not be necessary for the user to understand
//     how SuperMix stores physical quantities.
//
// SuperMix accomplishes the 4th goal by using a defined set
// of constants to scale numerical quantities in order to
// convert them to its internal numerical representation. These
// constants are defined in the Supermix header files "global.h"
// and "units.h"; "units.h" also includes extensive comments
// describing how the constants are to be used.

// This program gives a brief introduction to the use of units
// in SuperMix.

int main()
{
  // -----------------------------------------------------------------
  // BASIC USE OF SUPERMIX UNITS

  // Here we define two variables which must hold the numerical
  // values of some real physical quantities. WE MUST ASSIGN
  // VALUES TO THESE VARIABLES USING THE APPROPRIATE UNITS AS
  // MULTIPLIERS !!!

  double v = 0.5*Volt;  // v is now 0.5 Volts (RMS).
  double r = 100*Ohm;

  // Now if we perform some calculations using these quantities,
  // we can be sure that the results will have the proper internal
  // numerical representation.

  double i = v/r;      // i holds the properly normalized current

  // To see the result converted back into some meaningful numerical
  // value, we divide by the desired unit. We can use standard powers
  // of 10 multipliers as well:

  cout << "v (volts)      = " << v/(Volt)      << endl;
  cout << "r (kohms)      = " << r/(Kilo*Ohm)  << endl;
  cout << "i (milliamps)  = " << i/(Milli*Amp) << endl;

  // Since amplitudes are RMS, getting the average power is easy:

  cout << "P (milliwatts) = " << (v*i)/(Milli*Watt) << endl;

  // Note the units conversion rules: Multiply by the unit to
  // convert a physical value into its internal representation;
  // divide by the unit to convert an internal value into a
  // physical numerical quantity.


  // -----------------------------------------------------------------
  // SOME USEFUL CONSTANTS

  // Although not really units conversions, SuperMix also provides
  // some frequently-used constants for degree-radian and RMS-peak
  // conversions:

  // RmsToPeak == sqrt(2.0)
  cout << "peak v (volts) = " << RmsToPeak*v/Volt << endl;

  // Pi == 3.14159...
  cout << "sin(Pi/2)      = " << sin(Pi/2) << endl;

  // Degree == Pi/180; use to convert angles:
  cout << "sin(90*Degree) = " << sin(90*Degree)   << endl;
  cout << "atan(1) = " << atan(1.0)/Degree << " degrees" << endl;


  // -----------------------------------------------------------------
  // DEFINING NEW UNITS

  // It is easy to create additional units as needed:
  const double Inch = 2.54*Centi*Meter;
  const double Mil  = Inch/1000;

  double x = 1*Mil;   // this length has the correct internal form
  cout << x/Mil << " mil = " << x/Micron << " micron" << endl;


  // -----------------------------------------------------------------
  // PREDEFINED PHYSICAL CONSTANTS IN SUPERMIX 

  // SuperMix includes several physical constants already in the
  // proper internal representation; see "units.h" for the complete
  // list.

  // Speed of Light:
  const double Foot = 12*Inch;  // using our unit we defined above
  cout << "c in Km/S  = " << cLight/(Kilo*Meter/Second) << endl;
  cout << "c in ft/nS = " << cLight/(Foot/nSec) << endl;
  
  // e/h (convert voltage to frequency using e*V == h*f):
  v = 1.0*Milli*Volt;
  double f = VoltToFreq * v;   // f = (e/h)*v
  cout << v/(Milli*Volt) << " mV --> " << f/GHz << " gigaHertz" << endl;

}
