// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ************************************************************************
// trlines.h
//
// JZ 9/23/97; FRR 9/17/98; JSW 6/3/99
//
// trlines.h holds classes related to transmission line
// elements used in the linear circuit simulator.
// This includes:
//
//   class dielectric : returns a complex dielectric constant.
//         In general, may be a function of frequency and
//         temperature.
//   class const_diel : A simple class for a dielectric that
//         is frequency and temperature independent. Inherits
//         class dielectric.
//   class trl_base : generic abstract class inherited by all
//         concrete transmission line classes.
//   class microstrip : uses surface impedance classes defined
//         in surfaceZ.h to build real, lossy microstrips
//   class cpw : ideal, lossless coplanar waveguide.
//   class r_waveguide : real, lossy rectangular waveguide.
//   class trline  : a concrete, generic transmission line in which
//         phase length, power loss per wavelength, and impedance
//         are controlled by parameters set by the user
// ************************************************************************
//
// class dielectric
//
// An abstract class from which other classes may be derived, it specifies
// an interface to return a dielectric constant for use in transmission
// line classes:
//   epsilon(freq,Temp);  // returns the complex dielectric constant
//
// NOTE: For a lossy dielectric, the imaginary part < 0. This is because the
// phase of a wave has exp(+I omega t) time dependence, so the propagation
// factor is exp(-I * sqrt(epsilon) * (omega / c) * distance). Since the
// wave must attenuate for increasing distance, we must have
// imag(sqrt(epsilon)) <= 0. The Complex::sqrt() returns a value with the
// same sign for the imaginary part as its argument, so imag(epsilon) <= 0. 
//
// ************************************************************************
//
// class const_diel
//
// A class which provides a concrete implementation of class dielectric, it
// returns the dielectric constant given by parameters eps, tand:
//   eps : real part of dielectric constant
//   tand: loss tangent, such that -eps*tand is the imaginary part
//
// The default values for these parameters are 1 and 0, simulating vacuum.
// See the comments for class dielectric (above) for an explanation of
// why the imaginary part must be <= 0. 
//
// The simplest way to use this class is to assign constant values to
// eps and tand, thus the name "const_diel". A more sophisticated use
// would be to have these parameters shadow some other abstract
// parameters (see abstract_real_parameter.h) which return values
// dependent on device::f and device::T, etc.
//
// example declaration:
//   const_diel quartz(3.78, 0.0001);
//
// ************************************************************************
//
// class trl_base
//
// All concrete transmission line classes in this header inherit trl_base.
// A trl_base object is usable as a class nport object (i.e. can return S
// (scattering) and C (noise correlation) matrices, and the b vector
// (source wave amplitudes).
//
// The philosophy here is that the concrete transmission line classes (such
// as microstrip) are responsible for storing their physical parameters and
// using these to calculate their characteristic impedance and propagation
// constant. The conversion of these quantities to the S and C matrices is
// generic, and can therefore be handled by the trl_base class. Also, we
// insist that all transmission line classes be able to return their
// characteristic impedance and propagation constant to the user, so we
// include virtual interface functions for these in this trl_base class.
//
// All classes derived from trl_base are therefore 2-port circuit elements
// which also provide the following public functions:
//
//   complex Zchar(f,T);  // characteristic impedance at freq f and Temp T
//   complex Kprop(f,T);  // propagation constant at f and T
//
// The sign convention on the propagation constant is that the phase of
// a propagating wave goes as exp(-Kprop*length), so the real part of
// Kprop is POSITIVE for a lossy transmission line.
//
// In addition, the following public static (global) functions are
// provided:
//
//   double trl_base::wavelength(complex k);
//     // returns the wavelength corresponding to a propagation constant k
//
//   double trl_base::percent_loss(complex k);
//     // returns the power loss per wavelength in percent corresponding
//     // to the propagation constant k. Loss requires k.real > 0. 
//  
// ************************************************************************
//
// class microstrip
//
// This is a concrete class representing a lossy microstrip transmission
// line. The materials of the line are represented using class dielectric
// for the dielectric materials and class surfimp for the conductors as
// follows (cross-section through a microstrip is shown):
//
//
//   superstrate()   <-- width -->              sub_thick
//                   _____________              |
//       ____________|///////////|______________v_
//                        |
//   substrate()          top_strip()    ground_plane()
//       _______________________________/_________
//       \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ ^
//                                              |
//
// The user must first create two objects derived from class dielectric to
// describe the substrate and superstrate materials, as well as two objects 
// derived from class surfimp (see surfaceZ.h) to specify the top strip and
// groundplane conductors. These objects are then passed to the microstrip 
// using the member functions:
//
//    superstrate(d1);  // dielectric d1 is the superstrate material
//    substrate(d2);    // dielectric d2 is the substrate material
//    top_strip(c1);    // surfimp (conductor) c1 is the top strip material
//    ground_plane(c2); // surfimp (conductor) c2 is the groundplane
//
// The following member parameters are used to control other
// characteristics of the microstrip:
//  
//    sub_thick;  // the substrate thickness
//    width;      // the top strip line width
//    length;     // the length of the microstrip line
//    Temp;       // the temperature, if different from device::T
//
// The surfimp objects have their own member parameters which control
// the thicknesses of the top strip and groundplane conductors.
//
// The following member functions are provided to examine the electrical
// characteristics of the microstrip, in addition to Zchar() and Kprop(),
// which are described in class trl_base.
//
//    complex Zser(f,T);  // series impedance at freq f and Temp T
//    complex Ypar(f,T);  // parallel admittance at freq f and Temp T
//
// Attempting to calculate the electrical characteristics of a microstrip
// with unphysical or uninitialized properties will result in a fatal
// error, with one exception: if the length of the strip is 0, then its
// 2-port response will be calculated without error, giving an S matrix
// which has no effect, ie:
//
//        0  1           0  0 
//   S =            C = 
//        1  0           0  0
//
// This will be the result returned regardless of the settings of the other
// parameters controlling the microstrip.
//   
// ************************************************************************
//
// class cpw
//
// This is a concrete class representing a coplanar waveguide. Currently
// this class is lossless, noiseless, and assumes that the vacuum
// superstrate is infinitely thick (i.e., that there is no cover plate).
// The materials are represented using class dielectric for the substrate
// material and class surfimp for the conductors as follows (cross-section
// through a cpw is shown):
//
//                   vacuum
//
//            <-- space --><-- width -->   sub_thick    ground plane
//       ______            _____________       |    ___/__
//       /////|____________|///////////|_______v____|/////
//                              |
//          substrate()         top_strip()         bottom_plane()*
//       __________________________________________/______ 
//       \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\  ^  \\\\\\\\                .
//                                             |
//   *The bottom_plane need not be present
//
// The user must first create an object derived from class dielectric to
// describe the substrate material, as well as objects derived from class
// surfimp (see surfaceZ.h) to specify the top conductor and, if required,
// the bottom plane conductor. The ground plane on either side of the
// center top strip will use the same surfimp object as the center
// conductor. These objects are then passed to the cpw object using the
// member functions:
//
//    substrate(d);     // dielectric d is the substrate material
//    top_strip(c1);    // surfimp (conductor) c1 is the top material
//    bottom_plane(c2); // surfimp (conductor) c2 is the bottom plane
//
// If the function bottom_plane() is not called, the cpw will be calculated
// without a conductive backing on the bottom surface of the substrate. If
// called, the cpw will be calculated with a conductive backing.
//
// The following member parameters are used the control other
// characteristics of the cpw line (note that the center conductor is
// centered between the ground planes):
//  
//    sub_thick;  // the substrate thickness
//    width;      // the top strip center conductor line width
//    space;      // the gap between the center conductor each ground plane
//    length;     // the length of the cpw line
//    Temp;       // the temperature, if different from device::T
//
// The surfimp objects have their own member parameters which control
// the thicknesses of the top and bottom conductors.
//
// The following member functions are provided to examine the electrical
// characteristics of the cpw line, in addition to Zchar() and Kprop(),
// which are described in class trl_base.
//
//    complex Zser(f,T);  // series impedance at freq f and Temp T
//    complex Ypar(f,T);  // parallel admittance at freq f and Temp T
//
// Attempting to calculate the electrical characteristics of a cpw line
// with unphysical or uninitialized properties will result in a fatal
// error, with one exception: if the length of the strip is 0, then its
// 2-port response will be calculated without error, giving an S matrix
// which has no effect, ie:
//
//        0  1           0  0 
//   S =            C = 
//        1  0           0  0
//
// This will be the result returned regardless of the settings of the other
// parameters controlling the cpw line.
//
// See Ghione, G. and Naldi, C.: IEEE Transactions on Microwave Theory and
// Techniques, Vol MTT-35, No. 3, March 1987, pp 260 - 267.
//
// ************************************************************************
//
// class r_waveguide
//
// This is a concrete class representing a rectangular waveguide propagating
// the TE10 mode. Currently this class can include real, lossy materials, but
// it assumes the walls of the guide are perfectly smooth (no roughness
// contribution to the loss), and the loss calculations are accurate only
// when they result in the lossy part of the propagation constant << the
// imaginary part, so the calculations may be very inaccurate near cutoff.
// Below cutoff, the losses of the materials are ignored; in particular, the
// characteristic impedance below cutoff will be purely imaginary, as it
// would be for a lossless waveguide.
//
// The following figure shows the definitions of the parameters the user must
// set:
//
//
//                         <----- a ------>   |
//                       //________________// v
//                       /|                |/ 
//              wall() ->/|     fill()     |/ b
//                       /|                |/ 
//                       /|________________|/
//                       //////////////////// ^
//                                            |
//
// Parameter a is assumed to be the long dimension of the guide and
// consequently defines the cutoff frequency.
//
// The user may specify materials to be used in the guide by calling
// member functions fill() and/or wall(). If these functions aren't
// called, vacuum is assumed to fill the guide and the walls are assumed
// to be perfect conductors. If these functions are to be called, the
// user must first create an object derived from class dielectric to pass
// to fill(), and/or an object derived from class surfimp (see surfaceZ.h)
// to pass to wall().
//
//    fill(d);     // dielectric d fills the waveguide
//    wall(c);     // surfimp (conductor) c forms the walls of the guide
//
// The following member parameters are used to set the remaining
// characteristics of the rectangular waveguide:
//  
//    a;       // the long dimension of the guide cross section
//    b;       // the short dimension of the guide cross section
//    length;  // the length of the guide
//    Temp;    // the temperature of the guide (default is device::T)
//
// The following member function is provided to examine the electrical
// characteristics of the waveguide, in addition to Zchar() and Kprop(),
// which are described in class trl_base.
//
//    double  fc(freq,Temp);     // the cutoff frequency of the guide
//    complex Zwave(freq, Temp); // the wave impedance in the guide
//
// Note that fc() is a function of frequency and temperature, since the
// dielectric constant of the fill material may be such a function. The
// value of fc() in this case is not the true cutoff frequency, but the
// cutoff frequency which would result assuming constant dielectric
// properties given by the properties at the specified conditions. This
// is the relevant value when calculating the propagation properties of
// the guide at the given frequency and temperature.
//
// The Zchar() calculation returns the "Zpv" impedance, which relates wave
// power and voltage between the top and bottom surfaces of the guide.
// It is given by Zpv = 2*b/a*Zwave, where Zwave is the wave impedance
// in the guide. If you need another version of the guide impedance,
// consider:
//
//    Zpv = Zchar(); Zvi = Pi/4*Zpv;  Zpi = Pi/4*Zvi
//
// Attempting to calculate the electrical characteristics of a r_waveguide
// with unphysical or uninitialized properties will result in a fatal
// error, with one exception: if the length of the guide is 0, then its
// 2-port response will be calculated without error, giving an S matrix
// which has no effect, ie:
//
//        0  1           0  0 
//   S =            C = 
//        1  0           0  0
//
// This will be the result returned regardless of the settings of the other
// parameters controlling the r_waveguide.
//
// ************************************************************************
//
// class trline
//
// This is a concrete class which can be used to represent any transmission
// line whose electrical characteristics are known or can be caluclated.
//
// The following member parameters are used to set the characteristics of
// the trline:
//  
//   freq;     // frequency for which the phase length is given
//   theta;    // phase length of the line at freq, in radians
//   loss;     // power loss per wavelength (between 0 and 1)
//   zchar;    // characteristic impedance (may be complex)
//   Temp;     // the temperature (shadow device::T by default)
//
// The calculations assume that the phase velocity and loss per wavelength
// are independent of frequency (if the parameters values are held constant).
// Since the phase velocity cannot be determined from the electrical
// parameters above, the calculations assume that the phase velocity has a
// numerical value equal to 2*Pi*freq, so that the wave number == 1.0 for
// frequency == freq. This assumption leads to the following expression for
// the complex propagation constant Kprop as a function of frequency f:
//   Kprop(f) = (f/freq) * ( -ln(1-loss)/(4*Pi) + I );
//
// and a physical length given by:
//   length = theta;
//
// Clearly this means that the function Kprop(f,T) gives results that are
// pretty much meaningless as far as the absolute number returned.
//
// ************************************************************************

#ifndef TRLINES_H
#define TRLINES_H 1

#include "math.h"
#include "global.h"
#include "units.h"
#include "SIScmplx.h"
#include "surfaceZ.h"
#include "parameter.h"
#include "parameter/complex_parameter.h"
#include "nport.h"


// ************************************************************************
class dielectric {
public:
  // This is the entire interface !
  virtual Complex epsilon(double freq, double Temp)=0;
  Complex epsilon() {return epsilon(device::f, device::T);}

  // Virtual destructor is necessary to ensure proper subclass destruction.
  virtual ~dielectric() { } 
};


// ************************************************************************
class const_diel : public dielectric {
public:

  // these parameters are dimensionless
  parameter eps ;    // dielectric constant
  parameter tand ;   // loss tangent
  
  // constructor initializes parameters with constant values:
  const_diel(double e = 1.0, double t = 0.0 ) : eps(e), tand(t) { }

  // The definition of epsilon() is simple: return eps*(1 - I*tand)
  // The -I ensures that wave attenuates as it propagates (recall
  // exp(+I omega t) time dependence, so propagation factor is
  // exp (-I * sqrt(epsilon) * (omega / c) * distance) 
  // const_dielectric ignores the freq and Temp arguments. 
  Complex epsilon(double, double)
    { return eps.get()*Complex(1., -tand.get()); }
  
  // Virtual destructor is necessary to ensure proper subclass destruction.
  virtual ~const_diel() { }
};


// ************************************************************************
class trl_base : public nport
{
public:

  // These interfaces are included because we want all transmission line
  // classes to be able to report these parameters to the user

  // - characteristic impedance
  virtual Complex Zchar(double freq, double Temp) = 0 ;
          Complex Zchar() { return Zchar(device::f, device::T); }

  // - propagation constant
  virtual Complex Kprop(double freq, double Temp) = 0;
          Complex Kprop() { return Kprop(device::f, device::T); }


  // some utility functions to get wavelength and percent loss per wavelength
  // from a propagation constant (note that they are static functions).

  static double wavelength(Complex prop_constant) ;
  static double percent_loss(Complex prop_constant) ;


  // All trl_base devices have 2 ports.
  int size() { return 2; }

  // Constructor creates a sourceless nport object with 2 ports.
  trl_base() : nport(2) { info.source = false; }

  // Virtual destructor is necessary to ensure proper subclass destruction.
  virtual ~trl_base() { }

protected:
  // This is where the S matrix etc. is calculated from the propagation
  // constant and characteristic impedance of the line.
  // Note that it should be called by recalc() in subclasses of trl_base.
  // Note also that if length == 0, then S11 = S22 = 0, S12 = S21 = 1
  // will always be the result, regardless of beta, zchar, znorm.
  void calc(Complex beta, Complex zchar, double length, double znorm) ;


};

// ************************************************************************
class microstrip : public trl_base {
  
public:

  // Microstrip length
  parameter length ;

  // Microstrip width
  parameter width;

  // Substrate thickness
  parameter sub_thick ;

  // Temperature -- shadows device::T by default
  parameter Temp ;

  // Constructor
  microstrip() ;

  // Functions to set the various materials.
  // Note that we pass objects by reference.

  microstrip & substrate(dielectric & d)   // substrate dielectric
    {sub = &d; return *this;}
  microstrip & superstrate(dielectric & d) // superstrate dielectric
    {super = &d; return *this;}
  microstrip & top_strip(surfimp & s)      // top strip surface impedance
    {top = &s; return *this;}
  microstrip & ground_plane(surfimp & s)   // ground plane surface impedance
    {ground = &s; return *this;}

  // functions to access the materials used in the microstrip
  dielectric & substrate()    {return *sub;}
  dielectric & superstrate()  {return *super;}
  surfimp &    top_strip()    { return *top; }
  surfimp &    ground_plane() { return *ground; }

  // Return characteristic impedance and propagation constant
  Complex Kprop(double freq, double T);
  Complex Zchar(double freq, double T);

  // Return series impedance and shunt admittance
  Complex Zser(double freq, double T);
  Complex Ypar(double freq, double T);
  
  // microstrips are "active" noise sources if not at device::T
  const nport::data_info & get_data_info()
  { info.active = (Temp != device::T); return info; }
  
  // Virtual destructor is necessary to ensure proper subclass destruction.
  virtual ~microstrip() { }


protected:
  
  // Pointers to dielectric objects
  dielectric *sub ;  
  dielectric *super ;

  // pointers to surface impedance objects
  surfimp *top ;
  surfimp *ground ;

  // This function manages the recalculation of the microstrip
  void update(double freq, double Temp);

  // These functions perform the numerical calculations and are called
  // by update().
  void mstrip1();  // calculate members of m1
  void mstrip2();  // calculate members of m2
  void mstrip3();  // calculate members of m3 and m4
  void mstrip4();  // calculate members of m4
  void mstrip5();  // calculate zchar, beta, Zs and Yp

  // make sure to call update() before trusting these
  Complex Zs;
  Complex Yp;
  Complex zchar;
  Complex beta ;


protected:

  // The calculations are called here.
  void recalc_S()
    {
      if(length != 0.0) update(device::f,Temp);
      calc(beta, zchar, length, device::Z0);
    }
      
  void recalc()
    { recalc_S(); data.passive_noise(device::f,Temp); }

  // input parameters to calculations 
  struct { double  h, w, t1, esub, tdsub, eup, tdup, freq;
           Complex Esubc, Eupc, Ztop, Zground; }
  i_param;

  // m1 affected only by substrate or top thickness changes
  struct { double tnorm, p, sp, L1, L2; }
  m1;

  // m2 affected only by changes in dielectric constants
  struct { double erel, cosh, b, p2, p4; }
  m2;

  // m3 affected by m1, m2, and top width
  struct { double u, ee0, p1a, p3a, g1; }
  m3;

  // output results from mstrip3() and mstrip4() calculations
  struct { double Z0, epeff, tandel, g2strip, g2ground; }
  m4;
} ;

inline Complex microstrip::Zser(double freq, double T)
{ update(freq,T); return Zs; }
inline Complex microstrip::Ypar(double freq, double T)
{ update(freq,T); return Yp; }
inline Complex microstrip::Kprop(double freq, double T)
{ update(freq,T); return beta; }
inline Complex microstrip::Zchar(double freq, double T)
{ update(freq,T); return zchar; }


// ************************************************************************
class cpw : public trl_base {
  
public:

  // cpw length
  parameter length ;

  // conductor width
  parameter width;

  // conductor to ground spacing
  parameter space;

  // Substrate thickness
  parameter sub_thick ;

  // Temperature -- shadows device::T by default
  parameter Temp ;

  // Constructor
  cpw() ;

  // Functions to set materials:
  // note that we pass objects by reference

  cpw & substrate(dielectric & d)  // substrate dielectric
    {sub = &d; return *this;}
  cpw & top_strip(surfimp & s)      // top strip surface impedance
    {top = &s; return *this;}

  // If the function bottom_plane() is not called, the cpw will be calculated
  // without a conductive backing.  If it is called, the cpw will be calculated
  // with a conductive backing.
  cpw & bottom_plane(surfimp & s)   // conductive backing surface impedance
    {bottom = &s; return *this;}

  // Return characteristic impedance and propagation constant
  Complex Kprop(double freq, double T);
  Complex Zchar(double freq, double T);

  // Return series impedance and shunt admittance
  Complex Zser(double freq, double T);
  Complex Ypar(double freq, double T);
  
  // cpw lines are "active" noise sources if not at device::T
  const nport::data_info & get_data_info()
  { info.active = (Temp != device::T); return info; }
  
  // Virtual destructor is necessary to ensure proper subclass destruction.
  virtual ~cpw() { }

protected:

  // Pointer to dielectric substrate.
  // Superstrate is assumed to be vacuum.
  dielectric *sub ;  

  // Pointer to surface impedance object for conductive plane.
  // If bottom==0, then no conductive backing is used in calculation.
  surfimp *top ;
  surfimp *bottom ;

private:

  // This function recalculates the cpw electrical properties
  void update(double freq, double Temp) ;

  // make sure to call update() before trusting these
  // These variables provide a simple way for update() return multiple values
  Complex zchar ;
  Complex Zs;
  Complex Yp;
  Complex beta ;
  
  // "complete elliptical function ratio"
  // Compute ratio of complete elliptical function of the first kind over
  // the complete elliptical function of the first kind of the complement.
  // Current implementation is an approximation. Called by update().
  double cefr(double);

  // The calculations are called here.
  void recalc_S()
    {
      if(length != 0.0) update(device::f,Temp);
      calc(beta, zchar, length, device::Z0);
    }
      
  void recalc()
    { recalc_S(); data.C = 0.0; }  // noiseless

} ;

inline Complex cpw::Kprop(double freq, double T)
{ update(freq,T); return beta; }
inline Complex cpw::Zchar(double freq, double T)
{ update(freq,T); return zchar; }
inline Complex cpw::Zser(double freq, double T)
{ update(freq,T); return Zs; }
inline Complex cpw::Ypar(double freq, double T)
{ update(freq,T); return Yp; }


// ************************************************************************
class r_waveguide : public trl_base
{
public:

  parameter a;       // waveguide long dimension
  parameter b;       // waveguide short dimension
  parameter length;  // waveguide physical length
  parameter Temp ;   // shadows device::T by default

  // Functions to set materials:
  // note that we pass objects by reference
  r_waveguide & fill(dielectric & d) { fill_ = &d; return *this; }
  r_waveguide & wall(surfimp & s)    { wall_ = &s; return *this; }

  // Return characteristic impedance and propagation constant
  Complex Zwave(double freq, double Temp);
  Complex Zchar(double freq, double Temp);
  Complex Kprop(double freq, double Temp);

  // Return cutoff frequency - generates an error if a <= 0
  double fc(double freq, double Temp);
  double fc() { return fc(device::f,device::T); }

  // Constructor
  r_waveguide()
    : trl_base(), a(0.0), b(0.0), length(0.0), Temp(&device::T),
      fill_(0), wall_(0)
 { }

  // r_waveguides are "active" noise sources if not at device::T
  // and have loss; they are noiseless if lossless
  const nport::data_info & get_data_info();
  
  // Virtual destructor is necessary to ensure proper subclass destruction.
  virtual ~r_waveguide() { }


protected:

  // Pointer to dielectric. If zero, assumes vacuum.
  dielectric *fill_ ;  

  // Pointer to surface impedance object for the walls. If zero,
  // assumes perfectly conducting (no loss).
  surfimp *wall_ ;


private:

  // This function recalculates the electrical properties
  void update(double freq, double T);

  // make sure to call update() before trusting these
  // These variables provide a simple way for update() return multiple values
  Complex zwave ;
  Complex gamma ;

  // The calculations are called here.
  void recalc_S()
    {
      if(length != 0.0) update(device::f, Temp);
      calc(gamma, 2*b/a*zwave, length, device::Z0);
    }
      
  void recalc()
    { recalc_S(); data.C = 0.0; }  // noiseless
};

inline Complex r_waveguide::Kprop(double freq, double T)
{ update(freq, T); return gamma; }
inline Complex r_waveguide::Zwave(double freq, double T)
{ update(freq, T); return zwave; }
inline Complex r_waveguide::Zchar(double freq, double T)
{ return 2.0*b/a * Zwave(freq,T); }  // Zpv version


// ************************************************************************
class trline : public trl_base
{
public:

  parameter         theta;    // phase length of the line at freq
  parameter         freq;     // frequency for which phase length is given
  parameter         loss;     // power loss per wavelength (between 0 and 1)
  complex_parameter zchar;    // characteristic impedance (may be complex)
  parameter         Temp;     // the temperature (shadow device::T by default)


  // member functions to set parameters (either directly or shadowing):

  trline & set_theta( double T ) { theta = T; return *this; }
  trline & set_theta( abstract_real_parameter * pT )
  { theta = pT; return *this; }

  trline & set_freq( double F ) { freq = F; return *this; }
  trline & set_freq( abstract_real_parameter * pF )
  { freq = pF; return *this; }

  trline & set_loss( double L ) { loss = L; return *this; }
  trline & set_loss( abstract_real_parameter * pL )
  { loss = pL; return *this; }

  trline & set_zchar( Complex Z ) { zchar = Z; return *this; }
  trline & set_zchar( abstract_complex_parameter * pZ )
  { zchar = pZ; return *this; }

  trline & set_T(double t) { Temp = t; return *this; }
  trline & set_T(abstract_real_parameter * pt)
  { Temp = pt; return *this; }

  // functions required to be defined by trl_base:
  Complex Zchar(double, double) { return zchar; }
  Complex Kprop(double f, double)
  { return (f/freq)*Complex(-log(1.0-loss)/(4*Pi), 1.0); }
  

  // Constructor gives a lossless, zero-length line
  // The default zchar is device::Z0, Temp is device::T
  trline()
    : trl_base(),
      theta(0), freq(0), loss(0), zchar(&Z0), Temp(&T)
  { }

  // Virtual destructor is necessary to ensure proper subclass destruction.
  virtual ~trline() { }

  // trlines are "active" noise sources if not at device::T
  const nport::data_info & get_data_info()
    {
      info.noise = (loss != 0.0); info.active = (Temp != device::T && loss != 0.0);
      return info;
    }
  

private:

  void recalc_S()
    { calc(Kprop(device::f,device::T), zchar, theta, device::Z0); }

  void recalc()
    { recalc_S(); data.passive_noise(device::f,Temp); }

};

#endif  // TRLINES_H
