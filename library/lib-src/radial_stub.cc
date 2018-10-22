// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
//
// radial_stub.cc

#include "radial_stub.h"
#include "error.h"
#include <cmath>

using namespace std;

radial_stub::radial_stub() : data_ptr_nport()
{
  info.source = false;
  // Unless a separate temperature parameter gets added, stubs are passive
  info.active = false;
  nsections = 15;

  for(int i=0; i<=nsections; i++)
    sec[i].sub_thick = &sub_thick;

  sec[0].width = &width;
  sec[0].length = &length;

  for(int i=0; i<nsections; i++)
    rs.connect(sec[i], 2, sec[i+1], 1);

  rs.add_port(sec[0], 1);
  rs.add_port(sec[nsections], 2);
}

radial_stub & radial_stub::substrate(dielectric & d)
{
  sub = &d;

  for(int i=0; i<=nsections; i++)
    sec[i].substrate(*sub);

  return *this;
}

radial_stub & radial_stub::superstrate(dielectric & d)
{
  super = &d;

  for(int i=0; i<=nsections; i++)
    sec[i].superstrate(*super);

  return *this;
}

radial_stub & radial_stub::top_strip(surfimp & s)
{
  top = &s;

  for(int i=0; i<=nsections; i++)
    sec[i].top_strip(*top);

  return *this;
}

radial_stub & radial_stub::ground_plane(surfimp & s)
{
  ground = &s;

  for(int i=0; i<=nsections; i++)
    sec[i].ground_plane(*ground);

  return *this;
}

void radial_stub::recalc(bool noise)
{
  // Radius of the inner part of the stub that overlaps with the
  // transmission line that feeds the stub.
  double r0;

  // Length of the annular slices.
  double sect_len;

  // Calculate inner radius so that the sum of the areas of the transmission
  // lines equals the area of the real physical radial stub.
  r0 = width / sqrt(2. * angle * tan(0.5*angle));

  // Make sure the stub is bigger than the transmission line feeding it.
  if(r0 >= radius)
  {
    error::fatal("radial_stub is smaller than the transmission line feeding it!");
  }

  sect_len = (radius - r0) / nsections;
 
  // Set the width and length of the annular slices.
  // sec[0] should already be set, because its parameters are shadowing.
  for(int i=1; i<=nsections; i++)
  {
    sec[i].length = sect_len;
    sec[i].width = angle * (r0 + ((i-0.5) * sect_len));
  }

  // Now that the microstrips are set, have the circuit calculate itself.
  data_ptr = (noise) ? &rs.get_data() : &rs.get_data_S();

  // // Print out microstrip sizes for debugging.
  // double area = 0.0;
  // cout << "width = " << width/Micron << endl;
  // cout << "angle = " << angle << endl;
  // cout << "r0 = " << r0/Micron << endl;
  // for(int i=0; i <= nsections; i++)
  // {
  //   cout << "Section " << i << ": " << sec[i].width/Micron
  //        << " microns wide X " << sec[i].length/Micron
  //        << " microns long." << endl;
  //   area += (sec[i].width/Micron) * (sec[i].length/Micron);
  // }
  // cout << "Total area = " << area << " microns^2." << endl;
}

radial_stub::radial_stub(const radial_stub & r) : data_ptr_nport(r)
{
  info.source = false;
  // Unless a separate temperature parameter gets added, stubs are passive
  info.active = false;
  nsections = r.nsections;

  radius = r.radius;
  angle = r.angle;
  length = r.length;
  width = r.width;
  sub_thick = r.sub_thick;

  if(r.sub != 0)
    substrate(*r.sub);
  if(r.super != 0)
    superstrate(*r.super);
  if(r.top != 0)
    top_strip(*r.top);
  if(r.ground != 0)
    ground_plane(*r.ground);
  
  for(int i=0; i<=nsections; i++)
    sec[i].sub_thick = &sub_thick;

  sec[0].width = &width;
  sec[0].length = &length;

  for(int i=0; i<nsections; i++)
    rs.connect(sec[i], 2, sec[i+1], 1);

  rs.add_port(sec[0], 1);
  rs.add_port(sec[nsections], 2);
}

radial_stub & radial_stub::operator=(const radial_stub & r)
{
  // Beware of self assignment: r = r
  if(this != &r)
  {
    radius = r.radius;
    angle = r.angle;
    length = r.length;
    width = r.width;
    sub_thick = r.sub_thick;

    if(r.sub != 0)
      substrate(*r.sub);
    if(r.super != 0)
      superstrate(*r.super);
    if(r.top != 0)
      top_strip(*r.top);
    if(r.ground != 0)
      ground_plane(*r.ground);

  }
  return *this;
}
