#include "CDataPixelRegion.hpp"

#include "asrtbas.h"
#include "memory.h"

#include "CImg.hpp"
#include "CArrPixel.hpp"
#include "array.h"
#include "stdlib.h"

struct i_dataPixelRegion_t
{
	unsigned long m_idRegion; 
	unsigned long m_xPixel, m_yPixel;

	bool m_isBorder;
	double m_fred, m_fgreen, m_fblue;
	double m_fredRegion, m_fgreenRegion, m_fblueRegion;
};

//-----------------------------------------------------------------------

static struct i_dataPixelRegion_t *i_createData(
						unsigned long idRegion,
						unsigned long xPixel, unsigned long yPixel,
						bool isBorder,
						double fred, double fgreen, double fblue,
						double fredRegion, double fgreenRegion, double fblueRegion)
{
	struct i_dataPixelRegion_t *dataPixelRegion;
	
	dataPixelRegion = MALLOC(struct i_dataPixelRegion_t);

	dataPixelRegion->m_idRegion = idRegion;
	
	dataPixelRegion->m_xPixel = xPixel; 
	dataPixelRegion->m_yPixel = yPixel;
	
	dataPixelRegion->m_isBorder = isBorder;
	dataPixelRegion->m_fred = fred; 
	dataPixelRegion->m_fgreen = fgreen; 
	dataPixelRegion->m_fblue = fblue;
	
	dataPixelRegion->m_fredRegion = fredRegion;  
	dataPixelRegion->m_fgreenRegion = fgreenRegion; 
	dataPixelRegion->m_fblueRegion = fblueRegion;
	
	return dataPixelRegion;
}

//-----------------------------------------------------------------------

static void i_destroyDataPixelRegion(struct i_dataPixelRegion_t **dataPixelRegion)
{
	FREE_T(dataPixelRegion, struct i_dataPixelRegion_t); 
}

//-----------------------------------------------------------------------

CDataPixelRegion::CDataPixelRegion(
						unsigned long idRegion,
						unsigned long xPixel, unsigned long yPixel,
						double fred, double fgreen, double fblue)
{
	bool isBorder;
	double fredRegion, fgreenRegion, fblueRegion;
	
	isBorder = false;
	fredRegion = 0.; 
	fgreenRegion = 0.; 
	fblueRegion = 0.;
	
	m_dataPixelRegion = i_createData(
						idRegion, xPixel, yPixel,
						isBorder,
						fred, fgreen, fblue,
						fredRegion, fgreenRegion, fblueRegion);
}

//-----------------------------------------------------------------------

CDataPixelRegion::~CDataPixelRegion()
{
	i_destroyDataPixelRegion(&m_dataPixelRegion);
}				

//-----------------------------------------------------------------------

bool CDataPixelRegion::isPixelAtDisparityWithColorByDistance(
						const class CImg *image,
						unsigned long disparity,
						double tolerance) const
{ 
	long xPixel, yPixel;
	
	assert_no_null(m_dataPixelRegion);
	assert_no_null(image);
	
	xPixel = m_dataPixelRegion->m_xPixel + disparity; 
	yPixel = m_dataPixelRegion->m_yPixel;
	
	if (image->isPosition(xPixel, yPixel) == true)
		return image->isPixelWithColor(
					xPixel, yPixel, 
					m_dataPixelRegion->m_fred, m_dataPixelRegion->m_fgreen, m_dataPixelRegion->m_fblue,
					tolerance);
	else
		return false;
}

//-----------------------------------------------------------------------

void CDataPixelRegion::appendPixel(class CArrPixel *pixels)
{
	assert_no_null(m_dataPixelRegion);
	pixels->add(m_dataPixelRegion->m_xPixel, m_dataPixelRegion->m_yPixel);
}

//-----------------------------------------------------------------------

void CDataPixelRegion::acumulateColor(double *fred, double *fgreen, double *fblue) const
{
	assert_no_null(fred);
	assert_no_null(fgreen);
	assert_no_null(fblue);
	
	*fred += m_dataPixelRegion->m_fred;
	*fgreen += m_dataPixelRegion->m_fgreen;
	*fblue += m_dataPixelRegion->m_fblue;
}

//-----------------------------------------------------------------------

void CDataPixelRegion::setColorRegion(double fred, double fgreen, double fblue)
{
	assert_no_null(m_dataPixelRegion);
	
	m_dataPixelRegion->m_fredRegion = fred; 
	m_dataPixelRegion->m_fgreenRegion = fgreen; 
	m_dataPixelRegion->m_fblueRegion = fblue; 
}
		
//-----------------------------------------------------------------------

void CDataPixelRegion::getColorRegion(double *fred, double *fgreen, double *fblue) const
{
	assert_no_null(fred);
	assert_no_null(fgreen);
	assert_no_null(fblue);
	assert_no_null(m_dataPixelRegion);
	
	if (m_dataPixelRegion->m_isBorder == true && getenv("BORDER_OFF") == NULL)
	{
		*fred = 0.; 
		*fgreen = 0.; 
		*fblue = 0.;
	}
	else
	{
		*fred = m_dataPixelRegion->m_fredRegion; 
		*fgreen = m_dataPixelRegion->m_fgreenRegion; 
		*fblue = m_dataPixelRegion->m_fblueRegion;
	}
}

//-----------------------------------------------------------------------

void CDataPixelRegion::selectBorder(const CDataPixelRegion *dataPixel)
{
	assert_no_null(dataPixel);
	
	if (dataPixel->m_dataPixelRegion->m_idRegion != m_dataPixelRegion->m_idRegion)
		m_dataPixelRegion->m_isBorder = true;
}

//-----------------------------------------------------------------------

class CRegionPixels *CDataPixelRegion::selectRegion(ArrClass(CRegionPixels) *regions) const
{
	assert_no_null(m_dataPixelRegion);
	return array_GetClass(CRegionPixels, regions, m_dataPixelRegion->m_idRegion);
}
