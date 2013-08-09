/* -----------------------------------------------------------------------------
 *  CDisplaySimulationRobot.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CDisplaySimulationRobot.inl"

#include "CRobotSprites.inl"
#include "CConstants.inl"
#include "CDisplayExecuteRobot.inl"
#include "CWorldSimulationRobot.inl"
#include "CTransformRobot2D.inl"

#include "asrtbas.h"
#include "CDisplayMultiSprite.hpp"
#include "CDisplaySprite.hpp"
#include "CDisplay3D.hpp"
#include "CLight.hpp"
#include "CPositionCamera.hpp"
#include "CDescriptionModel3d.hpp"
#include "CColor.hpp"
#include "CDescriptionSpriteOneImage.hpp"
#include "CDescriptionSpriteRotated.hpp"
#include "CDescriptionSpriteWithDataFigure.hpp"

const char *CDisplaySimulationRobot::SYMBOL_ROBOT = "Robot";
const char *CDisplaySimulationRobot::SYMBOL_SCENARIO = "Sceneario";
const char *CDisplaySimulationRobot::SYMBOL_MEMORY = "Memory";

static const unsigned long PRV_SIZE_SCREEN_X = 600;
static const unsigned long PRV_SIZE_SCREEN_Y = 600;

static const unsigned long PRV_SIZE_STATUS_X = 64;
static const unsigned long PRV_SIZE_STATUS_Y = 64;

static const long PRV_POSX_MARK = 77;
static const long PRV_POSY_MARK = 45;

static const long PRV_POSX_MEMORY = 130;
static const long PRV_POSY_MEMORY = 46;

//-----------------------------------------------------------------------

static class IDescription *prv_createDescriptionRobot(void)
{
    class CArray<CImg> *imagesRobot;
    class CDescriptionSpriteRotated *spritesRobot;

    imagesRobot = CRobotSprites::createImagesRobot();
    spritesRobot = new CDescriptionSpriteRotated(&imagesRobot);

    return spritesRobot;
}

//-----------------------------------------------------------------------

static class CDisplaySprite *prv_displayMap(class IWorld *world)
{
    class CDisplaySprite *displaySprite;
    class CImg *imageScenario;
    class IDescription *descriptionScenario, *descriptionRobot;
    bool originCartesian;

    originCartesian = false;
    displaySprite = new CDisplaySprite(originCartesian, 32, 32, PRV_SIZE_SCREEN_X, PRV_SIZE_SCREEN_Y, world);

    imageScenario = CRobotSprites::createImageMap();
    descriptionScenario = new CDescriptionSpriteOneImage(&imageScenario);
    displaySprite->appendDescription(CDisplaySimulationRobot::SYMBOL_SCENARIO, &descriptionScenario);

    descriptionRobot = prv_createDescriptionRobot();
    displaySprite->appendDescription(CDisplaySimulationRobot::SYMBOL_ROBOT, &descriptionRobot);

    return displaySprite;
}

//-----------------------------------------------------------------------

static void i_appendMark(class CDisplaySprite *displaySprite, const char *mark, class CImg **imageMark)
{
    class IDescription *descriptionMark;

    descriptionMark = new CDescriptionSpriteOneImage(PRV_POSX_MARK, PRV_POSY_MARK, imageMark);
    displaySprite->appendDescription(mark, &descriptionMark);
}

//-----------------------------------------------------------------------

static class CDisplaySprite *prv_displayStatus(void)
{
    class CDisplaySprite *displaySprite;
    class CImg *imageMark, *imageBackgroundStatus;
    class IDescription *descriptionMark;

    imageBackgroundStatus = CRobotSprites::backgroundStatus();

    displaySprite = new CDisplaySprite(&imageBackgroundStatus, NULL);

    imageMark = CRobotSprites::imageMark1();
    i_appendMark(displaySprite, CConstants::MARK_OBJECTIVE1, &imageMark);

    imageMark = CRobotSprites::imageMark2();
    i_appendMark(displaySprite, CConstants::MARK_OBJECTIVE2, &imageMark);

    imageMark = CRobotSprites::imageMark3();
    i_appendMark(displaySprite, CConstants::MARK_OBJECTIVE3, &imageMark);

    imageMark = CRobotSprites::imageMark4();
    i_appendMark(displaySprite, CConstants::MARK_OBJECTIVE4, &imageMark);

    imageMark = CRobotSprites::imageMarkEmpty();
    i_appendMark(displaySprite, CConstants::MARK_EMPTY, &imageMark);

    descriptionMark = new CDescriptionSpriteWithDataFigure(PRV_POSX_MEMORY, PRV_POSY_MEMORY);
    displaySprite->appendDescription(CDisplaySimulationRobot::SYMBOL_MEMORY, &descriptionMark);

    return displaySprite;
}

//-----------------------------------------------------------------------

static class IDisplay *prv_displayExecuteRobot(class CWorldSimulationRobot *world)
{
    class CDisplayExecuteRobot *displayExecuteRobot;
    class IDescription *descriptionRobot;

    displayExecuteRobot = new CDisplayExecuteRobot(world);

    descriptionRobot = new CDescriptionExecuteRobot();
    displayExecuteRobot->appendDescription(CDisplaySimulationRobot::SYMBOL_ROBOT, &descriptionRobot);

    return displayExecuteRobot;
}

//-----------------------------------------------------------------------

class IDisplay *CDisplaySimulationRobot::createDisplaySprite(class CWorldSimulationRobot *world)
{
    class CDisplayMultiSprite *displayMain;
    class IDisplay *displayMap, *displayStatus, *displayExecute;

    displayExecute = prv_displayExecuteRobot(world);
    displayMap = prv_displayMap(world);
    displayStatus = prv_displayStatus();

    displayMain = new CDisplayMultiSprite(CDisplayMultiSprite::HORIZONTAL);

    displayMain->appendDisplaySprite(&displayExecute);
    displayMain->appendDisplaySprite(&displayMap);
    displayMain->appendDisplaySprite(&displayStatus);

    return displayMain;
}

//-----------------------------------------------------------------------

static class CLight *prv_createLight(void)
{
    class CColor *colorAmbient, *colorDiffuse, *colorSpecular;
    double xPosLight, yPosLight, zPosLight;

    xPosLight = 7.;
    yPosLight = 7.;
    zPosLight = 30.;

    colorAmbient = new CColor(0.4, 0.4, 0.4);
    colorDiffuse = new CColor(0.7, 0.7, 0.7);
    colorSpecular = new CColor(0.8, 0.6, 0.6);

    return new CLight(&colorAmbient, &colorDiffuse, &colorSpecular, xPosLight, yPosLight, zPosLight);
}

//-----------------------------------------------------------------------

static class CPositionCamera *prv_createPositionCameraDefault(void)
{
    double eyeX, eyeY, eyeZ;
    double pointReferenceX, pointReferenceY, pointReferenceZ;
    double upX, upY, upZ;

    eyeX = 0.;
    eyeY = 0.;
    eyeZ = 30.;

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

static class IDisplay *prv_createDisplayGL(class CWorldSimulationRobot *world)
{
    class CDisplay3D *display3d;
    class CLight *light;
    class CPositionCamera *positionCamera;
    class IDescription *descriptionScenario, *descriptionRobot;
    class CModel3d *modelScenario, *modelRobot;

    assert_no_null(world);

    light = prv_createLight();
    positionCamera = prv_createPositionCameraDefault();

    modelScenario = CRobotSprites::createModelScenario();
    descriptionScenario = new CDescriptionModel3d(&modelScenario);

    modelRobot = CRobotSprites::createModelRobot();
    descriptionRobot = new CDescriptionModel3d(&modelRobot);

    display3d = new CDisplay3D(world, &light, &positionCamera);

    display3d->appendDescription(CDisplaySimulationRobot::SYMBOL_ROBOT, &descriptionRobot);
    display3d->appendDescription(CDisplaySimulationRobot::SYMBOL_SCENARIO, &descriptionScenario);

    return display3d;
}

//-----------------------------------------------------------------------

class IDisplay *CDisplaySimulationRobot::createDisplayGL(class CWorldSimulationRobot *world)
{
    class CDisplayMultiSprite *displayMain;
    class IDisplay *displayGL, *displayExecute;

    displayExecute = prv_displayExecuteRobot(world);
    displayGL = prv_createDisplayGL(world);

    displayMain = new CDisplayMultiSprite(CDisplayMultiSprite::HORIZONTAL);

    displayMain->appendDisplaySprite(&displayExecute);
    displayMain->appendDisplaySprite(&displayGL);

    return displayMain;
}
