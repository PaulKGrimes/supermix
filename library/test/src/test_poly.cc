// test polynomial.h

#include <list>
#include "SIScmplx.h"
#include "polynomial.h"
#include <iostream>

using namespace std;

int main()
{

  typedef poly_fit<complex> PF;
  typedef PF::entry  P;
  typedef PF::result Pans;

  PF pd;

  double x;
  Pans answer;

  // y = x*x:
  P p1(0.0,0.0), p2(1.0,1.0), p3(-2.0,4.0), p4(-1.0,1.0);

  vector<P> v;                   // use an STL vector,
  P a[4];                        // and a C-style array.

  v.push_back(p1);
  v.push_back(p2);
  v.push_back(p3);
  v.push_back(p4);

  a[0] = p1; a[1] = p2; a[2] = p3; a[3] = p4;

  cout << "P(x) should be x*x:" << endl;

  cout << endl;

  // send the STL vector:
  x = 5.0;
  cout << "STL vector: " << endl;
  answer = pd(x,v);
  cout << "P(" << x << "): " << answer.first << endl;
  cout << "error: " << answer.second << endl;

  cout << endl;

  // check that it uses the previous points for another call:
  x = 1.1;
  answer = pd(x);
  cout << "P(" << x << "): " << answer.first << endl;
  cout << "error: " << answer.second << endl;

  cout << endl;

  x = -sqrt(2.0);
  cout << "use previous data: " << endl;
  answer = pd(x);
  cout << "P(" << x << "): " << answer.first << endl;
  cout << "error: " << answer.second << endl;

  cout << endl;

  // use an iterator to the STL vector, with a count:
  x = 0.0;
  cout << "STL vector iterator and count: " << endl;
  answer = pd(x, v.begin(), 4);
  cout << "P(" << x << "): " << answer.first << endl;
  cout << "error: " << answer.second << endl;

  cout << endl;


  // test copy constructor:
  const PF pc(pd);

  x = 5.0;
  cout << "copy constructor: " << endl;
  answer = pc(x);
  cout << "P(" << x << "): " << answer.first << endl;
  cout << "error: " << answer.second << endl;

  cout << endl;

  x = -sqrt(2.0);
  answer = pc(x);
  cout << "P(" << x << "): " << answer.first << endl;
  cout << "error: " << answer.second << endl;

  cout << endl;

  x = 10;
  cout << "C array pointer and count: " << endl;
  answer = pd(x, a, 4);
  cout << "P(" << x << "): " << answer.first << endl;
  cout << "error: " << answer.second << endl;

  cout << endl;

  x = 1.1;
  answer = pd(x);
  cout << "P(" << x << "): " << answer.first << endl;
  cout << "error: " << answer.second << endl;

}
