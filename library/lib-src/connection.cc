// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
//
// connection.cc

#include "connection.h"
#include "error.h"

using namespace std;

//**************************************************************
// intraconnection within a single device

connection::connection(nport & d, int p1, int p2)
  : dev1(d), dev2(*this), port1(p1), port2(p2), connection_type(INTRA)
{
  int devsize = dev1.size();

  // Device should have at least 3 ports
  if(devsize < 3)
    error::fatal("Device must have at least 3 ports for intraconnection.");
 
  // Make sure the ports exist before we connect them.
  if((port1 < 1) || (port1 > devsize)
     || (port2 < 1) || (port2 > devsize))
    error::fatal("Illegal port index for intraconnection.");
 
  // Create data for the new device with the appropriate number of ports.
  data.resize(devsize - 2);  // o.k., since dev1.size() >= 3
  portmap = portArray(data.size());

  // Create the port map.
  // i is the index on the old device, a is the index for the new device.
  int a=0;
  for(int i=1; i<=devsize; ++i)
    if((i != port1) && (i != port2))
    {
      ++a;
      portmap.set(a, dev1.get_port(i));
    }
}
 
void connection::calc_intra()
{
  // We will access raw data since it's faster.
  /*register*/ int k = port1;
  /*register*/ int l = port2;

  // Get the data from the device
  info = dev1.get_data_info();
  const sdata& devdata = (info.noise && info.active && calc_noise) ?
    dev1.get_data() : dev1.get_data_S();

  int devdatasize = devdata.size();
  data.set_znorm(devdata.get_znorm());

  /*register*/ int j;  // Inner loop index variable should be registered for speed.
  /*register*/ int i;
  int a, b;

  // Reduce the amount of pointer arithmetic in the loops.
  complex const *const *s = & devdata.S[0];
  complex const *sk = s[k];
  complex const *sl = s[l];
  complex skl = 1.0 - sk[l];
  complex slk = 1.0 - sl[k];
  complex skk = sk[k];
  complex sll = sl[l];

  complex denom = skl*slk - skk*sll;
  if (zabs(denom) < Tiny) denom = Tiny;
  denom = 1.0/denom;  // We don't want to have to divide in inner loops.

  complex x, num;

  // Calculate S matrix
  a = 1;
  for(i=1; i<=devdatasize; ++i) if(i != k && i != l)
  {
    complex const *si = s[i];
    complex sil = si[l];
    complex sik = si[k];
    complex tmp1 = sil*slk + sll*sik;
    complex tmp2 = sik*skl + skk*sil;
    b = 1;
    for(j=1; j<=devdatasize; ++j) if(j != k && j != l)
    {
      num = (sk[j] * tmp1) + (sl[j] * tmp2);
      data.S[a][b] = si[j] + (num*denom);
      ++b;
    }
    ++a;
  }
 
  // Calculate C matrix
  if(calc_noise) {
    if(!info.noise) {
      // device is noiseless:
      data.C = 0.0;
    }
    else if(info.active) {
      // need the full noise matrix calculation:
      complex const *const *c = & devdata.C[0];
      complex const *ck = c[k];
      complex const *cl = c[l];
      complex denom1 = zmagsq(denom);

      a = 1;
      for(i=1; i<=devdatasize; ++i) if(i != k && i != l) {
	complex const *ci = c[i];
	complex const *si = s[i];
	complex sil = si[l];
	complex sik = si[k];
	complex tmp1 = sil*slk + sll*sik;
	complex tmp2 = sik*skl + skk*sil;
	b = 1;
	for(j=1; j<=devdatasize; ++j) if(j != k && j != l) {
	  complex tmp3 = zconj(s[j][l]*slk+sll*s[j][k]);
	  complex tmp4 = zconj(s[j][k]*skl+skk*s[j][l]);
	  x = tmp1 * (ck[l]* tmp4 + ck[k]* tmp3);
	  x += tmp2 * (cl[k]* tmp3 + cl[l]* tmp4);
	  x *= denom1;
	  x += ci[j];
	  x += (cl[j]* tmp2 + ck[j]* tmp1) * denom;
	  x += (ci[l]* tmp4 + ci[k]* tmp3) * zconj(denom);
	  data.C[a][b] = x;
	  ++b;
	}
	++a;
      }
    }

    else {
      // passive noise calculation is sufficient:
      data.passive_noise(device::f, device::T);
    }
  }

  // Calculate B (source) vector
  if(info.source) {
    complex const *const bs = & devdata.B[0];
    a = 1;
    for(i=1; i<=devdatasize; ++i) if(i != k && i != l)
      {
	complex const *si = s[i];
	x = bs[i];
	x += bs[l]*(si[k]*skl+skk*si[l])*denom;
	x += bs[k]*(si[l]*slk+sll*si[k])*denom; 
	data.B[a] = x;
	++a;
      }
  }
  else
    data.B = 0.0;

  // Finally set up normalizing impedance to device::Z0 (or 0, if input z_norm == 0)
  data.change_norm(device::Z0);

}


//**************************************************************
// interconnection between two devices

connection::connection(nport& d1, int p1, nport& d2, int p2)
 : dev1(d1), dev2(d2), port1(p1), port2(p2), connection_type(INTER)
{
  // Since we access sizes often, store them locally
  int dev1size = dev1.size();
  int dev2size = dev2.size();

  // Don't connect two one-ports.
  if((dev1size <= 1) && (dev2size <= 1))
    error::fatal("Connecting two one-port devices yields a null matrix.");

  // Make sure the ports exist before we connect them.
  if((port1 < 1) || (port1 > dev1size)
     || (port2 < 1) || (port2 > dev2size))
    error::fatal("Illegal port index for interconnection.");

  // Create data for the new device with the appropriate number of ports.
  data.resize(dev1size + dev2size - 2);
  portmap = portArray(data.size());

  // Create the port map.
  int a=0;
  int i;
  for(i=1; i<=dev1size; ++i)
    if(i != port1)
    {
      ++a;
      portmap.set(a, dev1.get_port(i));
    }
  for(i=1; i<=dev2size; ++i)
    if(i != port2)
    {
      ++a;
      portmap.set(a, dev2.get_port(i));
    }
}

void connection::calc_inter()
{

  // We will access raw data since it's faster.
  /*register*/ int k = port1;
  /*register*/ int l = port2;
  sdata *pd1 = 0, *pd2 = 0;   // we'll use these only if renormalization needed

  // Get the data from dev1 and dev2
  const data_info & info1 = dev1.get_data_info();
  const data_info & info2 = dev2.get_data_info();
  const sdata& d1ref =  (calc_noise && (info1.active || info2.active)) ?
    dev1.get_data() : dev1.get_data_S();
  const sdata& d2ref =  (calc_noise && (info1.active || info2.active)) ?
    dev2.get_data() : dev2.get_data_S();

  // set up normalizing impedance and the references data1 and data2
  data.set_znorm(device::Z0);
  if(d1ref.get_znorm() != device::Z0 && d1ref.get_znorm() != 0.0)
    pd1 = new sdata(d1ref, device::Z0);
  if(d2ref.get_znorm() != device::Z0 && d2ref.get_znorm() != 0.0)
    pd2 = new sdata(d2ref, device::Z0);

  const sdata& data1 = (pd1) ? *pd1 : d1ref;
  const sdata& data2 = (pd2) ? *pd2 : d2ref;

  // Since we access sizes often, store them locally
  int data1size = data1.size();
  int data2size = data2.size();

  /*register*/ int j;
  /*register*/ int i;
  int a, b;
 
  // Reduce the amount of pointer arithmetic in the loops.
  complex const *const *s = & data1.S[0];
  complex const *const *t = & data2.S[0];
  complex skk = s[k][k];
  complex tll = t[l][l];

  complex denom = 1 - skk*tll;
  if (zabs(denom) < Tiny) denom = Tiny;
  denom = 1.0/denom;  // We don't want to have to divide in inner loops.

  complex x;


  // Calculate S matrix, broken up into 4 submatrices
  a = 1;
  for(i=1; i<=data1size; ++i) if(i != k)
  {
    b = 1;
    for(j=1; j<=data1size; ++j) if(j != k)
    {
      data.S[a][b] = s[i][j] + s[k][j]*tll*s[i][k]*denom;
      ++b;
    }
    ++a;
  }

  a = data1size;
  for(i=1; i<=data2size; ++i) if(i != l)
  {
    b = data1size;
    for(j=1; j<=data2size; ++j) if(j != l)
    {
      data.S[a][b] = t[i][j] + t[l][j]*skk*t[i][l]*denom;
      ++b;
    }
    ++a;
  }

  a = 1;
  for(i=1; i<=data1size; ++i) if(i != k)
  {
    b = data1size;
    for(j=1; j<=data2size; ++j) if(j != l)
    {
      data.S[a][b] = t[l][j]*s[i][k]*denom;
      ++b;
    }
    ++a;
  }

  a = data1size;
  for(i=1; i<=data2size; ++i) if(i != l)
  {
    b = 1;
    for(j=1; j<=data1size; ++j) if(j != k)
    {
      data.S[a][b] =  s[k][j]*t[i][l]*denom;
      ++b;
    }
    ++a;
  }

  // Calculate C matrix
  if(calc_noise) {
    if(!info1.noise && !info2.noise) {
      // both devices are noiseless
      data.C = 0.0;
    }
    else if(!info1.active && !info2.active) {
      // both devices are passive
      data.passive_noise(device::f, device::T);
    }
    // here at least one device has noise and at least one is active:
    else if(info1.noise && info2.noise) {
      // full noise matrix calculation required
      complex const *const *c = & data1.C[0];
      complex const *const *u = & data2.C[0];
      complex ckk = c[k][k];
      complex ull = u[l][l];

      complex denom1 = zmagsq(denom);

      a = 1;
      for(i=1; i<=data1size; ++i) if(i != k)
	{
	  b = 1;
	  for(j=1; j<=data1size; ++j) if(j != k)
	    {
	      x = c[i][j] + c[i][k]*zconj(s[j][k]*tll*denom);
	      x += c[k][j]*s[i][k]*tll*denom;
	      x += (ull + zmagsq(tll)* ckk) *
		s[i][k]*zconj(s[j][k])*denom1;
	      data.C[a][b] = x;
	      ++b;
	    }
	  ++a;
	}

      a = data1size;
      for(i=1; i<=data2size; ++i) if(i != l)
	{
	  b = data1size;
	  for(j=1; j<=data2size; ++j) if(j != l)
	    {
	      x = u[i][j] + u[i][l]*zconj(t[j][l]*skk*denom);
	      x += u[l][j]*t[i][l]*skk*denom;
	      x += (ckk + zmagsq(skk)* ull) *
		t[i][l]*zconj(t[j][l])*denom1;
	      data.C[a][b] = x;
	      ++b;
	    }
	  ++a;
	}
      
      a = 1;
      for(i=1; i<=data1size; ++i) if(i != k)
	{
	  b = data1size;
	  for(j=1; j<=data2size; ++j) if(j != l)
	    {
	      x = c[i][k]*zconj(t[j][l]*denom);
	      x += u[l][j]*s[i][k]*denom;
	      x += (tll*ckk + zconj(skk)*ull) *
		s[i][k]*zconj(t[j][l])*denom1;
	      data.C[a][b] = x;
	      ++b;
	    }
	  ++a;
	}
      
      a = data1size;
      for(i=1; i<=data2size; ++i) if(i != l)
	{
	  b = 1;
	  for(j=1; j<=data1size; ++j) if(j != k)
	    {
	      x = u[i][l]*zconj(s[j][k]*denom);
	      x += c[k][j]*t[i][l]*denom;
	      x += (skk*ull + zconj(tll)*ckk) *
		t[i][l]*zconj(s[j][k])*denom1;
	      data.C[a][b] = x;
	      ++b;
	    }
	  ++a;
	}
    }
    else if(info1.noise) {
      // dev1 is the culprit; dev2 is noiseless
      complex const *const *c = & data1.C[0];
      complex ckk = c[k][k];

      complex denom1 = zmagsq(denom);

      a = 1;
      for(i=1; i<=data1size; ++i) if(i != k)
	{
	  b = 1;
	  for(j=1; j<=data1size; ++j) if(j != k)
	    {
	      x = c[i][j] + c[i][k]*zconj(s[j][k]*tll*denom);
	      x += c[k][j]*s[i][k]*tll*denom;
	      x += zmagsq(tll)*ckk*s[i][k]*zconj(s[j][k])*denom1;
	      data.C[a][b] = x;
	      ++b;
	    }
	  ++a;
	}

      a = data1size;
      for(i=1; i<=data2size; ++i) if(i != l)
	{
	  b = data1size;
	  for(j=1; j<=data2size; ++j) if(j != l)
	    {
	      data.C[a][b] = ckk*t[i][l]*zconj(t[j][l])*denom1;
	      ++b;
	    }
	  ++a;
	}
      
      a = 1;
      for(i=1; i<=data1size; ++i) if(i != k)
	{
	  b = data1size;
	  for(j=1; j<=data2size; ++j) if(j != l)
	    {
	      x = c[i][k]*zconj(t[j][l]*denom);
	      x += tll*ckk*s[i][k]*zconj(t[j][l])*denom1;
	      data.C[a][b] = x;
	      ++b;
	    }
	  ++a;
	}
      
      a = data1size;
      for(i=1; i<=data2size; ++i) if(i != l)
	{
	  b = 1;
	  for(j=1; j<=data1size; ++j) if(j != k)
	    {
	      x = c[k][j]*t[i][l]*denom;
	      x += zconj(tll)*ckk*t[i][l]*zconj(s[j][k])*denom1;
	      data.C[a][b] = x;
	      ++b;
	    }
	  ++a;
	}
    }
    else {
      // dev2 has the noise
      complex const *const *u = & data2.C[0];
      complex ull = u[l][l];

      complex denom1 = zmagsq(denom);

      a = 1;
      for(i=1; i<=data1size; ++i) if(i != k)
	{
	  b = 1;
	  for(j=1; j<=data1size; ++j) if(j != k)
	    {
	      data.C[a][b] = ull*s[i][k]*zconj(s[j][k])*denom1;
	      ++b;
	    }
	  ++a;
	}

      a = data1size;
      for(i=1; i<=data2size; ++i) if(i != l)
	{
	  b = data1size;
	  for(j=1; j<=data2size; ++j) if(j != l)
	    {
	      x = u[i][j] + u[i][l]*zconj(t[j][l]*skk*denom);
	      x += u[l][j]*t[i][l]*skk*denom;
	      x += zmagsq(skk)*ull*t[i][l]*zconj(t[j][l])*denom1;
	      data.C[a][b] = x;
	      ++b;
	    }
	  ++a;
	}
      
      a = 1;
      for(i=1; i<=data1size; ++i) if(i != k)
	{
	  b = data1size;
	  for(j=1; j<=data2size; ++j) if(j != l)
	    {
	      x = u[l][j]*s[i][k]*denom;
	      x += zconj(skk)*ull*s[i][k]*zconj(t[j][l])*denom1;
	      data.C[a][b] = x;
	      ++b;
	    }
	  ++a;
	}
      
      a = data1size;
      for(i=1; i<=data2size; ++i) if(i != l)
	{
	  b = 1;
	  for(j=1; j<=data1size; ++j) if(j != k)
	    {
	      x = u[i][l]*zconj(s[j][k]*denom);
	      x += skk*ull*t[i][l]*zconj(s[j][k])*denom1;
	      data.C[a][b] = x;
	      ++b;
	    }
	  ++a;
	}
    }
  }

  // Calculate B (source) vector
  if(info1.source && info2.source) {
    // both dev1 and dev2 are sources
    complex const *bs = & data1.B[0];
    complex const *bt = & data2.B[0];
    a = 1;
    for(i=1; i<=data1size; ++i) if(i != k)
      {
	x = bs[i];
	x += s[i][k]*(tll*bs[k]+bt[l])*denom;
	data.B[a] = x;
	++a;
      }
    for(i=1; i<=data2size; ++i) if(i != l)
      {
	x = bt[i];
	x += t[i][l]*(skk*bt[l]+bs[k])*denom;
	data.B[a] = x;
	++a;
      }
  }
  else if(info1.source) {
    // only dev1 has source data
    complex const *bs = & data1.B[0];
    a = 1;
    for(i=1; i<=data1size; ++i) if(i != k)
      {
	x = bs[i];
	x += s[i][k]*tll*bs[k]*denom;
	data.B[a] = x;
	++a;
      }
    for(i=1; i<=data2size; ++i) if(i != l)
      {
	x = t[i][l]*bs[k]*denom;
	data.B[a] = x;
	++a;
      }
  }
  else if(info2.source) {
    // only dev2 is a source
    complex const *bt = & data2.B[0];
    a = 1;
    for(i=1; i<=data1size; ++i) if(i != k)
      {
	x = s[i][k]*bt[l]*denom;
	data.B[a] = x;
	++a;
      }
    for(i=1; i<=data2size; ++i) if(i != l)
      {
	x = bt[i];
	x += t[i][l]*skk*bt[l]*denom;
	data.B[a] = x;
	++a;
      }
  }
  else {
    // neither are sources
    data.B = 0.0;
  }

  // release the memory pointed to by pd1 and/or pd2
  delete pd1; delete pd2;

}


//**************************************************************
// Join two devices without connecting any ports.

connection::connection(nport & d1, nport & d2)
  : dev1(d1), dev2(d2), connection_type(BLOCK)
{
  // Since we access sizes often, store them locally
  int data1size = dev1.size();
  int data2size = dev2.size();

  // Create data for this object of the appropriate size.
  data.resize(data1size + data2size);
  portmap = portArray(data.size());

  // Now fill in the portmap.
  int a=0;
  int i;
  for(i=1; i<=data1size; ++i)
  {
    ++a;
    portmap.set(a, dev1.get_port(i));
  }
  for(i=1; i<=data2size; ++i)
  {
    ++a;
    portmap.set(a, dev2.get_port(i));
  }
}

void connection::calc_block()
{
  sdata *pd1 = 0, *pd2 = 0;   // we'll use these only if renormalization needed

  // Get the data from the devices to be joined.
  const sdata& d1ref = (calc_noise) ? dev1.get_data() : dev1.get_data_S();
  const sdata& d2ref = (calc_noise) ? dev2.get_data() : dev2.get_data_S();

  // set up normalizing impedance and the references data1 and data2
  data.set_znorm(device::Z0);
  if(d1ref.get_znorm() != device::Z0 && d1ref.get_znorm() != 0.0)
    pd1 = new sdata(d1ref, device::Z0);
  if(d2ref.get_znorm() != device::Z0 && d2ref.get_znorm() != 0.0)
    pd2 = new sdata(d2ref, device::Z0);

  const sdata& data1 = (pd1) ? *pd1 : d1ref;
  const sdata& data2 = (pd2) ? *pd2 : d2ref;

  // Since we access sizes often, store them locally
  int data1size = data1.size();
  int data2size = data2.size();

  int a, b, i, j;

  // Fill in the S and C matrices.
  a=0;
  b=0;
  for(i=1; i<=data1size; ++i)
  {
    ++a;
    b=0;
    for(j=1; j<=data1size; ++j)
    {
      ++b;
      data.S[a][b] = data1.S[i][j];
      data.C[a][b] = data1.C[i][j];
    }
  }

  for(i=1; i<=data2size; ++i)
  {
    ++a;
    b=data1size;
    for(j=1; j<=data2size; ++j)
    {
      ++b;
      data.S[a][b] = data2.S[i][j];
      data.C[a][b] = data2.C[i][j];
    }
  }

  // Now fill in the B vector.
  a=0;
  for(i=1; i<=data1size; ++i)
  {
    ++a;
    data.B[a] = data1.B.read(i);
  }
  for(i=1; i<=data2size; ++i)
  {
    ++a;
    data.B[a] = data2.B.read(i);
  }

  // release the memory pointed to by pd1 and/or pd2
  delete pd1; delete pd2;

}


//**************************************************************
// get info/status; recalc interfaces:

const nport::data_info & connection::get_data_info()
{
  info = dev1.get_data_info();
  if(connection_type != INTRA) {
    const nport::data_info & info2 = dev2.get_data_info();
    info.noise = info.noise || info2.noise;
    info.active = info.active || info2.active;
    info.source = info.source || info2.source;
  }
  return info;
}

port connection::get_port(int index)
{
  if((index > 0) && (index <= size()))
    return portmap.get(index);

  // Subscript out of range, return default port.
  error::warning("Port index out of range.");
  return port();
}

int connection::get_port(port p)
{

  for(int i=1; i<=size(); ++i)
    if(portmap.get(i) == p)
    {
      return i;
    }

  return 0;
}

void connection::recalc()
{
  calc_noise = true;
  switch(connection_type)
  {
    case INTER:
      calc_inter();
      break;
    case INTRA:
      calc_intra();
      break;
    case BLOCK:
      calc_block();
      break;
    default:
      error::fatal("Unknown connection type in connection::recalc()");
  }
}

void connection::recalc_S()
{
  calc_noise = false;
  switch(connection_type)
  {
    case INTER:
      calc_inter();
      break;
    case INTRA:
      calc_intra();
      break;
    case BLOCK:
      calc_block();
      break;
    default:
      error::fatal("Unknown connection type in connection::recalc()");
  }
}

