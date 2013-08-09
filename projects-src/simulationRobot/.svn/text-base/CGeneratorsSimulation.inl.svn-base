/* -----------------------------------------------------------------------------
 *  CGeneratorsSimulation.inl
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CAgentGeneratorEvent.hpp"
#include "CGeneratorKey.hpp"

class CGeneratorEventsLaserSimulation : public CAgentGeneratorEvent
{
    public:

        CGeneratorEventsLaserSimulation(const class CWorldSimulationRobot *world);
        ~CGeneratorEventsLaserSimulation();

    private:

        virtual void beginEvolution(class CCollectionEventsSystem *allEvents);
        virtual void endEvolution(class CCollectionEventsSystem *allEvents) {;}

        struct prv_dataPrivateGeneratorEventsLaserSimulation_t *m_dataPrivate;
};

class CGeneratorEventsVideoSimulation : public CAgentGeneratorEvent
{
    public:

        CGeneratorEventsVideoSimulation(const class CWorldSimulationRobot *world);
        ~CGeneratorEventsVideoSimulation();

        virtual void beginEvolution(class CCollectionEventsSystem *allEvents);
        virtual void endEvolution(class CCollectionEventsSystem *allEvents) {;}

    private:

        struct prv_dataPrivateGeneratorEventsVideoSimulation_t *m_dataPrivate;
};

class CGeneratorEventsObjectiveSimulation : public CGeneratorKey
{
    public:

        virtual void generateEventWithKey(const class CEventKey *evtKey, class CCollectionEventsSystem *events);
};

class CGeneratorEventsDecide : public CAgentGeneratorEvent
{
    private:

        virtual void beginEvolution(class CCollectionEventsSystem *allEvents);
        virtual void endEvolution(class CCollectionEventsSystem *allEvents) {;}
};
