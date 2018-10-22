/*

Test class branch and class series_tee.


* 7/28/98: added <iostream.h> to includes, since SIScmplx.h doesn't*
*/

#include <iostream>
#include "nport.h"
#include "elements.h"
#include "circuit.h"
#include "units.h"
#include "error.h"

using namespace std;
 
int main(void)
{
  // create a branch with default constructor;
  branch br;
  sdata sd = br.get_data();
  cout << "Branch S matrix: " << endl;
  sd.S.show();
  cout << "Branch C matrix: " << endl;
  (sd.C / Kelvin).show();

  // create a branch with 5 ports
  br = branch();
  br.set_branches(5);
  sd = br.get_data();
  cout << "Branch S matrix: " << endl;
  sd.S.show();
  cout << "Branch C matrix: " << endl;
  (sd.C / Kelvin).show();
 
  // Create a series_tee
  series_tee st;
  sd = st.get_data();
  cout << "series_tee S matrix: " << endl;
  sd.S.show();
  cout << "series_tee C matrix: " << endl;
  (sd.C / Kelvin).show();
}
