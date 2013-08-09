#include "AbstractGeneratorDataPixelsMatrix.hpp"

#include "CDataPixelsMatrix.hpp"
#include "asrtbas.h"

//-----------------------------------------------------------------------

AbstractGeneratorDataPixelsMatrix::AbstractGeneratorDataPixelsMatrix(
						const class CDataPixelsMatrix *matrixDataPixel)
{
	m_matrixDataPixel = matrixDataPixel;	
}

//-----------------------------------------------------------------------

void AbstractGeneratorDataPixelsMatrix::getSize(unsigned long *width, unsigned long *heigth) const
{
	assert_no_null(m_matrixDataPixel);
	m_matrixDataPixel->getSize(width, heigth);
}

//-----------------------------------------------------------------------

void AbstractGeneratorDataPixelsMatrix::generateRGB(long x, long y, double *fred, double *fgreen, double *fblue) const
{
	assert_no_null(m_matrixDataPixel);
	
	assert_no_null(fred);
	assert_no_null(fgreen);
	assert_no_null(fblue);
	
	if (m_matrixDataPixel->hasDataPixel(x, y) == true)
	{
		class IDataPixel *dataPixel;
		
		dataPixel = m_matrixDataPixel->getDataPixel(x, y);
		dataPixelToRGB(dataPixel, fred, fgreen, fblue);
	}
	else
	{
		*fred = 0.;
		*fgreen = 0.;
		*fblue = 0.;
	}
}
