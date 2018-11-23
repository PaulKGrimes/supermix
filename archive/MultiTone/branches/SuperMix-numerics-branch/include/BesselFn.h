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
*    01/07/04 : PKG : Changed class to use SuperMix based storage, as part of 
*               program to replace all storage with SuperMix classes, prior to
*               extension to multiple junctions.
*/

#ifndef BesselFnH
#define BesselFnH

#include "supermix.h"
#include <limits>

class BesselFn {

public:
  BesselFn();
  ~BesselFn();
  double Jnx(int n, double x);
  double IntegratedJnx(int n, double x);
  bool setArrayJnx(unsigned int n_last, double x, real_vector &J);

private:
  double DOUBLE_PRECISION;

};

#endif
