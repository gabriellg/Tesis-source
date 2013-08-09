/* -----------------------------------------------------------------------------
 *  CGraphicsSprite.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CGraphicsSprite.hpp"

#include "asrtbas.h"
#include "memory.h"

#include "CImg.hpp"
#include "CArraysTypes.hpp"
#include "CStackTransformation.hpp"

struct SPrvDataPrivateGraphicsSprite
{
    bool originCartesian;

    double angleCurrent;
    class CArrayDouble *stackAngle;

    class CImg *image;
    class CStackTransformation *stackTransforms;
};

//-----------------------------------------------------------------------

static void prv_integrity(struct SPrvDataPrivateGraphicsSprite *dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(dataPrivate->stackTransforms);
    assert_no_null(dataPrivate->stackAngle);
}

//-----------------------------------------------------------------------

static struct SPrvDataPrivateGraphicsSprite *prv_create(
                    bool originCartesian,
                    double angleCurrent,
                    class CArrayDouble **stackAngle,
                    class CImg **image,
                    class CStackTransformation **stackTransforms)
{
    struct SPrvDataPrivateGraphicsSprite *dataPrivate;

    dataPrivate = new SPrvDataPrivateGraphicsSprite;

    dataPrivate->originCartesian = originCartesian;

    dataPrivate->angleCurrent = angleCurrent;
    dataPrivate->stackAngle = ASSIGN_PP_NO_NULL(stackAngle, class CArrayDouble);

    dataPrivate->image = ASSIGN_PP_NO_NULL(image, class CImg);
    dataPrivate->stackTransforms = ASSIGN_PP_NO_NULL(stackTransforms, class CStackTransformation);

    prv_integrity(dataPrivate);

    return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroy(struct SPrvDataPrivateGraphicsSprite **dataPrivate)
{
    assert_no_null(dataPrivate);
    prv_integrity(*dataPrivate);

    assert((*dataPrivate)->image == NULL);

    DELETE_OBJECT(&(*dataPrivate)->stackTransforms, class CStackTransformation);
    DELETE_OBJECT(&(*dataPrivate)->stackAngle, class CArrayDouble);
    DELETE_OBJECT(dataPrivate, struct SPrvDataPrivateGraphicsSprite);
}

//-----------------------------------------------------------------------

struct SPrvDataPrivateGraphicsSprite *prv_createDefault(class CImg **image)
{
    bool originCartesian;
    double angleCurrent;
    class CArrayDouble *stackAngle;
    class CStackTransformation *stackTransforms;

    originCartesian = false;
    angleCurrent = 0.;
    stackAngle = new CArrayDouble;
    stackTransforms = new CStackTransformation();

    return prv_create(originCartesian, angleCurrent, &stackAngle, image, &stackTransforms);
}

//-----------------------------------------------------------------------

CGraphicsSprite::CGraphicsSprite(unsigned long width, unsigned long height, unsigned char r, unsigned char g, unsigned char b)
{
    class CImg *image;

    image = new CImg(width, height, r, g, b);
    m_dataPrivate = prv_createDefault(&image);
}

//-----------------------------------------------------------------------

CGraphicsSprite::CGraphicsSprite(const class CImg *imageBackgroud)
{
    class CImg *image;

    image = new CImg(imageBackgroud);
    m_dataPrivate = prv_createDefault(&image);
}

//-----------------------------------------------------------------------

CGraphicsSprite::~CGraphicsSprite()
{
    prv_destroy(&m_dataPrivate);
}

//-----------------------------------------------------------------------

void CGraphicsSprite::setOriginCartesian(void)
{
    assert_no_null(m_dataPrivate);
    m_dataPrivate->originCartesian = true;
}

//-----------------------------------------------------------------------

void CGraphicsSprite::setOriginImage(void)
{
    assert_no_null(m_dataPrivate);
    m_dataPrivate->originCartesian = false;
}

//-----------------------------------------------------------------------

static void prv_currentPositionImage(
                    const class CStackTransformation *stackTransforms,
                    bool originCartesian,
                    const class CImg *image,
                    const class CImg *imageResult,
                    long *xPixel, long *yPixel)
{
    long xPixel_loc, yPixel_loc;
    double xCurrent, yCurrent, zCurrent;

    assert_no_null(stackTransforms);
    assert_no_null(xPixel);
    assert_no_null(yPixel);

    stackTransforms->transformationPoint3d(0., 0., 0., &xCurrent, &yCurrent, &zCurrent);
    assert(xCurrent < 100000);
    assert(yCurrent < 100000);
    assert(xCurrent > -100000);
    assert(yCurrent > -100000);

    xPixel_loc = (long)xCurrent;

    if (originCartesian == true)
    {
        unsigned long sizeX, sizeY;
        unsigned long sizeXImage, sizeYImage;

        imageResult->size(&sizeX, &sizeY);
        image->size(&sizeXImage, &sizeYImage);

        yPixel_loc = sizeY - (long)(yCurrent + sizeYImage);
    }
    else
        yPixel_loc = (long)yCurrent;

    *xPixel = xPixel_loc;
    *yPixel = yPixel_loc;
}

//-----------------------------------------------------------------------

static long prv_traslateJustificeHorizontal(
                const class CImg *image,
                enum CGraphicsSprite::EJustificeHorizontal_t justificeHorizontal)
{
    long dx;
    unsigned long sizeX, sizeY;

    assert_no_null(image);

    image->size(&sizeX, &sizeY);

    switch (justificeHorizontal)
    {
        case CGraphicsSprite::LEFT:
        {
            dx = 0;
            break;
        }
        case CGraphicsSprite::CENTER_HORIZONTAL:
        {
            dx = -(long)sizeX / 2;
            break;
        }
        case CGraphicsSprite::RIGHT:
        {
            dx = -(long)sizeX;
            break;
        }
        default_error();
    }

    return dx;
}

//-----------------------------------------------------------------------

static long prv_traslateJustificeVerticalOriginImage(
                const class CImg *image,
                enum CGraphicsSprite::EJustificeVertical_t justificeVertical)
{
    long dy;
    unsigned long sizeX, sizeY;

    assert_no_null(image);

    image->size(&sizeX, &sizeY);

    switch (justificeVertical)
    {
        case CGraphicsSprite::DOWN:
        {
            dy = -(long)sizeY;
            break;
        }
        case CGraphicsSprite::CENTER_VERTICAL:
        {
            dy = -(long)sizeY / 2;
            break;
        }
        case CGraphicsSprite::UP:
        {
            dy = 0;
            break;
        }
        default_error();
    }

    return dy;
}

//-----------------------------------------------------------------------

static long prv_traslateJustificeVerticalOriginCartesian(
                const class CImg *image,
                enum CGraphicsSprite::EJustificeVertical_t justificeVertical)
{
    long dy;
    unsigned long sizeX, sizeY;

    assert_no_null(image);

    image->size(&sizeX, &sizeY);

    switch (justificeVertical)
    {
        case CGraphicsSprite::DOWN:
        {
            dy = (long)sizeY;
            break;
        }
        case CGraphicsSprite::CENTER_VERTICAL:
        {
            dy = (long)sizeY / 2;
            break;
        }
        case CGraphicsSprite::UP:
        {
            dy = 0;
            break;
        }
        default_error();
    }

    return dy;
}

//-----------------------------------------------------------------------

static void prv_applyJustifice(
                const class CImg *image,
                enum CGraphicsSprite::EJustificeHorizontal_t justificeHorizontal,
                enum CGraphicsSprite::EJustificeVertical_t justificeVertical,
                long xPixel, long yPixel,
                long *xPixelWithJustifice, long *yPixelWithJustifice)
{
    unsigned long sizeX, sizeY;
    long xPixelWithJustifice_loc, yPixelWithJustifice_loc;

    assert_no_null(image);
    assert_no_null(xPixelWithJustifice);
    assert_no_null(yPixelWithJustifice);

    image->size(&sizeX, &sizeY);

    switch (justificeHorizontal)
    {
        case CGraphicsSprite::LEFT:
        {
            xPixelWithJustifice_loc = xPixel;
            break;
        }
        case CGraphicsSprite::CENTER_HORIZONTAL:
        {
            xPixelWithJustifice_loc = xPixel - sizeX / 2;
            break;
        }
        case CGraphicsSprite::RIGHT:
        {
            xPixelWithJustifice_loc = xPixel - sizeX;
            break;
        }
        default_error();
    }

    switch (justificeVertical)
    {
        case CGraphicsSprite::DOWN:
        {
            yPixelWithJustifice_loc = yPixel - sizeY;
            break;
        }
        case CGraphicsSprite::CENTER_VERTICAL:
        {
            yPixelWithJustifice_loc = yPixel - sizeY / 2;
            break;
        }
        case CGraphicsSprite::UP:
        {
            yPixelWithJustifice_loc = yPixel;
            break;
        }
        default_error();
    }

    *xPixelWithJustifice = xPixelWithJustifice_loc;
    *yPixelWithJustifice = yPixelWithJustifice_loc;
}

//-----------------------------------------------------------------------

static void prv_drawImage(
                const class CStackTransformation *stackTransforms,
                bool originCartesian,
                const class CImg *image,
                enum CGraphicsSprite::EJustificeHorizontal_t justificeHorizontal,
                enum CGraphicsSprite::EJustificeVertical_t justificeVertical,
                class CImg *imageResult)
{
    long xPixel, yPixel, dx, dy;
    long xPixelWithJustifice, yPixelWithJustifice;
    unsigned long sizeXImage, sizeYImage;

    image->size(&sizeXImage, &sizeYImage);

    prv_currentPositionImage(
                stackTransforms,
                originCartesian,
                image, imageResult,
                &xPixel, &yPixel);

    dx = prv_traslateJustificeHorizontal(image, justificeHorizontal);

    if (originCartesian == true)
        dy = prv_traslateJustificeVerticalOriginCartesian(image, justificeVertical);
    else
        dy = prv_traslateJustificeVerticalOriginImage(image, justificeVertical);

    prv_applyJustifice(
                image,
                justificeHorizontal, justificeVertical,
                xPixel, yPixel, &xPixelWithJustifice, &yPixelWithJustifice);

    imageResult->drawImageOnImage(xPixel + dx, yPixel + dy, image);
}

//-----------------------------------------------------------------------

void CGraphicsSprite::drawImage(const struct CImg *image)
{
    prv_integrity(m_dataPrivate);
    assert_no_null(m_dataPrivate->image);

    prv_drawImage(m_dataPrivate->stackTransforms,
                    m_dataPrivate->originCartesian,
                    image,
                    LEFT, UP,
                    m_dataPrivate->image);
}

//-----------------------------------------------------------------------

void CGraphicsSprite::drawImageWithJustifice(
                        const class CImg *image,
                        enum CGraphicsSprite::EJustificeHorizontal_t justificeHorizontal,
                        enum CGraphicsSprite::EJustificeVertical_t justificeVertical)
{
    prv_integrity(m_dataPrivate);
    assert_no_null(m_dataPrivate->image);

    prv_drawImage(m_dataPrivate->stackTransforms,
                    m_dataPrivate->originCartesian,
                    image, justificeHorizontal, justificeVertical, m_dataPrivate->image);
}

//-----------------------------------------------------------------------

void CGraphicsSprite::translate(double dx, double dy)
{
    prv_integrity(m_dataPrivate);
    m_dataPrivate->stackTransforms->appendTranslation(dx, dy, 0.);
}

//-----------------------------------------------------------------------

void CGraphicsSprite::scale(double sx, double sy)
{
    prv_integrity(m_dataPrivate);
    m_dataPrivate->stackTransforms->appendScale(sx, sy, 1.);
}

//-----------------------------------------------------------------------

void CGraphicsSprite::rotate(double angle)
{
    m_dataPrivate->angleCurrent += angle;
}

//-----------------------------------------------------------------------

void CGraphicsSprite::pushTransformation(void)
{
    prv_integrity(m_dataPrivate);
    m_dataPrivate->stackAngle->add(m_dataPrivate->angleCurrent);
    m_dataPrivate->stackTransforms->pushTransformation();
}

//-----------------------------------------------------------------------

void CGraphicsSprite::popTransformation(void)
{
    prv_integrity(m_dataPrivate);
    m_dataPrivate->stackTransforms->popTransformation();
    m_dataPrivate->angleCurrent = m_dataPrivate->stackAngle->getLast();
    m_dataPrivate->stackAngle->eraseLast();
}

//-----------------------------------------------------------------------

class CImg *CGraphicsSprite::getImage(void)
{
    class CImg *imageResult;

    prv_integrity(m_dataPrivate);

    imageResult = m_dataPrivate->image;
    m_dataPrivate->image = NULL;

    return imageResult;
}

//-----------------------------------------------------------------------

void CGraphicsSprite::getRotate(double *angle) const
{
    prv_integrity(m_dataPrivate);
    assert_no_null(m_dataPrivate);

    *angle = m_dataPrivate->angleCurrent;
}

