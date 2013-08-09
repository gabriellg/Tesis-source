/* -----------------------------------------------------------------------------
 *  CMemoryRobot.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CMemoryRobot.inl"

#include "CConstants.inl"
#include "CDisplaySimulationRobot.inl"
#include "CTransformRobot2D.inl"

#include "asrtbas.h"
#include "memory.h"

#include "CMultiObject3d.hpp"
#include "CMatrixData.hpp"
#include "CString.hpp"
#include "CMath.hpp"
#include "CImg.hpp"
#include "CFigure.hpp"
#include "CTransform.hpp"
#include "CDescriptionSpriteWithDataFigure.hpp"

struct PrvDataMemory
{
    PrvDataMemory(const char *mark, bool obstacle);
    ~PrvDataMemory(void);
    char *m_mark;
    bool m_obstacle;
};

struct prv_dataPrivateMemoryRobot_t
{
    class CMatrixData<PrvDataMemory> *memory;
    class CImg *imageMemory;
    unsigned long widthRobot, heightRobot;
    long xRobot, yRobot;
    double angleRobot;
};

static unsigned long PRV_SIZE_X_MEMORY_IMAGE = 2;
static unsigned long PRV_SIZE_Y_MEMORY_IMAGE = 2;

//-----------------------------------------------------------------------

PrvDataMemory::PrvDataMemory(const char *mark, bool obstacle)
{
    m_mark = CString::copy(mark);
    m_obstacle = obstacle;
}

//-----------------------------------------------------------------------

PrvDataMemory::~PrvDataMemory(void)
{
    CString::free(&m_mark);
}

//-----------------------------------------------------------------------

static void prv_integriy(const struct prv_dataPrivateMemoryRobot_t *dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(dataPrivate->memory);
}

//-----------------------------------------------------------------------

static struct prv_dataPrivateMemoryRobot_t *prv_createMemoryRobot(
                    class CMatrixData<PrvDataMemory> **memory,
                    class CImg **imageMemory,
                    unsigned long widthRobot, unsigned long heightRobot,
                    long xRobot, long yRobot, double angleRobot)
{
    struct prv_dataPrivateMemoryRobot_t *dataPrivate;

    dataPrivate = MALLOC(struct prv_dataPrivateMemoryRobot_t);

    dataPrivate->memory = ASSIGN_PP_NO_NULL(memory, class CMatrixData<PrvDataMemory>);
    dataPrivate->imageMemory = ASSIGN_PP_NO_NULL(imageMemory, class CImg);

    dataPrivate->widthRobot = widthRobot;
    dataPrivate->heightRobot = heightRobot;

    dataPrivate->xRobot = xRobot;
    dataPrivate->yRobot = yRobot;

    dataPrivate->angleRobot = angleRobot;

    return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroyMemoryRobot(struct prv_dataPrivateMemoryRobot_t **dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(*dataPrivate);

    delete (*dataPrivate)->memory;
    delete (*dataPrivate)->imageMemory;

    FREE_T(dataPrivate, struct prv_dataPrivateMemoryRobot_t);
}

//-----------------------------------------------------------------------

static class CImg *prv_createImageMemory(unsigned long widthMemory, unsigned long heightMemory)
{
    unsigned long sizeXImageMemory, sizeYImageMemory;

    sizeXImageMemory = PRV_SIZE_X_MEMORY_IMAGE * widthMemory;
    sizeYImageMemory = PRV_SIZE_Y_MEMORY_IMAGE * heightMemory;

    return new CImg(sizeXImageMemory, sizeYImageMemory, 128, 128, 128);
}

//-----------------------------------------------------------------------

static void prv_markRGB(class CImg *imageMemory, long xCell, long yCell, unsigned char r, unsigned char g, unsigned char b)
{
    class CImg *imageMark;

    imageMark = new CImg(PRV_SIZE_X_MEMORY_IMAGE, PRV_SIZE_Y_MEMORY_IMAGE, r, g, b);
    imageMemory->drawImageOnImage(xCell, yCell, imageMark);
    delete imageMark;
}

//-----------------------------------------------------------------------

CMemoryRobot::CMemoryRobot(
                    unsigned long widthMemory, unsigned long heightMemory,
                    unsigned long widthRobot, unsigned long heightRobot,
                    long xRobot, long yRobot,
                    double angleRobot)
{
    class CMatrixData<PrvDataMemory> *memory;
    class CImg *imageMemory;

    memory = new CMatrixData<PrvDataMemory>(widthMemory, heightMemory);
    imageMemory = prv_createImageMemory(widthMemory, heightMemory);

    m_dataPrivate = prv_createMemoryRobot(&memory, &imageMemory, widthRobot, heightRobot, xRobot, yRobot, angleRobot);
}

//-----------------------------------------------------------------------

CMemoryRobot::~CMemoryRobot()
{
    prv_destroyMemoryRobot(&m_dataPrivate);
}

//-----------------------------------------------------------------------

static struct PrvDataMemory *prv_dataGetMemory(
                    class CMatrixData<PrvDataMemory> *memory,
                    long xCell, long yCell)
{
    struct PrvDataMemory *dataMemory;
    unsigned long widthMemory, heigthMemory;
    unsigned long xCellU, yCellU;

    assert_no_null(memory);

    memory->size(&widthMemory, &heigthMemory);

    //TODO: Mirar desplazamiento de la memoria;
    assert_no_null(memory);
    assert(xCell >= 0);
    assert(yCell >= 0);

    xCellU = (unsigned long)xCell;
    yCellU = (unsigned long)yCell;

    assert(xCellU < widthMemory);
    assert(yCellU < heigthMemory);

    if (memory->hasData(xCellU, yCellU) == true)
        dataMemory = memory->get(xCellU, yCellU);
    else
    {
        dataMemory = new PrvDataMemory("", false);
        memory->set(xCellU, yCellU, dataMemory);
    }

    return dataMemory;
}

//-----------------------------------------------------------------------

static void prv_move(long xRobot, long yRobot, double angle, double angleRobot, double distance, long *xCell, long *yCell)
{
    double x, y;

    assert_no_null(xCell);
    assert_no_null(yCell);

    x = xRobot + distance * CMath::cosine(angle + angleRobot);
    y = yRobot - distance * CMath::sine(angle + angleRobot);

    *xCell = CMath::round(x);
    *yCell = CMath::round(y);
}

//-----------------------------------------------------------------------

void CMemoryRobot::saveInMemoryLaser(double distance, double angle)
{
    long xCell, yCell;
    struct PrvDataMemory *dataMemory;

    prv_integriy(m_dataPrivate);

    prv_move(m_dataPrivate->xRobot, m_dataPrivate->yRobot, angle, m_dataPrivate->angleRobot, distance, &xCell, &yCell);

    dataMemory = prv_dataGetMemory(m_dataPrivate->memory, xCell, yCell);
    dataMemory->m_obstacle = true;
    prv_markRGB(m_dataPrivate->imageMemory, xCell, yCell, 255, 0, 0);
}

//-----------------------------------------------------------------------

static void prv_markLabel(class CImg *imageMemory, long xCell, long yCell, const char *mark)
{
    unsigned char r, g, b;

    if (CString::equal(mark, CConstants::MARK_OBJECTIVE1) == true)
    {
        r = 0; g = 255; b = 0;
    }
    else if (CString::equal(mark, CConstants::MARK_OBJECTIVE2) == true)
    {
        r = 0; g = 0; b = 255;
    }
    else if (CString::equal(mark, CConstants::MARK_OBJECTIVE3) == true)
    {
        r = 255; g = 255; b = 0;
    }
    else if (CString::equal(mark, CConstants::MARK_OBJECTIVE4) == true)
    {
        r = 255; g = 255; b = 255;
    }
    else
    {
        assert(CString::equal(mark, CConstants::MARK_EMPTY) == true);
        r = 128; g = 128; b = 128;
    }

    prv_markRGB(imageMemory, xCell, yCell, r, g, b);
}

//-----------------------------------------------------------------------

void CMemoryRobot::saveInMemoryMarkVideo(const char *mark)
{
    struct PrvDataMemory *dataMemory;

    prv_integriy(m_dataPrivate);

    dataMemory = prv_dataGetMemory(m_dataPrivate->memory, m_dataPrivate->xRobot, m_dataPrivate->yRobot);
    CString::free(&dataMemory->m_mark);

    dataMemory->m_mark = CString::copy(mark);
    prv_markLabel(m_dataPrivate->imageMemory, m_dataPrivate->xRobot, m_dataPrivate->yRobot, mark);
}

//-----------------------------------------------------------------------

static class CAgent *prv_decide(double dangle)
{
    class CAgent *figureRobot, *rotateRobot;

    figureRobot = new CFigure(CDisplaySimulationRobot::SYMBOL_ROBOT);
    rotateRobot = CTransformRobot::createRotateRobot(&figureRobot, dangle);
    return CTransformRobot::createTraslateRobot(&rotateRobot, 1., 0.);
}

//-----------------------------------------------------------------------

static double prv_initialAngle(void)
{
    int indRandom;

    /*indRandom = CMath::getRandomNumber(0, 3);*/
    indRandom = 0;

    return indRandom * CMath::PI / 2.;
}

//-----------------------------------------------------------------------

class CAgent *CMemoryRobot::decide(const char *markObjective)
{
    class CAgent *actions;

    prv_integriy(m_dataPrivate);

    if (CString::equal(markObjective, CConstants::MARK_EMPTY) == false)
    {
        unsigned long i, num;
        double dangle, distanceView, incrMoveRobot;

        dangle = prv_initialAngle();
        num = 4;
        distanceView = 1.;
        incrMoveRobot = 1.;

        for (i = 0; i < num; i++)
        {
            struct PrvDataMemory *dataMemory;
            long xCell, yCell;

            prv_move(m_dataPrivate->xRobot, m_dataPrivate->yRobot, dangle, m_dataPrivate->angleRobot, distanceView, &xCell, &yCell);
            dataMemory = prv_dataGetMemory(m_dataPrivate->memory, xCell, yCell);

            if (dataMemory->m_obstacle == false)
                break;

            dangle += CMath::PI / 2.;

            if (dangle > 2. * CMath::PI)
                dangle -= 2. * CMath::PI;
        }

        if (i != num)
        {
            actions = prv_decide(dangle);

            prv_move(
                   m_dataPrivate->xRobot, m_dataPrivate->yRobot, dangle,
                   m_dataPrivate->angleRobot, incrMoveRobot,
                   &m_dataPrivate->xRobot, &m_dataPrivate->yRobot);
            m_dataPrivate->angleRobot += dangle;
        }
        else
            actions = new CMultiObject3d;
    }
    else
        actions = new CMultiObject3d;

    return actions;
}

//-----------------------------------------------------------------------

class CFigure *CMemoryRobot::figureImageMemory(void) const
{
    class IDataSymbol *dataImageFigure;

    dataImageFigure = new CDataImageFigure(m_dataPrivate->imageMemory);
    return new CFigure(CDisplaySimulationRobot::SYMBOL_MEMORY, &dataImageFigure);
}
