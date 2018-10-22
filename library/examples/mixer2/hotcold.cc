// hotcold.cc
//
// Generate simulated pumped IV and hot/cold load curves using a
// full, nonlinear simulation of a multijunction SIS mixer.
//
// Shows a possible way to organize the source code for a complex simulation
// which offers improved maintainablity and reusability.
// ==========================================================================

// First include the physical dimensions and other specific data for
// a particular receiver using this design (also includes object definitions):

#include "specs.h"


int main(int argc, char** argv)
{

  // ==========================================================================
  // Here is where any command line parameters should be read, if needed.


  // ==========================================================================
  // INCLUDE CODE TO BUILD THE MIXER MODEL:
  //   This is an unusual approach to source code. Since there is significant
  //   work required to be executed in main() in order to build the receiver
  //   circuitry (for example, see rfmatch.cc and mixer.cc), and this code is
  //   required by any simulation of this design, we have pulled out this code
  //   into a special include file.
  //
  //   Since this include file contains code to be executed inside main(), it
  //   is clearly not a header file. We use the extension ".inc" to emphasize
  //   this distinction.

# include "build_mixer.inc"

  // ==========================================================================
  // CODE FOR THE SPECIFIC SIMULATION OR OPTIMIZATION FOLLOWS:

  // We are calculating unpumped and pumped IV curves as well as the IF ouput
  // noise power spectral density (expressed in Kelvin) at the IF freq,
  // with the LO source temperature set to a hot value and a cold value.
  // This simulates laboratory pumped IV curve and Y-factor measurements. We
  // will sweep the SIS bias voltage and output unpumped and pumped total SIS
  // bias current, and cold and hot load IF output noise.

  parameter SOURCE_TEMP;   // will be the hot/cold source temperature
  LO.Temp = & SOURCE_TEMP; // now LO temperature shadows the source temp

  // LO power and frequency will use the values in specs.h

  IF_FREQ = 1.5*GHz;

  double power = LO_POWER;
  double hot = 300*Kelvin, cold = 80*Kelvin;  // Hot and cold load temp's

  // Now we sweep the bias voltage and calculate the results
  for(double v = 0; v <= 4; v += .05) {

    // First set the SIS bias voltage and report it:

    V_BIAS = v * mVolt;
    cout << V_BIAS/mVolt;

    // Now for the unpumped and pumped IV curves. We set the LO power and
    // then perform a harmonic balance of the mixer. Following this we
    // fetch a Vector containing the junction currents at harmonic 0, i.e.,
    // DC. We sum these currents to get the total bias current, since
    // for this model the two junctions are in parallel at DC.

    // First the unpumped current:
 
    LO_POWER = 0.0;
    mix.balance();

    // mix.I_junc(n) returns a vector of the complex currents at harmonic
    // n. The vector has an element for each SIS junction in the mixer, in
    // the same order that the junctions were added to the mixer using
    // add_junction() (in the code in the file build_mixer.inc).
 
    cout << "\t" << (mix.I_junc(0)[1]+mix.I_junc(0)[2]).real/(Micro*Amp);

    // ditto with the LO power turned on, the pumped current:

    LO_POWER = power;
    mix.balance();
    cout << "\t" << (mix.I_junc(0)[1]+mix.I_junc(0)[2]).real/(Micro*Amp);

    // Now we need the cold and hot load IF output powers. By setting
    // the SOURCE_TEMP parameter we indirectly control the noise
    // temperature of the LO source using parameter shadowing. Class
    // mixer can perform two different but related small signal analyses:
    // get_data() does the same thing as it does for any other nport, but
    // get_term_data() first terminates the RF input ports at every
    // harmonic with the balance terminators set by calls to the mixer's
    // set_balance_terminator(). During harmonic balance, these
    // terminators are required to fully specify the linear embedding
    // network the SIS junctions see; often this is how you inject LO
    // power, which is only needed during harmonic balance calculations.
    // However, when you use get_term_data(), the same terminations are
    // used, reducing the mixer to an nport with only IF ports. These
    // terminations introduce noise given by their noise temperatures and
    // their RF sideband frequencies. If set to 0 Kelvin, they serve to
    // inject the quantum noise needed to be added to the mixer noise
    // calculations.


    // First the cold load calculation. We get the IF output noise
    // correlation matrix element of the source + mixer combination using
    // get_term_data(). The noise correlation element gives the noise
    // power spectral density at the IF frequency as a temperature,
    // kT = Power/Hz.

    SOURCE_TEMP = cold;
    cout << "\t" << mix.get_term_data().C[IF_PORT][IF_PORT].real/Kelvin;

    // The hot load calculation:

    SOURCE_TEMP = hot;
    cout << "\t" << mix.get_term_data().C[IF_PORT][IF_PORT].real/Kelvin;
    cout << endl;
  }

}
