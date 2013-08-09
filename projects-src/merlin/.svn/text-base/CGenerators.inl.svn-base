/* -----------------------------------------------------------------------------
 *  CGenerators.inl
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

class CGenerators
{
    public:

        CGenerators();
        virtual ~CGenerators();

        void add(class IGenerator *generator);

        bool isResourceSource(const char *fileSource) const;
        bool isResourceGenerated(const char *fileGenerated) const;

        bool mustGenerate(const char *fileSource) const;
        void generate(const char *fileSource, struct marco_t *marco) const;

    private:

        struct prv_dataPrivateGenerators_t *m_dataPrivate;
};
