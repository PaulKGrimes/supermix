// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ********************************************************************
// ioext.h
//
// i/o extensions to make older versions of the C++ i/o stream library
// a little more compatible with the latest C++ standard
// ********************************************************************
#ifndef IOEXT_H
#define IOEXT_H

#include <iomanip>

// Addtional io manipulator(s):

namespace std {

  // return to general (default) floating point format
  // usage example: cout << general;
  inline ios_base& general(ios_base& s)    
  { s.setf(ios_base::fmtflags(0),ios_base::floatfield); return s; }

} // namespace std

#endif  /* IOEXT_H */
