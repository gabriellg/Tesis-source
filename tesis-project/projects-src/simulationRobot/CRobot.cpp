/* -----------------------------------------------------------------------------
 *  CRobot.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CRobot.inl"

#include "CDisplaySimulationRobot.inl"
#include "CMemoryRobot.inl"
#include "CEventSimulationRobot.inl"
#include "CConstants.inl"
#include "CTypeDescriptionExecuteRobot.inl"

#include "asrtbas.h"
#include "memory.h"

#include "CMultiObject3d.hpp"
#include "CString.hpp"
#include "CFigure.hpp"
#include "CCollectionEventsSystem.hpp"
#include "CTypeDescrPhysics.hpp"
#include "CArrayRef.hpp"

struct SPrvDataPrivateRobot
{
    class CMemoryRobot *memory;
    char *markObjective;
    class CAgent *actions;
};

static const unsigned long MEMORY_WIDTH = 40;
static const unsigned long MEMORY_HEIGTH = 40;

//-----------------------------------------------------------------------

static struct SPrvDataPrivateRobot *prv_createDataRobot(
                        class CMemoryRobot **memory,
                        const char *markObjective,
                        class CAgent **actions)
{
    struct SPrvDataPrivateRobot *dataPrivate;

    dataPrivate = new SPrvDataPrivateRobot;

    dataPrivate->memory = ASSIGN_PP_NO_NULL(memory, CMemoryRobot);
    dataPrivate->markObjective = CString::copy(markObjective);
    dataPrivate->actions = ASSIGN_PP(actions, class CAgent);

    return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroyDataRobot(struct SPrvDataPrivateRobot **dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(*dataPrivate);

    CString::free(&(*dataPrivate)->markObjective);

    DELETE_OBJECT(&(*dataPrivate)->memory, class CMemoryRobot);

    if ((*dataPrivate)->actions != NULL)
        DELETE_OBJECT(&(*dataPrivate)->actions, class CAgent);

    DELETE_OBJECT(dataPrivate, struct SPrvDataPrivateRobot);
}

//-----------------------------------------------------------------------

CRobot::CRobot()
{
    class CMemoryRobot *memory;
    const char *markObjective;
    class CAgent *actions;
    long xRobot, yRobot;

    xRobot = MEMORY_WIDTH / 2;
    yRobot = MEMORY_HEIGTH / 2;

    memory = new CMemoryRobot(
                    MEMORY_WIDTH, MEMORY_HEIGTH,
                    32, 32, //widthRobot, long heightRobot,
                    xRobot, yRobot, 0); //xRobot, yRobot, angleRobot

    markObjective = CConstants::MARK_EMPTY;
    actions = NULL;

    m_dataPrivate = prv_createDataRobot(&memory, markObjective, &actions);
}

//-----------------------------------------------------------------------

CRobot::~CRobot()
{
    prv_destroyDataRobot(&m_dataPrivate);
}

//-----------------------------------------------------------------------

static void prv_processEventsLaser(const class CCollectionEventsSystem *allEvents, class CMemoryRobot *memory)
{
    class CArrayRef<CEventSystem> *allEventsLaser;

    assert_no_null(memory);

    allEventsLaser = allEvents->allTypeEvent(CEventLaser::ID_EVENT);

    for (unsigned long i = 0; i < allEventsLaser->size(); i++)
    {
        class CEventSystem *evtSystem;
        class CEventLaser *evtLaser;

        evtSystem = allEventsLaser->get(i);
        evtLaser = dynamic_cast<class CEventLaser *>(evtSystem);
        assert_no_null(evtLaser);

        evtLaser->saveInMemory(memory);
    }

    delete allEventsLaser;
}

//-----------------------------------------------------------------------

static void prv_processEventsVideo(
                            const class CCollectionEventsSystem *allEvents,
                            class CMemoryRobot *memory,
                            char **markObjective)
{
    class CArrayRef<CEventSystem> *allEventsVideo;

    assert_no_null(memory);
    assert_no_null(markObjective);
    assert_no_null(*markObjective);

    allEventsVideo = allEvents->allTypeEvent(CEventVideo::ID_EVENT);

    for (unsigned long i = 0; i < allEventsVideo->size(); i++)
    {
        class CEventSystem *evtSystem;
        class CEventVideo *evtVideo;

        evtSystem = allEventsVideo->get(i);
        evtVideo = dynamic_cast<class CEventVideo *>(evtSystem);
        assert_no_null(evtVideo);

        evtVideo->saveInMemory(memory);

        if (evtVideo->isObjetive(*markObjective) == true)
        {
            CString::free(markObjective);
            *markObjective = CString::copy(CConstants::MARK_EMPTY);
        }
    }

    delete allEventsVideo;
}

//-----------------------------------------------------------------------

class CAgent *CRobot::evolution(class CCollectionEventsSystem *allEvents)
{
    class CEventSystem *evtSystem;

    assert_no_null(m_dataPrivate);
    assert_no_null(allEvents);

    prv_processEventsLaser(allEvents, m_dataPrivate->memory);
    prv_processEventsVideo(allEvents, m_dataPrivate->memory, &m_dataPrivate->markObjective);

    if (allEvents->existEventSystem(CEventDecide::ID_EVENT, NULL) == true)
    {
        if (m_dataPrivate->actions != NULL)
            delete m_dataPrivate->actions;

        m_dataPrivate->actions = m_dataPrivate->memory->decide(m_dataPrivate->markObjective);
    }

    if (allEvents->existEventSystem(CEventMarkObjective::ID_EVENT, &evtSystem) == true)
    {
        class CEventMarkObjective *eventMarkObjetive;
        const char *markObjetive;

        eventMarkObjetive = dynamic_cast<class CEventMarkObjective *>(evtSystem);
        assert_no_null(eventMarkObjetive);

        CString::free(&m_dataPrivate->markObjective);
        markObjetive = eventMarkObjetive->markObjective();
        m_dataPrivate->markObjective = CString::copy(markObjetive);
    }

    return this;
}

//-----------------------------------------------------------------------

class CAgent *CRobot::representation(class CTypeDescription *evtDraw)
{
    class CAgent *representation;
    class CAgent *figureRobotMoved, *figureRobot, *figureMark, *figureMemory;
    class CTypeDescrPhysics *evtDrawMoveSprite;

    assert_no_null(m_dataPrivate);

    evtDrawMoveSprite = dynamic_cast<class CTypeDescrPhysics *>(evtDraw);

    if (evtDrawMoveSprite != NULL)
    {
        class CMultiObject3d *multiobjects;

        figureMark = new CFigure(m_dataPrivate->markObjective);
        figureRobot = new CFigure(CDisplaySimulationRobot::SYMBOL_ROBOT);
        figureRobotMoved = evtDrawMoveSprite->move("", &figureRobot);

        figureMemory = m_dataPrivate->memory->figureImageMemory();

        multiobjects = new CMultiObject3d;

        multiobjects->appendChild(&figureRobotMoved);
        multiobjects->appendChild(&figureMark);
        multiobjects->appendChild(&figureMemory);

        representation = multiobjects;
    }
    else
    {
        class CTypeDescriptionExecuteRobot *evtDrawExecuteRobot;

        evtDrawExecuteRobot = dynamic_cast<class CTypeDescriptionExecuteRobot *>(evtDraw);
        assert_no_null(evtDrawExecuteRobot);

        if (m_dataPrivate->actions != NULL)
        {
            representation = m_dataPrivate->actions;
            m_dataPrivate->actions = NULL;
        }
        else
            representation = NULL;
    }

    return representation;
}
