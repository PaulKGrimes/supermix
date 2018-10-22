// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
//
// circuit.cc

#include "circuit.h"
#include "error.h"

using namespace std;


//**************************************************************
// construction, assignment, and destruction

circuit::circuit() : nport(0), tree_is_built(false), Temp(&T)
{ }


circuit::circuit(const circuit & c) : nport(c), tree_is_built(false), Temp(c.Temp)
{
  devset = c.devset;
  constack = c.constack;
  labels = c.labels;
}


circuit & circuit::operator=(const circuit & c)
{
  // Beware of self assignment: c = c
  if(this != &c)
  {
    Temp = c.Temp;
    devset = c.devset;
    constack = c.constack;
    labels = c.labels;
    tree_is_built = false;
  }
  return *this;
}


circuit::~circuit()
{
  while(!del_stack.empty())
  {
    delete del_stack.top();
    del_stack.pop();
  }
}


//**************************************************************
// managing the data structures

void circuit::connect(nport & dev1, int index1, nport & dev2, int index2)
{
  port port1, port2;
  portArray p(2);

  // Make sure build_tree gets called to include this connection in the tree.
  tree_is_built = false;

  // Make sure port indices are valid.  If not, exit.
  if(dev1.size() == 0 || dev2.size() == 0)
    error::fatal("Can't connect an nport which doesn't know its size in circuit.");
  if(dev1.mode() != Index_1 || dev2.mode() != Index_1)
    error::fatal("Can only connect nports with mode()==Index_1 in circuit.");
  if((index1 < 1) || (index2 < 1) || (index1 > dev1.size()) || (index2 > dev2.size()))
    error::fatal("Illegal port index at circuit::connect.");

  port1 = port(dev1.id, index1);
  port2 = port(dev2.id, index2);

  // Make sure we're not connecting a port to itself
  if (port1 == port2)
    error::fatal("Can't connect a port to itself in circuit.");

  // Make sure neither port is already in connection list.
  if(constack.isPresent(port1) || constack.isPresent(port2))
    error::fatal("Can't connect one port in two places in circuit::connect.");

  // Make sure neither port is already in label list.
  if(labels.isPresent(port1) || labels.isPresent(port2))
    error::fatal("Can't connect one port in two places in circuit::connect.");

  // Create 2-element port array for adding to connection stack.
  p.set(1, port1);
  p.set(2, port2);

  // Add 2-element port array to the connection stack.
  constack.push(p);

  // Add the devices to the device set. The set makes sure they are only added once.
  devset.add(&dev1);
  devset.add(&dev2);
}


int circuit::add_port(nport & dev, int index)
{
  // Make sure build_tree gets called to include this connection in the tree.
  tree_is_built = false;

  // Make sure port index is valid.  If not, exit.
  if(dev.size() == 0)
    error::fatal("Can't add an nport which doesn't know its size in circuit.");
  if(dev.mode() != Index_1)
    error::fatal("Can only add nports with mode()==Index_1 in circuit.");
  if((index < 1) || (index > dev.size()))
    error::fatal("Illegal port index at circuit::add_port.");

  port p(dev.id, index);

  // Make sure port isn't used in connection list.
  if(constack.isPresent(p))
    error::fatal("Port is already used in connection list at circuit::add_port.");

  // Make sure port isn't already in label list.
  if(labels.isPresent(p))
    error::fatal("Port is already used in label list at circuit::add_port.");

  devset.add(&dev);
  return(labels.add(p));
}


void circuit::build_tree()
{
  nportSet devset_copy(devset);
  portStack constack_copy(constack);
  tmpList tmp_devs;
  nport *dev1;
  nport *dev2;
  int index1;
  int index2;
  nport *tmp1;
  nport *tmp2;
  connection *tmp3;

  portArray p;

  // Make sure the stack of temporary connections is empty.
  while(!del_stack.empty())
  {
    delete del_stack.top();
    del_stack.pop();
  }

  // Pop pairs of ports off connection stack until everything is connected.
  while(!constack_copy.isEmpty())
  {
    // Get a pair of ports to connect
    p = constack_copy.pop();

    // Get the first device
    dev1 = devset_copy.pop(p.get(1).id);
    if(dev1 == 0)	// Keep looking.
    {
      dev1 = tmp_devs.pop(p.get(1));
      if(dev1 == 0)
        error::fatal("Device not found in circuit::build_tree.");
    }
    index1 = dev1->get_port(p.get(1));


    // Get the second device
    dev2 = devset_copy.pop(p.get(2).id);
    if(dev2 == 0)	// Keep looking.
    {
      dev2 = tmp_devs.pop(p.get(2));
    }

    // Perform the connection
    if(dev2 == 0)	// Intra-connection
    {
      index2 = dev1->get_port(p.get(2));
      tmp1 = new connection(*dev1, index1, index2);
      del_stack.push(static_cast<connection*>(tmp1));
    }
    else	// Inter-connection
    {
      index2 = dev2->get_port(p.get(2));
      tmp1 = new connection(*dev1, index1, *dev2, index2);
      del_stack.push(static_cast<connection*>(tmp1));
    }

    // Stick the temporary device on the temporary device list.
    tmp_devs.add(static_cast<connection*>(tmp1));
  }

  // Now look for unconnected devices which are block-diagonal components.
  // Connect them in pairs.
  while(devset_copy.len() > 1)
  {
    // they must be added using connection, since circuit will delete them
    tmp1 = devset_copy.pop();
    tmp2 = devset_copy.pop();
    tmp3 = new connection(*tmp1, *tmp2);
    del_stack.push(tmp3);
    tmp_devs.add(tmp3);
  }

  // There could still be a device left.  If so, get it.  We can tell
  // we have one later if tmp1 is non-zero.
  tmp1 = NULL;
  if(devset_copy.len() > 0)
  {
    tmp1 = devset_copy.pop();
  }

  // By now, if there are any devices left in devset_copy, I've made a mistake.
  if(devset_copy.len() > 0)
  {
    error::fatal("Extra devices left on devset_copy in circuit::build_tree!");
  }

  // Make sure tmp1 points to a device before continuing.
  if(tmp1 == NULL)
  {
    if(tmp_devs.len() > 0)
      tmp1 = tmp_devs.pop();
    else
      error::fatal("No devices left after making connections in circuit::build_tree!");
  }

  // Connect remaining devices from tmp_devs list into block-diagonal device.
  while(tmp_devs.len() > 0)
  {
    tmp2 = tmp_devs.pop();
    tmp3 = new connection(*tmp1, *tmp2);
    del_stack.push(tmp3);
    
    tmp1 = tmp3;
  }
  tree_base = tmp1;

  tree_is_built = true;
}


//**************************************************************
// calculating the circuit response


const nport::data_info & circuit::get_data_info()
{
  if(!tree_is_built) build_tree();


  // save the current device::T and set to local Temp
  parameter old_T(device::T);
  device::T = Temp;  // if Temp shadows T, this does nothing

  // get the info and adjust results based on temperature
  info = tree_base->get_data_info();
  if(info.noise && Temp != old_T) info.active = true;

  // Restore device::T and return info
  device::T = old_T;
  return info;
}


void circuit::calc(bool noise)
{
  if(!tree_is_built) build_tree();

  // Verify that the number of ports left equals the size of the labels vector.
  if(tree_base->size() != labels.len())
    error::fatal("circuit::recalc(): Number of ports left after connecting circuit does"
		 " not equal the number of ports specified with circuit::add_port.");

  // save the current device::T and set to local Temp
  parameter old_T(device::T);
  device::T = Temp;  // if Temp shadows T, this does nothing

  // Get the unsorted data. What function we call to get the data
  // depends on whether or not we need tree_base to calculate the noise.
  const sdata& unsorted = (noise) ? tree_base->get_data() : tree_base->get_data_S();
  data.resize(unsorted.size());

  // sort the values into data, using the specified port assignments
  int i,j;
  int index1;
  int index2;
  for(i=1; i<=data.size(); i++)
  {
    index1 = tree_base->get_port(labels.get(i));
    for(j=1; j<=data.size(); j++) 
    {
      index2 = tree_base->get_port(labels.get(j));
      data.S[i][j] = unsorted.S.read(index1, index2);
      if(noise) data.C[i][j] = unsorted.C.read(index1, index2);
    }
    data.B[i] = unsorted.B.read(index1);
  }

  // now adjust normalizing impedance, if necessary:
  data.set_znorm(unsorted.get_znorm());
  data.change_norm(device::Z0);

  // Restore device::T
  device::T = old_T;
}


//**************************************************************

cascade::cascade() : data_ptr_nport(), Temp(&T), last(0)
{
  data.resize(2);
  data.S[1][2] = data.S[2][1] = 1.0;  // the branch equiv.
  data_ptr = &data;
  info.source = info.noise = info.active = false;  // for the branch equiv.
}


void cascade::recalc()
{
  if (last) {
    data.resize(last->size());
    c.Temp = Temp;
    data_ptr = &c.get_data();
  }
}


void cascade::recalc_S()
{
  if (last) {
    data.resize(last->size());
    c.Temp = Temp;
    data_ptr = &c.get_data_S();
  }
}


const nport::data_info & cascade::get_data_info()
{
  if (last) {
    c.Temp = Temp;
    return c.get_data_info();
  }
  else
    return info;
}


cascade & cascade::add(nport & n)
{
  if (last) {
    // not the first device added
    if (last->size() == 1)
      error::fatal("Can't add to a 1-port cascade.");

    switch (n.size()) {

    case 1: {
      c.pop_port();
      c.connect(*last, 2, n, 1);
      last = &n;
      break;
    }

    case 2: {
      c.pop_port();
      c.connect(*last, 2, n, 1);
      c.add_port(n, 2);
      last = &n;
      break;
    }

    default: {
      // improper size for n
      error::fatal("Device added to cascade must have 1 or 2 ports");
      break;
    }

    } //switch

  }
  else {
    // first device added

    switch (n.size()) {

    case 1: {
      c.add_port(n, 1);
      last = &n;
      break;
    }

    case 2: {
      c.add_port(n, 1);
      c.add_port(n, 2);
      last = &n;
      break;
    }

    default: {
      // improper size for n
      error::fatal("Device added to cascade must have 1 or 2 ports");
      break;
    }

    } //switch

  }

  return *this;
}




