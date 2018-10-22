// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// **************************************************************************
/**
 * @file connection.h
 *
 * A connection is a device created by joining nport
 * devices.  It has four  constructors, one for
 * interconnections, one for intraconnections, one for
 * joining two devices without connecting any ports, one for
 * sticking in a single device with no connections.
 *
 * connection is used by the class circuit.
 *
 * A connection is intended to be used only during the
 * construction and calculation of a circuit by the circuit
 * class.
 *
 * A possible improvement would be to make all members 
 * private, and call class circuit a friend.  This would
 * prevent accidental abuse of this class.
 *
 * @author John Ward
 * @date September 8, 1997
 */
// **************************************************************************

#ifndef CONNECTION_H
#define CONNECTION_H

#include "nport.h"

class connection : public nport
{
private:
  /** We need an array of port labels to keep track of what we connected. */
  portArray portmap;

  /** The device(s) to be connected. */
  nport & dev1;

  /** The device(s) to be connected. */
  nport & dev2;

  /** The ports to be connected. */
  int port1;
  int port2;

  /** Type of connection to make */
  enum {INTER, INTRA, BLOCK} connection_type;

  /**
   * Should calc functions calculate noise? This flag is managed
   * by recalc() and recalc_S().
   */
  bool calc_noise;

  /** The actual calculations for an interconnection. */
  void calc_inter();

  /** The actual calculations for an intraconnection. */
  void calc_intra();

  /**
   * The actual calculations for a block connection.  This combines two
   * devices without connecting any ports.
   */
  void calc_block();

  // These call one of the above calc routines.
  void recalc();
  void recalc_S();

public:
  /** Intraconnection constructor */
  connection(nport &, int, int);

  /** Interconnection constructor */
  connection(nport &, int, nport &, int);

  /** Constructor to join two devices without connecting any ports */
  connection(nport &, nport &);

  /**
   * The number of ports this device has.
   *
   * @return the number of ports
   */
  int size() { return data.size(); }

  /** @return noise and source status info about the sdata of the device */
  const nport::data_info & get_data_info();

  /**
   * Return label for a given port index, or the default port (0,0) if
   * index out of range.
   *
   * @param index the index of the port
   * @return the port corresponding to index, or (0,0) if index is not
   * in proper range.
   */
  port get_port(int index);

  /** @return the index for a given port label, or 0 if not found */
  int get_port(port);
};

#endif /* CONNECTION_H */
