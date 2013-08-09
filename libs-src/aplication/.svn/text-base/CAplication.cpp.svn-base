#include "CAplication.hpp"

#include "CWorkspace.hpp"
#include "CStatusLine.hpp"

#include "asrtbas.h"
#include "strbas.h"
#include "dialogo.h"
#include "menu.h"
#include "menubar.h"
#include "rcpaccion.h"
#include "evtaccion.h"
#include "stdio.h"
#include "marcogrf.syh"
#include "etiqueta.h"
#include "panel.h"

#include "opcmenu.hpp"
#include "CImg.hpp"
#include "CAplication__salir.iig"

//---------------------------------------------------------

CAplication::CAplication(const char *titleAplication)
{
    m_withHandleArchive = false;
	m_titleAplication = strbas_copyString(titleAplication);
}

//---------------------------------------------------------

CAplication::~CAplication()
{
	strbas_destroy(&m_titleAplication);
}

//---------------------------------------------------------

static void i_nuevoFichero(struct evtaccion_t *evt)
{
	//TODO: Hacer nuevo fichero
}

//---------------------------------------------------------

static void i_abrirFichero(struct evtaccion_t *evt)
{
	struct dialogo_t *dlgFather;
	char *fichero_seleccionado;
	
	dlgFather = evtaccion_dialogo_padre(evt);
	
	fichero_seleccionado = NULL;
	if (dialogo_lanza_seleccion_fichero(
						dlgFather, "Abrir...", 
						&fichero_seleccionado) == true)
	{
		printf("Abrir: %s", fichero_seleccionado);
		strbas_destroy(&fichero_seleccionado);
	}
}

//---------------------------------------------------------

static void i_guardarFichero(struct evtaccion_t *evt)
{
	struct dialogo_t *dlgFather;
	char *fichero_seleccionado;
	
	dlgFather = evtaccion_dialogo_padre(evt);
	
	fichero_seleccionado = NULL;
	if (dialogo_lanza_seleccion_fichero_grabar(
						dlgFather, "Guardar como...", 
						&fichero_seleccionado) == true)
	{
		printf("Guardar como: %s\n", fichero_seleccionado);
		strbas_destroy(&fichero_seleccionado);
	}
}

//---------------------------------------------------------

static void i_salirAplicacion(struct evtaccion_t *evt)
{
	struct dialogo_t *dlgFather;
	
	dlgFather = evtaccion_dialogo_padre(evt);
	
	if (dialogo_pregunta(dlgFather, "Desea salir") == true)
		dialogo_cierra_frame(dlgFather);
}

//---------------------------------------------------------

static struct menu_t *i_createMenuArchive(class CWorkspace *workspace, bool withHandleArchive)
{
	struct menu_t *menu;
	struct rcpaccion_t *rcpAction;
	struct OpcMenu_t *option;
	class CImg *image;
	
    if (withHandleArchive == true)
    {
    	menu = menu_crea("Archivo");
    	
    	rcpAction = rcpaccion_crea_sin_destruye_class(i_nuevoFichero, CWorkspace, workspace);
    	option = opcmenu_crea("Nuevo", &rcpAction);
    	menu_append_opcion(menu, &option);
    
    	rcpAction = rcpaccion_crea_sin_destruye_class(i_abrirFichero, CWorkspace, workspace);
    	option = opcmenu_crea("Abrir...", &rcpAction);
    	menu_append_opcion(menu, &option);
    
    	rcpAction = rcpaccion_crea_sin_destruye_class(i_guardarFichero, CWorkspace, workspace);
    	option = opcmenu_crea("Guardar como...", &rcpAction);
    	menu_append_opcion(menu, &option);
    
    	option = opcmenu_crea_separador();
    	menu_append_opcion(menu, &option);
    }
    else
        menu = menu_crea("Aplicacion");
	
	rcpAction = rcpaccion_crea_sin_destruye_class(i_salirAplicacion, CWorkspace, workspace);

	image = cimg_load_local_png(salir);
	option = opcmenu_crea_opcion_con_icono("Salir", &image, &rcpAction);

	menu_append_opcion(menu, &option);

	return menu;	
}

//---------------------------------------------------------

static struct panel_t *i_panelPrincipal(class CWorkspace *workspace, class CStatusLine *statusLine)
{
	struct panel_t *panel;
	struct panel_t *panelStatusBar, *panelWorkspace;
	
	panelStatusBar = statusLine->panel();
	panelWorkspace = workspace->panel();
	
	panel_set_expand(panelWorkspace);
	panel_set_fill(panelWorkspace);
	
	panel = panel_crea_con_disposicion_vertical();
	
	panel_append_panel(panel, &panelWorkspace);
	panel_append_panel(panel, &panelStatusBar);
	
	return panel;
}

//---------------------------------------------------------

void CAplication::run(int argc, char **argv)
{
	class CWorkspace *workspace;
	struct marcogrf_t *frameAplication;
	struct panel_t *panelPrincipal;
	struct dialogo_t *dlgPrincipal;
	struct menubar_t *menubar;
	struct menu_t *menuArchive;
	class CStatusLine *statusLine;
	
	frameAplication = getFrameworkAplication();
    marcogrf_inicia_aplicacion(frameAplication, argc, argv);
    
    statusLine = new CStatusLine();
	
	workspace = createWorkspace();
	workspace->createData();
	
	menuArchive = i_createMenuArchive(workspace, m_withHandleArchive); 
	menubar = menubar_crea();
	menubar_append_menu(menubar, &menuArchive);
	workspace->appendMenus(menubar);
	workspace->setStatusLine(statusLine);
	
	panelPrincipal = i_panelPrincipal(workspace, statusLine);

    statusLine->setTextLeft("Left");
    statusLine->setTextRigth("Right");
	
	dlgPrincipal = dialogo_crea_frame(frameAplication, m_titleAplication, &panelPrincipal);
	
	dialogo_lanza_frame(dlgPrincipal, &menubar);

	workspace->close();
	delete workspace;
	
	delete statusLine;
	
	dialogo_destruye(&dlgPrincipal);
	marcogrf_destruye(&frameAplication);
}
