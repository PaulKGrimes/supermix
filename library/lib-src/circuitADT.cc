// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
//
// circuitADT.cc

#include "circuitADT.h"
#include "error.h"
#include <iostream>

using namespace std;

nportSet::nportSet()
{
  this->head = 0;
  numnodes = 0;
}

nportSet::nportSet(const nportSet & s)
{
  // Initialize the new list before doing anything else.
  head = 0;
  numnodes = 0;

  nportSetNode *ptr;

  ptr = s.head;

  while(ptr != 0)
  {
    add(ptr->dev);
    ptr = ptr->next;
  }
}

nportSet::~nportSet()
{
  while(numnodes > 0)
    remove(head);
}

nportSetNode *nportSet::find(unsigned long id)
{
  nportSetNode *node = this->head;
  while(node != 0)
  {
    if(node->key == id) return node;
    node = node->next;
  }
  return 0;
}

void nportSet::remove(nportSetNode *node)
{
  if(node != 0)
  {
    if(node->prev != 0)
      node->prev->next = node->next;
    else
      this->head = node->next;

    if(node->next != 0)
      node->next->prev = node->prev;

    delete node;

    numnodes--;
  }
}

int nportSet::len() const
{
  return numnodes;
}

void nportSet::add(nport *n)
{
  // Only add device if it is *not* already in the list.
  if(find(n->id) == 0)
  {
    nportSetNode *node = new nportSetNode;
    node->dev = n;
    node->key = n->id;
    node->next = this->head;
    node->prev = 0;
    this->head = node;
    if(node->next != 0)
      node->next->prev = node;
    numnodes++;
  }
}

nport *nportSet::pop(unsigned long id)
{
  nportSetNode *tmp;
  nport *n;

  tmp = find(id);

  // Found it!
  if(tmp != 0)
  {
    n = tmp->dev;
    remove(tmp);
    return n;
  }

  // Didn't find it.
  return 0;
}

nport *nportSet::pop()
{
  nport *tmpdev;
  nportSetNode *tmpnode;

  // Don't do anything if the list is empty.
  if(numnodes == 0)
    return 0;

  // Pop the list's head off.
  tmpnode = head;
  head = tmpnode->next;
  if(head != 0)
    head->prev = 0;
  numnodes--;

  // Don't lose the head's device, store it in tmpdev
  tmpdev = tmpnode->dev;

  // Delete the old head node.
  delete tmpnode;

  return tmpdev;
}

nportSet& nportSet::operator=(const nportSet & s)
{
  // Beware of self assignment s = s
  if(this != &s)
  {
    // Delete the current nportSet
    while(numnodes > 0)
      remove(head);

    nportSetNode *ptr;

    ptr = s.head;

    while(ptr != 0)
    {
      add(ptr->dev);
      ptr = ptr->next;
    }
  }
  return *this;
}

void nportSet::show()
{
  nportSetNode *ptr = head;

  while(ptr != 0)
  {
    cout << ptr->key << " ";
    ptr = ptr->next;
  }
  cout << endl;
}

tmpList::tmpList()
{
  this->head = 0;
  numnodes = 0;
}

tmpList::tmpList(const tmpList & l)
{
  // Initialize the new list before doing anything else.
  this->head = 0;
  numnodes = 0;

  tmpListNode *ptr;

  ptr = l.head;

  while(ptr != 0)
  {
    add(ptr->dev);
    ptr = ptr->next;
  }
}

tmpList::~tmpList()
{
  while(numnodes > 0)
    remove(head);
}

tmpListNode *tmpList::find(port p)
{
  tmpListNode *node = this->head;
  while(node != 0)
  {
    if(node->dev->get_port(p) != 0) return node;
    node = node->next;
  }
  return 0;
}

void tmpList::remove(tmpListNode *node)
{
  if(node != 0)
  {
    if(node->prev != 0)
      node->prev->next = node->next;
    else
      this->head = node->next;

    if(node->next != 0)
      node->next->prev = node->prev;

    delete node;

    numnodes--;
  }
}

int tmpList::len() const
{
  return numnodes;
}

void tmpList::add(connection *c)
{
  tmpListNode *node = new tmpListNode;
  node->dev = c;
  node->next = this->head;
  node->prev = 0;
  this->head = node;
  if(node->next != 0)
    node->next->prev = node;
  numnodes++;
}

connection *tmpList::pop(port p)
{
  tmpListNode *tmp;
  connection *c;

  tmp = find(p);

  // Found it!
  if(tmp != 0)
  {
    c = tmp->dev;
    remove(tmp);
    return c;
  }

  // Didn't find it.
  return 0;
}

connection *tmpList::pop()
{
  connection *tmpdev;
  tmpListNode *tmpnode;

  // Don't do anything if the list is empty.
  if(numnodes == 0)
    return 0;

  // Pop the list's head off.
  tmpnode = head;
  head = tmpnode->next;
  if(head != 0)
    head->prev = 0;
  numnodes--;

  // Don't lose the head's device, store it in tmpdev
  tmpdev = tmpnode->dev;

  // Delete the old head node.
  delete tmpnode;

  return tmpdev;
}

tmpList& tmpList::operator=(const tmpList & l)
{
  // Beware of self assignment l = l
  if(this != &l)
  {
    // Delete the current tmpList
    while(numnodes > 0)
      remove(head);

    tmpListNode *ptr;

    ptr = l.head;

    while(ptr != 0)
    {
      add(ptr->dev);
      ptr = ptr->next;
    }
  }
  return *this;
}

void tmpList::show()
{
  tmpListNode *ptr = head;

  while(ptr != 0)
  {
    cout << ptr->dev->id << " ";
    ptr = ptr->next;
  }
  cout << endl;
}

portStack::portStack()
{
  this->head = 0;
}

portStack::portStack(const portStack & s)
{
  // Initialize new stack before doing anything else.
  head = 0;

  portStackNode *current;

  current = s.head;

  while(current != 0)
  {
    push(current->cons);
    current = current->next;
  }
}

portStack::~portStack()
{
  while(!isEmpty())
    pop();
}

int portStack::isEmpty()
{
  if(head == 0)
    return TRUE;
  else
    return FALSE;
}

int portStack::isPresent(port p)
{
  portStackNode *current;

  current = head;

  while(current != 0)
  {
    if((current->cons.get(1) == p) || (current->cons.get(2) == p))
      return TRUE;
    current = current->next;
  }

  return FALSE;
}

void portStack::push(portArray c)
{
  if(c.len() == 2)
  {
    portStackNode *node = new portStackNode;
    node->cons = c;
    node->next = this->head;
    this->head = node;
  }
  else
    error::warning("portArray must be of length 2 for portStack::push");
}

portArray portStack::pop()
{
  portArray tmpcons;
  portStackNode *tmpnode;

  // Don't do anything if the stack is empty.
  if(isEmpty())
    error::fatal("Attempt to call portStack::pop for an empty stack.");

  // Pop the stack's head off.
  tmpnode = head;
  head = tmpnode->next;

  // Don't lose the head's portArray, store it in tmparray
  tmpcons = tmpnode->cons;

  // Delete the old head node.
  delete tmpnode;

  return tmpcons;
}

portStack& portStack::operator=(const portStack & s)
{
  // Beware of self assignment s = s
  if(this != &s)
  {
    // Delete the current portStack
    while(!isEmpty())
      pop();

    portStackNode *current;

    current = s.head;

    while(current != 0)
    {
      push(current->cons);
      current = current->next;
    }
  }
  return *this;
}

void portStack::show()
{
  portStackNode *current;

  current = head;

  while(current != 0)
  {
    cout << current->cons.get(1).id << ":";
    cout << current->cons.get(1).index << ", ";
    cout << current->cons.get(2).id << ":";
    cout << current->cons.get(2).index << endl;
    current = current->next;
  }
}

portVector::portVector()
{
  this->head = 0;
  numnodes = 0;
}

portVector::portVector(const portVector & v)
{
  // Initialize the new vector before doing anything else.
  this->head = 0;
  numnodes = 0;

  // Copy the old vector into the new vector.
  for(int i=1; i<=v.len(); i++)
    add(v.get(i));
}

portVector::~portVector()
{
  portVectorNode *tmp;

  while(head !=0)
  {
    tmp = head;
    head = head->next;
    delete tmp;
  }
}

int portVector::len() const
{
  return numnodes;
}

int portVector::isPresent(port p)
{
  portVectorNode *current;

  current = head;

  while(current != 0)
  {
    if(current->label == p)
      return TRUE;
    current = current->next;
  }

  return FALSE;
}

int portVector::add(port p)
{
  numnodes++;
  portVectorNode *node = new portVectorNode;
  node->label = p;
  node->key = numnodes;
  node->next = this->head;
  node->prev = 0;
  this->head = node;
  if(node->next != 0)
    node->next->prev = node;

  return numnodes;
}

port portVector::pop()
{
  if (head == 0)
    // an empty vector
    error::fatal("Attempt to remove a port from an empty list.");

  portVectorNode *node = head;  // the node to be deleted
  port result = node->label;
  head = head->next;
  --numnodes;
  if (head) head->prev = 0;  // only if a node remains
  delete node;
  return result;
}

port portVector::get(int index) const
{
  if((index<1) || (index>numnodes))
  {
    error::warning("Index out of bounds for portVector get(int).");
    return port();
  }

  portVectorNode *tmp;

  tmp = head;

  while(tmp != 0)
  {
    if(tmp->key == index)
      return tmp->label;
    else
      tmp = tmp->next;
  }

  // Didn't find it.
  error::warning("portVector get(int) failed.");
  return port();
}

portVector& portVector::operator=(const portVector & v)
{
  // Beware of self assignment v = v
  if(this != &v)
  {
    portVectorNode *tmp;

    // Delete the current nportSet
    while(head !=0)
    {
      tmp = head;
      head = head->next;
      delete tmp;
    }

    numnodes = 0;

    for(int i=1; i<=v.len(); i++)
      add(v.get(i));
  }
  return *this;
}
