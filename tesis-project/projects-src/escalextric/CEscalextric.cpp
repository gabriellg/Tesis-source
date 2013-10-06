//CEscalextric.cpp

#include "CEscalextric.inl"

#include "CWorldEscalextric.inl"
#include "CDisplayEscalextric.inl"
#include "CDataCircuit.inl"
#include "CCar.inl"
#include "CCircuit.inl"
#include "CGeneratorAccelerationKey.inl"

#include "asrtbas.h"
#include "memory.h"

#include "CDocXML.hpp"
#include "CString.hpp"
#include "CScene.hpp"
#include "CFigure.hpp"
#include "CGestorDisplays.hpp"

struct SPrvDataPrivateEscalextric
{
    bool is3d;
    class CDataCircuit *dataCircuit;
    class CWorldEscalextric *worldEscalextric;
};

//-----------------------------------------------------------------------

static struct SPrvDataPrivateEscalextric *prv_createDataPrivateEscalextric(
                            bool is3d,
                            class CDataCircuit **dataCircuit,
                            class CWorldEscalextric **worldEscalextric)
{
	struct SPrvDataPrivateEscalextric *dataPrivate;

	dataPrivate = MALLOC(struct SPrvDataPrivateEscalextric);

	dataPrivate->is3d = is3d;
	dataPrivate->dataCircuit = ASSIGN_PP_NO_NULL(dataCircuit, class CDataCircuit);
	dataPrivate->worldEscalextric = ASSIGN_PP_NO_NULL(worldEscalextric, class CWorldEscalextric);

	return dataPrivate;
}

//-----------------------------------------------------------------------

CEscalextric::CEscalextric(bool is3d, const char *fileNameXML)
{
	class CWorldEscalextric *worldEscalextric;
    class CDataCircuit *dataCircuit;

    dataCircuit = new CDataCircuit(fileNameXML);

	worldEscalextric = new CWorldEscalextric(dataCircuit);
	m_dataPrivate = prv_createDataPrivateEscalextric(is3d, &dataCircuit, &worldEscalextric);
}

//-----------------------------------------------------------------------

CEscalextric::~CEscalextric()
{
    assert_no_null(m_dataPrivate);

    delete m_dataPrivate->dataCircuit;
    delete m_dataPrivate->worldEscalextric;

    FREE_T(&m_dataPrivate, struct SPrvDataPrivateEscalextric);
}

//-----------------------------------------------------------------------

void CEscalextric::appendElementsToScene(class CScene *scene)
{
	class CAgent *circuit;
	class CAgent *carLeft, *carRight;
	class CAgent *generatorAccelerationKey;
	
	assert_no_null(scene);
	assert_no_null(m_dataPrivate);

    m_dataPrivate->worldEscalextric->appendCarLaneLeft(CCar::LEFT);
    m_dataPrivate->worldEscalextric->appendCarLaneRight(CCar::RIGHT);

	generatorAccelerationKey = new CGeneratorAccelerationKey();
    circuit = new CCircuit(m_dataPrivate->worldEscalextric);
    carLeft = new CCar(CCar::LEFT);
    carRight = new CCar(CCar::RIGHT);

    generatorAccelerationKey->appendChild(&carLeft);
    generatorAccelerationKey->appendChild(&carRight);
    circuit->appendChild(&generatorAccelerationKey);

	scene->appendAgent(&circuit);
}

//-----------------------------------------------------------------------

void CEscalextric::appendDisplays(class CGestorDisplays *displays)
{
    class ITraslatorDisplay *display;

    assert_no_null(m_dataPrivate);

    if (m_dataPrivate->is3d == true)
        display = CDisplayEscalextric::createDisplayGL(m_dataPrivate->dataCircuit, m_dataPrivate->worldEscalextric);
    else
        display = CDisplayEscalextric::createDisplaySprite(m_dataPrivate->dataCircuit, m_dataPrivate->worldEscalextric);

    displays->appendDisplay(&display);
}

//-----------------------------------------------------------------------

void CEscalextric::defineLayers(class IGraphics *graphics)
{
	//TODO: Esta función debería estar en CDisplay3D
}
