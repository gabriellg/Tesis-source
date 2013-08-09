#include "CWorkspace.hpp"

#include "CView.hpp"
#include "CDataWorkspace.hpp"

#include "CEvtOpcAreaDib.hpp"
#include "CRcpOpcAreaDib.hpp"

#include "asrtbas.h"
#include "array.h"
#include "menu.h"
#include "menubar.h"
#include "panel.h"
#include "areadibujo.hpp"
#include "rcpmouse.h"
#include "evtaccion.h"
#include "rcpaccion.h"
#include "rcpkey.h"
#include "memory.h"

#include "evtmouse.h"
#include "opcmenu.hpp"
#include "CImg.hpp"

struct i_dataOption_t
{
	class CWorkspace *workspace;
	class CRcpOpcAreaDib *rcpOption;
};

struct prv_dataPrivateWorspace_t
{
	class CView *viewCurrent;
	class CDataWorkspace *dataWorkspace;
	class CStatusLine *statusLine;
	ArrTipo(menu_t) *menus;
	ArrTipo(areaDibujo_t) *areasDib;
	
	class CRcpOpcAreaDib *rcpRunning;
};

static unsigned long i_FRAME_FOR_MILISECONDS = 100;

//---------------------------------------------------------

static struct i_dataOption_t *i_createDataOption(
						class CWorkspace *workspace,
						class CRcpOpcAreaDib **rcpOption)
{
	struct i_dataOption_t *dataOption;
	
	dataOption = MALLOC(struct i_dataOption_t);
	
	dataOption->workspace = workspace;
	dataOption->rcpOption = ASSIGN_PP_NO_NULL(rcpOption, class CRcpOpcAreaDib);
	
	return dataOption;
}

//---------------------------------------------------------

static void i_destroyDataOption(struct i_dataOption_t **dataOption)
{
	assert_no_null(dataOption);
	assert_no_null(*dataOption);
	
	delete (*dataOption)->rcpOption;
	
	FREE_T(dataOption, struct i_dataOption_t);
}

//---------------------------------------------------------

static struct prv_dataPrivateWorspace_t *prv_create(
					class CView *viewCurrent,
					class CDataWorkspace *dataWorkspace,
					class CStatusLine *statusLine,
					ArrTipo(menu_t) *menus,
					ArrTipo(areaDibujo_t) *areasDib,
					class CRcpOpcAreaDib *rcpRunning)
{
	struct prv_dataPrivateWorspace_t *dataPrivate;
	
	dataPrivate = MALLOC(struct prv_dataPrivateWorspace_t);
	
	dataPrivate->viewCurrent = viewCurrent; 
	dataPrivate->dataWorkspace = dataWorkspace; 
	dataPrivate->statusLine = statusLine;
	dataPrivate->menus = menus;
	dataPrivate->areasDib= areasDib;
	dataPrivate->rcpRunning = rcpRunning;
	
	return dataPrivate;
}

//---------------------------------------------------------

static void prv_destroy(struct prv_dataPrivateWorspace_t **dataPrivate)
{
	assert_no_null(dataPrivate);
	assert_no_null(*dataPrivate);

	assert((*dataPrivate)->viewCurrent == NULL);
	assert((*dataPrivate)->dataWorkspace == NULL);
	assert((*dataPrivate)->menus == NULL);
	
	if ((*dataPrivate)->areasDib != NULL)
		array_Destruye(areaDibujo_t, &(*dataPrivate)->areasDib, NULL);
	
	FREE_T(dataPrivate, struct prv_dataPrivateWorspace_t);
}

//---------------------------------------------------------

CWorkspace::CWorkspace()
{
	class CView *viewCurrent;
	class CDataWorkspace *dataWorkspace;
	ArrTipo(menu_t) *menus;
	ArrTipo(areaDibujo_t) *areasDib;
	class CRcpOpcAreaDib *rcpRunning;
	class CStatusLine *statusLine;
	
	statusLine = NULL;
	dataWorkspace = NULL;
	viewCurrent = NULL;
	rcpRunning = NULL;

	menus = NULL;
	areasDib = NULL;
	
	m_dataPrivate = prv_create(viewCurrent, dataWorkspace, statusLine, menus, areasDib, rcpRunning);
}

//---------------------------------------------------------

static void i_destroy(class CView **viewCurrent, class CDataWorkspace **dataWorkspace)
{
	assert_no_null(viewCurrent);
	assert_no_null(*viewCurrent);
	assert_no_null(dataWorkspace);
	assert_no_null(*dataWorkspace);
	
	delete *viewCurrent;
	*viewCurrent = NULL;
	
	delete *dataWorkspace;
	*dataWorkspace = NULL; 
}

//---------------------------------------------------------

CWorkspace::~CWorkspace()
{
	prv_destroy(&m_dataPrivate);
}

//---------------------------------------------------------

void CWorkspace::appendMenu(const char *textMenu)
{
	struct menu_t *menu;
	
	assert_no_null(m_dataPrivate);
	
	menu = menu_crea(textMenu);
	array_Anyadir(menu_t, m_dataPrivate->menus, menu);
}

//---------------------------------------------------------

static void i_startOption(struct evtaccion_t *evt)
{
	struct i_dataOption_t *dataOption;
	struct dialogo_t *dlgFather;
	
	dataOption = evtaccion_datos(struct i_dataOption_t, evt);
	dlgFather = evtaccion_dialogo_padre(evt);
	
	assert_no_null(dataOption);
	assert_no_null(dataOption->workspace);
	
	dataOption->workspace->startOption(dlgFather, dataOption->rcpOption);
}

//---------------------------------------------------------

static void prv_appendOptionIntoLastMenu(ArrTipo(menu_t) *menus, struct OpcMenu_t **option) 
{
	unsigned long numMenus;
    struct menu_t *menu;
	
	numMenus = array_numElementos(menu_t, menus);
	assert(numMenus > 0);
	
	menu = array_Get(menu_t, menus, numMenus - 1);
	menu_append_opcion(menu, option);
}

//---------------------------------------------------------

void CWorkspace::appendOptionIntoLastMenu(class CRcpOpcAreaDib *rcpOption)
{
	const char *txtMenu;
	struct OpcMenu_t *option;
	struct i_dataOption_t *dataOption;
	struct rcpaccion_t *rcp_startOption;

	assert_no_null(m_dataPrivate);
	
	txtMenu = rcpOption->getTextMenu();
	
	dataOption = i_createDataOption(this, &rcpOption);
	rcp_startOption = rcpaccion_crea(i_startOption, i_destroyDataOption, i_dataOption_t, &dataOption);
	option = opcmenu_crea(txtMenu, &rcp_startOption);

	prv_appendOptionIntoLastMenu(m_dataPrivate->menus, &option);
}

//---------------------------------------------------------

void CWorkspace::appendOptionWithIconIntoLastMenu(class CImg **imageIcon, class CRcpOpcAreaDib *rcpOption)
{
	const char *textOption; 
    struct OpcMenu_t *option;
    struct i_dataOption_t *dataOption;
    struct rcpaccion_t *rcp_startOption;

	assert_no_null(m_dataPrivate);
	
    textOption = rcpOption->getTextMenu();
    
    dataOption = i_createDataOption(this, &rcpOption);
    rcp_startOption = rcpaccion_crea(i_startOption, i_destroyDataOption, i_dataOption_t, &dataOption);
    
    option = opcmenu_crea_opcion_con_icono(textOption, imageIcon, &rcp_startOption);
    
	prv_appendOptionIntoLastMenu(m_dataPrivate->menus, &option);
}

//---------------------------------------------------------

void CWorkspace::appendSeparatorIntoLastMenu(void)
{
    struct OpcMenu_t *optSeparator;

	assert_no_null(m_dataPrivate);
    
    optSeparator = opcmenu_crea_separador();
	prv_appendOptionIntoLastMenu(m_dataPrivate->menus, &optSeparator);
}

//---------------------------------------------------------

void CWorkspace::createData(void)
{
	assert_no_null(m_dataPrivate);
	assert(m_dataPrivate->dataWorkspace == NULL);
	assert(m_dataPrivate->viewCurrent == NULL);
	
	m_dataPrivate->dataWorkspace = createDataWorkspace();
	m_dataPrivate->viewCurrent = createView(m_dataPrivate->dataWorkspace);
}

//---------------------------------------------------------

void CWorkspace::close(void)
{
	assert_no_null(m_dataPrivate);
	i_destroy(&m_dataPrivate->viewCurrent, &m_dataPrivate->dataWorkspace);
}

//---------------------------------------------------------

static void i_executeEvtMouse(
						const struct EvtMouse_t *evtMouse,
						CWorkspace *workspace)
{
	assert_no_null(workspace);

	/*	
	switch(evtmouse_type(evtMouse))
	{
    	case EVTMOUSE_OUT:
    	{
    		printf("Sale rat�n\n");
    		break;
    	}
    	case EVTMOUSE_IN:
    	{
    		printf("Entra rat�n\n");
    		break;
    	}
    	case EVTMOUSE_MOVE:
    	{
    		double x, y;
    		
    		evtmouse_position(evtMouse, &x, &y);
    		printf("Mueve %.2f, %.2f\n", x, y);
    		break;
    	}
    	case EVTMOUSE_PULSE_LEFT:
    	{
    		double x, y;
    		
    		evtmouse_position(evtMouse, &x, &y);
    		printf("Boton Izquierdo %.2f, %.2f\n", x, y);
    		break;
    	}
    	case EVTMOUSE_PULSE_RIGHT:
    	{
    		double x, y;
    		
    		evtmouse_position(evtMouse, &x, &y);
    		printf("Boton Derecho %.2f, %.2f\n", x, y);
    		break;
    	}
    	default_error();
	}
	*/
}

//---------------------------------------------------------

static void i_setupReceptorMouse(ArrTipo(areaDibujo_t) *areasDibujo, CWorkspace *workspace)
{
	unsigned long i, num;
	
	num = array_numElementos(areaDibujo_t, areasDibujo);
	
	for (i = 0; i < num; i++)
	{
		struct areaDibujo_t *areaDibujo;
		struct RcpMouse_t *rcpMouse;
		
		areaDibujo = array_Get(areaDibujo_t, areasDibujo, i);
		
		rcpMouse = rcpmouse_create(CWorkspace, workspace, i_executeEvtMouse);
		areadibujo_appendReceptorMouse(areaDibujo, &rcpMouse);
	}
}

//---------------------------------------------------------

static void i_redrawAllViews(ArrTipo(areaDibujo_t) *areasDib)
{
	unsigned long i, num;

	num = array_numElementos(areaDibujo_t, areasDib);

	for (i = 0; i < num; i++)
	{
		struct areaDibujo_t *areaDib;

		areaDib = array_Get(areaDibujo_t, areasDib, i);
		areadibujo_redraw(areaDib);
	}
}

//---------------------------------------------------------

static void i_sendEventOption(
						struct dialogo_t *dlgFather,
						class CDataWorkspace *dataWorkspace,
						class CStatusLine *statusLine,
                        class CView *view,
						enum CEvtOpcAreaDib::TypeEvent_t typeEvent,
						struct EvtMouse_t *evtMouse,
						struct EvtKey_t *evtKey,
						ArrTipo(areaDibujo_t) *areasDib,
						bool *isEnded_opc,
						class CRcpOpcAreaDib **rcpOption)
{
	bool ended;
	class CEvtOpcAreaDib *evtAreaDib;
	enum CRcpOpcAreaDib::Result_t result;

	assert_no_null(rcpOption);
	assert_no_null(*rcpOption);

	evtAreaDib = new CEvtOpcAreaDib(typeEvent, dataWorkspace, statusLine, view, dlgFather, evtMouse, evtKey);

	result = (*rcpOption)->sendEvent(evtAreaDib);

	if (evtAreaDib->redrawAllViews() == true)
		i_redrawAllViews(areasDib);

	delete evtAreaDib;

	switch (result)
	{
		case CRcpOpcAreaDib::END:

			*rcpOption = NULL;
			ended = true;
			break;

		case CRcpOpcAreaDib::CONTINUE:

			ended = false;
			break;

		default_error();
	}

	if (isEnded_opc != NULL)
		*isEnded_opc = ended;
}

//---------------------------------------------------------

static void i_on_time(struct evtaccion_t *evt)
{
	struct prv_dataPrivateWorspace_t *dataPrivate;

	dataPrivate = evtaccion_datos(struct prv_dataPrivateWorspace_t, evt);
	assert_no_null(dataPrivate);

	if (dataPrivate->rcpRunning != NULL)
	{
		struct dialogo_t *dlgFather;

		dlgFather = evtaccion_dialogo_padre(evt);
		i_sendEventOption(
						dlgFather,
						dataPrivate->dataWorkspace, dataPrivate->statusLine, dataPrivate->viewCurrent,
						CEvtOpcAreaDib::TIME,
						NULL, //evtMouse,
						NULL, //evtKey,
						dataPrivate->areasDib,
						NULL, //isEnded_opc,
						&dataPrivate->rcpRunning);
	}
}

//---------------------------------------------------------

static void i_onKeyScene(struct EvtKey_t *evtKey, struct prv_dataPrivateWorspace_t *dataPrivate)
{
	if (dataPrivate->rcpRunning != NULL)
	{
		struct dialogo_t *dlgFather;

		dlgFather = NULL;

		i_sendEventOption(
						dlgFather,
						dataPrivate->dataWorkspace,
						dataPrivate->statusLine,
						dataPrivate->viewCurrent,
						CEvtOpcAreaDib::KEY,
						NULL, //evtMouse,
						evtKey,
						dataPrivate->areasDib,
						NULL, //isEnded_opc,
						&dataPrivate->rcpRunning);
	}
}

//---------------------------------------------------------

struct panel_t *CWorkspace::panel(void)
{
	struct panel_t *panel;
	struct rcpaccion_t *rcpaccion_time;
	struct RcpKey_t *rcpKey;
	
	assert_no_null(m_dataPrivate);
	
	assert(m_dataPrivate->areasDib == NULL);
	assert_no_null(m_dataPrivate->viewCurrent);
	
	panel = m_dataPrivate->viewCurrent->panel(m_dataPrivate->dataWorkspace);
	
	m_dataPrivate->areasDib = panel_componentsOf(panel, areaDibujo_t);
	i_setupReceptorMouse(m_dataPrivate->areasDib, this);
	
	rcpaccion_time = rcpaccion_crea_sin_destruye(i_on_time, prv_dataPrivateWorspace_t , m_dataPrivate);
    panel_append_time(panel, i_FRAME_FOR_MILISECONDS, &rcpaccion_time);

    rcpKey = rcpkey_create(prv_dataPrivateWorspace_t , m_dataPrivate, i_onKeyScene);
    panel_appendReceptorKeys(panel, &rcpKey);

	return panel;
}

//---------------------------------------------------------

static bool i_finalizeOption(
						struct dialogo_t *dlgFather, 
						class CDataWorkspace *dataWorkspace,
						class CStatusLine *statusLine,
                        class CView *view,
						ArrTipo(areaDibujo_t) *areasDib,
						class CRcpOpcAreaDib **rcpOption)
{
	bool ended;
	
	assert_no_null(rcpOption);
	
	if (*rcpOption != NULL)
	{
		i_sendEventOption(
						dlgFather, dataWorkspace, statusLine, view, 
						CEvtOpcAreaDib::END,
						NULL, //evtMouse,
						NULL, //evtKey,
						areasDib,
						&ended,
						rcpOption);
	}
	else
		ended = true;
		
	return ended;
}

//---------------------------------------------------------

static void i_startOption(
						struct dialogo_t *dlgFather, 
						class CDataWorkspace *dataWorkspace,
						class CStatusLine *statusLine,
                        class CView *view,
						ArrTipo(areaDibujo_t) *areasDib,
						class CRcpOpcAreaDib **rcpOption)
{
	i_sendEventOption(
					dlgFather, dataWorkspace, statusLine, view, 
					CEvtOpcAreaDib::START,
					NULL, //evtMouse,
					NULL, //evtKey,
					areasDib,
					NULL, //isEnded
					rcpOption);
}

//---------------------------------------------------------

void CWorkspace::startOption(struct dialogo_t *dlgFather, class CRcpOpcAreaDib *rcpOption)
{
	bool itcanStartOption;
	
	assert_no_null(m_dataPrivate);

	if (m_dataPrivate->rcpRunning != NULL)
		itcanStartOption = i_finalizeOption(
								dlgFather, 
								m_dataPrivate->dataWorkspace, 
								m_dataPrivate->statusLine, 
								m_dataPrivate->viewCurrent, 
								m_dataPrivate->areasDib,
								&m_dataPrivate->rcpRunning);
	else
		itcanStartOption = true;
		
	if (itcanStartOption == true)
	{
		class CRcpOpcAreaDib *rcpOpcInitial;

		assert(m_dataPrivate->rcpRunning == NULL);

		rcpOpcInitial = rcpOption;

		i_startOption(
					dlgFather,
					m_dataPrivate->dataWorkspace,
					m_dataPrivate->statusLine,
					m_dataPrivate->viewCurrent,
					m_dataPrivate->areasDib,
					&rcpOpcInitial);

		m_dataPrivate->rcpRunning = rcpOpcInitial;
	}
}

//---------------------------------------------------------

void CWorkspace::setStatusLine(class CStatusLine *statusLine)
{
	assert_no_null(m_dataPrivate);
	assert(m_dataPrivate->statusLine == NULL);
	
	m_dataPrivate->statusLine = statusLine;
}

//---------------------------------------------------------

void CWorkspace::appendMenus(struct menubar_t *menubar)
{
	unsigned long i, num;
	
	assert_no_null(m_dataPrivate);
	assert(m_dataPrivate->menus == NULL);
	
	m_dataPrivate->menus = array_CreaPuntero(0, menu_t);
	
	defineMenu();
	
	num = array_numElementos(menu_t, m_dataPrivate->menus);
	
	for (i = 0; i < num; i++)
	{
		struct menu_t *menu;
		
		menu = array_Get(menu_t, m_dataPrivate->menus, i);
		menubar_append_menu(menubar, &menu);
	}
	
	array_Destruye(menu_t, &m_dataPrivate->menus, NULL);
}
