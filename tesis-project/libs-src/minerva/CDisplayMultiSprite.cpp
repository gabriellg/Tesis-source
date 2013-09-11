/* -----------------------------------------------------------------------------
 *  CDisplayMultiSprite.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CDisplayMultiSprite.hpp"

#include "asrtbas.h"
#include "memory.h"

#include "IGraphics.hpp"
#include "CArray.hpp"
#include "CArrPixel.hpp"

struct prv_dataPrivateDisplayMultiSprite_t
{
    enum CDisplayMultiSprite::ETypeDistribution typeDistribution;

    class CArray<IDisplay> *displays;
    class CArrPixel *positionsWindows;
};

//-----------------------------------------------------------------------

static void prv_integrity(const struct prv_dataPrivateDisplayMultiSprite_t *data)
{
    assert_no_null(data);
    assert_no_null(data->displays);
}

//-----------------------------------------------------------------------

static struct prv_dataPrivateDisplayMultiSprite_t *prv_createDisplayMultiSprite(
                        enum CDisplayMultiSprite::ETypeDistribution typeDistribution,
                        class CArray<IDisplay> **displays,
                        class CArrPixel **positionsWindows)
{
    struct prv_dataPrivateDisplayMultiSprite_t *dataPrivate;

    dataPrivate = MALLOC(struct prv_dataPrivateDisplayMultiSprite_t);

    dataPrivate->typeDistribution = typeDistribution;
    dataPrivate->displays = ASSIGN_PP_NO_NULL(displays, class CArray<IDisplay>);
    dataPrivate->positionsWindows = ASSIGN_PP(positionsWindows, class CArrPixel);

    prv_integrity(dataPrivate);

    return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroyDisplayMultiSprite(struct prv_dataPrivateDisplayMultiSprite_t **dataPrivate)
{
    assert_no_null(dataPrivate);
    prv_integrity(*dataPrivate);

    delete (*dataPrivate)->displays;

    if ((*dataPrivate)->positionsWindows != NULL)
        delete (*dataPrivate)->positionsWindows;

    FREE_T(dataPrivate, struct prv_dataPrivateDisplayMultiSprite_t);
}

//-----------------------------------------------------------------------

CDisplayMultiSprite::CDisplayMultiSprite(enum CDisplayMultiSprite::ETypeDistribution typeDistribution)
{
    class CArray<IDisplay> *displays;
    class CArrPixel *positionsWindows;

    displays = new class CArray<IDisplay>;
    positionsWindows = NULL;

    m_dataPrivate = prv_createDisplayMultiSprite(typeDistribution, &displays, &positionsWindows);
}

//-----------------------------------------------------------------------

CDisplayMultiSprite::~CDisplayMultiSprite()
{
    prv_destroyDisplayMultiSprite(&m_dataPrivate);
}

//-----------------------------------------------------------------------

void CDisplayMultiSprite::appendDisplaySprite(class IDisplay **display)
{
    prv_integrity(m_dataPrivate);

    m_dataPrivate->displays->add(*display);
    *display = NULL;
}

//-----------------------------------------------------------------------

void CDisplayMultiSprite::setInitialPositionCamera(void)
{
    unsigned long num;

    prv_integrity(m_dataPrivate);

    num = m_dataPrivate->displays->size();

    for (unsigned long i = 0; i < num; i++)
    {
        class IDisplay *display;

        display = m_dataPrivate->displays->get(i);
        assert_no_null(display);

        display->setInitialPositionCamera();
    }
}

//-----------------------------------------------------------------------

void CDisplayMultiSprite::positionCamera(class IGraphics *graphics) const
{
    unsigned long num;

    prv_integrity(m_dataPrivate);

    num = m_dataPrivate->displays->size();

    for (unsigned long i = 0; i < num; i++)
    {
        class IDisplay *display;

        display = m_dataPrivate->displays->get(i);
        assert_no_null(display);

        display->positionCamera(graphics);
    }
}

//-----------------------------------------------------------------------

void CDisplayMultiSprite::makeRotationCamera(class IGraphics *graphics) const
{
    unsigned long num;

    prv_integrity(m_dataPrivate);

    num = m_dataPrivate->displays->size();

    for (unsigned long i = 0; i < num; i++)
    {
        const class IDisplay *display;

        display = m_dataPrivate->displays->get(i);
        assert_no_null(display);

        display->makeRotationCamera(graphics);
    }
}

//-----------------------------------------------------------------------

void CDisplayMultiSprite::setRotationCamera(
                        struct areaDibujo_t *areaDibujo,
                        double rotXDegrees, double rotYDegrees, double rotZDegrees)
{
    unsigned long num;

    prv_integrity(m_dataPrivate);

    num = m_dataPrivate->displays->size();

    for (unsigned long i = 0; i < num; i++)
    {
        class IDisplay *display;

        display = m_dataPrivate->displays->get(i);
        assert_no_null(display);

        display->setRotationCamera(areaDibujo, rotXDegrees, rotYDegrees, rotZDegrees);
    }
}

//-----------------------------------------------------------------------

void CDisplayMultiSprite::incrRotateCamera(struct areaDibujo_t *areaDibujo, double incrRotateX, double incrRotateY, double incrRotateZ)
{
    unsigned long num;

    prv_integrity(m_dataPrivate);

    num = m_dataPrivate->displays->size();

    for (unsigned long i = 0; i < num; i++)
    {
        class IDisplay *display;

        display = m_dataPrivate->displays->get(i);
        assert_no_null(display);

        display->incrRotateCamera(areaDibujo, incrRotateX, incrRotateY, incrRotateZ);
    }
}

//-----------------------------------------------------------------------

void CDisplayMultiSprite::frontCamera(struct areaDibujo_t *areaDibujo, double step)
{
    unsigned long num;

    prv_integrity(m_dataPrivate);

    num = m_dataPrivate->displays->size();

    for (unsigned long i = 0; i < num; i++)
    {
        class IDisplay *display;

        display = m_dataPrivate->displays->get(i);
        assert_no_null(display);

        display->frontCamera(areaDibujo, step);
    }
}

//-----------------------------------------------------------------------

void CDisplayMultiSprite::backCamera(struct areaDibujo_t *areaDibujo, double step)
{
    unsigned long num;

    prv_integrity(m_dataPrivate);

    num = m_dataPrivate->displays->size();

    for (unsigned long i = 0; i < num; i++)
    {
        class IDisplay *display;

        display = m_dataPrivate->displays->get(i);
        assert_no_null(display);

        display->backCamera(areaDibujo, step);
    }
}

//-----------------------------------------------------------------------

bool CDisplayMultiSprite::isArea3D(void) const
{
    unsigned long num;

    prv_integrity(m_dataPrivate);

    num = m_dataPrivate->displays->size();

    for (unsigned long i = 0; i < num; i++)
    {
        const class IDisplay *display;

        display = m_dataPrivate->displays->get(i);
        assert_no_null(display);

        if (display->isArea3D() == true)
            return true;
    }

    return false;
}

//-----------------------------------------------------------------------

static class CArrPixel *prv_calculatePositionWindows(
                            enum CDisplayMultiSprite::ETypeDistribution typeDistribution,
                            const class CArray<IDisplay> *displays)
{
    class CArrPixel *positionsWindows;
    unsigned long numDisplay;
    unsigned long x, y;

    numDisplay = displays->size();

    positionsWindows = new CArrPixel(numDisplay);

    x = 0;
    y = 0;

    for (unsigned long i = 0; i < numDisplay; i++)
    {
        unsigned long width, height;
        class IDisplay *display;

        positionsWindows->set(i, (long)x, (long)y);

        display = displays->get(i);
        assert_no_null(display);

        if (display->withSize(&width, &height) == true)
        {
            switch(typeDistribution)
            {
                case CDisplayMultiSprite::HORIZONTAL:

                    x += width;
                    break;

                case CDisplayMultiSprite::VERTICAL:

                    y += height;
                    break;

                default_error();
            }
        }
    }

    return positionsWindows;
}

//-----------------------------------------------------------------------

void CDisplayMultiSprite::appendDescription(const char *symbol, class IDescription **description)
{
    assert_message("It mustn't be used");
}

//-----------------------------------------------------------------------

void CDisplayMultiSprite::drawScene(class IGraphics *graphics, const class CScene *scene)
{
    unsigned long numDisplay;

    prv_integrity(m_dataPrivate);

    if (m_dataPrivate->positionsWindows == NULL)
        m_dataPrivate->positionsWindows = prv_calculatePositionWindows(m_dataPrivate->typeDistribution, m_dataPrivate->displays);

    numDisplay = m_dataPrivate->displays->size();
    assert(numDisplay == m_dataPrivate->positionsWindows->size());

    for (unsigned long i = 0; i < numDisplay; i++)
    {
        class IDisplay *display;
        long xDisplay, yDisplay;

        display = m_dataPrivate->displays->get(i);
        m_dataPrivate->positionsWindows->get(i, &xDisplay, &yDisplay);

        graphics->pushTransformation();
        graphics->traslation(xDisplay, yDisplay, 0);

        display->drawScene(graphics, scene);

        graphics->popTransformation();
    }
}
