//CRcpOpcAreaDib.cpp

#include "CRcpOpcAreaDib.hpp"

#include "CEvtOpcAreaDib.hpp"

#include "asrtbas.h"
#include "memory.h"
#include "strbas.h"
#include "timesys.h"

struct prv_dataPrivateOpcAreaDib_t
{
	char *nameOption;

	bool isRunning;

	class CEvtOpcAreaDib *evtAreaMouse;
	class CEvtOpcAreaDib *evtAreaKey;
	class CEvtOpcAreaDib *evtAreaTime;
};

//---------------------------------------------------------

static struct prv_dataPrivateOpcAreaDib_t *prv_create(
		const char *nameOption,
		bool isRuning,
		class CEvtOpcAreaDib *evtAreaMouse,
		class CEvtOpcAreaDib *evtAreaKey,
		class CEvtOpcAreaDib *evtAreaTime)
{
	struct prv_dataPrivateOpcAreaDib_t *dataPrivate;
	
	dataPrivate = MALLOC(struct prv_dataPrivateOpcAreaDib_t);
	
	dataPrivate->nameOption = strbas_copyString(nameOption);

	dataPrivate->isRunning = isRuning;

	dataPrivate->evtAreaMouse = evtAreaMouse;
	dataPrivate->evtAreaKey = evtAreaKey;
	dataPrivate->evtAreaTime = evtAreaTime;
	
	return dataPrivate;
}

//---------------------------------------------------------

static void prv_destroy(struct prv_dataPrivateOpcAreaDib_t **dataPrivate)
{
	assert_no_null(dataPrivate);
	assert_no_null(*dataPrivate);

	//TODO: Debemos ponerlo cuando desactive la opción al cerrar la aplicación.
	//assert((*dataPrivate)->isRunning == false);
	assert((*dataPrivate)->evtAreaKey == NULL);
	assert((*dataPrivate)->evtAreaTime == NULL);
	
	strbas_destroy(&(*dataPrivate)->nameOption);

	FREE_T(dataPrivate, struct prv_dataPrivateOpcAreaDib_t);
}

//---------------------------------------------------------

CRcpOpcAreaDib::CRcpOpcAreaDib(const char *nameOption)
{
	bool isRuning;
	class CEvtOpcAreaDib *evtAreaMouse;
	class CEvtOpcAreaDib *evtAreaKey;
	class CEvtOpcAreaDib *evtAreaTime;
	
	isRuning = false;

	evtAreaMouse = NULL;
	evtAreaKey = NULL;
	evtAreaTime = NULL;

	m_dataPrivate = prv_create(nameOption, isRuning, evtAreaMouse, evtAreaKey, evtAreaTime);
}

//---------------------------------------------------------

CRcpOpcAreaDib::~CRcpOpcAreaDib()
{
	prv_destroy(&m_dataPrivate);
}

//---------------------------------------------------------

static bool prv_existEvtPending(
							class CEvtOpcAreaDib *evtAreaMouse,
							class CEvtOpcAreaDib *evtAreaKey,
							class CEvtOpcAreaDib *evtAreaTime)
{
	if (evtAreaKey != NULL)
		return true;

	if (evtAreaMouse != NULL)
		return true;

	if (evtAreaTime != NULL)
		return true;

	return false;
}

//---------------------------------------------------------

static bool prv_isStoping(bool isRuning)
{
	if (isRuning == true)
		return false;
	else
		return true;
}

//---------------------------------------------------------

static void prv_waitPending(struct prv_dataPrivateOpcAreaDib_t *dataPrivate)
{
	unsigned long numStepWait;

	assert_no_null(dataPrivate);

	numStepWait = 0;
	dataPrivate->isRunning = false;

	while(prv_existEvtPending(
			dataPrivate->evtAreaMouse,
			dataPrivate->evtAreaKey,
			dataPrivate->evtAreaTime) == true)
	{
		timesys_sleep(1000);
		numStepWait++;
		assert(numStepWait < 1000);
	}
}

//---------------------------------------------------------

enum CRcpOpcAreaDib::Result_t CRcpOpcAreaDib::sendEvent(class CEvtOpcAreaDib *evtAreaDib)
{
	enum CRcpOpcAreaDib::Result_t result;
	enum CEvtOpcAreaDib::TypeEvent_t typeEvent;
	
	assert_no_null(m_dataPrivate);

	if (evtAreaDib->isStart() == true)
		evtAreaDib->setMessageLeft(m_dataPrivate->nameOption);

	typeEvent = evtAreaDib->typeEvent();

	switch(typeEvent)
	{
		case CEvtOpcAreaDib::START:
		{
			assert(prv_existEvtPending(
							m_dataPrivate->evtAreaMouse,
							m_dataPrivate->evtAreaKey,
							m_dataPrivate->evtAreaTime) == false);

			result = processStart(evtAreaDib);

			m_dataPrivate->isRunning = true;
			break;
		}
		case CEvtOpcAreaDib::END:
		{
			result = END;
			break;
		}
		case CEvtOpcAreaDib::MOUSE:
		{
			if (m_dataPrivate->isRunning == true && m_dataPrivate->evtAreaMouse == NULL)
			{
				m_dataPrivate->evtAreaMouse = evtAreaDib;
				result = processEvtMouse(evtAreaDib);
				m_dataPrivate->evtAreaMouse = NULL;
			}
			else
				result = CONTINUE;
			break;
		}
		case CEvtOpcAreaDib::TIME:
		{
			if (m_dataPrivate->isRunning == true && m_dataPrivate->evtAreaTime == NULL)
			{
				m_dataPrivate->evtAreaTime = evtAreaDib;
				result = processEvtTime(evtAreaDib);
				m_dataPrivate->evtAreaTime = NULL;
			}
			else
				result = CONTINUE;
			break;
		}
		case CEvtOpcAreaDib::KEY:
		{
			if (m_dataPrivate->isRunning == true && m_dataPrivate->evtAreaKey == NULL)
			{
				m_dataPrivate->evtAreaKey = evtAreaDib;
				result = processEvtKey(evtAreaDib);
				m_dataPrivate->evtAreaKey = NULL;
			}
			else
				result = CONTINUE;
			break;
		}
		default_error();
	}

	if (result == END)
	{
		if (prv_isStoping(m_dataPrivate->isRunning) == false)
		{
			m_dataPrivate->isRunning = false;

			prv_waitPending(m_dataPrivate);

			result = processEnd(evtAreaDib);

			if (result != END)
				m_dataPrivate->isRunning = true;
			else
				evtAreaDib->setMessageLeft("");
		}
		else
			result = CONTINUE;
	}
	
	return result;
}

//---------------------------------------------------------

const char *CRcpOpcAreaDib::getTextMenu(void)
{
	assert_no_null(m_dataPrivate);
	assert_no_null(m_dataPrivate->nameOption);
	
	return m_dataPrivate->nameOption;
}

//---------------------------------------------------------

enum CRcpOpcAreaDib::Result_t CRcpOpcAreaDib::processEvtTime(class CEvtOpcAreaDib *evtAreaDib)
{
	assert_no_null(evtAreaDib);
	return CRcpOpcAreaDib::CONTINUE;
}

//---------------------------------------------------------

enum CRcpOpcAreaDib::Result_t CRcpOpcAreaDib::processEvtKey(class CEvtOpcAreaDib *evtAreaDib)
{
	assert_no_null(evtAreaDib);
	return CRcpOpcAreaDib::CONTINUE;
}

//---------------------------------------------------------

enum CRcpOpcAreaDib::Result_t CRcpOpcAreaDib::processEvtMouse(class CEvtOpcAreaDib *evtAreaDib)
{
	assert_no_null(evtAreaDib);
	return CRcpOpcAreaDib::CONTINUE;
}

//---------------------------------------------------------

enum CRcpOpcAreaDib::Result_t CRcpOpcAreaDib::processStart(class CEvtOpcAreaDib *evtAreaDib)
{
	assert_no_null(evtAreaDib);
	return CRcpOpcAreaDib::CONTINUE;
}

//---------------------------------------------------------

enum CRcpOpcAreaDib::Result_t CRcpOpcAreaDib::processEnd(class CEvtOpcAreaDib *evtAreaDib)
{
	assert_no_null(evtAreaDib);
	return CRcpOpcAreaDib::END;
}

