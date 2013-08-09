/* -----------------------------------------------------------------------------
*  PrvBaseArray
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*          Gabriel López García (gabriellg AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include "PrvBaseArray.inl"

#include "asrtbas.h"
#include "memory.h"

struct prv_privateArray_t
{
    unsigned long numElements;
    struct prv_item_t **array;
    CArray_FPtr_destroyElement func_freeElement;
};

//------------------------------------------------------------------------------
//
static struct prv_item_t **prv_createArray(unsigned long numElements)
{
    struct prv_item_t **array;

    if (numElements > 0)
        array = CALLOC(numElements, struct prv_item_t *);
    else
        array = NULL;

    return array;
}

//------------------------------------------------------------------------------
//
static struct prv_item_t **prv_copyArray(
                        unsigned long numElements, struct prv_item_t **array,
                        CArray_FPtr_copyElement funcCopy)
{
    struct prv_item_t **arrayCopied;

    if (numElements > 0)
    {
        assert_no_null(array);

        arrayCopied = prv_createArray(numElements);

        for (unsigned long i = 0; i < numElements; ++i)
        {
            if (funcCopy != NULL)
                arrayCopied[i] = funcCopy(array[i]);
            else
                arrayCopied[i] = array[i];
        }
    }
    else
        arrayCopied = NULL;

    return arrayCopied;
}

//------------------------------------------------------------------------------
//
static void prv_integrity(const struct prv_privateArray_t *array)
{
    assert_no_null(array);
    //assert_no_null(array->func_freeElement);
}

//------------------------------------------------------------------------------
//
static struct prv_privateArray_t *prv_create(
                        unsigned long numElements,
                        struct prv_item_t ***array,
                        CArray_FPtr_destroyElement func_freeElement)
{
    struct prv_privateArray_t *data;

    data = MALLOC(struct prv_privateArray_t);

    data->numElements = numElements;
    data->array = ASSIGN_PP(array, struct prv_item_t *);

    data->func_freeElement = func_freeElement;

    prv_integrity(data);

    return data;
}

//------------------------------------------------------------------------------
//
static struct prv_privateArray_t *prv_copy(
                    const struct prv_privateArray_t *data,
                    CArray_FPtr_copyElement funcCopy)
{
    struct prv_item_t **arrayCopied;
    CArray_FPtr_destroyElement func_freeElement;

    prv_integrity(data);

    if (funcCopy != NULL)
    {
        assert_no_null(data->func_freeElement);
        func_freeElement = data->func_freeElement;
    }
    else
        func_freeElement = NULL;

    arrayCopied = prv_copyArray(data->numElements, data->array, funcCopy);

    return prv_create(data->numElements, &arrayCopied, func_freeElement);
}

//------------------------------------------------------------------------------
//
PrvBaseArray::PrvBaseArray(
                        unsigned long numElements,
                        CArray_FPtr_destroyElement destroyElement)
{
    struct prv_item_t **array;

    array = prv_createArray(numElements);
    m_data = prv_create(numElements, &array, destroyElement);
}

//------------------------------------------------------------------------------
//
PrvBaseArray::PrvBaseArray(const class PrvBaseArray *array, CArray_FPtr_copyElement funcCopy)
{
    assert_no_null(array);
    m_data = prv_copy(array->m_data, funcCopy);
}

//------------------------------------------------------------------------------
//
PrvBaseArray::PrvBaseArray(struct prv_privateArray_t **data)
{
    m_data = ASSIGN_PP_NO_NULL(data, struct prv_privateArray_t);
}

//------------------------------------------------------------------------------
//
class PrvBaseArray *PrvBaseArray::invert(CArray_FPtr_copyElement funcCopy) const
{
    struct prv_privateArray_t *dataInverted;
    unsigned long halfNumArray;

    dataInverted = prv_copy(m_data, funcCopy);

    halfNumArray = dataInverted->numElements / 2;

    for (unsigned long i = 0; i < halfNumArray; i++)
    {
        struct prv_item_t *swapTemp;
        unsigned long indHalfLast;

        indHalfLast = dataInverted->numElements - i - 1;

        swapTemp = dataInverted->array[i];
        dataInverted->array[i] = dataInverted->array[indHalfLast];
        dataInverted->array[indHalfLast] = swapTemp;
    }

    return new PrvBaseArray(&dataInverted);
}

//------------------------------------------------------------------------------
//
static void prv_releaseMemory(
                    unsigned long *numElements,
                    struct prv_item_t ***array,
                    CArray_FPtr_destroyElement func_freeElement)
{
    assert_no_null(array);
    assert_no_null(*array);
    assert_no_null(numElements);

    if( func_freeElement != NULL )
    {
        for (unsigned long i = 0; i < *numElements; i++)
            func_freeElement(&(*array)[i]);
    }

    *numElements = 0;
    FREE_T(array, struct prv_item_t *);
}

//------------------------------------------------------------------------------
//
PrvBaseArray::~PrvBaseArray()
{
    prv_integrity(m_data);

    if (m_data->numElements > 0)
        prv_releaseMemory(&m_data->numElements, &m_data->array, m_data->func_freeElement);

    FREE_T(&m_data, struct prv_privateArray_t);
}

//------------------------------------------------------------------------------
//
void PrvBaseArray::clear()
{
    prv_integrity(m_data);

    if (m_data->numElements > 0)
        prv_releaseMemory(&m_data->numElements, &m_data->array, m_data->func_freeElement);
}

//------------------------------------------------------------------------------
//
unsigned long PrvBaseArray::size() const
{
    prv_integrity(m_data);
    return m_data->numElements;
}

//------------------------------------------------------------------------------
//
struct prv_item_t* PrvBaseArray::get(unsigned long ind) const
{
    prv_integrity(m_data);
    assert(ind < m_data->numElements);

    return m_data->array[ind];
}

//------------------------------------------------------------------------------
//
struct prv_item_t* PrvBaseArray::getLast(void) const
{
    prv_integrity(m_data);
    assert(m_data->numElements > 0);

    return m_data->array[m_data->numElements - 1];
}

//------------------------------------------------------------------------------
//
static struct prv_item_t **prv_realloc(struct prv_item_t ***array, unsigned long numElements )
{
    if (array != NULL)
    {
        REALLOC(array, numElements, struct prv_item_t **);
        return *array;
    }
    else
        return CALLOC(numElements, struct prv_item_t *);
}

//------------------------------------------------------------------------------
//
void PrvBaseArray::set( unsigned long ind, struct prv_item_t* item)
{
    prv_integrity(m_data);
    assert(ind < m_data->numElements);
    m_data->array[ind] = item;
}

//------------------------------------------------------------------------------
//
void PrvBaseArray::add(struct prv_item_t *item)
{
    prv_integrity(m_data);

    m_data->array = prv_realloc(&m_data->array, m_data->numElements + 1);
    m_data->array[m_data->numElements] = item;
    m_data->numElements++;
}

//------------------------------------------------------------------------------
//
void PrvBaseArray::insert(unsigned long ind, struct prv_item_t *item)
{
    prv_integrity(m_data);
    assert_no_null(item);

    m_data->array = prv_realloc(&m_data->array, m_data->numElements + 1);

    if (m_data->numElements > 0)
    {
        unsigned long i;

        assert(ind < m_data->numElements);

        for (i = m_data->numElements; i > ind; i--)
            m_data->array[i] = m_data->array[i - 1];
    }

    m_data->array[ind] = item;
    m_data->numElements++;
}

//------------------------------------------------------------------------------
//
static void prv_eraseElement(struct prv_privateArray_t *m_data, unsigned long ind, bool isReference)
{
    struct prv_item_t **ptr;

    prv_integrity(m_data);

    assert(ind < m_data->numElements);

    if(isReference == false && m_data->func_freeElement != NULL)
        m_data->func_freeElement(&(m_data->array[ind]));

    if (m_data->numElements > 1)
    {
        ptr = CALLOC(m_data->numElements-1, struct prv_item_t *);
        for (unsigned long i = 0; i < ind; i++)
            ptr[i] = m_data->array[i];

        for (unsigned long i = ind; i < m_data->numElements-1; i++)
            ptr[i] = m_data->array[i+1];
    }
    else
        ptr = NULL;

    FREE_T(&(m_data->array), struct prv_item_t *);
    m_data->array = ptr;

    m_data->numElements--;
}

//------------------------------------------------------------------------------
//
void PrvBaseArray::erase(unsigned long ind)
{
    bool isReference;

    isReference = false;
    prv_eraseElement(m_data, ind, isReference);
}

//------------------------------------------------------------------------------
//
void PrvBaseArray::eraseRef(unsigned long ind)
{
    bool isReference;

    isReference = true;
    prv_eraseElement(m_data, m_data->numElements - 1, isReference);
}

//------------------------------------------------------------------------------
//
void PrvBaseArray::eraseLast()
{
    bool isReference;

    prv_integrity(m_data);
    assert(m_data->numElements > 0);

    isReference = false;
    prv_eraseElement(m_data, m_data->numElements - 1, isReference);
}

//------------------------------------------------------------------------------
//
void PrvBaseArray::eraseRefLast()
{
    bool isReference;

    prv_integrity(m_data);
    assert(m_data->numElements > 0);

    isReference = true;
    prv_eraseElement(m_data, m_data->numElements - 1, isReference);
}

//------------------------------------------------------------------------------
//
void PrvBaseArray::concatenate(
                        const class PrvBaseArray *array,
                        CArray_FPtr_copyElement funcCopy)
{
    prv_integrity(m_data);
    prv_integrity(array->m_data);

    if (array->m_data->numElements > 0)
    {
        m_data->array = prv_realloc(&m_data->array, m_data->numElements + array->m_data->numElements);

        for (unsigned long i = 0; i < array->m_data->numElements; i++)
        {
            if (funcCopy != NULL)
                m_data->array[i + m_data->numElements] = funcCopy(array->m_data->array[i]);
            else
                m_data->array[i + m_data->numElements] = array->m_data->array[i];
        }

        m_data->numElements += array->m_data->numElements;
    }
}

//------------------------------------------------------------------------------
//
static bool prv_comparePtr(const struct prv_item_t *item1, const struct prv_itemSearch_t *item2 )
{
    return item1 == (prv_item_t*)item2;
}

//------------------------------------------------------------------------------
//
class PrvBaseArray* PrvBaseArray::search(
                        const struct prv_itemSearch_t *itemSearch,
                        CArray_FPtr_search func_search_optional) const
{
    CArray_FPtr_search func_search;
    PrvBaseArray *array;
    unsigned long numElements;

    prv_integrity(m_data);
    assert_no_null(itemSearch);

    numElements = 0;
    array = new PrvBaseArray(numElements, NULL);

    if(func_search_optional == NULL)
        func_search = prv_comparePtr;
    else
        func_search = func_search_optional;

    for (unsigned long i = 0; i < m_data->numElements; i++)
    {
        if(func_search(m_data->array[i], itemSearch) == true)
            array->add(m_data->array[i]);
    }

    return array;
}

//------------------------------------------------------------------------------
//
static bool prv_searchElementFromPosition(
                    struct prv_item_t **array,
                    unsigned long fromIndex,
                    unsigned long numElements,
                    const struct prv_itemSearch_t *itemSearch,
                    CArray_FPtr_search func_search_optional,
                    unsigned long *ind_optional)
{
    if (numElements > 0)
    {
        CArray_FPtr_search func_search;

        assert_no_null(array);

        if(func_search_optional == NULL)
            func_search = prv_comparePtr;
        else
            func_search = func_search_optional;

        for (unsigned long i = fromIndex; i < numElements; i++)
        {
            if (func_search(array[i], itemSearch) == true)
            {
                if (ind_optional != NULL)
                    *ind_optional = i;

                return true;
            }
        }
    }

    return false;
}

//------------------------------------------------------------------------------
//
bool PrvBaseArray::exist(
                        const struct prv_itemSearch_t *itemSearch,
                        CArray_FPtr_search func_search_optional,
                        unsigned long *indFirst_optional) const
{
    assert_no_null(m_data);
    return prv_searchElementFromPosition(
                        m_data->array,
                        0, //fromIndex;
                        m_data->numElements,
                        itemSearch,
                        func_search_optional, 
                        indFirst_optional );
}

//------------------------------------------------------------------------------
//
bool PrvBaseArray::existFrom(
                        const struct prv_itemSearch_t *itemSearch,
                        unsigned long fromIndex,
                        CArray_FPtr_search func_search_optional,
                        unsigned long *indNext_optional) const
{
    assert_no_null(m_data);
    return prv_searchElementFromPosition(
                        m_data->array,
                        fromIndex,
                        m_data->numElements,
                        itemSearch,
                        func_search_optional,
                        indNext_optional );
}

//------------------------------------------------------------------------------
//
bool PrvBaseArray::existOnlyOneElement(
                        const struct prv_itemSearch_t *itemSearch,
                        CArray_FPtr_search func_search_optional,
                        unsigned long *ind_optional) const
{
    bool exist;
    unsigned long indElement;

    assert_no_null(m_data);
    exist = prv_searchElementFromPosition(
                        m_data->array,
                        0, // fromIndex
                        m_data->numElements,
                        itemSearch,
                        func_search_optional,
                        &indElement );
    if (exist == true)
    {
        bool existOther;

        existOther = prv_searchElementFromPosition(
                                            m_data->array,
                                            indElement + 1,
                                            m_data->numElements,
                                            itemSearch,
                                            func_search_optional,
                                            NULL );
        assert( existOther == false );

        if (ind_optional != NULL)
            *ind_optional = indElement;

        return true;
    }

    return false;
}

//------------------------------------------------------------------------------
//
struct prv_item_t *PrvBaseArray::getOnlyOneElement(
                        const struct prv_itemSearch_t *itemSearch,
                        CArray_FPtr_search func_search_optional) const
{
    bool exist, existOther;
    unsigned long indElement;

    assert_no_null(m_data);
    exist = prv_searchElementFromPosition(m_data->array,
                                            0, // fromIndex
                                            m_data->numElements,
                                            itemSearch,
                                            func_search_optional,
                                            &indElement);
    assert(exist == true);

    existOther = prv_searchElementFromPosition(
                                            m_data->array,
                                            indElement + 1,
                                            m_data->numElements,
                                            itemSearch,
                                            func_search_optional,
                                            NULL);
    assert(existOther == false);

    return m_data->array[indElement];
}
