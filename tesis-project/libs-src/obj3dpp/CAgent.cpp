/* -----------------------------------------------------------------------------
 *  CAgent.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

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

//-----------------------------------------------------------------------

static void prv_destroyAgentNotUsed(
                class CArrayRef<CAgent> *agentsNotRepeatedNew,
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

class CArrayRef<CAgent> *CAgent::prv_allReferenceNotRepeated(class CArrayRef<CAgent> *agents)
{
    class CArrayRef<CAgent> *allReference;

    allReference = new CArrayRef<CAgent>;

    unsigned long num;

    num = agents->size();

    for (unsigned long i = 0; i < num; i++)
    {
        class CAgent *agent;

        agent = agents->get(i);
        assert_no_null(agent);

        agent->prv_appendRefNotRepeated(0, allReference);
    }

    return allReference;
}


//-----------------------------------------------------------------------

class CAgent *CAgent::prv_nextGenerationRecursive(unsigned long numRecursive, class CCollectionEventsSystem *allEvents)
{
    class CAgent *agentEvolved;

    prv_integrityAgent(m_dataPrivate);
    assert(numRecursive < 1000);
    numRecursive++;

    beginEvolution(allEvents);

    agentEvolved = evolution(allEvents);

    if (agentEvolved != NULL)
    {
        unsigned long num;

        num = m_dataPrivate->sons->size();

        if (num > 0)
        {
            class CArrayRef<CAgent> *nextGenerationSons;

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

            if (agentEvolved == this)
            {
                class CArrayRef<CAgent> *oldSons, *newSons;

                oldSons = prv_allReferenceNotRepeated(m_dataPrivate->sons);
                newSons = prv_allReferenceNotRepeated(nextGenerationSons);

                prv_destroyAgentNotUsed(newSons, oldSons);

                delete oldSons;
                delete newSons;

                delete agentEvolved->m_dataPrivate->sons;
                agentEvolved->m_dataPrivate->sons = nextGenerationSons;
            }
            else
            {
                agentEvolved->m_dataPrivate->sons->concatenate(nextGenerationSons);
                delete nextGenerationSons;
            }
        }
    }

    endEvolution(allEvents);

    return agentEvolved;
}

//-----------------------------------------------------------------------

class CAgent *CAgent::nextGeneration(class CCollectionEventsSystem *allEvents)
{
    return prv_nextGenerationRecursive(0, allEvents);
}

//-----------------------------------------------------------------------

void CAgent::prv_getRepresentationRecursive(
                        unsigned long numRecursive,
                        class CArrayRef<CAgent> *stringRepresentation,
                        class CTypeDescription *evtDescription)
{
    class CAgent *representationAgent;
    unsigned long num;

    prv_integrityAgent(m_dataPrivate);

    assert(numRecursive < 1000);
    numRecursive++;

    representationAgent = representation(evtDescription);

    if (representationAgent != NULL)
        stringRepresentation->add(representationAgent);

    num = m_dataPrivate->sons->size();

    for (unsigned long i = 0; i < num; i++)
    {
        class CAgent *son;

        son = m_dataPrivate->sons->get(i);
        assert_no_null(son);

        son->prv_getRepresentationRecursive(numRecursive, stringRepresentation, evtDescription);
    }
}

//-----------------------------------------------------------------------

class CArrayRef<CAgent> *CAgent::getRepresentation(class CTypeDescription *evtDescription)
{
    class CArrayRef<CAgent> *stringRepresentation;

    stringRepresentation = new class CArrayRef<CAgent>;
    prv_getRepresentationRecursive(0, stringRepresentation, evtDescription);

    return stringRepresentation;
}

//-----------------------------------------------------------------------

void CAgent::prv_traslateRepresentationToDescription(unsigned long numRecursive, class CTypeDescription *evtDescription)
{
    unsigned long num;

    prv_integrityAgent(m_dataPrivate);

    assert(numRecursive < 1000);
    numRecursive++;

    beginRepresentation(evtDescription);
    drawRepresentation(evtDescription);

    num = m_dataPrivate->sons->size();

    for (unsigned long i = 0; i < num; i++)
    {
        class CAgent *son;

        son = m_dataPrivate->sons->get(i);
        assert_no_null(son);
        son->prv_traslateRepresentationToDescription(numRecursive, evtDescription);
    }

    endRepresentation(evtDescription);
}

//-----------------------------------------------------------------------

void CAgent::traslateRepresentationToDescription(class CTypeDescription *evtDescription)
{
    unsigned long num;

    prv_integrityAgent(m_dataPrivate);

    beginRepresentation(evtDescription);
    drawRepresentation(evtDescription);

    num = m_dataPrivate->sons->size();

    for (unsigned long i = 0; i < num; i++)
    {
        class CAgent *son;

        son = m_dataPrivate->sons->get(i);
        assert_no_null(son);
        son->prv_traslateRepresentationToDescription(0, evtDescription);
    }

    endRepresentation(evtDescription);
}

//---------------------------------------------------------------

void CAgent::prv_appendRefNotRepeated(
                        unsigned long numRecursivity,
                        class CArrayRef<CAgent> *agentsNotRepeated)
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

//---------------------------------------------------------------

void CAgent::deleteAllAgentNotRepeted(class CAgent **agent)
{
    class CArray<CAgent> *agentsNotRepeated;

    assert_no_null(agent);
    assert_no_null(*agent);

    agentsNotRepeated = new CArray<CAgent>;
    (*agent)->prv_appendRefNotRepeated(0, (class CArrayRef<CAgent> *)agentsNotRepeated);

    delete agentsNotRepeated;

    *agent = NULL;
}

//---------------------------------------------------------------

void CAgent::eraseOldObjects(class CAgent *newAgent, class CAgent **oldAgent)
{
    class CArrayRef<CAgent> *agentsNotRepeatedNew;
    class CArrayRef<CAgent> *agentsNotRepeatedOld;

    assert_no_null(newAgent);
    assert_no_null(oldAgent);
    assert_no_null(*oldAgent);

    agentsNotRepeatedNew = new CArrayRef<CAgent>;
    agentsNotRepeatedOld = new CArrayRef<CAgent>;

    newAgent->prv_appendRefNotRepeated(0, agentsNotRepeatedNew);
    (*oldAgent)->prv_appendRefNotRepeated(0, agentsNotRepeatedOld);

    prv_destroyAgentNotUsed(agentsNotRepeatedNew, agentsNotRepeatedOld);

    delete agentsNotRepeatedNew;
    delete agentsNotRepeatedOld;
    *oldAgent = NULL;
}
