/* -----------------------------------------------------------------------------
 *  CMatrixData.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CMatrixData.hpp"

#include "asrtbas.h"
#include "memory.h"

struct prv_dataPrivateMatrixData_t
{
    unsigned long width, heigth;
    struct prv_itemDataMatrix_t ***matrixData;

    CMatrixData_FPtr_destroyElement func_destroyElement;
};

//-----------------------------------------------------------------------

static void prv_integrity(const struct prv_dataPrivateMatrixData_t *dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(dataPrivate->matrixData);
}

//-----------------------------------------------------------------------

static struct prv_itemDataMatrix_t ***prv_createMatrix(unsigned long width, unsigned long heigth)
{
    struct prv_itemDataMatrix_t ***matrixData;
    unsigned long x, y;

    matrixData = CALLOC(heigth, struct prv_itemDataMatrix_t **);

    for (y = 0; y < heigth; y++)
    {
        matrixData[y] = CALLOC(width, struct prv_itemDataMatrix_t *);

        for (x = 0; x < width; x++)
            matrixData[y][x] = NULL;
    }

    return matrixData;
}

//-----------------------------------------------------------------------

static struct prv_dataPrivateMatrixData_t *prv_createMatrixData(
                        unsigned long width, unsigned long heigth,
                        struct prv_itemDataMatrix_t ****matrixData,
                        CMatrixData_FPtr_destroyElement func_destroyElement)
{
    struct prv_dataPrivateMatrixData_t *dataPrivate;

    dataPrivate = MALLOC(struct prv_dataPrivateMatrixData_t);

    dataPrivate->width = width;
    dataPrivate->heigth = heigth;
    dataPrivate->matrixData = ASSIGN_PP_NO_NULL(matrixData, struct prv_itemDataMatrix_t **);

    dataPrivate->func_destroyElement = func_destroyElement;

    prv_integrity(dataPrivate);

    return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroyMatrixData(struct prv_dataPrivateMatrixData_t **dataPrivate)
{
    assert_no_null(dataPrivate);
    prv_integrity(*dataPrivate);

    for (unsigned long y = 0; y < (*dataPrivate)->heigth; y++)
    {
        if ((*dataPrivate)->func_destroyElement != NULL)
        {
            for (unsigned long x = 0; x < (*dataPrivate)->width; x++)
            {
                if ((*dataPrivate)->matrixData[y][x] != NULL)
                    (*dataPrivate)->func_destroyElement(&((*dataPrivate)->matrixData[y][x]));
            }
        }

        FREE_T(&((*dataPrivate)->matrixData[y]), struct prv_itemDataMatrix_t *);
    }

    FREE_T(&((*dataPrivate)->matrixData), struct prv_itemDataMatrix_t **);

    FREE_T(dataPrivate, struct prv_dataPrivateMatrixData_t);
}

//-----------------------------------------------------------------------

PrvNotUseMatrixData::PrvNotUseMatrixData(unsigned long width, unsigned long heigth, CMatrixData_FPtr_destroyElement func_destroyElement)
{
    struct prv_itemDataMatrix_t ***matrixData;

    matrixData = prv_createMatrix(width, heigth);
    m_dataPrivate = prv_createMatrixData(width, heigth, &matrixData, func_destroyElement);
}

//-----------------------------------------------------------------------

PrvNotUseMatrixData::~PrvNotUseMatrixData()
{
    prv_destroyMatrixData(&m_dataPrivate);
}

//-----------------------------------------------------------------------

bool PrvNotUseMatrixData::hasData(unsigned long row, unsigned long col) const
{
    prv_integrity(m_dataPrivate);

    if (m_dataPrivate->matrixData[row][col] != NULL)
        return true;
    else
        return false;
}

//-----------------------------------------------------------------------

void PrvNotUseMatrixData::size(unsigned long *width, unsigned long *heigth) const
{
    prv_integrity(m_dataPrivate);
    assert_no_null(width);
    assert_no_null(heigth);

    *width = m_dataPrivate->width;
    *heigth = m_dataPrivate->heigth;
}

//-----------------------------------------------------------------------

struct prv_itemDataMatrix_t *PrvNotUseMatrixData::prv_getData(unsigned long row, unsigned long col)
{
    prv_integrity(m_dataPrivate);
    assert(col < m_dataPrivate->width);
    assert(row < m_dataPrivate->heigth);

    return m_dataPrivate->matrixData[row][col];
}

//-----------------------------------------------------------------------

const struct prv_itemDataMatrix_t *PrvNotUseMatrixData::prv_getDataConst(unsigned long row, unsigned long col) const
{
    prv_integrity(m_dataPrivate);
    assert(col < m_dataPrivate->width);
    assert(row < m_dataPrivate->heigth);

    return m_dataPrivate->matrixData[row][col];
}

//-----------------------------------------------------------------------

void PrvNotUseMatrixData::prv_setData(unsigned long row, unsigned long col, struct prv_itemDataMatrix_t *data)
{
    prv_integrity(m_dataPrivate);
    assert(col < m_dataPrivate->width);
    assert(row < m_dataPrivate->heigth);

    if (m_dataPrivate->matrixData[row][col] != NULL && m_dataPrivate->func_destroyElement != NULL)
        m_dataPrivate->func_destroyElement(&m_dataPrivate->matrixData[row][col]);

    m_dataPrivate->matrixData[row][col] = data;
}
