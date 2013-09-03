// Generador de eventos de arboles a crecer.

#include "CGeneratorEventsTree.inl"

#include "CMetereoUtils.inl"
#include "CEventGrowningTree.inl"
#include "CEventStorm.inl"

#include "asrtbas.h"
#include "memory.h"
#include "CEventTime.hpp"
#include "CCollectionEventsSystem.hpp"

#define NUM_TREE        18
#define NUM_LIGHTNING   12

struct SPrvDataPrivateGeneratorEventsTree
{
    double sizeXTree, sizeYTree;

    unsigned long indPositionTree;
    unsigned long indPositionLightning;
};

//-----------------------------------------------------------------------

static struct SPrvDataPrivateGeneratorEventsTree *prv_create(
                        double sizeXTree, double sizeYTree,
                        unsigned long indPositionTree,
                        unsigned long indPositionLightning)
{
    struct SPrvDataPrivateGeneratorEventsTree *dataPrivate;

    dataPrivate = MALLOC(struct SPrvDataPrivateGeneratorEventsTree);

    dataPrivate->sizeXTree = sizeXTree;
    dataPrivate->sizeYTree = sizeYTree;

    dataPrivate->indPositionTree = indPositionTree;
    dataPrivate->indPositionLightning = indPositionLightning;

    return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroy(struct SPrvDataPrivateGeneratorEventsTree **dataPrivate)
{
    FREE_T(dataPrivate, struct SPrvDataPrivateGeneratorEventsTree);
}

//-----------------------------------------------------------------------

CGeneratorEventsTree::CGeneratorEventsTree()
{
    double sizeXTree, sizeYTree;
    unsigned long indPositionTree, indPositionLightning;

    CMetereoUtils::sizeTree(&sizeXTree, &sizeYTree);
    indPositionTree = 0;
    indPositionLightning = 0;

    m_dataPrivate = prv_create(sizeXTree, sizeYTree, indPositionTree, indPositionLightning);
}

//-----------------------------------------------------------------------

CGeneratorEventsTree::~CGeneratorEventsTree()
{
    prv_destroy(&m_dataPrivate);
}

//---------------------------------------------------------------

static void prv_position(
                        const struct SPrvDataPrivateGeneratorEventsTree *dataGeneratorEventsTree,
                        unsigned long indRow, unsigned long indCol,
                        double *xPos, double *yPos)
{
    assert_no_null(dataGeneratorEventsTree);
    assert_no_null(xPos);
    assert_no_null(yPos);

    *xPos = indCol * dataGeneratorEventsTree->sizeXTree + dataGeneratorEventsTree->sizeXTree / 2.;
    *yPos = indRow * dataGeneratorEventsTree->sizeYTree + dataGeneratorEventsTree->sizeYTree / 2.;
}

//---------------------------------------------------------------

static bool prv_generatedPositionTree(
                    struct SPrvDataPrivateGeneratorEventsTree *dataGeneratorEventsTree,
                    double *xPos, double *yPos)
{
    static const unsigned long generatorX[NUM_TREE] = { 5, 5, 3, 7, 9, 3, 7, 9, 6, 3, 9, 8, 6, 1, 5, 1, 3, 6};
    static const unsigned long generatorY[NUM_TREE] = { 1, 5, 2, 6, 9, 4, 2, 8, 3, 5, 2, 3, 8, 9, 7, 2, 6, 7};
    unsigned long row, col;

    assert_no_null(dataGeneratorEventsTree);

    row = generatorX[dataGeneratorEventsTree->indPositionTree];
    col = generatorY[dataGeneratorEventsTree->indPositionTree];

    if (dataGeneratorEventsTree->indPositionTree < NUM_TREE)
    {
        prv_position(dataGeneratorEventsTree, row, col, xPos, yPos);
        dataGeneratorEventsTree->indPositionTree++;

        return true;
    }
    else
        return false;
}

//---------------------------------------------------------------

static bool prv_generatedPositionLightning(
                    struct SPrvDataPrivateGeneratorEventsTree *dataGeneratorEventsTree,
                    double *xPos, double *yPos)
{
    static const unsigned long generatorX[NUM_LIGHTNING] = { 0, 5, 1, 2, 6, 3, 2, 4, 2, 6, 3, 2};
    static const unsigned long generatorY[NUM_LIGHTNING] = { 1, 5, 1, 6, 6, 2, 1, 6, 2, 5, 1, 3};
    unsigned long row, col;

    assert_no_null(dataGeneratorEventsTree);

    row = generatorX[dataGeneratorEventsTree->indPositionLightning];
    col = generatorY[dataGeneratorEventsTree->indPositionLightning];

    prv_position(dataGeneratorEventsTree, row, col, xPos, yPos);
    dataGeneratorEventsTree->indPositionLightning = (dataGeneratorEventsTree->indPositionLightning + 1) % NUM_LIGHTNING;

    return true;
}

//---------------------------------------------------------------

void CGeneratorEventsTree::beginEvolution(class CCollectionEventsSystem *allEvents)
{
    static unsigned long num = 0;
    class CEventSystem *evtSystem;
    
    evtSystem = new CEventTime();
    allEvents->appendEventSystem(&evtSystem);

    if (num % 5 == 0)
    {
        double xPos, yPos;
        
        if (prv_generatedPositionTree(m_dataPrivate, &xPos, &yPos) == true)
        {
            evtSystem = new CEventGrowningTree(xPos, yPos);
            allEvents->appendEventSystem(&evtSystem);
        }
    }
    
    if (num % 15 == 0)
    {
        double xPos, yPos;
        
        if (prv_generatedPositionLightning(m_dataPrivate, &xPos, &yPos) == true)
        {
            evtSystem = new CEventStorm(xPos, yPos);
            allEvents->appendEventSystem(&evtSystem);
        }
    }
    
    num++;
}
