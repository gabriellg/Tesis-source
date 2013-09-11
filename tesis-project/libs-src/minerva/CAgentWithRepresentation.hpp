/* -----------------------------------------------------------------------------
 *  CAgentWithRepresentation.hpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#ifndef CAGENTWITHREPRESENTATION_HPP_
#define CAGENTWITHREPRESENTATION_HPP_

#include "CAgent.hpp"

class CAgentWithRepresentation: public CAgent
{
    public:

        CAgentWithRepresentation() {;};

    private:

        virtual void beginEvolution(class CCollectionEventsSystem *allEvents) {;}
        virtual void endEvolution(class CCollectionEventsSystem *allEvents) {;}

        virtual void beginRepresentation(class CTypeDescription *evtDescription);
        virtual void drawRepresentation(class CTypeDescription *evtDescription);
        virtual void endRepresentation(class CTypeDescription *evtDescription);
};

#endif /* CAGENTWITHREPRESENTATION_HPP_ */
