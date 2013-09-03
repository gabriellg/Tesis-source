// Genera los RGB de los pixels de una imagen.

#include "AbstractGeneratorRGB.hpp"

#include "asrtbas.h"
#include "CColor.hpp"

//-----------------------------------------------------------------------

unsigned char *CAbstractGeneratorRGB::generateImage(
						unsigned long *width, 
						unsigned long *height) const
{
	unsigned long width_loc, height_loc;
	unsigned long x, y;
	unsigned char *buffer, *ptr;
	
	assert_no_null(width);
	assert_no_null(height);
	
	getSize(&width_loc, &height_loc);
	
	buffer = createBuffer(width_loc, height_loc);
	ptr = buffer;

	for (y = 0; y < height_loc; y++) 
	{
		for (x = 0; x < width_loc; x++) 
		{
			double fred, fgreen, fblue;
			unsigned char red, green, blue;
			
			generateRGB(x, y, &fred, &fgreen, &fblue);
		    CColor::transformToRGB(fred, fgreen, fblue, &red, &green, &blue);
					
			*ptr = blue; ptr++;
			*ptr = green; ptr++;
			*ptr = red; ptr++;
		}
	}

	*width = width_loc;  
	*height = height_loc;  
	
	return buffer; 
}
