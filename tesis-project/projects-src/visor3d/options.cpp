/* -----------------------------------------------------------------------------
 *  options.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "options.hpp"

#include "asrtbas.h"
#include "memory.h"

#include "CMaterial.hpp"
#include "CMesh.hpp"
#include "CMatrix4x4.hpp"
#include "CModel3d.hpp"
#include "CArray.hpp"
#include "CArrPoint2d.hpp"
#include "CArrPoint3d.hpp"
#include "CPolylines.hpp"
#include "CSurface2d.hpp"
#include "CString.hpp"
#include "CStackTransformation.hpp"
#include "CGeneratorSolid.hpp"
#include "CGeneratorModel.hpp"
#include "CReaderModel3d.hpp"
#include "CScaleColorWithHeight.hpp"
#include "CImg.hpp"

struct SPrvDataPrivateOptionCreateModel3d
{
    class CModel3d **modelApplication;
};

//-----------------------------------------------------------------------

static struct SPrvDataPrivateOptionCreateModel3d *prv_createOptionCreateModel3d(class CModel3d **modelApplication)
{
    struct SPrvDataPrivateOptionCreateModel3d *dataPrivate;

    dataPrivate = MALLOC(struct SPrvDataPrivateOptionCreateModel3d);

    dataPrivate->modelApplication = modelApplication;

    return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroyOptionCreateModel3d(struct SPrvDataPrivateOptionCreateModel3d **dataPrivate)
{
    FREE_T(dataPrivate, struct SPrvDataPrivateOptionCreateModel3d);
}

//-----------------------------------------------------------------------

COptionCreateModel3d::COptionCreateModel3d(class CModel3d **modelApplication)
{
    m_dataPrivate = prv_createOptionCreateModel3d(modelApplication);
}

//-----------------------------------------------------------------------

COptionCreateModel3d::~COptionCreateModel3d()
{
    prv_destroyOptionCreateModel3d(&m_dataPrivate);
}

//-----------------------------------------------------------------------

enum IOptionOGL::EResult COptionCreateModel3d::execute(void)
{
    class CModel3d *model;

    assert_no_null(m_dataPrivate);
    model = createModel();

    if (*(m_dataPrivate->modelApplication) != NULL)
        delete *(m_dataPrivate->modelApplication);

    *(m_dataPrivate->modelApplication) = model;

    return IOptionOGL::REGENERATE;
}

//-----------------------------------------------------------------------

static class CModel3d *prv_createModel(const char *nameMaterial, class CMaterial **material, class CMesh **mesh)
{
    class CModel3d *model;

    assert_no_null(mesh);

    model = new CModel3d;

    model->appendMaterial(material);
    model->appendMesh(nameMaterial, *mesh);

    delete *mesh;
    *mesh = NULL;

    return model;
}

//-----------------------------------------------------------------------

class CModel3d *COptionCreateCube::createModel(void)
{
    class CMaterial *material;
    class CMesh *mesh;
    const char *PRV_MATERIAL_CUBE = "Cube";
    class CMatrix4x4 *matrix;
    class CImg *imageTexture;

    matrix = CMatrix4x4::createScale(10., 10., 10.);

    imageTexture = new CImg("./img/metal.jpg");

    mesh = CGeneratorSolid::createUnitCube();
    mesh->applyMatrix(matrix);
    mesh->calculateCoordsTexturesInPlane(CMesh::TEXTURE_DECAL);

    delete matrix;

    material = new CMaterial(PRV_MATERIAL_CUBE, 0., 1., 0., 1., &imageTexture);
    return prv_createModel(PRV_MATERIAL_CUBE, &material, &mesh);
}

//-----------------------------------------------------------------------

class CModel3d *COptionCreateGrid3d::createModel(void)
{
    class CModel3d *model;
    class CMaterial *material;
    class CMesh *meshCylinder;
    const char *PRV_MATERIAL_CYLINDER = "Cylinder";
    class CArrPoint2d *circle;
    class CArrPoint3d *circle0, *circle1;
    class CArray<CArrPoint3d> *controlPoints;

    circle = CPolylines::createCircle(2., 32);

    circle0 = CPolylines::createPolyline2dTo3d(circle, 0.);
    circle1 = CPolylines::createPolyline2dTo3d(circle, 5.);
    controlPoints = new CArray<CArrPoint3d>(2);

    controlPoints->set(0, circle0);
    controlPoints->set(1, circle1);

    meshCylinder = CGeneratorModel::createWithGrid3d((const class CArrayRef<CArrPoint3d> *)controlPoints);
    material = new CMaterial(PRV_MATERIAL_CYLINDER, 0., 0., 1., 1.);

    model = prv_createModel(PRV_MATERIAL_CYLINDER, &material, &meshCylinder);

    delete controlPoints;
    delete circle;

    return model;
}

//-----------------------------------------------------------------------

static class CArray<CArrPoint3d> *prv_polylinesToPath(
                    unsigned long numPolylines,
                    const class CArrPoint2d *polyline,
                    double angleBegin, double angleEnd,
                    double UxRotation, double UyRotation, double UzRotation)
{
    class CArray<CArrPoint3d> *polylinesInPathWithSamePoints;
    class CStackTransformation *stackTransformation;
    double dAngle, incrAngleDegree;

    stackTransformation = new CStackTransformation();
    polylinesInPathWithSamePoints = new CArray<CArrPoint3d>(numPolylines);

    dAngle = angleEnd - angleBegin;

    stackTransformation->appendRotation(angleBegin, UxRotation, UyRotation, UzRotation);

    incrAngleDegree = dAngle / (numPolylines - 1);

    for (unsigned long i = 0; i < numPolylines; i++)
    {
        class CArrPoint3d *polyline3d;

        polyline3d = stackTransformation->transformationPoints2d(polyline);
        polylinesInPathWithSamePoints->set(i, polyline3d);

        stackTransformation->appendRotation(incrAngleDegree, UxRotation, UyRotation, UzRotation);
    }

    delete stackTransformation;

    return polylinesInPathWithSamePoints;
}

//-----------------------------------------------------------------------

static class CArrPoint2d *prv_polylineToPath(double width, double height, double dx)
{
    class CArrPoint2d *polyline, *polylineInverted;

    polyline = CPolylines::createRectangleCentredInOrigin(width, height);
    CPolylines::move2d(polyline, dx + width / 2., 0.);
    polylineInverted = polyline->invert();

    delete polyline;

    return polylineInverted;
}

//-----------------------------------------------------------------------

class CModel3d *COptionCreatePathOpen::createModel(void)
{
    class CModel3d *model;
    class CMaterial *material;
    const char *PRV_MATERIAL_PATH_OPENED = "PathOpened";
    class CMesh *meshPath;
    unsigned long numPolylines;
    class CArrPoint2d *polyline;
    class CArray<CArrPoint3d> *polylinesInPathWithSamePoints;

    polyline = prv_polylineToPath(32., 10., 8.);

    numPolylines = 32;
    polylinesInPathWithSamePoints = prv_polylinesToPath(numPolylines, polyline, 0., 90., 0., 1., 0.);

    meshPath = CGeneratorModel::createWithPathOpen((const class CArrayRef<CArrPoint3d> *)polylinesInPathWithSamePoints);

    material = new CMaterial(PRV_MATERIAL_PATH_OPENED, 0., 1., 1., 1.);
    model = prv_createModel(PRV_MATERIAL_PATH_OPENED, &material, &meshPath);

    delete polyline;
    delete polylinesInPathWithSamePoints;

    return model;
}

//-----------------------------------------------------------------------

class CModel3d *COptionCreatePathClosed::createModel(void)
{
    class CModel3d *model;
    class CMaterial *material;
    const char *PRV_MATERIAL_PATH_CLOSED = "PathClosed";
    class CMesh *meshPath;
    unsigned long numPolylines;
    class CArrPoint2d *polyline;
    class CArray<CArrPoint3d> *polylinesInPathWithSamePoints;

    polyline = prv_polylineToPath(32., 10., 8.);

    numPolylines = 32;
    polylinesInPathWithSamePoints = prv_polylinesToPath(numPolylines, polyline, 0., 270., 0., 1., 0.);

    meshPath = CGeneratorModel::createWithPathClosed((const class CArrayRef<CArrPoint3d> *)polylinesInPathWithSamePoints);

    material = new CMaterial(PRV_MATERIAL_PATH_CLOSED, 1., 0., 1., 1.);
    model = prv_createModel(PRV_MATERIAL_PATH_CLOSED, &material, &meshPath);

    delete polyline;
    delete polylinesInPathWithSamePoints;

    return model;
}

//-----------------------------------------------------------------------

class CModel3d *COptionCreatePrism::createModel(void)
{
    class CModel3d *model;
    class CMaterial *material;
    const char *PRV_MATERIAL_PRISM = "Prism";
    class CMesh *meshPrism;
    class CArrPoint2d *polyline;

    polyline = CPolylines::createPoligonRegular(5., 5);

    meshPrism = CGeneratorSolid::createSolidByExtrusion(polyline, 10.);

    material = new CMaterial(PRV_MATERIAL_PRISM, 0.6, 0., 0.4, 1.);
    model = prv_createModel(PRV_MATERIAL_PRISM, &material, &meshPrism);

    delete polyline;

    return model;
}

//-----------------------------------------------------------------------

class CModel3d *COptionCreateSurface::createModel(void)
{
    class CModel3d *model;
    class CMaterial *material;
    const char *PRV_MATERIAL_SURFACE = "Surface";
    class CMesh *meshPrism;
    class CSurface2d *surface;
    class CArrPoint2d *polyline;

    surface = new CSurface2d;

    polyline = CPolylines::createPoligonRegular(5., 4);
    surface->appendContour(&polyline);

    polyline = CPolylines::createPoligonRegular(3., 4);
    surface->appendHole(&polyline);

    meshPrism = CGeneratorSolid::createSolidWithSurfaceByExtrusion(surface, 10.);

    material = new CMaterial(PRV_MATERIAL_SURFACE, 0.4, 1., 0.6, 1.);
    model = prv_createModel(PRV_MATERIAL_SURFACE, &material, &meshPrism);

    delete surface;

    return model;
}

//-----------------------------------------------------------------------

class CModel3d *COptionCreateCylinder::createModel(void)
{
    class CMaterial *material;
    const char *PRV_MATERIAL_CYLINDER = "Cylinder";
    class CMesh *meshCylinder;

    meshCylinder = CGeneratorSolid::createCylinder(2., 1., 5., 32);

    material = new CMaterial(PRV_MATERIAL_CYLINDER, 0.4, 0.6, 0.6, 1.);
    return prv_createModel(PRV_MATERIAL_CYLINDER, &material, &meshCylinder);
}

//-----------------------------------------------------------------------

COptionCreateModelRead::COptionCreateModelRead(
                    const char *nameFile, class CModel3d **modelApplication) : COptionCreateModel3d(modelApplication)
{
    m_nameFile = CString::copy(nameFile);
};

//-----------------------------------------------------------------------

COptionCreateModelRead::~COptionCreateModelRead()
{
    CString::free(&m_nameFile);
}

//-----------------------------------------------------------------------

class CModel3d *COptionCreateModelRead::createModel(void)
{
    return CReaderModel3d::readModel3d(m_nameFile);
}

//-----------------------------------------------------------------------

class CModel3d *COptionCreateWithImage::createModel(void)
{
    class CMaterial *material;
    const char *PRV_MATERIAL_WITH_IMAGE = "WithImage";
    class CMesh *meshWithImage;
    class CImg *image;
    class CScaleColorWithHeight *colorsAndHeights;
    const char *fileImage;

    fileImage = "./img/prueba3.png";
    image = new CImg(fileImage);

    colorsAndHeights = new CScaleColorWithHeight;
    colorsAndHeights->appendColorWithHeight(0, 0, 0, 0.);
    colorsAndHeights->appendColorWithHeight(255, 255, 255, 1.);

    meshWithImage = CGeneratorModel::createGridWithImage(image, colorsAndHeights, 0.5, 0.5);

    material = new CMaterial(PRV_MATERIAL_WITH_IMAGE, 0.4, 0.6, 0.6, 1.);

    delete image;
    delete colorsAndHeights;

    return prv_createModel(PRV_MATERIAL_WITH_IMAGE, &material, &meshWithImage);
}
