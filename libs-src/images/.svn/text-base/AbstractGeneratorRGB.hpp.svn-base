// Genera los RGB de los pixels de una imagen.

#include "AbstractGeneratorImage.hpp"

class CAbstractGeneratorRGB : public CAbstractGeneratorImage
{
	public:
	
		virtual void getSize(unsigned long *width, unsigned long *heigth) const = 0;
		virtual void generateRGB(long x, long y, double *fred, double *fgreen, double *fblue) const = 0;

	private:
			
		virtual unsigned char *generateImage(unsigned long *width, unsigned long *height) const;
};
