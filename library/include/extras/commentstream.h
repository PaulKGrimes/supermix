// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ********************************************************************
/**
 * @file commentstream.h
 *
 * an ofilterstream class which prepends each output line with a
 * comment delimiter
 *
 * @author Frank Rice
 * @date Aug 1, 2001
 *
 */
// ********************************************************************
//                       Class commentstream
//
// This useful class is an ostream which prepends every output line
// a user-supplied string, eg: a comment delimiter such as "# ".
// It forwards its output along to another underlying ostream which
// actually puts the output characters to some device.
//
// Here's an example:
//
// Suppose you want to send output to cout which is a combination of
// data and status information describing some parameters which were
// used to generate the data. You want the status info to be
// identified by a comment prefix on each of the info lines so that
// they cannot be confused with lines containing data. You can easily
// accomplish this using a commentstream as follows:
//
//   commentstream comment(std::cout, "# ");
//   coment << "Status info goes here." << endl;
//   std::cout << "Now data is output here." << endl;
//
// The resulting output (to cout) would be:
//
//   # Status info goes here.
//   Now data is output here.
//
// See the supermix/examples subdirectories for further examples.
//
// ********************************************************************

#ifndef COMMENTSTREAM_H
#define COMMENTSTREAM_H

#include "extras/filterstream.h"
#include <string>

// ********************************************************************
// class commentstream
//
// ofilterstream which prepends each new line with a comment delimiter 
// ********************************************************************
class commentstream : public ofilterstream {
public:
  explicit 
  commentstream(std::ostream & s, const std::string & delimiter);

  // Return a reference to the commentstream's prefix string, which is
  // prepended to all output lines generated by the commentstream. This
  // allows the prefix to be changed as follows (where 'cs' is a
  // commentstream and 'text' is the new string to be used as a prefix):
  // cs.prefix() = text;
  std::string & prefix() { return prefix_ ; } 
  const std::string & prefix() const { return prefix_ ; } 

protected:
  bool filter(char c);   // implementation using ofilterstream

private:
  std::string prefix_ ;  // The comment prefix
  bool need_prefix;      // Next char will go on a new line if true
};


// ******************************************************************
// This templated version of << should ensure that 
// concatenated << operators still see a commentstream and not
// simply a base class of commentstream:

template < typename T >
inline commentstream & 
operator << (commentstream & fs, T arg)
{ *(static_cast<basic_ofilterstream<char>* >(&fs)) << arg; return fs; }


// ******************************************************************
// The following code defines a manipulator and a << operator
// to change the comment prefix of a commentstream. It is used like
// this (where 'cs' is a commentstream and 'text' is the new string
// to be used as a prefix): cs << prefix(text);

namespace internals {
  struct commentstreamchanger {
    std::string prefix;
    commentstreamchanger(const std::string & s) : prefix(s) { }
    void changeprefix(commentstream & cs) { cs.prefix() = prefix; }
  };
}

inline internals::commentstreamchanger prefix(const std::string & s)
{ return internals::commentstreamchanger(s); }

inline commentstream & 
operator << (commentstream & cs, internals::commentstreamchanger f)
{ f.changeprefix(cs); return cs; }


// ******************************************************************
// class commentstream member definitions

inline commentstream::commentstream 
(std::ostream & s, const std::string & delimiter)
  : ofilterstream(s), prefix_(delimiter), need_prefix(true) { }

inline bool commentstream::filter(char c)
{
  if(need_prefix) {
    ps->write(prefix_.c_str(), prefix_.length());
    need_prefix = false; 
  }
  char_out(c);
  if(c == '\n') need_prefix = true;
  return *ps;
}

#endif  /* COMMENTSTREAM_H */
