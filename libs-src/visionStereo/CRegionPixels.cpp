#include "CRegionPixels.hpp"
#include "CDataPixelRegion.hpp"

#include "asrtbas.h"
#include "memory.h"
#include "array.h"
#include "strbas.h"

#include "CArrPixel.hpp"
#include "CImg.hpp"
#include "IDataPixel.hpp"

#include "sys/param.h"
#include "stdio.h"

struct i_region_t
{
	unsigned long idRegion;
	
	double fred, fgreen, fblue;
	
	ArrClass(CDataPixelRegion) *pixelsRegion;
};

//-----------------------------------------------------------------------

static void i_destroyDataPixelRegion(class CDataPixelRegion **dataPixelRegion)
{
	assert_no_null(dataPixelRegion);
	assert_no_null(*dataPixelRegion);
	
	delete *dataPixelRegion;
}

//-----------------------------------------------------------------------

static struct i_region_t *i_createRegion(
						unsigned long idRegion,
						double fred, double fgreen, double fblue, 
						ArrClass(CDataPixelRegion) **pixelsRegion)
{
	struct i_region_t *region;
	
	region = MALLOC(struct i_region_t);
	
	region->idRegion = idRegion;
	region->fred = fred; 
	region->fgreen = fgreen; 
	region->fblue = fblue;
	region->pixelsRegion = ASSIGN_PP_NO_NULL(pixelsRegion, ArrClass(CDataPixelRegion));
	  
	return region;
}

//-----------------------------------------------------------------------

static struct i_region_t *i_createRegionDefault(unsigned long idRegion)
{
	ArrClass(CDataPixelRegion) *pixelsRegion;
	double fred, fgreen, fblue;
	
	pixelsRegion = array_CreaClassPuntero(0, CDataPixelRegion);
	
	fred = 0.; 
	fgreen = 0.; 
	fblue = 0.;
	
	return i_createRegion(idRegion, fred, fgreen, fblue, &pixelsRegion);
}

//-----------------------------------------------------------------------

static void i_destroyRegion(struct i_region_t **region)
{
	array_DestruyeClass(CDataPixelRegion, &(*region)->pixelsRegion, i_destroyDataPixelRegion);
	FREE_T(region, struct i_region_t);
}

//-----------------------------------------------------------------------

CRegionPixels::CRegionPixels(unsigned long idRegion)
{
	m_regionData = i_createRegionDefault(idRegion);
}

//-----------------------------------------------------------------------

CRegionPixels::~CRegionPixels()
{
	assert_no_null(m_regionData);
	i_destroyRegion(&m_regionData);
}

//-----------------------------------------------------------------------

void CRegionPixels::appendPixel(
						long xPixel, long yPixel,
						class CDataPixelRegion *dataPixelRegion)
{
	assert_no_null(m_regionData);
	array_AnyadirClass(CDataPixelRegion, m_regionData->pixelsRegion, dataPixelRegion);
}

//-----------------------------------------------------------------------

static unsigned long i_calculateNumPixelsOk(
						const class CImg *image,
						ArrClass(CDataPixelRegion) *pixelsRegion,
						unsigned long disparity, double tolerance)
{
	unsigned long i, numPixelsRegion;
	unsigned long numPixelsOk;

	assert_no_null(image);

	numPixelsOk = 0;

	numPixelsRegion = array_numElementosClass(CDataPixelRegion, pixelsRegion);

	for (i = 0; i < numPixelsRegion; i++)
	{
		const CDataPixelRegion *pixelRegion;
		
		pixelRegion = array_GetClass(CDataPixelRegion, pixelsRegion, i);
		assert_no_null(pixelsRegion);
		
		if (pixelRegion->isPixelAtDisparityWithColorByDistance( 
						image, disparity, tolerance) == true)
		{
			numPixelsOk++;
		}
	}
	
	return numPixelsOk;
}

//-----------------------------------------------------------------------

bool CRegionPixels::searchRegionToImage(
						const class CImg *image,
						unsigned long numPixelsExploration,
						double tolerance,
						unsigned long *numPixelsDisparity_opt) const 
{
	unsigned long i, numPixelsRegion, numPixelsOkMax, numPixelsRegionMinimum;
	unsigned long disparityOkMax;
	
	assert_no_null(m_regionData);
	numPixelsRegion = array_numElementosClass(
						CDataPixelRegion,
						m_regionData->pixelsRegion);
						
	numPixelsOkMax = 0;
	disparityOkMax = 0;
	
	numPixelsRegionMinimum = strbas_setEnv("NUM_PIXELS_MIN", 15);
	
	if (numPixelsRegion >= numPixelsRegionMinimum)
	{
		for (i = 0; i < numPixelsExploration; i++)
		{
			unsigned long numPixelsOk;
			
			numPixelsOk = i_calculateNumPixelsOk(
							image, m_regionData->pixelsRegion, 
							i, tolerance);
							
			if (numPixelsOk > numPixelsOkMax)
			{
				numPixelsOkMax = numPixelsOk; 
				disparityOkMax = i;
			}
		}
	}
	
	if (numPixelsDisparity_opt != NULL)
		*numPixelsDisparity_opt = disparityOkMax; 

	return true; 
}

//-----------------------------------------------------------------------

class CArrPixel *CRegionPixels::pixels() const
{
	class CArrPixel *pixels;
	unsigned long i, numPixelsRegion;
	
	assert_no_null(m_regionData);

	numPixelsRegion = array_numElementosClass(
							CDataPixelRegion, 
							m_regionData->pixelsRegion);
	pixels = new CArrPixel();

	for (i = 0; i < numPixelsRegion; i++)
	{
		CDataPixelRegion *pixelRegion;
		
		pixelRegion = array_GetClass(CDataPixelRegion, m_regionData->pixelsRegion, i);
		assert_no_null(pixelRegion);
		
		pixelRegion->appendPixel(pixels);
	}
	
	return pixels;
}

//-----------------------------------------------------------------------

void CRegionPixels::colorRegion(double *fred, double *fgreen, double *fblue) const
{
	assert_no_null(fred); 
	assert_no_null(fgreen);
	assert_no_null(fblue);
	
	*fred = ((m_regionData->idRegion) % 255) / 255.;
	*fgreen = ((m_regionData->idRegion) % 255) / 255.;
	*fblue = ((m_regionData->idRegion) % 255) / 255.;
}

//-----------------------------------------------------------------------

void CRegionPixels::calculateRegionColor()
{
	unsigned long i, numPixelsRegion;
	double fred_sum, fgreen_sum, fblue_sum;
	
	numPixelsRegion = array_numElementosClass(CDataPixelRegion, m_regionData->pixelsRegion);
	
	fred_sum = 0.; 
	fgreen_sum = 0.; 
	fblue_sum = 0.;
	
	for (i = 0; i < numPixelsRegion; i++)
	{
		class CDataPixelRegion *pixelRegion;
		
		pixelRegion = array_GetClass(CDataPixelRegion, m_regionData->pixelsRegion, i);
		assert_no_null(pixelRegion);
		
		pixelRegion->acumulateColor(&fred_sum, &fgreen_sum, &fblue_sum);
	}
	
	m_regionData->fred = fred_sum / numPixelsRegion;
	m_regionData->fgreen = fgreen_sum / numPixelsRegion;
	m_regionData->fblue = fblue_sum / numPixelsRegion;
	
	for (i = 0; i < numPixelsRegion; i++)
	{
		class CDataPixelRegion *pixelRegion;
		
		pixelRegion = array_GetClass(CDataPixelRegion, m_regionData->pixelsRegion, i);
		assert_no_null(pixelRegion);
		
		pixelRegion->setColorRegion(m_regionData->fred, m_regionData->fgreen, m_regionData->fblue);
	}
}
