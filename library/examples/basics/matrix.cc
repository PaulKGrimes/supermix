// matrix.cc

// Brief introduction to the SuperMix numeric matrix and vector
// classes.

#include "supermix.h"

// SuperMix analyzes a circuit by calculating its complex-valued
// scattering and noise correlation matrices from those of its
// individual components. Consequently SuperMix requires a fairly
// sophisticated linear algebra capability. The SuperMix matrix
// and vector classes provide a linear algebra package for
// real-valued and complex-valued vector and matrix data. The
// classes are defined and described in detail in the header files
// "vector.h" and "table.h" found in the/ SuperMix include/
// directory. A complete list of many more aritmetic operations
// is available in the header file "matmath.h".

// Including "matmath.h" alone makes just the complex number and
// linear algebra capabilities of the SuperMix library available to
// your program. This is convenient for programs which do not
// require the full circuit simulation package.

int main()
{
  // -----------------------------------------------------------------
  // BASIC MATRIX CREATION AND ELEMENT ASSIGNMENT

  // We can declare a complex-valued matrix as follows (note that the
  // type Matrix starts with an uppercase M):

  Matrix A;      // a 0x0, complex-valued matrix (no available elements)

  // This default matrix is of size zero. To get a matrix with a
  // definite size, we can give dimensions as parameters:

  Matrix B(3);   // a square matrix (3x3)
  Matrix C(2,4); // a 2x4 matrix (2 rows, 4 columns)

  // All elements in a freshly-created matrix are initialized to 0,
  // unless the matrix is created as a copy of some other. Let's look at
  // our finite-sized matrices:

  cout << "B:" << endl << B << endl << endl;
  cout << "C:" << endl << C << endl << endl;

  // Assigning values to elements:
  // The element indexing for matrices A, B, and C above starts
  // with element 1 (not 0, as for C-style arrays). This is the
  // default indexing mode, but is not the only indexing mode. Look
  // in vector.h and table.h for a complete description of the
  // available indexing modes.

  B = 3 + 4*I;         // all elements of B set to 3+i4
  C[1][3] = 2 - 5*I;   // only the (1,3) element of C affected.
  C[2][2] = B[2][2];
  cout << "B:" << endl << B << endl << endl;
  cout << "C:" << endl << C << endl << endl;


  // -----------------------------------------------------------------
  // BASIC MATRIX ARITHMETIC

  // All the standard matrix operations are available: multiplication by
  // a scalar, matrix addition and subtraction, matrix multiplication.
  // When the result of an operation is assigned to a matrix, its size
  // and memory allocation are adjusted as required to hold the result.

  Matrix B2(B);        // B2 is a copy of B
  A = B + B2;          // A is dynamically resized to hold the result
  cout << "B, B2:"      << endl << B << endl << endl;
  cout << "A = B + B2:" << endl << A << endl << endl;

  // Two matrices can be operated upon by a binary operation even though
  // they don't have dimensions compatible with the operation (e.g.,
  // their dimensions aren't the same in the case of matrix addition).
  // An operation involving any two matrices is interpreted as follows:
  //
  //    (1) Copy the two matrices into temporary, working matrices. Pad
  //        each temoprary matrix with the minimum number of rows and/or
  //        columns filled with zeroes which make the dimensions of
  //        the two temporary matrices compatible for the operation to
  //        be performed.
  //
  //    (2) Perform the operation on these two new, expanded matrices.
  //
  //    (3) The result of the operation is the result of step (2).
  //
  // This process is applied for the binary matrix operations of
  // addition, subtraction, and multiplication. The actual algoithms
  // are actually quite efficient and don't require unnecesary copying.

  A = B + C;  // Note how the result follows from the above description
  cout << "A = B + C:" << endl << A << endl << endl;

  A = B * C;  // Ditto for matrix multiplication
  cout << "A = B * C:" << endl << A << endl << endl;
  A = C * B;
  cout << "A = C * B:" << endl << A << endl << endl;


  // -----------------------------------------------------------------
  // MATRIX SOLVER AND MATRIX INVERSION

  // We can solve nonsingular matrix equations:

  B += 2*identity_matrix(B);  // now B is nonsingular
  C.resize(3,2).diagonal(1);  // C is now 3x2, diagonal elements == 1
  cout << "B:" << endl << B << endl << endl;
  cout << "C:" << endl << C << endl << endl;

  A = solve(B,C);    // solve "B*A = C" for A
  cout << "A = solve(B,C):" << endl << A << endl << endl;

  // Let's verify the solution:
  cout << fixed << setprecision(15);  // so we don't see tiny numbers
  cout << "C - B*A (should be all 0's):" << endl
       << C - B*A << endl << endl;
  // And the result is zero to within the precision of a double.

  cout << general << setprecision(0); // back to default format

  // If there is not a unique solution for solve(), then it returns a
  // matrix with no elements; if this had been the case in the above
  // example, then the member function A.is_empty() would return a
  // nonzero value, indicating that it contained no valid elements. 

  // Matrix inversion is also available:

  A = inverse(B);
  cout << general << setprecision(0);
  cout << "A = inverse(B):" << endl << A << endl << endl;

  // Let's again verify the solution:
  cout << fixed << setprecision(8);  // so we don't see tiny numbers
  cout << "A*B (should be the identity matrix):" << endl
       << A*B << endl << endl;
  cout << general << setprecision(0); // back to default format


  // ----------------------------------------------------------------
  // MORE ABOUT MATRICES AND VECTORS
  //
  // The type "Matrix" is really a typedef alias for the actual
  // typename "complex_matrix". There is a real-valued matrix type
  // "real_matrix" whose behavior is just like "complex_matrix."
  // Similarly, "Vector" is a typedef of "complex_vector"; a
  // "real_vector" type is also available.
  //
  // Read the comments at the beginning of the header files:
  //    matmath.h
  //    vector.h
  //    table.h
  //
  // Play around with the various features of the classes and
  // operations by adding code to a copy of this program and seeing
  // what happens.

}
