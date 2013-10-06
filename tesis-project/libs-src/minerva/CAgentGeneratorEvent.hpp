/* -----------------------------------------------------------------------------
 *  CAgentGeneratorEvent.hpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#ifndef CAGENTGENERATOREVENT_HPP_
#define CAGENTGENERATOREVENT_HPP_

#include "CAgent.hpp"

class CAgentGeneratorEvent: public CAgent
{
    public:

        CAgentGeneratorEvent() {;}

    private:

        virtual class CAgent *evolution(class CCollectionEventsSystem *allEvents, class CArrayRef<CAgent> **nextGenerationSons);

        virtual class CArray<IObjectDraw> *createRepresentation(class CTypeDescription *evtDescription,
                class CArray<IObjectDraw> **childsOpt);
};

#endif /* CAGENTGENERATOREVENT_HPP_ */
