// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
//
// datafile.cc

#include "datafile.h"
#include "error.h"
#include <cstdio>             // for snprintf()
#include <cstdlib>            // for strtod()
#include <cstring>            // for strchr()
#include <cctype>             // for isspace()
#include <iostream>
#include <fstream>            // file version of iostream

using namespace std;

// Maximum line length (in characters) we wish to process
const unsigned int datafile::maxline = 32767 ;    

// Maximum error message string length (in characters)
const int msglen = 2000 ;

// Comment delimiters (may want to move to global.h)
static char comment[] = "#!" ;

// fuctions to skip over whitespace or nonwhitespace chars
static void skipwhite(char * & c)
{ while (*c && isspace(*c)) c++; return; }
static void skipnonwhite(char * & c)
{ while (*c && !isspace(*c)) c++; return; }

//
// datafile constructor
//
// The constructor initializes data table with zero lines, columns
// it then reads in the data file, throwing out comments, and storing
// all doubles it finds in a temporary list, along with the line and column
// numbers associated with the double. It then creates a data table
// large enough to hold all of the data, and fills it in from the list
// (the list elements are then deleted).
// The number of columns in the table is set to the *maximum* number of
// columns found in the data file. If the lines in the file do not all
// have the same number of columns, the table is padded with zeros for
// the missing columns. (Columns are always filled in starting at 
// column 1.)

datafile::datafile(char const * const filename) : 
  ncolumns(0), nlines(0), data(0)
{
  ifstream infile(filename) ;
  if(!infile) {   // couldn't open the file
    char errmsg[msglen] ;
    snprintf(errmsg, msglen, "Couldn't open data file: %s", filename);
    error::warning(errmsg) ;
    return ;
  }

  // file opened OK, so let's read in the data
  
  // we'll read lines into a freshly allocated char[] array...
  char * line = new char[maxline] ;

  int iline = 0 ;                // number of current line of data
  int file_line_number = 0 ;     // number of current line in file
  tempnumber *head = 0 ;         // first member in our temp data list

  while(!infile.eof()) {
    int icol = 0 ;               // a new line, so no columns yet
    file_line_number++ ;         // keep track of where we are in file

    // read the next line
    infile.get(line, maxline, '\n') ;
    // discard trailing '\n', check if line truncated and complain if so
    if (!infile.eof() && infile.get() != '\n') {
      // then there are additional chars on the line
      // discard them and final newline
      while (!infile.eof() && infile.get() != '\n') ;  // discarding
      // then complain to the user
      char errmsg[msglen] ;
      snprintf(errmsg, msglen,
        "Line number %i in file: %s exceeds %i characters. Truncating!!",
        file_line_number, filename, maxline) ;
      error::warning(errmsg) ;
    }

    // process the line
    char * c = line;             // moving pointer within line
    char * next;                 // used to update c
    double value ;               // holds converted doubles
    
    while ( *c ) {               // not '\0'
      skipwhite(c);              // move c to next token in line
      
      // a comment char effectively terminates the line
      if ( strchr(comment, *c) ) {
	// found a comment delimiter
	*c = '\0';
	continue;
      }

      // attempt to convert token to double, stored in value
      value = strtod(c, & next);
      if ( next == c ) {
	// then it's not a double, so skip it
	skipnonwhite(c);
	continue;
      }

      // a valid double was found; store it in list and move on
      c = next; // skip over the parsed double
      if (icol == 0) iline++ ;   // increment iline only once each line 
      icol++ ;  // now (icol,iline) are coordinates of this value
      tempnumber *temp  = new tempnumber ;  // allocate another link 
      if(!temp) {                // memory allocation failed
	// we're in trouble - function call may bomb (no memory)
	error::fatal(            // but attempt to tell user
	  "Out of memory in datafile constructor - file too large!!") ;
      }
      temp->value = value ;
      temp->icol = icol ;                
      temp->iline = iline ;
      temp->next = head ;        // update links
      head = temp ;

    } // finished processing the line
    if (icol > ncolumns) ncolumns = icol;  // update max column number

  } // finished processing the file
  infile.close();
  nlines = iline;

  // now we need to check if there was really any data
  if(nlines == 0 || ncolumns == 0) {
    // an empty table
    char errmsg[msglen] ;
    snprintf(errmsg, msglen,
	     "Didn't find any data in file: %s",
	     filename) ;
    error::warning(errmsg) ;
    nlines = ncolumns = 0;       // make sure both are zero
    return ;
  }
  
  // allocate memory for table, using 1..N indexing, and fill it
  data.reallocate(ncolumns, nlines, Index_1, Index_1) ;
  // don't need this, already full of 0.0: data.fill(0.) ;
  tempnumber *temp = head ;
  tempnumber *trash ;
  while(temp) {
    data[temp->icol][temp->iline] = temp->value ;
    trash = temp ; temp = temp->next ;
    delete trash ;  // recover heap memory
  }

  // finally recover memory for character buffer
  delete [] line;

}


// copy constructor - just initialize using data member copy constructors

datafile::datafile(const datafile &df) :
  ncolumns(df.ncolumns), nlines(df.nlines), data(df.data)
{ }

// assignment operator
// not declared with const argument until table operator= is fixed -- fixed F.R.

datafile & datafile::operator=(const datafile & df)
{
  if(this != &df) {                        // no self-assignment
    ncolumns = df.ncolumns ;
    nlines = df.nlines ;
    data.resize(0) = df.data ;             // throw away old, copy the new table
  }
  return(*this) ;
}
