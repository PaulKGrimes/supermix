// SuperMix version 1.6 C++ source file
// Copyright (c) 1999, 2001, 2004, 2009 California Institute of Technology.
// All rights reserved.
//
// sweeper.cc
// 5/8/09  fixed silly code which assumed number of bits in an int

#include "sweeper.h"
#include "error.h"
#include <algorithm>

using namespace std;

int sweeper::max_points = 100*1000*1000;  // a hundred million points

// Definitions for sweep_parameter
// Note that class sweep_parameter is contained within class sweeper

// no default constructor 

// constructor using startv, stopv, step
sweeper::sweep_parameter::sweep_parameter(real_parameter &rp,
                            double startvp, double stopvp, double stepp) :
  rpr(&rp),
  vecmode(false), 
  startv(0.), 
  stopv(0.), 
  step(0.),
  values(0), 
  num_values(0), 
  index(0),
  wrapped(false) 
{ 
  // need to check parameter values
  if(stepp == 0.) {
    error::warning("Fixing zero step size in sweeper::sweep_parameter() !") ;
    stepp = (stopvp-startvp) ;
  }
  double dtry = (stopvp-startvp)/stepp +1;
  if(dtry < 0.) {
    error::warning(
    "Fixing inconsistent start, stop, and step in sweeper::sweep_parameter()"
                  ) ;
    stepp = -stepp ;
    dtry = -dtry ;
  }
  if(dtry > max_points) {
    error::warning("Fixing very small step in sweeper::sweep_parameter()") ;
    dtry = double(max_points) ;
    stepp = (stopvp-startvp)/(dtry-1) ;
  }
  // All is OK now, so set values
  startv = startvp ;
  stopv = stopvp ;
  step = stepp ;
  num_values = int(dtry) ;
}

// constructor using C-style array of values
sweeper::sweep_parameter::sweep_parameter(real_parameter &rp,
                                          double *valp, int nump) :
  rpr(&rp),
  vecmode(true), 
  startv(0.), 
  stopv(0.), 
  step(0.),
  values(0), 
  num_values(0), 
  index(0),
  wrapped(true) 
{
  if(nump <= 0) {
    error::warning(
    "sweeper::sweep_parameter() -- number of data values in array is <= 0 !"
                  );
  }
  else {
    values.resize(nump) ;    // make enough room
    std::copy(valp, valp+nump, values.begin()) ; // copy into the vector<>
    num_values = values.size() ; // lets ask the vector how many it has
    wrapped = false ;
  }
}

// constructor using SuperMix real_vector
sweeper::sweep_parameter::sweep_parameter(real_parameter &rp,
                                          const real_vector & rvec):
  rpr(&rp),
  vecmode(true), 
  startv(0.), 
  stopv(0.), 
  step(0.),
  values(0), 
  num_values(0), 
  index(0),
  wrapped(true) 
{
  int nump = rvec.maxindex() - rvec.minindex() +1 ;
  if(nump <= 0) {
    error::warning(
    "sweeper::sweep_parameter() -- number of data values in array is <= 0 !"
                  );
  }
  else {
    values.resize(nump) ;    // make enough room
    int imin = rvec.minindex() ;
    for(int i = 0; i < nump; i++)  // copy the vector
      values[i] = rvec.read(i+imin) ;
    num_values = values.size() ; // lets ask the vector how many it has
    wrapped = false ;
  }
}

// constructor using STL vector<double> array
sweeper::sweep_parameter::sweep_parameter(real_parameter &rp,
					  const std::vector<double> & val) :
  rpr(&rp),
  vecmode(true), 
  startv(0.), 
  stopv(0.), 
  step(0.),
  values(val), 
  num_values(val.size()), 
  index(0),
  wrapped(false) 
{
  if(num_values == 0) {
    wrapped = true ;
  }
}

// increment operator
// define postfix form of increment only 
// see Stroustrup 11.11

sweeper::sweep_parameter &
sweeper::sweep_parameter::operator ++(int)
{
  if(num_values <=0) {
    index = 0 ;
    wrapped = true ;
  }
  else {
    wrapped = false ;
    index++ ;
    if(index >= num_values) {
      wrapped = true ;
      index = 0 ;
    }
  }
  *rpr = getval() ; // set the value of the actual parameter
  return(*this) ;
}


// makes sure parameter has the proper value without incrementing
sweeper::sweep_parameter &
sweeper::sweep_parameter::touch()
{
  if(index < num_values)
    *rpr = getval() ; // set the value of the parameter
  return(*this) ;
}

// reset() the index

void sweeper::sweep_parameter::reset()
{
  index = 0 ;
  wrapped = false ;
  if(num_values <=0) {
    wrapped = true ;
  }  
  *rpr = getval() ; // set the value of the actual parameter
  return ;
}

// This function returns the current value of the sweep_parameter

double sweeper::sweep_parameter::getval() const
{
  double result ;
  if(vecmode) {
    result = values[index] ;
  }
  else {
    result = startv + index*step ;
  }
  return(result) ;
}

//**************************************************************************
// Now follow the definitions for class sweeper proper.

sweeper::sweeper() :
  parms(),  // creates an empty list
  alldone(false),
  num_values(1)
{ }

void sweeper::sweep(real_parameter & rp,
		    double startv, double stopv, double step)
{
  parms.push_back(sweep_parameter(rp, startv, stopv, step)) ;
  // now the last element of the list has this latest sweep
  int n = parms.rbegin()->npoints();
  if(n > 1) num_values *= n;
}

void sweeper::sweep(real_parameter & rp, double *valp, int nump)
{
  parms.push_back(sweep_parameter(rp, valp, nump)) ;
  // now the last element of the list has this latest sweep
  int n = parms.rbegin()->npoints();
  if(n > 1) num_values *= n;
}

void sweeper::sweep(real_parameter & rp, const real_vector & rvec)
{
  parms.push_back(sweep_parameter(rp, rvec)) ;
  // now the last element of the list has this latest sweep
  int n = parms.rbegin()->npoints();
  if(n > 1) num_values *= n;
}

void sweeper::sweep(real_parameter & rp, const std::vector<double> & val)
{
  parms.push_back(sweep_parameter(rp, val)) ;
  // now the last element of the list has this latest sweep
  int n = parms.rbegin()->npoints();
  if(n > 1) num_values *= n;
}

void sweeper::initialize(real_parameter & rp, double value)
{
  parms.push_front(sweep_parameter(rp, std::vector<double>(1,value))) ;
}

// define postfix form of increment only 
// see Stroustrup 11.11
sweeper & sweeper::operator ++(int)
{
  std::list<sweeper::sweep_parameter>::iterator ipar = parms.begin() ;
  if(!alldone) {
    bool done = false ;  // done goes true when no more incrementing needed
    while(ipar != parms.end() && !done) {
      (*ipar)++ ;   // postfix increment this parameter
      if(ipar->check_wrap() == false) // this parameter didn't wrap
	done = true ;
      ipar++ ;      // go on to next parameter
    }
    while(ipar != parms.end()) (ipar++)->touch();  // set without incrementing
    if(!done)   // all parameters wrapped this time
      alldone = true ;                 // so we're done sweeping

    // call the setup function now that all parameters are set:
    setup();
  }
  else {
    error::warning(
    "cannot increment sweeper object -- the sweep is finished");
  }
    return(*this) ;
}

void sweeper::reset()
{
  alldone = false ;
  // need to reset all parameters in our list
  for(std::list<sweep_parameter>::iterator ipar = parms.begin() ;
      ipar != parms.end() ; ipar++) {
    ipar->reset() ;
  }

  // call the setup function now that all parameters are set:
  setup();
}
