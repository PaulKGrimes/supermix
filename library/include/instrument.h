// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// ********************************************************************
// instrument.h
//
// contains classes: inst_circuit
// ********************************************************************

#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include "circuit.h"
#include "elements.h"
#include <vector>

// ********************************************************************
// class inst_circuit
//
// inst_circuit is used to implement a circuit object which contains
// voltmeter and ampmeter ports; these ports may be accessed to determine
// voltages and currents in the circuit.
//
// ********************************************************************
class inst_circuit : public nport
{
public:

  // constructor:
  //
  // internal_circuit:  the nport managed directly by inst_circuit.  Its first several
  //   ports are for connection to other external circuitry; next are the ports for 
  //   ampmeter connections, finally the ports for voltmeter connections.
  //
  // external_circuit:  the external circuitry to be connected to internal_circuit
  //   when determining the currents or voltages at the instrument ports.
  //
  // num_voltmeters: how many of the instrument ports are for voltage measurements.
  //   the last num_voltmeters ports of internal_circuit will be used for voltmeter
  //   connections; any remaining ports will be used for ampmeter connections. Note
  //   the default value is 0 (no voltmeters), and any supplied value must be
  //   nonnegative.
  //
  // example of port assignments: if internal_circuit has M ports, external_circuit
  // has N ports (N < M), and num_voltmeters is K, with N + K < M, then:
  //
  //  (1) the ports 1 to N of internal_circuit are for connections to the external
  //      circuitry; the constructed inst_circuit object is an nport with N ports
  //      corresponding to ports 1 to N of internal_circuit.  When the voltages or
  //      currents are calculated, these ports will be connected to the corresponding
  //      ports of external_circuit and the resulting combined circuit is calculated.
  //
  //  (2) the ports N+1 to M-K of internal circuit are for ampmeter access. When
  //      the circuit is calculated, these ports are terminated by inst_circuit with
  //      short circuits.
  //
  //  (3) the ports M-K+1 to M of internal circuit are for voltmeter access. When
  //      the circuit is calculated, these ports are terminated by inst_circuit with
  //      open circuits.

  inst_circuit
  (nport & internal_circuit,  nport & external_circuit,  int num_voltmeters = 0);

  // How to properly create ports in your internal_circuit:
  //
  // Use a series_tee object at the point where you need to measure a current.
  // The ampmeter port should be port 1 of this series_tee.  The source of the
  // current should be connected to port 2 of the series_tee, the sink of the
  // current should be connected to port 3 of the series_tee.
  // Using the above connections will result in a positive current reading if the
  // actual current flows from the source to the sink.
  //
  // Use a branch object (with the default 3 ports) where you need to measure
  // a voltage. Since the branch is symmetrical with respect to all its ports,
  // any port of the branch may be used as the voltmeter port.
  //
  // When adding ports to your resulting circuit object (using circuit::add_port()),
  // always add all interface ports first, then add the ampmeter ports, if any, and
  // finally add the voltmeter ports, if any.


  // The following 2 functions access the instrument ports: each function returns a
  // complex_vector of current or voltage readings. Index 1 of the vector corresponds
  // to the value at the first instrument port for reading the given type quantity:

  const Vector & voltages() { return volts = zdata(voltages_c.get_data()).Vs; }
  const Vector & currents() { return amps = ydata(currents_c.get_data()).Is; }


private:
  void recalc() { data = get_data_c.get_data(); }

  int interface_ports, amp_ports, volt_ports;
  nport & int_c, & ext_c;
  std::vector <open_term> voltmeters;
  std::vector <short_term> ampmeters;
  circuit get_data_c, voltages_c, currents_c;
  Vector volts, amps;
};
#endif /* INSTRUMENT_H */
