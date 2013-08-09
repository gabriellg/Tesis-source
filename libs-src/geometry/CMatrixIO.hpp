/* -----------------------------------------------------------------------------
*  CMatrixIO
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#ifndef   CMATRIXIO_HPP
#define   CMATRIXIO_HPP

class CMatrixIO
{
    public:
        static class CMatrix* fileLoad( const char* filename );
        static void fileSave( const CMatrix *matrix, const char* filename );

        static class CMatrix* streamLoad( class CFileInTxt **fileIn );
        static void streamSave( const CMatrix *matrix, class CFileOutTxt **fileOut );

        static void print( const class CMatrix *matrix, const char* name );
};

#endif