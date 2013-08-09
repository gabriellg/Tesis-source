/* -----------------------------------------------------------------------------
*  CEventSimulationRobot.cpp
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include "CEventSimulationRobot.inl"

#include "CMemoryRobot.inl"

#include "asrtbas.h"
#include "memory.h"
#include "CString.hpp"

const char *CEventLaser::ID_EVENT = "CEventLaser";
const char *CEventMarkObjective::ID_EVENT = "CEventMarkObjective";
const char *CEventVideo::ID_EVENT = "CEventVideo";
const char *CEventDecide::ID_EVENT = "CEventDecide";

struct prv_dataPrivateEventLaser_t
{
    double distance;
    double angle;
};

//-----------------------------------------------------------------------

static struct prv_dataPrivateEventLaser_t *prv_createEventLaser(double distance, double angle)
{
    struct prv_dataPrivateEventLaser_t *dataPrivate;

    dataPrivate = MALLOC(struct prv_dataPrivateEventLaser_t);

    dataPrivate->distance = distance;
    dataPrivate->angle = angle;

    return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroyEventLaser(struct prv_dataPrivateEventLaser_t **dataPrivate)
{
    FREE_T(dataPrivate, struct prv_dataPrivateEventLaser_t);
}

//-----------------------------------------------------------------------

CEventLaser::CEventLaser(double distance, double angle) : CEventSystem(CEventLaser::ID_EVENT)
{
    m_dataPrivate = prv_createEventLaser(distance, angle);
}

//-----------------------------------------------------------------------

CEventLaser::~CEventLaser()
{
    prv_destroyEventLaser(&m_dataPrivate);
}

//-----------------------------------------------------------------------

void CEventLaser::saveInMemory(class CMemoryRobot *memory)
{
    memory->saveInMemoryLaser(m_dataPrivate->distance, m_dataPrivate->angle);
}

//-----------------------------------------------------------------------

CEventMarkObjective::CEventMarkObjective(const char *mark) : CEventSystem(CEventMarkObjective::ID_EVENT)
{
    m_dataPrivate = (struct prv_dataPrivateEventVideo_t *)CString::copy(mark);
}

//-----------------------------------------------------------------------

CEventMarkObjective::~CEventMarkObjective()
{
    char *mark;

    mark = (char *)m_dataPrivate;
    CString::free(&mark);
}

//-----------------------------------------------------------------------
const char *CEventMarkObjective::markObjective(void) const
{
    char *mark;

    mark = (char *)m_dataPrivate;
    return mark;
}

//-----------------------------------------------------------------------

CEventVideo::CEventVideo(const char *mark) : CEventSystem(CEventVideo::ID_EVENT)
{
    m_dataPrivate = (struct prv_dataPrivateEventVideo_t *)CString::copy(mark);
}

//-----------------------------------------------------------------------

CEventVideo::~CEventVideo()
{
    char *mark;

    mark = (char *)m_dataPrivate;
    CString::free(&mark);
}

//-----------------------------------------------------------------------

void CEventVideo::saveInMemory(class CMemoryRobot *memory)
{
    char *mark;

    mark = (char *)m_dataPrivate;
    memory->saveInMemoryMarkVideo(mark);
}

//-----------------------------------------------------------------------

bool CEventVideo::isObjetive(const char *markObjetive) const
{
    char *mark;

    mark = (char *)m_dataPrivate;
    return CString::equal(mark, markObjetive);
}

