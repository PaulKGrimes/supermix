// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// **************************************************************************
/**
 * @file error.h
 *
 * Tools to be used for %error handling, such as displaying %error and
 * warning messages.
 *
 * @author John Ward
 * @date September 8, 1997
 */
// ********************************************************************

#ifndef ERROR_H
#define ERROR_H

#include <string>
#include <iostream>

/**
 * Provides useful %error-handling functions.
 *
 * If two arguments are supplied to an %error reporting function,
 * error will output the two arguments separated by a single space.
 *
 * Output will always be terminated by a newline.
 *
 * Output is sent to standard error (cerr) unless redirected
 * to another ostream using error::stream.
 *
 * fatal will always exit gracefully after displaying the
 * message.  warning will not exit unless error::strict is
 * true.  message will not exit at all.
 *
 * message will not display unless error::messages is true.
 *
 * error::strict and error::messages default to false.
 */

class error
{
public:
  /** Boolean to cause warnings to abort if true.  Defaults to false. */
  static bool strict;

  /** Boolean to display messages if true.  Defaults to false. */
  static bool messages;

  /** Always exit after displaying message. */
  static void fatal(const char *);

  /** Always exit after displaying message. */
  static void fatal(const std::string &);

  /** Always exit after displaying message. */
  static void fatal(const char *, const char *);

  /** Always exit after displaying message. */
  static void fatal(const std::string &, const std::string &);

  /**
   * Print an error message and exit if error::strict is true.
   * Otherwise just warn the user.
   */
  static void warning(const char *);

  /**
   * Print an error message and exit if error::strict is true.
   * Otherwise just warn the user.
   */
  static void warning(const std::string &);

  /**
   * Print an error message and exit if error::strict is true.
   * Otherwise just warn the user.
   */
  static void warning(const char *, const char *);

  /**
   * Print an error message and exit if error::strict is true.
   * Otherwise just warn the user.
   */
  static void warning(const std::string &, const std::string &);

  /** Print an informational message if error::messages is nonzero. */
  static void message(const char *);

  /** Print an informational message if error::messages is nonzero. */
  static void message(const std::string &);

  /** Print an informational message if error::messages is nonzero. */
  static void message(const char *, const char *);

  /** Print an informational message if error::messages is nonzero. */
  static void message(const std::string &, const std::string &);

  /**
   * Set the output stream.  Default is cerr.
   * @param s the new message output stream
   * @return the previous value.
   */
  static std::ostream & stream(std::ostream & s)
  { std::ostream *temp = pout; pout = &s; return *temp; }


  /** @return the current error stream */
  static std::ostream & stream()
  { return *pout; }

private:
  /** The output stream for errors.  Defaults to standard error (cerr). */
  static std::ostream *pout;
};

#endif /* ERROR_H */
