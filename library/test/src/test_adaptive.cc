#include "adaptive.h"
#include "matmath.h"
#include "global.h"
#include <iostream>

using namespace std;

typedef double type;

double f(double x) { return tanh(x)*sin(x); }

int main()
{

  interpolator<type> Y;


  adaptive_fill(Y, f, -10, 10);

  cout << "# The number of data points in Y: " << Y.size() << endl;
  cout << "# The data points: (x, y(x)):" << endl;
  for (unsigned i = 0; i < Y.size(); ++i)
    cout << Y.x(i) << "\t" << Y[i] << endl;
}
