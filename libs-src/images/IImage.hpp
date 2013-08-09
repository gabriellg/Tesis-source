//IImage.hpp:

#ifndef IIMAGE_HPP_
#define IIMAGE_HPP_

class IImage
{
	public:

		enum TypeImage {RGB, GRAY, BW};

		virtual void save(const char *fileNameWithExtension) const = 0;
		virtual void sizeImage(unsigned long *xSize, unsigned long *ySize) const = 0;

		virtual enum IImage::TypeImage getTypeImage(void) const = 0;
		virtual const struct ImageDataRaw_t *getImageDataRaw(void) const = 0;
};

#endif /* IIMAGE_HPP_ */
