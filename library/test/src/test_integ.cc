// test integrate.h

#include <iostream>
#include <iomanip>
#include <cmath>
#include "ioext.h"
#include "global.h"
#include "matmath.h"
#include "integrate.h"

using namespace std;

typedef integrator<double>  DI;

double tol = 1.0e-10;
double a; // coefficient in f6() and f9()

double f1(double x) { return 2*x; }
double f2(double x) { DI d; return d(f1,0,x); }
double f3(double x) { return cos(x); }
double f4(double x) 
{ DI d(DI::OPEN); d.rel_tolerance = d.abs_tolerance = tol; return d(f3,0,x); }

double f5(double x) { return 0.5/sqrt(fabs(x)); }
double f6(double x) { return exp(-a*x); }
double f7(double x) { return 1.0; }
double f7a(double x) { return 1.0; }
double f8(double x) { return 1/(x*x); }
double f9(double x) { return sqrt(a*a - x*x); }

double f10(double x) 
{ DI d(DI::OPEN); d.rel_tolerance = d.abs_tolerance = tol;
 double s = a; a = f9(x); double A = d(f9,-a,a); a = s; return A; }

double f11(double x) { return 1.0 + 1.0/(x*x*x); }
double f12(double x) { return f11(x)+f11(-x); }
double f13(double x) { return 2*x + 3.0/(a-x); }

Vector fv(double x) 
{ static Vector v(3); v[1]=complex(1.0,2*x); v[2]=complex(2*x,3*x*x); 
  v[3]=complex(3*x*x,4*x*x*x); return v; }

double myabs(double x) { return fabs(x); }


int main()
{
  cout << "Error tolerance (both absolute and relative): " << tol << endl;

  DI dcl(DI::CLOSED);    dcl.rel_tolerance = dcl.abs_tolerance = tol;
  DI dop(DI::OPEN);      dop.rel_tolerance = dop.abs_tolerance = tol;
  DI dsq(DI::SQRTLOWER); dsq.rel_tolerance = dsq.abs_tolerance = tol;

  CauchyPV ch; ch.rel_tolerance = ch.abs_tolerance = tol;

  int max;


  // simple sum of integrals:
  cout << setprecision(6) << showpoint;
  cout << endl << "Simple sum of integrals" << endl;
  max = 20;
  for(int i = -max; i <= max; ++i) {
    DI d = dop;
    double x = i;
    cout << x 
	 <<"\t"<< dop(f7, 0, x, myabs)
	 <<"\t"<< d(f7, x/3, x, myabs) + d(f7, 0, x/3, myabs)
	 << endl; 
  }

  // sin's and cos's:
  cout << setprecision(3) << showpoint;
  cout << endl << "Compare accuracies of CLOSED and OPEN methods:" << endl;
  max = 20;
  for(int i = -max; i <= max; ++i) {
    double x = 2.0*Pi*double(i)/double(max);
    cout << x/Pi 
	 <<"\t"<< log10(abs(dcl(f3, 0, x)-sin(x)))
	 <<"\t"<< log10(abs(dop(f3, 0, x)-sin(x)))
	 << endl; 
  }

  // check out sqrt singularity methods
  cout << setprecision(10) << showpoint;
  cout << endl << "Check out sqrt singularity methods:" << endl;
  max = 20;
  for(int i = 1; i <= max; ++i) {
    double x = i*i;

    cout << x <<"\t"<< dsq.sqrtlower()(f5, 0, x) <<"\t"<< -dsq.sqrtupper()(f5, x, 0);
    cout      <<"\t"<< dsq.sqrtupper()(f5, -x, 0) <<"\t"<< -dsq.sqrtlower()(f5, 0, -x);
      
    cout << endl; 
  }

  // check out INFINITE
  cout << setprecision(15) << showpoint;
  cout << endl << "Check out method INFINITE:" << endl;
  dop.infinite();
  max = 20;
  for(int i = 1; i <= max; ++i) {
    double x = 5.0/i;
    cout << i/5.0 <<"\t"<< dop(f8, x, 1e30);
    cout <<"\t"<< -dop(f8, -x, -1e30);
    cout << endl; 
  }

  // check out EXP
  cout << setprecision(15) << showpoint;
  cout << endl << "Check out method EXP:" << endl;
  dop.abs_tolerance = 1e-100;
  max = 20;
  for(int i = 1; i <= max; ++i) {
    a = i;
    cout << (1/a)*exp(-a) <<"\t"<< dop.exp()(f6, 1, 0);
    cout <<"\t"<< dop.method(DI::INFINITE)(f6, 1, 1e30);
    cout << endl; 
  }
  dop.abs_tolerance = tol;

  // attempt a Cauchy PV type calculation
  cout << setprecision(15) << showpoint;
  dop.method(DI::OPEN);
  cout << endl << "Attempt a Cauchy PV type calculation:" << endl;
  max = 20;
  for(int i = 1; i <= max; ++i) {
    a = 0.01*i;
    cout << 2*a <<"\t"<< dop(f12, 0, a) <<"\t"<< dop(CauchyPV_calc(f11,0), 0, a);
    cout << endl; 
  }


  // attempt a Cauchy PV calculation
  cout << setprecision(6) << showpoint;
  cout << endl << "Attempt a Cauchy PV calculation:" << endl;
  max = 20;
  for(int i = 1; i <= max; ++i) {
    double x = i;
    cout << 3.5*x
	 <<"\t" << dop(f7,-x/2, 3*x)
	 <<"\t" << dop(f7, x/2, 3*x) + dop(CauchyPV_calc(f7,0), 0, x/2)
	 << "\t" << ch(f7, -x/2, 0, 3*x);
    cout << endl; 
  }

  // another attempt a Cauchy PV calculation
  cout << setprecision(8) << showpoint;
  cout << endl << "Another Cauchy PV calculation:" << endl;
  max = 20;
  for(int i = 1; i <= max; ++i) {
    double x = i;
    a = 0.5*x;
    cout << x*x << "\t" << Cauchy_integrate(f13, 0, a, x);
    //    cout << "\t" << dop(CauchyPV_calc(f13,a), 0, x/2);
    cout <<"\t"<< ch(f13, 0, a, x);
    cout << endl; 
  }

  // volume of a sphere using a double integral
  dop.method(DI::OPEN);
  cout << setprecision(3) << showpoint;
  cout << endl << "Volume of a sphere using a double integral:" << endl;
  max = 20;
  for(int i = 1; i <= max; ++i) {
    double save_tol = tol;
    dop.rel_tolerance = dop.abs_tolerance = tol = 1e-7;  
    a = i;
    cout << a << "\t"<< fabs((2*dop(f10, -a, a) - 4*Pi*a*a*a/3)/(4*Pi*a*a*a/3)) ;
    cout << endl;
    tol = save_tol;
  }
  dop.rel_tolerance = dop.abs_tolerance = tol;  

  // double integrate sin's and cos's:
  cout << setprecision(3) << showpoint;
  cout << endl << "Double integrate cos(x) to get cos(x) back:" << endl;
  max = 20;
  for(int i = -max; i <= max; ++i) {
    double x = 2.0*Pi*double(i)/double(max);
    cout << x/Pi 
	 <<"\t"<< fabs(dcl(f4, x, 0.5*Pi)-f3(x)) 
	 << endl; 
  }

  // integrate a Vector function
  cout << setprecision(3) << showpoint;
  cout << endl << "A Vector return value integrated:" << endl;
  integrator<Vector> dv; dv.order = 6;
  max = 20;
  for(int i = 1; i <= max; ++i) {
    double x = i;
    cout << x << "\t" << dv(fv, 0, x);
    cout << endl; 
  }

}
