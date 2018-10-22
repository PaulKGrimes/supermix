// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ********************************************************************
// num_utility.h
//
// Utility declarations and definitions used by various numerical routines
//
// F. Rice 7/8/99
// ********************************************************************
#ifndef NUM_UTILITY_H
#define NUM_UTILITY_H

#include <bits/stl_function.h>

// ********************************************************************
// A default norm for doubles, consistent with those for other data types.
// Returns the magnitude squared of the argument

inline double norm(double x) { return x*x; }


// ********************************************************************
// the following functional provides access to the overloaded global norm()
// functions, with overload resolution as a template. Useful as an argument
// to something which needs a norm function.
// Usage: default_norm<double> dnorm;  then say:  dnorm(x);

template <typename X>
struct default_norm : public std::unary_function<X,double>
{
  double operator() (const X & x) const { return ::norm(x); }
};


// ********************************************************************
// Here are functions to convert a member function obj.f(x), where "obj"
// is of type "Class", to a form suitable for passing as an argument
// to something which requires a functional or a regular function. The
// resulting argument for use in such a a call would be:
//    member_function(&Class::f, obj)
// Note that the member function f() should take exactly one argument.

template <class S, class T, class A>
inline std::binder1st< std::mem_fun1_ref_t<S,T,A> > 
member_function(S (T::*f)(A), T obj)
{ return std::bind1st(std::mem_fun_ref(f),obj); }

template <class S, class T, class A>
inline std::binder1st< std::const_mem_fun1_ref_t<S,T,A> > 
member_function(S (T::*f)(A) const, T obj)
{ return std::bind1st(std::mem_fun_ref(f), obj); }


#endif /* NUM_UTILITY_H */

