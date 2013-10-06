// CCircuit.hpp: Agent Circuit.

#include "CCircuit.inl"

#include "CDisplayEscalextric.inl"
#include "CEventApplyVelocity.inl"

#include "CArray.hpp"
#include "CFigure.hpp"
#include "asrtbas.h"
#include "CCollectionEventsSystem.hpp"

//-----------------------------------------------------------------------
//
CCircuit::CCircuit(class CWorldEscalextric *worldEscalextric)
{
    m_dataPrivate = (struct SPrvCircuit *)worldEscalextric;
}

//-----------------------------------------------------------------------
//
void CCircuit::beginEvolution(class CCollectionEventsSystem *allEvents)
{
    class CWorldEscalextric *world;
    class CEventSystem *evtSystem;

    world = (class CWorldEscalextric *)m_dataPrivate;
    evtSystem = new CEventApplyVelocity(world);
    allEvents->appendEventSystem(&evtSystem);
}

//---------------------------------------------------------------

class CAgent *CCircuit::evolution(class CCollectionEventsSystem *allEvents, class CArrayRef<CAgent> **sons)
{
    setSons(sons);
    return this;
}

//---------------------------------------------------------------

class CArray<IObjectDraw> *CCircuit::createRepresentation(class CTypeDescription *evtDescription,
        class CArray<IObjectDraw> **childsOpt)
{
    class IObjectDraw *circuit;

    assert_no_null(childsOpt);
    assert_no_null(*childsOpt);

    circuit = new CFigure(CDisplayEscalextric::SYMBOL_CIRCUIT);

    (*childsOpt)->insert(0, circuit);

    return *childsOpt;
}
