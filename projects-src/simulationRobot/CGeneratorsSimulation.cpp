/* -----------------------------------------------------------------------------
 *  CGeneratorsSimulation.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CGeneratorsSimulation.inl"

#include "CWorldSimulationRobot.inl"
#include "CConstants.inl"
#include "CEventSimulationRobot.inl"

#include "asrtbas.h"
#include "memory.h"
#include "CEventKey.hpp"
#include "CCollectionEventsSystem.hpp"
#include "CArraysTypes.hpp"
#include "CArrayString.hpp"
#include "CMath.hpp"

struct prv_dataPrivateGeneratorEventsLaserSimulation_t
{
    double angleSpectrum, incrAngleSpectrum;
    double distanceSpectrum, incrDistanceSpectrum;

    const class CWorldSimulationRobot *world;
};

struct prv_dataPrivateGeneratorEventsVideoSimulation_t
{
    double angleFocus, distanceFocus;

    const class CWorldSimulationRobot *world;
};

static double PRV_ANGLE_SPECTRUM_DEGREE = 45.;
static double PRV_INCR_ANGLE_SPECTRUM_DEGREE = 5.;
static double PRV_DISTANCE_SPECTRUM = 3.;
static double PRV_INCR_DISTANCE = 1.;

static double PRV_ANGLE_FOCUS_DEGREE = 45.;
static double PRV_DISTANCE_FOCUS = 3.;

//-----------------------------------------------------------------------

static struct prv_dataPrivateGeneratorEventsLaserSimulation_t *prv_createGeneratorEventsLaserSimulation(
                        double angleSpectrum, double incrAngleSpectrum,
                        double distanceSpectrum, double incrDistanceSpectrum,
                        const class CWorldSimulationRobot *world)
{
    struct prv_dataPrivateGeneratorEventsLaserSimulation_t *dataPrivate;

    dataPrivate = MALLOC(struct prv_dataPrivateGeneratorEventsLaserSimulation_t);

    dataPrivate->angleSpectrum = angleSpectrum;
    dataPrivate->incrAngleSpectrum = incrAngleSpectrum;

    dataPrivate->distanceSpectrum = distanceSpectrum;
    dataPrivate->incrDistanceSpectrum = incrDistanceSpectrum;

    dataPrivate->world = world;

    return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroyGeneratorEventsLaserSimulation(struct prv_dataPrivateGeneratorEventsLaserSimulation_t **dataPrivate)
{
    FREE_T(dataPrivate, struct prv_dataPrivateGeneratorEventsLaserSimulation_t);
}

//-----------------------------------------------------------------------

CGeneratorEventsLaserSimulation::CGeneratorEventsLaserSimulation(const class CWorldSimulationRobot *world)
{
    double angleSpectrum, incrAngleSpectrum;

    angleSpectrum = CMath::degreeToRadian(PRV_ANGLE_SPECTRUM_DEGREE);
    incrAngleSpectrum = CMath::degreeToRadian(PRV_INCR_ANGLE_SPECTRUM_DEGREE);

    m_dataPrivate = prv_createGeneratorEventsLaserSimulation(
                            angleSpectrum, incrAngleSpectrum,
                            PRV_DISTANCE_SPECTRUM, PRV_INCR_DISTANCE, world);
}

//-----------------------------------------------------------------------

CGeneratorEventsLaserSimulation::~CGeneratorEventsLaserSimulation()
{
    prv_destroyGeneratorEventsLaserSimulation(&m_dataPrivate);
}

//-----------------------------------------------------------------------

void CGeneratorEventsLaserSimulation::beginEvolution(class CCollectionEventsSystem *allEvents)
{
    class CArrayDouble *distances, *angles;
    unsigned long num;

    assert_no_null(m_dataPrivate);
    assert_no_null(allEvents);

    m_dataPrivate->world->getObstaclesSimulation(
                            m_dataPrivate->angleSpectrum, m_dataPrivate->incrAngleSpectrum,
                            m_dataPrivate->distanceSpectrum, m_dataPrivate->incrDistanceSpectrum,
                            &distances, &angles);

    num = distances->size();
    assert(num == angles->size());

    for (unsigned long i = 0; i < num; i++)
    {
        class CEventSystem *evtLaser;
        double distance, angle;

        distance = distances->get(i);
        angle = angles->get(i);

        evtLaser = new CEventLaser(distance, angle);
        allEvents->appendEventSystem(&evtLaser);
    }

    delete distances;
    delete angles;
}

//-----------------------------------------------------------------------

static struct prv_dataPrivateGeneratorEventsVideoSimulation_t *prv_createGeneratorEventsVideoSimulation(
                    double angleFocus,
                    double distanceFocus,
                    const class CWorldSimulationRobot *world)
{
    struct prv_dataPrivateGeneratorEventsVideoSimulation_t *dataPrivate;

    dataPrivate = MALLOC(struct prv_dataPrivateGeneratorEventsVideoSimulation_t);

    dataPrivate->angleFocus = angleFocus;
    dataPrivate->distanceFocus = distanceFocus;
    dataPrivate->world = world;

    return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroyGeneratorEventsVideoSimulation(struct prv_dataPrivateGeneratorEventsVideoSimulation_t **dataPrivate)
{
    FREE_T(dataPrivate, struct prv_dataPrivateGeneratorEventsVideoSimulation_t);
}

//-----------------------------------------------------------------------

CGeneratorEventsVideoSimulation::CGeneratorEventsVideoSimulation(const class CWorldSimulationRobot *world)
{
    double angleFocus;

    angleFocus = CMath::degreeToRadian(PRV_ANGLE_FOCUS_DEGREE);

    m_dataPrivate = prv_createGeneratorEventsVideoSimulation(angleFocus, PRV_DISTANCE_FOCUS, world);
}

//-----------------------------------------------------------------------

CGeneratorEventsVideoSimulation::~CGeneratorEventsVideoSimulation()
{
    prv_destroyGeneratorEventsVideoSimulation(&m_dataPrivate);
}

//-----------------------------------------------------------------------

void CGeneratorEventsVideoSimulation::beginEvolution(class CCollectionEventsSystem *allEvents)
{
    class CArrayString *marks;
    unsigned long numMarks;

    assert_no_null(m_dataPrivate);
    assert_no_null(m_dataPrivate->world);
    assert_no_null(allEvents);

    marks = NULL;
    m_dataPrivate->world->getMarkSimulation(m_dataPrivate->angleFocus, m_dataPrivate->distanceFocus, &marks);

    numMarks = marks->size();

    for (unsigned long i = 0; i < numMarks; i++)
    {
        const char *mark;
        class CEventSystem *evtVideo;

        mark = marks->get(i);
        evtVideo = new CEventVideo(mark);
        allEvents->appendEventSystem(&evtVideo);
    }

    delete marks;
}

//-----------------------------------------------------------------------

static void prv_appendEventMark(const char *mark, class CCollectionEventsSystem *events)
{
    class CEventSystem *objetive;

    objetive = new CEventMarkObjective(mark);
    events->appendEventSystem(&objetive);
}

//-----------------------------------------------------------------------

void CGeneratorEventsObjectiveSimulation::generateEventWithKey(const class CEventKey *evtKey, class CCollectionEventsSystem *events)
{
    unsigned int key;

    assert_no_null(evtKey);

    key = evtKey->key();

    if (key == '1')
        prv_appendEventMark(CConstants::MARK_OBJECTIVE1, events);
    else if (key == '2')
        prv_appendEventMark(CConstants::MARK_OBJECTIVE2, events);
    else if (key == '3')
        prv_appendEventMark(CConstants::MARK_OBJECTIVE3, events);
    else if (key == '4')
        prv_appendEventMark(CConstants::MARK_OBJECTIVE4, events);
}

//-----------------------------------------------------------------------

void CGeneratorEventsDecide::beginEvolution(class CCollectionEventsSystem *allEvents)
{
    class CEventSystem *evtDecide;

    assert_no_null(allEvents);

    evtDecide = new CEventDecide();
    allEvents->appendEventSystem(&evtDecide);
}
