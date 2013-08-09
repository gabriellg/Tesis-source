/* -----------------------------------------------------------------------------
*  CMatrixIO
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include "CMatrixIO.hpp"

#include <iostream>
#include <iomanip>   // setw(4)

#include "CMatrix.hpp"
#include "CFileInTxt.hpp"
#include "CFileOutTxt.hpp"
#include "asrtbas.h"
using namespace std;

//------------------------------------------------------------------------------
//
class CMatrix* CMatrixIO :: fileLoad( const char* filename )
{
    class CFileInTxt *fileIn;
    class CMatrix *matrix;

    fileIn = new CFileInTxt( filename );

    matrix = CMatrixIO::streamLoad( &fileIn );
    assert_no_null( matrix );

    return matrix;
}

//------------------------------------------------------------------------------
//
void CMatrixIO :: fileSave( const CMatrix *matrix, const char* filename )
{
    class CFileOutTxt *fileOut;

    assert_no_null( matrix );

    fileOut = new CFileOutTxt( filename );

    CMatrixIO::streamSave( matrix, &fileOut );

    delete fileOut;
}

//------------------------------------------------------------------------------
//
class CMatrix* CMatrixIO :: streamLoad( class CFileInTxt **fileIn )
{
    class CMatrix *matrix;
    unsigned int rows, columns;

    assert_no_null( *fileIn );

    rows = (*fileIn)->readULong();
    columns = (*fileIn)->readULong();

    matrix = new CMatrix( rows, columns );


    for (unsigned int r=0; r < rows; ++r)
    {
        for (unsigned int c=0; c < columns; ++c)
        {
            double value;

            value = (*fileIn)->readDouble();

            matrix->setValue( r, c, value );
        }
    }

    return matrix;
}

//------------------------------------------------------------------------------
//
void CMatrixIO :: streamSave( const CMatrix *matrix, class CFileOutTxt **fileOut )
{
    unsigned long rows, columns;

    assert_no_null( matrix );
    assert_no_null( *fileOut );

    rows = matrix->rows();
    columns = matrix->cols();

    (*fileOut)->writeULong( rows );
    (*fileOut)->writeChar( ' ' );
    (*fileOut)->writeULong( columns );
    (*fileOut)->writeChar( '\n' );

    for (unsigned int r=0; r < rows; ++r)
    {
        for (unsigned int c=0; c < columns; ++c)
        {
            if( c > 0 )
                (*fileOut)->writeChar( ' ' );

            (*fileOut)->writeDouble( matrix->getValue( r, c ) );
        }
        (*fileOut)->writeChar( '\n' );
    }
}

//------------------------------------------------------------------------------
//
void CMatrixIO :: print( const class CMatrix *matrix, const char* name )
{
    unsigned int rows, cols;

    assert_no_null( matrix );

    rows = matrix->rows();
    cols = matrix->cols();

    cout << endl << name << '[';
    cout << rows << ',' << cols << ']' << endl;

    for ( unsigned int r=0; r < rows; ++r )
    {
        for ( unsigned int c=0; c < cols; ++c )
        {
            cout << " " << setw(10) << matrix->getValue( r, c );
        }
        cout << endl;
    }
}
