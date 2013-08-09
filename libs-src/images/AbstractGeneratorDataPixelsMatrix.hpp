#ifndef ABSTRACTGENERATORDATAPIXELSMATRIX_HPP_
#define ABSTRACTGENERATORDATAPIXELSMATRIX_HPP_

#include "AbstractGeneratorRGB.hpp"

class AbstractGeneratorDataPixelsMatrix : public CAbstractGeneratorRGB
{
	public:
	
		AbstractGeneratorDataPixelsMatrix(const class CDataPixelsMatrix *matrixDataPixel);
		
		virtual void dataPixelToRGB(const class IDataPixel *dataPixel, double *fred, double *fgreen, double *fblue) const = 0;
		
	private:
	
		const class CDataPixelsMatrix *m_matrixDataPixel;
	
		virtual void getSize(unsigned long *width, unsigned long *heigth) const;
		virtual void generateRGB(long x, long y, double *fred, double *fgreen, double *fblue) const;
};

#endif /*ABSTRACTGENERATORDATAPIXELSMATRIX_HPP_*/
