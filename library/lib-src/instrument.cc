// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
//
// instrument.cc

#include "instrument.h"
#include "error.h"

using namespace std;


inst_circuit::inst_circuit
(nport & internal_circuit,  nport & external_circuit,  int num_voltmeters) :
  nport(external_circuit.size()),
  interface_ports(external_circuit.size()),
  amp_ports(internal_circuit.size() - external_circuit.size() - num_voltmeters),
  volt_ports(num_voltmeters),
  int_c(internal_circuit), ext_c(external_circuit),
  voltmeters(volt_ports),
  ampmeters(amp_ports),
  volts(volt_ports),
  amps(amp_ports)

{
  int i, j;

  // check validity of arguments:
  if(interface_ports <= 0)
    error::fatal("external_circuit must know its size in inst_circuit constructor.");
  if(amp_ports < 0)
    error::fatal("internal_circuit doesn't have enough ports in inst_circuit constructor.");
  if(volt_ports < 0)
    error::fatal("volt_ports < 0 in inst_circuit constructor.");

  // build get_data_c:
  for(i = 1; i <= interface_ports; ++i)
    get_data_c.add_port(int_c, i);
  for(j = 0 ; j < amp_ports; ++i, ++j)   // starts loop at i = interface_ports + 1
    get_data_c.connect(int_c, i, ampmeters[j], 1);
  for(j = 0 ; j < volt_ports; ++i, ++j)  // starts loop at i = interface_ports + amp_ports + 1
    get_data_c.connect(int_c, i, voltmeters[j], 1);

  // build voltages_c:
  for(i = 1; i <= interface_ports; ++i)
    voltages_c.connect(int_c, i, ext_c, i);
  for(j = 0 ; j < amp_ports; ++i, ++j)   // starts loop at i = interface_ports + 1
    voltages_c.connect(int_c, i, ampmeters[j], 1);
  for(j = 0 ; j < volt_ports; ++i, ++j)  // starts loop at i = interface_ports + amp_ports + 1
    voltages_c.add_port(int_c, i);

  // build currents_c:
  for(i = 1; i <= interface_ports; ++i)
    currents_c.connect(int_c, i, ext_c, i);
  for(j = 0 ; j < amp_ports; ++i, ++j)   // starts loop at i = interface_ports + 1
    currents_c.add_port(int_c, i);
  for(j = 0 ; j < volt_ports; ++i, ++j)  // starts loop at i = interface_ports + amp_ports + 1
    currents_c.connect(int_c, i, voltmeters[j], 1);
}
