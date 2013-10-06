//CGeneratorKey.hpp

#ifndef CGENERATORKEY_INL__
#define CGENERATORKEY_INL__

#include "CAgentGeneratorEvent.hpp"

class CGeneratorKey : public CAgentGeneratorEvent
{
    public:

        CGeneratorKey() {;}

	private:

        virtual void beginEvolution(class CCollectionEventsSystem *allEvents);

        virtual void generateEventWithKey(const class CEventKey *evtKey, class CCollectionEventsSystem *events) = 0;
};

#endif /*CGENERATORKEY_INL__*/
