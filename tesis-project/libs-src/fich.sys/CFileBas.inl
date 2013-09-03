/* -----------------------------------------------------------------------------
*  CFileBas
*  Author: Gabriel López García (gabriel.lg AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

class CFileBas
{
    public:

        enum TypeOpen_t
        {
            READ, WRITE
        };

        static void *openBin(const char *nameFile, enum TypeOpen_t typeOpen);
        static void close(void **file);
};

