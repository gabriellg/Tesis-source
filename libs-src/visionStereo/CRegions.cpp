#include "CRegions.hpp"

#include "CRegionPixels.hpp"
#include "CDataPixelRegion.hpp"
#include "CMatrixDisparity.hpp"

#include "CImg.hpp"
#include "CDataPixelsMatrix.hpp"
#include "AbstractGeneratorDataPixelsMatrix.hpp"
#include "IProcessPixel.hpp"
#include "IProcessDataPixel.hpp"

#include "asrtbas.h"
#include "memory.h"
#include "array.h"
#include "identif.h"
#include "strbas.h"
#include "stdio.h"

static const double i_TOLERANCE_RGB = 0.05;
static const unsigned long i_NUM_PIXELS_EXPLORATION = 25; 

class PrvCalculateRegions : public IProcessDataPixel
{
	public:
	
		PrvCalculateRegions(class CDataPixelsMatrix *pixelsMatrix,unsigned long numRegions);
		~PrvCalculateRegions();
		
		ArrClass(CRegionPixels) *getRegions();
	
	private:
	
		virtual void processDataPixel(unsigned long xPixel, unsigned long yPixel, IDataPixel *dataPixel);
		
		class CDataPixelsMatrix *m_pixelsMatrix;
		ArrClass(CRegionPixels) *m_regions;
};

class PrvCSegmentationSemilla : public IProcessPixel
{
	public:
	
		PrvCSegmentationSemilla(const class CImg *image);
		~PrvCSegmentationSemilla();
	
		void getResult(class CDataPixelsMatrix **pixelsMatrix, ArrClass(CRegionPixels) **regions);
		
	private:

		void processPixel(const CImg *image, unsigned long xPixel, unsigned long yPixel);
	
		unsigned long m_idNewRegion;
		class CDataPixelsMatrix *m_pixelsMatrix;
};

class PrvGeneratorImageAllRegions : public AbstractGeneratorDataPixelsMatrix
{
	public:
	
		PrvGeneratorImageAllRegions(const ArrClass(CRegionPixels) *regions, const class CDataPixelsMatrix *pixelsMatrix); 
	
	private:
	
		void dataPixelToRGB(const class IDataPixel *dataPixel, double *fred, double *fgreen, double *fblue) const;
		
		const ArrClass(CRegionPixels) *m_regions;
};

//-----------------------------------------------------------------------

static void i_destroyRegion(class CRegionPixels **region)
{
	assert_no_null(region);
	assert_no_null(*region);
	
	delete *region;
	*region = NULL;
}

//-----------------------------------------------------------------------

PrvCalculateRegions::PrvCalculateRegions(
				class CDataPixelsMatrix *pixelsMatrix,
				unsigned long numRegions)
{
	m_pixelsMatrix = pixelsMatrix;
	m_regions = array_CreaClassPuntero(numRegions, CRegionPixels);
	
	for (unsigned long i = 0; i < numRegions; i++)
	{
		class CRegionPixels *regionPixel;
		
		regionPixel = new CRegionPixels(i);
		array_SetClass(CRegionPixels, m_regions, i, regionPixel);		
	}
}

//-----------------------------------------------------------------------

PrvCalculateRegions::~PrvCalculateRegions()
{
	assert(m_regions == NULL);
}
		
//-----------------------------------------------------------------------

ArrClass(CRegionPixels) *PrvCalculateRegions::getRegions()
{
	ArrClass(CRegionPixels) *regionsToReturn;
	
	assert_no_null(m_regions);
	
	regionsToReturn = m_regions;
	m_regions = NULL;
	
	return regionsToReturn;	
}
	
//-----------------------------------------------------------------------

void PrvCalculateRegions::processDataPixel(
						unsigned long xPixel, unsigned long yPixel,
						IDataPixel *dataPixel)
{
	class CRegionPixels *region;
	class CDataPixelRegion *dataPixelRegion;
	long dy, dx;
	
	dataPixelRegion = (class CDataPixelRegion *)dataPixel;
		
	region = dataPixelRegion->selectRegion(m_regions);
	assert_no_null(region);
	 
	region->appendPixel(xPixel, yPixel, dataPixelRegion);
	
	for (dy = -1; dy < 2; dy++)
		for (dx = -1; dx < 2; dx++)
	{
		if (m_pixelsMatrix->isPosition(xPixel + dx, yPixel + dy) == true)
		{
			class CDataPixelRegion *dataPixelRegionNext;
			
			dataPixelRegionNext = (class CDataPixelRegion *)m_pixelsMatrix->getDataPixel(xPixel + dx, yPixel + dy);
			dataPixelRegion->selectBorder(dataPixelRegionNext);
		}
	}
}

//-----------------------------------------------------------------------

PrvCSegmentationSemilla::PrvCSegmentationSemilla(const class CImg *image)
{
    unsigned long width, height;

    assert_no_null(image);
    image->size(&width, &height);

	m_pixelsMatrix = new CDataPixelsMatrix(width, height);
	m_idNewRegion = 0;
}
		
//-----------------------------------------------------------------------

PrvCSegmentationSemilla::~PrvCSegmentationSemilla()
{
	assert(m_pixelsMatrix == NULL);
}

//-----------------------------------------------------------------------

static void i_calculateColorRegions(ArrClass(CRegionPixels) *regions)
{
	unsigned long i, numRegions;
	
	numRegions = array_numElementosClass(CRegionPixels, regions);
	
	for (i = 0; i < numRegions; i++)
	{
		CRegionPixels *region;
		
		region = array_GetClass(CRegionPixels, regions, i);
		assert_no_null(region);
		
		region->calculateRegionColor();		
	}
}

//-----------------------------------------------------------------------

void PrvCSegmentationSemilla::getResult(class CDataPixelsMatrix **pixelsMatrix, ArrClass(CRegionPixels) **regions)
{
	ArrClass(CRegionPixels) *regions_loc;
	PrvCalculateRegions *calculaRegions;
	
	assert_no_null(pixelsMatrix);
	assert_no_null(regions);
	assert(m_idNewRegion > 0);
	
	calculaRegions = new PrvCalculateRegions(m_pixelsMatrix, m_idNewRegion); 
	m_pixelsMatrix->processDataPixels(calculaRegions);
	regions_loc = calculaRegions->getRegions();
	delete calculaRegions;  
	
	i_calculateColorRegions(regions_loc);
	
	*pixelsMatrix = m_pixelsMatrix;	
	*regions = regions_loc;
	
	m_pixelsMatrix = NULL;
}

//-----------------------------------------------------------------------

static void i_asignaRegion(
					unsigned long numRecursive, 
					const CImg *image,
					unsigned long idRegion,
					double fred, double fgreen, double fblue, 
					unsigned long xPixel, unsigned long yPixel,
					class CDataPixelsMatrix *pixelsMatrix)
{
	numRecursive++;
	assert(numRecursive < 100000);
	
	if (image->isPosition(xPixel, yPixel) == true
			&& pixelsMatrix->hasDataPixel(xPixel, yPixel) == false
			&& image->isPixelWithColor(
					xPixel, yPixel, 
					fred, fgreen, fblue, 
					i_TOLERANCE_RGB) == true)
	{
		CDataPixelRegion *dataPixel;
		double fredPixel, fgreenPixel, fbluePixel;
		long dx, dy;
		
		image->getSpace3dRGB(xPixel, yPixel, &fredPixel, &fgreenPixel, &fbluePixel); 
		
		dataPixel = new CDataPixelRegion(
						idRegion, xPixel, yPixel, 
						fredPixel, fgreenPixel, fbluePixel);
		pixelsMatrix->setDataPixel(xPixel, yPixel, dataPixel);

		for (dy = -1; dy < 2; dy++)
			for (dx = -1; dx < 2; dx++)
		{
			if (dx != 0 || dy != 0)
				i_asignaRegion(
					numRecursive, image, idRegion,
					fred, fgreen, fblue, 
					xPixel + dx, yPixel + dy,
					pixelsMatrix);
		}
	}
}
		
//-----------------------------------------------------------------------

void PrvCSegmentationSemilla::processPixel(const CImg *image, unsigned long xPixel, unsigned long yPixel)
{
	assert_no_null(m_pixelsMatrix);
	
	if (image->isPosition(xPixel, yPixel) == true
			&& m_pixelsMatrix->hasDataPixel(xPixel, yPixel) == false)
	{
		unsigned long numRecursive;
		double fred, fgreen, fblue;
		
		image->getSpace3dRGB(xPixel, yPixel, &fred, &fgreen, &fblue);
		 
		numRecursive = 0;
		i_asignaRegion(
					numRecursive, image,
					m_idNewRegion,
					fred, fgreen, fblue, 
					xPixel, yPixel,
					m_pixelsMatrix);
		
		m_idNewRegion++;
	}
}

//-----------------------------------------------------------------------

PrvGeneratorImageAllRegions::PrvGeneratorImageAllRegions(
						const ArrClass(CRegionPixels) *regions, 
						const class CDataPixelsMatrix *pixelsMatrix) :
						AbstractGeneratorDataPixelsMatrix(pixelsMatrix) 
{
	m_regions = regions;
} 
	
//-----------------------------------------------------------------------

void PrvGeneratorImageAllRegions::dataPixelToRGB(
				const class IDataPixel *dataPixel, 
				double *fred, double *fgreen, double *fblue) const
{
	const class CDataPixelRegion *dataPixelRegion; 
	
	assert_no_null(dataPixel);
	
	dataPixelRegion = (const class CDataPixelRegion *)dataPixel;
	dataPixelRegion->getColorRegion(fred, fgreen, fblue);
}
		
//-----------------------------------------------------------------------

CRegions::CRegions(const class CImg *image)
{
	PrvCSegmentationSemilla *segmentationSemilla;
	unsigned long numRegiones;
	
	assert_no_null(image);
	
	segmentationSemilla = new PrvCSegmentationSemilla(image);
	
	image->processAllPixels(segmentationSemilla);
	
	segmentationSemilla->getResult(&m_pixelsMatrix, &m_regions);
	
	numRegiones = array_numElementosClass(CRegionPixels, m_regions);
	printf("------------ Segmentacion ----------- \n");
	printf("Numero Regiones calculadas: %ld\n", numRegiones);

	delete segmentationSemilla; 
}

//-----------------------------------------------------------------------

CRegions::~CRegions()
{
	assert_no_null(m_pixelsMatrix);
	assert_no_null(m_regions);
	
	delete m_pixelsMatrix;
	array_DestruyeClass(CRegionPixels, &m_regions, i_destroyRegion);
}

//-----------------------------------------------------------------------

class CImg *CRegions::imageAllRegionsCalculated(void) const
{
	class CImg *image;
	PrvGeneratorImageAllRegions *generatorImage;
	
	generatorImage = new PrvGeneratorImageAllRegions(m_regions, m_pixelsMatrix);
	
	image = new CImg(generatorImage);
	
	delete generatorImage;
	
	return image;
}

//-----------------------------------------------------------------------

class CMatrixDisparity *CRegions::calculateMatrixDisparity(
					const class CImg *theOtherImage) const
{
	unsigned long numPixelExploration;
	
	numPixelExploration = strbas_setEnv("NUM_EXPLORATION", i_NUM_PIXELS_EXPLORATION);
	
	return new CMatrixDisparity(
					theOtherImage, m_regions, numPixelExploration,
					i_TOLERANCE_RGB);
}
