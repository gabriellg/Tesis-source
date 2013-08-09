// Generador de eventos de arboles a crecer.

#include "CAgentGeneratorEvent.hpp"

class CGeneratorEventsTree : public CAgentGeneratorEvent
{
    public:
    
        CGeneratorEventsTree();
        virtual ~CGeneratorEventsTree();
       
    private:

        virtual void beginEvolution(class CCollectionEventsSystem *allEvents);
        virtual void endEvolution(class CCollectionEventsSystem *allEvents) {;}

        struct SPrvDataPrivateGeneratorEventsTree *m_dataPrivate;
};
