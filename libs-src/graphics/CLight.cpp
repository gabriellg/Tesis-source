//CLight.cpp Define una luz.

#include "CLight.hpp"

#include "asrtbas.h"
#include "memory.h"

#include "IGraphics.hpp"
#include "CColor.hpp"

struct prv_dataPrivateCLight_t
{
	class CColor *colorAmbient, *colorDiffuse;
	
	bool withSpecular;
	class CColor *colorSpecular;
	
	bool hasPosition;
	double positionX, positionY, positionZ;
};

//-----------------------------------------------------------------------

static void prv_integrity(const struct prv_dataPrivateCLight_t *dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(dataPrivate->colorAmbient);
    assert_no_null(dataPrivate->colorDiffuse);
}

//-----------------------------------------------------------------------

static struct prv_dataPrivateCLight_t *prv_create(
		class CColor **colorAmbient, 
		class CColor **colorDiffuse,
		bool withSpecular,
		class CColor **colorSpecular,
		bool hasPosition,
		double positionX, double positionY, double positionZ)
{
	struct prv_dataPrivateCLight_t *dataPrivate;
	
	dataPrivate = MALLOC(struct prv_dataPrivateCLight_t);
			
	dataPrivate->colorAmbient = ASSIGN_PP_NO_NULL(colorAmbient, class CColor);
	dataPrivate->colorDiffuse = ASSIGN_PP_NO_NULL(colorDiffuse, class CColor);
	
	dataPrivate->withSpecular = withSpecular;
	dataPrivate->colorSpecular = ASSIGN_PP(colorSpecular, class CColor);  
	
	dataPrivate->hasPosition = hasPosition;
	dataPrivate->positionX = positionX; 
	dataPrivate->positionY = positionY; 
	dataPrivate->positionZ = positionZ;

	return dataPrivate;
}

//---------------------------------------------------------

static void prv_destroy(struct prv_dataPrivateCLight_t **dataPrivate)
{
	assert_no_null(dataPrivate);
	prv_integrity(*dataPrivate);
	
	delete (*dataPrivate)->colorAmbient;
	delete (*dataPrivate)->colorDiffuse;

    if ((*dataPrivate)->colorSpecular != NULL)
        delete (*dataPrivate)->colorSpecular;
	
	FREE_T(dataPrivate, struct prv_dataPrivateCLight_t);
}

//---------------------------------------------------------

CLight::CLight(class CColor **colorAmbient, class CColor **colorDiffuse)
{
	bool withSpecular;
	class CColor *colorSpecular;
	bool hasPosition;
	double positionX, positionY, positionZ;

	withSpecular = false;
	colorSpecular = NULL;
	
	hasPosition = false;
	positionX = 0.; positionY = 0.; positionZ = 0.;
	
	m_dataPrivate = prv_create(colorAmbient, colorDiffuse, withSpecular, &colorSpecular, hasPosition, positionX, positionY, positionZ);
}

//---------------------------------------------------------

CLight::CLight(class CColor **colorAmbient, class CColor **colorDiffuse, class CColor **colorSpecular)
{
	bool withSpecular;
	bool hasPosition;
	double positionX, positionY, positionZ;

	withSpecular = true;
	hasPosition = false;
	positionX = 0.; positionY = 0.; positionZ = 0.;
	
	m_dataPrivate = prv_create(colorAmbient, colorDiffuse, withSpecular, colorSpecular, hasPosition, positionX, positionY, positionZ);
}

//---------------------------------------------------------

CLight::CLight(class CColor **colorAmbient, class CColor **colorDiffuse, class CColor **colorSpecular, double positionX, double positionY, double positionZ)
{
	bool withSpecular;
	bool hasPosition;

	withSpecular = true;
	hasPosition = true;
	
	m_dataPrivate = prv_create(colorAmbient, colorDiffuse, withSpecular, colorSpecular, hasPosition, positionX, positionY, positionZ);
}

//---------------------------------------------------------

CLight::~CLight()
{
	prv_destroy(&m_dataPrivate);
}

//---------------------------------------------------------

static void prv_setLight(
					class IGraphics *graphics,
					const class CColor *color, 
					enum IGraphics::ETypeLight type,
					enum IGraphics::ELight light)
{
	double r, g, b;
	
	color->getColorDouble(&r, &g, &b);
	graphics->defineLight(light, type, r, g, b);
}

//---------------------------------------------------------

void CLight::putLight(class IGraphics *graphics, enum IGraphics::ELight light) const
{
	assert_no_null(m_dataPrivate);

	prv_setLight(graphics, m_dataPrivate->colorAmbient, IGraphics::LIGHT_AMBIENT, light);
	prv_setLight(graphics, m_dataPrivate->colorDiffuse, IGraphics::LIGHT_DIFUSSE, light);
	
    if (m_dataPrivate->withSpecular == true)
    	prv_setLight(graphics, m_dataPrivate->colorSpecular, IGraphics::LIGHT_SPECULAR, light);
    
    if (m_dataPrivate->hasPosition == true)
        graphics->positionLight(
						light,
						m_dataPrivate->positionX, 
						m_dataPrivate->positionY, 
						m_dataPrivate->positionZ);
}
