/*
  Test class parameter

  Note: Some of the expected return values displayed are incorrect;
  they have not been corrected in order to continue regression
  testing, since previous versions have the same error.

* 7/28/98: added <iostream.h> to includes, since SIScmplx.h doesn't*

*/

// 1/17/04:  Modified for gcc 3.x


#include <iostream>
#include "parameter.h"
#include "parameter/scaled_real_parameter.h"
#include "parameter/function_real_parameter.h"

using namespace std;

// Use this function later in a parameter to multiply two parameters.
double multiply(real_parameter *a)
{
  // Test operator *(parameter, parameter)
  
  return(a[0] * a[1]);
}

// Use this function later in a parameter to multiply a parameter by 3.0.
double times3(real_parameter *a)
{
  // Test operator *(parameter, double)
  return(*a * 3.0);
}

int main()
{
  parameter a, b, c, d, e;

  // Create a chain of parameter references.
  a = 5.0;
  b = 5.0;
  c = &a;
  d = &c;
  e = &d;

  cout << "Expect 55555: " << a << b << c << d << e << endl;

  // Change the head.  It should change all of them.
  a = 1.0;
  cout << "Expect 15111: " << a << b << c << d << e << endl;

  // Change something in the middle.  It should change all of them.
  e = 2.0;
  cout << "Expect 25222: " << a << b << c << d << e << endl;

  b = &a;

  cout << "Expect 2: " << b << endl;

  // Test max and min.
  b.set_max(12.0);
  b.set_min(8.0);

  cout << "Expect 8: " << b << endl;

  b.set_min(-10.0);
  b.set_max(1.5);

  cout << "Expect 1.5: " << b << endl;

  // Test constructors
  b = parameter(5.0);
  cout << "Expect 5: " << b << endl;
  b = parameter(3);
  cout << "Expect 3: " << b << endl;

  // Test max and min
  a.set_max(100.0);
  a.set_min(0.0);
  b = a;
  cout << "Expect 1.5 1.5: " << a << " " << b << endl;
  b = 3.0;
  cout << "Expect 1.5 3: " <<  a << " " << b << endl;

  // Self-shadowing should throw an error.
  cout << "Attempt self-shadowing (a = &a):" << endl;
  a = &a;
  cout << a << endl;

  // Infinite parameter loops should throw an error.
  cout << "Attempt inifinite shadow loop:" << endl;
  b = &a;
  c = &b;
  a = &c;
  cout << a << " " << b << endl;

  // Test parameter that is calculated by a function 
  parameter q, r[2];
  function_real_parameter p;
  p.set(&times3, &q);
  q = 5.0;
  cout << "Expect 15: " << p << endl;

  // Test parameter that is calculated by a function 
  p.set(&multiply, r);
  r[0].set(6.0);
  r[1].set(7.0);
  cout << "Expect 42: " << p << endl;
  r[0].set(7.0);
  cout << "Expect 49: " << p << endl;

  // Test addition
  parameter y, z;
  y = 5.0;
  z = 3.0 + y;
  cout << "Expect 8: " << z << endl;

  // Test << and >>
  parameter par;

/* commented out this test 2/5/98 FRR 
  cout << "Enter a double value: ";
  cin >> par;
  cout << "You entered " << par << endl;
*/

  cout << "Test unary operators" << endl;
  par = 5.0;
  cout << par << " unary -: " << -par << endl;
  cout << par << " unary +: " << +par << endl;

  cout << "Test +=, -=, *=, /=" << endl;
  parameter n, m;
  n = 5.0;
  m = 3.0;
  n += m;
  cout << "Expect 8: " << n << endl;
  n += 3.0;
  cout << "Expect 11: " << n << endl;
  n = 5.0;
  n -= m;
  cout << "Expect 2: " << n << endl;
  n -= 4;
  cout << "Expect -2: " << n << endl;
  n *= -2;
  cout << "Expect 4: " << n << endl;
  n *= m;
  cout << "Expect 12: " << n << endl;
  n /= m;
  cout << "Expect 4: " << n << endl;
  n /= 2;
  cout << "Expect 2: " << n << endl;

  cout << "Test scaling." << endl;
  parameter sc1=5.0;
  scaled_real_parameter sc2(2.0, sc1);
  cout << "Expect 5, 10: " << sc1 << ", " << sc2 << endl;
  sc1 = 6;
  cout << "Expect 6, 12: " << sc1 << ", " << sc2 << endl;
  sc2.set(3.0, sc1);
  cout << "Expect 6, 18: " << sc1 << ", " << sc2 << endl;
}
