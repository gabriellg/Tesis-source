/* -----------------------------------------------------------------------------
 *  CPlugins.hpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

class CPlugins
{
    public:

        CPlugins(void);
        virtual ~CPlugins();

        bool readFunction(const char *nameLibrary, const char *symbol, void **addressFunction, char **messageError);

    private:

        struct prv_dataPrivatePlugins_t *m_dataPrivate;
};
