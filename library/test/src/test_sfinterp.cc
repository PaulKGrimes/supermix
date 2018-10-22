// 9/22/00: fixed Nb Tc. Now Tc = 9.2 Kelvin.
// 11/16/99: updated for new super_film class

#include "supermix.h"

double FMIN = 0.002*GHz;
double FMAX = 2000*GHz;
double tol  = .000001;
int    plotpoints = 1000;


struct sc_material { const double Vgap, Tc, rho_normal; }
  nb    = {2.9*mVolt, 9.2*Kelvin, 5.*Micro*Ohm*Centi*Meter},
  nbtin = {5.0*mVolt, 15.75*Kelvin, 30.*Micro*Ohm*Centi*Meter};

super_film film1, film2;

int main()
{
  double T = device::T = 4.0*Kelvin;
  error::stream(cerr);
  complex::out_space();

  film1.Vgap = nb.Vgap;
  film1.Tc   = nb.Tc;
  film1.rho_normal = nb.rho_normal;
  film1.no_interpolate();

  film2.Vgap = nb.Vgap;
  film2.Tc   = nb.Tc;
  film2.rho_normal = nb.rho_normal;
  film2.tol = tol;  

  double lrange = log(FMAX/FMIN);
  double d = lrange/(plotpoints-1);

  cout << left << "# " << setw(10) << "F (GHz)" << "    " << setw(20) << "Sigma"
       << "    " << setw(20) << "Interpolated" << endl;

  for(int i = 0; i < plotpoints-1; ++i) {
    double f = FMIN*exp(i*d);
    complex sref = film1.sigma(f,T);
    complex sinterp = film2.sigma(f,T);
    complex::out_degree();
    cout << setw(12) << f/GHz << "    " << setw(20) << sref << "    " << setw(20) << sinterp;
    if(abs((sinterp-sref)/sref) > 1.0e-7) cout << "    error exceeds 0.1 ppm";
    cout << endl;
  }
}
