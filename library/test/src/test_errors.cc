/*

Do some illegal stuff to make sure that error-handling code works.


* 7/28/98: added <iostream.h> to includes, since SIScmplx.h doesn't*
*/

#include <iostream>
#include "nport.h"
#include "elements.h"
#include "units.h"
#include "error.h"

using namespace std;
 
int main(void)
{
  // Set the global frequency.
  device::f = 5.0 * GHz;

  // Create some elements to play with.
  series_tee st;
  resistor *r1 = new resistor();
  sdata sd;

  // Get an element that doesn't exist
  cout << "Do something illegal...." << endl;
  complex cplx = st.S(4,1);
  cplx = 0.0;
 
  // Try giving a bad parameter
  cout << "How about a negative resistance?" << endl;
  r1->R.set(-4.5 * Ohm);
  sd = r1->get_data();
  cout << "Resistor S matrix: " << endl;
  sd.S.show();
  cout << "Resistor C matrix: " << endl;
  (sd.C / Kelvin).show();
 
  // Get an element that doesn't exist, this time with strict=TRUE.
  cout << "Now with strict=TRUE...." << endl;
  error::strict = TRUE;
  cplx = st.S(4,1);
}
