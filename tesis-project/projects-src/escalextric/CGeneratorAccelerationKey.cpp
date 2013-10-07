/* -----------------------------------------------------------------------------
 *  CGeneratorAcceleration.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CGeneratorAccelerationKey.inl"

#include "CCar.inl"
#include "CEventAcceleration.inl"

#include "asrtbas.h"
#include "memory.h"
#include "CCollectionEventsSystem.hpp"
#include "CEventKey.hpp"
#include "CString.hpp"
#include "CArray.hpp"

static const double PRV_INCR_ACCELARATION = 1.;

struct SPrvKeyboardCar
{
    virtual ~SPrvKeyboardCar();

    char *id;
    unsigned int keyAccelerator, keyDesacelerator;
};

struct SPrvGeneratorAccelerationKey
{
    class CArray<SPrvKeyboardCar> *carsWithKeyboard;
};

//-----------------------------------------------------------------------

static struct SPrvKeyboardCar *prv_createKeyboardCar(const char *id, unsigned int keyAccelerator, unsigned int keyDesacelerator)
{
    struct SPrvKeyboardCar *keyboardCar;

    keyboardCar = new struct SPrvKeyboardCar;

    keyboardCar->id = CString::copy(id);
    keyboardCar->keyAccelerator = keyAccelerator;
    keyboardCar->keyDesacelerator = keyDesacelerator;

    return keyboardCar;
}

//-----------------------------------------------------------------------

SPrvKeyboardCar::~SPrvKeyboardCar()
{
    assert_no_null(id);
    FREE_T(&id, char);
}

//-----------------------------------------------------------------------

static struct SPrvGeneratorAccelerationKey *prv_createGeneratorAccelerationKey(
        class CArray<SPrvKeyboardCar> **carsWithKeyboard)
{
    struct SPrvGeneratorAccelerationKey *dataPrivate;

    dataPrivate = new struct SPrvGeneratorAccelerationKey;

    dataPrivate->carsWithKeyboard = ASSIGN_PP_NO_NULL(carsWithKeyboard, class CArray<SPrvKeyboardCar>);

    return dataPrivate;
}

//---------------------------------------------------------------

CGeneratorAccelerationKey::CGeneratorAccelerationKey()
{
    class CArray<SPrvKeyboardCar> *carsWithKeyboard;

    carsWithKeyboard = new CArray<SPrvKeyboardCar>;

    m_dataPrivate = prv_createGeneratorAccelerationKey(&carsWithKeyboard);
}

//---------------------------------------------------------------

CGeneratorAccelerationKey::~CGeneratorAccelerationKey()
{
    assert_no_null(m_dataPrivate);

    DELETE_OBJECT(&m_dataPrivate->carsWithKeyboard, class CArray<SPrvKeyboardCar>);

    FREE_T(&m_dataPrivate, struct SPrvGeneratorAccelerationKey);
}

//-----------------------------------------------------------------------
//
void CGeneratorAccelerationKey::appendCarKey(const char *id, unsigned int keyAccelerator, unsigned int keyDesacelerator)
{
    struct SPrvKeyboardCar *keyboardCar;

    assert_no_null(m_dataPrivate);

    keyboardCar = prv_createKeyboardCar(id, keyAccelerator, keyDesacelerator);
    m_dataPrivate->carsWithKeyboard->add(keyboardCar);
}

//-----------------------------------------------------------------------
//
static void prv_appendEvent(const char *idCar, double incrAcceleration, class CCollectionEventsSystem *events)
{
    class CEventSystem *evtSystem;

    evtSystem = new CEventAcceleration(idCar, incrAcceleration);
    events->appendEventSystem(&evtSystem);
}

//-----------------------------------------------------------------------
//
static bool prv_isKeyUsed(const struct SPrvKeyboardCar *keyboardCar, const unsigned int *key)
{
    assert_no_null(keyboardCar);
    assert_no_null(key);

    if (keyboardCar->keyAccelerator == *key || keyboardCar->keyDesacelerator == *key)
        return true;
    else
        return false;
}

//-----------------------------------------------------------------------
//
void CGeneratorAccelerationKey::generateEventWithKey(const class CEventKey *evtKey,
        class CCollectionEventsSystem *events)
{
    unsigned int keyPressed;
    unsigned long indKeyboard;

    assert_no_null(evtKey);

    keyPressed = evtKey->key();

    if (m_dataPrivate->carsWithKeyboard->existOnlyOneElement<unsigned int>(&keyPressed, prv_isKeyUsed, &indKeyboard)
            == true)
    {
        const struct SPrvKeyboardCar *keyboardCar;

        keyboardCar = m_dataPrivate->carsWithKeyboard->get(indKeyboard);

        if (keyboardCar->keyAccelerator == keyPressed)
            prv_appendEvent(keyboardCar->id, PRV_INCR_ACCELARATION, events);
        else
        {
            assert(keyboardCar->keyDesacelerator == keyPressed);
            prv_appendEvent(keyboardCar->id, -PRV_INCR_ACCELARATION, events);
        }
    }
}
