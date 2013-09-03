// Define el actor que genera árboles.

#include "CAgentWithRepresentation.hpp"

class CCreatorElements : public CAgentWithRepresentation
{
    public:
    
	   CCreatorElements() {;}
	   virtual ~CCreatorElements() {;}

    private:

       virtual class CAgent *evolution(class CCollectionEventsSystem *allEvents);
       virtual class CAgent *representation(class CTypeDescription *evtDescription);
};

