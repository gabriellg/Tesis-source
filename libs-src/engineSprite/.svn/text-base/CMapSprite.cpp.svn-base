//CMapSprite.cpp

#include "CMapSprite.hpp"

#include "ICreatorActors.hpp"
#include "CEventCreateActor.inl"
#include "CTypeDescLimitsSprite.inl"
#include "CCollectionPositionCreation.hpp"

#include "asrtbas.h"
#include "memory.h"
#include "CString.hpp"

#include "CCollectionEventsSystem.hpp"
#include "CFigure.hpp"
#include "CMultiObject3d.hpp"

//-----------------------------------------------------------------------

CMapSprite::CMapSprite(const char *idFigureScenario)
{
	m_dataPrivate = (struct prv_dataPrivateMapSprite_t *)CString::copy(idFigureScenario);
}

//-----------------------------------------------------------------------

CMapSprite::~CMapSprite()
{
    char *idFigure;

    idFigure = (char *)m_dataPrivate;
    CString::free(&idFigure);
}

//---------------------------------------------------------------

class CAgent *CMapSprite::evolution(class CCollectionEventsSystem *allEvents)
{
    class CAgent *agentNextGeneration;
    class CEventSystem *evtSystem;

    if (allEvents->existEventSystem(CEventCreateActor::ID_EVENT, &evtSystem) == true)
    {
        class CEventCreateActor *evtCreatorActor;
        class CAgent *actorGenerated;

        evtCreatorActor = dynamic_cast<class CEventCreateActor *>(evtSystem);
        assert_no_null(evtCreatorActor);

        if (evtCreatorActor->createActor(&actorGenerated) == true)
        {
            class CMultiObject3d *multi3d;
            class CAgent *thisAgent;

            multi3d = new CMultiObject3d();

            thisAgent = this;

            multi3d->appendChild(&thisAgent);
            multi3d->appendChild(&actorGenerated);

            agentNextGeneration = multi3d;
        }
        else
            agentNextGeneration = this;
    }
    else
        agentNextGeneration = this;

    return agentNextGeneration;
}

//-----------------------------------------------------------------------

class CAgent *CMapSprite::representation(class CTypeDescription *evtDraw)
{
    const char *idFigureScenario;

    idFigureScenario = (const char *)m_dataPrivate;
	return new CFigure(idFigureScenario);
}
