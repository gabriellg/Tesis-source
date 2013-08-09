/* -----------------------------------------------------------------------------
*  CFileName
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

class CFileName
{
    public:
        static char *appenToFolder(const char *nameFolder, const char *nameFileOrFolderToAppend);
        static void addFinalSlashBar( char** foldername );

        static char* getPathName( const char* filename );
        static char* getFileName( const char* filename );
        static char* getFileNameWithoutExtension( const char* filename );

        static char* getExtension( const char* filename );
        static char* getExtensionLowercase( const char* filename );

        static bool compareExtension( const char* filename, const char* extension );

		static char* changeExtension(const char *filename, const char *newExtension);

        static char* generateNumberedFilename( const char* path, const char* filename,
                                unsigned int numeration, bool numberAsPrefix,
                                const char* separatorOfNumberAndName );
};
