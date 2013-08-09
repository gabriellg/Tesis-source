// Animacion arbol ardiendo.

#include "CAgentWithRepresentation.hpp"

class CTreeBurning : public CAgentWithRepresentation
{
    public:
    
    	CTreeBurning(double xPos, double yPos);
    	virtual ~CTreeBurning();
        
    private:
    
        virtual class CAgent *evolution(class CCollectionEventsSystem *allEvents);
        virtual class CAgent *representation(class CTypeDescription *evtDescription);
    
        struct SPrvDataPrivateTreeBurning *m_dataPrivate;
};
