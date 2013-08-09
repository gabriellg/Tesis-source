// Define un arbol.

#include "CAgentWithRepresentation.hpp"

class CTreeActor : public CAgentWithRepresentation
{
    public:

        CTreeActor(double xPos, double yPos);
        virtual ~CTreeActor();
    
    private:
    
        virtual class CAgent *evolution(class CCollectionEventsSystem *allEvents);
        virtual class CAgent *representation(class CTypeDescription *evtDescription);
    
        struct SPrvDataPrivateTree *m_dataPrivate;
};
