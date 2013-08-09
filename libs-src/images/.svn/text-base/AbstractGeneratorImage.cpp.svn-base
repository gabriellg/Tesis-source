// Interfaz para generar im�genes.

#include "AbstractGeneratorImage.hpp"

#include "asrtbas.h"
#include "memory.h"

//-----------------------------------------------------------------------

unsigned char *CAbstractGeneratorImage::createBuffer(unsigned long width, unsigned long height) const
{
	size_t numPixels, sizePixel, sizeImage;
	
	sizePixel = 3;
	
	assert(width > 0);
	assert(height > 0);
	
	numPixels = width * height;
	sizeImage = numPixels * sizePixel;
	
	return CALLOC(sizeImage, unsigned char);
}
