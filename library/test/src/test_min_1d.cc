// test class min_1d
// 8/3/99

#include "minimize1.h"
#include "num_utility.h"
#include <cmath>
#include <iostream>

using namespace std;

struct dummy {
  double fd(double x) const { return x*(-1+x*(.1*x*x -1)+sin(3*x)); }
} d;


dummy *pd = & d;

double f(double x)
{
  return pd->fd(x);
}



int main()
{
  double a,b,c,fa,fb,fc;

  a = -1; b = 0;

  cout << endl;
  cout << "return value fm bracket_minimum(): "
       << bracket_minimum(f,a,b,c,fa,fb,fc) << endl;
  cout << "a: " << a << ", b: " << b << ", c: " << c << endl;
  cout << "fa: " << fa << ", fb: " << fb << ", fc: " << fc << endl;

  cout << endl;
  cout << "return value fm refine_minimum(): "
       << refine_minimum(f,a,b,c,0) << endl;
  cout << "a: " << a << ", b: " << b << ", c: " << c << endl;

  a = -2.5; b = -2.4;

  cout << endl;
  cout << "return value fm bracket_minimum(): "
       << bracket_minimum(f,a,b,c,fa,fb,fc) << endl;
  cout << "a: " << a << ", b: " << b << ", c: " << c << endl;
  cout << "fa: " << fa << ", fb: " << fb << ", fc: " << fc << endl;

  cout << endl;
  cout << "return value fm refine_minimum(): "
       << refine_minimum(f,a,b,c,0) << endl;
  cout << "a: " << a << ", b: " << b << ", c: " << c << endl;

  a = 10; b = 10.1;

  cout << endl;
  cout << "return value fm bracket_minimum(): "
       << bracket_minimum(f,a,b,c,fa,fb,fc) << endl;
  cout << "a: " << a << ", b: " << b << ", c: " << c << endl;
  cout << "fa: " << fa << ", fb: " << fb << ", fc: " << fc << endl;

  cout << endl;
  cout << "return value fm refine_minimum(): "
       << refine_minimum(f,a,b,c,0) << endl;
  cout << "a: " << a << ", b: " << b << ", c: " << c << endl;

  a = 10; b = (1+10*1e-7)*a;

  cout << endl;
  cout << "return value fm minimize1(f,"<< a << "," << b << "): " << endl;
  { 
    double fm = minimize1(f,a,b);
    cout << " fmin = " << fm << ", xmin = " << a << endl;
  }

  a = -2.5; b = (1+10*1e-7)*a;

  cout << endl;
  cout << "return value fm minimize1(f,"<< a << "," << b << "): " << endl;
  { 
    double fm = minimize1(member_function(&dummy::fd,d),a,b);
    cout << " fmin = " << fm << ", xmin = " << a << endl;
  }
}
