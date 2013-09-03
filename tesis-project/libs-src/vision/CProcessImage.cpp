// Procesa una imagen.

#include "CProcessImage.hpp"

#include "asrtbas.h"

#include "CImageData.hpp"
#include "CResultData.hpp"
#include "IGraphics.hpp"

//-----------------------------------------------------------------------

void CProcessImage::draw(const class CResultData *dataResult, class IGraphics *graphics) const
{
	double mitadx, mitady;
	
	mitadx = 10. / 2.; 
	mitady = 15. / 2.;
	
	if (dataResult != NULL)
		dataResult->draw(graphics);
	else
	    graphics->drawQuads(
					-mitadx, -mitady, 0., 
					mitadx, -mitady, 0., 
					mitadx, mitady, 0., 
					-mitadx, mitady, 0.);
}
