/*
 * CEventCamera.cpp
 *
 *  Created on: 04/11/2013
 *      Author: gabi
 */

#include "CEventCamera.hpp"

#include "asrtbas.h"
#include "memory.h"

const char *CEventCamera::ID_EVENT = "CEventCamera";

struct SPrvEventCamera
{
    enum CEventCamera::ETypeEventCamera typeEvent;

    double step;

    double incrXRotate, incrYRotate, incrZRotate;

    double rotXDegrees, rotYDegrees, rotZDegrees;
};

//-----------------------------------------------------------------------

static struct SPrvEventCamera *prv_createEventCamera(enum CEventCamera::ETypeEventCamera typeEvent, double step, double incrXRotate,
        double incrYRotate, double incrZRotate, double rotXDegrees, double rotYDegrees, double rotZDegrees)
{
    struct SPrvEventCamera *dataPrivate;

    dataPrivate = new struct SPrvEventCamera;

    dataPrivate->typeEvent = typeEvent;

    dataPrivate->step = step;

    dataPrivate->incrXRotate = incrXRotate;
    dataPrivate->incrYRotate = incrYRotate;
    dataPrivate->incrZRotate = incrZRotate;

    dataPrivate->rotXDegrees = rotXDegrees;
    dataPrivate->rotYDegrees = rotYDegrees;
    dataPrivate->rotZDegrees = rotZDegrees;

    return dataPrivate;
}

//---------------------------------------------------------------

CEventCamera::CEventCamera(struct SPrvEventCamera **dataPrivate) : CEventSystem(CEventCamera::ID_EVENT)
{
    m_dataPrivate = ASSIGN_PP_NO_NULL(dataPrivate, struct SPrvEventCamera);
}

//---------------------------------------------------------------

CEventCamera::~CEventCamera()
{
    assert_no_null(m_dataPrivate);
    FREE_T(&m_dataPrivate, struct SPrvEventCamera);
}

//---------------------------------------------------------------

class CEventCamera *CEventCamera::createInitialPositionCamera()
{
    struct SPrvEventCamera *dataPrivate;

    dataPrivate = prv_createEventCamera(INITIAL_CAMERA, 0., //step.
            0., 0., 0., //incrXRotate, incrYRotate, incrZRotate
            0., 0., 0.); //rotXDegrees, rotYDegrees, rotZDegrees

    return new CEventCamera(&dataPrivate);
}

//---------------------------------------------------------------

class CEventCamera *CEventCamera::createTraslateCamera(double step)
{
    struct SPrvEventCamera *dataPrivate;

    dataPrivate = prv_createEventCamera(TRASLATE, step, 0., 0., 0., //incrXRotate, incrYRotate, incrZRotate
            0., 0., 0.); //rotXDegrees, rotYDegrees, rotZDegrees

    return new CEventCamera(&dataPrivate);
}

//---------------------------------------------------------------

class CEventCamera *CEventCamera::createAnimationRotation(double rotXDegrees, double rotYDegrees, double rotZDegrees)
{
    struct SPrvEventCamera *dataPrivate;

    dataPrivate = prv_createEventCamera(ROTATION_ANIMATION, 0., 0., 0., 0., rotXDegrees, rotYDegrees, rotZDegrees);

    return new CEventCamera(&dataPrivate);
}

//---------------------------------------------------------------

class CEventCamera *CEventCamera::createIncrRotateCamera(double incrXRotate, double incrYRotate, double incrZRotate)
{
    struct SPrvEventCamera *dataPrivate;

    dataPrivate = prv_createEventCamera(INCR_ROTATE, 0., incrXRotate, incrYRotate, incrZRotate, 0., 0., 0.);

    return new CEventCamera(&dataPrivate);
}

//---------------------------------------------------------------

enum CEventCamera::ETypeEventCamera CEventCamera::getType() const
{
    assert_no_null(m_dataPrivate);
    return m_dataPrivate->typeEvent;
}

//---------------------------------------------------------------

void CEventCamera::getTranslate(double *step) const
{
    assert_no_null(m_dataPrivate);
    assert_no_null(step);
    assert(m_dataPrivate->typeEvent == CEventCamera::TRASLATE);

    *step = m_dataPrivate->step;
}

//---------------------------------------------------------------

void CEventCamera::getIncrRotate(double *incrXRotate, double *incrYRotate, double *incrZRotate) const
{
    assert_no_null(m_dataPrivate);
    assert_no_null(incrXRotate);
    assert_no_null(incrYRotate);
    assert_no_null(incrZRotate);
    assert(m_dataPrivate->typeEvent == CEventCamera::INCR_ROTATE);

    *incrXRotate = m_dataPrivate->incrXRotate;
    *incrYRotate = m_dataPrivate->incrYRotate;
    *incrZRotate = m_dataPrivate->incrZRotate;
}

//---------------------------------------------------------------

void CEventCamera::getRotateAnimation(double *xRotate, double *yRotate, double *zRotate) const
{
    assert_no_null(m_dataPrivate);
    assert_no_null(xRotate);
    assert_no_null(yRotate);
    assert_no_null(zRotate);
    assert(m_dataPrivate->typeEvent == CEventCamera::ROTATION_ANIMATION);

    *xRotate = m_dataPrivate->rotXDegrees;
    *yRotate = m_dataPrivate->rotYDegrees;
    *zRotate = m_dataPrivate->rotZDegrees;
}
