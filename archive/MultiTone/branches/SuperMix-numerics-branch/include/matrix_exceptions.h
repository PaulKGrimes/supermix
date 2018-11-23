/* matrix_exceptions.h
*
*   Written by Paul Grimes
*
*   Definition of exceptions  STL style matrix template.  All exceptions inherit from matrix_error.
*/

/* Changes
*   
*/

/*! Parent exception class containing all errors from matrix */
class matrix_error { };

/*! Exception thrown for general matrix sizing errors */
class matrix_size_error : public matrix_error { };

/*! Exception thrown by methods that only make sense on square matrices. */
class matrix_not_square_error : public matrix_size_error { };

/*! Exception thrown by inversion methods if matrix is found to be singular */
class matrix_singular_error : public matrix_error { };
