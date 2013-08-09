// Interfaz para generar im�genes.

#ifndef CABSTRACTGENERATORIMAGE_HPP_
#define CABSTRACTGENERATORIMAGE_HPP_

class CAbstractGeneratorImage
{
	public:
	
		virtual ~CAbstractGeneratorImage() {;}
		
		virtual unsigned char *generateImage(unsigned long *width, unsigned long *height) const = 0;
		
	protected:
		
		unsigned char *createBuffer(unsigned long width, unsigned long height) const;
};

#endif /*CABSTRACTGENERATORIMAGE_HPP_*/
