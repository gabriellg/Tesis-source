/* -----------------------------------------------------------------------------
 *  CGeneratorAcceleration.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CGeneratorPhysics.inl"

#include "CCar.inl"
#include "CEventAcceleration.inl"

#include "asrtbas.h"
#include "CCollectionEventsSystem.hpp"
#include "CEventKey.hpp"

static const double PRV_INCR_ACCELARATION = 1.;

//-----------------------------------------------------------------------
//
static void prv_appendEvent(const char *idCar, double incrAcceleration, class CCollectionEventsSystem *events)
{
    class CEventSystem *evtSystem;

    evtSystem = new CEventAcceleration(idCar, incrAcceleration);
    events->appendEventSystem(&evtSystem);
}

//-----------------------------------------------------------------------
//
static bool prv_isKeyUsed(unsigned int keyPressed)
{
    if (keyPressed == 'a' || keyPressed == 'z' || keyPressed == 'j' || keyPressed == 'm')
        return true;
    else
        return false;
}

//-----------------------------------------------------------------------
//
void CGeneratorAccelerationKey::generateEventWithKey(const class CEventKey *evtKey, class CCollectionEventsSystem *events)
{
    unsigned int keyPressed;

    assert_no_null(evtKey);

    keyPressed = evtKey->key();

    if (prv_isKeyUsed(keyPressed) == true)
    {
        switch (keyPressed)
        {
            case 'a':

                prv_appendEvent(CCar::LEFT, PRV_INCR_ACCELARATION, events);
                break;

            case 'z':

                prv_appendEvent(CCar::LEFT, -PRV_INCR_ACCELARATION, events);
                break;

            case 'j':

                prv_appendEvent(CCar::RIGHT, PRV_INCR_ACCELARATION, events);
                break;

            case 'm':

                prv_appendEvent(CCar::RIGHT, -PRV_INCR_ACCELARATION, events);
                break;

            default_error();
        }
    }
}
