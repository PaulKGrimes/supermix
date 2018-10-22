// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
/**************************************************************\
* datafile.h                                                   *
*                                                              *
* J. Zmuidzinas  October 31, 1997                              *
*                                                              *
* datafile.h contains the declaration for the data file class  *
* used by the circuit program. A data file is essentially just *
* a set of columns of floating-point numbers. Comments may be  *
* included - all characters following a ! or # character are   *
* treated as a comment and are thrown away.                    *
*                                                              *
* NOTE THE ORDER IN DATA ACCESS:                               *
*  read(column#, line#)                                        *
*  where column#, row# refer to the position in the file       *
*                                                              *
* Data are stored in the private real_table in this            *
* order; however, the first index of a table is usually called *
* the "row" index and the second index is called the           * 
* "column" index.                                              *
\**************************************************************/

#ifndef DATAFILE_H
#define DATAFILE_H 1

#include "table.h"

/**************************************************************\
*                                                              *
* class datafile                                               *
*                                                              *
\**************************************************************/
class datafile
{
private:
  static const unsigned int maxline ; // maximum number of characters per line
  int ncolumns ;               // number of columns in data file
  int nlines ;                 // number of lines
  real_table data ;            // this is where we hold the data

//
// declare a little structure to hold the data temporarily
//
  class tempnumber {
    public:
    int icol ;
    int iline ;
    double value ;
    tempnumber *next ;
  };

public:
  // No default constructor is provided, since the file name must be specified
  datafile(char const * const filename) ;

  // Copy constructor
  datafile(const datafile & df) ;

  // Assignment operator - change to const argument once table is fixed--fixed
  datafile & operator=(const datafile & df);

  // Data retrieval - we can let table class do bounds checking
  double read(int col, int line) const
    { return data.read(col, line); }

  int numcolumns() const {return ncolumns;}
  int numlines() const {return nlines;}

  // Return a pointer to the internal table; to allow copying
  const real_table * table() const {return & data; }

};

#endif /* DATAFILE_H */
