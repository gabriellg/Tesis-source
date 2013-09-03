
#include "CMatrix.hpp"

#include <iostream>
#include <cstring>
#include <math.h>

#include "CMath.hpp"
#include "asrtbas.h"
using namespace std;

#define EPSILON     0.000001

struct prv_matrix_t
{
    double       *val;
    unsigned int rows;
    unsigned int cols;
};

//------------------------------------------------------------------------------
//
static struct prv_matrix_t* prv_create( unsigned int rows, unsigned int columns )
{
    assert( rows > 0 && columns > 0 );

    struct prv_matrix_t *data = new prv_matrix_t;
    assert_no_null( data );

    data->rows = rows;
    data->cols = columns;
    data->val = new double [ rows * columns ];
    assert_no_null( data->val );

    return data;
}

//------------------------------------------------------------------------------
//
CMatrix :: CMatrix( unsigned int rows, unsigned int columns )
{
    m_data = prv_create( rows, columns );
}

//------------------------------------------------------------------------------
//
CMatrix :: CMatrix( unsigned int rows, unsigned int columns, double value )
{
    m_data = prv_create( rows, columns );

    this->setAll( value );
}

//------------------------------------------------------------------------------
//
CMatrix :: CMatrix( const CMatrix *M )
{
    assert_no_null( M );

    m_data = prv_create( M->m_data->rows, M->m_data->cols );

    const unsigned long size = m_data->rows * m_data->cols;

    memcpy( m_data->val, M->m_data->val, sizeof(double) * size );
}

//------------------------------------------------------------------------------
//
CMatrix :: ~CMatrix()
{
    assert_no_null( m_data );

    if ( m_data->val )
        delete [] m_data->val;

    delete m_data;
}

//------------------------------------------------------------------------------
//
unsigned int CMatrix :: rows() const
{
    return m_data->rows;
}

//------------------------------------------------------------------------------
//
unsigned int CMatrix :: cols() const
{
    return m_data->cols;
}

//------------------------------------------------------------------------------
//
unsigned long CMatrix :: size() const
{
    return m_data->rows * m_data->cols;
}

//------------------------------------------------------------------------------
//
double CMatrix :: getValue( unsigned int row, unsigned int col ) const
{
    assert( row < m_data->rows && col < m_data->cols );

    return m_data->val[ ( row * m_data->cols ) + col ];
}

//------------------------------------------------------------------------------
//
void CMatrix :: setValue( unsigned int row, unsigned int col, double value )
{
    assert( row < m_data->rows && col < m_data->cols );

    m_data->val[ ( row * m_data->cols ) + col ] = value;
}

//------------------------------------------------------------------------------
//
void CMatrix :: setAll( double value )
{
    unsigned long numElems = m_data->rows * m_data->cols;

    for ( unsigned long r = 0; r < numElems; ++r )
        m_data->val[r] = value;
}

//------------------------------------------------------------------------------
//
void CMatrix :: setDiagonal( double value )
{
    assert( this->isSquare() );

    for( unsigned int n = 0; n < m_data->rows; ++n )
        this->setValue( n, n, value );
}

//------------------------------------------------------------------------------
//
void CMatrix :: setIdentity()
{
    assert( this->isSquare() );

    this->setAll( 0 );

    for( unsigned int n = 0; n < m_data->rows; ++n )
        this->setValue( n, n, 1 );
}

//------------------------------------------------------------------------------
//
void CMatrix :: generateRandomData( int min, int max )
{
    for( unsigned r = 0; r < m_data->rows; ++r )
    {
        for( unsigned c = 0; c < m_data->cols; ++c )
        {
            this->setValue( r, c, CMath::getRandomNumber( min, max ) );
        }
    }
}

//------------------------------------------------------------------------------
//
void CMatrix :: copyValues( const CMatrix *M )
{
    assert_no_null( M );
    assert( M->m_data->rows == m_data->rows && M->m_data->cols == m_data->cols );

    unsigned long numElems = m_data->rows * m_data->cols;

    for ( unsigned long r = 0; r < numElems; ++r )
        m_data->val[r] = M->m_data->val[r];
}

//------------------------------------------------------------------------------
//
bool CMatrix :: equal( const CMatrix *M ) const
{
    assert_no_null( M );
    assert( m_data->val && M->m_data->val );

    if ( m_data->rows != M->m_data->rows || m_data->cols != M->m_data->cols) {
        return false;
    }

    unsigned long numElems = m_data->rows * m_data->cols;

    for ( unsigned long r = 0; r < numElems; ++r )
        if ( fabs( m_data->val[r] - M->m_data->val[r] ) > EPSILON )
            return false;

    return true;
}

//-----------------------------------------------------------------------------
//
bool CMatrix :: isSquare() const
{
    return m_data->rows == m_data->cols;
}

//-----------------------------------------------------------------------------
// All elements are 0 except the main diagonal
bool CMatrix :: isDiagonal() const
{
    if ( ! isSquare() )
      return false;

    for( unsigned int r = 1; r < m_data->rows; ++r)
    {
        for( unsigned int c = 0; c < r; ++c )
        {
            if( fabs( this->getValue(r,c) ) > EPSILON ||
                fabs( this->getValue(c,r) ) > EPSILON )
            {
                return false;
            }
        }
    }
    return true;
}

//-----------------------------------------------------------------------------
// All elements are 0 except the main diagonal whose values are != 0 and all equal
bool CMatrix :: isScalar() const
{
    if ( ! isDiagonal() )
        return false;

    const double S = this->getValue(0,0);

    for( unsigned int i = 1; i < m_data->rows; ++i )
    {
        if ( fabs( this->getValue(i,i) - S ) > EPSILON ) {
            return false;
        }
    }
   return true;
}

//-----------------------------------------------------------------------------
// Scalar matrix in which the elements of the main diagonal are equal to 1
bool CMatrix :: isUnit() const
{
    if( fabs( this->getValue(0,0) - 1.0 ) > EPSILON )
        return false;

    return isScalar();
}

//-----------------------------------------------------------------------------
// All the elements are 0
bool CMatrix :: isNull() const
{
    for ( unsigned int r=0; r < m_data->rows; ++r )
    {
        for ( unsigned int c=0; c < m_data->cols; ++c )
        {
            if ( fabs( this->getValue(r,c) ) > EPSILON ) {
                return false;
            }
        }
    }
    return true;
}

//-----------------------------------------------------------------------------
// A square matrix which is equal to its transpose
bool CMatrix :: isSymmetric() const
{
    if ( ! isSquare() )
        return false;

    CMatrix *M = this->transpose();

    return this->equal( M );
}

//-----------------------------------------------------------------------------
// The elements that are below the main diagonal are all zero
// aij = 0 forAll i<j.
bool CMatrix :: isUpperTiangular() const
{
    if ( ! isSquare() )
        return false;

    for ( unsigned int r=1; r < m_data->rows; ++r )
    {
        for ( unsigned int c=0; c < r; ++c )
        {
            if ( fabs( this->getValue(r,c) ) > EPSILON ) {
                return false;
            }
        }
    }
    return true;
}

//-----------------------------------------------------------------------------
// The elements that are above the main diagonal are all zero.
// aij =0 forAll j<i.
bool CMatrix :: isLowerTiangular() const
{
    if ( ! isSquare() )
        return false;

    for ( unsigned int c=1; c < m_data->cols; ++c )
    {
        for ( unsigned int r=0; r < c; ++r )
        {
            if ( fabs( this->getValue(r,c) ) > EPSILON ) {
                return false;
            }
        }
    }
    return true;
}

//------------------------------------------------------------------------------
// BASIC OPERATIONS //
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//
CMatrix* CMatrix :: matrixAddition( const CMatrix *M ) const
{
    assert_no_null( M );
    assert( m_data->rows == M->m_data->rows && m_data->cols == M->m_data->cols );

    CMatrix *RM = new CMatrix( m_data->rows, m_data->cols );

    unsigned long numElems = m_data->rows * m_data->cols;

    for ( unsigned long r = 0; r < numElems; ++r )
        RM->m_data->val[r] = m_data->val[r] + M->m_data->val[r];

    return RM;
}

//------------------------------------------------------------------------------
//
CMatrix* CMatrix :: matrixSubtraction( const CMatrix *M ) const
{
    assert_no_null( M );
    assert( m_data->rows == M->m_data->rows && m_data->cols == M->m_data->cols );

    CMatrix *RM = new CMatrix( m_data->rows, m_data->cols );

    unsigned long numElems = m_data->rows * m_data->cols;

    for ( unsigned long r = 0; r < numElems; ++r )
        RM->m_data->val[r] = m_data->val[r] - M->m_data->val[r];

    return RM;
}

//------------------------------------------------------------------------------
//
CMatrix* CMatrix :: matrixMultiplication( const CMatrix *M ) const
{
    assert_no_null( M );
    assert( m_data->cols == M->m_data->rows );

    CMatrix *RM = new CMatrix( m_data->rows, M->m_data->cols );

    double sum;

    for ( unsigned int r = 0; r < m_data->rows; ++r )
    {
        for ( unsigned int c = 0; c < M->m_data->cols; ++c )
        {
            sum = 0;

            for ( unsigned int k = 0; k < m_data->cols; ++k)
            {
                sum += this->getValue(r,k) * M->getValue(k,c);
            }

            RM->setValue(r,c, sum);
        }
    }
    return RM;
}

//------------------------------------------------------------------------------
//
CMatrix* CMatrix :: matrixDivision( const CMatrix *M ) const
{
    assert_no_null( M );
    assert( m_data->rows == M->m_data->rows && this->isSquare() );

    CMatrix *M_inverse = M->inverse();

    CMatrix *RM = this->matrixMultiplication( M_inverse );

    delete M_inverse;

    return RM;
}

//------------------------------------------------------------------------------
//
void CMatrix :: scalarAddition( double value )
{
    unsigned long numElems = m_data->rows * m_data->cols;

    for ( unsigned long r = 0; r < numElems; ++r )
        m_data->val[r] += value;
}

//------------------------------------------------------------------------------
//
void CMatrix :: scalarSubtraction( double value )
{
    unsigned long numElems = m_data->rows * m_data->cols;

    for ( unsigned long r = 0; r < numElems; ++r )
        m_data->val[r] -= value;
}

//------------------------------------------------------------------------------
//
void CMatrix :: scalarLeftSubtraction( double value )
{
    unsigned long numElems = m_data->rows * m_data->cols;

    for ( unsigned long r = 0; r < numElems; ++r )
        m_data->val[r] = value - m_data->val[r];
}

//------------------------------------------------------------------------------
//
void CMatrix :: scalarMultiplication( double value )
{
    unsigned long numElems = m_data->rows * m_data->cols;

    for ( unsigned long r = 0; r < numElems; ++r )
        m_data->val[r] *= value;
}

//------------------------------------------------------------------------------
//
void CMatrix :: scalarDivision( double value )
{
    assert( fabs( value ) > EPSILON );

    unsigned long numElems = m_data->rows * m_data->cols;

    for ( unsigned long r = 0; r < numElems; ++r )
        m_data->val[r] /= value;
}

//------------------------------------------------------------------------------
//
void CMatrix :: scalarLeftDivision( double value )
{
    unsigned long numElems = m_data->rows * m_data->cols;

    for ( unsigned long r = 0; r < numElems; ++r )
    {
        assert( fabs( m_data->val[r] ) > EPSILON );
        m_data->val[r] = value / m_data->val[r];
    }
}

//------------------------------------------------------------------------------
// ADVANCED OPERATIONS
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//
CMatrix* CMatrix :: transpose() const
{
    CMatrix *RM = new CMatrix( m_data->cols, m_data->rows );

    for ( unsigned int r=0; r < m_data->rows; ++r )
        for ( unsigned int c=0; c < m_data->cols; ++c)
            RM->setValue( c, r, this->getValue(r,c) );

    return RM;
}

//------------------------------------------------------------------------------
//
CMatrix* CMatrix :: cofactor() const
{
    assert( this->size() > 1 && this->isSquare() );

    unsigned int size = this->rows();

    CMatrix *RM = new CMatrix( size, size );
    CMatrix *C = new CMatrix( size-1, size-1 );

    for ( unsigned int j = 0; j < size; ++j )
    {
        for ( unsigned int i = 0; i < size; ++i )
        {
            // Form the adjoint A_ij this->getValue(i,j)
            unsigned int j1, i1 = 0;

            for ( unsigned int ii = 0; ii < size; ++ii )
            {
                if (ii == i)
                    continue;
                j1 = 0;
                for ( unsigned int jj = 0; jj < size; ++jj )
                {
                    if (jj == j)
                        continue;
                    C->setValue( i1, j1, this->getValue(ii,jj) );
                    j1++;
                }
                i1++;
            }

            // Fill in the elements of the cofactor
            RM->setValue( i, j, pow( -1.0, i + j + 2.0 ) * C->determinant() );
        }
    }

    delete C;

    return RM;
}

//------------------------------------------------------------------------------
//
CMatrix* CMatrix :: adjoint() const
{
    assert( this->isSquare() );

    return ( this->cofactor() )->transpose();
}

//------------------------------------------------------------------------------
//
CMatrix* CMatrix :: inverse() const
{
    assert( this->isSquare() );

    double determinant = this->determinant();

    assert( fabs( determinant ) > EPSILON && "This matrix is singular" );

    CMatrix *M = this->adjoint();

    M->scalarMultiplication( 1.0 / determinant );

    return M;
}

//------------------------------------------------------------------------------
//
void CMatrix :: divideElementwise( const CMatrix *M )
{
    assert_no_null( M );
    assert( m_data->rows == M->m_data->rows && m_data->cols == M->m_data->cols );

    unsigned long numElems = m_data->rows * m_data->cols;

    for( unsigned long r = 0; r < numElems; ++r )
        m_data->val[r] /= M->m_data->val[r];
}

//------------------------------------------------------------------------------
//
void CMatrix :: multiplyElementwise( const CMatrix *M )
{
    assert_no_null( M );
    assert( m_data->rows == M->m_data->rows && m_data->cols == M->m_data->cols );

    unsigned long numElems = m_data->rows * m_data->cols;

    for( unsigned long r = 0; r < numElems; ++r )
        m_data->val[r] *= M->m_data->val[r];
}

//------------------------------------------------------------------------------
//
void CMatrix :: powerElementwise( double exponent )
{
    unsigned long numElems = m_data->rows * m_data->cols;

    for( unsigned long r = 0; r < numElems; ++r )
        m_data->val[r] = pow( m_data->val[r], exponent );
}

//------------------------------------------------------------------------------
//
CMatrix* CMatrix :: power( unsigned int exponent ) const
{
    assert( this->isSquare() );

    CMatrix *RM = new CMatrix( this );

    if( exponent == 0 )
    {
        RM->setIdentity();
    }
    else if( exponent > 1 )
    {
        CMatrix *aux;

        for( unsigned int e = 1; e < exponent; ++e )
        {
            aux = RM;
            RM = aux->matrixMultiplication( this );
            delete aux;
        }
    }

    return RM;
}

//------------------------------------------------------------------------------
//
CMatrix* CMatrix :: maxRing ( const CMatrix *M ) const
{
    assert_no_null( M );
    assert( m_data->cols == M->m_data->rows );

    CMatrix *RM = new CMatrix( m_data->rows, M->m_data->cols );

    double maxvalue;

    for ( unsigned int r = 0; r < m_data->rows; ++r )
    {
        for ( unsigned int c = 0; c < M->m_data->cols; ++c )
        {
            maxvalue = CMath::minDouble();

            for ( unsigned int k = 0; k < m_data->cols; ++k)
            {
                maxvalue = max( maxvalue, this->getValue(r,k) * M->getValue(k,c) );
            }

            RM->setValue( r, c, maxvalue );
        }
    }
    return RM;
}

//------------------------------------------------------------------------------
//
double CMatrix :: sumRow( unsigned int row ) const
{
    assert( row < m_data->rows );

    double total = 0.0;

    for( unsigned int c = 0; c < m_data->cols; ++c )
    {
        total += this->getValue( row, c );
    }

    return total;
}

//------------------------------------------------------------------------------
//
double CMatrix :: sumColumn( unsigned int column ) const
{
    assert( column < m_data->cols );

    double total = 0.0;

    for( unsigned int r = 0; r < m_data->rows; ++r )
    {
        total += this->getValue( r, column );
    }

    return total;
}

//------------------------------------------------------------------------------
// Recursive definition of determinate using expansion by minors.
//
// Notes: 1) Determinant is a recursive function, calling itself repeatedly
//           each time with a sub-matrix of the original till a terminal
//           2X2 matrix is achieved and a simple determinat can be computed.
//           As the recursion works backwards, cumulative determinants are
//           found till ultimately, the final determinate is returned to the
//           initial function caller.
//
//        2) m is a matrix (4X4 in example)  and m13 is a minor of it.
//           A minor of m is a 3X3 in which a row and column of values
//           had been excluded.   Another minor of the submartix is also
//           possible etc.
//             m  a b c d   m13 . . . .
//                e f g h       e f . h     row 1 column 3 is elminated
//                i j k l       i j . l     creating a 3 X 3 sub martix
//                m n o p       m n . p
//
//        3) the following function finds the determinant of a matrix
//           by recursively minor-ing a row and column, each time reducing
//           the sub-matrix by one row/column.  When a 2X2 matrix is
//           obtained, the determinat is a simple calculation:
//                m n
//                o p  determinant = m*p - n*o
//
//        4) the final determinant value is the sum of sub determinants
//
static double determinant_recursive( const CMatrix *M )
{
    assert_no_null( M );
    assert( M->size() > 0 && M->isSquare() );

    unsigned int size = M->rows();

    if ( size == 1 ) {
        return M->getValue(0,0) ;
    }

    else if ( size == 2 )              // basic 2X2 sub-matrix
    {
        return ( M->getValue(0,0) * M->getValue(1,1) - M->getValue(1,0) * M->getValue(0,1) );
    }

    else                               // solve the next minor by building a
    {                                  // sub-matrix
        double det = 0;

                                       // for each column in sub-matrix
        for ( unsigned int c1 = 0 ; c1 < size ; ++c1 )
        {
            CMatrix *subM = new CMatrix( size-1, size-1 );
            unsigned int c2;

            for ( unsigned int r = 1 ; r < size ; ++r )
            {
                c2 = 0 ;               // start at first sub-matrix column position
                                       // loop to copy source matrix less one column
                for ( unsigned int c = 0 ; c < size ; ++c )
                {
                    if ( c == c1 )
                        continue ; // don't copy the minor column element

                    subM->setValue( r-1, c2, M->getValue(r,c) );
                                        // copy source element into new sub-matrix
                                        // i-1 because new sub-matrix is one row
                                        // (and column) smaller with excluded minors
                    ++c2 ;              // move to next sub-matrix column position
                }
            }

            det += pow(-1.0, 1.0 + c1 + 1.0) * M->getValue(0,c1) * determinant_recursive( subM ) ;
                                        // sum x raised to y power
                                        // recursively get determinant of next
                                        // sub-matrix which is now one
                                        // row & column smaller

            delete subM;
        }

        return det;
    }
}

//------------------------------------------------------------------------------
//
double CMatrix :: determinant() const
{
    assert( this->isSquare() );

    return determinant_recursive( this );
}

//------------------------------------------------------------------------------
//
CMatrix* CMatrix :: appendRight( const CMatrix *M ) const
{
    assert_no_null( M );
    assert( m_data->rows == M->m_data->rows );

    CMatrix *RM = new CMatrix( m_data->rows, m_data->cols + M->m_data->cols );

    for ( unsigned int r = 0; r < m_data->rows; ++r )
    {
        for ( unsigned int c = 0; c < m_data->cols; ++c )
            RM->setValue( r, c, this->getValue(r, c) );

        for ( unsigned int c = 0; c < M->m_data->cols; ++c )
            RM->setValue( r, m_data->cols + c, M->getValue(r, c) );
    }

    return RM;
}

//------------------------------------------------------------------------------
//
CMatrix* CMatrix :: appendBottom( const CMatrix *M ) const
{
    assert_no_null( M );
    assert( m_data->cols == M->m_data->cols );

    CMatrix *RM = new CMatrix( m_data->rows + M->m_data->rows, m_data->cols );

    unsigned long size = m_data->rows * m_data->cols;

    memcpy( RM->m_data->val, this->m_data->val, sizeof(double) * size );

    memcpy( &RM->m_data->val[ size ], M->m_data->val, sizeof(double) * ( M->m_data->rows * M->m_data->cols ) );

    return RM;
}

/**
*/