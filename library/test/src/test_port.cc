/*

Test class port.

*/

#include <iostream>
#include "port.h"

using namespace std;

int main(void)
{
  port p1, p2, p3;
 
  p1.id = 1;
  p1.index = 1;
  p2.id = 2;
  p2.index = 20;
  p3.id = 2;
  p3.index = 20;
 
  cout << "Test port == operator:" << endl;
  cout << "false: " << (p1==p2) << endl;
  cout << "false: " << (p1==p3) << endl;
  cout << "true: " << (p2==p3) << endl;
}
