/* -----------------------------------------------------------------------------
 *  CEventAcceleration.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CEventAcceleration.inl"

#include "asrtbas.h"
#include "memory.h"

#include "CString.hpp"

const char *CEventAcceleration::ID_EVENT = "Acceleration";

struct prv_dataPrivateEventAcceleration_t
{
    char *idCar;
    double incrAcceleration;
};

//-----------------------------------------------------------------------

static struct prv_dataPrivateEventAcceleration_t *prv_create(const char *idCar, double incrAcceleration)
{
    struct prv_dataPrivateEventAcceleration_t *dataPrivate;

    dataPrivate = MALLOC(struct prv_dataPrivateEventAcceleration_t);

    dataPrivate->idCar = CString::copy(idCar);
    dataPrivate->incrAcceleration = incrAcceleration;

    return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroy(struct prv_dataPrivateEventAcceleration_t **dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(*dataPrivate);

    CString::free(&(*dataPrivate)->idCar);

    FREE_T(dataPrivate, struct prv_dataPrivateEventAcceleration_t);
}

//-----------------------------------------------------------------------

CEventAcceleration::CEventAcceleration(const char *idCar, double incrAcceleration) : CEventSystem(CEventAcceleration::ID_EVENT)
{
    m_dataPrivate = prv_create(idCar, incrAcceleration);
}

//-----------------------------------------------------------------------

CEventAcceleration::~CEventAcceleration()
{
    prv_destroy(&m_dataPrivate);
}

//-----------------------------------------------------------------------

void CEventAcceleration::applyAcceleration(const char *idCar, double *acceleration)
{
    assert_no_null(m_dataPrivate);

    if (CString::equal(idCar, m_dataPrivate->idCar) == true)
        *acceleration += m_dataPrivate->incrAcceleration;
}
