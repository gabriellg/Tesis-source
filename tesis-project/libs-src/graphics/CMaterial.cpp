/* -----------------------------------------------------------------------------
 *  CMaterial.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CMaterial.hpp"

#include "asrtbas.h"
#include "memory.h"

#include "CString.hpp"
#include "CImg.hpp"

struct SDataPrivateMaterial_t
{
    char *nameMaterial;

    enum CMaterial::EPriorityZFighting zFighting;

    float r, b, g, a;

    bool withTexture;
    class CImg *textureImage;
};

//-----------------------------------------------------------------------

static struct SDataPrivateMaterial_t *prv_createMaterial(
                const char *nombreMaterial,
                enum CMaterial::EPriorityZFighting zFighting,
                float r, float g, float b, float a,
                bool withTexture,
                class CImg **textureImage)
{
    struct SDataPrivateMaterial_t *dataPrivate;

    dataPrivate = MALLOC(struct SDataPrivateMaterial_t);

    dataPrivate->nameMaterial = CString::copy(nombreMaterial);

    dataPrivate->zFighting = zFighting;

    dataPrivate->r = r;
    dataPrivate->g = g;
    dataPrivate->b = b;
    dataPrivate->a = a;

    dataPrivate->withTexture = withTexture;
    dataPrivate->textureImage = ASSIGN_PP(textureImage, class CImg);

    return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroyMaterial(struct SDataPrivateMaterial_t **dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(*dataPrivate);

    CString::free(&(*dataPrivate)->nameMaterial);

    if ((*dataPrivate)->withTexture == true)
    {
        assert_no_null((*dataPrivate)->textureImage);
        delete (*dataPrivate)->textureImage;
    }

    FREE_T(dataPrivate, struct SDataPrivateMaterial_t);
}

//-----------------------------------------------------------------------

CMaterial::CMaterial(const char *nameMaterial, float r, float g, float b, float a)
{
    bool withTexture;
    class CImg *textureImage;

    withTexture = false;
    textureImage = NULL;
    m_dataPrivate = prv_createMaterial(nameMaterial, CMaterial::MEDIUM, r, g, b, a, withTexture, &textureImage);
}

//-----------------------------------------------------------------------

CMaterial::CMaterial(const char *nameMaterial, float r, float g, float b, float a, class CImg **textureImage)
{
    bool withTexture;

    withTexture = true;
    m_dataPrivate = prv_createMaterial(nameMaterial, CMaterial::MEDIUM, r, g, b, a, withTexture, textureImage);
}

//-----------------------------------------------------------------------

CMaterial::CMaterial(const CMaterial *material)
{
    class CImg *textureCopy;

    assert_no_null(material);

    if (material->m_dataPrivate->withTexture == true)
        textureCopy = new CImg(material->m_dataPrivate->textureImage);
    else
        textureCopy = NULL;

    m_dataPrivate = prv_createMaterial(
                            material->m_dataPrivate->nameMaterial,
                            material->m_dataPrivate->zFighting,
                            material->m_dataPrivate->r,
                            material->m_dataPrivate->g,
                            material->m_dataPrivate->b,
                            material->m_dataPrivate->a,
                            material->m_dataPrivate->withTexture,
                            &textureCopy);
}

//-----------------------------------------------------------------------

CMaterial::~CMaterial()
{
    prv_destroyMaterial(&m_dataPrivate);
}

//-----------------------------------------------------------------------

void CMaterial::setPriorityZFighting(enum CMaterial::EPriorityZFighting zFighting)
{
    assert_no_null(m_dataPrivate);
    m_dataPrivate->zFighting = zFighting;
}

//-----------------------------------------------------------------------

enum CMaterial::EPriorityZFighting CMaterial::getPriorityZFighting() const
{
    assert_no_null(m_dataPrivate);
    return m_dataPrivate->zFighting;
}

//-----------------------------------------------------------------------

bool CMaterial::isSameMaterial(const class CMaterial *material) const
{
    assert_no_null(material);
    assert_no_null(material->m_dataPrivate);
    assert_no_null(m_dataPrivate);

    return CString::equal(m_dataPrivate->nameMaterial, material->m_dataPrivate->nameMaterial);
}

//-----------------------------------------------------------------------

bool CMaterial::isMaterial(const char *nameMaterial) const
{
    assert_no_null(m_dataPrivate);
    return CString::equal(m_dataPrivate->nameMaterial, nameMaterial);
}

//-----------------------------------------------------------------------

const char *CMaterial::getNameMaterial(void) const
{
    assert_no_null(m_dataPrivate);
    return m_dataPrivate->nameMaterial;
}

//-----------------------------------------------------------------------

bool CMaterial::hasTexture(void) const
{
    assert_no_null(m_dataPrivate);
    return m_dataPrivate->withTexture;
}

//-----------------------------------------------------------------------

const class CImg *CMaterial::getTexture(void) const
{
    assert_no_null(m_dataPrivate);
    return m_dataPrivate->textureImage;
}

//-----------------------------------------------------------------------

void CMaterial::getColorRGBA(float *r, float *b, float *g, float *a) const
{
    assert_no_null(m_dataPrivate);
    assert_no_null(r);
    assert_no_null(g);
    assert_no_null(b);
    assert_no_null(a);

    *r = m_dataPrivate->r;
    *b = m_dataPrivate->g;
    *g = m_dataPrivate->b;
    *a = m_dataPrivate->a;
}
