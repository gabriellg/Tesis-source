/* -----------------------------------------------------------------------------
 *  CCar.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CCar.inl"

#include "CDisplayEscalextric.inl"
#include "CEventAcceleration.inl"
#include "CEventApplyVelocity.inl"

#include "asrtbas.h"
#include "memory.h"
#include "CProcessEventSystem.hpp"
#include "CString.hpp"
#include "CFigure.hpp"
#include "CCollectionEventsSystem.hpp"
#include "CTypeDescrPhysics.hpp"

const char *CCar::LEFT = "left";
const char *CCar::RIGHT = "right";

struct prv_dataPrivateCar_t
{
    char *idCar;
	double acceleration;
};

//-----------------------------------------------------------------------

static void prv_integrity(const struct prv_dataPrivateCar_t *car)
{
    assert_no_null(car);
    assert_no_null(car->idCar);
}

//-----------------------------------------------------------------------

static struct prv_dataPrivateCar_t *prv_create(const char *idCar, double acceleration)
{
    struct prv_dataPrivateCar_t *dataPrivate;

    dataPrivate = MALLOC(struct prv_dataPrivateCar_t);

    dataPrivate->idCar = CString::copy(idCar);
    dataPrivate->acceleration = acceleration;

    prv_integrity(dataPrivate);

    return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroy(struct prv_dataPrivateCar_t **dataPrivate)
{
    assert_no_null(dataPrivate);
    prv_integrity(*dataPrivate);

    CString::free(&(*dataPrivate)->idCar);

    FREE_T(dataPrivate, struct prv_dataPrivateCar_t);
}

//---------------------------------------------------------------

CCar::CCar(const char *idCar)
{
    double acceleration;

    acceleration = 0.;
    m_dataPrivate = prv_create(idCar, acceleration);
}

//---------------------------------------------------------------

CCar::~CCar()
{
	prv_destroy(&m_dataPrivate);
}

//---------------------------------------------------------------

class CAgent *CCar::evolution(class CCollectionEventsSystem *allEvents)
{
    class CEventSystem *evtSystem;

    if (allEvents->existEventSystem(CEventAcceleration::ID_EVENT, &evtSystem) == true)
    {
        class CEventAcceleration *evtAcceleration;

        evtAcceleration = dynamic_cast<class CEventAcceleration *>(evtSystem);
        assert_no_null(evtAcceleration);

        evtAcceleration->applyAcceleration(m_dataPrivate->idCar, &m_dataPrivate->acceleration);
    }

    if (allEvents->existEventSystem(CEventApplyVelocity::ID_EVENT, &evtSystem) == true)
    {
        class CEventApplyVelocity *evtApplyVelocity;

        evtApplyVelocity = dynamic_cast<class CEventApplyVelocity *>(evtSystem);
        assert_no_null(evtApplyVelocity);

        evtApplyVelocity->applyVelocity(m_dataPrivate->idCar, m_dataPrivate->acceleration);
        m_dataPrivate->acceleration = 0.;
    }

    return this;
}

//---------------------------------------------------------------

class CAgent *CCar::representation(class CTypeDescription *description)
{
    class CTypeDescrPhysics *evtRepresentation;
    class CAgent *figureCar;

    evtRepresentation = dynamic_cast<class CTypeDescrPhysics *>(description);
    assert_no_null(evtRepresentation);

    figureCar = new CFigure(CDisplayEscalextric::SYMBOL_CAR);
    return evtRepresentation->move(m_dataPrivate->idCar, &figureCar);
}
