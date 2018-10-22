// test_data_ptr.cc

#include "supermix.h"
 
int main(void) 
{
  cout<< fixed << setprecision(5) << setw(16);
  // Set global temperature
  device::T = 12.0 * Kelvin;

  sdata sd;

  kukje *k1;
  kukje k2;

  k1 = new kukje();

  sd = k1->get_data();

  k2 = *k1;

//  delete k1;

  sd = k2.get_last_data();
  sd.S.show();

  sd = k2.get_data();
  sd.S.show();
}
