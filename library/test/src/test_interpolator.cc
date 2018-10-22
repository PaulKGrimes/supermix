// test_interpolator.cc
// test class interpolator
//
// F. Rice 3/26/99
// ********************************************************************

#include "interpolate.h"
#include "matmath.h"
#include <iostream>

using namespace std;

int main()
{
  interpolator<Vector> A;
  Vector a(2),b(2),c(2),d(2),e(2),f(2);
  b = 3.0; c = 4.0; d = 2.0; (e.unit(1))*= 4.0; f = c - e; 
  A.build();

  cout << "--------- Add some data; list it" << endl;
  A.add(1.0,a).add(2.0,c).add(-3.0,d).add(0.5,e).add(3.0,f);
  A.list_data_list();
  cout << "--------- Build previous set, added more data; list built set" << endl;
  A.build();
  A.add(1.1,b).add(-2.0,f).add(-3.1,d).add(0.9,c).add(3.2,f);
  A.list_lists();
  cout << "--------- List all data (used and unused)" << endl;
  A.list_data_list();

  cout << "--------- Now rebuild and list built set" << endl;
  A.build().list_lists();

  cout << "--------- Now try size(), x(), and operator []" << endl;
  for (unsigned i = 0; i < A.size(); ++i)
    cout << A.x(i) << " , " << A[i] << endl;

  cout << "--------- test ready(); should be 1" << endl;
  cout << A.ready() << endl;

  cout << "--------- clear and add different data (try a build with 1 entry)" << endl;
  (a.unit(1))*=(complex(-1,1)); b = 0; (c.unit(2))*=(complex(-2,2));
  A.clear().add(0,a).build().add(-2,b).add(2,c);
  A.list_data_list();

  cout << "--------- no build() yet, so ready() should be 0" << endl;
  cout << A.ready() << endl;

  cout << "--------- build and list built set" << endl;
  A.build().list_lists();

  cout << "--------- now try some linear interpolations/extrapolations" << endl;
  { double x;
  x = 1; cout << "y(" << x << ") = " << A(x) << "; y'(" << x << ") = " << A.prime(x) << endl;
  x = 3; cout << "y(" << x << ") = " << A(x) << "; y'(" << x << ") = " << A.prime(x) << endl;
  x = 6; cout << "y(" << x << ") = " << A(x) << "; y'(" << x << ") = " << A.prime(x) << endl;
  x = -1; cout << "y(" << x << ") = " << A(x) << "; y'(" << x << ") = " << A.prime(x) << endl;
  x = 10; cout << "y(" << x << ") = " << A(x) << "; y'(" << x << ") = " << A.prime(x) << endl;
  x = 4; cout << "y(" << x << ") = " << A(x) << "; y'(" << x << ") = " << A.prime(x) << endl;
  x = 2.5; cout << "y(" << x << ") = " << A(x) << "; y'(" << x << ") = " << A.prime(x) << endl;
  }

  cout << "--------- now for a copy constructor" << endl;
  interpolator<Vector> B(A);
  A.clear();
  { double x;
  x = 1; cout << "y(" << x << ") = " << B(x) << "; y'(" << x << ") = " << B.prime(x) << endl;
  x = 3; cout << "y(" << x << ") = " << B(x) << "; y'(" << x << ") = " << B.prime(x) << endl;
  x = 6; cout << "y(" << x << ") = " << B(x) << "; y'(" << x << ") = " << B.prime(x) << endl;
  x = -1; cout << "y(" << x << ") = " << B(x) << "; y'(" << x << ") = " << B.prime(x) << endl;
  x = 10; cout << "y(" << x << ") = " << B(x) << "; y'(" << x << ") = " << B.prime(x) << endl;
  x = 4; cout << "y(" << x << ") = " << B(x) << "; y'(" << x << ") = " << B.prime(x) << endl;
  x = 2.5; cout << "y(" << x << ") = " << B(x) << "; y'(" << x << ") = " << B.prime(x) << endl;
  }
  

  cout << "========= now for a simple natural spline" << endl;
  A.clear().type(interpolator<double>::SPLINE);
  a = 0.0; b.unit(1); c.unit(2);
  A.add(0,a).add(-1,c-b).add(1,c+b);
  A.left_slope(3*b-2*c).right_slope(3*b+2*c);
  A.build().list_lists();

  cout << "--------- data is for x,(x^3,x^2); -1<x<1" << endl;
  { double x;
  x = 1; cout << "y(" << x << ") = " << A(x) << "; y'(" << x << ") = " << A.prime(x) << endl;
  x = -.5; cout << "y(" << x << ") = " << A(x) << "; y'(" << x << ") = " << A.prime(x) << endl;
  x = 0; cout << "y(" << x << ") = " << A(x) << "; y'(" << x << ") = " << A.prime(x) << endl;
  x = +.3; cout << "y(" << x << ") = " << A(x) << "; y'(" << x << ") = " << A.prime(x) << endl;
  x = -1; cout << "y(" << x << ") = " << A(x) << "; y'(" << x << ") = " << A.prime(x) << endl;
  x = 4; cout << "y(" << x << ") = " << A(x) << "; y'(" << x << ") = " << A.prime(x) << endl;
  x = -2.5; cout << "y(" << x << ") = " << A(x) << "; y'(" << x << ") = " << A.prime(x) << endl;
  }

  cout << "--------- try operator =" << endl;
  { double x;
  B = A; B.no_extrapolation_warning(1); A.clear();
  x = 1; cout << "y(" << x << ") = " << B(x) << "; y'(" << x << ") = " << B.prime(x) << endl;
  x = -.5; cout << "y(" << x << ") = " << B(x) << "; y'(" << x << ") = " << B.prime(x) << endl;
  x = 0; cout << "y(" << x << ") = " << B(x) << "; y'(" << x << ") = " << B.prime(x) << endl;
  x = +.3; cout << "y(" << x << ") = " << B(x) << "; y'(" << x << ") = " << B.prime(x) << endl;
  x = -1; cout << "y(" << x << ") = " << B(x) << "; y'(" << x << ") = " << B.prime(x) << endl;
  x = 4; cout << "y(" << x << ") = " << B(x) << "; y'(" << x << ") = " << B.prime(x) << endl;
  x = -2.5; cout << "y(" << x << ") = " << B(x) << "; y'(" << x << ") = " << B.prime(x) << endl;
  }

  cout << "--------- add a duplicate; should be fatal..." << endl;
  B.add(A.x(0),a);    // a duplicate -- fatal error
  B.list_data_list(); // shouldn't get here

}
