//CAgentNoEvolution.hpp: Agent only representation.

#include "CAgentNoEvolution.hpp"

//---------------------------------------------------------------

class CAgent *CAgentNoEvolution::evolution(class CCollectionEventsSystem *allEvents, class CArrayRef<CAgent> **sons)
{
    setSons(sons);
    return this;
}
