// CAgent.hpp: Definición de Agente

#pragma once

#include "IObjectDraw.hpp"

class CAgent
{
public:

    CAgent(void);
    virtual ~CAgent();

    void appendChild(class CAgent **son);
    void appendChildren(class CArrayRef<CAgent> **sons);

    void setSons(class CArrayRef<CAgent> **sons);

    static void nextGeneration(class CCollectionEventsSystem *allEvents, class CArrayRef<CAgent> **currentGeneration);
    class CArray<IObjectDraw> *getRepresentation(class CTypeDescription *evtDescription);

    virtual void beginEvolution(class CCollectionEventsSystem *allEvents) = 0;
    virtual class CAgent *evolution(class CCollectionEventsSystem *allEvents, class CArrayRef<CAgent> **nextGenerationSons) = 0;

    virtual class CArray<IObjectDraw> *createRepresentation(class CTypeDescription *evtDescription,
            class CArray<IObjectDraw> **childsOpt) = 0;

    static void destroyAllAgentsNotRepeated(class CArrayRef<CAgent> **agents);

private:

    static void prv_destroyOldObjects(class CArrayRef<CAgent> *agents, class CArrayRef<CAgent> **oldAgents);
    static class CArrayRef<CAgent>* prv_allReferenceNotRepeated(class CArrayRef<CAgent>* agents);

    class CAgent *prv_nextGenerationRecursive(unsigned long numRecursive, class CCollectionEventsSystem *allEvents);
    void prv_appendRefNotRepeated(unsigned long numRecursivity, class CArrayRef<CAgent> *agentsNotRepeated);

    struct SPrvDataPrivateAgent *m_dataPrivate;
};
