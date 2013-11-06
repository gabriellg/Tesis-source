// CCircuit.hpp: Agent Circuit.

#include "CCircuit.inl"

#include "CDisplayEscalextric.inl"
#include "CEventApplyVelocity.inl"

#include "CArray.hpp"
#include "CFigure.hpp"
#include "asrtbas.h"
#include "CCollectionEventsSystem.hpp"
#include "CTransform3D.hpp"
#include "CTransform.hpp"

#include "asrtbas.h"
#include "memory.h"

struct SPrvCircuit
{
    class CWorldEscalextric *worldEscalextricRef;
    double xCenter, yCenter, zCenter;
};

//-----------------------------------------------------------------------

static struct SPrvCircuit *prv_createCircuit(class CWorldEscalextric *worldEscalextricRef, double xCenter,
        double yCenter, double zCenter)
{
    struct SPrvCircuit *dataPrivate;

    dataPrivate = new struct SPrvCircuit;

    dataPrivate->worldEscalextricRef = worldEscalextricRef;

    dataPrivate->xCenter = xCenter;
    dataPrivate->yCenter = yCenter;
    dataPrivate->zCenter = zCenter;

    return dataPrivate;
}

//-----------------------------------------------------------------------
//
CCircuit::CCircuit(class CWorldEscalextric *worldEscalextric, double xCenterCircuit, double yCenterCircuit, double zCenterCircuit)
{
    m_dataPrivate = prv_createCircuit(worldEscalextric, xCenterCircuit, yCenterCircuit, zCenterCircuit);
}

//---------------------------------------------------------------

CCircuit::~CCircuit()
{
    DELETE_OBJECT(&m_dataPrivate, struct SPrvCircuit);
}

//-----------------------------------------------------------------------
//
void CCircuit::beginEvolution(class CCollectionEventsSystem *allEvents)
{
    class CEventSystem *evtSystem;

    evtSystem = new CEventApplyVelocity(m_dataPrivate->worldEscalextricRef);
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
    class IObjectDraw *centerCircuit;
    class CArray<IObjectDraw> *result;

    assert_no_null(childsOpt);
    assert_no_null(*childsOpt);

    circuit = new CFigure(CDisplayEscalextric::SYMBOL_CIRCUIT);
    (*childsOpt)->insert(0, circuit);

    centerCircuit = CTransform3D::createTraslateArray3d(childsOpt, -m_dataPrivate->xCenter, -m_dataPrivate->yCenter, -m_dataPrivate->zCenter);

    result = new CArray<IObjectDraw>(1);
    result->set(0, centerCircuit);

    return result;
}
