// Define un relámpago.

#include "CAgentWithRepresentation.hpp"

class CLightning : public CAgentWithRepresentation
{
    public:
    
    	CLightning(double xPos, double yPos);
    	virtual ~CLightning();
        
    private:
    
        virtual class CAgent *evolution(class CCollectionEventsSystem *allEvents);
        virtual class CAgent *representation(class CTypeDescription *evtDescription);

        struct SPrvDataPrivateLightning *m_dataPrivate;
};
