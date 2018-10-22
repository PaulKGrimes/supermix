// test_mixer_speed
// construct a mixer, then evaluate it repeatedly


#include "supermix.h"
#include <cstdlib>

using namespace std;

int main(int argc, char** argv)
{
  // get iterations from the command line
  int iterbal, iterget;
  if (argc == 2)
    iterbal = iterget = atoi(argv[1]);
  else if (argc == 3) {
    iterbal = atoi(argv[1]);
    iterget = atoi(argv[2]);
  }
  else {
    cout << "Usage: " << argv[0] << " <balance iterations> <get_data iterations>\n"
	 << "where the arguments give count numbers for the outer loops." << endl;
    cout << "Returns: total number of calls to nrcalc() during balances." << endl;
    return 1;
  }
  if(iterget < 1) iterget = 1;
  if(iterbal < 1) iterbal = 1;

  // make the mixer
  parameter LO, IF;
  device::f = &IF;
  device::T = 4*Kelvin;

  mixer m;
  m.harmonics(3);

  resistor R1(50*Ohm), R2(100*Ohm);
  branch b(3);
  circuit Rf, If;
  Rf.connect(R1,1,b,1); Rf.connect(R2,1,b,2);
  Rf.add_port(R1,2); Rf.add_port(R2,2); Rf.add_port(b,3);
  If.connect(R1,1,b,1); If.connect(R2,1,b,2);
  If.add_port(R1,2); If.add_port(R2,2); If.add_port(b,3);

  m.set_rf(Rf).set_if(If).set_LO(&LO);

  ivcurve iv("iv.dat","ikk.dat");
  parameter Rn = 10*Ohm;
  parameter Vn = 3*mVolt;
  parameter Cap = 0;

  sis_basic_device j1, j2;

  j1.set_iv(iv);
  j1.Rn = &Rn;
  j1.Vn = &Vn;
  j1.Cap = &Cap;

  j2.set_iv(iv);
  j2.Rn = &Rn;
  j2.Vn = &Vn;
  j2.Cap = &Cap;

  voltage_source j1_bias, j2_bias;

  j1_bias.source_voltage = Vn/2;
  j2_bias.source_voltage = Vn/3;

  circuit bias;
  bias.add_port(j1_bias, 1);
  bias.add_port(j2_bias, 1);
  
  m.add_junction(j1).add_junction(j2).set_bias(bias);

  voltage_source LO_source;
  LO_source.source_f = &LO;
  LO_source.source_width = 1*GHz;
  LO_source.source_voltage = 0.5*Vn/RmsToPeak;  // Peak value 1/2 the gap voltage
  m.set_balance_terminator(LO_source, 3);

  LO = 0.5*Vn*VoltToFreq;
  IF = 10*GHz;
  double LO_step = LO/(10*iterbal);
  double IF_step = LO/(10*iterget);
  
  int i, count;

  // balance loop
  for(i = 0, count = 0; i < iterbal; ++i, count += m.balance_iterations()) {
    LO = LO + LO_step;
    m.balance();
  }

  // small signal loop
  for(i = 0; i < iterget; ++i) {
    IF = IF + IF_step;
    m.get_data();
  }
  cout << count << endl;
}
