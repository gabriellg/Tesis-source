// Define some agents.

#include "CAgent.hpp"

class CMultiObject3d : public CAgent
{
	private:

        virtual void beginEvolution(class CCollectionEventsSystem *allEvents) {;}
        virtual void endEvolution(class CCollectionEventsSystem *allEvents) {;}

        virtual class CAgent *evolution(class CCollectionEventsSystem *allEvents);
        virtual class CAgent *representation(class CTypeDescription *allEvents);

        virtual void beginRepresentation(class CTypeDescription *description) {;}
        virtual void drawRepresentation(class CTypeDescription *description) {;}
        virtual void endRepresentation(class CTypeDescription *description){;}
};

