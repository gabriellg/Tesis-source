/* -----------------------------------------------------------------------------
 *  CEventApplyVelocity.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CEventApplyVelocity.inl"

#include "CWorldEscalextric.inl"

#include "asrtbas.h"

const char *CEventApplyVelocity::ID_EVENT = "CEventApplyVelocity";

//-----------------------------------------------------------------------

CEventApplyVelocity::CEventApplyVelocity(class CWorldEscalextric *world) : CEventSystem(CEventApplyVelocity::ID_EVENT)
{
    m_dataPrivate = (struct prv_dataPrivateEventApplyVelocity_t *)world;
}

//-----------------------------------------------------------------------

void CEventApplyVelocity::applyVelocity(const char *idCar, double acceleration)
{
    class CWorldEscalextric *world;

    assert_no_null(m_dataPrivate);
    world = (class CWorldEscalextric *)m_dataPrivate;
    world->applyVelocity(idCar, acceleration);
}
