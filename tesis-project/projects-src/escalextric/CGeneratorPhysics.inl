/* -----------------------------------------------------------------------------
 *  CGeneratorAcceleration.inl
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CAgentGeneratorEvent.hpp"
#include "CGeneratorKey.hpp"

class CGeneratorAccelerationKey : public CGeneratorKey
{
    protected:

        virtual void generateEventWithKey(const class CEventKey *evtKey, class CCollectionEventsSystem *events);
};
