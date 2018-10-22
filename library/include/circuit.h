// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// **************************************************************************
/**
 * @file circuit.h
 *
 * circuit.h holds the classes that connect primitive elements
 * together into complete circuits.
 *
 * @author John Ward
 * @date September 9, 1997
 */
// **************************************************************************

#ifndef CIRCUIT_H
#define CIRCUIT_H

#include "nport.h"
#include "circuitADT.h"
#include "connection.h"
#include "parameter.h"
#include <stack>

// **************************************************************************

/**
 * @class circuit
 *
 * Class circuit creates lists of devices to be connected,
 * how to connect them, and how to arrange the ports of the
 * final device.  It then uses class connection to connect
 * the devices and store the results in its own member data.
 */
class circuit : public nport
{
private:
  /** We need a set of devices to use for connections. */
  nportSet devset;

  /** We need a stack of connections to be performed. */
  portStack constack;

  /**
   * A vector of port labels so that final data will be in desired order.
   * For example, we would want to define labels so that an amplifier gain
   * will be S21 rather than S12.
   */
  portVector labels;

  /**
   * Build the connection tree.
   * Uses lists to create temporary class connection devices.  Since each
   * instance of class connection holds pointers to the devices it is
   * to connect, the connections form a tree.  The tips of the branches
   * are the nports created by the user.  The rest of the tree is connection
   * instances created by this function.
   */
  void build_tree();

  /**
   * Base of the tree of connections.  Calling tree_base->get_data() causes
   * all of the connections to be computed by recursively calling get_data
   * up all of the branches.
   * Remember that data returned by tree_base->get_data() is not necessarily
   * in the proper order desired by the user.
   */
  nport *tree_base;

  /**
   * This boolean remembers if the tree has already been built.
   * We only want to build the tree once, not every time we recalculate.
   */
  bool tree_is_built;

  /** Keep a stack of connection class instances to be freed by destructor. */
  std::stack <connection *> del_stack;

  /**
   * Calculate this circuit's data by performing all connections, etc.
   *
   * @param noise ignore noise if this is false to save time
   */
  void calc(bool noise);

  void recalc()   { calc(true); }
  void recalc_S() { calc(false); }

public:

  /**
   * The temperature of this circuit.  The default shadows device::T.  The
   * temperature is passed to subelements using device::T, so any subelement
   * may override with its own explicit Temp value.
   */
  parameter Temp;

  /**
   * The default constructor initializes with 0 ports and empty lists/stacks.
   */
  circuit();

  /** A destructor is necessary to free "class connection" instances. */
  ~circuit();

  /**
   * Copy a circuit.
   * Most of the lists are copied.  The elements in the lists are *not* copied.
   *
   * @param c the circuit to be copied
   */
  circuit(const circuit & c);

  /**
   * The number of ports this device has.
   *
   * @return the number of ports
   */
  int size() { return labels.len(); }

  /**
   * Noise and source status.
   *
   * @return info about the sdata of the device
   */
  const nport::data_info & get_data_info();

  /**
   * Connect two ports.
   * This is the main function that the user calls to build up the lists of
   * devices and connections to be made.  The integer parameters are the
   * port indices.
   */
  void connect(nport &, int, nport &, int);

  /**
   * Allow the user to specify the order of the ports in the
   * final circuit.  The parameters of device and port index specify a
   * port.  The first time add_port is called specifies the first port
   * and returns the new index to that port, 1.  The second time specifies
   * the second port and returns its new index, 2.  Etc.
   */
  int add_port(nport &, int);

  /**
   * Removes the most recently added port to the circuit.  It
   * returns a port object identifying the port supplied to add_port()
   * for the port just removed. The next port added with a subsequent
   * add_port will be given the index previously assigned to this deleted
   * port. If no calls to add_port() have been made, calling pop_port will
   * result in a fatal error.
   *
   * @return the port most recently added to the circuit
   */
  port pop_port() { tree_is_built = false; return labels.pop(); }

  /**
   * Assignment operator.
   * Most of the lists are copied.  The elements in the lists are *not* copied.
   *
   * @param c the circuit to be copied
   */
  circuit & operator =(const circuit & c);
};

// **************************************************************************

/**
 * @class cascade
 *
 * class cascade is a special type of composite circuit which
 * can only be a 2-port or a 1-port, and can be built only from
 * 2-ports and/or a single 1-port. It implements a simple
 * cascade connection of its component devices. Port 1 of the
 * cascade is assigned to port 1 of the first device added;
 * port 2 of the cascade to port 2 of the last device added,
 * unless the last device added is a 1-port, in which case
 * the cascade is also a 1-port. As long as no 1-port is
 * added to the cascade, 2-ports may continue to be added;
 * adding a 1-port at any time makes the cascade a 1-port and
 * no further devices may be added.  At creation, cascade
 * behaves as a 2-port branch.
 */
class cascade : public data_ptr_nport
{
public:

  /**
   * The temperature of this circuit.  The default shadows device::T.  The
   * temperature is passed to subelements using device::T, so any subelement
   * may override with its own explicit Temp value
   */
  parameter Temp;

  /** The default constructor initializes with a 2-port branch equivalent. */
  cascade();

  /**
   * The number of ports this device has.
   *
   * @return the number of ports
   */
  int size() { return (last) ? last->size() : 2; }

  /**
   * Noise and source status.
   *
   * @return info about the sdata of the device
   */
  const nport::data_info & get_data_info();

  /**
   * This is the main function that the user calls to build up the cascade of
   * devices.
   *
   * @param n the next device to add to the cascade
   */
  cascade & add(nport & n);

  /**
   * A shorthand alias for add().
   *
   * @param n the next device to add to the cascade
   */
  inline cascade & a(nport & n) { return add(n); }

private:

  /** The last component added. */
  nport *last;

  /** The actual circuit. */
  circuit c;

  void recalc();
  void recalc_S();
};

#endif /* CIRCUIT_H */
