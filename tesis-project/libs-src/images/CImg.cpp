/* -----------------------------------------------------------------------------
*  CImage (Generic RGB / Gray / BW image class)
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include "CImg.hpp"

#include "libcv.inl"

#include "asrtbas.h"
#include "memory.h"

#include "AbstractGeneratorImage.hpp"
#include "IProcessPixel.hpp"
#include "CColor.hpp"
#include "nomfich.h"

#define PRV_PTRTOCV(data) ((IplImage *)data)
#define PRV_PTRTOPRIVATE(data) ((struct prv_dataPrivateImage_t *)data)

//------------------------------------------------------------------------------

static unsigned char *prv_getPixel(IplImage *cvimg, unsigned long x, unsigned long y)
{
    unsigned char *ptr;

    assert_no_null(cvimg);
    assert(y < (unsigned long)cvimg->height);
    assert(x < (unsigned long)cvimg->width);

    ptr = (unsigned char *)(cvimg->imageData + y * cvimg->widthStep);
    return &ptr[x * cvimg->nChannels];
}

//------------------------------------------------------------------------------
//
static void prv_getPixelRGB(
                        IplImage *cvimg,
                        unsigned long x, unsigned long y,
                        unsigned char *r, unsigned char *g, unsigned char *b)
{
    unsigned char *ptrPixel;

    assert_no_null(r);
    assert_no_null(g);
    assert_no_null(b);

    ptrPixel = prv_getPixel(cvimg, x, y);

    switch(cvimg->nChannels)
    {
        case 4:
        case 3:

            *r = *(ptrPixel+2);
            *g = *(ptrPixel+1);
            *b = *ptrPixel;
            break;

        case 1:

            *r = *ptrPixel;
            *g = *ptrPixel;
            *b = *ptrPixel;
            break;

        default_error();
    }
}

//------------------------------------------------------------------------------
//
static void prv_setPixelRGB(
                        IplImage *cvImage,
                        unsigned long x, unsigned long y,
                        unsigned char r, unsigned char g, unsigned char b)
{
    unsigned char *ptrPixel;

    ptrPixel = prv_getPixel(cvImage, x, y);

    switch(cvImage->nChannels)
    {
        case 3:

            *(ptrPixel+2) = r;
            *(ptrPixel+1) = g;
            *ptrPixel = b;
            break;

        case 1:

            assert(r == g);
            assert(g == b);
            *ptrPixel = r;
            break;

        default_error();
    }
}

//-----------------------------------------------------------------------
//
static IplImage *prv_createRGB(unsigned long width, unsigned long height, unsigned long nChannels)
{
    IplImage *cvimg;

    cvimg = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, nChannels);

    return cvimg;
}

//-----------------------------------------------------------------------
//
CImg::CImg(struct prv_dataPrivateImage_t **dataPrivate)
{
    m_dataPrivate = ASSIGN_PP_NO_NULL(dataPrivate, struct prv_dataPrivateImage_t);
    assert_no_null(m_dataPrivate);
}

//-----------------------------------------------------------------------
//
CImg::CImg(unsigned long width, unsigned long height)
{
    IplImage *cvImage;

    cvImage = prv_createRGB(width, height, 3);
    m_dataPrivate = PRV_PTRTOPRIVATE(cvImage);
    assert_no_null(m_dataPrivate);
}

//-----------------------------------------------------------------------

CImg::CImg(unsigned long width, unsigned long height, unsigned long nChannels, unsigned char r, unsigned char g, unsigned char b)
{
    IplImage *cvImage;

    cvImage = prv_createRGB(width, height, nChannels);
    cvSet(cvImage, CV_RGB(r, g, b), 0);

    m_dataPrivate = PRV_PTRTOPRIVATE(cvImage);
    assert_no_null(m_dataPrivate);
}

//-----------------------------------------------------------------------
//
CImg::CImg(unsigned long width, unsigned long height, unsigned long nChannels, class CColor *color)
{
	IplImage *cvImage;
    unsigned char r, g, b;

    assert_no_null(color);
	color->getColor(&r, &g, &b);

	cvImage = prv_createRGB(width, height, nChannels);
    cvSet(cvImage, CV_RGB(r, g, b), 0);

    m_dataPrivate = PRV_PTRTOPRIVATE(cvImage);
    assert_no_null(m_dataPrivate);
}

//-----------------------------------------------------------------------

CImg::CImg(unsigned long width, unsigned long height, unsigned long widthStep, unsigned long nChannels, const unsigned char *buffer)
{
    IplImage *cvImage;
    unsigned long sizeBuffer;

    sizeBuffer = widthStep * height;

    cvImage = prv_createRGB(width, height, nChannels);

    assert(cvImage->widthStep == (long)widthStep);
    assert(cvImage->imageSize == (int)sizeBuffer);

    memcpy(cvImage->imageData, buffer, sizeBuffer);

    m_dataPrivate = PRV_PTRTOPRIVATE(cvImage);
    assert_no_null(m_dataPrivate);
}

//-----------------------------------------------------------------------

CImg::CImg(class CAbstractGeneratorImage *generator)
{
    IplImage *cvImage;
    unsigned long width, height, size;
    unsigned char *buffer;

    assert_no_null(generator);
    buffer = generator->generateImage(&width, &height);

    //TODO: Falta en canal.
    cvImage = prv_createRGB(width, height, 3);

    size = width * height;
    memcpy(cvImage->imageData, buffer, size);

    FREE_T(&buffer, unsigned char);

    m_dataPrivate = PRV_PTRTOPRIVATE(cvImage);
    assert_no_null(m_dataPrivate);
}

//-----------------------------------------------------------------------
//
CImg::CImg(const CImg *image)
{
    IplImage *cvImage;
    IplImage *cvImageCopy;

    assert_no_null(image);
    assert_no_null(image->m_dataPrivate);

    cvImage = PRV_PTRTOCV(image->m_dataPrivate);
    cvImageCopy = cvCloneImage(cvImage);

    m_dataPrivate = PRV_PTRTOPRIVATE(cvImageCopy);
    assert_no_null(m_dataPrivate);
}

//-----------------------------------------------------------------------
//
CImg::CImg(const char* filename)
{
    IplImage *cvImage;
    bool existFile;

    assert_no_null(filename);

    existFile = nomfich_existe_fichero(filename);
    assert(existFile == true);

    cvImage = cvLoadImage(filename, CV_LOAD_IMAGE_UNCHANGED);
    m_dataPrivate = PRV_PTRTOPRIVATE(cvImage);
    assert_no_null(m_dataPrivate);
}

//-----------------------------------------------------------------------
//
CImg::~CImg()
{
    IplImage *cvImage;

    assert_no_null(m_dataPrivate);

    cvImage = PRV_PTRTOCV(m_dataPrivate);
    cvReleaseImage(&cvImage);
}

//-----------------------------------------------------------------------
//
void CImg::size(unsigned long *width, unsigned long *height) const
{
    IplImage *cvImage;

    assert_no_null(m_dataPrivate);
    assert_no_null(width);
    assert_no_null(height);

    cvImage = PRV_PTRTOCV(m_dataPrivate);

    *width = cvImage->width;
    *height = cvImage->height;
}

//-----------------------------------------------------------------------
//
unsigned long CImg::getNumChannels(void) const
{
    IplImage *cvImage;

    assert_no_null(m_dataPrivate);
    cvImage = PRV_PTRTOCV(m_dataPrivate);
    assert_no_null(cvImage);

    return cvImage->nChannels;
}

//-----------------------------------------------------------------------
//
void CImg::show(const char *idWindow) const
{
    IplImage *cvImage;

    assert_no_null(m_dataPrivate);

    cvImage = PRV_PTRTOCV(m_dataPrivate);

    cvNamedWindow(idWindow, CV_WINDOW_AUTOSIZE);
    cvShowImage(idWindow, cvImage);
    cvWaitKey(0);
    cvDestroyWindow(idWindow);
}

//-----------------------------------------------------------------------
//
void CImg::save( const char* filename ) const
{
    bool status;
    IplImage *cvImage;

    assert_no_null(m_dataPrivate);
    cvImage = PRV_PTRTOCV(m_dataPrivate);

    status = cvSaveImage(filename, cvImage);
    assert(status);
}

//------------------------------------------------------------------------------
//
class CColor *CImg::getValue(unsigned long x, unsigned long y) const
{
	unsigned char r, g, b;
    IplImage *cvImage;

    assert_no_null(m_dataPrivate);
    cvImage = PRV_PTRTOCV(m_dataPrivate);

	prv_getPixelRGB(cvImage, x, y, &r, &g, &b);

    return new CColor(r, g, b);
}

//-----------------------------------------------------------------------

void CImg::getPixelRGB(unsigned long x, unsigned long y, unsigned char *r, unsigned char *g, unsigned char *b) const
{
    IplImage *cvImage;

    assert_no_null(m_dataPrivate);
    cvImage = PRV_PTRTOCV(m_dataPrivate);

    prv_getPixelRGB(cvImage, x, y, r, g, b);
}

//-----------------------------------------------------------------------

void CImg::getSpace3dRGB(unsigned long x, unsigned long y, double *fredPixel, double *fgreenPixel, double *fbluePixel) const
{
    IplImage *cvImage;
    unsigned char r, g, b;

    assert_no_null(m_dataPrivate);
    cvImage = PRV_PTRTOCV(m_dataPrivate);

    prv_getPixelRGB(cvImage, x, y, &r, &g, &b);
    CColor::transformColorRGB(r, g, b, fredPixel, fgreenPixel, fbluePixel);
}

//------------------------------------------------------------------------------
//
void CImg::setValue(unsigned long x, unsigned long y, const class CColor *color)
{
    IplImage *cvImage;
	unsigned char r, g, b;

    assert_no_null(m_dataPrivate);
	assert_no_null(color);

    cvImage = PRV_PTRTOCV(m_dataPrivate);

    color->getColor(&r, &g, &b);
    prv_setPixelRGB(cvImage, x, y, r, g, b);
}

//------------------------------------------------------------------------------
//
void CImg::setValueRGB(unsigned long x, unsigned long y, unsigned char r, unsigned char g, unsigned char b)
{
    IplImage *cvImage;

    assert_no_null(m_dataPrivate);
    cvImage = PRV_PTRTOCV(m_dataPrivate);
    prv_setPixelRGB(cvImage, x, y, r, g, b);
}

//-----------------------------------------------------------------------

void CImg::processAllPixels(class IProcessPixel *processOnePixel) const
{
    IplImage *cvImage;

    assert_no_null(m_dataPrivate);
    assert_no_null(processOnePixel);

    cvImage = PRV_PTRTOCV(m_dataPrivate);

    for (unsigned long y = 0; y < (unsigned long)cvImage->height; y++)
        for (unsigned long x = 0; x < (unsigned long)cvImage->width; x++)
            processOnePixel->processPixel(this, x, y);
}

//-----------------------------------------------------------------------

bool CImg::isPosition(unsigned long x, unsigned long y) const
{
    IplImage *cvImage;

    assert_no_null(m_dataPrivate);
    cvImage = PRV_PTRTOCV(m_dataPrivate);

    if (y >= (unsigned long)cvImage->height)
        return false;

    if (x >= (unsigned long)cvImage->width)
        return false;

    return true;
}

//-----------------------------------------------------------------------

bool CImg::isPixelWithColor(
                        unsigned long xPixel, unsigned long yPixel,
                        double fred, double fgreen, double fblue,
                        double tolerance) const
{
    IplImage *cvImage;
    unsigned char r, g, b;
    double frPixel, fgPixel, fbPixel;

    assert_no_null(m_dataPrivate);
    cvImage = PRV_PTRTOCV(m_dataPrivate);

    prv_getPixelRGB(cvImage, xPixel, yPixel, &r, &g, &b);
    CColor::transformColorRGB(r, g, b, &frPixel, &fgPixel, &fbPixel);
    return CColor::isSameColorWithTolerance(fred, fgreen, fblue, frPixel, fgPixel, fbPixel, tolerance);
}

//------------------------------------------------------------------------------
//
class CImg *CImg::transformToRGB(void) const
{
    struct prv_dataPrivateImage_t *dataNewImage;
    IplImage *cvImage;
	IplImage *destImgRGB;

    assert_no_null(m_dataPrivate);
    cvImage = PRV_PTRTOCV(m_dataPrivate);

	destImgRGB = cvCreateImage(cvSize(cvImage->width, cvImage->height), IPL_DEPTH_8U, 3);
	cvConvertImage(cvImage, destImgRGB);

	dataNewImage = PRV_PTRTOPRIVATE(destImgRGB);
	return new CImg(&dataNewImage);
}

//------------------------------------------------------------------------------
//
class CImg *CImg::transformToGray(void) const
{
    struct prv_dataPrivateImage_t *dataNewImage;
    IplImage *cvImage;
	IplImage *destImgGray;

    assert_no_null(m_dataPrivate);
    cvImage = PRV_PTRTOCV(m_dataPrivate);

	destImgGray = cvCreateImage(cvSize(cvImage->width, cvImage->height), IPL_DEPTH_8U, 1);
	cvCvtColor(cvImage, destImgGray,CV_BGR2GRAY);

    dataNewImage = PRV_PTRTOPRIVATE(destImgGray);
	return new CImg(&dataNewImage);
}

//------------------------------------------------------------------------------
//
class CImg *CImg::thresholdBinaryBW(unsigned char threshold) const
{
    struct prv_dataPrivateImage_t *dataNewImage;
    IplImage *cvImage;
	IplImage *destImgBW;
	unsigned char valueBinary;

    assert_no_null(m_dataPrivate);
    cvImage = PRV_PTRTOCV(m_dataPrivate);
	assert(cvImage->nChannels == 1);

	destImgBW = cvCreateImage(cvSize(cvImage->width, cvImage->height), IPL_DEPTH_8U, 1);
	valueBinary = 255;
    cvThreshold(cvImage, destImgBW, threshold, valueBinary, CV_THRESH_BINARY);

    dataNewImage = PRV_PTRTOPRIVATE(destImgBW);
	return new CImg(&dataNewImage);
}

//------------------------------------------------------------------------------
//
class CImg *CImg::scale(unsigned long newWidth, unsigned long newHeight)
{
    struct prv_dataPrivateImage_t *dataNewImage;
    IplImage *cvImage;
    IplImage *imageScaled;

    assert_no_null(m_dataPrivate);
    cvImage = PRV_PTRTOCV(m_dataPrivate);

	imageScaled = cvCloneImage(cvImage);
    cvResize(cvImage, imageScaled, CV_INTER_LINEAR);

    dataNewImage = PRV_PTRTOPRIVATE(imageScaled);
    return new CImg(&dataNewImage);
}

//-----------------------------------------------------------------------
//
class CImg *CImg::dilate3x3(unsigned long iterations)
{
    struct prv_dataPrivateImage_t *dataNewImage;
    IplImage *cvImage;
    IplImage *imageDilated;

    assert_no_null(m_dataPrivate);
    cvImage = PRV_PTRTOCV(m_dataPrivate);

	imageDilated = cvCloneImage(cvImage);
    cvDilate(cvImage, imageDilated, NULL, iterations);

    dataNewImage = PRV_PTRTOPRIVATE(imageDilated);
    return new CImg(&dataNewImage);
}

//-----------------------------------------------------------------------
//
class CImg *CImg::erode3x3(unsigned long iterations)
{
    struct prv_dataPrivateImage_t *dataNewImage;
    IplImage *cvImage;
    IplImage *imageEroded;

    assert_no_null(m_dataPrivate);
    cvImage = PRV_PTRTOCV(m_dataPrivate);

	imageEroded = cvCloneImage(cvImage);
    cvErode(cvImage, imageEroded, NULL, iterations);

    dataNewImage = PRV_PTRTOPRIVATE(imageEroded);
    return new CImg(&dataNewImage);
}

//-----------------------------------------------------------------------
//
class CImg *CImg::canny( double threshold1, double threshold2, unsigned int apertureSize)
{
    struct prv_dataPrivateImage_t *dataNewImage;
    IplImage *cvImage;
    IplImage *imageCanny;

    assert_no_null(m_dataPrivate);
    cvImage = PRV_PTRTOCV(m_dataPrivate);

	imageCanny = cvCloneImage(cvImage);
	cvCanny(cvImage, imageCanny, threshold1, threshold2, apertureSize);

    dataNewImage = PRV_PTRTOPRIVATE(imageCanny);
    return new CImg(&dataNewImage);
}

//-----------------------------------------------------------------------
//
class CImg *CImg::sobel(unsigned long derivateOrderX, unsigned long derivateOrderY, unsigned long apertureSize)
{
    struct prv_dataPrivateImage_t *dataNewImage;
    IplImage *cvImage;
    IplImage *imageSobeledFloat, *imageSobeledScaled;

    assert_no_null(m_dataPrivate);
    assert(apertureSize == 1 || apertureSize == 3 || apertureSize == 5 || apertureSize == 7);

    cvImage = PRV_PTRTOCV(m_dataPrivate);

    imageSobeledFloat = cvCreateImage(cvGetSize(cvImage), IPL_DEPTH_32F, cvImage->nChannels);
    imageSobeledScaled = cvCloneImage(cvImage);

    cvSobel(cvImage, imageSobeledFloat, derivateOrderX, derivateOrderY, apertureSize);
    cvConvertScale(imageSobeledFloat, imageSobeledScaled);

    cvReleaseImage(&imageSobeledFloat);

    dataNewImage = PRV_PTRTOPRIVATE(imageSobeledScaled);
    return new CImg(&dataNewImage);
}

//-----------------------------------------------------------------------
//
class CImg *CImg::laplace(unsigned long apertureSize)
{
    struct prv_dataPrivateImage_t *dataNewImage;
    IplImage *cvImage;
    IplImage *imageLaplaced, *imageLaplacedScaled;

    assert_no_null(m_dataPrivate);
    cvImage = PRV_PTRTOCV(m_dataPrivate);

    imageLaplaced = cvCreateImage(cvGetSize(cvImage), IPL_DEPTH_32F, cvImage->nChannels);
    imageLaplacedScaled = cvCloneImage(cvImage);

    cvLaplace(cvImage, imageLaplaced, apertureSize);
    cvConvertScale(imageLaplaced, imageLaplacedScaled);

    cvReleaseImage(&imageLaplaced);

    dataNewImage = PRV_PTRTOPRIVATE(imageLaplacedScaled);
    return new CImg(&dataNewImage);
}

//-----------------------------------------------------------------------
//
void CImg::drawImageOnImage(long x, long y, const class CImg *image)
{
    IplImage *cvSource, *cvDest;

    assert_no_null(m_dataPrivate);
    assert_no_null(image);
    assert_no_null(image->m_dataPrivate);

    cvDest = PRV_PTRTOCV(m_dataPrivate);
    cvSource = PRV_PTRTOCV(image->m_dataPrivate);

    assert(cvSource->nChannels == cvDest->nChannels);

    cvSetImageROI(cvDest, cvRect(x, y, cvSource->width, cvSource->height));

	cvCopy(cvSource, cvDest, NULL);

    cvResetImageROI(cvDest);
}

//-----------------------------------------------------------------------
//
void CImg::getDataRaw(unsigned long *width, unsigned long *height, unsigned long *widthStep, unsigned long *nChannels, unsigned char **buffer) const
{
    IplImage *cvImage;

    assert_no_null(m_dataPrivate);
    assert_no_null(width);
    assert_no_null(height);
    assert_no_null(widthStep);
    assert_no_null(nChannels);
    assert_no_null(buffer);

    cvImage = PRV_PTRTOCV(m_dataPrivate);

    *width = cvImage->width;
    *height = cvImage->height;
    *nChannels = cvImage->nChannels;
    *widthStep = cvImage->widthStep;
    *buffer = (unsigned char *)cvImage->imageData;
}
