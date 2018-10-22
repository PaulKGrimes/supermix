// fakeiv.cc
//
// Generates simple, simulated SIS DC IV curves suitable for use with
// supermix. The output IV curve will be normalized such that Vgap =
// Rn = 1. Command line parameters set the shape of the IV; just type
// the program name without any command line parameters to get a usage
// prompt. Uses an adaptive algorithm designed to output a minimal
// number of points suitable for use with a cubic spline interpolator
// (see "adaptive.h" and "interpolate.h"). Should output around 70-100
// points or so over the normalized voltage range of 0.0 to 2.0.
//
// You can input this curve to class ivcurve for use with class
// sis_basic_device. See "junction.h" and "sisdevice.h". When used
// with sis_basic_device, set the member variables Vn and Rn to the
// actual SIS gap voltage and normal resistance, respectively.
//
// The output will be written to the standard output stream.
//
// Here is a basic cartoon of the IV curve parameterization:
//
//  I  |                  |--| Gap_width            
//     |                             .             
//     |                           .               
//     |                         .                 
// 1.0 +                 -     .  <-- Slope here == 1.0,
//     |        I_defect |   .        so I(V) = V - I_defect             
//     |                 -  /                      
//     |                    |                      
//     |                    |                      
//     |  I_leakage         |                      
//     | /                  |    Slope below gap == 1/R_subgap,
//     | ................../  __/ so I(V) = I_leakage + V/R_subgap
//     +--------------------+-----------------
//                         1.0               V  
//
// Typical values for a submillimeter niobium SIS might be:
//    R_subgap  = 20.0      Gap_width = 0.02
//    I_leakage = 0.01      I_defect  = 0.10
//
// The Vn and Rn used with this curve in sis_basic_device might be:
//    Vn = 2.9*mVolt;  Rn = 17.0*Ohm
// ---------------------------------------------------------------------------

#include "supermix.h"
#include "numerical.h"  // this loads the adaptive fill routine

// numerical.h includes the adaptive interpolation builder (see
// adaptive.h).  This code is not included by supermix.h.

// ---------------------------------------------------------------------------
// Function to print a usage prompt, with definitions of the
// parameters

void prompt(const char *const *const argv);

// ---------------------------------------------------------------------------
// Global parameters:

double  G_subgap;      // Subgap/Normal conductance ratio  (0.0 - 1.0)
double  Gap_width;     // Relative width of the gap        (0.0 - 1.0)
double  I_leakage;     // Leakage current/(Vgap/Rn)        (0.0 - 1.0)
double  I_defect;      // Current defect above gap         (0.0 - 1.0)

const int cmd_line_params = 4; // the expected number of command line params

const char *comment = "#";     // comment prefix character for output

// ---------------------------------------------------------------------------
// Functions used to simulate a simple SIS dc IV response:

// output a straight line fit: y = mx+b:
inline double line(double x, double m, double b) { return m*x + b; }

// a hyperbolic tangent step function with some parameter checking
double sym_step(double x, double w)
{
  if (w > 0.0) return tanh(x/w);
  else {
    if (x  < 0.0) return -1.0;
    else if (x == 0.0) return  0.0;
    else return  1.0;
  }
}

// a finite-width generalization of the Heaviside step function:
inline double step(double x, double w) { return 0.5*(sym_step(x,w)+1.0); }

// the full simulation of the iv curve, using the curve parameters:
// (this function will be passed to the adaptive interpolator fill
// algorithm) It uses sym_step() at the origin to fade in the subgap
// behavior, and step() at the gap to transition from subgap to
// above-gap behavior.
double f(double v)
{
  double av     = fabs(v);
  double i_sg   = line(av, G_subgap, I_leakage)*sym_step(v, sqrt(Gap_width));
  double i_ag   = line(av, 1.0, -I_defect) * sym_step(v, Gap_width);
  double weight = step(av - 1.0, Gap_width);
  return weight*i_ag + (1.0-weight)*i_sg;
}

// ---------------------------------------------------------------------------
int main(int argc, char ** argv)
{
  // Input command line parameters; print a usage prompt if needed
  bool bad_param = false;
  if (argc != 1+cmd_line_params) bad_param = true;
  else {
    Gap_width = atof(argv[2]);
    G_subgap  = 1/atof(argv[1]);
    I_leakage = atof(argv[3]);
    I_defect  = atof(argv[4]);

    if (Gap_width <= 0.0 || Gap_width >  1.0) bad_param = true;
    if (G_subgap  <  0.0 || G_subgap  >  1.0) bad_param = true;
    if (I_leakage <  0.0 || I_leakage >  1.0) bad_param = true;
    if (I_defect  <  0.0 || I_defect  >  1.0) bad_param = true;
  }

  if (bad_param) {
    prompt(argv);
    return 1;
  }


  // Adaptively fill an interpolator with simulated IV curve
  // points. We use an interpolator here, since the SuperMix class
  // ivcurve does. We want to ensure that the points we output will
  // allow an interpolator to accurately match the desired IV curve
  // response. See interpolate.h and adaptive.h

  // Class interpolator<> is defined in interpolate.h and is a
  // templated class.  To construct an interpolator object we must
  // specify the data type of the dependent (y) variable. The
  // independent (x) variable is always of type double.  The default
  // is to use cubic spline interpolation, which is what we'll use.

  interpolator<double> idc;

  // Now we fill the interpolator with y(x) data points using an
  // adaptive algoithm.  The function adaptive_fill() is defined in
  // adaptive.h. It is a special numerical routine not normally
  // included by supermix.h, so we bring in its definition using
  // numerical.h, in the list of #include's at the beginning of this
  // program.

  // The adaptive fill algorithm works by first sprinkling the
  // interpolator with a few uniformly-spaced points from the function
  // to be interpolated. It then evaluates the accuracy of the
  // interpolation between each pair of points and adds additional
  // points recursively until it thinks it has met the accuracy
  // target. The resulting interpolator is filled with a set of
  // unevenly-spaced points, which keeps the total number of points in
  // the interpolator small, even for quite accurate interpolations.

  adaptive_fill(idc, f, 0, 2, .0001, .01);

  // In the above function call,
  //  idc   : the interpolator to be filled
  //  f     : the function object to be interpolated such that y = f(x)
  //  0     : the lower limit on the domain of the interpolation
  //  2     : the upper limit on the domain of the interpolation
  //  .0001 : the absolute error target for the interpolation
  //  .01   : the relative error target for the interpolation
  //
  // We don't set the error targets too high since we're just
  // simulating real iv curves here; these values will give us a nice
  // smooth spline.


  // Now output the (V,I) points in the interpolator.

  cout << fixed << setprecision(10);
  cout << comment << " Normalized simulated SIS DC IV characteristic curve" << endl;
  cout << comment << " Parameter values used in the simulation:" << endl;
  cout << comment << "  Gap Width                = " << Gap_width << endl;
  cout << comment << "  Subgap Resistance Ratio  = " << 1/G_subgap  << endl;
  cout << comment << "  Subgap Current Leakage   = " << I_leakage << endl;
  cout << comment << "  Normal Current Defect    = " << I_defect  << endl;
  cout << comment << endl;
  cout << comment << " V:         " << "\t" << "I:" << endl;

  // Now we print out the points actually used in the
  // interpolator. The size() member function of the interpolator
  // tells us how many points there are.

  for(unsigned i = 0; i < idc.size(); ++i)

    // The x(i) member function gives us the x value of the ith point;
    // the points are sorted by x value with the lowest x value
    // associated with point 0.  The indexing operation [i] on an
    // interpolator gives the y value of the ith point, just like with
    // a C-style array.

    cout << idc.x(i) << "\t" << idc[i] << endl;

}  // main()

// ---------------------------------------------------------------------------
// Definition of prompt()
void prompt(const char *const *const argv)
{
  cerr << argv[0] << ": construct a simulated, normalized SIS IV curve for"
       << endl << "use with SuperMix." << endl << endl;
  cerr << "Usage: " << argv[0] << "  R_subgap  Gap_width  I_leakage  I_defect"
       << endl << endl;

  cerr << "Where:" << endl
       << "       Parameter    Description                         Valid Range \n"
       << "       ---------    ---------------------------------   ----------- \n"
       << "       R_subgap     Subgap/Normal resistance ratio      > 1.0       \n"
       << "       Gap_width    Width of the gap relative to Vgap   0.0 - 1.0   \n"
       << "       I_leakage    Leakage current just above V = 0    0.0 - 1.0   \n"
       << "       I_defect     Current defect just above gap       0.0 - 1.0   \n"
       << endl
       << "       The output is normalized so that Vgap = Rn = 1.0.            \n"
       << "       Currents (I_leakage and I_defect) are normalized to Vgap/Rn. \n"
       << "       I_defect is the amount that the current just above Vgap      \n"
       << "       falls short of being equal to V/Rn. It rerpesents a constant \n"
       << "       offset below the I=V/Rn line for all currents with V>Vgap.   \n"
       << endl
       << "       Typical values for a fair submm SIS might be:  \n"
       << "          R_subgap  = 20          \n"
       << "          Gap_width = 0.02        \n"
       << "          I_leakage = 0.01        \n"
       << "          I_defect  = 0.1         \n"
       << endl;
}

