/* -----------------------------------------------------------------------------
 *  CDescriptionsGL.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CDescriptionsGL.inl"

#include "asrtbas.h"
#include "memory.h"

#include "CMaterial.hpp"
#include "IGraphics.hpp"

static const double prv_HEIGTH_TRUNKS = 4.;

static const double prv_WIDTH_BASE = 2.;
static const double prv_HEIGHT_BRACHES = 10.;

static const char *prv_LAYER_FORREST = "Forrest";
static const char *prv_LAYER_TRUNKS = "Trunks";
static const char *prv_LAYER_BRANCHES = "Branches";
static const char *prv_LAYER_LIGHTNING = "Lightning";

struct prv_dataPrivateDescriptionForrestGL_t
{
    double sizeX, sizeY;
};

//---------------------------------------------------------------

static void prv_drawTrunks(class IGraphics *graphics, double heigth)
{
    graphics->pushTransformation();

    graphics->scale(1., 1., heigth);
    graphics->drawUnitCylinder(IGraphics::LEVELDETAIL_NORMAL, IGraphics::TYPEDRAW_SOLID);

    graphics->popTransformation();
}

//---------------------------------------------------------------

static void prv_drawBranches(class IGraphics *graphics, double widthBase, double heightBranches)
{
    assert_no_null(graphics);

    graphics->pushTransformation();

    graphics->scale(widthBase, widthBase, heightBranches);

    graphics->setMaterial(prv_LAYER_BRANCHES);
    graphics->drawUnitCone(IGraphics::LEVELDETAIL_NORMAL, IGraphics::TYPEDRAW_SOLID);

    graphics->popTransformation();
}

//-----------------------------------------------------------------------

static struct prv_dataPrivateDescriptionForrestGL_t *prv_createDescriptionForrestGL(double sizeX, double sizeY)
{
    struct prv_dataPrivateDescriptionForrestGL_t *dataPrivate;

    dataPrivate = MALLOC(struct prv_dataPrivateDescriptionForrestGL_t);

    dataPrivate->sizeX = sizeX;
    dataPrivate->sizeY = sizeY;

    return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroyDescriptionForrestGL(struct prv_dataPrivateDescriptionForrestGL_t **dataPrivate)
{
    FREE_T(dataPrivate, struct prv_dataPrivateDescriptionForrestGL_t);
}

//-----------------------------------------------------------------------

CDescriptionForrestGL::CDescriptionForrestGL(double sizeX, double sizeY)
{
    m_dataPrivate = prv_createDescriptionForrestGL(sizeX, sizeY);
}

//-----------------------------------------------------------------------

CDescriptionForrestGL::~CDescriptionForrestGL()
{
    prv_destroyDescriptionForrestGL(&m_dataPrivate);
}

//---------------------------------------------------------------

void CDescriptionForrestGL::drawSymbol(class IGraphics *graphics, const class IDataSymbol *dataFigure)
{
    assert_no_null(m_dataPrivate);
    assert_no_null(graphics);
    assert(dataFigure == NULL);

    graphics->setMaterial(prv_LAYER_FORREST);

    graphics->pushTransformation();
    graphics->scale(m_dataPrivate->sizeX, m_dataPrivate->sizeY, 1.);
    graphics->drawUnitCube(IGraphics::TYPEDRAW_SOLID);
    graphics->popTransformation();
};

//---------------------------------------------------------------

void CDescriptionForrestGL::defineLayer(class IGraphics *graphics)
{
    class CMaterial *material;
    double red, green, blue;

    red = 255.;
    green = 224.;
    blue = 140.;

    material = new CMaterial(prv_LAYER_FORREST, red / 255., green / 255., blue / 255., 1.);
    graphics->defineMaterial(material);
    delete material;
}

//---------------------------------------------------------------

void CDescriptionTreeGL::drawSymbol(class IGraphics *graphics, const class IDataSymbol *dataFigure)
{
    assert_no_null(graphics);
    assert(dataFigure == NULL);

    graphics->setMaterial(prv_LAYER_TRUNKS);

    prv_drawTrunks(graphics, prv_HEIGTH_TRUNKS);

    graphics->pushTransformation();

    graphics->traslation(0., 0., prv_HEIGTH_TRUNKS);
    prv_drawBranches(graphics, prv_WIDTH_BASE, prv_HEIGHT_BRACHES);

    graphics->popTransformation();
}

//---------------------------------------------------------------

void CDescriptionTreeGL::defineLayer(class IGraphics *graphics)
{
    class CMaterial *materialTrunks, *materialBranches;

    assert_no_null(graphics);

    materialTrunks = new CMaterial(prv_LAYER_TRUNKS, 184. / 255., 120. / 255., 18 / 255., 1.);
    materialBranches = new CMaterial(prv_LAYER_BRANCHES, 28. / 255., 162. / 255., 91. / 255., 1.);

    graphics->defineMaterial(materialBranches);
    graphics->defineMaterial(materialTrunks);

    delete materialTrunks;
    delete materialBranches;
}

//---------------------------------------------------------------

void CDescriptionCylinderLightningGL::drawSymbol(class IGraphics *graphics, const class IDataSymbol *dataFigure)
{
    assert_no_null(graphics);
    assert(dataFigure == NULL);

    graphics->setMaterial(prv_LAYER_LIGHTNING);
    graphics->drawUnitCylinder(IGraphics::LEVELDETAIL_NORMAL, IGraphics::TYPEDRAW_SOLID);
}

//---------------------------------------------------------------

void CDescriptionCylinderLightningGL::defineLayer(class IGraphics *graphics)
{
    class CMaterial *material;
    double red, green, blue;

    assert_no_null(graphics);

    red = 227.;
    green = 225.;
    blue = 80.;

    material = new CMaterial(prv_LAYER_LIGHTNING, red / 255, green / 255., blue / 255., 1.);
    graphics->defineMaterial(material);
    delete material;
}

