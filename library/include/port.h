// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ************************************************************************
/**
 * @file port.h
 *
 * @author John Ward
 * @date September 10, 1997
 */
// ************************************************************************

#ifndef PORT_H
#define PORT_H

/**
 * @class port
 *
 * An instance of port is a unique tag for a port.
 * It is used when connecting two nport devices to keep track
 * of which port gets mapped where.
 *
 * class port needs the relational operator ==, below.
 */
class port
{
public:
  /** The default constructor creates port (0,0). */
  port();
 
  /**
   * @param id the id tag of the device
   * @param index the index of the port
   */
  port(unsigned long id, int index);
 
  /**
   * The id tag for the device where the port originally came from.
   */
  unsigned long id;
 
  /**
   * The index that the port was on the device specified by id.
   */
  int index;
 
};
 
int operator ==(const port &, const port &);

/**
 * The operator > was defined to allow the use of class
 * port with templates.  This was necessitated by a bug in
 * gcc.  Operator > will probably never be used.
 */
int operator >(const port &, const port &);
 

/**
 * @class portArray
 *
 * A simple array of ports, indexed from 1..length.
 * class portArray takes care of dynamic memory allocation and
 * deallocation, provides copy and assignment functions, and
 * handles indexing errors.
 *
 * A nice future improvement might be to overload the []
 * operator.
 */
class portArray
{
private:
  /** The length of the array. */
  int length;

  /** The actual list of ports. */
  port *data;

public:
  /** Create an empty (length 0) array. */
  portArray();

  /** Create an array of length l. */
  portArray(int l);

  /** Copy array p. */
  portArray(const portArray & p);

  /** Free memory in destructor. */
  ~portArray();

  /** @return array length */
  int len();

  /**
   * Set an array element.
   *
   * @param index the index of the element to be set
   * @param value the new value
   */
  void set(int index, port value);

  /** Set all array values to zero. */
  void zero();

  /**
   * Get a port from the array.
   *
   * @param index the index of the element to be returned
   * @return the port
   */
  port get(int index) const;

  /** We must overload the = operator because we use dynamic memory. */
  portArray & operator =(const portArray &);
};

#endif /* PORT_H */
