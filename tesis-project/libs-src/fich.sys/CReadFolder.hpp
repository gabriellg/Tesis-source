/* -----------------------------------------------------------------------------
 *  CReadFolder.hpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

class CReadFolder
{
    public:

        CReadFolder(const char *pathFolder);
        virtual ~CReadFolder();

        bool hasFiles() const;

        class CArrayString *files(void) const;
        class CArrayString *subFolder(void) const;

    private:

        struct prv_dataPrivateReadFolder_t *m_dataPrivate;
};
