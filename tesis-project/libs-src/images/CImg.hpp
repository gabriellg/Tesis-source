/* -----------------------------------------------------------------------------
*  CImage (Generic RGB / Gray / BW image class)
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

class CImg
{
    public:

		CImg(unsigned long width, unsigned long height);
		CImg(unsigned long width, unsigned long height, unsigned long nChannels, unsigned char r, unsigned char g, unsigned char b);
        CImg(unsigned long width, unsigned long height, unsigned long nChannels, class CColor *color);
        CImg(unsigned long width, unsigned long height, unsigned long widthStep, unsigned long nChannels, const unsigned char *buffer);
        CImg(class CAbstractGeneratorImage *generator);
        CImg(const class CImg *image);
        CImg(const char *filename);
        ~CImg();

        void size(unsigned long *width, unsigned long *height) const;
        unsigned long getNumChannels(void) const;

        void show(const char *idWindow) const;

        void save(const char* filename) const;

        class CColor *getValue(unsigned long x, unsigned long y) const;
        void getPixelRGB(unsigned long x, unsigned long y, unsigned char *r, unsigned char *g, unsigned char *b) const;
        void getSpace3dRGB(unsigned long x, unsigned long y, double *fredPixel, double *fgreenPixel, double *fbluePixel) const;
        void setValue(unsigned long x, unsigned long y, const class CColor *color);
        void setValueRGB(unsigned long x, unsigned long y, unsigned char r, unsigned char g, unsigned char b);

        void processAllPixels(class IProcessPixel *processOnePixel) const;
        bool isPosition(unsigned long x, unsigned long y) const;
        bool isPixelWithColor(unsigned long xPixel, unsigned long yPixel, double fred, double fgreen, double fblue, double tolerance) const;

        class CImg *transformToRGB(void) const;
        class CImg *transformToGray(void) const;
        class CImg *thresholdBinaryBW(unsigned char threshold) const;

        class CImg *scale(unsigned long newWidth, unsigned long newHeight);
        class CImg *dilate3x3(unsigned long iterations);
        class CImg *erode3x3(unsigned long iterations);
        class CImg *sobel(unsigned long derivateOrderX, unsigned long derivateOrderY, unsigned long apertureSize);
        class CImg *laplace(unsigned long apertureSize);
        class CImg *canny( double threshold1, double threshold2, unsigned int apertureSize);

        void drawImageOnImage(long x, long y, const class CImg *image);

        void getDataRaw(unsigned long *width, unsigned long *height, unsigned long *widthStep, unsigned long *nChannels, unsigned char **buffer) const;

    private:

        CImg(struct prv_dataPrivateImage_t **dataPrivate);

        struct prv_dataPrivateImage_t *m_dataPrivate;
};

#define cimg_load_local_png(nameIcon) i_##nameIcon##_png()
