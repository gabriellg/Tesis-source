// CRcpActionRun.cpp

#include "CRcpActionRun.inl"

#include "CDataWorkspaceWorldVirtual.inl"

#include "asrtbas.h"
#include "memory.h"
#include "time.h"
#include "strbas.h"

#include "CEvtOpcAreaDib.hpp"

struct prv_dataPrivateCRcpActionRun_t
{
	unsigned short numFrames;
	time_t timeStart;
};

//-----------------------------------------------------------------------

static struct prv_dataPrivateCRcpActionRun_t *prv_create(
						unsigned short numFrames,
						time_t timeStart)
{
	struct prv_dataPrivateCRcpActionRun_t *dataPrivate;

	dataPrivate = MALLOC(struct prv_dataPrivateCRcpActionRun_t);

	dataPrivate->numFrames = numFrames;
	dataPrivate->timeStart = timeStart;

	return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroy(struct prv_dataPrivateCRcpActionRun_t **dataPrivate)
{
	FREE_T(dataPrivate, struct prv_dataPrivateCRcpActionRun_t);
}

//-----------------------------------------------------------------------

CRcpActionRun::CRcpActionRun() : CRcpOpcAreaDib("Play")
{
	unsigned short numFrames;
	time_t timeStart;

	numFrames = 0;
	timeStart = 0;

	m_dataPrivate = prv_create(numFrames, timeStart);
}

//-----------------------------------------------------------------------

CRcpActionRun::~CRcpActionRun()
{
	prv_destroy(&m_dataPrivate);
}

//---------------------------------------------------------------

static char *i_calculaFPS(unsigned short numFrames, time_t timeStart)
{
	time_t timeEnd;
	double secondsElapsed, FPS;

	timeEnd = clock();

	if (timeEnd > timeStart)
	{
		secondsElapsed = (double)(timeEnd - timeStart) / CLOCKS_PER_SEC;
		FPS = numFrames / secondsElapsed;

		return strbas_printf("FPS: %.0f",FPS);
	}
	else
		return strbas_printf("FPS: ???");
}

//---------------------------------------------------------------

enum CRcpOpcAreaDib::Result_t CRcpActionRun::processEvtTime(class CEvtOpcAreaDib *evtAreaDib)
{
    class CDataWorkspaceWorldVirtual *dataWorkspaceWorld;

    dataWorkspaceWorld = (class CDataWorkspaceWorldVirtual *)evtAreaDib->getDataWorkspace();
    assert_no_null(dataWorkspaceWorld);

	assert_no_null(m_dataPrivate);

    if (m_dataPrivate->numFrames == 0)
    	m_dataPrivate->timeStart = clock();

    dataWorkspaceWorld->nextFrame();

    m_dataPrivate->numFrames++;

    if (m_dataPrivate->numFrames == 30)
    {
    	char *txtFPS;

    	txtFPS = i_calculaFPS(m_dataPrivate->numFrames, m_dataPrivate->timeStart);
		evtAreaDib->setMessageRight(txtFPS);
    	strbas_destroy(&txtFPS);

    	m_dataPrivate->numFrames = 0;
    }

    evtAreaDib->setRedraw();
    return CRcpOpcAreaDib::CONTINUE;
}

//---------------------------------------------------------------

enum CRcpOpcAreaDib::Result_t CRcpActionRun::processEvtKey(class CEvtOpcAreaDib *evtAreaDib)
{
    class CDataWorkspaceWorldVirtual *dataWorkspaceWorld;
    const struct EvtKey_t *evtKey;

    dataWorkspaceWorld = (class CDataWorkspaceWorldVirtual *)evtAreaDib->getDataWorkspace();
    assert_no_null(dataWorkspaceWorld);

    evtKey = evtAreaDib->getEvtKey();

    dataWorkspaceWorld->appendKeyToScene(evtKey);

    return CRcpOpcAreaDib::CONTINUE;
}

//---------------------------------------------------------------

enum CRcpOpcAreaDib::Result_t CRcpActionRun::processStart(class CEvtOpcAreaDib *evtAreaDib)
{
	evtAreaDib->setMessageRight("FPS:");
	return CRcpOpcAreaDib::CONTINUE;
}


//---------------------------------------------------------------

enum CRcpOpcAreaDib::Result_t CRcpActionRun::processEnd(class CEvtOpcAreaDib *evtAreaDib)
{
    assert(evtAreaDib->isEnd() == true);

    evtAreaDib->setMessageRight("");
	return CRcpOpcAreaDib::END;
}
