// CCircuit.hpp: Agent Circuit.

#include "CAgent.hpp"

class CCircuit: public CAgent
{
public:

    CCircuit(class CWorldEscalextric *worldEscalextric, double xCenterCircuit, double yCenterCircuit, double zCenterCircuit);
    virtual ~CCircuit();

private:

    virtual void beginEvolution(class CCollectionEventsSystem *allEvents);
    virtual class CAgent *evolution(class CCollectionEventsSystem *allEvents, class CArrayRef<CAgent> **nextGenerationSons);

    virtual class CArray<IObjectDraw> *createRepresentation(class CTypeDescription *evtDescription,
            class CArray<IObjectDraw> **childsOpt);

    struct SPrvCircuit *m_dataPrivate;
};
