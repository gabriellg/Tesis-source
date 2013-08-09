#include "CDataPixelsMatrix.hpp"

#include "IDataPixel.hpp"
#include "IProcessDataPixel.hpp"

#include "asrtbas.h"
#include "memory.h"

struct i_DataPixelsMatrix_t
{
	unsigned long m_width, m_heigth;
	class IDataPixel ***m_matrixPixels;
};

//-----------------------------------------------------------------------

static class IDataPixel ***i_createMatrix(unsigned long width, unsigned long heigth)
{
	class IDataPixel ***matrixPixels;
	unsigned long x, y;
	
	matrixPixels = new IDataPixel **[heigth];
	
	for (y = 0; y < heigth; y++)
	{
		matrixPixels[y] = NULL;
		matrixPixels[y] = new IDataPixel *[width];
		
		for (x = 0; x < width; x++) 
			matrixPixels[y][x] = NULL;
	}
	
	return matrixPixels;
}

//-----------------------------------------------------------------------

static struct i_DataPixelsMatrix_t *i_createDataPixelsMatrix(
						unsigned long width, unsigned long heigth,
						class IDataPixel ****matrixPixels)
{
	struct i_DataPixelsMatrix_t *dataPixelsMatrix;
	
	dataPixelsMatrix = MALLOC(struct i_DataPixelsMatrix_t);
	
	dataPixelsMatrix->m_width = width;
	dataPixelsMatrix->m_heigth = heigth;
	
	dataPixelsMatrix->m_matrixPixels = *matrixPixels;
	*matrixPixels = NULL;
	
	return dataPixelsMatrix;
} 

//-----------------------------------------------------------------------

static void i_destroyDataPixelsMatrix(struct i_DataPixelsMatrix_t **dataPixelsMatrix)
{
	unsigned long x, y;
	
	assert_no_null(dataPixelsMatrix);
	assert_no_null(*dataPixelsMatrix);

	for (y = 0; y < (*dataPixelsMatrix)->m_heigth; y++)
	{
		for (x = 0; x < (*dataPixelsMatrix)->m_width; x++) 
		{
			if ((*dataPixelsMatrix)->m_matrixPixels[y][x] != NULL)
				delete (*dataPixelsMatrix)->m_matrixPixels[y][x];
		}
		
		delete (*dataPixelsMatrix)->m_matrixPixels[y];
	} 	
	
	delete (*dataPixelsMatrix)->m_matrixPixels;
	
	FREE_T(dataPixelsMatrix, struct i_DataPixelsMatrix_t);
} 

//-----------------------------------------------------------------------

CDataPixelsMatrix::CDataPixelsMatrix(unsigned long width, unsigned long heigth)
{
	class IDataPixel ***matrixPixels;
	
	matrixPixels = i_createMatrix(width, heigth);
	m_dataPixelMatrix = i_createDataPixelsMatrix(width, heigth, &matrixPixels);
}

//-----------------------------------------------------------------------

CDataPixelsMatrix::~CDataPixelsMatrix()
{
	i_destroyDataPixelsMatrix(&m_dataPixelMatrix);
}

//-----------------------------------------------------------------------

bool CDataPixelsMatrix::hasDataPixel(long xPixel, long yPixel) const
{
	assert_no_null(m_dataPixelMatrix);
	assert(xPixel >= 0 && yPixel >= 0);
	assert(xPixel < (long)m_dataPixelMatrix->m_width && yPixel < (long)m_dataPixelMatrix->m_heigth);
	
	if (m_dataPixelMatrix->m_matrixPixels[yPixel][xPixel] != NULL)
		return true;
	else
		return false;
}

//-----------------------------------------------------------------------

bool CDataPixelsMatrix::isPosition(long xPixel, long yPixel) const
{
	assert_no_null(m_dataPixelMatrix);
	
	if (xPixel < 0 || yPixel < 0)
		return false;

	if (xPixel >= (long)m_dataPixelMatrix->m_width || yPixel >= (long)m_dataPixelMatrix->m_heigth)
		return false;
	
	return true;
}

//-----------------------------------------------------------------------

static void i_integrityPosition(
					long xPixel, long yPixel, 
					unsigned long width, unsigned long heigth)
{
	assert(xPixel >= 0);
	assert(yPixel >= 0);
	assert((unsigned long)xPixel < width);
	assert((unsigned long)yPixel < heigth); 
} 

//-----------------------------------------------------------------------

class IDataPixel *CDataPixelsMatrix::getDataPixel(long xPixel, long yPixel) const
{
	assert_no_null(m_dataPixelMatrix);
	assert_no_null(m_dataPixelMatrix->m_matrixPixels[yPixel][xPixel]);
	
	return m_dataPixelMatrix->m_matrixPixels[yPixel][xPixel];
}

//-----------------------------------------------------------------------

void CDataPixelsMatrix::setDataPixel(long xPixel, long yPixel, class IDataPixel *dataPixel)
{
	assert_no_null(m_dataPixelMatrix);
	i_integrityPosition(xPixel, yPixel, m_dataPixelMatrix->m_width, m_dataPixelMatrix->m_heigth);
	
	if (m_dataPixelMatrix->m_matrixPixels[yPixel][xPixel] != NULL)
		delete m_dataPixelMatrix->m_matrixPixels[yPixel][xPixel];
		
	m_dataPixelMatrix->m_matrixPixels[yPixel][xPixel] = dataPixel; 
}

//-----------------------------------------------------------------------

void CDataPixelsMatrix::getSize(unsigned long *width, unsigned long *heigth) const
{
	assert_no_null(m_dataPixelMatrix);
	assert_no_null(width);	
	assert_no_null(heigth);
	
	*width = m_dataPixelMatrix->m_width;
	*heigth = m_dataPixelMatrix->m_heigth;
}

//-----------------------------------------------------------------------

void CDataPixelsMatrix::processDataPixels(class IProcessDataPixel *processDataPixel) const
{
	assert_no_null(m_dataPixelMatrix);
	assert_no_null(processDataPixel);
	
	for (unsigned long y = 0; y < m_dataPixelMatrix->m_heigth; y++)
	{
		for (unsigned long x = 0; x < m_dataPixelMatrix->m_width; x++)
		{
			if (m_dataPixelMatrix->m_matrixPixels[y][x] != NULL)
				processDataPixel->processDataPixel(x, y, m_dataPixelMatrix->m_matrixPixels[y][x]);
		}
	} 
}
