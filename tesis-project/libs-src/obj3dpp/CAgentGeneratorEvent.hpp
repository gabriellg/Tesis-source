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

        virtual class CAgent *evolution(class CCollectionEventsSystem *allEvents);
        virtual class CAgent *representation(class CTypeDescription *evtDescription);

        virtual void beginRepresentation(class CTypeDescription *evtDescription);
        virtual void drawRepresentation(class CTypeDescription *evtDescription);
        virtual void endRepresentation(class CTypeDescription *evtDescription);
};

#endif /* CAGENTGENERATOREVENT_HPP_ */
