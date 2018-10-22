// ********************************************************************
// test_complex_input
//
// test various ways of reading complex numbers
//
// 1/11/04:  Modified for gcc 3.x
// 7/29/98
//
// ********************************************************************

#include "SIScmplx.h"
#include <iostream>

using namespace std;
 
int main()
{
  complex z1, z2;

  while (cin) {

    cout << "Input two complex numbers using default, cartesian format:" << endl;
    cin >> z1 >> z2;
    
    cout << "\nYou input:\n" << z1 << "  " << z2 << endl << endl;
  
    Complex::in_degree();
    cout << "Input two complex numbers using default, degree format:" << endl;
    cin >> z1 >> z2;

    cout << "\nYou input:\n" << z1 << "  " << z2 << endl << endl;
  
    Complex::in_db();
    cout << "Input two complex numbers using default, db format:" << endl;
    cin >> z1 >> z2;

    cout << "\nYou input:\n" << z1 << "  " << z2 << endl << endl;
    
    Complex::in_cartesian(); Complex::in_delimited();
    cout << "Input two complex numbers using delimited, cartesian format:" << endl;
    cin >> z1 >> z2;

    cout << "\nYou input:\n" << z1 << "  " << z2 << endl << endl;
    
    Complex::in_degree(); Complex::in_delimited();
    cout << "Input two complex numbers using delimited, degree format:" << endl;
    cin >> z1 >> z2;

    cout << "\nYou input:\n" << z1 << "  " << z2 << endl << endl;

    Complex::in_cartesian(); Complex::in_default();
  }

}
