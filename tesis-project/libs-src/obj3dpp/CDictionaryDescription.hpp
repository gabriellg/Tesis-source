/* -----------------------------------------------------------------------------
 *  CDictionaryDescription.hpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

class CDictionaryDescription
{
    public:

        CDictionaryDescription();
        virtual ~CDictionaryDescription();

        void addDescription(const char *symbol, class IDescription **description);
        bool searchSymbol(const char *symbol, class IDescription **description) const;

    private:

        struct prv_dataPrivateDictioryDescription_t *m_dataPrivate;
};
