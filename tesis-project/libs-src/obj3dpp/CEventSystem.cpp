#include "CEventSystem.hpp"

#include "asrtbas.h"

#include "CString.hpp"

//---------------------------------------------------------------

CEventSystem::CEventSystem(const char *nameTypeEvent)
{
    m_nameTypeEvent = CString::copy(nameTypeEvent);
}

//---------------------------------------------------------------

CEventSystem::~CEventSystem()
{
    CString::free(&m_nameTypeEvent);
}

//---------------------------------------------------------------

bool CEventSystem::isTypeEvent(const char *nameTypeEvent) const
{
    return CString::equal(m_nameTypeEvent, nameTypeEvent);
}
