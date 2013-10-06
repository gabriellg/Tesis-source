/* -----------------------------------------------------------------------------
 *  CGeneratorAcceleration.inl
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CAgentGeneratorEvent.hpp"
#include "CGeneratorKey.hpp"

class CGeneratorKinematicAndForces: public CAgentGeneratorEvent
{
    public:

        CGeneratorKinematicAndForces(class CWorldEscalextric *worldEscalextric);

        virtual void beginEvolution(class CCollectionEventsSystem *allEvents);

    private:

        struct SDataPrivateGeneratorKinematicAndForces *m_dataPrivate;
};

class CGeneratorAccelerationKey : public CGeneratorKey
{
    protected:

        virtual void generateEventWithKey(const class CEventKey *evtKey, class CCollectionEventsSystem *events);
};
