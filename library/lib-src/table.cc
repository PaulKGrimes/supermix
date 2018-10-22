// SuperMix version 1.4 C++ source file
// Copyright (c) 1999, 2001, 2004 California Institute of Technology.
// All rights reserved.
//
// table.cc

#include "table.h"
#include "vector.h"
#include "datafile.h"
#include <iostream>
#include <iomanip>

using namespace std;

// Useful functions:

static inline int min(int a, int b)  { return (a < b)? a : b; }
static inline int max(int a, int b)  { return (a > b)? a : b; }
static inline int length(int a, int b)
{ int n = b - a + 1; return (n < 0)? 0 : n; }

// Determine the result index mode which maximizes number of elements:
static v_index_mode ResultModeMax(const v_index_mode a, const v_index_mode b)
{
  if((a == Index_S)||(b == Index_S)) return Index_S;
  else if ((a == Index_C)||(b == Index_C)) return Index_C;
  else return Index_1;
}

// i/o formatting static variable

static string _out_separator("\n");


// ************************************************************************
// real_matrix definitions:


// matrix output:

ostream & operator << (ostream & s, const real_matrix & A)
{
  std::streamsize w = s.width();
  if (A.Lminindex() <= A.Lmaxindex()) { // matrix isn't empty
    { // output first row (using a row alias)
      real_vector v(const_cast<double *>(A[A.Lminindex()]), A.Rsize, A.Rmode);
      v.maxindex(A.Rmaxindex());  // set same valid index range
      s << v;
    }
    // output remaining rows
    for (int i = A.Lminindex() + 1; i <= A.Lmaxindex(); ++i) {
      real_vector v(const_cast<double *>(A[i]), A.Rsize, A.Rmode);
      v.maxindex(A.Rmaxindex());
      s << _out_separator << setw(w) << v;
    }
  }
  return s << setw(0);  // just in case no output was generated
}

real_matrix & real_matrix::show(ostream & s)
{
  s << endl << *this << endl;
  return *this;
}

real_matrix & real_matrix::show()
{
  return show(cout);
}

const real_matrix & real_matrix::show(ostream & s) const
{
  s << endl << *this << endl;
  return *this;
}

const real_matrix & real_matrix::show() const
{
  return show(cout);
}

string real_matrix::out_separator()
{ return _out_separator; }

string real_matrix::out_separator(const string & s)
{ string temp = _out_separator; _out_separator = s; return temp; }

string real_matrix::out_separator(const char * const s)
{ string temp = _out_separator; _out_separator = s; return temp; }

string real_matrix::out_separator(const char s)
{ string temp = _out_separator; _out_separator = s; return temp; }

//-------------------------------------------------------------------------
// construct() do the real work of matrix construction

void real_matrix::construct(const int n, const int m,
			   const v_index_mode tl,
			   const v_index_mode tr)
{
  // set up matrix private member variables
  internal_Lsize = (n >= 0) ? n : 0;
  internal_Rsize = (m >= 0) ? m : 0;
  internal_Lmode = tl;
  internal_Rmode = tr;
  trashptr = & trash;

  // determine number of columns and rows requested:
  int nrows = Lsize; if (Lmode == Index_S) nrows += (nrows + 1);
  int ncols = Rsize; if (Rmode == Index_S) ncols += (ncols + 1);

  // allocate memory for the array of pointers to rows, if required:
  data = delete_pointer_data = (nrows) ? new double *[nrows] : 0;

  // allocate memory for the array of data elements, if required:
  int nelem = ncols * nrows;
  if (nelem) {
    // need to allocate memory
    delete_pointer_rows = new double[nelem];
    if (delete_pointer_rows == 0) {
      // memory alloc failed, so give back pointer array as well
      delete [] delete_pointer_data;
      data = delete_pointer_data = 0;
    }
  }
  else {
    // no allocation necessary, nelem == 0
    delete_pointer_rows = 0;
  }

  // now that allocation has been attempted, update sizes and index limits
  if (data == 0) {
    // no pointer array
    internal_Lsize = nrows = 0; // in case they weren't already
    if (Lmode == Index_S) data = & trashptr; //so **data = trash
    Lmaxindexvalue = (Lmode == Index_1) ? 0 : -1;
  }
  else {
    // pointer array exists, original Lsize and nrows o.k.
    switch (Lmode) {
    case Index_C: {
      Lmaxindexvalue = Lsize - 1;
      break;
    }
    case Index_1: {
      Lmaxindexvalue = Lsize;
      --data;        // so *delete_pointer_data = data[1]
      break;
    }
    case Index_S: {
      Lmaxindexvalue = Lsize;
      data += Lsize; // so *delete_pointer_data = data[-Lsize]
      break;
    }
    } //switch
  } //else
  // final left index setup step is to set Lminindexvalue:
  switch (Lmode) {
  case Index_C: { Lminindexvalue = 0; break; }
  case Index_1: { Lminindexvalue = 1; break; }
  case Index_S: { Lminindexvalue = -Lmaxindexvalue; break; }
  }
  // setup of left index complete

  // now for the right index:
  if (delete_pointer_rows == 0) {
    // no data elements
    internal_Rsize = ncols = 0; // in case they weren't already
    Rmaxindexvalue = (Rmode == Index_1) ? 0 : -1;
  }
  else {
    // data array exists, original Rsize and ncols o.k.
    Rmaxindexvalue = (Rmode == Index_C) ? Rsize - 1 : Rsize;
  }
  switch (Rmode) {
  case Index_C: { Rminindexvalue = 0; break; }
  case Index_1: { Rminindexvalue = 1; break; }
  case Index_S: { Rminindexvalue = -Rmaxindexvalue; break; }
  }
  // setup of right index complete

    
  // now fill pointer array with pointers to the [0] element of each row
  int i, offset; // offset is the pointer offset into each row due to Rmode
  switch (Rmode) {
  default:
  case Index_C: { offset = 0; break; }
  case Index_1: { offset = -1; break; }
  case Index_S: { offset = Rsize; break; }
  }
  if (ncols == 0) offset = 0; // no offset if no data
  for(i = Lminindexvalue; i <= Lmaxindexvalue; ++i, offset += ncols)
    // this loop will only execute if pointer memory was allocated
    data[i] = delete_pointer_rows + offset;

} // construct()

//-------------------------------------------------------------------------
// copy constructors:

real_matrix::real_matrix(const real_matrix & B)
  : Lsize(internal_Lsize), Rsize(internal_Rsize), 
    Lmode(internal_Lmode), Rmode(internal_Rmode)
{
  construct(B.Lsize, B.Rsize, B.Lmode, B.Rmode);
  int i,j;
  // the following loop will work even if alloc for B failed and
  // B.Lmode == Index_S.  In this case, the contents of trash will be
  // copied.
  for (i = Lminindex(); i <= Lmaxindex(); ++i)
    for (j = Rminindex(); j <= Rmaxindex(); ++j)
      data[i][j] = B[i][j];

  if(B.is_empty())
    make_empty();
  else {
    Lmaxindex(B.Lmaxindex());
    Rmaxindex(B.Rmaxindex());
  }
}

real_matrix::real_matrix(const real_vector & v)
  : Lsize(internal_Lsize), Rsize(internal_Rsize), 
    Lmode(internal_Lmode), Rmode(internal_Rmode)
{
  construct(v.size, 1, v.mode, Index_1);
  // the following loop will work even if alloc for v failed and
  // v.mode == Index_S.  In this case, the contents of trash will be
  // copied.
  for (int i = Lminindex(); i <= Lmaxindex(); ++i)
    data[i][1] = v[i];

  Lmaxindexvalue = v.maxindex();
  Lminindexvalue = v.minindex();
}

real_matrix::real_matrix(const datafile & D)
  : Lsize(internal_Lsize), Rsize(internal_Rsize), 
    Lmode(internal_Lmode), Rmode(internal_Rmode)
{
  const real_matrix * p = D.table();
  construct(p->Lsize, p->Rsize, p->Lmode, p->Rmode);
  int i,j;
  for (i = Lminindex(); i <= Lmaxindex(); ++i)
    for (j = Rminindex(); j <= Rmaxindex(); ++j)
      data[i][j] = p->data[i][j];

  if(p->is_empty())
    make_empty();
  else {
    Lmaxindex(p->Lmaxindex());
    Rmaxindex(p->Rmaxindex());
  }
}

//-------------------------------------------------------------------------
// reindex()  change the index modes

real_matrix & real_matrix::reindex(const v_index_mode Lnew, const v_index_mode Rnew)
{
  if (Lnew == Lmode && Rnew == Rmode) return *this;

  // some index change is needed; calculate dimensions of matrix
  int nrows = (Lmode == Index_S) ? 2*Lsize + 1 : Lsize;
  int ncols = (Rmode == Index_S) ? 2*Rsize + 1 : Rsize;
  
  // any dimension with new Index_S needs an odd number of values
  if (Lnew == Index_S && (nrows>>1)<<1 == nrows) {  // nrows even
    // since nrows even, Lmode can't be Index_S
    reallocate(Lsize+1,Rsize,Lmode,Rmode);
    nrows = Lsize; // the new Lsize
  }
  if (Rnew == Index_S && (ncols>>1)<<1 == ncols) {  // ncols even
    reallocate(Lsize,Rsize+1,Lmode,Rmode);
    ncols = Rsize; // the new Rsize
  }

  // finally, if resulting matrix is empty, just reallocate to fix modes
  if (delete_pointer_rows == 0)  {
    reallocate(0,0,Lnew,Rnew);
    return *this;
  }

  // if here, the matrix has some elements

  // set up new left indexing:
  if (Lnew != Lmode) { // reindexing required of Left Index
    int ptr_offset; // offset into row pointer array for data[0]
    switch (Lnew) {
    default:
      { return *this; } // unknown mode, so do nothing
    case Index_C:
      { ptr_offset = 0; break; }
    case Index_1:
      { ptr_offset = -1; break; }
    case Index_S:
      { ptr_offset = (nrows - 1)/2; break; }
    } // switch
    internal_Lsize = (Lnew == Index_S) ? ptr_offset : nrows;
    Lminindexvalue = -ptr_offset; 
    Lmaxindexvalue = Lminindexvalue + nrows - 1;
    data = delete_pointer_data + ptr_offset;
    internal_Lmode = Lnew;
  }

  // now for the right indexing:
  if (Rnew != Rmode) {
    int ptr_offset; // pointer offset into a row to data[][0]

    switch (Rnew) { // the new offset
    default:
      { return *this; } // unknown mode, so do nothing
    case Index_C:
      { ptr_offset = 0; break; }
    case Index_1:
      { ptr_offset = -1; break; }
    case Index_S:
      { ptr_offset = (ncols - 1)/2; break; }
    } // switch
    int old_Rsize = Rsize;  // save for switch below
    internal_Rsize = (Rnew == Index_S) ? ptr_offset : ncols;
    Rminindexvalue = -ptr_offset; 
    Rmaxindexvalue = Rminindexvalue + ncols - 1;
    switch (Rmode) { // subtract off the old offset
    default:
    case Index_C:
      { ptr_offset -= 0; break; }
    case Index_1:
      { ptr_offset -= -1; break; }
    case Index_S:
      { ptr_offset -= old_Rsize; break; }
    }
    // now ptr_offset is how far we must adjust each row pointer
    for (int i = Lminindexvalue; i <= Lmaxindexvalue; ++i)
      data[i] += ptr_offset;
    internal_Rmode = Rnew;
  } // if (Rnew != Rmode)  

  return *this;
}

//-------------------------------------------------------------------------
// constfill()  fill the matrix with a constant value
//              Note: data subset control will affect elements copied

void real_matrix::constfill(const double f)
{
  for (int i = Lminindex(); i <= Lmaxindex(); ++i)
    for (int j = Rminindex(); j <= Rmaxindex(); ++j)
      data[i][j] = f;
}

//-------------------------------------------------------------------------
// fillall()  fill regardless of data subset control

real_matrix & real_matrix::fillall(const double f)
{
  int save_Lmax = Lmaxindexvalue; 
  int save_Rmax = Rmaxindexvalue;
  int save_Lmin = Lminindexvalue; 
  int save_Rmin = Rminindexvalue;

  maxindex(Lsize+Rsize); // access full matrix
  constfill(f);
  Lmaxindexvalue = save_Lmax; Rmaxindexvalue = save_Rmax;  // restore subset
  Lminindexvalue = save_Lmin; Rminindexvalue = save_Rmin;
  return *this;
}

//-------------------------------------------------------------------------
// fill a row or column from a vector

real_matrix & real_matrix::fillrow(const int n, const real_vector & v)
{
  if ((n >= Lminindex())&&(n <= Lmaxindex())) {  // then n is o.k.
    for (int i = Rminindex(); i <= Rmaxindex(); ++i)
      data[n][i] = v.read(i);
  }
  return *this;
}

real_matrix & real_matrix::fillcol(const int n, const real_vector & v)
{
  if ((n >= Rminindex())&&(n <= Rmaxindex())) {  // then n is o.k.
    for (int i = Lminindex(); i <= Lmaxindex(); ++i)
      data[i][n] = v.read(i);
  }
  return *this;
}

//-------------------------------------------------------------------------
// Lmaxindex(n), Rmaxindex(n), make_empty()

int real_matrix::Lmaxindex(const int n)
{
  if (Lsize == 0 && Lmode != Index_S)
    // can't change the maxindex if no data elements
    return Lmaxindex();
  else {
    switch (Lmode) {
    case Index_C: {
      Lmaxindexvalue = min((Lsize - 1), max(0,n));
      break;
    }
    case Index_1: {
      Lmaxindexvalue = min(Lsize, max(1,n));
      break;
    }
    case Index_S: {
      Lmaxindexvalue = min(Lsize, max(0,n));
      Lminindexvalue = -Lmaxindexvalue;
      break;
    }
    } //switch
    return Lmaxindexvalue;
  } //else
}

int real_matrix::Rmaxindex(const int n)
{
  if (Rsize == 0 && Rmode != Index_S)
    // can't change the maxindex if no data elements
    return Rmaxindex();
  else {
    switch (Rmode) {
    case Index_C: {
      Rmaxindexvalue = min((Rsize - 1), max(0,n));
      break;
    }
    case Index_1: {
      Rmaxindexvalue = min(Rsize, max(1,n));
      break;
    }
    case Index_S: {
      Rmaxindexvalue = min(Rsize, max(0,n));
      Rminindexvalue = -Rmaxindexvalue;
      break;
    }
    } //switch
    return Rmaxindexvalue;
  } //else
}

real_matrix & real_matrix::make_empty()
{
  switch (Lmode) {
  case Index_C:
    Lmaxindexvalue = -1;
    Lminindexvalue = 0;
    break;

  case Index_1:
    Lmaxindexvalue = 0;
    Lminindexvalue = 1;
    break;

  case Index_S:
    Lmaxindexvalue = -1;
    Lminindexvalue = 1;
    break;

  }
  
  switch (Rmode) {
  case Index_C:
    Rmaxindexvalue = -1;
    Rminindexvalue = 0;
    break;

  case Index_1:
    Rmaxindexvalue = 0;
    Rminindexvalue = 1;
    break;

  case Index_S:
    Rmaxindexvalue = -1;
    Rminindexvalue = 1;
    break;

  }
  
  return *this;
}


//-------------------------------------------------------------------------
// copy() copy contents of another matrix

real_matrix & real_matrix::copy(const real_matrix & B)
{
  // check for self copy: "A.copy(A)", and do something intelligent:
  if (this == & B) { clean(); return *this; }

  // otherwise, not a self copy, so:
  maxindex(Lsize+Rsize);  // make all data accessible
  constfill(0);           // and clear the matrix
  // now set data subset on the result:
  if(B.is_empty())
    make_empty();
  else {
    Lmaxindex(B.Lmaxindex());
    Rmaxindex(B.Rmaxindex());
    // loop over the index range intersection and copy:
    int Lmax = min(Lmaxindex(), B.Lmaxindex());
    int Lmin = max(Lminindex(), B.Lminindex());
    int Rmax = min(Rmaxindex(), B.Rmaxindex());
    int Rmin = max(Rminindex(), B.Rminindex());
    int i,j;
    for (i = Lmin; i <= Lmax; ++i)
      for (j = Rmin; j <= Rmax; ++j)
	data[i][j] = B[i][j];
  }
  return *this;
}

//-------------------------------------------------------------------------
// reallocate() change size or indexing modes

real_matrix & real_matrix::reallocate
  (const int n, const int m, const v_index_mode tl, const v_index_mode tr)
{
  // just return if reallocation is unnecessary:
  if ((Lsize == n)&&(Rsize == m)&&(Lmode == tl)&&(Rmode == tr)) return *this;

  // if we're here, we have to reallocate:
  // save the data subset control values and then maximize:
  int new_Lmax = Lmaxindexvalue;
  int new_Rmax = Rmaxindexvalue;
  maxindex(Lsize + Rsize);

  // check if data subset control was turned off; if so, keep turned off
  if (new_Lmax == Lmaxindexvalue) new_Lmax = n; 
  if (new_Rmax == Rmaxindexvalue) new_Rmax = m; 

  // now save the necessary private data members:
  int Lmax = Lmaxindexvalue;
  int Lmin = Lminindexvalue;
  int Rmax = Rmaxindexvalue;
  int Rmin = Rminindexvalue;
  double **old_data = data; 
  double **delete1 = delete_pointer_data;
  double  *delete2 = delete_pointer_rows;

  // with this done, now construct the new matrix:
  construct(n, m, tl, tr);
  constfill(0);

  // now copy in the data using the saved private data members:
  Lmax = min(Lmaxindex(), Lmax);
  Lmin = max(Lminindex(), Lmin);
  Rmax = min(Rmaxindex(), Rmax);
  Rmin = max(Rminindex(), Rmin);
  int i,j;
  for (i = Lmin; i <= Lmax; ++i)
    for (j = Rmin; j <= Rmax; ++j)
      data[i][j] = old_data[i][j];

  // set data subset control
  Lmaxindex(new_Lmax);
  Rmaxindex(new_Rmax);

  // finally delete the old memory allocation:
  delete [] delete1;
  delete [] delete2;

  return *this;
}

//-------------------------------------------------------------------------
// resize: the only one which couldn't be defined in the header file

real_matrix & real_matrix::resize(const complex_matrix & B)
{
  int Lsize = (B.Lmode == Index_C) ? B.Lmaxindex()+1: B.Lmaxindex();
  int Rsize = (B.Rmode == Index_C) ? B.Rmaxindex()+1: B.Rmaxindex();
  return reallocate(Lsize, Rsize, B.Lmode, B.Rmode);
}

//-------------------------------------------------------------------------
// Basic assignment operators

real_matrix & real_matrix::operator = (const real_matrix & B)
{
  // check for self assignment: "A = A", and do something intelligent:
  if (this == & B) { clean(); return *this; }

  // reallocate memory if needed:
  maximize();
  if ((B.Lmode != Lmode)||(B.Rmode != Rmode)||
      (B.Lmaxindex() > Lmaxindexvalue)||(B.Rmaxindex() > Rmaxindexvalue)) {
    delete [] delete_pointer_data; delete [] delete_pointer_rows;
    int newLsize = (B.Lmode == Index_C) ? B.Lmaxindex()+1: B.Lmaxindex();
    int newRsize = (B.Rmode == Index_C) ? B.Rmaxindex()+1: B.Rmaxindex();
    construct(newLsize, newRsize, B.Lmode, B.Rmode);
  }

  copy(B);
  return *this;
}

real_matrix & real_matrix::operator = (const datafile & D)
{
  return (*this = *D.table());
}

//---------------------------------------------------------------------
// other assignment operators (no memory reallocation for these):

real_matrix & real_matrix::operator += (const double s)
{
  /*register*/ double *d, *limit;  // local pointers within a row
  /*register*/ int i;
  for(i = Lminindexvalue; i <= Lmaxindexvalue; ++i) {
    // make local pointers to the first and last elements in the row:
    limit = d = data[i];
    d += Rminindexvalue; limit += Rmaxindexvalue;
    // use the pointer to the first element as the loop variable:
    while (d <= limit) *(d++) += s;
  }
  return *this;
}

real_matrix & real_matrix::operator -= (const double s)
{
  /*register*/ double *d, *limit;  // local pointers within a row
  /*register*/ int i;
  for(i = Lminindexvalue; i <= Lmaxindexvalue; ++i) {
    // make local pointers to the first and last elements in the row:
    limit = d = data[i];
    d += Rminindexvalue; limit += Rmaxindexvalue;
    // use the pointer to the first element as the loop variable:
    while (d <= limit) *(d++) -= s;
  }
  return *this;
}

real_matrix & real_matrix::operator *= (const double s)
{
  /*register*/ double *d, *limit;  // local pointers within a row
  /*register*/ int i;
  for(i = Lminindexvalue; i <= Lmaxindexvalue; ++i) {
    // make local pointers to the first and last elements in the row:
    limit = d = data[i];
    d += Rminindexvalue; limit += Rmaxindexvalue;
    // use the pointer to the first element as the loop variable:
    while (d <= limit) *(d++) *= s;
  }
  return *this;
}

real_matrix & real_matrix::operator /= (const double s)
{
  /*register*/ double *d, *limit;  // local pointers within a row
  /*register*/ int i;
  for(i = Lminindexvalue; i <= Lmaxindexvalue; ++i) {
    // make local pointers to the first and last elements in the row:
    limit = d = data[i];
    d += Rminindexvalue; limit += Rmaxindexvalue;
    // use the pointer to the first element as the loop variable:
    while (d <= limit) *(d++) /= s;
  }
  return *this;
}

real_matrix & real_matrix::operator += (const real_matrix & B)
{
  clean();  // ensure nonvalid elements are 0
  // increase target's valid data range if necessary:
  if (Lmaxindexvalue < B.Lmaxindex()) Lmaxindex(B.Lmaxindex());
  if (Rmaxindexvalue < B.Rmaxindex()) Rmaxindex(B.Rmaxindex());
  // find the intersection of the valid data ranges:
  int Lmax = min(Lmaxindex(), B.Lmaxindex());
  int Lmin = max(Lminindex(), B.Lminindex());
  int Rmax = min(Rmaxindex(), B.Rmaxindex());
  int Rmin = max(Rminindex(), B.Rminindex());
  // loop over the intersection of the ranges:
  /*register*/ int i,j;
  for (i = Lmin; i <= Lmax; ++i)
    for (j = Rmin; j <= Rmax; ++j)
      data[i][j] += B[i][j];
  return *this;
}

real_matrix & real_matrix::operator -= (const real_matrix & B)
{
  clean();  // ensure nonvalid elements are 0
  // increase target's valid data range if necessary:
  if (Lmaxindexvalue < B.Lmaxindex()) Lmaxindex(B.Lmaxindex());
  if (Rmaxindexvalue < B.Rmaxindex()) Rmaxindex(B.Rmaxindex());
  // find the intersection of the valid data ranges:
  int Lmax = min(Lmaxindex(), B.Lmaxindex());
  int Lmin = max(Lminindex(), B.Lminindex());
  int Rmax = min(Rmaxindex(), B.Rmaxindex());
  int Rmin = max(Rminindex(), B.Rminindex());
  // loop over the intersection of the ranges:
  /*register*/ int i,j;
  for (i = Lmin; i <= Lmax; ++i)
    for (j = Rmin; j <= Rmax; ++j)
      data[i][j] -= B[i][j];
  return *this;
}

//-------------------------------------------------------------------------
// matrix math member fcns (other than assignment)

real_matrix & real_matrix::add(const real_matrix & B, const real_matrix & C)
{
  // first check that neither B nor C is this matrix:
  if ((&B != this)&&(&C != this)) {
    // make the size of the result correct, and initiallize to all zeroes:
    maximize();
    v_index_mode Lm = ResultModeMax(B.Lmode, C.Lmode);
    v_index_mode Rm = ResultModeMax(B.Rmode, C.Rmode);
    int Ls = max(B.Lmaxindex(), C.Lmaxindex());
    int Rs = max(B.Rmaxindex(), C.Rmaxindex());
    if ((Lmode!=Lm)||(Rmode!=Rm)||(Lmaxindex()<Ls)||(Rmaxindex()<Rs)) {
      // fix up the result matrix in two steps: index modes first, then sizes
      // the result will automatically be zeroed
      reallocate(0,0,Lm,Rm); get(0,0) = 0; resize(Ls,Rs);
      // we need the above get() = 0 in case Lm == Rm == Index_S
    }
    else {
      // result's modes and sizes o.k., just zero and set maxindexes
      fill(0.0);
      Lmaxindex(Ls); Rmaxindex(Rs);
    }
    // Now do the sum:
    (*this) += B; (*this) += C;
  }
  else if (&B == this) { // the first argument is this matrix
    // if necessary, resize this matrix to hold all elements in the sum
    int Ls = max(Lmaxindex(), C.Lmaxindex());
    int Rs = max(Rmaxindex(), C.Rmaxindex());
    if ((Lmaxindex() < Ls)||(Rmaxindex() < Rs)) {
      clean(); maximize();
    }
    v_index_mode Lm = ResultModeMax(Lmode, C.Lmode);
    v_index_mode Rm = ResultModeMax(Rmode, C.Rmode);
    if ((Lmode!=Lm)||(Rmode!=Rm)||(Lmaxindex()<Ls)||(Rmaxindex()<Rs)) {
      int sL = (Lm == Index_C) ? Ls + 1 : Ls;
      int sR = (Rm == Index_C) ? Rs + 1 : Rs;
      reallocate(sL,sR,Lm,Rm);
    }
    Lmaxindex(Ls); Rmaxindex(Rs);
    // Now do the sum:
    (*this) += C;
  }
  else { // the second argument is this matrix
    // if necessary, resize this matrix to hold all elements in the sum
    int Ls = max(Lmaxindex(), B.Lmaxindex());
    int Rs = max(Rmaxindex(), B.Rmaxindex());
    if ((Lmaxindex() < Ls)||(Rmaxindex() < Rs)) {
      clean(); maximize();
    }
    v_index_mode Lm = ResultModeMax(Lmode, B.Lmode);
    v_index_mode Rm = ResultModeMax(Rmode, B.Rmode);
    if ((Lmode!=Lm)||(Rmode!=Rm)||(Lmaxindex()<Ls)||(Rmaxindex()<Rs)) {
      int sL = (Lm == Index_C) ? Ls + 1 : Ls;
      int sR = (Rm == Index_C) ? Rs + 1 : Rs;
      reallocate(sL,sR,Lm,Rm);
    }
    Lmaxindex(Ls); Rmaxindex(Rs);
    // Now do the sum:
    (*this) += B;
  }
  return *this;
}

real_matrix & real_matrix::sub(const real_matrix & B, const real_matrix & C)
{
  // first check that neither B nor C is this matrix:
  if ((&B != this)&&(&C != this)) {
    // make the size of the result correct, and initiallize to all zeroes:
    maximize();
    v_index_mode Lm = ResultModeMax(B.Lmode, C.Lmode);
    v_index_mode Rm = ResultModeMax(B.Rmode, C.Rmode);
    int Ls = max(B.Lmaxindex(), C.Lmaxindex());
    int Rs = max(B.Rmaxindex(), C.Rmaxindex());
    if ((Lmode!=Lm)||(Rmode!=Rm)||(Lmaxindex()<Ls)||(Rmaxindex()<Rs)) {
      // fix up the result matrix in two steps: index modes first, then sizes
      // the result will automatically be zeroed
      reallocate(0,0,Lm,Rm); get(0,0) = 0; resize(Ls,Rs);
      // we need the above get() = 0 in case Lm == Rm == Index_S
    }
    else {
      // result's modes and sizes o.k., just zero and set maxindexes
      fill(0.0);
      Lmaxindex(Ls); Rmaxindex(Rs);
    }
    // Now do the subtraction:
    (*this) += B; (*this) -= C;
  }
  else if (&B == this) { // the first argument is this matrix
    // if necessary, resize this matrix to hold all elements
    int Ls = max(Lmaxindex(), C.Lmaxindex());
    int Rs = max(Rmaxindex(), C.Rmaxindex());
    if ((Lmaxindex() < Ls)||(Rmaxindex() < Rs)) {
      clean(); maximize();
    }
    v_index_mode Lm = ResultModeMax(Lmode, C.Lmode);
    v_index_mode Rm = ResultModeMax(Rmode, C.Rmode);
    if ((Lmode!=Lm)||(Rmode!=Rm)||(Lmaxindex()<Ls)||(Rmaxindex()<Rs)) {
      int sL = (Lm == Index_C) ? Ls + 1 : Ls;
      int sR = (Rm == Index_C) ? Rs + 1 : Rs;
      reallocate(sL,sR,Lm,Rm);
    }
    Lmaxindex(Ls); Rmaxindex(Rs);
    // Now do the subtraction:
    (*this) -= C;
  }
  else { // the second argument is this matrix
    (*this) *= -1;  // since it's the 2nd argument
    // if necessary, resize this matrix to hold all elements
    int Ls = max(Lmaxindex(), B.Lmaxindex());
    int Rs = max(Rmaxindex(), B.Rmaxindex());
    if ((Lmaxindex() < Ls)||(Rmaxindex() < Rs)) {
      clean(); maximize();
    }
    v_index_mode Lm = ResultModeMax(Lmode, B.Lmode);
    v_index_mode Rm = ResultModeMax(Rmode, B.Rmode);
    if ((Lmode!=Lm)||(Rmode!=Rm)||(Lmaxindex()<Ls)||(Rmaxindex()<Rs)) {
      int sL = (Lm == Index_C) ? Ls + 1 : Ls;
      int sR = (Rm == Index_C) ? Rs + 1 : Rs;
      reallocate(sL,sR,Lm,Rm);
    }
    Lmaxindex(Ls); Rmaxindex(Rs);
    // Now do the sum:
    (*this) += B;
  }
  return *this;
}

real_matrix & real_matrix::real(const complex_matrix & B)
{
  resize(B);
  if(B.is_empty())
    make_empty();
  else {
    Lmaxindex(B.Lmaxindex());
    Rmaxindex(B.Rmaxindex());
    int i,j;
    for (i = Lminindex(); i <= Lmaxindex(); ++i)
      for (j = Rminindex(); j <= Rmaxindex(); ++j)
	data[i][j] = B[i][j].real;
  }
  return *this;
}

real_matrix & real_matrix::imaginary(const complex_matrix & B)
{
  resize(B);
  if(B.is_empty())
    make_empty();
  else {
    Lmaxindex(B.Lmaxindex());
    Rmaxindex(B.Rmaxindex());
    int i,j;
    for (i = Lminindex(); i <= Lmaxindex(); ++i)
      for (j = Rminindex(); j <= Rmaxindex(); ++j)
	data[i][j] = B[i][j].imaginary;
  }
  return *this;
}

//-------------------------------------------------------------------------
// clean() clear elements beyond the valid index range

real_matrix & real_matrix::clean(void)
{
  // save index limits and maximize:
  int Lmax = Lmaxindexvalue;
  int Lmin = Lminindexvalue;
  int Rmax = Rmaxindexvalue;
  int Rmin = Rminindexvalue;
  maxindex(Lsize + Rsize);

  // now zero excess elements:
  int i,j;
  for (j = Rminindex(); j < Rmin; ++j)
    for (i = Lminindex(); i <= Lmaxindex(); ++i)
      data[i][j] = 0;
  for (j = Rmaxindex(); j > Rmax; --j)
    for (i = Lminindex(); i <= Lmaxindex(); ++i)
      data[i][j] = 0;
  for (i = Lmaxindex(); i > Lmax; --i)
    for (j = Rmin; j <= Rmax; ++j)
      data[i][j] = 0;
  for (i = Lminindex(); i < Lmin; ++i)
    for (j = Rmin; j <= Rmax; ++j)
      data[i][j] = 0;

  // restore index limits and return:
  Lmaxindex(Lmax);
  Rmaxindex(Rmax);
  return *this;
}

//---------------------------------------------------------------------
// other miscellaneous funtions:

real_matrix & real_matrix::diagonal(const double s)
{
  fill(0.0);
  int min = minindex();
  int max = maxindex();
  for(/*register*/ int i = min; i <= max; ++i)
    get(i,i) = s;
  return *this;
}

real_matrix & real_matrix::rowswap(const int n, const int m)
{
  // check for do nothing n == m case first:
  if (n == m) return *this;

  // n != m if we get here:
  const int nBad = 1; // flag values for n or m out of limits
  const int mBad = 2;
  int nCheck = 0;     // flags set if n or m out of limits
  int mCheck = 0;
  if ((n < Lminindexvalue)||(n > Lmaxindexvalue)) nCheck = nBad;
  if ((m < Lminindexvalue)||(m > Lmaxindexvalue)) mCheck = mBad;
  switch (nCheck + mCheck) {
  case 0: {
    // then both n and m are valid
    double * temp = data[n];
    data[n] = data[m];
    data[m] = temp;
    break;
  }
  case nBad: {
    // only m is valid, so fill with zeros
    real_vector y(data[m], Rsize, Rmode);  // alias the row as a vector
    y.fill(0.0);
    break;
  }
  case mBad: {
    // only n is valid, so fill with zeros
    real_vector y(data[n], Rsize, Rmode);  // alias the row as a vector
    y.fill(0.0);
    break;
  }
  case (nBad+mBad): {
    // both indexes are invalid, so do nothing
    break;
  }
  } // switch
  return *this;
}

real_matrix & real_matrix::apply(double (*f)(double))
{
  /*register*/ double *d, *limit;  // local pointers within a row
  /*register*/ int i;
  for(i = Lminindexvalue; i <= Lmaxindexvalue; ++i) {
    // make local pointers to the first and last elements in the row:
    limit = d = data[i];
    d += Rminindexvalue; limit += Rmaxindexvalue;
    // use the pointer to the first element as the loop variable:
    for ( ; d <= limit; ++d) *d = f(*d);
  }
  return *this;
}

real_matrix & real_matrix::apply(double (*f)(const double &))
{
  /*register*/ double *d, *limit;  // local pointers within a row
  /*register*/ int i;
  for(i = Lminindexvalue; i <= Lmaxindexvalue; ++i) {
    // make local pointers to the first and last elements in the row:
    limit = d = data[i];
    d += Rminindexvalue; limit += Rmaxindexvalue;
    // use the pointer to the first element as the loop variable:
    for ( ; d <= limit; ++d) *d = f(*d);
  }
  return *this;
}

real_matrix & real_matrix::apply(double (*f)(double, int, int))
{
  int i;                     // row counter (left index)
  /*register*/ double *d;       // inner loop pointer within a row
  /*register*/ int j, limit;     // inner loop counter and a limit
  for(i = Lminindexvalue; i <= Lmaxindexvalue; ++i) {
    j = Rminindexvalue; limit = Rmaxindexvalue;
    for (d = data[i] + j; j <= limit; ++d, ++j) *d = f(*d, i, j);
  }
  return *this;
}

real_matrix & real_matrix::apply(double (*f)(const double &, int, int))
{
  int i;                     // row counter (left index)
  /*register*/ double *d;       // inner loop pointer within a row
  /*register*/ int j, limit;     // inner loop counter and a limit
  for(i = Lminindexvalue; i <= Lmaxindexvalue; ++i) {
    j = Rminindexvalue; limit = Rmaxindexvalue;
    for (d = data[i] + j; j <= limit; ++d, ++j) *d = f(*d, i, j);
  }
  return *this;
}

// end of real_matrix definitions


// ************************************************************************
// complex_matrix definitions:


// matrix output:

ostream & operator << (ostream & s, const complex_matrix & A)
{
  std::streamsize w = s.width();
  if (A.Lminindex() <= A.Lmaxindex()) { // matrix isn't empty
    { // output first row (using a row alias)
      complex_vector v(const_cast<Complex *>(A[A.Lminindex()]), A.Rsize, A.Rmode);
      v.maxindex(A.Rmaxindex());  // set same valid index range
      s << v;
    }
    // output remaining rows
    for (int i = A.Lminindex() + 1; i <= A.Lmaxindex(); ++i) {
      complex_vector v(const_cast<Complex *>(A[i]), A.Rsize, A.Rmode);
      v.maxindex(A.Rmaxindex());
      s << _out_separator << setw(w) << v;
    }
  }
  return s << setw(0);  // just in case no output was generated
}

complex_matrix & complex_matrix::show(ostream & s)
{
  s << endl << *this << endl;
  return *this;
}

complex_matrix & complex_matrix::show()
{
  return show(cout);
}

const complex_matrix & complex_matrix::show(ostream & s) const
{
  s << endl << *this << endl;
  return *this;
}

const complex_matrix & complex_matrix::show() const
{
  return show(cout);
}

string complex_matrix::out_separator()
{ return _out_separator; }

string complex_matrix::out_separator(const string & s)
{ string temp = _out_separator; _out_separator = s; return temp; }

string complex_matrix::out_separator(const char * const s)
{ string temp = _out_separator; _out_separator = s; return temp; }

string complex_matrix::out_separator(const char s)
{ string temp = _out_separator; _out_separator = s; return temp; }

//-------------------------------------------------------------------------
// construct() do the real work of matrix construction

void complex_matrix::construct(const int n, const int m,
			   const v_index_mode tl,
			   const v_index_mode tr)
{
  // set up some internal constants
  internal_Lsize = (n >= 0) ? n : 0;
  internal_Rsize = (m >= 0) ? m : 0;
  internal_Lmode = tl;
  internal_Rmode = tr;
  trashptr = & trash;

  // determine number of columns and rows requested:
  int nrows = Lsize; if (Lmode == Index_S) nrows += (nrows + 1);
  int ncols = Rsize; if (Rmode == Index_S) ncols += (ncols + 1);

  // allocate memory for the array of pointers to rows, if required:
  data = delete_pointer_data = (nrows) ?  new Complex *[nrows] : 0;

  // allocate memory for the array of data elements, if required:
  int nelem = ncols * nrows;
  if (nelem) {
    // need to allocate memory
    delete_pointer_rows = new Complex[nelem];
    if (delete_pointer_rows == 0) {
      // memory alloc failed, so give back pointer array as well
      delete [] delete_pointer_data;
      data = delete_pointer_data = 0;
    }
  }
  else {
    // no allocation necessary, nelem == 0
    delete_pointer_rows = 0;
  }

  // now that allocation has been attempted, update sizes and index limits
  if (data == 0) {
    // no pointer array
    internal_Lsize = nrows = 0; // in case they weren't already
    if (Lmode == Index_S) data = & trashptr; //so **data = trash
    Lmaxindexvalue = (Lmode == Index_1) ? 0 : -1;
  }
  else {
    // pointer array exists, original Lsize and nrows o.k.
    switch (Lmode) {
    case Index_C: {
      Lmaxindexvalue = Lsize - 1;
      break;
    }
    case Index_1: {
      Lmaxindexvalue = Lsize;
      --data;        // so *delete_pointer_data = data[1]
      break;
    }
    case Index_S: {
      Lmaxindexvalue = Lsize;
      data += Lsize; // so *delete_pointer_data = data[-Lsize]
      break;
    }
    } //switch
  } //else
  // final left index setup step is to set Lminindexvalue:
  switch (Lmode) {
  case Index_C: { Lminindexvalue = 0; break; }
  case Index_1: { Lminindexvalue = 1; break; }
  case Index_S: { Lminindexvalue = -Lmaxindexvalue; break; }
  }
  // setup of left index complete
  // now for the right index:
  if (delete_pointer_rows == 0) {
    // no data elements
    internal_Rsize = ncols = 0; // in case they weren't already
    Rmaxindexvalue = (Rmode == Index_1) ? 0 : -1;
  }
  else {
    // data array exists, original Rsize and ncols o.k.
    Rmaxindexvalue = (Rmode == Index_C) ? Rsize - 1 : Rsize;
  }
  switch (Rmode) {
  case Index_C: { Rminindexvalue = 0; break; }
  case Index_1: { Rminindexvalue = 1; break; }
  case Index_S: { Rminindexvalue = -Rmaxindexvalue; break; }
  }
  // setup of right index complete

    
  // now fill pointer array with pointers to the [0] element of each row
  int i, offset; // offset is the pointer offset into each row due to Rmode
  switch (Rmode) {
  default:
  case Index_C: { offset = 0; break; }
  case Index_1: { offset = -1; break; }
  case Index_S: { offset = Rsize; break; }
  }
  if (ncols == 0) offset = 0; // no offset if no data
  for(i = Lminindexvalue; i <= Lmaxindexvalue; ++i, offset += ncols)
    // this loop will only execute if pointer memory was allocated
    data[i] = delete_pointer_rows + offset;

} // construct()

//-------------------------------------------------------------------------
// copy constructors:

complex_matrix::complex_matrix(const complex_matrix & B)
  : Lsize(internal_Lsize), Rsize(internal_Rsize), 
    Lmode(internal_Lmode), Rmode(internal_Rmode)
{
  construct(B.Lsize, B.Rsize, B.Lmode, B.Rmode);
  int i,j;
  // the following loop will work even if alloc for B failed and
  // B.Lmode == Index_S.  In this case, the contents of trash will be
  // copied.
  for (i = Lminindex(); i <= Lmaxindex(); ++i)
    for (j = Rminindex(); j <= Rmaxindex(); ++j)
      data[i][j] = B[i][j];

  if(B.is_empty())
    make_empty();
  else {
    Lmaxindex(B.Lmaxindex());
    Rmaxindex(B.Rmaxindex());
  }
}

complex_matrix::complex_matrix(const real_matrix & B)
  : Lsize(internal_Lsize), Rsize(internal_Rsize), 
    Lmode(internal_Lmode), Rmode(internal_Rmode)
{
  construct(B.Lsize, B.Rsize, B.Lmode, B.Rmode);
  int i,j;
  // the following loop will work even if alloc for B failed and
  // B.Lmode == Index_S.  In this case, the contents of trash will be
  // copied.
  for (i = Lminindex(); i <= Lmaxindex(); ++i)
    for (j = Rminindex(); j <= Rmaxindex(); ++j)
      data[i][j] = B[i][j];

  if(B.is_empty())
    make_empty();
  else {
    Lmaxindex(B.Lmaxindex());
    Rmaxindex(B.Rmaxindex());
  }
}

complex_matrix::complex_matrix(const real_vector & v)
  : Lsize(internal_Lsize), Rsize(internal_Rsize), 
    Lmode(internal_Lmode), Rmode(internal_Rmode)
{
  construct(v.size, 1, v.mode, Index_1);
  // the following loop will work even if alloc for v failed and
  // v.mode == Index_S.  In this case, the contents of trash will be
  // copied.
  for (int i = Lminindex(); i <= Lmaxindex(); ++i)
    data[i][1] = v[i];

  Lmaxindexvalue = v.maxindex();
  Lminindexvalue = v.minindex();
}

complex_matrix::complex_matrix(const complex_vector & v)
  : Lsize(internal_Lsize), Rsize(internal_Rsize), 
    Lmode(internal_Lmode), Rmode(internal_Rmode)
{
  construct(v.size, 1, v.mode, Index_1);
  // the following loop will work even if alloc for v failed and
  // v.mode == Index_S.  In this case, the contents of trash will be
  // copied.
  for (int i = Lminindex(); i <= Lmaxindex(); ++i)
    data[i][1] = v[i];

  Lmaxindexvalue = v.maxindex();
  Lminindexvalue = v.minindex();
}

//-------------------------------------------------------------------------
// reindex()  change the index modes

complex_matrix & complex_matrix::reindex(const v_index_mode Lnew, const v_index_mode Rnew)
{
  if (Lnew == Lmode && Rnew == Rmode) return *this;

  // some index change is needed; calculate dimensions of matrix
  int nrows = (Lmode == Index_S) ? 2*Lsize + 1 : Lsize;
  int ncols = (Rmode == Index_S) ? 2*Rsize + 1 : Rsize;
  
  // any dimension with new Index_S needs an odd number of values
  if (Lnew == Index_S && (nrows>>1)<<1 == nrows) {  // nrows even
    // since nrows even, Lmode can't be Index_S
    reallocate(Lsize+1,Rsize,Lmode,Rmode);
    nrows = Lsize; // the new Lsize
  }
  if (Rnew == Index_S && (ncols>>1)<<1 == ncols) {  // ncols even
    reallocate(Lsize,Rsize+1,Lmode,Rmode);
    ncols = Rsize; // the new Rsize
  }

  // finally, if resulting matrix is empty, just reallocate to fix modes
  if (delete_pointer_rows == 0)  {
    reallocate(0,0,Lnew,Rnew);
    return *this;
  }

  // if here, the matrix has some elements

  // set up new left indexing:
  if (Lnew != Lmode) { // reindexing required of Left Index
    int ptr_offset; // offset into row pointer array for data[0]
    switch (Lnew) {
    default:
      { return *this; } // unknown mode, so do nothing
    case Index_C:
      { ptr_offset = 0; break; }
    case Index_1:
      { ptr_offset = -1; break; }
    case Index_S:
      { ptr_offset = (nrows - 1)/2; break; }
    } // switch
    internal_Lsize = (Lnew == Index_S) ? ptr_offset : nrows;
    Lminindexvalue = -ptr_offset; 
    Lmaxindexvalue = Lminindexvalue + nrows - 1;
    data = delete_pointer_data + ptr_offset;
    internal_Lmode = Lnew;
  }

  // now for the right indexing:
  if (Rnew != Rmode) {
    int ptr_offset; // pointer offset into a row to data[][0]

    switch (Rnew) { // the new offset
    default:
      { return *this; } // unknown mode, so do nothing
    case Index_C:
      { ptr_offset = 0; break; }
    case Index_1:
      { ptr_offset = -1; break; }
    case Index_S:
      { ptr_offset = (ncols - 1)/2; break; }
    } // switch
    int old_Rsize = Rsize;  // save for switch below
    internal_Rsize = (Rnew == Index_S) ? ptr_offset : ncols;
    Rminindexvalue = -ptr_offset; 
    Rmaxindexvalue = Rminindexvalue + ncols - 1;
    switch (Rmode) { // subtract off the old offset
    default:
    case Index_C:
      { ptr_offset -= 0; break; }
    case Index_1:
      { ptr_offset -= -1; break; }
    case Index_S:
      { ptr_offset -= old_Rsize; break; }
    }
    // now ptr_offset is how far we must adjust each row pointer
    for (int i = Lminindexvalue; i <= Lmaxindexvalue; ++i)
      data[i] += ptr_offset;
    internal_Rmode = Rnew;
  } // if (Rnew != Rmode)  

  return *this;
}

//-------------------------------------------------------------------------
// constfill()  fill the matrix with a constant value
//              Note: data subset control will affect elements copied

void complex_matrix::constfill(const Complex f)
{
  for (int i = Lminindex(); i <= Lmaxindex(); ++i)
    for (int j = Rminindex(); j <= Rmaxindex(); ++j)
      data[i][j] = f;
}

//-------------------------------------------------------------------------
// fillall()  fill regardless of data subset control

complex_matrix & complex_matrix::fillall(const Complex f)
{
  int save_Lmax = Lmaxindexvalue; 
  int save_Rmax = Rmaxindexvalue;
  int save_Lmin = Lminindexvalue; 
  int save_Rmin = Rminindexvalue;

  maxindex(Lsize+Rsize); // access full matrix
  constfill(f);
  Lmaxindexvalue = save_Lmax; Rmaxindexvalue = save_Rmax;  // restore subset
  Lminindexvalue = save_Lmin; Rminindexvalue = save_Rmin;
  return *this;
}

//-------------------------------------------------------------------------
// fill a row or column from a vector

complex_matrix & complex_matrix::fillrow(const int n, const real_vector & v)
{
  if ((n >= Lminindex())&&(n <= Lmaxindex())) {  // then n is o.k.
    for (int i = Rminindex(); i <= Rmaxindex(); ++i)
      data[n][i] = v.read(i);
  }
  return *this;
}

complex_matrix & complex_matrix::fillrow(const int n, const complex_vector & v)
{
  if ((n >= Lminindex())&&(n <= Lmaxindex())) {  // then n is o.k.
    for (int i = Rminindex(); i <= Rmaxindex(); ++i)
      data[n][i] = v.read(i);
  }
  return *this;
}

complex_matrix & complex_matrix::fillcol(const int n, const real_vector & v)
{
  if ((n >= Rminindex())&&(n <= Rmaxindex())) {  // then n is o.k.
    for (int i = Lminindex(); i <= Lmaxindex(); ++i)
      data[i][n] = v.read(i);
  }
  return *this;
}

complex_matrix & complex_matrix::fillcol(const int n, const complex_vector & v)
{
  if ((n >= Rminindex())&&(n <= Rmaxindex())) {  // then n is o.k.
    for (int i = Lminindex(); i <= Lmaxindex(); ++i)
      data[i][n] = v.read(i);
  }
  return *this;
}

//-------------------------------------------------------------------------
// Lmaxindex(n), Rmaxindex(n), make_empty()

int complex_matrix::Lmaxindex(const int n)
{
  if (Lsize == 0 && Lmode != Index_S)
    // can't change the maxindex if no data elements
    return Lmaxindex();
  else {
    switch (Lmode) {
    case Index_C: {
      Lmaxindexvalue = min((Lsize - 1), max(0,n));
      break;
    }
    case Index_1: {
      Lmaxindexvalue = min(Lsize, max(1,n));
      break;
    }
    case Index_S: {
      Lmaxindexvalue = min(Lsize, max(0,n));
      Lminindexvalue = -Lmaxindexvalue;
      break;
    }
    } //switch
    return Lmaxindexvalue;
  } //else
}

int complex_matrix::Rmaxindex(const int n)
{
  if (Rsize == 0 && Rmode != Index_S)
    // can't change the maxindex if no data elements
    return Rmaxindex();
  else {
    switch (Rmode) {
    case Index_C: {
      Rmaxindexvalue = min((Rsize - 1), max(0,n));
      break;
    }
    case Index_1: {
      Rmaxindexvalue = min(Rsize, max(1,n));
      break;
    }
    case Index_S: {
      Rmaxindexvalue = min(Rsize, max(0,n));
      Rminindexvalue = -Rmaxindexvalue;
      break;
    }
    } //switch
    return Rmaxindexvalue;
  } //else
}

complex_matrix & complex_matrix::make_empty()
{
  switch (Lmode) {
  case Index_C:
    Lmaxindexvalue = -1;
    Lminindexvalue = 0;
    break;

  case Index_1:
    Lmaxindexvalue = 0;
    Lminindexvalue = 1;
    break;

  case Index_S:
    Lmaxindexvalue = -1;
    Lminindexvalue = 1;
    break;

  }
  
  switch (Rmode) {
  case Index_C:
    Rmaxindexvalue = -1;
    Rminindexvalue = 0;
    break;

  case Index_1:
    Rmaxindexvalue = 0;
    Rminindexvalue = 1;
    break;

  case Index_S:
    Rmaxindexvalue = -1;
    Rminindexvalue = 1;
    break;

  }
  
  return *this;
}


//-------------------------------------------------------------------------
// copy() copy contents of another matrix

complex_matrix & complex_matrix::copy(const complex_matrix & B)
{
  // check for self copy: "A.copy(A)", and do something intelligent:
  if (this == & B) { clean(); return *this; }

  // otherwise, not a self copy, so:
  maxindex(Lsize+Rsize);  // make all data accessible
  constfill(0);           // and clear the matrix
  // now set data subset on the result:
  if(B.is_empty())
    make_empty();
  else {
    Lmaxindex(B.Lmaxindex());
    Rmaxindex(B.Rmaxindex());
    // loop over the index range intersection and copy:
    int Lmax = min(Lmaxindex(), B.Lmaxindex());
    int Lmin = max(Lminindex(), B.Lminindex());
    int Rmax = min(Rmaxindex(), B.Rmaxindex());
    int Rmin = max(Rminindex(), B.Rminindex());
    int i,j;
    for (i = Lmin; i <= Lmax; ++i)
      for (j = Rmin; j <= Rmax; ++j)
	data[i][j] = B[i][j];
  }
  return *this;
}

complex_matrix & complex_matrix::copy(const real_matrix & B)
{
  maxindex(Lsize+Rsize);  // make all data accessible
  constfill(0);           // and clear the matrix
  // now set data subset on the result:
  if(B.is_empty())
    make_empty();
  else {
    Lmaxindex(B.Lmaxindex());
    Rmaxindex(B.Rmaxindex());
    // loop over the index range intersection and copy:
    int Lmax = min(Lmaxindex(), B.Lmaxindex());
    int Lmin = max(Lminindex(), B.Lminindex());
    int Rmax = min(Rmaxindex(), B.Rmaxindex());
    int Rmin = max(Rminindex(), B.Rminindex());
    int i,j;
    for (i = Lmin; i <= Lmax; ++i)
      for (j = Rmin; j <= Rmax; ++j)
	data[i][j] = B[i][j];
  }
  return *this;
}

//-------------------------------------------------------------------------
// reallocate() change size or indexing modes

complex_matrix & complex_matrix::reallocate
  (const int n, const int m, const v_index_mode tl, const v_index_mode tr)
{
  // just return if reallocation is unnecessary:
  if ((Lsize == n)&&(Rsize == m)&&(Lmode == tl)&&(Rmode == tr)) return *this;

  // if we're here, we have to reallocate:
  // save the data subset control values and then maximize:
  int new_Lmax = Lmaxindexvalue;
  int new_Rmax = Rmaxindexvalue;
  maxindex(Lsize + Rsize);

  // check if data subset control was turned off; if so, keep turned off
  if (new_Lmax == Lmaxindexvalue) new_Lmax = n; 
  if (new_Rmax == Rmaxindexvalue) new_Rmax = m; 

  // now save the necessary private data members:
  int Lmax = Lmaxindexvalue;
  int Lmin = Lminindexvalue;
  int Rmax = Rmaxindexvalue;
  int Rmin = Rminindexvalue;
  Complex **old_data = data; 
  Complex **delete1 = delete_pointer_data;
  Complex  *delete2 = delete_pointer_rows;

  // with this done, now construct the new matrix:
  construct(n, m, tl, tr);
  constfill(0);

  // now copy in the data using the saved private data members:
  Lmax = min(Lmaxindex(), Lmax);
  Lmin = max(Lminindex(), Lmin);
  Rmax = min(Rmaxindex(), Rmax);
  Rmin = max(Rminindex(), Rmin);
  int i,j;
  for (i = Lmin; i <= Lmax; ++i)
    for (j = Rmin; j <= Rmax; ++j)
      data[i][j] = old_data[i][j];

  // set data subset control
  Lmaxindex(new_Lmax);
  Rmaxindex(new_Rmax);

  // finally delete the old memory allocation:
  delete [] delete1;
  delete [] delete2;

  return *this;
}

//-------------------------------------------------------------------------
// Basic assignment operator

complex_matrix & complex_matrix::operator = (const complex_matrix & B)
{
  // check for self assignment: "A = A", and do something intelligent:
  if (this == & B) { clean(); return *this; }

  // reallocate memory if needed:
  maximize();
  if ((B.Lmode != Lmode)||(B.Rmode != Rmode)||
      (B.Lmaxindex() > Lmaxindexvalue)||(B.Rmaxindex() > Rmaxindexvalue)) {
    delete [] delete_pointer_data; delete [] delete_pointer_rows;
    int newLsize = (B.Lmode == Index_C) ? B.Lmaxindex()+1: B.Lmaxindex();
    int newRsize = (B.Rmode == Index_C) ? B.Rmaxindex()+1: B.Rmaxindex();
    construct(newLsize, newRsize, B.Lmode, B.Rmode);
  }

  copy(B);
  return *this;
}

complex_matrix & complex_matrix::operator = (const real_matrix & B)
{
  // reallocate memory if needed:
  maximize();
  if ((B.Lmode != Lmode)||(B.Rmode != Rmode)||
      (B.Lmaxindex() > Lmaxindexvalue)||(B.Rmaxindex() > Rmaxindexvalue)) {
    delete [] delete_pointer_data; delete [] delete_pointer_rows;
    int newLsize = (B.Lmode == Index_C) ? B.Lmaxindex()+1: B.Lmaxindex();
    int newRsize = (B.Rmode == Index_C) ? B.Rmaxindex()+1: B.Rmaxindex();
    construct(newLsize, newRsize, B.Lmode, B.Rmode);
  }

  copy(B);
  return *this;
}

//---------------------------------------------------------------------
// other assignment operators (no memory reallocation for these):

complex_matrix & complex_matrix::operator += (const Complex s)
{
  /*register*/ Complex *d, *limit;  // local pointers within a row
  /*register*/ int i;
  for(i = Lminindexvalue; i <= Lmaxindexvalue; ++i) {
    // make local pointers to the first and last elements in the row:
    limit = d = data[i];
    d += Rminindexvalue; limit += Rmaxindexvalue;
    // use the pointer to the first element as the loop variable:
    while (d <= limit) *(d++) += s;
  }
  return *this;
}

complex_matrix & complex_matrix::operator -= (const Complex s)
{
  /*register*/ Complex *d, *limit;  // local pointers within a row
  /*register*/ int i;
  for(i = Lminindexvalue; i <= Lmaxindexvalue; ++i) {
    // make local pointers to the first and last elements in the row:
    limit = d = data[i];
    d += Rminindexvalue; limit += Rmaxindexvalue;
    // use the pointer to the first element as the loop variable:
    while (d <= limit) *(d++) -= s;
  }
  return *this;
}

complex_matrix & complex_matrix::operator *= (const Complex s)
{
  /*register*/ Complex *d, *limit;  // local pointers within a row
  /*register*/ int i;
  for(i = Lminindexvalue; i <= Lmaxindexvalue; ++i) {
    // make local pointers to the first and last elements in the row:
    limit = d = data[i];
    d += Rminindexvalue; limit += Rmaxindexvalue;
    // use the pointer to the first element as the loop variable:
    while (d <= limit) *(d++) *= s;
  }
  return *this;
}

complex_matrix & complex_matrix::operator /= (const Complex s)
{
  /*register*/ Complex *d, *limit;  // local pointers within a row
  /*register*/ int i;
  for(i = Lminindexvalue; i <= Lmaxindexvalue; ++i) {
    // make local pointers to the first and last elements in the row:
    limit = d = data[i];
    d += Rminindexvalue; limit += Rmaxindexvalue;
    // use the pointer to the first element as the loop variable:
    while (d <= limit) *(d++) /= s;
  }
  return *this;
}

complex_matrix & complex_matrix::operator += (const complex_matrix & B)
{
  clean();  // ensure nonvalid elements are 0
  // increase target's valid data range if necessary:
  if (Lmaxindexvalue < B.Lmaxindex()) Lmaxindex(B.Lmaxindex());
  if (Rmaxindexvalue < B.Rmaxindex()) Rmaxindex(B.Rmaxindex());
  // find the intersection of the valid data ranges:
  int Lmax = min(Lmaxindex(), B.Lmaxindex());
  int Lmin = max(Lminindex(), B.Lminindex());
  int Rmax = min(Rmaxindex(), B.Rmaxindex());
  int Rmin = max(Rminindex(), B.Rminindex());
  // loop over the intersection of the ranges:
  /*register*/ int i,j;
  for (i = Lmin; i <= Lmax; ++i)
    for (j = Rmin; j <= Rmax; ++j)
      data[i][j] += B[i][j];
  return *this;
}

complex_matrix & complex_matrix::operator -= (const complex_matrix & B)
{
  clean();  // ensure nonvalid elements are 0
  // increase target's valid data range if necessary:
  if (Lmaxindexvalue < B.Lmaxindex()) Lmaxindex(B.Lmaxindex());
  if (Rmaxindexvalue < B.Rmaxindex()) Rmaxindex(B.Rmaxindex());
  // find the intersection of the valid data ranges:
  int Lmax = min(Lmaxindex(), B.Lmaxindex());
  int Lmin = max(Lminindex(), B.Lminindex());
  int Rmax = min(Rmaxindex(), B.Rmaxindex());
  int Rmin = max(Rminindex(), B.Rminindex());
  // loop over the intersection of the ranges:
  /*register*/ int i,j;
  for (i = Lmin; i <= Lmax; ++i)
    for (j = Rmin; j <= Rmax; ++j)
      data[i][j] -= B[i][j];
  return *this;
}

complex_matrix & complex_matrix::operator += (const real_matrix & B)
{
  clean();  // ensure nonvalid elements are 0
  // increase target's valid data range if necessary:
  if (Lmaxindexvalue < B.Lmaxindex()) Lmaxindex(B.Lmaxindex());
  if (Rmaxindexvalue < B.Rmaxindex()) Rmaxindex(B.Rmaxindex());
  // find the intersection of the valid data ranges:
  int Lmax = min(Lmaxindex(), B.Lmaxindex());
  int Lmin = max(Lminindex(), B.Lminindex());
  int Rmax = min(Rmaxindex(), B.Rmaxindex());
  int Rmin = max(Rminindex(), B.Rminindex());
  // loop over the intersection of the ranges:
  /*register*/ int i,j;
  for (i = Lmin; i <= Lmax; ++i)
    for (j = Rmin; j <= Rmax; ++j)
      data[i][j] += B[i][j];
  return *this;
}

complex_matrix & complex_matrix::operator -= (const real_matrix & B)
{
  clean();  // ensure nonvalid elements are 0
  // increase target's valid data range if necessary:
  if (Lmaxindexvalue < B.Lmaxindex()) Lmaxindex(B.Lmaxindex());
  if (Rmaxindexvalue < B.Rmaxindex()) Rmaxindex(B.Rmaxindex());
  // find the intersection of the valid data ranges:
  int Lmax = min(Lmaxindex(), B.Lmaxindex());
  int Lmin = max(Lminindex(), B.Lminindex());
  int Rmax = min(Rmaxindex(), B.Rmaxindex());
  int Rmin = max(Rminindex(), B.Rminindex());
  // loop over the intersection of the ranges:
  /*register*/ int i,j;
  for (i = Lmin; i <= Lmax; ++i)
    for (j = Rmin; j <= Rmax; ++j)
      data[i][j] -= B[i][j];
  return *this;
}

//-------------------------------------------------------------------------
// matrix math member fcns (other than assignment)

complex_matrix & complex_matrix::add
(const complex_matrix & B, const complex_matrix & C)
{
  // first check that neither B nor C is this matrix:
  if ((&B != this)&&(&C != this)) {
    // make the size of the result correct, and initiallize to all zeroes:
    maximize();
    v_index_mode Lm = ResultModeMax(B.Lmode, C.Lmode);
    v_index_mode Rm = ResultModeMax(B.Rmode, C.Rmode);
    int Ls = max(B.Lmaxindex(), C.Lmaxindex());
    int Rs = max(B.Rmaxindex(), C.Rmaxindex());
    if ((Lmode!=Lm)||(Rmode!=Rm)||(Lmaxindex()<Ls)||(Rmaxindex()<Rs)) {
      // fix up the result matrix in two steps: index modes first, then sizes
      // the result will automatically be zeroed
      reallocate(0,0,Lm,Rm); get(0,0) = 0; resize(Ls,Rs);
      // we need the above get() = 0 in case Lm == Rm == Index_S
    }
    else {
      // result's modes and sizes o.k., just zero and set maxindexes
      fill(0.0);
      Lmaxindex(Ls); Rmaxindex(Rs);
    }
    // Now do the sum:
    (*this) += B; (*this) += C;
  }
  else if (&B == this) { // the first argument is this matrix
    // if necessary, resize this matrix to hold all elements in the sum
    int Ls = max(Lmaxindex(), C.Lmaxindex());
    int Rs = max(Rmaxindex(), C.Rmaxindex());
    if ((Lmaxindex() < Ls)||(Rmaxindex() < Rs)) {
      clean(); maximize();
    }
    v_index_mode Lm = ResultModeMax(Lmode, C.Lmode);
    v_index_mode Rm = ResultModeMax(Rmode, C.Rmode);
    if ((Lmode!=Lm)||(Rmode!=Rm)||(Lmaxindex()<Ls)||(Rmaxindex()<Rs)) {
      int sL = (Lm == Index_C) ? Ls + 1 : Ls;
      int sR = (Rm == Index_C) ? Rs + 1 : Rs;
      reallocate(sL,sR,Lm,Rm);
    }
    Lmaxindex(Ls); Rmaxindex(Rs);
    // Now do the sum:
    (*this) += C;
  }
  else { // the second argument is this matrix
    // if necessary, resize this matrix to hold all elements in the sum
    int Ls = max(Lmaxindex(), B.Lmaxindex());
    int Rs = max(Rmaxindex(), B.Rmaxindex());
    if ((Lmaxindex() < Ls)||(Rmaxindex() < Rs)) {
      clean(); maximize();
    }
    v_index_mode Lm = ResultModeMax(Lmode, B.Lmode);
    v_index_mode Rm = ResultModeMax(Rmode, B.Rmode);
    if ((Lmode!=Lm)||(Rmode!=Rm)||(Lmaxindex()<Ls)||(Rmaxindex()<Rs)) {
      int sL = (Lm == Index_C) ? Ls + 1 : Ls;
      int sR = (Rm == Index_C) ? Rs + 1 : Rs;
      reallocate(sL,sR,Lm,Rm);
    }
    Lmaxindex(Ls); Rmaxindex(Rs);
    // Now do the sum:
    (*this) += B;
  }
  return *this;
}

complex_matrix & complex_matrix::add
(const complex_matrix & B, const real_matrix & C)
{
  // first check that B is not this matrix:
  if (&B != this) {
    // make the size of the result correct, and initiallize to all zeroes:
    maximize();
    v_index_mode Lm = ResultModeMax(B.Lmode, C.Lmode);
    v_index_mode Rm = ResultModeMax(B.Rmode, C.Rmode);
    int Ls = max(B.Lmaxindex(), C.Lmaxindex());
    int Rs = max(B.Rmaxindex(), C.Rmaxindex());
    if ((Lmode!=Lm)||(Rmode!=Rm)||(Lmaxindex()<Ls)||(Rmaxindex()<Rs)) {
      // fix up the result matrix in two steps: index modes first, then sizes
      // the result will automatically be zeroed
      reallocate(0,0,Lm,Rm); get(0,0) = 0; resize(Ls,Rs);
      // we need the above get() = 0 in case Lm == Rm == Index_S
    }
    else {
      // result's modes and sizes o.k., just zero and set maxindexes
      fill(0.0);
      Lmaxindex(Ls); Rmaxindex(Rs);
    }
    // Now do the sum:
    (*this) += B; (*this) += C;
  }
  else { // B is this matrix
    // if necessary, resize this matrix to hold all elements in the sum
    int Ls = max(Lmaxindex(), C.Lmaxindex());
    int Rs = max(Rmaxindex(), C.Rmaxindex());
    if ((Lmaxindex() < Ls)||(Rmaxindex() < Rs)) {
      clean(); maximize();
    }
    v_index_mode Lm = ResultModeMax(Lmode, C.Lmode);
    v_index_mode Rm = ResultModeMax(Rmode, C.Rmode);
    if ((Lmode!=Lm)||(Rmode!=Rm)||(Lmaxindex()<Ls)||(Rmaxindex()<Rs)) {
      int sL = (Lm == Index_C) ? Ls + 1 : Ls;
      int sR = (Rm == Index_C) ? Rs + 1 : Rs;
      reallocate(sL,sR,Lm,Rm);
    }
    Lmaxindex(Ls); Rmaxindex(Rs);
    // Now do the sum:
    (*this) += C;
  }
  return *this;
}

complex_matrix & complex_matrix::add
(const real_matrix & B, const real_matrix & C)
{
  // make the size of the result correct, and initiallize to all zeroes:
  maximize();
  v_index_mode Lm = ResultModeMax(B.Lmode, C.Lmode);
  v_index_mode Rm = ResultModeMax(B.Rmode, C.Rmode);
  int Ls = max(B.Lmaxindex(), C.Lmaxindex());
  int Rs = max(B.Rmaxindex(), C.Rmaxindex());
  if ((Lmode != Lm)||(Rmode != Rm)||(Lmaxindex() < Ls)||(Rmaxindex() < Rs)) {
    // fix up the result matrix in two steps: index modes first, then sizes
    // the result will automatically be zeroed
    reallocate(0,0,Lm,Rm); get(0,0) = 0; resize(Ls,Rs);
    // we need the above get() = 0 in case Lm == Rm == Index_S
  }
  else {
    // result's modes and sizes o.k., just zero and set maxindexes
    fill(0.0);
    Lmaxindex(Ls); Rmaxindex(Rs);
  }

  // Now do the sum and return:
  (*this) += B; (*this) += C;
  return *this;
}

complex_matrix & complex_matrix::sub
(const complex_matrix & B, const complex_matrix & C)
{
  // first check that neither B nor C is this matrix:
  if ((&B != this)&&(&C != this)) {
    // make the size of the result correct, and initiallize to all zeroes:
    maximize();
    v_index_mode Lm = ResultModeMax(B.Lmode, C.Lmode);
    v_index_mode Rm = ResultModeMax(B.Rmode, C.Rmode);
    int Ls = max(B.Lmaxindex(), C.Lmaxindex());
    int Rs = max(B.Rmaxindex(), C.Rmaxindex());
    if ((Lmode!=Lm)||(Rmode!=Rm)||(Lmaxindex()<Ls)||(Rmaxindex()<Rs)) {
      // fix up the result matrix in two steps: index modes first, then sizes
      // the result will automatically be zeroed
      reallocate(0,0,Lm,Rm); get(0,0) = 0; resize(Ls,Rs);
      // we need the above get() = 0 in case Lm == Rm == Index_S
    }
    else {
      // result's modes and sizes o.k., just zero and set maxindexes
      fill(0.0);
      Lmaxindex(Ls); Rmaxindex(Rs);
    }
    // Now do the subtraction:
    (*this) += B; (*this) -= C;
  }
  else if (&B == this) { // the first argument is this matrix
    // if necessary, resize this matrix to hold all elements
    int Ls = max(Lmaxindex(), C.Lmaxindex());
    int Rs = max(Rmaxindex(), C.Rmaxindex());
    if ((Lmaxindex() < Ls)||(Rmaxindex() < Rs)) {
      clean(); maximize();
    }
    v_index_mode Lm = ResultModeMax(Lmode, C.Lmode);
    v_index_mode Rm = ResultModeMax(Rmode, C.Rmode);
    if ((Lmode!=Lm)||(Rmode!=Rm)||(Lmaxindex()<Ls)||(Rmaxindex()<Rs)) {
      int sL = (Lm == Index_C) ? Ls + 1 : Ls;
      int sR = (Rm == Index_C) ? Rs + 1 : Rs;
      reallocate(sL,sR,Lm,Rm);
    }
    Lmaxindex(Ls); Rmaxindex(Rs);
    // Now do the subtraction:
    (*this) -= C;
  }
  else { // the second argument is this matrix
    (*this) *= -1;  // since it's the 2nd argument
    // if necessary, resize this matrix to hold all elements
    int Ls = max(Lmaxindex(), B.Lmaxindex());
    int Rs = max(Rmaxindex(), B.Rmaxindex());
    if ((Lmaxindex() < Ls)||(Rmaxindex() < Rs)) {
      clean(); maximize();
    }
    v_index_mode Lm = ResultModeMax(Lmode, B.Lmode);
    v_index_mode Rm = ResultModeMax(Rmode, B.Rmode);
    if ((Lmode!=Lm)||(Rmode!=Rm)||(Lmaxindex()<Ls)||(Rmaxindex()<Rs)) {
      int sL = (Lm == Index_C) ? Ls + 1 : Ls;
      int sR = (Rm == Index_C) ? Rs + 1 : Rs;
      reallocate(sL,sR,Lm,Rm);
    }
    Lmaxindex(Ls); Rmaxindex(Rs);
    // Now do the sum:
    (*this) += B;
  }
  return *this;
}

complex_matrix & complex_matrix::sub
(const complex_matrix & B, const real_matrix & C)
{
  // first check that B is not this matrix:
  if (&B != this) {
    // make the size of the result correct, and initiallize to all zeroes:
    maximize();
    v_index_mode Lm = ResultModeMax(B.Lmode, C.Lmode);
    v_index_mode Rm = ResultModeMax(B.Rmode, C.Rmode);
    int Ls = max(B.Lmaxindex(), C.Lmaxindex());
    int Rs = max(B.Rmaxindex(), C.Rmaxindex());
    if ((Lmode!=Lm)||(Rmode!=Rm)||(Lmaxindex()<Ls)||(Rmaxindex()<Rs)) {
      // fix up the result matrix in two steps: index modes first, then sizes
      // the result will automatically be zeroed
      reallocate(0,0,Lm,Rm); get(0,0) = 0; resize(Ls,Rs);
      // we need the above get() = 0 in case Lm == Rm == Index_S
    }
    else {
      // result's modes and sizes o.k., just zero and set maxindexes
      fill(0.0);
      Lmaxindex(Ls); Rmaxindex(Rs);
    }
    // Now do the subtraction:
    (*this) += B; (*this) -= C;
  }
  else { // B is this matrix
    // if necessary, resize this matrix to hold all elements
    int Ls = max(Lmaxindex(), C.Lmaxindex());
    int Rs = max(Rmaxindex(), C.Rmaxindex());
    if ((Lmaxindex() < Ls)||(Rmaxindex() < Rs)) {
      clean(); maximize();
    }
    v_index_mode Lm = ResultModeMax(Lmode, C.Lmode);
    v_index_mode Rm = ResultModeMax(Rmode, C.Rmode);
    if ((Lmode!=Lm)||(Rmode!=Rm)||(Lmaxindex()<Ls)||(Rmaxindex()<Rs)) {
      int sL = (Lm == Index_C) ? Ls + 1 : Ls;
      int sR = (Rm == Index_C) ? Rs + 1 : Rs;
      reallocate(sL,sR,Lm,Rm);
    }
    Lmaxindex(Ls); Rmaxindex(Rs);
    // Now do the subtraction:
    (*this) -= C;
  }
  return *this;
}

complex_matrix & complex_matrix::sub
(const real_matrix & B, const complex_matrix & C)
{
  // first check that C is not this matrix:
  if (&C != this) {
    // make the size of the result correct, and initiallize to all zeroes:
    maximize();
    v_index_mode Lm = ResultModeMax(B.Lmode, C.Lmode);
    v_index_mode Rm = ResultModeMax(B.Rmode, C.Rmode);
    int Ls = max(B.Lmaxindex(), C.Lmaxindex());
    int Rs = max(B.Rmaxindex(), C.Rmaxindex());
    if ((Lmode!=Lm)||(Rmode!=Rm)||(Lmaxindex()<Ls)||(Rmaxindex()<Rs)) {
      // fix up the result matrix in two steps: index modes first, then sizes
      // the result will automatically be zeroed
      reallocate(0,0,Lm,Rm); get(0,0) = 0; resize(Ls,Rs);
      // we need the above get() = 0 in case Lm == Rm == Index_S
    }
    else {
      // result's modes and sizes o.k., just zero and set maxindexes
      fill(0.0);
      Lmaxindex(Ls); Rmaxindex(Rs);
    }
    // Now do the subtraction:
    (*this) += B; (*this) -= C;
  }
  else { // C is this matrix
    (*this) *= -1;  // since it's the 2nd argument
    // if necessary, resize this matrix to hold all elements
    int Ls = max(Lmaxindex(), B.Lmaxindex());
    int Rs = max(Rmaxindex(), B.Rmaxindex());
    if ((Lmaxindex() < Ls)||(Rmaxindex() < Rs)) {
      clean(); maximize();
    }
    v_index_mode Lm = ResultModeMax(Lmode, B.Lmode);
    v_index_mode Rm = ResultModeMax(Rmode, B.Rmode);
    if ((Lmode!=Lm)||(Rmode!=Rm)||(Lmaxindex()<Ls)||(Rmaxindex()<Rs)) {
      int sL = (Lm == Index_C) ? Ls + 1 : Ls;
      int sR = (Rm == Index_C) ? Rs + 1 : Rs;
      reallocate(sL,sR,Lm,Rm);
    }
    Lmaxindex(Ls); Rmaxindex(Rs);
    // Now do the sum:
    (*this) += B;
  }
  return *this;
}

complex_matrix & complex_matrix::sub
(const real_matrix & B, const real_matrix & C)
{
  // make the size of the result correct, and initiallize to all zeroes:
  maximize();
  v_index_mode Lm = ResultModeMax(B.Lmode, C.Lmode);
  v_index_mode Rm = ResultModeMax(B.Rmode, C.Rmode);
  int Ls = max(B.Lmaxindex(), C.Lmaxindex());
  int Rs = max(B.Rmaxindex(), C.Rmaxindex());
  if ((Lmode != Lm)||(Rmode != Rm)||(Lmaxindex() < Ls)||(Rmaxindex() < Rs)) {
    // fix up the result matrix in two steps: index modes first, then sizes
    // the result will automatically be zeroed
    reallocate(0,0,Lm,Rm); get(0,0) = 0; resize(Ls,Rs);
    // we need the above get() = 0 in case Lm == Rm == Index_S
  }
  else {
    // result's modes and sizes o.k., just zero and set maxindexes
    fill(0.0);
    Lmaxindex(Ls); Rmaxindex(Rs);
  }

  // Now do the sum and return:
  (*this) += B; (*this) -= C;
  return *this;
}

complex_matrix & complex_matrix::real(const complex_matrix & B)
{
  if (this != &B) {    // don't resize if B is this matrix
    resize(B);
    if(B.is_empty())
      make_empty();
    else {
      Lmaxindex(B.Lmaxindex());
      Rmaxindex(B.Rmaxindex());
    }
  }
  int i,j;
  for (i = Lminindex(); i <= Lmaxindex(); ++i)
    for (j = Rminindex(); j <= Rmaxindex(); ++j)
      data[i][j] = B[i][j].real;
  return *this;
}

complex_matrix & complex_matrix::imaginary(const complex_matrix & B)
{
  if (this != &B) {    // don't resize if B is this matrix
    resize(B);
    if(B.is_empty())
      make_empty();
    else {
      Lmaxindex(B.Lmaxindex());
      Rmaxindex(B.Rmaxindex());
    }
  }
  int i,j;
  for (i = Lminindex(); i <= Lmaxindex(); ++i)
    for (j = Rminindex(); j <= Rmaxindex(); ++j)
      data[i][j] = B[i][j].imaginary;
  return *this;
}

//-------------------------------------------------------------------------
// clean() clear elements beyond the valid index range

complex_matrix & complex_matrix::clean(void)
{
  // save index limits and maximize:
  int Lmax = Lmaxindexvalue;
  int Lmin = Lminindexvalue;
  int Rmax = Rmaxindexvalue;
  int Rmin = Rminindexvalue;
  maxindex(Lsize + Rsize);

  // now zero excess elements:
  int i,j;
  for (j = Rminindex(); j < Rmin; ++j)
    for (i = Lminindex(); i <= Lmaxindex(); ++i)
      data[i][j] = 0;
  for (j = Rmaxindex(); j > Rmax; --j)
    for (i = Lminindex(); i <= Lmaxindex(); ++i)
      data[i][j] = 0;
  for (i = Lmaxindex(); i > Lmax; --i)
    for (j = Rmin; j <= Rmax; ++j)
      data[i][j] = 0;
  for (i = Lminindex(); i < Lmin; ++i)
    for (j = Rmin; j <= Rmax; ++j)
      data[i][j] = 0;

  // restore index limits and return:
  Lmaxindex(Lmax);
  Rmaxindex(Rmax);
  return *this;
}

//---------------------------------------------------------------------
// other miscellaneous funtions:

complex_matrix & complex_matrix::diagonal(const Complex s)
{
  fill(0.0);
  int min = minindex();
  int max = maxindex();
  for(/*register*/ int i = min; i <= max; ++i)
    get(i,i) = s;
  return *this;
}

complex_matrix & complex_matrix::rowswap(const int n, const int m)
{
  // check for do nothing n == m case first:
  if (n == m) return *this;

  // n != m if we get here:
  const int nBad = 1;
  const int mBad = 2;
  int nCheck = 0;
  int mCheck = 0;
  if ((n < Lminindexvalue)||(n > Lmaxindexvalue)) nCheck = nBad;
  if ((m < Lminindexvalue)||(m > Lmaxindexvalue)) mCheck = mBad;
  switch (nCheck + mCheck) {
  case 0: {
    // then both n and m are valid
    Complex * temp = data[n];
    data[n] = data[m];
    data[m] = temp;
    break;
  }
  case nBad: {
    // only m is valid, so fill with zeros
    complex_vector y(data[m], Rsize, Rmode);  // alias the row as a vector
    y.fill(0.0);
    break;
  }
  case mBad: {
    // only n is valid, so fill with zeros
    complex_vector y(data[n], Rsize, Rmode);  // alias the row as a vector
    y.fill(0.0);
    break;
  }
  case (nBad+mBad): {
    // both indexes are invalid, so do nothing
    break;
  }
  } // switch
  return *this;
}

complex_matrix & complex_matrix::apply(Complex (*f)(Complex))
{
  /*register*/ Complex *d, *limit;  // local pointers within a row
  /*register*/ int i;
  for(i = Lminindexvalue; i <= Lmaxindexvalue; ++i) {
    // make local pointers to the first and last elements in the row:
    limit = d = data[i];
    d += Rminindexvalue; limit += Rmaxindexvalue;
    // use the pointer to the first element as the loop variable:
    for ( ; d <= limit; ++d) *d = f(*d);
  }
  return *this;
}

complex_matrix & complex_matrix::apply(Complex (*f)(const Complex &))
{
  /*register*/ Complex *d, *limit;  // local pointers within a row
  /*register*/ int i;
  for(i = Lminindexvalue; i <= Lmaxindexvalue; ++i) {
    // make local pointers to the first and last elements in the row:
    limit = d = data[i];
    d += Rminindexvalue; limit += Rmaxindexvalue;
    // use the pointer to the first element as the loop variable:
    for ( ; d <= limit; ++d) *d = f(*d);
  }
  return *this;
}

complex_matrix & complex_matrix::apply(Complex (*f)(Complex, int, int))
{
  int i;                     // row counter (left index)
  /*register*/ Complex *d;       // inner loop pointer within a row
  /*register*/ int j, limit;     // inner loop counter and a limit
  for(i = Lminindexvalue; i <= Lmaxindexvalue; ++i) {
    j = Rminindexvalue; limit = Rmaxindexvalue;
    for (d = data[i] + j; j <= limit; ++d, ++j) *d = f(*d, i, j);
  }
  return *this;
}

complex_matrix & complex_matrix::apply(Complex (*f)(const Complex &, int, int))
{
  int i;                     // row counter (left index)
  /*register*/ Complex *d;       // inner loop pointer within a row
  /*register*/ int j, limit;     // inner loop counter and a limit
  for(i = Lminindexvalue; i <= Lmaxindexvalue; ++i) {
    j = Rminindexvalue; limit = Rmaxindexvalue;
    for (d = data[i] + j; j <= limit; ++d, ++j) *d = f(*d, i, j);
  }
  return *this;
}

// end of complex_matrix definitions
