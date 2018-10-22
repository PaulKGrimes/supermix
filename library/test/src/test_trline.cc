// test_trline
// Various checks of the trline class
// 11/11/98: Changed table access to new syntax

#include "supermix.h"
#include "parameter/complex_parameter.h"
#include "parameter/scaled_real_parameter.h"
#include <iostream>


using namespace std;

int diff(const sdata & s1, const sdata & s2); // return 1 if not identical

int main()
{

  Complex::rel_tolerance = 1.0e-10;  // get rid of tiny imaginary parts
  Complex::out_degree(); Complex::out_delimited();
  cout << endl;
  device::f = 100*GHz; device::T = 4*Kelvin;

  trline t1;

  //================================================================================
  // INITIAL TESTS OF CREATION, COPYING, OPERATOR =, PARAMETER SETTING

  // check that default values are set correctly:

  if(t1.Temp != device::T)
    cout << "Default temperature not equal to device::T. Set to: " << t1.Temp << endl;
  if(t1.zchar != complex(device::Z0)) 
    cout << "Default impedance not equal to device::Z0. Set to: " << t1.zchar << endl;
  if(t1.loss != 0.0) 
    cout << "Default loss not equal to 0. Set to: " << t1.loss << endl;
  if(t1.theta != 0.0) 
    cout << "Default phase length not equal to 0. Set to: " << t1.theta << endl;
  if(t1.freq != 0.0) 
    cout << "Default freq (for phase length) not equal to zero. Set to: " << t1.freq << endl;

  // check that initial sdata is size two
  if(t1.size() != 2)
    cout << "Size not equal two. Set to: " << t1.size() << endl;


  // now try setting values in various ways:

  parameter P1 = 10.0;
  complex_parameter P2 = complex(2.0,3.0);
  double saveP1 = P1; complex saveP2 = P2;

  // set to constants:
  t1.set_theta(1).set_freq(2).set_loss(3).set_zchar(4).set_T(5);
  if((t1.theta != 1.0)||(t1.freq != 2.0)||(t1.loss != 3.0)||(t1.zchar != complex(4.0))
     ||(t1.Temp != 5.0))
    cout << "Setting values using set_*(constant) failed. Values should be:\n"
	 << 1.0 << 2.0 << 3.0 << complex(4.0) << 5.0 << endl
	 << "Values actually are:\n"
	 << t1.theta << t1.freq << t1.loss << t1.zchar << t1.Temp << endl;

  // set to parameters, no shadowing:
  t1.set_theta(P1).set_freq(P1).set_loss(P1).set_zchar(P2).set_T(P1);
  if((t1.theta != P1)||(t1.freq != P1)||(t1.loss != P1)||(t1.zchar != P2)
     ||(t1.Temp != P1))
    cout << "Setting values using set_*(parameter) failed. Values should be:\n"
	 << P1 << P1 << P1 << P2 << P1 << endl
	 << "Values actually are:\n"
	 << t1.theta << t1.freq << t1.loss << t1.zchar << t1.Temp << endl;
  P1 = 20; P2 = complex(30,40);
  if((t1.theta != saveP1)||(t1.freq != saveP1)||(t1.loss != saveP1)||(t1.zchar != saveP2)
     ||(t1.Temp != saveP1))
    cout << "Values set using set_*(parameter) changed when parameter changed."
	 << " Values should be:\n"
	 << saveP1 << saveP1 << saveP1 << saveP2 << saveP1 << endl
	 << "Values actually are:\n"
	 << t1.theta << t1.freq << t1.loss << t1.zchar << t1.Temp << endl;

  // set to parameters using shadowing: 
  P1 = saveP1; P2 = saveP2;
  t1.set_theta(&P1).set_freq(&P1).set_loss(&P1).set_zchar(&P2).set_T(&P1);
  if((t1.theta != P1)||(t1.freq != P1)||(t1.loss != P1)||(t1.zchar != P2)
     ||(t1.Temp != P1))
    cout << "Setting values using set_*(&parameter) failed. Values should be:\n"
	 << P1 << P1 << P1 << P2 << P1 << endl
	 << "Values actually are:\n"
	 << t1.theta << t1.freq << t1.loss << t1.zchar << t1.Temp << endl;
  P1 = 20; P2 = complex(30,40);
  if((t1.theta != P1)||(t1.freq != P1)||(t1.loss != P1)||(t1.zchar != P2)
     ||(t1.Temp != P1))
    cout << "Values set using set_*(&parameter) don't change when parameter changes."
	 << " Values should be:\n"
	 << P1 << P1 << P1 << P2 << P1 << endl
	 << "Values actually are:\n"
	 << t1.theta << t1.freq << t1.loss << t1.zchar << t1.Temp << endl;


  // now check copy constructor and oper =:

  trline t2(t1);
  if((t2.theta != t1.theta)||(t2.freq != t1.freq)||(t2.loss != t1.loss)
     ||(t2.zchar != t1.zchar)||(t2.Temp != t1.Temp))
    cout << "Copy constructor failed. Values should be:\n"
	 << t1.theta << t1.freq << t1.loss << t1.zchar << t1.Temp << endl
	 << "Values actually are:\n"
	 << t2.theta << t2.freq << t2.loss << t2.zchar << t2.Temp << endl;
  if(t2.size() != 2)
    cout << "Copy constructor size not equal two. Set to: " << t2.size() << endl;

  // make sure t2's parameters are shadowing, since t1's were:  
  P1 = saveP1; P2 = saveP2;
  if((t2.theta != P1)||(t2.freq != P1)||(t2.loss != P1)||(t2.zchar != P2)
     ||(t2.Temp != P1))
    cout << "Copy constructor didn't properly set parameter shadowing."
	 << " Values should be:\n"
	 << P1 << P1 << P1 << P2 << P1 << endl
	 << "Values actually are:\n"
	 << t2.theta << t2.freq << t2.loss << t2.zchar << t2.Temp << endl;

  // reset t1 to default using operator =; make sure t1 is reset, t2 doesn't change:
  t1 = trline();
  if(t1.Temp != device::T)
    cout << "In t1 = trline. Default temperature not equal to device::T. Set to: " 
	 << t1.Temp << endl;
  if(t1.zchar != complex(device::Z0)) 
    cout << "In t1 = trline. Default impedance not equal to device::Z0. Set to: " 
	 << t1.zchar << endl;
  if(t1.loss != 0.0) 
    cout << "In t1 = trline. Default loss not equal to 0. Set to: " 
	 << t1.loss << endl;
  if(t1.theta != 0.0) 
    cout << "In t1 = trline. Default theta not equal to 0. Set to: " 
	 << t1.theta << endl;
  if(t1.freq != 0.0) 
    cout << "In t1 = trline. Default freq not equal to 0. Set to: " 
	 << t1.freq << endl;
  if(t1.size() != 2)
    cout << "In t1 = trline. Size not equal two. Set to: " << t1.size() << endl;
  if((t2.theta != P1)||(t2.freq != P1)||(t2.loss != P1)||(t2.zchar != P2)
     ||(t2.Temp != P1))
    cout << "Copy changed when original was changed following copy operation."
	 << " Values should be:\n"
	 << P1 << P1 << P1 << P2 << P1 << endl
	 << "Values actually are:\n"
	 << t2.theta << t2.freq << t2.loss << t2.zchar << t2.Temp << endl;

  // now set t1 = t2, check values and shadowing yet again:
  t1 = t2;
  if((t2.theta != t1.theta)||(t2.freq != t1.freq)||(t2.loss != t1.loss)
     ||(t2.zchar != t1.zchar)||(t2.Temp != t1.Temp))
    cout << "Operator = failed. Values should be:\n"
	 << t2.theta << t2.freq << t2.loss << t2.zchar << t2.Temp << endl
	 << "Values actually are:\n"
	 << t1.theta << t1.freq << t1.loss << t1.zchar << t1.Temp << endl;
  if(t1.size() != 2)
    cout << "Operator =  size not equal two. Set to: " << t1.size() << endl;
  P1 = 20; P2 = complex(30,40);
  if((t1.theta != P1)||(t1.freq != P1)||(t1.loss != P1)||(t1.zchar != P2)
     ||(t1.Temp != P1))
    cout << "Operator = didn't shadow parameters properly."
	 << " Values should be:\n"
	 << P1 << P1 << P1 << P2 << P1 << endl
	 << "Values actually are:\n"
	 << t1.theta << t1.freq << t1.loss << t1.zchar << t1.Temp << endl;

  //================================================================================
  // COMPARISON TO THE SIMPLE TIME_DELAY CLASS

  time_delay d1;

  double f0 = 5.523 * GHz;	// Just any random frequency.
  parameter Length;
  scaled_real_parameter time(0.5/(f0*Pi), Length);   // convert Length to time

  t1 = trline();           // reset t1 to default values
  t1.set_theta(&Length);  // shadow length
  t1.set_freq(f0);
  d1.time = &time;         // shadow time

  //now t1 and d1 should have identical behaviors:

  int error_flag = 0;
  for(int j = 1; j <= 30; ++j) {
    Length = 20* Degree * j; 
    for(int i = 1; i <= 10; ++i) {
      device::f = i*GHz;
      if(diff(t1.get_data(),d1.get_data())) {
	error_flag = 1;
	break;
      }
    }
    if (error_flag) break;
  }

  if(error_flag)
    cout << "t1 not a pure time delay at frequency " << device::f/GHz << " GHz:" << endl;

  // display sdata values:
  cout << "Length (Degrees): " << Length/Degree
       << "  Freq (GHz): " << device::f/GHz << endl << endl;
  cout << setprecision(10);
  cout << "t1 S:\n" << t1.get_last_data().S << endl
       << "d1 S:\n" << d1.get_last_data().S << endl << endl
       << "t1 C:\n" << t1.get_last_data().C << endl
       << "d1 C:\n" << d1.get_last_data().C << endl << endl
       << "t1 B:\n" << t1.get_last_data().B << endl
       << "d1 B:\n" << d1.get_last_data().B << endl << endl;

  //================================================================================
  // 1/4 AND 1/2 WAVELENGTH TRANSFORMERS

  cout << setprecision(6);
  device::f = 100*GHz;
  Length = 360. * Degree;     // Length = 1 wavelength at device::f

  zterm r1(100*Ohm);             // 1-port termination resistor
  t1 = trline();
  t1.set_zchar(1*Ohm).set_theta(Length/4).set_freq(device::f);    // a 1/4 wave, low impedance line

  cascade c; c.add(t1).add(r1);  // the 1-port circuit

  zdata z0(r1.get_data()), zc(c.get_data());  // the 1/4 wave impedance = 1/r1
  cout << "A 1/4 wave transformer: should invert impedance:" << endl
       << "the original imedance (Ohm):     " << z0.Z/Ohm << endl
       << "the transformed impedance (Ohm): " << zc.Z/Ohm << endl << endl;

  device::f *= 2;                // now a 1/2 wave line
  z0 = zdata(r1.get_data()); zc = zdata(c.get_data());
  cout << "A 1/2 wave transformer: shouldn't change impedance:" << endl
       << "the original imedance (Ohm):     " << z0.Z/Ohm << endl
       << "the transformed impedance (Ohm): " << zc.Z/Ohm << endl << endl;
  cout << "Noise matrices should be equal as well:" << endl
       << "the original (Volt^2):     " << z0.C/(Volt*Volt) << endl
       << "the transformed (Volt^2)): " << zc.C/(Volt*Volt) << endl << endl;

  //================================================================================
  // LOSSY LINE CHECKS

  cout << "\nTest line with 3dB loss per wavelength." << endl;
  device::f = 10. * GHz;
  trline trl;
  trl.zchar = 50. * Ohm;
  trl.freq = 10. * GHz;
  trl.theta = 360. * Degree;
  trl.loss = 0.498812766;       // 3 dB / wavelength
  sdata sd = trl.get_data();
  cout << "Gain = " << sd.SdB(2,1) << " dB (expect -3) " << endl;
  device::f = 20. * GHz;
  sd = trl.get_data();
  cout << "Gain = " << sd.SdB(2,1) << " dB (expect -6) " << endl;
  device::f = 5. * GHz;
  sd = trl.get_data();
  cout << "Gain = " << sd.SdB(2,1) << " dB (expect -1.5) " << endl;

  cout << "All tests completed." << endl;
}

//================================================================================

int diff(const Matrix & A1, const Matrix & A2);
int diff(const Vector & v1, const Vector & v2);

int diff(const sdata & s1, const sdata & s2)
{
  int size = s1.size();
  if (size != s2.size()) return 1;

  return (diff(s1.S,s2.S)||diff(s1.C,s2.C)||diff(s1.B,s2.B));
}

int diff(const Matrix & A1, const Matrix & A2)
{
  if ((A1.Lmaxindex() != A2.Lmaxindex())||(A1.Rmaxindex() != A2.Rmaxindex())) return 1;
  if ((A1.Lminindex() != A2.Lminindex())||(A1.Rminindex() != A2.Rminindex())) return 1;

  int Lmin = A1.Lminindex(), Lmax = A1.Lmaxindex();
  int Rmin = A1.Rmaxindex(), Rmax = A1.Rmaxindex();
  int result = 0;

  for(int i = Lmin; i <= Lmax; ++i) {
    for(int j = Rmin; j <= Rmax; ++j) {
      if (norm(A2[i][j]) > 1e-10) {
	if (norm((A1[i][j] - A2[i][j])/A2[i][j]) > 1e-10)
	  result = 1;
      }
      else if (norm(A1[i][j]) > 1e-9)
	result = 1;
      
      if(result) break;
    }
    if(result) break;
  }

  return result;
}

int diff(const Vector & A1, const Vector & A2)
{
  if ((A1.maxindex() != A2.maxindex())||(A1.minindex() != A2.minindex())) return 1;

  int min = A1.minindex(), max = A1.maxindex();
  int result = 0;

  for(int i = min; i <= max; ++i) {
    if (norm(A2[i]) > 1e-10) {
      if (norm((A1[i] - A2[i])/A2[i]) > 1e-10)
	result = 1;
    }
    else if (norm(A1[i]) > 1e-9)
      result = 1;
    
    if(result) break;
  }

  return result;
}
