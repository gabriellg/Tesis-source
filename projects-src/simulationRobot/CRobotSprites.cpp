//CPlanetFigure.cpp

#include "CRobotSprites.inl"

#include "CConstants.inl"

#include "IEvaluateMap.hpp"
#include "CWorldSprite.hpp"

#include "asrtbas.h"
#include "CString.hpp"
#include "IEvaluateMap.hpp"
#include "CArray.hpp"

#include "CImg.hpp"
#include "CModel3d.hpp"
#include "CMesh.hpp"
#include "CScaleColorWithHeight.hpp"
#include "CMaterial.hpp"
#include "CGeneratorModel.hpp"
#include "CGeneratorSolid.hpp"
#include "CReaderModel3d.hpp"
#include "CStackTransformation.hpp"

#include "CRobotSprites__grid1.iig"
#include "CRobotSprites__grid4.iig"
#include "CRobotSprites__map.iig"
#include "CRobotSprites__floor.iig"
#include "CRobotSprites__mark1.iig"
#include "CRobotSprites__mark2.iig"
#include "CRobotSprites__mark3.iig"
#include "CRobotSprites__mark4.iig"
#include "CRobotSprites__empty.iig"

#include "CRobotSprites__backgroundStatus.iig"

#include "CRobotSprites__robotEast.iig"
#include "CRobotSprites__robotWest.iig"
#include "CRobotSprites__robotNorth.iig"
#include "CRobotSprites__robotSud.iig"

static const double prv_SIZE_CELL = 1.;

class CPrvEvaluateMap : public IEvaluateMap
{
    public:

        virtual class CImg *evaluteImage(unsigned char r, unsigned char g, unsigned char b) const;
        virtual enum ETypeObstable evaluteObstable(unsigned char r, unsigned char g, unsigned char b) const;
        virtual bool isPositionCreateAgent(unsigned char r, unsigned char g, unsigned char b, char **typePosition) const;
};

//-----------------------------------------------------------------------

static bool prv_isColorObstacle(unsigned char r, unsigned char g, unsigned char b)
{
    if (r == 0 && g == 0 && b == 0)
        return true;
    else if (r == 255 && g == 255 && b == 0)
        return true;
    else
        return false;
}

//-----------------------------------------------------------------------

enum IEvaluateMap::ETypeObstable CPrvEvaluateMap::evaluteObstable(unsigned char r, unsigned char g, unsigned char b) const
{
    if (prv_isColorObstacle(r, g, b) == true)
        return IEvaluateMap::OBSTACLE;
    else
        return IEvaluateMap::FREE;
}

//-----------------------------------------------------------------------

static bool prv_isMark1(unsigned char r, unsigned char g, unsigned char b)
{
    if (r == 255 && g == 0 && b == 0)
        return true;
    else
        return false;
}

//-----------------------------------------------------------------------

static bool prv_isMark2(unsigned char r, unsigned char g, unsigned char b)
{
    if (r == 0 && g == 255 && b == 0)
        return true;
    else
        return false;
}

//-----------------------------------------------------------------------

static bool prv_isMark3(unsigned char r, unsigned char g, unsigned char b)
{
    if (r == 0 && g == 0 && b == 255)
        return true;
    else
        return false;
}

//-----------------------------------------------------------------------

static bool prv_isMark4(unsigned char r, unsigned char g, unsigned char b)
{
    if (r == 0 && g == 255 && b == 255)
        return true;
    else
        return false;
}

//-----------------------------------------------------------------------

struct CImg *CPrvEvaluateMap::evaluteImage(unsigned char r, unsigned char g, unsigned char b) const
{
    if (r == 0 && g == 0 && b == 0)
        return cimg_load_local_png(grid1);
    if (r == 255 && g == 255 && b == 0)
        return cimg_load_local_png(grid4);
    else if (r == 255 && g == 255 && b == 255)
        return cimg_load_local_png(floor);
    else if (prv_isMark1(r, g, b) == true)
        return cimg_load_local_png(mark1);
    else if (prv_isMark2(r, g, b) == true)
        return cimg_load_local_png(mark2);
    else if (prv_isMark3(r, g, b) == true)
        return cimg_load_local_png(mark3);
    else if (prv_isMark4(r, g, b) == true)
        return cimg_load_local_png(mark4);
    else
    {
        assert_message("Pixels without sprite");
        return NULL;
    }
}

//-----------------------------------------------------------------------

bool CPrvEvaluateMap::isPositionCreateAgent(unsigned char r, unsigned char g, unsigned char b, char **typePosition) const
{
    assert_message("Not implemented");
    return false;
}

//-----------------------------------------------------------------------

class CImg *CRobotSprites::createMap(void)
{
    return cimg_load_local_png(map);
}

//-----------------------------------------------------------------------

class CImg *CRobotSprites::createImageMap(void)
{
    class CImg *imageScenario;
    class CImg *imageMap;
    class CPrvEvaluateMap *evaluateMap;

    imageMap = cimg_load_local_png(map);
    evaluateMap = new CPrvEvaluateMap();

    imageScenario = CWorldSprite::imageScenarioPlane(imageMap, evaluateMap, 32, 32);

    delete evaluateMap;
    delete imageMap;

    return imageScenario;
}

//-----------------------------------------------------------------------

static class CScaleColorWithHeight *prv_createScaleColors(void)
{
    class CScaleColorWithHeight *colorsAndHeights;

    colorsAndHeights = new CScaleColorWithHeight(0.);

    colorsAndHeights->appendColorWithHeight(0, 0, 0, 2.);
    colorsAndHeights->appendColorWithHeight(255, 255, 0, 2.);

    return colorsAndHeights;
}

//-----------------------------------------------------------------------

static void prv_appendModelMark(double x, double y, class CModel3d *model)
{
    class CModel3d *modelMark;
    class CStackTransformation *transformation;

    transformation = new CStackTransformation();
    transformation->appendTranslation(x, y, 0.);

    modelMark = CReaderModel3d::readModel3d("./objetos3d/senyal.obj");
    modelMark->InsideCube(prv_SIZE_CELL, prv_SIZE_CELL, 1.5 * prv_SIZE_CELL);

    modelMark->applyTransformation(transformation);

    model->appendModel(modelMark);

    delete modelMark;
    delete transformation;
}

//-----------------------------------------------------------------------

class CModel3d *CRobotSprites::createModelScenario(void)
{
    class CModel3d *modelScenario;
    class CImg *imageMap, *imageTexture;
    class CMesh *meshScenario;
    class CScaleColorWithHeight *scaleColor;
    class CMaterial *material;
    double red, green, blue;

    imageMap = cimg_load_local_png(map);
    scaleColor = prv_createScaleColors();

    meshScenario = CGeneratorModel::createGridWithImage(imageMap, scaleColor, prv_SIZE_CELL, prv_SIZE_CELL);
    meshScenario->calculateCoordsTexturesInPlane(CMesh::TEXTURE_DECAL);

    modelScenario = new CModel3d;

    red = 1.;
    green = 224. / 255.;
    blue = 140. / 255.;

    imageTexture = new CImg("./img/metal.jpg");
    material = new CMaterial("Scenario", red, green, blue, 1., &imageTexture);

    modelScenario->appendMaterial(&material);
    modelScenario->appendMesh("Scenario", meshScenario);

    prv_appendModelMark(3., 4., modelScenario);
    prv_appendModelMark(10., 6., modelScenario);
    prv_appendModelMark(12., 13., modelScenario);
    prv_appendModelMark(5., 16., modelScenario);

    delete imageMap;
    delete scaleColor;
    delete meshScenario;

    return modelScenario;
}

//-----------------------------------------------------------------------

class CArray<CImg> *CRobotSprites::createImagesRobot(void)
{
    class CArray<CImg> *imagesRobot;
    class CImg *imageRobot;

    imagesRobot = new CArray<CImg>(4);

    imageRobot = cimg_load_local_png(robotEast);
    imagesRobot->set(0, imageRobot);

    imageRobot = cimg_load_local_png(robotNorth);
    imagesRobot->set(1, imageRobot);

    imageRobot = cimg_load_local_png(robotWest);
    imagesRobot->set(2, imageRobot);

    imageRobot = cimg_load_local_png(robotSud);
    imagesRobot->set(3, imageRobot);

    return imagesRobot;
}

//-----------------------------------------------------------------------

class CModel3d* CRobotSprites::createModelRobot(void)
{
    class CModel3d *modelRobot;

    modelRobot = CReaderModel3d::readModel3d("./objetos3d/robot.obj");

    modelRobot->InsideCube(prv_SIZE_CELL, prv_SIZE_CELL, 6. * prv_SIZE_CELL);

    return modelRobot;
}

//-----------------------------------------------------------------------

bool CRobotSprites::isObstable(class CImg *map, long x, long y)
{
    bool isPixelObstacle;
    unsigned char r, g, b;
    unsigned long width, height;

    map->size(&width, &height);

    if (x >= 0 && y >= 0 && x < (long)width && y < (long)height)
    {
        map->getPixelRGB(x, y, &r, &g, &b);
        isPixelObstacle = prv_isColorObstacle(r, g, b);
    }
    else
        isPixelObstacle = true;

    return isPixelObstacle;
}

//-----------------------------------------------------------------------

bool CRobotSprites::cellWithMark(class CImg *map, long x, long y, char **markObjective)
{
    bool isCellMark;
    unsigned long width, height;

    map->size(&width, &height);

    if (x >= 0 && y >= 0 && x < (long)width && y < (long)height)
    {
        unsigned char r, g, b;
        const char *mark;

        map->getPixelRGB(x, y, &r, &g, &b);

        if (prv_isMark1(r, g, b) == true)
            mark = CConstants::MARK_OBJECTIVE1;
        else if (prv_isMark2(r, g, b) == true)
            mark = CConstants::MARK_OBJECTIVE2;
        else if (prv_isMark3(r, g, b) == true)
            mark = CConstants::MARK_OBJECTIVE3;
        else if (prv_isMark4(r, g, b) == true)
            mark = CConstants::MARK_OBJECTIVE4;
        else
            mark = NULL;

        if (mark != NULL)
        {
            *markObjective = CString::copy(mark);
            isCellMark = true;
        }
        else
            isCellMark = false;
    }
    else
        isCellMark = false;

    return isCellMark;
}

//-----------------------------------------------------------------------

class CImg *CRobotSprites::imageMark1(void)
{
    return cimg_load_local_png(mark1);
}

//-----------------------------------------------------------------------

class CImg *CRobotSprites::imageMark2(void)
{
    return cimg_load_local_png(mark2);
}

//-----------------------------------------------------------------------

class CImg *CRobotSprites::imageMark3(void)
{
    return cimg_load_local_png(mark3);
}

//-----------------------------------------------------------------------

class CImg *CRobotSprites::imageMark4(void)
{
    return cimg_load_local_png(mark4);
}

//-----------------------------------------------------------------------

class CImg *CRobotSprites::imageMarkEmpty(void)
{
    return cimg_load_local_png(empty);
}

//-----------------------------------------------------------------------

class CImg *CRobotSprites::backgroundStatus(void)
{
    return cimg_load_local_png(backgroundStatus);
}
