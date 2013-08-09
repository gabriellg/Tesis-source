/* -----------------------------------------------------------------------------
 *  CMaterial.hpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

class CMaterial
{
    public:

        CMaterial(const char *nameMaterial, float r, float g, float b, float a);
        CMaterial(const char *nameMaterial, float r, float g, float b, float a, class CImg **textureImage);
        CMaterial(const CMaterial *material);

        virtual ~CMaterial();

        bool isSameMaterial(const class CMaterial *material) const;
        bool isMaterial(const char *nameMaterial) const;

        const char *getNameMaterial(void) const;

        bool hasTexture(void) const;
        const class CImg *getTexture(void) const;

        void getColorRGBA(float *r, float *b, float *g, float *a) const;

    private:

        struct SDataPrivateMaterial_t *m_dataPrivate;
};
