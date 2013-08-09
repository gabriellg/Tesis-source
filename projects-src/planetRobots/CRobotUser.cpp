//CRobotUser.hpp: Robot is moved by user using keyboard. It is defined as a sprite.

#include "CRobotUser.inl"

#include "asrtbas.h"
#include "memory.h"

#include "IDisplay.hpp"
#include "CFigure.hpp"
#include "CString.hpp"
#include "CProcessEventSystem.hpp"
#include "CEventTraslate.hpp"
#include "CEventDesire.hpp"
#include "CEventCollision.hpp"
#include "CTypeDescription.hpp"
#include "CArray.hpp"
#include "CDescriptionSpriteOneImage.hpp"
#include "CTypeDescrPhysics.hpp"
#include "CGeneratorCollisionSprite.hpp"
#include "CCollectionEventsSystem.hpp"

#include "CImg.hpp"
#include "CRobotUser__sprite1.iig"

struct SPrvDataPrivateRobotUser
{
	char *idDesire;
};

static const char *PRV_ROBOTUSER = "RobotUser";
static const char *PRV_NO_DESIRE = "NO_DESIRE";

//-----------------------------------------------------------------------

static struct SPrvDataPrivateRobotUser *prv_create(const char *idDesire)
{
	struct SPrvDataPrivateRobotUser *dataPrivate;

	dataPrivate = MALLOC(struct SPrvDataPrivateRobotUser);

	dataPrivate->idDesire = CString::copy(idDesire);

	return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroy(struct SPrvDataPrivateRobotUser **dataPrivate)
{
	assert_no_null(dataPrivate);
	assert_no_null(*dataPrivate);

	CString::free(&(*dataPrivate)->idDesire);

	FREE_T(dataPrivate, struct SPrvDataPrivateRobotUser);
}

//-----------------------------------------------------------------------

CRobotUser::CRobotUser()
{
	m_dataPrivate = prv_create(PRV_NO_DESIRE);
}

//-----------------------------------------------------------------------

CRobotUser::~CRobotUser()
{
	prv_destroy(&m_dataPrivate);
}

//-----------------------------------------------------------------------

class CAgent *CRobotUser::evolution(class CCollectionEventsSystem *allEvents)
{
    class CEventSystem *evtSystem;

    assert_no_null(m_dataPrivate);

    if (allEvents->existEventSystem(CEventTraslate::ID_EVENT, &evtSystem) == true)
    {
        class CEventTraslate *evtTraslate;

        evtTraslate = dynamic_cast<class CEventTraslate *>(evtSystem);
        assert_no_null(evtTraslate);

        m_dataPrivate->idDesire = evtTraslate->applyTraslate(
                            CEventDesire::DESIRE_GO_RIGHT, CEventDesire::DESIRE_GO_LEFT,
                            CEventDesire::DESIRE_GO_FORWARD, CEventDesire::DESIRE_GO_BACK);
    }

    if (allEvents->existEventSystem(CEventDesire::ID_EVENT, &evtSystem) == true)
    {
        class CEventDesire *evtDesire;

        evtDesire = dynamic_cast<class CEventDesire *>(evtSystem);
        assert_no_null(evtDesire);
        evtDesire->setDesire(PRV_ROBOTUSER, m_dataPrivate->idDesire);
    }

    if (allEvents->existEventSystem(CEventCollision::ID_EVENT, &evtSystem) == true)
    {
        class CEventCollision *evtCollision;
        bool withCollision;

        //TODO: Ahora no hacemos nada. Solo verificar que hay una colision.
        evtCollision = dynamic_cast<class CEventCollision *>(evtSystem);
        withCollision = false;

        if (evtCollision->hasCollisionBackground(PRV_ROBOTUSER) == true)
            withCollision = true;

        if (evtCollision->hasCollisionWithElement(PRV_ROBOTUSER, NULL) == true)
            withCollision = true;

        if (withCollision == true)
            ;
    }

    return this;
}

//-----------------------------------------------------------------------

class CAgent *CRobotUser::representation(class CTypeDescription *evtDraw)
{
    class CAgent *figureRobot;
	class CTypeDescrPhysics *evtDrawMoveSprite;

	assert_no_null(m_dataPrivate);

    evtDrawMoveSprite = dynamic_cast<class CTypeDescrPhysics *>(evtDraw);
    assert_no_null(evtDrawMoveSprite);

	if (CEventDesire::isDesire(m_dataPrivate->idDesire) == true)
	{
		CString::free(&m_dataPrivate->idDesire);
		m_dataPrivate->idDesire = CString::copy(PRV_NO_DESIRE);
	}

    figureRobot = new CFigure(PRV_ROBOTUSER);
    return evtDrawMoveSprite->move(PRV_ROBOTUSER, &figureRobot);
}

//-----------------------------------------------------------------------

const char *CRobotUser::getIdRobot(void)
{
	return PRV_ROBOTUSER;
}

//-----------------------------------------------------------------------

void CRobotUser::appendRobotToCollision(class CGeneratorCollisionSprite *generatorCollision)
{
    class CImg *imageRobot;

    assert_no_null(generatorCollision);

    imageRobot = cimg_load_local_png(sprite1);
    generatorCollision->appendSymbolFigureToConsider(PRV_ROBOTUSER, &imageRobot);
}

//-----------------------------------------------------------------------

void CRobotUser::appendSymbolRobot(class IDisplay *display)
{
    class CImg *imageRobot;
    class IDescription *description;

    imageRobot = cimg_load_local_png(sprite1);
    description = new CDescriptionSpriteOneImage(&imageRobot);
    display->appendDescription(PRV_ROBOTUSER, &description);
}
