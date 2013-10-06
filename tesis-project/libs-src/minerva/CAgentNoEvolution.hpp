//CAgentNoEvolution.hpp: Agent only representation.

#include "CAgent.hpp"

class CAgentNoEvolution: public CAgent
{

private:

    virtual void beginEvolution(class CCollectionEventsSystem *allEvents) {;}
    virtual class CAgent *evolution(class CCollectionEventsSystem *allEvents, class CArrayRef<CAgent> **nextGenerationSons);

};
