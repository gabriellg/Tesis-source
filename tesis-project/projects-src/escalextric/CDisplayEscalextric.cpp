/* -----------------------------------------------------------------------------
 *  CDisplayEscalextric.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CDisplayEscalextric.inl"

#include "CWorldEscalextric.inl"
#include "CDataCircuit.inl"

#include "asrtbas.h"

#include "CDisplaySprite.hpp"
#include "CImg.hpp"
#include "CPolylines.hpp"
#include "CDescriptionSpriteOneImage.hpp"
#include "CReaderModel3d.hpp"
#include "CArrPoint2d.hpp"

#include "CDisplay3D.hpp"
#include "CPositionCamera.hpp"
#include "CColor.hpp"
#include "CLight.hpp"
#include "CDescriptionModel3d.hpp"
#include "CModel3d.hpp"
#include "CMaterial.hpp"
#include "CMesh.hpp"
#include "CGeneratorSolid.hpp"
#include "CGeneratorModel.hpp"
#include "CStackTransformation.hpp"

const char *CDisplayEscalextric::SYMBOL_CIRCUIT = "Circuit";
const char *CDisplayEscalextric::SYMBOL_CAR = "Car";
const char *CDisplayEscalextric::SYMBOL_SKY = "Sky";
const char *CDisplayEscalextric::SYMBOL_FLOOR = "Floor";

//-----------------------------------------------------------------------
//
class ITraslatorDisplay *CDisplayEscalextric::createDisplaySprite(
                            const class CDataCircuit *dataCircuit,
                            class CWorldEscalextric *worldEscalextric)
{
    class CDisplaySprite *displaySprite;
    class IDescription *descriptionCircuit, *descriptionCar;
    class CImg *circuitImg, *imageCar;
    bool originCartesian;
    unsigned long sizeXTrack, sizeYTrack;
    unsigned long sizeXCircuit, sizeYCircuit;

    assert_no_null(dataCircuit);
    assert_no_null(worldEscalextric);

    worldEscalextric->setWorld2d();

    circuitImg = dataCircuit->image(&sizeXTrack, &sizeYTrack);
    circuitImg->size(&sizeXCircuit, &sizeYCircuit);
    descriptionCircuit = new CDescriptionSpriteOneImage(&circuitImg);

    imageCar = new CImg("./imagesCircuit/car.png");
    descriptionCar = CDescriptionSpriteOneImage::createSpriteOneImageInCenter(&imageCar);

    originCartesian = true;
    displaySprite = new CDisplaySprite(originCartesian, sizeXTrack, sizeYTrack, sizeXCircuit, sizeYCircuit, worldEscalextric);

    displaySprite->appendDescription(SYMBOL_CIRCUIT, &descriptionCircuit);
    displaySprite->appendDescription(SYMBOL_CAR, &descriptionCar);

    return displaySprite;
}

//-----------------------------------------------------------------------

static class CPositionCamera *prv_createPositionCameraDefault(void)
{
    double eyeX, eyeY, eyeZ;
    double pointReferenceX, pointReferenceY, pointReferenceZ;
    double upX, upY, upZ;

    eyeX = 0.;
    eyeY = 0.;
    eyeZ = 15.;

    pointReferenceX = 0.;
    pointReferenceY = 0.;
    pointReferenceZ = 0.;

    upX = 0.;
    upY = 1.;
    upZ = 0.;

    return new CPositionCamera(
                    eyeX, eyeY, eyeZ,
                    pointReferenceX, pointReferenceY, pointReferenceZ,
                    upX, upY, upZ);
}

//-----------------------------------------------------------------------

static class CLight *prv_createLight(void)
{
    class CColor *colorAmbient, *colorDiffuse, *colorSpecular;
    double xPosLight, yPosLight, zPosLight;

    xPosLight = 7.;
    yPosLight = 7.;
    zPosLight = 15.;

    colorAmbient = new CColor(0.4, 0.4, 0.4);
    colorDiffuse = new CColor(0.7, 0.7, 0.7);
    colorSpecular = new CColor(0.8, 0.6, 0.6);

    return new CLight(&colorAmbient, &colorDiffuse, &colorSpecular, xPosLight, yPosLight, zPosLight);
}

//-----------------------------------------------------------------------
//
static struct CModel3d *prv_createModelCar(void)
{
    class CModel3d *modelCar;
    const double PRV_SIZE_CAR = .5;

    modelCar = CReaderModel3d::readModel3d("./objetos3d/car.3ds");

    modelCar->insideCube(PRV_SIZE_CAR, PRV_SIZE_CAR, PRV_SIZE_CAR);

    return modelCar;
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
//
static struct CModel3d *prv_createModelSky(void)
{
    const char *PRV_MATERIAL_SKY = "Sky";
    class CModel3d *model;
    class CMaterial *material;
    class CMesh *meshSky;
    class CImg *imageSky;

    meshSky = CGeneratorModel::createSky(64, 300., 0.01, 10);
    meshSky->calculateCoordsTexturesXY(CMesh::TEXTURE_DECAL);

    imageSky = new CImg("./imagesCircuit/sky.png");
    material = new CMaterial(PRV_MATERIAL_SKY, 0.3, 0.3, 0.3, 1., &imageSky);

    model = prv_createModel(PRV_MATERIAL_SKY, &material, &meshSky);

    model->setIsInLimits(false);

    return model;
}

//-----------------------------------------------------------------------
//
static struct CModel3d *prv_createModelFloor(void)
{
    const char *PRV_MATERIAL_FLOOR = "Floor";
    class CModel3d *model;
    class CMaterial *material;
    class CMesh *meshFloor;
    class CImg *imageFloor;
    class CArrPoint2d *rectangle;
    double x1, y1, x2, y2, x3, y3, x4, y4;
    double Nx, Ny, Nz;

    rectangle = CPolylines::createRectangleCentredInOrigin(150., 150.);
    rectangle->get(0, &x1, &y1);
    rectangle->get(1, &x2, &y2);
    rectangle->get(2, &x3, &y3);
    rectangle->get(3, &x4, &y4);

    Nx = 0.; Ny = 0.; Nz = 1.;

    meshFloor = new CMesh();
    meshFloor->appendQuad(
                    x1, y1, 0., Nx, Ny, Nz,
                    x2, y2, 0., Nx, Ny, Nz,
                    x3, y3, 0., Nx, Ny, Nz,
                    x4, y4, 0., Nx, Ny, Nz);

    meshFloor->calculateCoordsTexturesXY(CMesh::TEXTURE_REPEAT);

    imageFloor = new CImg("./imagesCircuit/grass.jpg");
    material = new CMaterial(PRV_MATERIAL_FLOOR, 0.3, 0.3, 0.3, 1., &imageFloor);
    material->setPriorityZFighting(CMaterial::LOW);
    model = prv_createModel(PRV_MATERIAL_FLOOR, &material, &meshFloor);
    model->setIsInLimits(false);

    return model;
}

//-----------------------------------------------------------------------
//
class ITraslatorDisplay *CDisplayEscalextric::createDisplayGL(
                            const class CDataCircuit *dataCircuit,
                            class CWorldEscalextric *worldEscalextric)
{
    class CDisplay3D *display3d;
    class CLight *light;
    class CPositionCamera *positionCamera;
    class IDescription *descriptionCircuit, *descriptionCar, *descriptionSky, *descriptionFloor;
    class CModel3d *modelCircuit, *modelCar, *modelSky, *modelFloor;

    assert_no_null(dataCircuit);
    assert_no_null(worldEscalextric);

    worldEscalextric->setWorld3d();

    light = prv_createLight();
    positionCamera = prv_createPositionCameraDefault();

    modelCircuit = dataCircuit->model3d();
    descriptionCircuit = new CDescriptionModel3d(&modelCircuit);

    modelCar = prv_createModelCar();
    descriptionCar = new CDescriptionModel3d(&modelCar);

    modelSky = prv_createModelSky();
    descriptionSky = new CDescriptionModel3d(&modelSky);

    modelFloor = prv_createModelFloor();
    descriptionFloor = new CDescriptionModel3d(&modelFloor);

    display3d = new CDisplay3D(worldEscalextric, &light, &positionCamera);

    display3d->appendDescription(SYMBOL_CIRCUIT, &descriptionCircuit);
    display3d->appendDescription(SYMBOL_CAR, &descriptionCar);
    display3d->appendDescription(SYMBOL_SKY, &descriptionSky);
    display3d->appendDescription(SYMBOL_FLOOR, &descriptionFloor);

    return display3d;
}
