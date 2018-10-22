// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
// **************************************************************
// reduced_nport.h
//
// 10/11/00 FRR
//
// Here is an nport which acts like its parent nport with several ports
// removed. If given an nport which models a circuit, this device simply
// selects specified rows and columns from the parent's S matrix and
// drops the rest. The new, smaller S matrix is returned, along with a
// noise correlation matrix generated from the new S matrix using
// sdata::passive_noise(). The result is a passive, sourceless nport.
//
// The parent nport can be specified in the constructor or with the
// nport() member function.
//
// The ports from the parent which are to be included in the result are
// specified in the constructor and/or with the add_port() member
// function.
//
// Example:
// -------
// Suppose you have a circuit model for a 4-port object, but you only
// need to consider ports 2 and 4 of the model, because the other two
// ports are always terminated by the normalizing impedance device::Z0,
// and you only are concerned with the waves at ports 2 and 4.
//
// circuit a;                // the 4-port model
// reduced_nport b(a,2,4);   // b uses only a's ports 2 and 4
//
// sdata s = b.get_data();   // b calls a.get_data(), and then only keeps
//                           // the data for ports 2 and 4, which it
//                           // returns in the form of a 2-port sdata.
//
// This functionality is not included by supermix.h. You must
// explicitly #include this header file.
// **************************************************************

#ifndef REDUCED_NPORT_H
#define REDUCED_NPORT_H

#include "nport.h"
#include "matmath.h"
#include <list>


class reduced_nport : public nport
{
private:
  typedef list<int> array;
  array ports;
  int size_;
  nport *p;

  bool matrix_reduce(const Matrix & in)
    {
      if (size_ <= 0) return false;
      int min = in.minindex(), max = in.maxindex();

      int i, j;
      array::const_iterator ii, jj;
      for (i = 1, ii = ports.begin(); i <= size_; ++i) {
	int p1 = *(ii++);
	if(p1 < min || p1 > max) return false;
	data.S[i][i] = in[p1][p1];
	for (j = i+1, jj = ii; j <= size_; ++j, ++jj) {
	  int p2 = *jj;
	  data.S[i][j] = in[p1][p2];
	  data.S[j][i] = in[p2][p1];
	}
      }

      return true;
    }

  void recalc() { recalc_S(); data.passive_noise(device::f, device::T); }
  void recalc_S()
    {
      if(!matrix_reduce(p->get_data_S().S))
	error::fatal("Couldn't reduce ports in reduced_nport.");
    }

public:

  // The default constructor creates an uninitialized (size 0) instance.
  reduced_nport() : ports(), size_(0), p(0)
    { info.active = info.source = false; }

  // This constructor takes a parent nport and up to 4 ports to be assigned
  // to the reduced version (A port argument of 0 is ignored). The ports of
  // the result are numbered such that port 1 corresponds to the first port
  // argument, port 2 to the second, etc. If you need more than 4 ports,
  // make add_port() calls following construction to identify the additional
  // ports you need from the parent.
  reduced_nport(nport & ckt, int p1 = 0, int p2 = 0, int p3 = 0, int p4 = 0)
    : ports(), size_(), p(&ckt)
    {
      if (p1 != 0) add_port(p1);
      if (p2 != 0) add_port(p2);
      if (p3 != 0) add_port(p3);
      if (p4 != 0) add_port(p4);
      info.active = info.source = false;
    }

  // Here's how you specify or change the parent nport; it also clears any
  // previous port assignments, so you need to make add_port() calls after
  // calling this function. It returns a reference to its object.
  reduced_nport & nport(nport & ckt)
    {
      p = &ckt;
      size_ = 0;
      ports.erase(ports.begin(),ports.end());
      data.S.make_empty(); data.C.make_empty(); data.B.make_empty();
      return *this;
    } 

  // Here's how you add another port assignment. The argument is a port index
  // of the parent nport; the function returns the index of the reduced S
  // matrix to which this port will correspond. The first port assigned will
  // correspond to index 1, the next to index 2, etc.
  int add_port(int n)
    {
      ports.push_back(n);
      ++size_;
      data.resize(size_);
      return size_;
    }

  int size() { return size_;}

};

#endif /* REDUCED_NPORT_H */




