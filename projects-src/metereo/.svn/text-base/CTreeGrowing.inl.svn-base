// Animación de arbol creciendo.

#include "CAgentWithRepresentation.hpp"

class CTreeGrowing : public CAgentWithRepresentation
{
    public:
    
    	CTreeGrowing(double xPos, double yPos);
    	virtual ~CTreeGrowing();
        
    private:
    
    	class CAgent *evolution(class CCollectionEventsSystem *allEvent);
        class CAgent *representation(class CTypeDescription *evtDraw);

        struct SPrvDataPrivateTreeGrowning *m_dataPrivate;
};
