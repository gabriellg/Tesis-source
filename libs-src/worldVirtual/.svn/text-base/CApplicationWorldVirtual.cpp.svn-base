// Define la aplicación para mundos virtuales.
 
#include "CApplicationWorldVirtual.hpp"

#include "asrtbas.h"
#include "memory.h"
#include "CWorkspaceWorldVirtual.inl"

//-----------------------------------------------------------------------

CApplicationWorldVirtual::CApplicationWorldVirtual(
                        class CDefineWorld *defineWorld,
                        const char *titleAplication) : CAplicationGTK(titleAplication)
{
    assert_no_null(defineWorld);
	m_defineWorld = defineWorld;
}

//-----------------------------------------------------------------------

CApplicationWorldVirtual::~CApplicationWorldVirtual()
{
    assert(m_defineWorld == NULL);
}

//-----------------------------------------------------------------------

class CWorkspace *CApplicationWorldVirtual::createWorkspace()
{
	return new CWorkspaceWorldVirtual(&m_defineWorld);
}
