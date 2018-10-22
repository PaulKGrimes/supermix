// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
//
// F. Rice 4/9/99
// ********************************************************************

#include "error.h"
#include <cmath>

// ********************************************************************
// Definitions of functions in class integrator<>

// The constructor definition:
template <class Y>
inline integrator<Y>::integrator(integrator<Y>::type t_) :
  order(4),
  recursion_limit(20),
  abs_tolerance(1.0e-100),
  rel_tolerance(1.0e-6),
  t(t_)
{ }


// --------------------------------------------------------------------
// integration router: operator()

template <class Y> template <class F, class N>
inline Y integrator<Y>::operator()(F f, double a, double b, N norm)
{
  // s will be set true if limits are swapped by qswap. 
  int s = 0;

  // integrate using the appropriate method:
  switch(t) {
  default:
  case CLOSED:
    s = qswap(a,b); romberg_closed(f,a,b,norm,&integrator<Y>::cl); break;
  case OPEN:
    s = qswap(a,b); romberg_open(f,a,b,norm,&integrator<Y>::op); break;
  case SQRTLOWER:
    s = qswap(a,b);
    if(s) romberg_open(f,a,b,norm,&integrator<Y>::su);
    else  romberg_open(f,a,b,norm,&integrator<Y>::sl);
    break;
  case SQRTUPPER:
    s = qswap(a,b);
    if(s) romberg_open(f,a,b,norm,&integrator<Y>::sl);
    else  romberg_open(f,a,b,norm,&integrator<Y>::su);
    break;
  case INFINITE:
    s = qswap(a,b); romberg_open(f,a,b,norm,&integrator<Y>::in); break;
  case EXP:
    romberg_open(f,a,b,norm,&integrator<Y>::ex); break;
  }

  // and return the result:
  if (s) result.first *= -1.0;    // flip sign if limits were swapped
  return result.first;
}

// --------------------------------------------------------------------
// romberg extrapolation routines: romberg_closed() and romberg_open()

template <class Y> template <class F, class N>
inline void integrator<Y>::romberg_closed(F f, double a, double b, N norm,
			  void (integrator<Y>::*method)(F,double,double,int))
{
  double abs_lim = abs_tolerance*abs_tolerance;  // since norm() = magnitude^2
  double rel_lim = rel_tolerance*rel_tolerance;
  double normy, dy;

  s.erase(s.begin(),s.end());
  s.push_back(entry(1.0,Y()));  // s[0] is the only element of s here

  unsigned i = 0;
  while ( i < recursion_limit) {
    (this->*method)(f,a,b,i);   // method result returned in s[i].second
    if(i >= order) {
      // enough points to extrapolate
      result = p(0.0, s.begin()+(i-order), order+1);

      // check error estimate against tolerances
      normy = norm(result.first);
      dy = norm(result.second);
      if (dy <=  abs_lim || (normy != 0.0 && dy/normy <= rel_lim)) return;
    }

    // prepare for next iteration:
    s.push_back(s[i++]);  // here's where we increment i
    s[i].first *= 0.25;   // stepsize decreases by a factor of 2; 2^2 = 4.
  }

  // exiting the loop normally means we didn't achieve accuracy goal
  error::warning("Recursion limit reached in integrator.");
}


template <class Y> template <class F, class N>
inline void integrator<Y>::romberg_open(F f, double a, double b, N norm,
			  void (integrator<Y>::*method)(F,double,double,int))
{
  double abs_lim = abs_tolerance*abs_tolerance;  // since norm() = magnitude^2
  double rel_lim = rel_tolerance*rel_tolerance;
  double normy, dy;

  s.erase(s.begin(),s.end());
  s.push_back(entry(1.0,Y()));  // s[0] is the only element of s here

  unsigned i = 0;
  while ( i < recursion_limit) {
    (this->*method)(f,a,b,i);   // method result returned in s[i].second
    if(i >= order) {
      // enough points to extrapolate
      result = p(0.0, s.begin()+(i-order), order+1);

      // check error estimate against tolerances
      normy = norm(result.first);
      dy = norm(result.second);
      if (dy <=  abs_lim || (normy != 0.0 && dy/normy <= rel_lim)) return;
    }

    // prepare for next iteration:
    s.push_back(s[i++]);  // here's where we increment i
    s[i].first /= 9.0;   // stepsize decreases by a factor of 2; 2^2 = 4.
  }

  // exiting the loop normally means we didn't achieve accuracy goal
  error::warning("Recursion limit reached in integrator.");
}


// --------------------------------------------------------------------
// integration method routines: cl(), op(), etc.

template <class Y> template <class F>
inline void integrator<Y>::cl(F f, double a, double b, int step)
{
  Y & sum = s[step].second;  // the result
  Y & s   = result.first;    // use to hold an intermediate
  if(step) {
    // refine the previous result, which is in sum
    int n = 1;             // the number of points to be added
    for(int j = 1; j < step; ++j) n <<= 1;  // n == 2^(step-1);
    double h = (b-a)/n;    // the interval between the new points (skipping old)
    double x = a+0.5*h;    // x coordinate of new points
    s = f(x);              // will hold the sum of y's at new points
    while((x += h) < b) s += f(x);
    sum += (s *= h);       // add in the new points, scaled by interval
    sum *= 0.5;            // interval between all points is h/2
  }
  else {
    // step == 0, so this is the first iteration
    sum = f(a); sum += f(b); sum *= 0.5*(b-a);
  }
}  


template <class Y> template <class F>
inline void integrator<Y>::op(F f, double a, double b, int step)
{
  Y & sum = s[step].second;  // the result
  Y & s   = result.first;    // use to hold an intermediate
  if(step) {
    // refine the previous result, which is in sum
    int n = 3;             // the number of points to be added
    for(int j = 1; j < step; ++j) n *= 3;  // n == 3^(step);
    double h = (b-a)/n;    // the interval between adjacent new points
    double h2 = 2.0*h;     // skips an existing (old) point
    double x = a+0.5*h;    // x coordinate of new points
    s = f(x);              // will hold the sum of y's at new points
    x += h2; s += f(x);    // skip old left-most point and add another
    while((x += h) < b) { 
      s += f(x);
      x += h2;   // skip old point
      s += f(x);
    }
    sum /= 3.0; sum += (s *= h);
  }
  else {
    // step == 0, so this is the first iteration
    sum = f(0.5*(a+b)); sum *= (b-a);
  }
}


template <class Y> template <class F>
inline void integrator<Y>::sl(F f, double a, double b, int step)
{
  // change limits for the variable change x -> a+x*x
  b = sqrt(b-a);  // assumed lower limit is 0 in code below

  // same algorithm as for op(), only use Func.sql(f,x,a) instead of f(x)
  Y & sum = s[step].second;  // the result
  Y & s   = result.first;    // use to hold an intermediate
  if(step) {
    // refine the previous result, which is in sum
    int n = 3;             // the number of points to be added
    for(int j = 1; j < step; ++j) n *= 3;  // n == 3^(step);
    double h = b/n;        // the interval between adjacent new points
    double h2 = 2.0*h;     // skips an existing (old) point
    double x = 0.5*h;      // x coordinate of new points
    s = Func.sql(f,x,a);            // will hold the sum of y's at new points
    x += h2; s += Func.sql(f,x,a);  // skip old left-most point and add another
    while((x += h) < b) { 
      s += Func.sql(f,x,a);
      x += h2;   // skip old point
      s += Func.sql(f,x,a);
    }
    sum /= 3.0; sum += (s *= h);
  }
  else {
    // step == 0, so this is the first iteration
    sum = Func.sql(f, 0.5*b, a); sum *= b;
  }
}


template <class Y> template <class F>
inline void integrator<Y>::su(F f, double a, double b, int step)
{
  // change limits for the variable change x -> b-x*x
  a = sqrt(b-a);  // a is now UPPER limit; lower limit is 0 in code below

  // same algorithm as for op(), only use Func.squ(f,x,b) instead of f(x)
  Y & sum = s[step].second;  // the result
  Y & s   = result.first;    // use to hold an intermediate
  if(step) {
    // refine the previous result, which is in sum
    int n = 3;             // the number of points to be added
    for(int j = 1; j < step; ++j) n *= 3;  // n == 3^(step);
    double h = a/n;        // the interval between adjacent new points
    double h2 = 2.0*h;     // skips an existing (old) point
    double x = 0.5*h;      // x coordinate of new points
    s = Func.squ(f,x,b);            // will hold the sum of y's at new points
    x += h2; s += Func.squ(f,x,b);  // skip old left-most point and add another
    while((x += h) < a) { 
      s += Func.squ(f,x,b);
      x += h2;   // skip old point
      s += Func.squ(f,x,b);
    }
    sum /= 3.0; sum += (s *= h);
  }
  else {
    // step == 0, so this is the first iteration
    sum = Func.squ(f, 0.5*a, b); sum *= a;
  }
}


template <class Y> template <class F>
inline void integrator<Y>::in(F f, double a, double b, int step)
{
  if (a*b <= 0.0)
    error::fatal("integrator: integration limits must have the same sign"
		 " when using method INFINITE.");

  // change limits to their inverses, for change of variable x -> 1/x:
  double temp = 1.0/b; b = 1.0/a; a = temp;

  // same algorithm as for op(), only use Func.inv(f,x) instead of f(x)
  Y & sum = s[step].second;  // the result
  Y & s   = result.first;    // use to hold an intermediate
  if(step) {
    // refine the previous result, which is in sum
    int n = 3;             // the number of points to be added
    for(int j = 1; j < step; ++j) n *= 3;  // n == 3^(step);
    double h = (b-a)/n;    // the interval between adjacent new points
    double h2 = 2.0*h;     // skips an existing (old) point
    double x = a+0.5*h;    // x coordinate of new points
    s = Func.inv(f,x);            // will hold the sum of y's at new points
    x += h2; s += Func.inv(f,x);  // skip old left-most point and add another
    while((x += h) < b) { 
      s += Func.inv(f,x);
      x += h2;   // skip old point
      s += Func.inv(f,x);
    }
    sum /= 3.0; sum += (s *= h);
  }
  else {
    // step == 0, so this is the first iteration
    sum = Func.inv(f, 0.5*(a+b)); sum *= (b-a);
  }
}


template <class Y> template <class F>
inline void integrator<Y>::ex(F f, double a, double b, int step)
{
  // change limits for the variable change x -> -exp(-x)
  b = ::exp(-a);  // need :: because of exp() member fcn

  //assumed lower limit is 0 in code below

  // same algorithm as for op(), only use Func.exf(f,x) instead of f(x)
  Y & sum = s[step].second;  // the result
  Y & s   = result.first;    // use to hold an intermediate
  if(step) {
    // refine the previous result, which is in sum
    int n = 3;             // the number of points to be added
    for(int j = 1; j < step; ++j) n *= 3;  // n == 3^(step);
    double h = b/n;        // the interval between adjacent new points
    double h2 = 2.0*h;     // skips an existing (old) point
    double x = 0.5*h;      // x coordinate of new points
    s = Func.exf(f,x);            // will hold the sum of y's at new points
    x += h2; s += Func.exf(f,x);  // skip old left-most point and add another
    while((x += h) < b) { 
      s += Func.exf(f,x);
      x += h2;   // skip old point
      s += Func.exf(f,x);
    }
    sum /= 3.0; sum += (s *= h);
  }
  else {
    // step == 0, so this is the first iteration
    sum = Func.exf(f, 0.5*b); sum *= b;
  }
}
 

// ********************************************************************
// Definitions of functions in class CauchyPV

// The constructor definitions:

inline CauchyPV::CauchyPV() : 
  abs_tolerance(integ.abs_tolerance), 
  rel_tolerance(integ.rel_tolerance),
  order(integ.order),
  recursion_limit(integ.recursion_limit),
  integ(I::OPEN) 
{ }

inline CauchyPV::CauchyPV(const CauchyPV & c) : 
  abs_tolerance(integ.abs_tolerance), 
  rel_tolerance(integ.rel_tolerance),
  order(integ.order),
  recursion_limit(integ.recursion_limit),
  integ(c.integ)
{ }


// --------------------------------------------------------------------
// A helper function for CauchyPV calculations

// a function to generate an object of class CauchyPV_calculator<>
// so CauchyPV_calc(f,s)(t) == f(s+t) + f(s-t), thus CauchyPV_calc(f,s)
// may be passed as the function argument to an integrator

template <class F>
inline CauchyPV_calculator<F> CauchyPV_calc(F f, double s) 
{ return CauchyPV_calculator<F>(f,s); }


// --------------------------------------------------------------------
// operator() definition:

template <class F>
inline double CauchyPV::operator()(F f, double a, double s, double b)
{
  double  da = s - a, db = b - s, p = da*db;

  if (p == 0.0) {
    error::warning("CauchyPV: pole singularity is at an integration limit.");
    return 0.0;
  }
  else if (p < 0) {
    // both limits lie to one side of the singularity
    return integ(f, a, b);
  }
  else {
    // do the Cauchy PV integral:
    double ans;
    if (fabs(da) < fabs(db)) {
      ans = integ(CauchyPV_calc(f,s), 0.0, da);
      ans += integ(f, s+da, b);
    }
    else {
      ans = integ(f, a, s-db);
      ans += integ(CauchyPV_calc(f,s), 0.0, db);
    }
    return ans;
  }
}
