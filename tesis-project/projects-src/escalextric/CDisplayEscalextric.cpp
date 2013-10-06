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
#include "CDescriptionSpriteOneImage.hpp"
#include "CReaderModel3d.hpp"

#include "CDisplay3D.hpp"
#include "CPositionCamera.hpp"
#include "CColor.hpp"
#include "CLight.hpp"
#include "CDescriptionModel3d.hpp"
#include "CModel3d.hpp"
#include "CMaterial.hpp"
#include "CMesh.hpp"
#include "CGeneratorSolid.hpp"
#include "CStackTransformation.hpp"

const char *CDisplayEscalextric::SYMBOL_CIRCUIT = "Circuit";
const char *CDisplayEscalextric::SYMBOL_CAR = "Car";

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

    modelCar->InsideCube(PRV_SIZE_CAR, PRV_SIZE_CAR, PRV_SIZE_CAR);

    return modelCar;
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
    class IDescription *descriptionCircuit, *descriptionCar;
    class CModel3d *modelCircuit, *modelCar;

    assert_no_null(dataCircuit);
    assert_no_null(worldEscalextric);

    worldEscalextric->setWorld3d();

    light = prv_createLight();
    positionCamera = prv_createPositionCameraDefault();

    modelCircuit = dataCircuit->model3d();
    descriptionCircuit = new CDescriptionModel3d(&modelCircuit);

    modelCar = prv_createModelCar();
    descriptionCar = new CDescriptionModel3d(&modelCar);

    display3d = new CDisplay3D(worldEscalextric, &light, &positionCamera);

    display3d->appendDescription(SYMBOL_CIRCUIT, &descriptionCircuit);
    display3d->appendDescription(SYMBOL_CAR, &descriptionCar);

    return display3d;
}
