/* -----------------------------------------------------------------------------
 *  CDisplay3dMetereo.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CDisplay3DMetereo.inl"

#include "CMetereoUtils.inl"
#include "CDescriptionsGL.inl"

#include "asrtbas.h"

#include "CDisplay3D.hpp"
#include "CPositionCamera.hpp"
#include "CColor.hpp"
#include "CLight.hpp"
#include "CGraphicsRect.hpp"

const char *CDisplay3DMetereo::SYMBOL_FORREST = "forrest";
const char *CDisplay3DMetereo::SYMBOL_TREE = "Tree";
const char *CDisplay3DMetereo::SYMBOL_LIGHTING ="Lightning";

//-----------------------------------------------------------------------

static class CPositionCamera *prv_createPositionCameraDefault(void)
{
    double eyeX, eyeY, eyeZ;
    double pointReferenceX, pointReferenceY, pointReferenceZ;
    double upX, upY, upZ;

    eyeX = 0.;
    eyeY = 0.;
    eyeZ = 60.;

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

class IDisplay *CDisplay3DMetereo::createDisplay3d(unsigned long numRows, unsigned long numCols)
{
    class CDisplay3D *display3D;
    class CLight *light;
    class CPositionCamera *positionCamera;
    class IDescription *descriptionSymbol;
    double sizeXTree, sizeYTree;

    CMetereoUtils::sizeTree(&sizeXTree, &sizeYTree);

    light = prv_createLight();
    positionCamera = prv_createPositionCameraDefault();

    display3D = new CDisplay3D(NULL, &light, &positionCamera);

    descriptionSymbol = new CDescriptionForrestGL(numRows * sizeXTree, numCols * sizeYTree);
    display3D->appendDescription(SYMBOL_FORREST, &descriptionSymbol);

    descriptionSymbol = new CDescriptionTreeGL();
    display3D->appendDescription(SYMBOL_TREE, &descriptionSymbol);

    descriptionSymbol = new CDescriptionCylinderLightningGL();
    display3D->appendDescription(SYMBOL_LIGHTING, &descriptionSymbol);

    return display3D;
}

//-----------------------------------------------------------------------

void CDisplay3DMetereo::defineLayers(class IGraphics *graphics)
{
    CDescriptionForrestGL::defineLayer(graphics);
    CDescriptionTreeGL::defineLayer(graphics);
    CDescriptionCylinderLightningGL::defineLayer(graphics);
}
