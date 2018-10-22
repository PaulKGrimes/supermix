// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ********************************************************************
// state_tag.h
//
// State tags are a system to allow devices to remember the operating
// state at which they were last calculated.
//
// The idea is that if a caller asks for a calculation which depends upon
// some external parameters which define a device's operating state, he would
// first set up the parameters and then call state_tag::get_tag() to get a
// new, unique state tag with which to refer to that state. The caller then
// asks a device for its operating data, passing this state_tag to the
// device. In the case of an nport (circuit) element, the caller would use
// nport::get_data(state_tag).
//
// The device will then compare the state_tag passed by the caller to
// its internally saved state_tag; if the two match, then the device
// assumes that the external parameters which define its operating state
// have not changed, so it can quickly return data based on the previous
// operating state calculation without performing a lengthy recalculation.
// If the state_tags do not match (or one of them is undefined), then the
// device assumes that the parameters have changed, requiring a full
// calculation. 
//
// Realize that state tags are not intelligent.  It is up to the user to
// ensure that relevant parts of the system are in the same state each
// time he uses a given state tag.
//
// When a state_tag object is first created, or is reset using the reset()
// member function, then it is tagged as having an "undefined" state. Such
// a state_tag will never match another state_tag (using ==), even if both
// state_tags are undefined. Consequently, the undefined state_tag has a 
// special meaning, i.e. that a recalculation needs to be performed.
// get_tag() will never return an undefined state_tag.
//
//
// examples for using state_tag:
//
//  state_tag s1, s2;
//
//  undefined(s1);   // true, since s1 is newly created
//  s1 == s2;        // false, since undefined state_tags never match
//
//  s1 = state_tag::get_tag();  // now s1 has a unique, defined value
//  s2 = s1;
//  s1 == s2;                   // true, since both are defined and equal
//  s2 = state_tag::get_tag();  // s2 has a new, unique value
//  s1 == s2;                   // false
//
//  s1.reset();      // now we've erased defined value for s1
//  undefined(s1);   // true
//
//
// WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
// state_tag::current_state is a static variable which must be defined and 
// initialized to some small value (like 1).  Since most programs which
// would use the supermix library will have to link to nport.o, the
// initialization is performed there (see nport.cc).  Beware that programs
// that don't link to nport.o and still want to use state_tag must take care
// of the initialization.
// WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
//
// John Ward 4/21/99
// ********************************************************************

#ifndef STATE_TAG_H
#define STATE_TAG_H

class state_tag
{
public:

  // The default constructor sets the state_tag to the undefined state
  state_tag();

  // Call this function to get a unique state_tag.
  static state_tag get_tag();

  // Reset sets the state_tag to undefined, so it will not match
  // another state_tag. This will usually force a full recalculation
  // by a device which cares about state_tags.
  state_tag & reset();

  // Functions to compare the states of two state_tags
  friend bool undefined(const state_tag & s1);
  friend bool operator ==(const state_tag & s1, const state_tag & s2);
  friend bool operator !=(const state_tag & s1, const state_tag & s2);

private:
  enum {UNDEFINED = 0L};  // defines "undefined"

  // The variable "tag" is the actual ID tag that this instance is holding.
  // No class needs to see the ID tag directly, they can only set it by
  // assignment or test it with == or !=.
  unsigned long tag;

  // This counter keeps incrementing every time get_tag() is called.
  static unsigned long current_state;

};

inline state_tag::state_tag() : tag(UNDEFINED) { }

inline state_tag state_tag::get_tag()
{
  state_tag s;
  s.tag = ++current_state;
  return s;
}

inline state_tag & state_tag::reset()
{ tag = UNDEFINED; return *this; }

inline bool undefined(const state_tag & s) { return state_tag::UNDEFINED == s.tag; }

inline bool operator ==(const state_tag & s1, const state_tag & s2)
{
  return (state_tag::UNDEFINED != s1.tag && state_tag::UNDEFINED != s2.tag  
	  && s1.tag == s2.tag);
}

inline bool operator !=(const state_tag & s1, const state_tag & s2)
{ return !(s1 == s2); }

#endif /* STATE_TAG_H */
