// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
//
// error.cc

#include "error.h"
#include <iostream>
#include <cstdlib>

using namespace std;

// static data:
bool error::strict = false;
bool error::messages = false;
ostream *error::pout = & cerr;

void error::fatal(const char *e)
{
  (*pout) << "\nFATAL ERROR: " << e << endl;
  exit(1);
}

void error::fatal(const string &e)
{ fatal(e.c_str()); } 

void error::fatal(const char *e, const char *f)
{
  (*pout) << "\nFATAL ERROR: " << e << " " << f << endl;
  exit(1);
}

void error::fatal(const string &e, const string &f)
{ fatal(e.c_str(), f.c_str()); } 

void error::warning(const char *e) 
{
  if(strict)
    fatal(e);
  else
    (*pout) << "\nWARNING: " << e << endl;
}

void error::warning(const string &e)
{ warning(e.c_str()); } 

void error::warning(const char *e, const char *f) 
{
  if(strict)
    fatal(e,f);
  else
    (*pout) << "\nWARNING: " << e << " " << f << endl;
}

void error::warning(const string &e, const string &f)
{ warning(e.c_str(), f.c_str()); } 

void error::message(const char *e) 
{
  if(messages)
    (*pout) << "\nMESSAGE: " << e << endl;
}

void error::message(const string &e)
{ message(e.c_str()); } 

void error::message(const char *e, const char *f) 
{
  if(messages)
    (*pout) << "\nMESSAGE: " << e << " " << f << endl;
}

void error::message(const string &e, const string &f)
{ message(e.c_str(), f.c_str()); } 
