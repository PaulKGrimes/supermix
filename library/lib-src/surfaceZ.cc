// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
//
// surfaceZ.cc

#include <iostream>
#include <cmath>
#include "global.h"
#include "units.h"
#include "SIScmplx.h"
#include "parameter.h"
#include "error.h"
#include "supcond.h"
#include "surfaceZ.h"
#include "adaptive.h"

using namespace std;

// **************************************************************
// surfimp definitions

double surfimp::thickness()
{
  if (Thick < 0.0) {
    error::warning("surfimp::Thick < 0; using 0.");
    return 0.0;
  }
  else 
    return Thick.get();
}

// **************************************************************
// local_film definitions

 // use free-space impedance as default backside termination
local_film::local_film() : Zterm(ZVacuum) { }


complex local_film::Zsurf(double freq, double Temp)
{

  // Calculate wave vector in free space, metal; wave impedance in metal
  //
  double k0 = 2.*Pi*freq/cLight ; // k in free space 
  //
  complex factor = sqrt(1. - I*ZVacuum*sigma(freq, Temp)/k0);
  if(imag(factor) > 0.) {          // choose proper branch of sqrt    
    factor *= -1.;                // to make sure wave is attenuated 
  }                                // as it propagates                
  //
  complex k1 = k0*factor;         // k in metal      
  complex Z1 = ZVacuum/factor;    // wave impedance in metal 

  // now calculate surface impedance using transmission-line analogy
  //
  complex gamma = (Zterm - Z1)/(Zterm + Z1); // Termination imped. parameter Zterm
  gamma *= exp(-2.*I*k1*thickness());            // propagation factor 
  return Z1 * (1.+gamma)/(1.-gamma);             // surface impedance 
}


// **************************************************************
// super_film definitions

const double super_film::TOL_MAX = 0.01 ;      // 1% worst case tol
const double super_film::TOL_MIN = 1.e-8 ;     // best case tol
const double super_film::TOL_DEFAULT = 1.e-6 ; // default tol
const int super_film::DEFAULT_MAXPTS = 1000 ;  // 1000 points max
const int super_film::START_NPTS = 25 ;        // 25 points to start

super_film::super_film()
  : Vgap_save(0.0), Tc_save(0.0), Temp_save(0.0), fmin(0.0), fmax(0.0),
    tol_save(0.0), interp_flag(true), initialized(false)
{
  // set defaults
  tol =  TOL_DEFAULT ;
  tol.set_min(TOL_MIN) ;
  tol.set_max(TOL_MAX) ;
  maxpts = DEFAULT_MAXPTS ;
  maxpts.set_min(2*START_NPTS) ;
}


// generate interpolation tables
//
bool super_film::table_init()
{
  // can we use the existing interpolator?
  if(initialized            && 
     Temp_calc == Temp_save &&
     Vgap      == Vgap_save &&
     Tc        == Tc_save   &&
     tol       >= tol_save ) {
    return false;
  }

  else {
    // we rebuild the table...

    // Let's perform a few sanity checks
    if(sanity_check())
      return true;        // uh-oh, some problems...
    
    // OK, let's regenerate the interpolation tables
    sigma_tab.clear().spline();

    fmin = fmax = VoltToFreq * Vgap;
    // just cover 3 orders of magnitude
    fmin /= 100.0;
    fmax *= 10.0;

    tol_save  = tol;
    Temp_save = Temp_calc;
    Vgap_save = Vgap ;
    Tc_save   = Tc;

    // the adaptive table builder setup:
    adaptive<complex> build(sigma_tab);
    build.min_x = log(fmin);   // we'll do log-log interpolation
    build.max_x = log(fmax);
    build.min_points = START_NPTS;
    build.max_points = maxpts;
    build.rel_tolerance = tol_save;
    build.abs_tolerance = tol_save;
    build.recursion_limit = 1000;

    // now adaptively fill the interpolator:
    if(build(member_function(&super_film::llsig, *this))) {
      error::warning("Interpolation for super_film object did not achieve"
		      " desired accuracy. Consider increasing maxpts") ;
    }
    else if (error::messages) {
      error::stream() << "Achieved desired accuracy for super_film" << endl
		      << "with  " << sigma_tab.size() << " points" << endl ;
    }
  }
  initialized = true;
  return false;
}


// extend interpolation tables
//
bool super_film::table_extend()
{
  // can we use the existing interpolator?
  if(fmin <= freq_calc && freq_calc <= fmax) return false;
  
  else {
    // we extend the table...
    
    interpolator<complex> extra_tab;
    double f_low, f_high;
    if(freq_calc < fmin) {
      f_low = freq_calc/10.0; f_high = fmin; fmin = f_low;
      f_low = log(f_low); f_high = log(f_high);
      f_high -= sigma_tab.x(1)-sigma_tab.x(0);
    }
    else {
      f_low = fmax; f_high = freq_calc*10.0; fmax = f_high;
      f_low = log(f_low); f_high = log(f_high);
      unsigned m = sigma_tab.size()-1;
      f_low += sigma_tab.x(m)-sigma_tab.x(m-1);
    }
    
    // the adaptive table builder setup:
    adaptive<complex> build(extra_tab);
    build.min_x = f_low;   // we'll do log-log interpolation
    build.max_x = f_high;
    build.min_points = START_NPTS;
    build.max_points = maxpts;
    build.rel_tolerance = tol_save;
    build.abs_tolerance = tol_save;
    build.recursion_limit = 10;
    
    // now adaptively fill the interpolator extension:
    build(member_function(&super_film::llsig, *this));
    
    // now add the extension to the main interpolator and rebuild it:
    sigma_tab.add(extra_tab);
    sigma_tab.build();
    
    return false;
  }
}


// generate the normalized sigma data for the lookup table
//
complex super_film::llsig(double lf) const
{
  return log(supcond(exp(lf),Temp_calc,Vgap,Tc));
}


// check parameter values
//
bool super_film::sanity_check() const
{
  if(rho_normal <= 0.0) {
    error::warning(
    "rho_normal <= 0 for super_film object");
    return(true) ;         // error exit
  }
  else if(Vgap <= 0.0) {
    error::warning(
    "Vgap <= 0 for super_film object");
    return(true) ;         // error exit
  }
  else if(Tc <= 0.0) {
    error::warning(
    "Tc <= 0 for super_film object");
    return(true) ;         // error exit
  }
  else 
    return(false) ;  // all OK
}

//
// Calculate complex conductivity using interpolation
//
complex super_film::sigma(double freq, double Temp)
{
  // bad argument checks:
  if(freq < 0.) {
    error::warning(
    "Cannot calculate super_film object at negative frequency"
    );
    return(complex(0.)) ;
  }
  if(Temp < 0.) {
    error::warning(
    "Cannot calculate super_film object at negative temperature"
    );
    return(complex(0.)) ;
  }

  // maybe we won't use interpolator:
  if(rho_normal == 0.0) {
    error::warning("super_film object has normal-state resistivity of 0") ;
    return(complex(1./Tiny)) ;  // something huge
  }
  if(!interp_flag || freq == 0.0 || Temp == 0.0)
    return supcond(freq, Temp, Vgap, Tc)/rho_normal;


  // o.k., use the interpolator:
  freq_calc = freq;
  Temp_calc = Temp;
  if(table_init() || table_extend()) // checks parameters and builds table first time through
    // something went wrong...
    return(complex(0.)) ;
  else {
    // all is OK
    return exp(sigma_tab(log(freq)))/rho_normal ;  // doing log-log interpolation
  }
}


// **************************************************************
// layerlist definitions

layerList::layerList()
{
  Zterm = ZVacuum ; // default backside termination
  this->head = 0;
}

layerList::layerList(const layerList & l) 
  : surfimp(static_cast<const surfimp&>(l))
{
  // We're starting with nothing in the list
  head = 0;

  layerListNode *current;

  current = l.head;

  while(current != 0)
  {
    addlayer(*(current->layer));
    current = current->next;
  }
}

layerList::~layerList()
{
  while(!isEmpty())
    remove();
}

int layerList::isEmpty()
{
  if(head == 0)
    return TRUE;
  else
    return FALSE;
}

void layerList::addlayer(local_film &lay)
{
    layerListNode *node = new layerListNode;
    node->layer = &lay;
    node->next = this->head;
    this->head = node;
}

local_film * layerList::remove()
{
  local_film * tmplayer;
  layerListNode *tmpnode;

  // Don't do anything if the stack is empty.
  if(isEmpty())
    error::fatal("Attempt to call layerList::remove for an empty list.");

  // Pop the last layer added.
  tmpnode = head;
  head = tmpnode->next;

  // Don't lose the head's layer pointer
  tmplayer = tmpnode->layer;

  // Delete the old head node.
  delete tmpnode;

  return tmplayer;
}

layerList& layerList::operator=(const layerList & l)
{
  // Beware of self assignment l = l
  if(this != &l)
  {
    // Delete the current layerList
    while(!isEmpty())
      remove();

    layerListNode *current;

    current = l.head;

    while(current != 0)
    {
      addlayer(*(current->layer));
      current = current->next;
    }
  }
  return *this;
}


//
// layerList::Zsurf()
// Calculates surface impedance of a multilayer metal film
// for local conductors
//
complex layerList::Zsurf(double freq, double Temp)
{
  complex sig, factor, k0, k1, Z1, gamA, gamB, Zs ;
  double th ;
//
  layerListNode *current ;

 //
 // Calculate wave vector in free space
 //
   k0 = 2.*Pi*freq/cLight ;       // k in free space 
   //
   // Loop over the layers, starting with outermost layer
   //
   current = head ;
   Zs = Zterm ;  // start with termination imedance paramter Zterm
   //
   while(current != 0) {
       sig = (current->layer)->sigma(freq, Temp); // get conductivity 
       th = (current->layer)->thickness() ; // get thickness 
       //
       // Calculate wave vector, wave impedance in metal
       //
       factor = sqrt(1. - I*ZVacuum*sig/k0);
       if(imag(factor) > 0.) {        // choose proper branch of sqrt    
	 factor *= -1. ;              // to make sure wave is attenuated 
       }                              // as it propagates                
       //
       k1 = k0*factor ;               // k in metal      
       Z1 = ZVacuum/factor ;          // wave impedance in metal 
       //
       // now calculate surface impedance using transmission-line analogy
       //
       gamA = (Zs - Z1)/(Zs + Z1) ;
       gamB = gamA * exp(-2.*I*k1*th) ; // propagation factor in metal 
       Zs = Z1 * (1.+gamB)/(1.-gamB) ;     // surface impedance 
       //
       // next layer...
       current = current->next ;
   }
   return(Zs) ;                        //  that's it ! 
 }

 //
 // layerList::thickness()
 // Calculates total thickness of a multilayer metal film
 //
 double layerList::thickness()
 {
   //
   // Loop over the layers, starting with outermost layer
   //
   layerListNode *current ;
   current = head ;

   double total = 0. ;
   while(current != 0) {
       total += (current->layer)->thickness() ; // add thickness 
       current = current->next ;                // next layer... 
   }
   return(total) ;                              // that's it ! 
 }
