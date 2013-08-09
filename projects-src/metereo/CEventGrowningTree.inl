// Evento para generar un arbol.

#include "CEventPosition.inl"

class CEventGrowningTree : public CEventPosition
{
    public:
    
        static const char *ID_EVENT;
    
        CEventGrowningTree(double xPos, double yPos);
    	virtual ~CEventGrowningTree() {;}
        
        class CTreeGrowning *createTreeGrowning();
};
