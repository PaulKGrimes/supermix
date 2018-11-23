/* matrix.h
*
*   Written by Paul Grimes, after Bjarne Stroustrop, "The C++ Programming Language
*   : Special Edition" pp672.
*
*   Definition of STL style matrix template, using sliced valarray for storage
*/

/* Changes
*   26/04/04 : PKG : Initial code directly from Stroustrop
*/

#include <valarray>
#include <iostream>
#include <string>
#include <vector>
#include "Slice_iter.h"
#include "matrix_exceptions.h"

using namespace std;

template<class C> class matrix
{
    valarray<C> * v;    /*<! Stores elements by column */
    size_t ncol, nrow;  /*<! Number of columns, number of rows */
    
public:
    matrix();
    matrix(size_t x, size_t y);     /*<! Constructor */
    matrix(const matrix<C>&);       /*<! Copy constructor */
    matrix& operator= (const matrix<C> &); /*<! Assignment constructor */
    ~matrix();                      /*<! Destructor */

    // Dimensions
    size_t size() { return ncol*nrow; }
    size_t nrows() { return nrow; }
    size_t ncols() { return ncol; }
    
    // Methods for filling the matrix
    void resize(size_t x, size_t y); /*<! Replace current matrix with new zero filled matrix */
    void resize(size_t x);  /*<! Replace current matrix with zero filled square */
    void unit();    /*<! Fill as unit matrix */
    void zero();    /*<! Fill with zeros */
    void fillDiagonal(C c);   /*<! Fill the diagonal with the value c */
    void fill(C c);          /*<! Fill the matrix with the value c*/

    // Row and column access
    Slice_iter<C> row(size_t i);
    CSlice_iter<C> row(size_t i) const;

    Slice_iter<C> column(size_t i);
    CSlice_iter<C> column(size_t i) const;

    // Fortran style subscripts
    C& operator() (size_t x, size_t y);
    C operator() (size_t x, size_t y) const;

    Slice_iter<C> operator() (size_t i) { return column(i); };
    CSlice_iter<C> operator() (size_t i) const { return column(i); };
    
    // C style subscripts
    Slice_iter<C> operator[] (size_t i) {return column(i); };
    CSlice_iter<C> operator[] (size_t i) const {return column(i); };
    
    ostream& write(ostream& s);
    
    // In place element by element operations
    matrix<C>& operator*=(C c);   /*<! Multiplication by constant */
    matrix<C>& operator/=(C c);   /*<! Divison by constant */
    matrix<C>& operator+=(C c);   /*<! Addition of constant */
    matrix<C>& operator-=(C c);   /*<! Substraction of constant */
    
    // In place matrix operations
    matrix<C>& operator+=(matrix& m);  /*<! In place addition of matrices */
    matrix<C>& operator-=(matrix& m);  /*<! In place subtraction of matrices */
    matrix<C>& operator*=(matrix& m);  /*<! In place inner product *this x m */
    void invert();      /*<! Invert matrix in place */
    void transpose();   /*<! Transpose matrix in place */
    void swap(Slice_iter<C> a, Slice_iter<C> b);  /*<! Swap values referenced by a and b in matrix */
    
    // Direct access to the valarray, useful for new NR functions
    valarray<C>& array() {return *v;};
};    


// Constructors/Destructor
template<class C> matrix<C>::matrix()
{
    ncol = 1;
    nrow = 1;
    v = new valarray<C>(1);
}

template<class C> matrix<C>::matrix(size_t x, size_t y)
{
    ncol = x;
    nrow = y;
    v = new valarray<C>(x*y);
}


template<class C> matrix<C>::matrix(const matrix<C> &m) : ncol(m.ncol), nrow(m.nrow)
{
    // Delete old array, create new valarray and copy data from m size_to it
    v = new valarray<C>(ncol*nrow);
    *v = *(m.v);
}


template<class C> matrix<C>& matrix<C>::operator= (const matrix<C> &m)
{
    if (this != &m)
    {
        ncol = m.ncol;
        nrow = m.nrow;
        // Delete old array, create new valarray and copy data from m size_to it
        delete v;
        v = new valarray<C>(ncol*nrow);
        *v = *(m.v);
    }
    return *this;
}


template<class C> matrix<C>::~matrix()
{
    delete v;
}


// Matrix filling operations
template<class C> void matrix<C>::resize(size_t x, size_t y)
{
    nrow = x;
    ncol = y;
    delete v;
    v = new valarray<C>(ncol*nrow);
    zero();
}


template<class C> void matrix<C>::resize(size_t x)
{
    resize(x, x);
}


template<class C> void matrix<C>::fill(C c)
{
    for (size_t i=0; i < ncol*nrow; i++) (*v)[i] = c;
}


template<class C> void matrix<C>::zero()
{
    fill((C)0.0);
}


template<class C> void matrix<C>::fillDiagonal(C c)
{
    zero();
    for (size_t i=0; i< ((nrow<ncol)?nrow:ncol); i++) (*this)[i][i] = c;
}


template<class C> void matrix<C>::unit()
{
    fillDiagonal((C)1.0);
}


// Element access
template<class C> inline Slice_iter<C> matrix<C>::row(size_t i)
{
    return Slice_iter<C>(v, slice(i, ncol, nrow));
}


template<class C> inline CSlice_iter<C> matrix<C>::row(size_t i) const
{
    return CSlice_iter<C>(v, slice(i, ncol, nrow));
}


template<class C> inline Slice_iter<C> matrix<C>::column(size_t i)
{
    return Slice_iter<C>(v, slice(i*nrow, nrow, 1));
}


template<class C> inline CSlice_iter<C> matrix<C>::column(size_t i) const
{
    return CSlice_iter<C>(v, slice(i*nrow, nrow, 1));
}


// Matrix output
template<class C> ostream& matrix<C>::write(ostream& s)
{
    s << "[ [ ";
    for (size_t i=0; i<(nrows-1); i++)
    {
        for (size_t j=0; j<ncol; j++) s << (*this)[j][i] << "\t";
        s << "]\n  [ "; 
    }
    for (size_t j=0; j<ncol; j++) s << (*this)[j][nrow-1] << "\t";
    s << "] ]\n";
    return s;
}


// Subscripting operators
template<class C> C& matrix<C>::operator()(size_t x, size_t y)
{
    return column(x)[y];
}


template<class C> C matrix<C>::operator()(size_t x, size_t y) const
{
    return column(x)[y];
}


// In place element by element operators
template<class C> matrix<C>& matrix<C>::operator*=(C c)
{
    (*v) *=c;
    return *this;
}


template<class C> matrix<C>& matrix<C>::operator/=(C c)
{
    (*v) /=c;
    return *this;
}


template<class C> matrix<C>& matrix<C>::operator+=(C c)
{
    (*v) +=c;
    return *this;
}


template<class C> matrix<C>& matrix<C>::operator-=(C c)
{
    (*v) -=c;
    return *this;
}


template<class C> matrix<C>& matrix<C>::operator+=(matrix<C>& m)
{
    // Check for consistent sizes
    if (m.nrow != nrow || m.ncol != ncol) throw matrix_size_error();
    
    valarray<C>& v2 = m.array();
    for (size_t i=0; i<nrow*ncol; i++) (*v)[i] += v2[i];
}


template<class C> matrix<C>& matrix<C>::operator-=(matrix<C>& m)
{
    // Check for consistent sizes
    if (m.nrow != nrow || m.ncol != ncol) throw matrix_size_error();
    
    valarray<C>& v2 = m.array();
    for (size_t i=0; i<nrow*ncol; i++) (*v)[i] -= v2[i];
}


template<class C> matrix<C>& matrix<C>::operator*=(matrix<C>& m)
{
    // Check for consistent sizes
    if (m.nrow != ncol) throw matrix_size_error();
    
    matrix<C> temp(*this);

    *this = temp*m;
    
    return *this;
}


// In place operations
// Pretty inefficient transpose
template<class C> void matrix<C>::transpose()
{
    matrix<C> temp(nrow, ncol);
    for (size_t i=0; i<nrow; i++)
    {
        for (size_t j=0; j<ncol; j++) temp[j][i] = (*this)[i][j];
    }
    
    (*this) = temp;
}

// Use Gauss-Jordan elimination with pivoting to invert matrix in place.
// Algorithm taken from numerical recipes in C 
template<class C> void matrix<C>::invert()
{
    // Check for squareness of matrix
    if (ncol != nrow) throw matrix_not_square_error();
    
    // Create temporaries required
    valarray<size_t> indxc(ncol), indxr(ncol), ipiv(ncol);
    for (size_t j=0; j<ncol; j++) ipiv[j] = 0;
    
    size_t irow, icol;
    C big, dum, pivinv, temp;
    
    for (size_t i=0; i<ncol; i++) // Main loop over columns to be reduced
    {
        big = (C)0.0;
        for (size_t j=0; j<ncol; j++) // Loop searching for pivot posize_t
        {
            if (ipiv[j] != 1)
            {
                for (size_t k=0; k<ncol; k++)
                {
                    if (ipiv[k] == 0)
                    {
                        if( abs((*this)[j][k]) >= abs(big))
                        {
                            big = abs((*this)[j][k]);
                            irow = j;
                            icol = k;
                        }
                    } else if (ipiv[k] > 1) throw matrix_singular_error();
                }
            }
        }
        ++(ipiv[icol]);
            
        // Found pivot posize_t, now swap rows so that it is on the diagonal
        if (irow != icol)
        {
            swap(row(irow), row(icol));
        }
        indxr[i]=irow;
        indxc[i]=icol;
        
        // Divide by pivot element
        if ((*this)[icol][icol] == (C)0.0) throw matrix_singular_error();
        pivinv = ((C)1.0)/(*this)[icol][icol];
        (*this)[icol][icol] = (C)1.0;
        for (size_t j=0; j<ncol; j++) (*this)[icol][j] *=pivinv;
        
        // Reduce the rows
        for (size_t j=0; j<ncol; j++)
        {
            if (j != icol)
            {
                dum = (*this)[j][icol];
                (*this)[j][icol] = (C)0;
                for (size_t k=0; k<ncol; k++) (*this)[j][k] -= (*this)[icol][k]*dum;
            }
        }
    }
     
    // Unscramble matrix
    for (size_t i=(ncol-1); i>=1; i--)
    {
        if (indxr[i] != indxc[i])
        {
            swap(column(indxr[i]), column(indxc[i]));
        }
    }
}


template<class C> void matrix<C>::swap( Slice_iter<C> a, Slice_iter<C> b)
{
    // Check that slice iterators are the same size
    if (a.size() != b.size()) throw matrix_size_error();
    
    C temp;
    // Carry out the swap
    for (size_t i = 0; i<a.size(); i++)
    {
        temp = a[i];
        a[i] = b[i];
        b[i] = temp;
    }
}


// Unary functions
template<class C> matrix<C> inverse(const matrix<C>& m)
{
    matrix<C> result(m);
    result.invert();
    return result;
}


template<class C> matrix<C> transpose(const matrix<C>& m)
{
    matrix<C> result(m);
    result.tranpose();
    return result;
}


template<class C> matrix<C> operator-(matrix<C>& m)
{
    matrix<C> result(m);
    result *= -1.0;
    
    return result;
}


// Binary functions
template<class C> ostream& operator<<(ostream& s, matrix<C>& m)
{
    s << "[ [ ";
    for (size_t i=0; i<(m.nrows()-1); i++)
    {
        for (size_t j=0; j<m.ncols(); j++) s << m[j][i] << "\t";
        s << "]\n  [ "; 
    }
    for (size_t j=0; j<m.ncols(); j++) s << m[j][m.nrows()-1] << "\t";
    s << "] ]\n";
    return s;
}


template<class C> matrix<C> operator*(matrix<C>& a, matrix<C>& b)
{
    if (a.ncols() != b.nrows()) throw matrix_size_error();
        
    matrix<C> result(a.nrows(), b.ncols());
    
    for (size_t i=0; i<b.ncols(); i++)
    {
        for (size_t j=0; j<a.nrows(); j++)
        {
            C res = a[0][i]*b[j][0];
            for (size_t k=1; k<b.nrows(); k++)
            {
                res += a[k][i]*b[j][k];
            }
            result[i][j] = res;
        }
    }
    
    return result;
}


template<class C> vector<C> operator*(matrix<C>& m, vector<C>& v)
{
    if (m.ncols() != v.size()) throw matrix_size_error();
        
    vector<C> result(m.nrows());
    
    for (size_t i=0; i<m.nrows(); i++)
    {
        C res = m[0][i]*v[0];
        for (size_t j=1; j<m.ncols(); j++)
            res+= m[j][i]*v[j];
        result[i] = res;
    }
    
    return result;
}


template<class C> vector<C> operator*(vector<C>& v, matrix<C>& m)
{
    if (m.nrows() != v.size()) throw matrix_size_error();
        
    vector<C> result(m.ncols());
    
    for (size_t i=0; i<m.ncols(); i++)
    {
        C res = m[i][0]*v[0];
        for (size_t j=1; j<m.nrows(); j++)
            res+= m[i][j]*v[j];
        result[i] = res;
    }
    
    return result;
}



template<class C> matrix<C> operator+(matrix<C>& a, matrix<C>& b)
{
    matrix<C> result(a);
    result += b;
    return result;
}


template<class C> matrix<C> operator-(matrix<C>& a, matrix<C>& b)
{
    matrix<C> result(a);
    result -= b;
    return result;
}
