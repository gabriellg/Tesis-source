/* -----------------------------------------------------------------------------
 *  CDisplayEscalextric.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CDisplayEscalextric.inl"

#include "CWorldEscalextric.inl"
#include "CDataCircuit.inl"

#include "asrtbas.h"
#include "memory.h"

#include "CDisplaySprite.hpp"
#include "CImg.hpp"
#include "CPolylines.hpp"
#include "CDescriptionSpriteOneImage.hpp"
#include "CReaderModel3d.hpp"
#include "CArrPoint2d.hpp"
#include "CArrPoint3d.hpp"
#include "CVector.hpp"

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
#include "CLight3d.hpp"

const char *CDisplayEscalextric::SYMBOL_CIRCUIT = "Circuit";
const char *CDisplayEscalextric::SYMBOL_CAR = "Car";
const char *CDisplayEscalextric::SYMBOL_WORLD = "World";
const char *CDisplayEscalextric::SYMBOL_LIGHT_AMBIENT = "LightAmbient";

//-----------------------------------------------------------------------
//
class ITraslatorDisplay *CDisplayEscalextric::createDisplaySprite(const class CDataCircuit *dataCircuit,
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
    displaySprite = new CDisplaySprite(originCartesian, sizeXTrack, sizeYTrack, sizeXCircuit, sizeYCircuit,
            worldEscalextric);

    displaySprite->appendDescription(SYMBOL_CIRCUIT, &descriptionCircuit);
    displaySprite->appendDescription(SYMBOL_CAR, &descriptionCar);

    return displaySprite;
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
//
static class CMesh *prv_SizeWorld(double sizeFloor,
        double dx, double dy, double dz,
        double angleDegrees, double Ux, double Uy, double Uz,
        enum CMesh::ETypeTexture typeTexture)
{
    class CMesh *mesh;
    class CArrPoint2d *edge;
    class CArrPoint3d *edge3d;
    class CStackTransformation *stack;
    double Nx, Ny, Nz;
    double x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4;

    edge = CPolylines::createRectangleCentredInOrigin(sizeFloor, sizeFloor);

    stack = new CStackTransformation();
    stack->appendTranslation(dx, dy, dz);
    stack->appendRotation(angleDegrees, Ux, Uy, Uz);
    stack->getNormal(&Nx, &Ny, &Nz);

    edge3d = stack->transformationPoints2d(edge);
    edge3d->get(0, &x1, &y1, &z1);
    edge3d->get(1, &x2, &y2, &z2);
    edge3d->get(2, &x3, &y3, &z3);
    edge3d->get(3, &x4, &y4, &z4);

    mesh = new CMesh();
    mesh->appendQuad(x1, y1, z1, Nx, Ny, Nz, x2, y2, z2, Nx, Ny, Nz, x3, y3, z3, Nx, Ny, Nz, x4, y4, z4, Nx, Ny, Nz);
    mesh->calculateCoordsTexturesInPlane(typeTexture);

    DELETE_OBJECT(&edge, class CArrPoint2d);
    DELETE_OBJECT(&edge3d, class CArrPoint3d);

    return mesh;
}

//-----------------------------------------------------------------------
//
static void prv_appendMeshToModel(const char *nameMaterial, const char *fileImage, class CMesh **mesh,
        class CModel3d *model)
{
    class CImg *image;
    class CMaterial *material;

    assert_no_null(mesh);

    image = new CImg(fileImage);
    material = new CMaterial(nameMaterial, 1., 1., 1., 1., &image);
    material->setPriorityZFighting(CMaterial::LOW);
    model->appendMaterial(&material);
    model->appendMesh(nameMaterial, *mesh);

    DELETE_OBJECT(mesh, class CMesh);
}

//-----------------------------------------------------------------------
//
static struct CModel3d *prv_createModelWorld(void)
{
    const char *PRV_MATERIAL_FLOOR = "Floor";
    const char *PRV_MATERIAL_CEIL = "Ceil";
    const char *PRV_MATERIAL_CEIL_XNEG = "Ceil_X-";
    const char *PRV_MATERIAL_CEIL_XPOS = "Ceil_X+";
    const char *PRV_MATERIAL_CEIL_YNEG = "Ceil_Y-";
    const char *PRV_MATERIAL_CEIL_YPOS = "Ceil_Y+";
    class CModel3d *model;
    class CMesh *meshFloor, *meshCeil, *meshXNeg, *meshXPos, *meshYNeg, *meshYPos;
    const double PRV_SIZE_WORLD = 150.;

    meshFloor = prv_SizeWorld(
            PRV_SIZE_WORLD,
            0., 0., 0., //dx,dy,dz
            0., 0., 1., 0.,//angleDegrees, Ux, Uy, Uz,
            CMesh::TEXTURE_REPEAT);

    meshCeil = prv_SizeWorld(
            PRV_SIZE_WORLD,
            0., 0., 0.5 * PRV_SIZE_WORLD, //dx,dy,dz
            180., 0., 1., 0.,//angleDegrees, Ux, Uy, Uz,
            CMesh::TEXTURE_DECAL);

    meshXNeg = prv_SizeWorld(
            PRV_SIZE_WORLD,
            -0.5 * PRV_SIZE_WORLD, 0., 0., //dx,dy,dz
            90., 0., 1., 0.,//angleDegrees, Ux, Uy, Uz,
            CMesh::TEXTURE_DECAL);

    meshXPos = prv_SizeWorld(
            PRV_SIZE_WORLD,
            0.5 * PRV_SIZE_WORLD, 0., 0., //dx,dy,dz
            -90., 0., 1., 0.,//angleDegrees, Ux, Uy, Uz,
            CMesh::TEXTURE_DECAL);

    meshYNeg = prv_SizeWorld(
            PRV_SIZE_WORLD,
            0., 0.5 * PRV_SIZE_WORLD, 0., //dx,dy,dz
            90., 1., 0., 0.,//angleDegrees, Ux, Uy, Uz,
            CMesh::TEXTURE_DECAL);

    meshYPos = prv_SizeWorld(
            PRV_SIZE_WORLD,
            0., -0.5 * PRV_SIZE_WORLD, 0., //dx,dy,dz
            -90., 1., 0., 0.,//angleDegrees, Ux, Uy, Uz,
            CMesh::TEXTURE_DECAL);

    model = new CModel3d;
    model->setIsInLimits(false);

    prv_appendMeshToModel(PRV_MATERIAL_FLOOR, "./imagesCircuit/grass.jpg", &meshFloor, model);
    prv_appendMeshToModel(PRV_MATERIAL_CEIL, "./imagesCircuit/ceil.jpg", &meshCeil, model);
    prv_appendMeshToModel(PRV_MATERIAL_CEIL_XNEG, "./imagesCircuit/sky-xneg.jpg", &meshXNeg, model);
    prv_appendMeshToModel(PRV_MATERIAL_CEIL_XPOS, "./imagesCircuit/sky-xpos.jpg", &meshXPos, model);
    prv_appendMeshToModel(PRV_MATERIAL_CEIL_YNEG, "./imagesCircuit/sky-yneg.jpg", &meshYNeg, model);
    prv_appendMeshToModel(PRV_MATERIAL_CEIL_YPOS, "./imagesCircuit/sky-ypos.jpg", &meshYPos, model);

    return model;
}

//-----------------------------------------------------------------------
//
class ITraslatorDisplay *CDisplayEscalextric::createDisplay3D(const class CDataCircuit *dataCircuit,
        class CWorldEscalextric *worldEscalextric)
{
    class CDisplay3D *display3d;
    class CLight *light;
    class IDescription *descriptionCircuit, *descriptionCar, *descriptionSky, *descriptionLight;
    class CModel3d *modelCircuit, *modelCar, *modelWorld;

    assert_no_null(dataCircuit);
    assert_no_null(worldEscalextric);

    worldEscalextric->setWorld3d();

    light = prv_createLight();

    modelCircuit = dataCircuit->model3d();
    descriptionCircuit = new CDescriptionModel3d(&modelCircuit);

    modelCar = prv_createModelCar();
    descriptionCar = new CDescriptionModel3d(&modelCar);

    modelWorld = prv_createModelWorld();
    descriptionSky = new CDescriptionModel3d(&modelWorld);

    display3d = new CDisplay3D(worldEscalextric);

    descriptionLight = new CLight3d(&light);

    display3d->appendDescription(SYMBOL_CIRCUIT, &descriptionCircuit);
    display3d->appendDescription(SYMBOL_CAR, &descriptionCar);
    display3d->appendDescription(SYMBOL_WORLD, &descriptionSky);
    display3d->appendDescription(SYMBOL_LIGHT_AMBIENT, &descriptionLight);

    return display3d;
}
