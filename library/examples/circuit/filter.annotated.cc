// filter.cc:
//
// The following code builds a 2 GHz maximally flat low-pass filter.
// See Pozar, page 489.

// access the supermix library definitions
#include "supermix.h"

int main()
{
  // Set the global temperature and normalization impedance.
  // This should be done early in main().
  // These variables are described in device.h
  // The physical units are described in global.h and units.h

  device::T = 295. * Kelvin;
  device::Z0 = 50. * Ohm;


  // capacitors and inductors are defined in elements.h; they are 2-port
  // primitive elements which can be either series or parallel.

  // Create a capacitor, call it c1.
  capacitor c1;           // c1 has been created with no particular value
  c1.parallel();
  c1.C = 0.984 * pFarad;  // here the value parameter is being accessed directly

  
  // Create an inductor, call it L2.
  inductor L2(6.438*nHenry);  // here's how to set the value at creation
  L2.series();

  
  // Create a capacitor, call it c3.
  capacitor c3;
  c3.parallel();
  c3.C = 3.183 * pFarad;

  
  // Create an inductor, call it L4.  Make it a copy of inductor L2.
  inductor L4 = L2;       // using the assignment operator

  
  // Create a capacitor, call it c5.  Make it a copy of capacitor c1.
  capacitor c5(c1);       // using the copy constructor (usually more efficient)

  
  // Create a circuit called filter to hold the connected circuit.
  // Actually a cascade would be more appropriate for this simple circuit.
  // circuits and cascades are defined in circuit.h

  circuit filter;

  // Build the filter circuit
  // The following statements could be rearranged in any desired order.

  // Specify the ports of the resulting circuit using add_port. The port
  // numbers of the circuit are automatically assigned consecutively.
  // add_port() returns the port number so you don't have to figure it out. 

  int input = filter.add_port(c1, 1);    // Specify the input port (port 1).
  int output = filter.add_port(c5, 2);   // Specify the output port (port 2).

  // Interconnect the component elements using connect().

  filter.connect(c1, 2, L2, 1);          // Make the connections in any
  filter.connect(L2, 2, c3, 1);          // convenient order.
  filter.connect(c3, 2, L4, 1);
  filter.connect(L4, 2, c5, 1);


  // Now set the output number format and display a header:  
  cout << fixed << setprecision(3);
  cout << "2 GHz low-pass filter" << endl << endl;
  cout << "F(GHz)" << "\t" << "S21" << "\t" << "Phase" << endl;

  // Here we set the desired complex number output format. See
  // SIScmplx.h for details.
  complex::out_degree();         // use magnitude and phase (in degrees)
  complex::out_separator("\t");  // separate the parts with a tab
                                 // (the default is a single space)
  
  // Loop over frequency and print out the frequency response
  for(double f=0.25; f<5.0; f+=0.25)  {

    // Set the global frequency. All circuit elements use device::f to
    // determine the frequency. Note the need for units, as always.

    device::f = f*GHz;


    // Find the complex transmission scattering parameter for the circuit.
    // Here we create a temporary sdata object to hold the circuit's
    // response at frequency device::f and temperature device::T. The
    // scattering matrix is normalized to impedance device::Z0. See
    // sdata.h for the definition of an sdata object. See nport.h for
    // a description of the member function get_data(), which actually
    // calculates and returns the circuit's response.

    sdata response = filter.get_data();
    complex S21 = response.S[output][input];  // pick out the S parameter
                                              // from the S matrix.

    // Display the results. Note use of units with the frequency
    cout << device::f/GHz << "\t" << S21 << endl;
  }

}
