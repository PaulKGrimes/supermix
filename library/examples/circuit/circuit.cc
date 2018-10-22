// circuit.cc
//
// How circuit elements are created, manipulated, and evaluated.

// access the supermix library definitions
#include "supermix.h"

int main()
{
  // -----------------------------------------------------------------
  // THE GLOBAL VALUES WHICH MUST BE SET

  // SuperMix must always know the following three facts about any
  // circuit it is called upon to analyze:
  //
  //   (1) The temperatures of the various parts of the circuit 
  //   (2) The frequency at which the circuit is to be analyzed
  //   (3) The normalizing impedance for the scattering matrices
  //
  // SuperMix therefore has three global variables which must be set
  // to provide these values (these variables are declared and
  // described in the SuperMix include file "device.h"). It is wise
  // to set these three variables early in main():

  device::T  = 295.*Kelvin;    // the default circuit temperature
  device::f  = 1*GHz;          // the frequency
  device::Z0 = 50.*Ohm;        // the S matrix normalizing impedance

  // NOTE THAT WE MUST PROVIDE UNITS WITH THE VALUES!!! See the file
  // examples/basics/using_units.cc

  
  // -----------------------------------------------------------------
  // HOW TO CREATE A CIRCUIT MODEL

  // First let's create a simple 2-port circuit consisting of a single
  // resistor in series with the two ports:

  resistor r;      // the type "resistor" is defined in "elements.h".
    r.series();    // now r represents a series resistance
    r.R = 50*Ohm;  // it now has a value; R is of type "parameter"

  // Circuit elements are all derived from the class "nport", defined
  // in "nport.h". This class provides the common interface for all
  // circuit elements, which includes the methods to calculate the
  // circuit elements' scattering and noise correlation matrices and
  // source vector (for elements that have embedded signal sources).

  // Now here's a parallel capacitor:

  capacitor c;     // also in "elements.h"
    c.parallel();
    c.C = 3*Pico*Farad;

  // We can create a circuit combining these two elements:

  circuit ckt;           // type "circuit" is defined in "circuit.h"

  // To build up our new circuit object, we list connections between
  // the ports of the component elements of our circuit. To do this we
  // use the member function connect():
 
  ckt.connect(r,2,c,1);  // we've connected port 2 of r to port 1 of c

  // Now we need to identify the ports of our newly-created
  // circuit. Member function add_port() accomplishes this. The ports
  // are added in order, so the first call to add_port identifies port
  // 1 of our circuit. add_port() returns the port number as an int,
  // so we can save the value if we want.

  int input  = ckt.add_port(r,1);  // port 1 of r becomes ckt's first port
  int output = ckt.add_port(c,2);  // port 2 of c becomes the other port

  // input == 1, since it was set by the first call to ckt.add_port().
  // similarly, output == 2. Let's check:

  cout << "input = " << input << ", output = " << output << endl << endl;

  
  // -----------------------------------------------------------------
  // CALCULATING CIRCUIT RESPONSE: sdata DATA TYPE

  // The results of a circuit response calcultion are presented using
  // the class "sdata", defined in "sdata.h". This class has 3 complex
  // member variables:
  //   S : a complex_matrix holding the scattering matrix
  //   C : a complex_matrix holding the noise correlation matrix
  //   B : a complex_vector holding the source vector

  // Let's look at the response of the resistor r:

  sdata s = r.get_data();

  // r.get_data() just calculated the response of r at the conditions
  // set by our global variables device::T and device::f, and
  // normalized the response using device::Z0. The results returned by
  // r.get_data() have been copied into the new variable s.

  cout << "Response of r at " << device::f/GHz << " GHz and " 
       << device::T/Kelvin << " Kelvin:" << endl << endl;

  cout << "S matrix:" << endl << s.S << endl << endl;
  cout << "C matrix (Kelvin):" << endl << s.C/Kelvin << endl << endl;
  cout << "B vector:" << endl << s.B << endl << endl;

  // The noise correlation matrix has elements with units of
  // Power/Bandwidth. SuperMix actually stores the elements using
  // Temperature units: (Power/Bandwidth)/(Boltzmann's Constant).
  // That's why we divided by Kelvin when outputting the C matrix, so
  // the numbers output would be meaningful.
  //
  // Since there isn't a signal generator in our resistor, the source
  // vector is all zeroes.

  // Here's the response of the RC circuit we built:

  s = ckt.get_data();
  
  cout << "Response of ckt:" << endl << endl;

  cout << "S matrix:" << endl << s.S << endl << endl;
  cout << "C matrix (Kelvin):" << endl << s.C/Kelvin << endl << endl;
  cout << "B vector:" << endl << s.B << endl << endl;


  // If we change the frequency, we get a different response:

  device::f = 10*GHz;
  s = ckt.get_data();

  cout << "Response of ckt at " << device::f/GHz << " GHz and " 
       << device::T/Kelvin << " Kelvin:" << endl << endl;

  cout << "S matrix:" << endl << s.S << endl << endl;
  cout << "C matrix (Kelvin):" << endl << s.C/Kelvin << endl << endl;
  cout << "B vector:" << endl << s.B << endl << endl;

  
  // -----------------------------------------------------------------
  // SOME OTHER FEATURES OF sdata

  // The sdata class has a couple of member functions for quickly
  // getting a specific feature of the response:

  // Gain in dB:
  cout << "Gain (dB):   " << s.SdB(output,input) << endl;

  // Noise temperature:
  cout << "Tn (Kelvin): " << s.tn(output,input)/Kelvin << endl;

  // Noise figure:
  cout << "NF:          " << s.NF(output,input) << endl;

  // (Note how we're using our port number variables we created)

  
}
