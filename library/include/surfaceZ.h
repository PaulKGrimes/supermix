// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// **************************************************************************
// surfaceZ.h
//
// Classes for surface impedance calculations
//
// J. Zmuidzinas  9/18/97
// **************************************************************************

#ifndef SURFACEZ_H
#define SURFACEZ_H

#include "SIScmplx.h"
#include "parameter.h"
#include "interpolate.h"

/**************************************************************\
*                                                              *
* class surfimp                                                *
*                                                              *
* An abstract base class for any film that can calculate its   *
* surface impedance. Provides an interface for use in          *
* transmission line calculations that need a surface impedance *
* and a layer thickness (e.g. microstrips)                     *
*                                                              *
\**************************************************************/

class surfimp {
public:

  parameter Thick;     // the layer thickness

  // Return the total thickness of the film
  // defualt behavior: returns Thick; warns and returns 0.0 if Thick < 0
  virtual double thickness();

  // Return the surface impedance of the film at the specified frequency
  // and temperature:
  virtual complex Zsurf(double freq, double Temp) = 0;

  // Virtual destructor is necessary to ensure proper subclass destruction.
  virtual ~surfimp() { }
};


/**************************************************************\
*                                                              *
* class ideal_film                                             *
*                                                              *
* An concrete class that represents the surface impedance of   *
* a perfectly conducting material (zero impedance)             *
*                                                              *
\**************************************************************/

class ideal_film : public surfimp {
public:
  complex Zsurf(double, double) { return complex(0.0); }
};


/**************************************************************\
*                                                              *
* class local_film                                             *
*                                                              *
* An abstract class derived from surfimp representing a film   *
* with the following characteristics:                          *
*  (i)   the film is homogeneous and isotropic                 *
*  (ii)  the conductivity at any point is independent of the   *
*        thickness of the layer and is uniform throughout.     *
*                                                              *
* Derived classes will have to implement sigma(), and may      *
* redefine thickness() if desired, or use the default provided *
*                                                              *
\**************************************************************/
// We treat local conductors (J = sigma x E) in a special way. 
// This is because we want to be able to calculate multilayer stacks
// of metal films (for instance a thin Nb layer on NbTiN) by
// calculating the contribution of each layer independently.
// Not all conductors are local - e.g. good normal metals at low
// temperatures have mean free paths greater than the classical skin
// depth. These cannot be used in our multilayer algorithm, which assumes
// local conductors only.
//
// Since the layer thickness and (local) conductivity are enough to
// calculate the surface impedance of the one layer by itself,
// each primitive layer can be derived from local_film, which
// inherits class surfimp and implements Zsurf() here using its sigma()
// and thichkness().

class local_film : public surfimp { 
public:
  // The "back" side of the film must be terminated with some effective
  // impedance. A standard choice would be the free-space impedance.
  // For thick films, this parameter is irrelevant. For thin films,
  // this parameter simulates the possibility of power loss due to 
  // radiation from the back side of the metal film into free space 
  // (or a dielectric). The difficulty is that the amount of radiation
  // will depend in detail on the shape and size of the conductor.
  // If this parameter turns out to be important in your application,
  // it is advisable to perform a full EM simulation of your structure,
  // using a planar solver such as Sonnet's em or HP Momentum.
  //
  parameter Zterm ;  // nominally set to ZVacuum at construction

  local_film() ;  // constructor needs to set value of Zterm

  // Complex conductivity sigma depends on the physics. This function must
  // be defined in classes derived from local_film
  virtual complex sigma(double freq, double Temp) = 0;

  // Surface impedance is calculated using thickness() and sigma()
  complex Zsurf(double freq, double Temp) ;    // define now    
  
  // Virtual destructor is necessary to ensure proper subclass destruction.
  virtual ~local_film() { }
};


/**************************************************************\
*                                                              *
* class normal_film                                            *
*                                                              *
\**************************************************************/
// A normal_film (normal metal film) is a local_film that
// is calculated using the supplied resistivity parameter rho.
// Since it is a parameter, it may shadow a function that 
// computes a temperature-dependent value.

class normal_film : public local_film {
public:
   parameter rho ;  // resistivity (resistance x length) 

  // if rho is left at the default value of 0.0, then
  // sigma() will be infinite... use ideal_film for a perfect metal

   complex sigma(double, double) { return complex(1./rho, 0.); }
  
  // Virtual destructor is necessary to ensure proper subclass destruction.
  virtual ~normal_film() { }
};


/**************************************************************\
*                                                              *
* class super_film                                             *
*                                                              *
\**************************************************************/
// A super_film (superconducting film)  is a local_film that
// is calculated using the Mattis-Bardeen theory in the local limit.
// This is a concrete class.

class super_film : public local_film {
public:

  // physical superconductor parameters:
  parameter Vgap ; // 2 x gap energy of superconductor (voltage) 
  parameter Tc ;   // Critical temperature 
  parameter rho_normal ;  // normal-state resistivity (resistance x length) 

  // control whether to interpolate (the default) or to calculate directly
  super_film & interpolate()    { interp_flag = true; return *this; }
  super_film & no_interpolate() { interp_flag = false; return *this; }

  // interpolator limits
  parameter tol;    // desired fractional accuracy (default is 1.0e-6)
  parameter maxpts; // maximum number of interpolation points (default is 1000)

  // return the superconducting conductance
  virtual complex sigma(double freq, double Temp) ; // use Mattis-Bardeen 
  
  // Constructor and destructor
  super_film();
  virtual ~super_film() { }

private:
  static const double TOL_MAX ;     // worst tolerance we'll accept (0.01)
  static const double TOL_MIN ;     // best tolerance we'll accept (1.0e-8)
  static const double TOL_DEFAULT ; // the default value set for tol (1.0e-6)

  static const int DEFAULT_MAXPTS ; // what we use if parameter not set (1000)
  static const int START_NPTS ;     // number of points we start with
                                    // before checking tolerance (25)

  // we keep a copy of the values of the parameters
  // used in the interpolation table so that we know if they've changed 
  double Vgap_save ; // 2 x gap energy of superconductor (voltage) 
  double Tc_save ;   // Critical temperature 
  double Temp_save ; // temperature interpolation table used

  // We also need to keep track of the current interpolation table's parameters
  double fmin ;        // frequency lower limit 
  double fmax ;        // upper limit 
  double tol_save  ;   // tolerance target used for interpolation
  
  // This object does the interpolation
  interpolator<complex> sigma_tab;

  // functions to build interpolation
  bool table_init() ;   // initially build table; returns true if an error occurred
  bool table_extend() ; // extend table to include desired frequency

  // some helper data and functions
  double freq_calc;    // the frequency in the call to sigma()
  double Temp_calc;    // the temperature in the call to sigma()
  bool interp_flag;    // true if interpolating to get sigma
  bool initialized ;   // true if interpolation has been built
  bool sanity_check() const;
  complex llsig(double lf) const;

};


/**************************************************************\
*                                                              *
* class layerListNode                                          *
*                                                              *
\**************************************************************/
class layerListNode
{
public:
  layerListNode *next;
  local_film *layer;
};

/**************************************************************\
*                                                              *
* class layerList                                              *
*                                                              *
* layerList holds a list of metal film layers                  *
* and calculates the surface impedance of the multilayer       *
*                                                              *
\**************************************************************/
class layerList : public surfimp
{
private:
  // The head points to the metal layer furthest from the current-
  // carrying surface.
  layerListNode *head;

public:

  parameter Zterm ;  // backside termination, initialized to ZVacuum in constructor

  // Default constructor creates the empty list.
  layerList();

  // Copy constructor creates a complete new copy of the list l.
  layerList(const layerList & l);

  // The destructor must free each node of the list.
  ~layerList();
 
  // Return TRUE if the list is empty.
  int isEmpty();

  // Add a metal film layer
  void addlayer(local_film &lay);

  // Remove a metal film layer
  local_film * remove() ;

  // We must overload the = operator because we use dynamic memory
  layerList& operator=(const layerList&);

  // Return head of list
       layerListNode *gethead() {return head; }

  double thickness() ;                      // define this now 
  complex Zsurf(double freq, double Temp) ; // define this now 

};

//
// Additional work to be done:
//
// - implement anomalous_film class - use nonlocal calculation for surface
//   impedance of a normal metal film.

#endif  /* SURFACEZ_H */




