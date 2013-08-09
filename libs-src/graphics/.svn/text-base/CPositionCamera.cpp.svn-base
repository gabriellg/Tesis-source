// Define posición de camara.

#include "CPositionCamera.hpp"

#include "asrtbas.h"
#include "memory.h"

#include "CGeometry.hpp"
#include "IGraphics.hpp"

struct i_dataPrivate_t
{
    double eyeX, eyeY, eyeZ;
    double pointReferenceX, pointReferenceY, pointReferenceZ;
    double upX, upY, upZ;
    
    double rotXDegrees, rotYDegrees, rotZDegrees;
};

//-----------------------------------------------------------------------

static struct i_dataPrivate_t *i_createDataPrivate(
                        double eyeX, double eyeY, double eyeZ,
                        double pointReferenceX, double pointReferenceY, double pointReferenceZ,
                        double upX, double upY, double upZ,
                        double rotXDegrees, double rotYDegrees, double rotZDegrees)
{
    struct i_dataPrivate_t *dataPrivate;
    
    dataPrivate = MALLOC(struct i_dataPrivate_t);
    
    dataPrivate->eyeX = eyeX; 
    dataPrivate->eyeY = eyeY;  
    dataPrivate->eyeZ = eyeZ;
     
    dataPrivate->pointReferenceX = pointReferenceX;  
    dataPrivate->pointReferenceY = pointReferenceY;   
    dataPrivate->pointReferenceZ = pointReferenceZ; 
    
    dataPrivate->upX = upX;  
    dataPrivate->upY = upY;  
    dataPrivate->upZ = upZ;
    
    dataPrivate->rotXDegrees = rotXDegrees;
    dataPrivate->rotYDegrees = rotYDegrees;
    dataPrivate->rotZDegrees = rotZDegrees;
    
    return dataPrivate;
}

//-----------------------------------------------------------------------

static void i_destroyDataPrivate(struct i_dataPrivate_t **dataPrivate)
{
    FREE_T(dataPrivate, struct i_dataPrivate_t); 
}

//-----------------------------------------------------------------------

CPositionCamera::CPositionCamera(
                        double eyeX, double eyeY, double eyeZ,
                        double pointReferenceX, double pointReferenceY, double pointReferenceZ,
                        double upX, double upY, double upZ)
{
    double rotX, rotY, rotZ;

    rotX = rotY = rotZ = 0.;
    
    m_dataPrivate = i_createDataPrivate(
                        eyeX, eyeY, eyeZ,
                        pointReferenceX, pointReferenceY, pointReferenceZ,
                        upX, upY, upZ, rotX, rotY, rotZ);
}

//-----------------------------------------------------------------------

CPositionCamera::CPositionCamera(const class CPositionCamera *positionCamera)
{
	assert_no_null(positionCamera);
	assert_no_null(positionCamera->m_dataPrivate);
	
	m_dataPrivate = i_createDataPrivate(
							positionCamera->m_dataPrivate->eyeX, 
							positionCamera->m_dataPrivate->eyeY, 
							positionCamera->m_dataPrivate->eyeZ,
							positionCamera->m_dataPrivate->pointReferenceX, 
							positionCamera->m_dataPrivate->pointReferenceY, 
							positionCamera->m_dataPrivate->pointReferenceZ,
							positionCamera->m_dataPrivate->upX, 
							positionCamera->m_dataPrivate->upY, 
							positionCamera->m_dataPrivate->upZ,
							positionCamera->m_dataPrivate->rotXDegrees, 
							positionCamera->m_dataPrivate->rotYDegrees, 
							positionCamera->m_dataPrivate->rotZDegrees);
}

//-----------------------------------------------------------------------

CPositionCamera::~CPositionCamera()
{
    i_destroyDataPrivate(&m_dataPrivate);
}

//-----------------------------------------------------------------------

static void i_makeRotationAxis(
						double rotXDegrees, double rotYDegrees, double rotZDegrees, 
						class IGraphics *graphics)
{
    graphics->rotation(rotXDegrees, 1., 0., 0.);
    graphics->rotation(rotYDegrees, 0., 1., 0.);
    graphics->rotation(rotZDegrees, 0., 0., 1.);
}

//-----------------------------------------------------------------------

void CPositionCamera::getRotation(double *rotXDegrees, double *rotYDegrees, double *rotZDegrees) const
{
    assert_no_null(m_dataPrivate);
    assert_no_null(rotXDegrees);
    assert_no_null(rotYDegrees);
    assert_no_null(rotZDegrees);

    *rotXDegrees = m_dataPrivate->rotXDegrees;
    *rotYDegrees = m_dataPrivate->rotYDegrees;
    *rotZDegrees = m_dataPrivate->rotZDegrees;
}

//-----------------------------------------------------------------------

void CPositionCamera::positionCamera(class IGraphics *graphics) const
{
    assert_no_null(m_dataPrivate);
    
    graphics->positionCamera(
                        m_dataPrivate->eyeX, m_dataPrivate->eyeY, m_dataPrivate->eyeZ,
                        m_dataPrivate->pointReferenceX, m_dataPrivate->pointReferenceY, m_dataPrivate->pointReferenceZ,
                        m_dataPrivate->upX, m_dataPrivate->upY, m_dataPrivate->upZ);

    i_makeRotationAxis(m_dataPrivate->rotXDegrees, m_dataPrivate->rotYDegrees, m_dataPrivate->rotZDegrees, graphics); 
}

//-----------------------------------------------------------------------

void CPositionCamera::makeRotationCamera(class IGraphics *graphics) const
{
    assert_no_null(m_dataPrivate);
    i_makeRotationAxis(m_dataPrivate->rotXDegrees, m_dataPrivate->rotYDegrees, m_dataPrivate->rotZDegrees, graphics); 
}

//-----------------------------------------------------------------------

void CPositionCamera::incrRotationCamera(double incrRotXDegrees, double incrRotYDegrees, double incrRotZDegrees)
{
    assert_no_null(m_dataPrivate);
    
    m_dataPrivate->rotXDegrees += incrRotXDegrees;
    m_dataPrivate->rotYDegrees += incrRotYDegrees;
    m_dataPrivate->rotZDegrees += incrRotZDegrees;
}

//-----------------------------------------------------------------------

void CPositionCamera::frontCamera(double step)
{
    m_dataPrivate->eyeZ -= step;
    m_dataPrivate->pointReferenceZ -= step;
}

//-----------------------------------------------------------------------

void CPositionCamera::backCamera(double step)
{
    m_dataPrivate->eyeZ += step;
    m_dataPrivate->pointReferenceZ += step;
}

//-----------------------------------------------------------------------

void CPositionCamera::setParametricRotation(
                        double t,
                        double rotXIniDegrees, double rotYIniDegrees, double rotZIniDegrees,
                        double rotXEndDegrees, double rotYEndDegrees, double rotZEndDegrees)
{
    double rotX, rotY, rotZ;

    assert_no_null(m_dataPrivate);

    CGeometry::calculateParametricPoint(
                rotXIniDegrees, rotYIniDegrees, rotZIniDegrees,
                rotXEndDegrees, rotYEndDegrees, rotZEndDegrees,
                t, &rotX, &rotY, &rotZ);

    m_dataPrivate->rotXDegrees = rotX;
    m_dataPrivate->rotYDegrees = rotY;
    m_dataPrivate->rotZDegrees = rotZ;
}

