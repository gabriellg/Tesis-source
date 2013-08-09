/* -----------------------------------------------------------------------------
 *  CFileSystem.hpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

class CFileSystem
{
    public:

        static bool existFile(const char *path);
        static bool existFolder(const char *pathFolder);

        static void rename(const char *filename, const char *newFilename);
        static void removeFile(const char *filename);
        static void copyFiles(const char *pathOrigin, const char *pathDestiny);

        static bool isExecutableFile(const char *filename);
        static bool isFolder(const char *path);

        static unsigned long modificationDate(const char *filename);
        static bool firstIsMoreModern(const char *filename1, const char *filename2);
};
