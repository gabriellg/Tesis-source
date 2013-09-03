#include "CEvtOpcAreaDib.hpp"

#include "CStatusLine.hpp"

#include "asrtbas.h"
#include "memory.h"

struct prv_dataPrivateEvtOpcAreaDib_t
{
    class CDataWorkspace *dataWorkspace;
    
    class CStatusLine *statusLine;
    
    enum CEvtOpcAreaDib::TypeEvent_t type;
    enum CEvtOpcAreaDib::TypeRedraw_t typeRedraw;

    class CView *viewCurrent;    
    struct dialogo_t *dlgFather;

    const struct EvtMouse_t *evtMouse;
    const struct EvtKey_t *evtKey;
};

//-----------------------------------------------------------------------

static struct prv_dataPrivateEvtOpcAreaDib_t *prv_create(
                        class CDataWorkspace *dataWorkspace,
                        class CStatusLine *statusLine,
                        enum CEvtOpcAreaDib::TypeEvent_t type,
                        enum CEvtOpcAreaDib::TypeRedraw_t typeRedraw,
                        class CView *viewCurrent,
                        struct dialogo_t *dlgFather,
                        const struct EvtMouse_t *evtMouse,
                        const struct EvtKey_t *evtKey)
{
	struct prv_dataPrivateEvtOpcAreaDib_t *dataPrivate;
    
    dataPrivate = MALLOC(struct prv_dataPrivateEvtOpcAreaDib_t);
    
    dataPrivate->dataWorkspace = dataWorkspace;
    
    dataPrivate->statusLine = statusLine;
    
    dataPrivate->type = type;
    dataPrivate->typeRedraw = typeRedraw;
    
    dataPrivate->viewCurrent = viewCurrent;
    dataPrivate->dlgFather = dlgFather;
    dataPrivate->evtMouse = evtMouse;
    dataPrivate->evtKey = evtKey;
    
    return dataPrivate;    
}

//-----------------------------------------------------------------------

static void prv_destroy(struct prv_dataPrivateEvtOpcAreaDib_t **dataPrivate)
{
    FREE_T(dataPrivate, struct prv_dataPrivateEvtOpcAreaDib_t);
}

//-----------------------------------------------------------------------

CEvtOpcAreaDib::CEvtOpcAreaDib(
				enum TypeEvent_t type, 
				class CDataWorkspace *dataWorkspace,
				class CStatusLine *statusLine,
                class CView *viewCurrent, 
				struct dialogo_t *dlgFather,
				const struct EvtMouse_t *evtMouse,
				const struct EvtKey_t *evtKey)
{
    m_dataPrivate = prv_create(
                dataWorkspace, statusLine, type, 
                CEvtOpcAreaDib::NO_REDRAW, 
                viewCurrent, dlgFather, evtMouse, evtKey);
}

//-----------------------------------------------------------------------

CEvtOpcAreaDib::~CEvtOpcAreaDib()
{
    prv_destroy(&m_dataPrivate);
}
        
//-----------------------------------------------------------------------

enum CEvtOpcAreaDib::TypeEvent_t CEvtOpcAreaDib::typeEvent(void) 
{ 
    assert_no_null(m_dataPrivate);
    return m_dataPrivate->type; 
}
        
//-----------------------------------------------------------------------

class CDataWorkspace *CEvtOpcAreaDib::getDataWorkspace(void) 
{
    assert_no_null(m_dataPrivate);
    return m_dataPrivate->dataWorkspace; 
}

//-----------------------------------------------------------------------

class CView *CEvtOpcAreaDib::getViewCurrent(void) 
{
    assert_no_null(m_dataPrivate);
    return m_dataPrivate->viewCurrent;
}

//-----------------------------------------------------------------------

const struct EvtKey_t *CEvtOpcAreaDib::getEvtKey(void)
{
	assert_no_null(m_dataPrivate);
	assert(m_dataPrivate->type == CEvtOpcAreaDib::KEY);

	return m_dataPrivate->evtKey;
}

//-----------------------------------------------------------------------

void CEvtOpcAreaDib::setRedraw(void)
{
    assert_no_null(m_dataPrivate);
	m_dataPrivate->typeRedraw = CEvtOpcAreaDib::REDRAW;
}

//-----------------------------------------------------------------------

bool CEvtOpcAreaDib::redrawAllViews(void)
{
	bool isRedraw;
	
    assert_no_null(m_dataPrivate);
	switch (m_dataPrivate->typeRedraw)
	{
		case NO_REDRAW:
		
			isRedraw = false;
			break;
		 
		case REDRAW: 
		
			isRedraw = true;
			break;
			
		default_error();
	}
	
	return isRedraw;
}

//-----------------------------------------------------------------------

bool CEvtOpcAreaDib::isStart(void)
{
	bool isStart;
	
	assert_no_null(m_dataPrivate);
	
	switch(m_dataPrivate->type)
	{
		case CEvtOpcAreaDib::START:
			
			isStart = true;
			break;
			
		case CEvtOpcAreaDib::END: 
		case CEvtOpcAreaDib::MOUSE:
		case CEvtOpcAreaDib::TIME:
		case CEvtOpcAreaDib::KEY:
			
			isStart = false;
			break;
			
		default_error();
	}
	
	return isStart;
}

//-----------------------------------------------------------------------

bool CEvtOpcAreaDib::isEnd(void)
{
	bool isEnd;

	assert_no_null(m_dataPrivate);

	switch(m_dataPrivate->type)
	{
		case CEvtOpcAreaDib::END:

			isEnd = true;
			break;

		case CEvtOpcAreaDib::START:
		case CEvtOpcAreaDib::MOUSE:
		case CEvtOpcAreaDib::TIME:
		case CEvtOpcAreaDib::KEY:

			isEnd = false;
			break;

		default_error();
	}

	return isEnd;
}

//-----------------------------------------------------------------------

void CEvtOpcAreaDib::setMessageLeft(const char *text)
{
	assert_no_null(m_dataPrivate);
	assert_no_null(m_dataPrivate->statusLine);
	
	m_dataPrivate->statusLine->setTextLeft(text);
}

//-----------------------------------------------------------------------

void CEvtOpcAreaDib::setMessageRight(const char *text)
{
	assert_no_null(m_dataPrivate);
	assert_no_null(m_dataPrivate->statusLine);

	m_dataPrivate->statusLine->setTextRigth(text);
}
