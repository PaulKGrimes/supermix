// test the reindexing functions of the table and vector classes
// 7/6/98
// 7/28/98: added <iostream.h>, since SIScmplx.h doesn't any more

#include <iostream>
#include "global.h"

using namespace std;

double v_index(const double, const int i)
{ return i; }
complex v_index(const complex, const int i)
{ return complex(1,1)*i; }
double v_index(const double, const int i, const int j)
{ return i+10*j; }
complex v_index(const complex, const int i, const int j)
{ return i+I*j; }

const char * const mode_name(const v_index_mode t)
{ 
  const char * p;
  switch(t) {
  case Index_C:
    { p = "Index_C"; break; }
  case Index_1:
    { p = "Index_1"; break; }
  case Index_S:
    { p = "Index_S"; break; }
  default:
    { p = "*** unknown mode ***"; break; }
  }
  return p;
}

int main()
{
  { // check real_vectors
    real_vector x[3][3];
    
    for(int i = 0; i < 3; ++i) 
      for(int j = 0; j < 3; ++j) {
	x[i][j].reallocate(3*i,v_index_mode(j));
	x[i][j].apply(v_index);
      }
    
    
    cout << "\nThe real_vectors:" << endl;
    
    for(int i = 0; i < 3; ++i) 
      for(int j = 0; j < 3; ++j) {
	cout << "\nx[" << i << "][" << j << "] : mode = " << mode_name(x[i][j].mode)
	     << "   min: " << x[i][j].minindex() << "   max: " << x[i][j].maxindex();
	x[i][j].show();
	for(int k = 0; k < 3; ++k) {
	  real_vector c(x[i][j]);
	  c.reindex(v_index_mode(k));
	  cout << "mode: " << mode_name(c.mode) << "   min: " << c.minindex() << "   max: " << c.maxindex();
	  c.show();
	}
      }
  }
  { // check complex_vectors
    complex_vector x[3][3];
    
    for(int i = 0; i < 3; ++i) 
      for(int j = 0; j < 3; ++j) {
	x[i][j].reallocate(3*i,v_index_mode(j));
	x[i][j].apply(v_index);
      }
    
    
    cout << "\nThe complex_vectors:" << endl;
    
    for(int i = 0; i < 3; ++i) 
      for(int j = 0; j < 3; ++j) {
	cout << "\nx[" << i << "][" << j << "] : mode = " << mode_name(x[i][j].mode)
	     << "   min: " << x[i][j].minindex() << "   max: " << x[i][j].maxindex();
	x[i][j].show();
	for(int k = 0; k < 3; ++k) {
	  complex_vector c(x[i][j]);
	  c.reindex(v_index_mode(k));
	  cout << "mode: " << mode_name(c.mode) << "   min: " << c.minindex() << "   max: " << c.maxindex();
	  c.show();
	}
      }
  }
  { // check real_tables
    real_table x[3][3];
    cout << "\nThe real_tables:" << endl;

    for(int size = 0; size < 3; ++size) {
    
      for(int i = 0; i < 3; ++i) 
	for(int j = 0; j < 3; ++j) {
	  x[i][j].reallocate(1*size,1*size,v_index_mode(i),v_index_mode(j));
	  x[i][j].apply(v_index);
	}

      for(int i = 0; i < 3; ++i) 
	for(int j = 0; j < 3; ++j) {
	  cout << "\nx[" << i << "][" << j << "] : " 
	       << mode_name(x[i][j].Lmode) << "," << mode_name(x[i][j].Rmode)
	       << "   min: " << x[i][j].Lminindex() << ","  << x[i][j].Rminindex() 
	       << "   max: " << x[i][j].Lmaxindex() << ","  << x[i][j].Rmaxindex();
	  x[i][j].show();
	  for(int k = 0; k < 3; ++k) {
	    real_table c(x[i][j]);
	    c.reindex(v_index_mode(k),v_index_mode(k));
	    cout << mode_name(c.Lmode) << "," << mode_name(c.Rmode) 
		 << "  size: " << c.Lsize << ","  << c.Rsize 
		 << "   min: " << c.Lminindex() << ","  << c.Rminindex() 
		 << "   max: " << c.Lmaxindex() << ","  << c.Rmaxindex();
	    c.show();
	  }
	}
    }
  }
  { // check complex_tables
    complex_table x[3][3];
    cout << "\nThe complex_tables:" << endl;
    
    for(int size = 0; size < 3; ++size) {
    
      for(int i = 0; i < 3; ++i) 
	for(int j = 0; j < 3; ++j) {
	  x[i][j].reallocate(1*size,1*size,v_index_mode(i),v_index_mode(j));
	  x[i][j].apply(v_index);
	}
    
      for(int i = 0; i < 3; ++i) 
	for(int j = 0; j < 3; ++j) {
	  cout << "\nx[" << i << "][" << j << "] : " 
	       << mode_name(x[i][j].Lmode) << "," << mode_name(x[i][j].Rmode)
	       << "   min: " << x[i][j].Lminindex() << ","  << x[i][j].Rminindex() 
	       << "   max: " << x[i][j].Lmaxindex() << ","  << x[i][j].Rmaxindex();
	  x[i][j].show();
	  for(int k = 0; k < 3; ++k) {
	    complex_table c(x[i][j]);
	    c.reindex(v_index_mode(k),v_index_mode(k));
	    cout << mode_name(c.Lmode) << "," << mode_name(c.Rmode) 
		 << "  size: " << c.Lsize << ","  << c.Rsize 
		 << "   min: " << c.Lminindex() << ","  << c.Rminindex() 
		 << "   max: " << c.Lmaxindex() << ","  << c.Rmaxindex();
	    c.show();
	  }
	}
    }
  }
}
