// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
/** ***********************************************************\
* circuitADT.h                                                 *
*                                                              *
* John Ward, September 10, 1997                                *
\**************************************************************/

#ifndef CIRCUITADT_H
#define CIRCUITADT_H

#include "nport.h"
#include "connection.h"

/** ***********************************************************\
*                                                              *
* class nportSetNode                                           *
*                                                              *
\**************************************************************/
class nportSetNode
{
public:
  nportSetNode *next;
  nportSetNode *prev;
  unsigned long key;
  nport *dev;
};

/** ***********************************************************\
*                                                              *
* class nportSet                                               *
*                                                              *
* nportSet is used to hold the set of all devices used in a    *
* circuit.                                                     *
*                                                              *
\**************************************************************/
class nportSet
{
private:
  // The head points to the first node in the list.
  nportSetNode *head;

  // The number of devices in the set.
  int numnodes;

  // Return a pointer to the node containing the desired device.
  // Find leaves the node in the set.
  // Find returns a 0 if the device isn't in the set.
  nportSetNode *find(unsigned long id);

  // Remove removes the node from the set and deletes it.
  // The device held in the node is not deleted.
  void remove(nportSetNode *n);

public:
  // Default constructor creates the empty set.
  nportSet();

  // Copy constructor creates a complete new copy of the set s.
  // NOTE:  The devices themselves don't get copied!
  nportSet(const nportSet & s);

  // The destructor must free each node of the set.
  // NOTE:  The devices don't get deleted.
  ~nportSet();

  // Return the number of devices in the set.
  int len() const;

  // Add a device to the set.
  // Add has no effect if the device is already in the set.
  void add(nport *n);

  // Remove device with id from the set and return it.
  // Return 0 if device not found.
  nport *pop(unsigned long id);

  // Remove and return the device at the head of the set.
  // Return 0 if list is empty.
  nport *pop();

  // We must overload the = operator because we use dynamic memory
  nportSet& operator=(const nportSet&);

  // Debugging routine.
  void show();
};

/** ***********************************************************\
*                                                              *
* class tmpListNode                                            *
*                                                              *
\**************************************************************/
class tmpListNode
{
public:
  tmpListNode *next;
  tmpListNode *prev;
  connection *dev;
};

/** ***********************************************************\
*                                                              *
* class tmpList                                                *
*                                                              *
* tmpList is used to hold temporary connection devices until   *
* they are needed, after which they must be deleted.           *
*                                                              *
\**************************************************************/
class tmpList
{
private:
  // The head points to the first node in the list.
  tmpListNode *head;

  // The number of devices in the list.
  int numnodes;

  // Return a pointer to the node containing the desired device.
  // Find leaves the node in the list.
  // Find returns a 0 if the device isn't in the list.
  tmpListNode *find(port p);

  // Remove removes the node from the list and deletes it.
  void remove(tmpListNode *n);

public:
  // Default constructor creates the empty list.
  tmpList();

  // Copy constructor creates a complete new copy of the list l.
  // NOTE:  The devices themselves don't get copied!
  tmpList(const tmpList & l);

  // The destructor must free each node of the list.
  // NOTE:  The devices don't get deleted.
  ~tmpList();

  // Return the number of devices in the list.
  int len() const;

  // Add a device to the list.
  // Device is added even if it is already in the list.
  void add(connection *c);

  // Remove device with port from the list and return it.
  // Return 0 if device not found.
  connection *pop(port p);

  // Remove and return the device at the head of the list.
  // Return 0 if list is empty.
  connection *pop();

  // We must overload the = operator because we use dynamic memory
  tmpList& operator=(const tmpList&);

  // Debugging routine.
  void show();
};

/** ***********************************************************\
*                                                              *
* class portStackNode                                          *
*                                                              *
\**************************************************************/
class portStackNode
{
public:
  portStackNode *next;
  portArray cons;
};

/** ***********************************************************\
*                                                              *
* class portStack                                              *
*                                                              *
* portStack is used to hold a stack of connections to make.    *
*                                                              *
\**************************************************************/
class portStack
{
private:
  // The head points to the first node in the stack.
  portStackNode *head;

public:
  // Default constructor creates the empty stack.
  portStack();

  // Copy constructor creates a complete new copy of the stack s.
  portStack(const portStack & s);

  // The destructor must free each node of the stack.
  ~portStack();

  // Return TRUE if the stack is empty.
  int isEmpty();

  // Return TRUE if the port is already in the stack.
  int isPresent(port);

  // Add a pair of ports to connect to the stack.
  // push fails if portArray is not of length 2.
  void push(portArray);

  // Remove a pair of ports to connect from the stack.
  // Throw a fatal error if the stack is empty.
  portArray pop();

  // We must overload the = operator because we use dynamic memory
  portStack& operator=(const portStack&);

  // Debugging routine.
  void show();
};

/** ***********************************************************\
*                                                              *
* class portVectorNode                                         *
*                                                              *
\**************************************************************/
class portVectorNode
{
public:
  portVectorNode *next;
  portVectorNode *prev;
  int key;
  port label;
};

/** ***********************************************************\
*                                                              *
* class portVector                                             *
*                                                              *
* portVector is used to keep track of the order that the       *
* ports of a circuit are to be assigned after the circuit      *
* is calculated.                                               *
*                                                              *
\**************************************************************/
class portVector
{
private:
  // The head points to the first node in the list.
  portVectorNode *head;

  // The number of ports in the list.
  int numnodes;

public:
  // Default constructor creates an empty list.
  portVector();

  // Copy constructor creates a complete new copy of the vector v.
  portVector(const portVector & v);

  // The destructor must free each node of the list.
  ~portVector();

  // Return the number of ports in the list.
  int len() const;

  // Return TRUE if the port is already in the list.
  int isPresent(port);

  // Add a port to the list.
  // Vector will increase in size, and the port will get added to the end.
  int add(port p);

  // Remove the most recently added port. Returns the port just removed.
  // It is a fatal error to attempt a pop from an empty portVector
  port pop();

  // Return port with this index.  Port stays in list.
  port get(int index) const;

  // We must overload the = operator because we use dynamic memory
  portVector & operator=(const portVector &);
};

#endif /* CIRCUITADT_H */
