// Define some agents.

#include "CMultiObject3d.hpp"

#include "asrtbas.h"

//-----------------------------------------------------------------------

class CAgent *CMultiObject3d::evolution(class CCollectionEventsSystem *allEvents)
{
    return this;
}

//-----------------------------------------------------------------------

class CAgent *CMultiObject3d::representation(class CTypeDescription *allEvents)
{
    return NULL;
}
