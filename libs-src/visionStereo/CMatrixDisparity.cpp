#include "CMatrixDisparity.hpp"

#include "CRegionPixels.hpp"

#include "asrtbas.h"
#include "array.h"

#include "CImg.hpp"
#include "CArrPixel.hpp"
#include "CDataPixelsMatrix.hpp"
#include "IDataPixel.hpp"
#include "IProcessDataPixel.hpp"
#include "AbstractGeneratorDataPixelsMatrix.hpp"
#include "CColorInterpolationRGB.hpp"

#include "math.h"
#include "sys/param.h"
#include "stdio.h"

class PrvCDataDisparity : public IDataPixel
{
	public:
	
		PrvCDataDisparity(unsigned long disparity) { m_disparity = disparity; }
		
	unsigned long m_disparity;
};

//-----------------------------------------------------------------------

class PrvIteratorMaxMinDisparity : public IProcessDataPixel
{
	public:
	
		PrvIteratorMaxMinDisparity(void);
		
		void getMinMax(double *minimum, double *maximum) const;
		
	private:
	
		double m_minimum, m_maximum;
	
		virtual void processDataPixel(unsigned long x, unsigned long y, IDataPixel *dataPixel);
};

//-----------------------------------------------------------------------

class PrvGeneratorImageDisparity : public AbstractGeneratorDataPixelsMatrix
{
	public:
	
		PrvGeneratorImageDisparity(const class CDataPixelsMatrix *matrixDataPixel, double minimum, double maximum);
		
		virtual ~PrvGeneratorImageDisparity();
		
		virtual void dataPixelToRGB(const class IDataPixel *dataPixel, double *fred, double *fgreen, double *fblue) const;
		
	private:
	
		class CColorInterpolationRGB *m_colorInterpolation;
};

//-----------------------------------------------------------------------

PrvIteratorMaxMinDisparity::PrvIteratorMaxMinDisparity(void)
{
	m_minimum = 1.e10;
	m_maximum = -1.e10;
}
		
//-----------------------------------------------------------------------

void PrvIteratorMaxMinDisparity::getMinMax(double *minimum, double *maximum) const
{
	assert_no_null(minimum);
	assert_no_null(maximum);
	
	*minimum = m_minimum;
	*maximum = m_maximum;
}
		
//-----------------------------------------------------------------------

void PrvIteratorMaxMinDisparity::processDataPixel(unsigned long x, unsigned long y, IDataPixel *dataPixel)
{
	class PrvCDataDisparity *dataDisparity;
	
	assert_no_null(dataPixel);
	
	dataDisparity = (class PrvCDataDisparity *)dataPixel;
	
	m_minimum = MIN(m_minimum, dataDisparity->m_disparity);
	m_maximum = MAX(m_maximum, dataDisparity->m_disparity);
}

//-----------------------------------------------------------------------
	
PrvGeneratorImageDisparity::PrvGeneratorImageDisparity(
						const class CDataPixelsMatrix *matrixDataPixel, 
						double minimum, double maximum) :
						AbstractGeneratorDataPixelsMatrix(matrixDataPixel)
{
	m_colorInterpolation = new CColorInterpolationRGB(minimum, maximum);
}
		
//-----------------------------------------------------------------------
	
PrvGeneratorImageDisparity::~PrvGeneratorImageDisparity()
{
	delete m_colorInterpolation;
}
		
//-----------------------------------------------------------------------
	
void PrvGeneratorImageDisparity::dataPixelToRGB(
						const class IDataPixel *dataPixel, 
						double *fred, double *fgreen, double *fblue) const
{
	class PrvCDataDisparity *dataDisparity;
	
	assert_no_null(m_colorInterpolation);
	
	dataDisparity = (class PrvCDataDisparity *)dataPixel;
	m_colorInterpolation->getColorValue(dataDisparity->m_disparity, fred, fgreen, fblue); 
}

//-----------------------------------------------------------------------

static void i_setPixelsDisparity(
					class CDataPixelsMatrix *matrixDisparity, 
					const class CArrPixel *pixelsRegion,
					unsigned long numPixelsDisparity)
{
	unsigned long i, num;
	
	assert_no_null(matrixDisparity);
    assert_no_null(pixelsRegion);
	
	num = pixelsRegion->size();
	
	for (i = 0; i < num; i++)
	{
		PrvCDataDisparity *pixelDisparity;
		long xPixel, yPixel;
		
		pixelsRegion->get(i, &xPixel, &yPixel);
		
		pixelDisparity = new PrvCDataDisparity(numPixelsDisparity);
		matrixDisparity->setDataPixel(xPixel, yPixel, pixelDisparity);
	}
}

//-----------------------------------------------------------------------

static class CDataPixelsMatrix *i_createMatrixDisparity(
					const class CImg *image,
					const ArrClass(CRegionPixels) *regions,
					unsigned long numPixelsExploration,
					double toleranceDistanceRGB)
{
	class CDataPixelsMatrix *matrixDisparity;
	unsigned long i, num;
	unsigned long width, height;
	
	assert_no_null(image);
	
	image->size(&width, &height);
	matrixDisparity = new CDataPixelsMatrix(width, height);
	
	num = array_numElementosClass(CRegionPixels, regions);
	
	for (i = 0; i < num; i++)
	{
		const class CRegionPixels *region;
		unsigned long numPixelsDisparity;
		
		region = array_GetClass(CRegionPixels, regions, i);
		assert_no_null(region);
		
		if (region->searchRegionToImage(
						image,
						numPixelsExploration,
						toleranceDistanceRGB,
						&numPixelsDisparity) == true)
		{
			class CArrPixel *pixelsRegion;
			
			pixelsRegion = region->pixels();
			i_setPixelsDisparity(matrixDisparity, pixelsRegion, numPixelsDisparity);
			delete pixelsRegion;
		}
	}
	
	return matrixDisparity;
}

//-----------------------------------------------------------------------

CMatrixDisparity::CMatrixDisparity(
					const class CImg *image,
					const ArrClass(CRegionPixels) *regions,
					unsigned long numPixelsExploration,
					double toleranceDistanceRGB)
{
	m_matrixDisparity = i_createMatrixDisparity(
					image, regions, numPixelsExploration,
					toleranceDistanceRGB);
}

//-----------------------------------------------------------------------

static void i_calculateMaximumMinimumDisparity(
					const class CDataPixelsMatrix *matrixDisparity,
					double *minimumDisparity, double *maximumDisparity)
{
	class PrvIteratorMaxMinDisparity *processMaximumMinimum;
	
	processMaximumMinimum = new PrvIteratorMaxMinDisparity();
	matrixDisparity->processDataPixels(processMaximumMinimum);
	processMaximumMinimum->getMinMax(minimumDisparity, maximumDisparity);
	
	delete processMaximumMinimum;
}

//-----------------------------------------------------------------------

class CImg *CMatrixDisparity::imageRepresentationDisparity()
{
	class CImg *imageDisparity;
	class PrvGeneratorImageDisparity *generator;
	double minimumDisparity, maximumDisparity;
	
	i_calculateMaximumMinimumDisparity(
					m_matrixDisparity,
					&minimumDisparity, &maximumDisparity);
					
	printf("Maxima disparidad: %lf\n", maximumDisparity);  
	printf("Minima disparidad: %lf\n", minimumDisparity);
	
	if (fabs(minimumDisparity - maximumDisparity) < 0.0001)
		maximumDisparity = minimumDisparity + 1.;
		
	generator = new PrvGeneratorImageDisparity(m_matrixDisparity, minimumDisparity, maximumDisparity);
	imageDisparity = new CImg(generator);
	delete generator;
	
	return imageDisparity; 					
}
