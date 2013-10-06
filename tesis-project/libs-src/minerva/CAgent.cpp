// CAgent.hpp: Definición de Agente

#include "CAgent.hpp"

#include "asrtbas.h"
#include "memory.h"

#include "CArray.hpp"
#include "CArrayRef.hpp"

struct SPrvDataPrivateAgent
{
    class CArrayRef<CAgent> *sons;
};

//-----------------------------------------------------------------------

static void prv_integrityAgent(const struct SPrvDataPrivateAgent *dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(dataPrivate->sons);
}

//-----------------------------------------------------------------------

static struct SPrvDataPrivateAgent *prv_createAgent(class CArrayRef<CAgent> **sons)
{
    struct SPrvDataPrivateAgent *dataPrivate;

    dataPrivate = new SPrvDataPrivateAgent;

    dataPrivate->sons = ASSIGN_PP_NO_NULL(sons, class CArrayRef<CAgent>);

    prv_integrityAgent(dataPrivate);

    return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroyAgent(struct SPrvDataPrivateAgent **dataPrivate)
{
    assert_no_null(dataPrivate);
    prv_integrityAgent(*dataPrivate);

    delete (*dataPrivate)->sons;

    delete *dataPrivate;
    *dataPrivate = NULL;
}

//-----------------------------------------------------------------------

CAgent::CAgent(void)
{
    class CArrayRef<CAgent> *sons;

    sons = new CArrayRef<CAgent>;
    m_dataPrivate = prv_createAgent(&sons);
}

//-----------------------------------------------------------------------

CAgent::~CAgent()
{
    prv_destroyAgent(&m_dataPrivate);
}

//-----------------------------------------------------------------------

void CAgent::appendChild(class CAgent **son)
{
    prv_integrityAgent(m_dataPrivate);
    assert_no_null(son);
    assert_no_null(*son);

    m_dataPrivate->sons->add(*son);
    *son = NULL;
}

//-----------------------------------------------------------------------

void CAgent::appendChildren(class CArrayRef<CAgent> **sons)
{
    prv_integrityAgent(m_dataPrivate);

    m_dataPrivate->sons->concatenate(*sons);

    delete *sons;
    *sons = NULL;
}

//---------------------------------------------------------------

void CAgent::setSons(class CArrayRef<CAgent> **sons)
{
    prv_integrityAgent(m_dataPrivate);
    DELETE_OBJECT(&m_dataPrivate->sons, class CArrayRef<CAgent>);
    m_dataPrivate->sons = ASSIGN_PP_NO_NULL(sons, class CArrayRef<CAgent>);
}


//-----------------------------------------------------------------------

static void prv_destroyAgentOldNotUsed(class CArrayRef<CAgent> *agentsNotRepeatedNew,
        class CArrayRef<CAgent> *agentsNotRepeatedOld)
{
    class CArray<CAgent> *agentsToDelete;
    unsigned long num;

    agentsToDelete = new CArray<CAgent>;
    num = agentsNotRepeatedOld->size();

    for (unsigned long i = 0; i < num; i++)
    {
        class CAgent *agent;

        agent = agentsNotRepeatedOld->get(i);

        if (agentsNotRepeatedNew->exist<CAgent>(agent, NULL, NULL) == false)
            agentsToDelete->add(agent);
    }

    delete agentsToDelete;
}

//-----------------------------------------------------------------------

static void prv_destroyAgent(class CArrayRef<CAgent> **agentsNotRepeated)
{
    class CArrayRef<CAgent> *agentsNotRepeated_loc;
    unsigned long num;

    assert_no_null(agentsNotRepeated);
    assert_no_null(*agentsNotRepeated);

    agentsNotRepeated_loc = *agentsNotRepeated;
    num = agentsNotRepeated_loc->size();

    for (unsigned long i = 0; i < num; i++)
    {
        class CAgent *agent;

        agent = agentsNotRepeated_loc->get(i);
        DELETE_OBJECT(&agent, class CAgent);
    }

    DELETE_OBJECT(agentsNotRepeated, class CArrayRef<CAgent>);
}

//-----------------------------------------------------------------------

class CArrayRef<CAgent>* CAgent::prv_allReferenceNotRepeated(class CArrayRef<CAgent> *agents)
{
    class CArrayRef<CAgent>* allReferenceNotRepeated;
    unsigned long num;

    allReferenceNotRepeated = new CArrayRef<CAgent>;

    num = agents->size();

    for (unsigned long i = 0; i < num; i++)
    {
        class CAgent *agent;

        agent = agents->get(i);
        assert_no_null(agent);

        agent->prv_appendRefNotRepeated(0, allReferenceNotRepeated);
    }

    return allReferenceNotRepeated;
}

//-----------------------------------------------------------------------

void CAgent::destroyAllAgentsNotRepeated(class CArrayRef<CAgent> **agents)
{
    class CArrayRef<CAgent> *allReferenceNotRepeated;

    assert_no_null(agents);

    allReferenceNotRepeated = prv_allReferenceNotRepeated(*agents);

    prv_destroyAgent(&allReferenceNotRepeated);

    DELETE_OBJECT(agents, class CArrayRef<CAgent>);
}

//-----------------------------------------------------------------------

void CAgent::prv_destroyOldObjects(class CArrayRef<CAgent> *agents, class CArrayRef<CAgent> **oldAgents)
{
    class CArrayRef<CAgent> *oldSons, *newSons;

    oldSons = prv_allReferenceNotRepeated(agents);
    newSons = prv_allReferenceNotRepeated(*oldAgents);

    prv_destroyAgentOldNotUsed(newSons, oldSons);

    DELETE_OBJECT(&oldSons, class CArrayRef<CAgent>);
    DELETE_OBJECT(&newSons, class CArrayRef<CAgent>);
}

//-----------------------------------------------------------------------

class CAgent *CAgent::prv_nextGenerationRecursive(unsigned long numRecursive, class CCollectionEventsSystem *allEvents)
{
    unsigned long num;
    class CArrayRef<CAgent> *nextGenerationSons;
    class CArrayRef<CAgent> *oldSons, *newSons;

    prv_integrityAgent(m_dataPrivate);
    assert(numRecursive < 1000);
    numRecursive++;

    beginEvolution(allEvents);

    num = m_dataPrivate->sons->size();

    nextGenerationSons = new CArrayRef<CAgent>;

    for (unsigned long i = 0; i < num; i++)
    {
        class CAgent *son;
        class CAgent *sonEvolved;

        son = m_dataPrivate->sons->get(i);
        assert_no_null(son);

        sonEvolved = son->prv_nextGenerationRecursive(numRecursive, allEvents);

        if (sonEvolved != NULL)
            nextGenerationSons->add(sonEvolved);
    }

    oldSons = prv_allReferenceNotRepeated(m_dataPrivate->sons);
    newSons = prv_allReferenceNotRepeated(nextGenerationSons);

    prv_destroyAgentOldNotUsed(newSons, oldSons);

    DELETE_OBJECT(&oldSons, class CArrayRef<CAgent>);
    DELETE_OBJECT(&newSons, class CArrayRef<CAgent>);

    return evolution(allEvents, &nextGenerationSons);
}

//-----------------------------------------------------------------------

void CAgent::nextGeneration(class CCollectionEventsSystem *allEvents, class CArrayRef<CAgent> **currentGeneration)
{
    unsigned long numAgents;
    class CArrayRef<CAgent> *nextGeneration;
    class CArrayRef<CAgent> *currentGeneration_loc;

    assert_no_null(currentGeneration);
    assert_no_null(*currentGeneration);

    currentGeneration_loc = *currentGeneration;

    numAgents = currentGeneration_loc->size();
    nextGeneration = new CArrayRef<CAgent>(numAgents);

    for (unsigned long i = 0; i < numAgents; i++)
    {
        class CAgent *agent;
        class CAgent *agentNextGeneration;

        agent = currentGeneration_loc->get(i);
        agentNextGeneration = agent->prv_nextGenerationRecursive(0, allEvents);
        if (agentNextGeneration != NULL)
            nextGeneration->set(i, agentNextGeneration);
    }

    prv_destroyOldObjects(nextGeneration, &currentGeneration_loc);
    *currentGeneration = nextGeneration;
}

//-----------------------------------------------------------------------

class CArray<IObjectDraw> *CAgent::getRepresentation(class CTypeDescription *evtDescription)
{
    class CArray<IObjectDraw> *representationAgent;
    class CArray<IObjectDraw> *representationSons;

    representationSons = new class CArray<IObjectDraw>;

    for (unsigned long i = 0, size = m_dataPrivate->sons->size(); i < size; i++)
    {
        class CArray<IObjectDraw> *representationSon;
        class CAgent *agentSon;

        agentSon = m_dataPrivate->sons->get(i);

        representationSon = agentSon->getRepresentation(evtDescription);
        if (representationSon->size() > 0)
            representationSons->concatenateDestroying(&representationSon);
        else
            DELETE_OBJECT(&representationSon, class CArray<IObjectDraw>);
    }

    if (representationSons->size() > 0)
        representationAgent = createRepresentation(evtDescription, &representationSons);
    else
    {
        representationAgent = createRepresentation(evtDescription, NULL);
        DELETE_OBJECT(&representationSons, class CArray<IObjectDraw>);
    }

    return representationAgent;
}

//---------------------------------------------------------------

void CAgent::prv_appendRefNotRepeated(unsigned long numRecursivity, class CArrayRef<CAgent> *agentsNotRepeated)
{
    unsigned long num;

    prv_integrityAgent(m_dataPrivate);
    assert_no_null(agentsNotRepeated);
    assert(numRecursivity < 100);
    numRecursivity++;

    if (agentsNotRepeated->exist<CAgent>(this, NULL, NULL) == false)
        agentsNotRepeated->add(this);

    num = m_dataPrivate->sons->size();

    for (unsigned long i = 0; i < num; i++)
    {
        class CAgent *agentSon;

        agentSon = m_dataPrivate->sons->get(i);
        assert_no_null(agentSon);
        agentSon->prv_appendRefNotRepeated(numRecursivity, agentsNotRepeated);
    }
}
