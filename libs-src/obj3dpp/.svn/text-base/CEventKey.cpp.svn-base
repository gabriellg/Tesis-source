/* -----------------------------------------------------------------------------
 *  CEventKey.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CEventKey.hpp"

#include "asrtbas.h"
#include "memory.h"
#include "evtkey.h"

const char *CEventKey::ID_EVENT = "CEventKey";

//-----------------------------------------------------------------------

CEventKey::CEventKey(const struct EvtKey_t *evtKey) : CEventSystem(CEventKey::ID_EVENT)
{
    struct EvtKey_t *evtKeyCopy;

    evtKeyCopy = evtkey_copy(evtKey);
    m_dataPrivate = (struct SPrvDataPrivateEventKey *)evtKeyCopy;
}

//-----------------------------------------------------------------------

CEventKey::~CEventKey()
{
    struct EvtKey_t *evtKey;

    evtKey = (struct EvtKey_t *)m_dataPrivate;
    evtkey_destroy(&evtKey);
}

//-----------------------------------------------------------------------

unsigned int CEventKey::key(void) const
{
    struct EvtKey_t *evtKey;

    evtKey = (struct EvtKey_t *)m_dataPrivate;
    return evtkey_key(evtKey);
}
