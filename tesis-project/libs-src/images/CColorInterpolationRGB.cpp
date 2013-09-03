#include "CColorInterpolationRGB.hpp"

#include "asrtbas.h"
#include "CGeometry.hpp"

#define NUM_INTERVAL 3

static const double i_VECTORS_REFERENCE[NUM_INTERVAL][3] =
{
	{0., 0., 0.},
	{0.5, 0.5, 0.5},
	{1., 1., 1.}
};

//-----------------------------------------------------------------------

CColorInterpolationRGB::CColorInterpolationRGB(double minimum, double maximum)
{
	assert(maximum > minimum);
	
	m_minimum = minimum; 
	m_factor = 1. / (maximum - minimum);  	
}

//-----------------------------------------------------------------------

static unsigned long i_calculateVectorReference(double t)
{
	assert(t >= -0.000001);
	assert(t <= 1.0000001);

	if (t < 0.5)
		return 0;
	else
		return 1; 
}

//-----------------------------------------------------------------------

void CColorInterpolationRGB::getColorValue(double value, double *fred, double *fgreen, double *fblue)
{
	double t;
	unsigned long indVectorReferenceOrigin, indVectorReferenceEnd;
	
	t = (value - m_minimum) * m_factor;
	
	assert(t >= -0.000001);
	assert(t <= 1.0000001);
	
	indVectorReferenceOrigin = i_calculateVectorReference(t);
	assert(indVectorReferenceOrigin < NUM_INTERVAL - 1);
	indVectorReferenceEnd = indVectorReferenceOrigin + 1;
	
	CGeometry::calculateParametricPoint(
					i_VECTORS_REFERENCE[indVectorReferenceOrigin][0], 
					i_VECTORS_REFERENCE[indVectorReferenceOrigin][1],
					i_VECTORS_REFERENCE[indVectorReferenceOrigin][2], 
					i_VECTORS_REFERENCE[indVectorReferenceEnd][0], 
					i_VECTORS_REFERENCE[indVectorReferenceEnd][1],
					i_VECTORS_REFERENCE[indVectorReferenceEnd][2], 
					t,
					fred, fgreen, fblue);
}
