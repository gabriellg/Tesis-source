//CPlanetFigure.cpp

#include "CPlanetFigure.inl"

#include "IEvaluateMap.hpp"
#include "CWorldSprite.hpp"
#include "ICreatorActors.hpp"
#include "CRobotUser.inl"

#include "asrtbas.h"
#include "identif.h"

#include "CString.hpp"

#include "CImg.hpp"
#include "CPlanetFigure__grid1.iig"
#include "CPlanetFigure__grid2.iig"
#include "CPlanetFigure__grid3.iig"
#include "CPlanetFigure__grid4.iig"
#include "CPlanetFigure__grid5.iig"
#include "CPlanetFigure__grid6.iig"
#include "CPlanetFigure__grid7.iig"
#include "CPlanetFigure__grid8.iig"
#include "CPlanetFigure__grid9.iig"
#include "CPlanetFigure__grid10.iig"
#include "CPlanetFigure__grid11.iig"

#include "CPlanetFigure__scenario.iig"

static const char *PRV_TYPE_ROBOT = "ROBOT";

class CPrvEvaluatePlanet : public IEvaluateMap
{
	public:

		virtual class CImg *evaluteImage(unsigned char r, unsigned char g, unsigned char b) const;
		virtual enum ETypeObstable evaluteObstable(unsigned char r, unsigned char g, unsigned char b) const;
		virtual bool isPositionCreateAgent(unsigned char r, unsigned char g, unsigned char b, char **typePosition) const;
};

class CPrvCreateRobot : public ICreatorActors
{
	public:

		CPrvCreateRobot();

		virtual bool isGoingToCreate(const char *typeActor);
		virtual bool createActor(const char *typeActor, char **idActor, class CAgent **actorCreate);

	private:

		bool m_isCreate;
};

//-----------------------------------------------------------------------

CPrvCreateRobot::CPrvCreateRobot()
{
	m_isCreate = false;
}

//-----------------------------------------------------------------------

bool CPrvCreateRobot::isGoingToCreate(const char *typeActor)
{
	if (m_isCreate == true)
		return false;
	else
		return true;
}

//-----------------------------------------------------------------------

bool CPrvCreateRobot::createActor(const char *typeActor, char **idActor, class CAgent **actorCreate)
{
	class CRobotUser *robot;

	assert_no_null(idActor);
	assert_no_null(actorCreate);
	assert(CString::equal(typeActor, PRV_TYPE_ROBOT) == true);

	robot = new CRobotUser();

	*actorCreate = robot;
	*idActor = CString::copy(robot->getIdRobot());

	m_isCreate = true;

	return true;
}

//-----------------------------------------------------------------------

enum IEvaluateMap::ETypeObstable CPrvEvaluatePlanet::evaluteObstable(unsigned char r, unsigned char g, unsigned char b) const
{
	if (r == 0 && g == 0 && b == 128)
		return IEvaluateMap::OBSTACLE;
	else
		return IEvaluateMap::FREE;
}

//-----------------------------------------------------------------------

struct CImg *CPrvEvaluatePlanet::evaluteImage(unsigned char r, unsigned char g, unsigned char b) const
{
	if (r == 128 && g == 0 && b == 0)
		return cimg_load_local_png(grid1);
	else if (r == 0 && g == 128 && b == 0)
		return cimg_load_local_png(grid2);
	else if (r == 0 && g == 0 && b == 128)
		return cimg_load_local_png(grid3);
	else if (r == 128 && g == 128 && b == 128)
		return cimg_load_local_png(grid4);
	else if (r == 128 && g == 128 && b == 0)
		return cimg_load_local_png(grid5);
	else if (r == 255 && g == 0 && b == 0)
		return cimg_load_local_png(grid6);
	else if (r == 255 && g == 1 && b == 1)
		return cimg_load_local_png(grid6);
	else if (r == 0 && g == 255 && b == 0)
		return cimg_load_local_png(grid7);
	else if (r == 0 && g == 0 && b == 255)
		return cimg_load_local_png(grid8);
	else if (r == 255 && g == 255 && b == 0)
		return cimg_load_local_png(grid9);
	else if (r == 0 && g == 255 && b == 255)
		return cimg_load_local_png(grid10);
	else if (r == 255 && g == 255 && b == 255)
		return cimg_load_local_png(grid11);
	else
	{
		assert_error();
		return NULL;
	}
}

//-----------------------------------------------------------------------

bool CPrvEvaluatePlanet::isPositionCreateAgent(unsigned char r, unsigned char g, unsigned char b, char **typePosition) const
{
	assert_no_null(typePosition);

	if (r == 255 && g == 1 && b == 1)
	{
		*typePosition = CString::copy(PRV_TYPE_ROBOT);
		return true;
	}
	else
		return false;
}

//-----------------------------------------------------------------------
class CWorldGeometry2DPixels *CPlanetFigure::createWorld(void)
{
    class CWorldGeometry2DPixels *world;
    class CImg *imageMap;
    class CPrvEvaluatePlanet *evaluatePlanet;

    imageMap = cimg_load_local_png(scenario);
    evaluatePlanet = new CPrvEvaluatePlanet();

    world = CWorldSprite::createWorld2d(imageMap, evaluatePlanet, 32, 32);

    delete evaluatePlanet;
    delete imageMap;

    return world;
}

//-----------------------------------------------------------------------
class CCollectionPositionCreation *CPlanetFigure::collectionPositionCreationPlane(void)
{
    class CCollectionPositionCreation *collectionPosicionCreation;
    class CImg *imageMap;
    class CPrvEvaluatePlanet *evaluatePlanet;

    imageMap = cimg_load_local_png(scenario);
    evaluatePlanet = new CPrvEvaluatePlanet();

    collectionPosicionCreation = CWorldSprite::collectionPositionCreationPlane(imageMap, evaluatePlanet, 32, 32);

    delete evaluatePlanet;
    delete imageMap;

    return collectionPosicionCreation;
}

//-----------------------------------------------------------------------
class CImg *CPlanetFigure::createImageScenario(void)
{
    class CImg *imageScenario;
    class CImg *imageMap;
    class CPrvEvaluatePlanet *evaluatePlanet;

    imageMap = cimg_load_local_png(scenario);
    evaluatePlanet = new CPrvEvaluatePlanet();

    imageScenario = CWorldSprite::imageScenarioPlane(imageMap, evaluatePlanet, 32, 32);

    delete evaluatePlanet;
    delete imageMap;

    return imageScenario;
}

//-----------------------------------------------------------------------

class ICreatorActors *CPlanetFigure::createActors(void)
{
    return new CPrvCreateRobot();
}
