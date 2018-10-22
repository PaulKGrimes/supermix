/*
Read in a file into a datafile object, and check for proper copying to
real_table objects. Must supply a file name as command line argument.

FR 2/6/98

* 7/28/98: added <iostream.h> to includes, since SIScmplx.h doesn't*
*/

#include <iostream>
#include "global.h"
#include "datafile.h"

using namespace std;

int compare(const real_table &, const datafile &); // comparison fcn

int main(int argc, char **argv) 
{
  if(argc < 2) {
    cout << "Need to specify input file name on command line\n" ;
    return 1;
  }

  int fail_flag = 0;

  cout << "Inputting data and building copies.\n" << endl;
  datafile D(argv[1]) ; // read in file data

  real_table A(D);      // try copy constructor;

  real_table B; B = D;  // try operator equal;

  // now check sizes and report if a problem:
  // (remember, "lines" in datafile go to Rindex in real_table)

  if ((A.Lminindex() != 1)||(A.Lmaxindex() != D.numcolumns())||
      (A.Rminindex() != 1)||(A.Rmaxindex() != D.numlines())) {
    // then A didn't get dimensioned properly
    cout << "*** copy constructor failure! ***\n\n"
	 << "input data: ncols: " << D.numcolumns() << "\t"
	 << " nlines: " << D.numlines() << "\n"
	 << "table data: Lmax:  " << A.Lmaxindex() << "\t"
	 << " Rmax:   " << A.Rmaxindex() << "\n"
	 << "            Lmin:  " << A.Lminindex() << "\t"
	 << " Rmin:   " << A.Rminindex() << "\n" << endl;
    fail_flag = 1;
  }
  if (compare(A, D)) {
    // then data differs in A
    cout << "*** copy constructor failure! ***\n\n"
	 << "Data in table is incorrect." << endl
	 << "input data: ncols: " << D.numcolumns() << "\t"
	 << " nlines: " << D.numlines() << "\n"
	 << "table data: Lmax:  " << A.Lmaxindex() << "\t"
	 << " Rmax:   " << A.Rmaxindex() << "\n"
	 << "            Lmin:  " << A.Lminindex() << "\t"
	 << " Rmin:   " << A.Rminindex() << "\n" << endl;
    transpose(A).show();
    fail_flag = 1;
  }

  if ((B.Lminindex() != 1)||(B.Lmaxindex() != D.numcolumns())||
      (B.Rminindex() != 1)||(B.Rmaxindex() != D.numlines())) {
    // then B didn't get dimensioned properly
    cout << "*** operator = failure! ***\n\n"
	 << "input data: ncols: " << D.numcolumns() << "\t"
	 << " nlines: " << D.numlines() << "\n"
	 << "table data: Lmax:  " << B.Lmaxindex() << "\t"
	 << " Rmax:   " << B.Rmaxindex() << "\n"
	 << "            Lmin:  " << B.Lminindex() << "\t"
	 << " Rmin:   " << B.Rminindex() << "\n" << endl;
     fail_flag = 1;
 }
  if (compare(B, D)) {
    // then data differs in B
    cout << "*** operator = failure! ***\n\n"
	 << "Data in table is incorrect." << endl
	 << "input data: ncols: " << D.numcolumns() << "\t"
	 << " nlines: " << D.numlines() << "\n"
	 << "table data: Lmax:  " << B.Lmaxindex() << "\t"
	 << " Rmax:   " << B.Rmaxindex() << "\n"
	 << "            Lmin:  " << B.Lminindex() << "\t"
	 << " Rmin:   " << B.Rminindex() << "\n" << endl;
    transpose(B).show();
    fail_flag = 1;
  }

  //  Now try to create real_table with a temp datafile object
  real_table C = datafile(argv[1]);
  if (compare(C, D)) {
    cout << "*** Constructor with temp datafile failure! ***\n\n"
	 << "Data in table is incorrect." << endl
	 << "input data: ncols: " << D.numcolumns() << "\t"
	 << " nlines: " << D.numlines() << "\n"
	 << "table data: Lmax:  " << C.Lmaxindex() << "\t"
	 << " Rmax:   " << C.Rmaxindex() << "\n"
	 << "            Lmin:  " << C.Lminindex() << "\t"
	 << " Rmin:   " << C.Rminindex() << "\n" << endl;
    transpose(C).show();
    fail_flag = 1;
  }

  if (!fail_flag) {
    transpose(C).show();  // display the data for comparisons
    cout << "\nRun completed successfully." << endl;
  }
  else 
    cout << "\nTest failure." << endl;

  return 0;
}

int compare(const real_table & A, const datafile & D)
{
  int c = 0;  // fail flag
  for(int i = 1; i <= D.numcolumns(); ++i) {
    if (c == 1) break;
    for(int j = 1; j <= D.numlines(); ++j)
      if(D.read(i,j) != A.read(i,j)) {
	c = 1; break;
      }
  }
  return c;
}

