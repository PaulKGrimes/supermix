/*

Test class nportSet.

* 7/28/98: added <iostream.h> to includes, since SIScmplx.h doesn't*
*/


#include <iostream>
#include "nport.h"
#include "elements.h"
#include "circuitADT.h"

using namespace std;

int main() {
  nportSet set;

  // Create some devices to put in set.
  nport *r1 = new resistor();
  nport *r2 = new resistor();
  nport *r3 = new resistor();
  nport *c1 = new capacitor();
  nport *c2 = new capacitor();

  // Put the devices in the set.
  cout << "Set length = " << set.len() << endl;
  set.show();
  set.add(r1);
  cout << "Set length = " << set.len() << endl;
  set.show();
  set.add(r2);
  cout << "Set length = " << set.len() << endl;
  set.show();
  set.add(r3);
  cout << "Set length = " << set.len() << endl;
  set.show();

  // Try to add something that's already in the set.
  set.add(r2);
  cout << "Set length = " << set.len() << endl;
  set.show();
  set.add(r1);
  cout << "Set length = " << set.len() << endl;
  set.show();
  set.add(r3);
  cout << "Set length = " << set.len() << endl;
  set.show();

  // Add some more stuff...
  set.add(c1);
  cout << "Set length = " << set.len() << endl;
  set.show();
  set.add(c2);
  cout << "Set length = " << set.len() << endl;
  set.show();

  // Try making a copy.
  cout << "Copying list." << endl;
  nportSet newset(set);
  cout << "New Set length = " << newset.len() << endl;
  newset.show();


  cout << "Popping:" << endl;
  set.pop(3);
  cout << "Set length = " << set.len() << endl;
  set.show();
  set.pop(1);
  cout << "Set length = " << set.len() << endl;
  set.show();
  set.pop(5);
  cout << "Set length = " << set.len() << endl;
  set.show();
  set.pop();
  cout << "Set length = " << set.len() << endl;
  set.show();
  set.pop();
  cout << "Set length = " << set.len() << endl;
  set.show();
  set.pop();
  cout << "Set length = " << set.len() << endl;
  set.show();


  // Is the new set still there?
  cout << "New Set length = " << newset.len() << endl;
  newset.show();


  // Try copying an empty set.
  nportSet another(set);
  cout << "copy of empty set length = " << another.len() << endl;
  another.show();

  newset.show();

/* Commented out memory leak tests 2/5/98 FRR
  // Test for memory leaks.  Run "top" while this loops to monitor memory usage
  nportSet *setptr;
  while(1)
  {
    setptr = new nportSet(newset);
    delete setptr;
  }
*/

}
