/* -----------------------------------------------------------------------------
*  CMatrix (2D array of doubles)
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#ifndef   CMATRIX_HPP
#define   CMATRIX_HPP

class CMatrix
{
    public:
        CMatrix( unsigned int rows, unsigned int columns );
        CMatrix( unsigned int rows, unsigned int columns, double value );
        CMatrix( const CMatrix *M );
        ~CMatrix();

        unsigned int rows() const;
        unsigned int cols() const;
        unsigned long size() const;

        double getValue( unsigned int row, unsigned int col ) const;
        void setValue( unsigned int row, unsigned int col, double value );
        void setAll( double value );
        void setDiagonal( double value );
        void setIdentity();

        void generateRandomData( int min, int max );

        void copyValues( const CMatrix *M );

        bool equal( const CMatrix *M ) const;

        bool isSquare() const;
        bool isDiagonal() const;
        bool isScalar() const;
        bool isUnit() const;
        bool isNull() const;
        bool isSymmetric() const;
        bool isUpperTiangular() const;
        bool isLowerTiangular() const;

        // BASIC OPERATIONS //

        CMatrix* matrixAddition( const CMatrix *M ) const;
        CMatrix* matrixSubtraction( const CMatrix *M ) const;
        CMatrix* matrixMultiplication( const CMatrix *M ) const;
        CMatrix* matrixDivision( const CMatrix *M ) const;

        void scalarAddition( double value );
        void scalarSubtraction( double value );
        void scalarLeftSubtraction( double value );
        void scalarMultiplication( double value );
        void scalarDivision( double value );
        void scalarLeftDivision( double value );

        // ADVANCED OPERATIONS //

        CMatrix* transpose() const;
        CMatrix* cofactor() const;
        CMatrix* adjoint() const;
        CMatrix* inverse() const;
        double determinant() const;

        void divideElementwise( const CMatrix *M );
        void multiplyElementwise( const CMatrix *M );
        void powerElementwise( double exponent );

        CMatrix* power( unsigned int exponent ) const;
        CMatrix* maxRing( const CMatrix *M ) const;

        double sumRow( unsigned int row ) const;
        double sumColumn( unsigned int column ) const;

        CMatrix* appendRight( const CMatrix *M ) const;
        CMatrix* appendBottom( const CMatrix *M ) const;

    private:
        struct prv_matrix_t *m_data;
};

#endif