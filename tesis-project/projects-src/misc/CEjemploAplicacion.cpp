#include "CVisionAplication.hpp"
#include "CFactoryProcessGraph.hpp"
#include "CProcessGraphImages.hpp"
#include "CProcessImage.hpp"
#include "CRegions.hpp"
#include "CImg.hpp"
#include "CImageData.hpp"
#include "CMatrixDisparity.hpp"

#include "asrtbas.h"
#include "strbas.h"
#include "array.h"
#include "stdio.h"

class CMatrixDisparityResult : public CResultData
{
	public:
	
		CMatrixDisparityResult(
						const class CRegions *regions,
						const class CImg *theOtherImage);
		virtual ~CMatrixDisparityResult();
		
		CRESULTDATA_DECLARE_TYPE(CMatrixDisparityResult);
		
		virtual void draw(class IGraphics *graphics) const;
		
	private:
		
		class CMatrixDisparity *m_matrixDisparity;
};

class CRegionDataResult : public CResultData
{
	public:
	
		CRegionDataResult(const CImg *image);
		virtual ~CRegionDataResult();
		
		class CMatrixDisparityResult *calculateMatrixDisparity(const class CImg *theOtherImage);
		
		CRESULTDATA_DECLARE_TYPE(CRegionDataResult);

		virtual void draw(class IGraphics *graphics) const;
	
	private:
		
		class CImg *m_image;
		class CRegions *m_regions;
};

class CLeeImagen : public CProcessImage
{
	public:
	
		CLeeImagen(const char *ficheroImagen);
		virtual ~CLeeImagen();
		
		virtual CResultData *processImage(const ArrClass(CResultData) *imagesIn);
		
	private:
	
		char *m_ficheroImagen;
};

class CSegmentacionImagen : public CProcessImage
{
	public:
	
		virtual CResultData *processImage(const ArrClass(CResultData) *imagesIn);
};

class CCalculateDisparity : public CProcessImage
{
	public:
	
		virtual CResultData *processImage(const ArrClass(CResultData) *imagesIn);
};

class CEjemploFactoryProcessGraph : public CFactoryProcessGraph
{
	public:
	
		CEjemploFactoryProcessGraph(const char *fichImageLeft, const char *fichImageRight);
		virtual ~CEjemploFactoryProcessGraph(); 
	
	protected:
	
		CProcessGraphImages *processGraphImages() const;
		
	private:
	
		char *m_fichImageLeft, *m_fichImageRight;
};

//-----------------------------------------------------------------------
//-----------------------------------------------------------------------

CMatrixDisparityResult::CMatrixDisparityResult(
						const class CRegions *regions,
						const class CImg *theOtherImage)
{
	assert_no_null(regions);
	m_matrixDisparity = regions->calculateMatrixDisparity(theOtherImage);
}
		
//-----------------------------------------------------------------------

CMatrixDisparityResult::~CMatrixDisparityResult()
{
	delete m_matrixDisparity;
}

//-----------------------------------------------------------------------

void CMatrixDisparityResult::draw(class IGraphics *graphics) const
{
	class CImg *imageDisparity;
	
	imageDisparity = m_matrixDisparity->imageRepresentationDisparity();
	assert_no_null(imageDisparity);
	
	delete imageDisparity; 
}

//-----------------------------------------------------------------------

CRegionDataResult::CRegionDataResult(const class CImg *image)
{
	m_regions = new CRegions(image);
	m_image = m_regions->imageAllRegionsCalculated();
}

//-----------------------------------------------------------------------

CRegionDataResult::~CRegionDataResult()
{
	delete m_regions;
	delete m_image;
}

//-----------------------------------------------------------------------

void CRegionDataResult::draw(class IGraphics *graphics) const
{
    ;
}

//-----------------------------------------------------------------------

CLeeImagen::CLeeImagen(const char *ficheroImagen) : CProcessImage()
{
	m_ficheroImagen = strbas_copyString(ficheroImagen); 
}

//-----------------------------------------------------------------------

CLeeImagen::~CLeeImagen()
{
	strbas_destroy(&m_ficheroImagen);
}
		
//-----------------------------------------------------------------------

CResultData *CLeeImagen::processImage(const ArrClass(CResultData) *imagesIn)
{
	assert(array_numElementosClass(CResultData, imagesIn) == 0);
	return new CImageData(m_ficheroImagen);
}

//-----------------------------------------------------------------------

class CResultData *CSegmentacionImagen::processImage(const ArrClass(CResultData) *imagesIn)
{
	const class CImg *image;
	class CImageData *imagenResult;
	class CResultData *dataIn;
	class CRegionDataResult *regionsResult;
	
	assert(array_numElementosClass(CResultData, imagesIn) == 1);
	dataIn = array_GetClass(CResultData, imagesIn, 0);
	imagenResult = CRESULTDATA_GET(CImageData, dataIn);
	image = imagenResult->getImage();
	
	regionsResult = new CRegionDataResult(image);
	
	return regionsResult; 
}

//-----------------------------------------------------------------------

class CMatrixDisparityResult *CRegionDataResult::calculateMatrixDisparity(
						const class CImg *theOtherImage)
{
	return new CMatrixDisparityResult(m_regions, theOtherImage);
}

//-----------------------------------------------------------------------

class CResultData *CCalculateDisparity::processImage(const ArrClass(CResultData) *imagesIn)
{
	const class CImg *theOtherImage;
	class CRegionDataResult *regionData;
	class CImageData *imageData;
	
	assert(array_numElementosClass(CResultData, imagesIn) == 2);
	
	regionData = cresultdata_searchDataResult(imagesIn, CRegionDataResult);
	imageData = cresultdata_searchDataResult(imagesIn, CImageData);
	theOtherImage = imageData->getImage();
	
	return regionData->calculateMatrixDisparity(theOtherImage);	
}

//-----------------------------------------------------------------------

CEjemploFactoryProcessGraph::CEjemploFactoryProcessGraph(
						const char *fichImageLeft, 
						const char *fichImageRight)
{
	m_fichImageLeft = strbas_copyString(fichImageLeft); 
	m_fichImageRight = strbas_copyString(fichImageRight);
}

//-----------------------------------------------------------------------

CEjemploFactoryProcessGraph::~CEjemploFactoryProcessGraph() 
{
	strbas_destroy(&m_fichImageLeft);
	strbas_destroy(&m_fichImageRight);
}

//-----------------------------------------------------------------------

class CProcessGraphImages *CEjemploFactoryProcessGraph::processGraphImages() const
{
	class CProcessGraphImages *processImages;
	class CProcessImage *processLeeImageIzquierda;
	class CProcessImage *processLeeImageDerecha;
	class CProcessImage *processSegmentationImageIzquierda, *processSegmentationImageDerecha;
	class CProcessImage *processCalculateDisparity;

	processLeeImageIzquierda = new CLeeImagen(m_fichImageLeft);
	processLeeImageDerecha = new CLeeImagen(m_fichImageRight);
	processSegmentationImageIzquierda = new CSegmentacionImagen();
	processSegmentationImageDerecha = new CSegmentacionImagen();
	processCalculateDisparity = new CCalculateDisparity();
	
	processImages = new CProcessGraphImages();
	
	processImages->initDefineGraphProcess();

		processImages->addProcessImage("imagenDerecha", &processLeeImageDerecha);
		processImages->addProcessImage("imagenIzquierda", &processLeeImageIzquierda);
		processImages->addProcessImage("segmentacionImagenIzquierda", &processSegmentationImageIzquierda);
		processImages->addProcessImage("segmentacionImagenDerecha", &processSegmentationImageDerecha);
		processImages->addProcessImage("calculaDisparidad", &processCalculateDisparity);
		
		processImages->addDependProcess("imagenIzquierda", "segmentacionImagenIzquierda");	
		processImages->addDependProcess("imagenDerecha", "segmentacionImagenDerecha");	
		processImages->addDependProcess("imagenDerecha", "calculaDisparidad");	
		processImages->addDependProcess("segmentacionImagenIzquierda", "calculaDisparidad");
						
	processImages->closeDefineGraphProcess();
	
	return processImages;
}


//-----------------------------------------------------------------------

int main(int argc, char *argv[])
{
	if (argc >= 3)
	{
		CVisionAplication *ejemplo;
		CEjemploFactoryProcessGraph *factoryGraph;
		
		factoryGraph = new CEjemploFactoryProcessGraph(argv[1], argv[2]);
		
		ejemplo = new CVisionAplication(factoryGraph, "Grafo de dependencias");
		
		ejemplo->run(argc, argv);
		
		delete ejemplo;
	}
	else
		printf("uso: CEjemploAplicacion imageIzquierda imagenDerecha\n");
		
	return 0;
}
