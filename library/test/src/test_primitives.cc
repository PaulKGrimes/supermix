/*

Test class resistor, capacitor, and inductor.

* 7/28/98: added <iostream.h> to includes, since SIScmplx.h doesn't*

*/


#include <iostream>
#include "nport.h"
#include "elements.h"
#include "circuit.h"
#include "units.h"
#include "error.h"

using namespace std;
 
int main(void) {
  // Set global frequency and temperature
  device::f = 5.0 * GHz;
  device::T = 100.0 * Kelvin;
 
  // Test series resistor
  resistor *r1 = new resistor();
  cout << "\nSeries resistor" << endl;
  cout << "Resistor id: " << r1->id << " size: " << r1->size() << endl;
  r1->series();
  r1->R = 20.0 * Ohm;
  cout << "Set R=20 * Ohm" << endl;
  sdata sd = r1->get_data();
  cout << "Resistor S matrix: " << endl;
  sd.S.show();
  cout << "Resistor C matrix: " << endl;
  (sd.C / Kelvin).show();
 
  // Test parallel resistor
  resistor *r2 = new resistor();
  cout << "Parallel resistor" << endl;
  cout << "Resistor id: " << r2->id << " size: " << r2->size() << endl;
  r2->parallel();
  r2->R = 20.0 * Ohm;
  cout << "Set R=20 * Ohm" << endl;
  sd = r2->get_data();
  cout << "Resistor S matrix: " << endl;
  sd.S.show();
  cout << "Resistor C matrix: " << endl;
  (sd.C / Kelvin).show();
 
  // Test copy of parallel resistor
  resistor *rcopy = new resistor(*r2);
  cout << "Copy of parallel resistor" << endl;
  cout << "Resistor id: " << rcopy->id << endl;
  sd = rcopy->get_data();
  cout << "Resistor S matrix: " << endl;
  sd.S.show();
  cout << "Resistor C matrix: " << endl;
  (sd.C / Kelvin).show();
 
  // Test series capacitor
  capacitor *x = new capacitor();
  cout << "Capacitor id: " << x->id << " size: " << x->size() << endl;
  x->series();
  x->C = 1.0 * pFarad;
  cout << "Set C=1 pFarad" << endl;
  sd = x->get_data();
  cout << "Capacitor S matrix: " << endl;
  sd.S.show();
  cout << "Capacitor C matrix: " << endl;
  (sd.C / Kelvin).show();
 
  // Test series inductor
  inductor *l = new inductor();
  cout << "Inductor id: " << l->id << " size: " << l->size() << endl;
  l->series();
  l->L = 10 * nHenry;
  cout << "Set L=1 nHenry" << endl;
  sd = l->get_data();
  cout << "Inductor S matrix: " << endl;
  sd.S.show();
  cout << "Inductor C matrix: " << endl;
  (sd.C / Kelvin).show();
 
  // Test out the copy function
  inductor *misc = new inductor(*l);
  cout << "make nport copy of inductor" << endl;
  sd = misc->get_data();
  cout << "Inductor S matrix: " << endl;
  sd.S.show();
  cout << "Inductor C matrix: " << endl;
  (sd.C / Kelvin).show();
 
  cout << "make a copy of a copy" << endl;
  inductor *misc2 = new inductor(*misc);
  sd = misc2->get_data();
  cout << "Inductor S matrix: " << endl;
  sd.S.show();
  cout << "Inductor C matrix: " << endl;
  (sd.C / Kelvin).show();
}
