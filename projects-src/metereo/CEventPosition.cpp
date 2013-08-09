// Define un evento con una posición
 
#include "CEventPosition.inl"

#include "asrtbas.h"
#include "memory.h"
#include "CGeometry.hpp"

static const double PRV_PRECISION = 0.00001;

struct prv_dataPrivate_t
{
    double xPos, yPos;
};

//-----------------------------------------------------------------------

static struct prv_dataPrivate_t *prv_create(double xPos, double yPos)
{
    struct prv_dataPrivate_t *dataPrivate;
    
    dataPrivate = MALLOC(struct prv_dataPrivate_t);
    
    dataPrivate->xPos = xPos;
    dataPrivate->yPos = yPos;
    
    return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroy(struct prv_dataPrivate_t **dataPrivate)
{
    FREE_T(dataPrivate, struct prv_dataPrivate_t);
}

//-----------------------------------------------------------------------

CEventPosition::CEventPosition(double xPos, double yPos, const char *id_event) : CEventSystem(id_event)
{
    m_dataPrivate = prv_create(xPos, yPos);
}

//-----------------------------------------------------------------------

CEventPosition::~CEventPosition()
{
    prv_destroy(&m_dataPrivate);
}

//-----------------------------------------------------------------------

bool CEventPosition::isSamePosition(double xPos, double yPos) const
{
    assert_no_null(m_dataPrivate);
    return CGeometry::isSamePoint2D(
                            m_dataPrivate->xPos, m_dataPrivate->yPos,
                            xPos, yPos, PRV_PRECISION);
}

//-----------------------------------------------------------------------

void CEventPosition::getPosition(double *xPos, double *yPos) const
{
    assert_no_null(m_dataPrivate);
    assert_no_null(xPos);
    assert_no_null(yPos);
    
    *xPos = m_dataPrivate->xPos;
    *yPos = m_dataPrivate->yPos;
}
