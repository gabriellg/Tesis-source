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
    class IObjectDraw *circuit;

    assert_no_null(childsOpt);
    assert_no_null(*childsOpt);

    circuit = new CFigure(CDisplayEscalextric::SYMBOL_CIRCUIT);

    (*childsOpt)->insert(0, circuit);

    return *childsOpt;
}
