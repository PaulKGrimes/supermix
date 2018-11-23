/* BesselFn.h
*
* Written by Phichet Kittara
*
* Definition of BesselFn object for calculating Bessel functions
*
* Used in MultiTone and NewMultitone
*/

/* Changes
*
*    14/04/04 : PKG : Removed "Physics.h" include
*                     Added <vector> and <limits> include
*                     Added private member DOUBLE_PRECISION, set from
*                     STL limits
*/

#ifndef BesselFnH
#define BesselFnH

#include <vector>
#include <limits>

class BesselFn {

public:
  BesselFn();
  ~BesselFn();
  long double Jnx(int n, long double x);
  long double IntegratedJnx(int n, long double x);
  bool setArrayJnx(unsigned int n_last, long double x, std::vector<long double> &J);
  
private:
  long double DOUBLE_PRECISION;

};

#endif
