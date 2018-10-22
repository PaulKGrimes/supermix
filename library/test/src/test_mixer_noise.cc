// test_mixer_noise
//
// Test limiting value of mixer noise temperature in the limit of no LO drive
// 8/18/98
//

#include "supermix.h"
#include <cstdlib>

using namespace std;

int main(int argc, char** argv)
{
  if (argc != 2) {
    cout << "Usage:" << endl
	 << "test_mixer_noise <Bias Voltage>" << endl
	 << "\tWhere bias voltage is in millivolts, and junction voltage is"
	 << " 3 millivolts." << endl;
    return 1;
  }

  // the junction:
  ivcurve iv("iv.dat","ikk.dat");
  sis_basic_device sis;
  sis.set_iv(iv).Vn = 3 * mVolt;
  sis.Rn = device::Z0;

  // the if and rf circuits:
  branch b1; b1.set_branches(2);
  branch b2 = b1;

  // the dc bias circuit:
  voltage_source v;
  v.source_voltage = atof(argv[1]) * mVolt;

  // the mixer
  mixer m;
  m.set_if(b1).set_rf(b2).set_bias(v).add_junction(sis);
  m.set_LO(300 * GHz).harmonics(2);

  // set the if freq and temperature
  device::f = .1 * GHz;
  device::T = 2 * Kelvin;

  // now calculate the mixer and display the if noise temperature
  m.balance();
  int if_port = m.port(2,0);

  double noise = m.C(if_port,if_port).real;

  cout << "\nIF noise temperature: " << noise/Kelvin << " Kelvin." << endl;

  cout << "\nApprox Formula (e*Vbias/2/BoltzK): "
       << eCharge * v.source_voltage / 2.0 / BoltzK / Kelvin << " Kelvin." << endl;

  double noise_current = sis.noise(device::f, device::T, m.harmonics()).read(0,0).real;

  cout << "\n H(if,if): " << noise_current /  (mAmp * mAmp / Hertz)
       << " (mAmp)^2/Hz." << endl;

  cout << "\nApprox Formula 2*e*Vbias/Rn: "
       << 2 * eCharge * v.source_voltage / sis.Rn / (mAmp * mAmp / Hertz)
       << " (mAmp)^2/Hz." << endl;

  ydata_ptr y( & sis.small_signal(device::f, m.harmonics()),
	       & sis.noise(device::f, device::T, m.harmonics()));
  double noise_2 = sdata(y).C.read(0,0).real;

  cout << "\nconverted H to sdata: " << noise_2/Kelvin << " Kelvin." << endl;

  cout << endl;
}

