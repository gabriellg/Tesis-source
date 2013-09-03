// Clase para procesar p�xeles.

class IProcessPixel
{
	public:
	
		virtual ~IProcessPixel() {;}
		
		virtual void processPixel(const class CImg *image, unsigned long x, unsigned long y) = 0;
};
