// Define el proceso de una animación.

#include "CProcessAnimation.hpp"

#include "CEventTime.hpp"

#include "asrtbas.h"
#include "memory.h"

//---------------------------------------------------------------

CProcessAnimation::CProcessAnimation() : CProcessEventSystem(CEventTime::ID_EVENT)
{
    ;
}
