// CCircuit.hpp: Agent Circuit.

#include "CCircuit.inl"

#include "CDisplayEscalextric.inl"

#include "CArray.hpp"
#include "CFigure.hpp"
#include "asrtbas.h"

//---------------------------------------------------------------

class CArray<IObjectDraw> *CCircuit::createRepresentation(class CTypeDescription *evtDescription,
        class CArray<IObjectDraw> **childsOpt)
{
    class CArray<IObjectDraw> *objectsDraw;
    class IObjectDraw *circuit;

    assert(childsOpt == NULL);

    circuit = new CFigure(CDisplayEscalextric::SYMBOL_CIRCUIT);

    objectsDraw = new CArray<IObjectDraw>(1);
    objectsDraw->set(0, circuit);

    return objectsDraw;
}
