// Definici�n de menu

#include "menu.h"
#include "menu.inl"

#include "componente.inl"
#include "rcpaccion.inl"
#include "opcmenu.inl"

#include "callback.syh"
#include "marcogrf.syh"

#include "memory.h"
#include "asrtbas.h"
#include "strbas.h"
#include "array.h"

struct menu_t
{
	char *texto;
	ArrTipo(OpcMenu_t) *opciones;
};

//---------------------------------------------------------

static void i_integridad(const struct menu_t *menu)
{
	assert_no_null(menu);
	assert_no_null(menu->texto);
    assert_no_null(menu->opciones);
}

//---------------------------------------------------------

static struct menu_t *i_crea(
						const char *texto,
						ArrTipo(OpcMenu_t) **opciones)
{
	struct menu_t *menu;
		
	menu = MALLOC(struct menu_t);
	
	menu->texto = strbas_copyString(texto);
	menu->opciones = ASSIGN_PP(opciones, ArrTipo(OpcMenu_t));
	
	i_integridad(menu);

	return menu;
}
	
//---------------------------------------------------------

struct menu_t *menu_crea(const char *texto)
{
	ArrTipo(OpcMenu_t) *opciones;
	
	opciones = array_CreaPuntero(0, OpcMenu_t);
	
	return i_crea(texto, &opciones);
}

//---------------------------------------------------------

void menu_destruye(struct menu_t **menu)
{
    assert_no_null(menu);
    i_integridad(*menu);
    
	strbas_destroy(&(*menu)->texto);
    array_Destruye(OpcMenu_t, &(*menu)->opciones, opcmenu_destruye);

    FREE_T(menu, struct menu_t);
}

//---------------------------------------------------------

void menu_append_opcion(struct menu_t *menu, struct OpcMenu_t **opcion)
{
	i_integridad(menu);
    assert_no_null(opcion);
    
	array_Anyadir(OpcMenu_t, menu->opciones, *opcion);
    *opcion = NULL;
}

//---------------------------------------------------------

void menu_append_menubar(
                        const struct menu_t *menu,
						struct marcogrf_t *marco,
						struct dialogo_t *dialogo_padre,
                        struct item_implementacion_dialogo_t *dialogo_so,
                        struct item_implementacion_menubar_t *menubar_so)
{
	struct item_implementacion_menu_t *menu_so;
	unsigned long i, num_opciones;

    i_integridad(menu);

	menu_so = marcogrf_crea_menu(marco, dialogo_so);

	num_opciones = array_numElementos(OpcMenu_t, menu->opciones);

	for (i = 0; i < num_opciones; i++)
	{
		struct OpcMenu_t *opcion;
			
		opcion = array_Get(OpcMenu_t, menu->opciones, i);
    
        opcmenu_append_opcion_implementacion(
                        opcion, marco, dialogo_padre,
                        dialogo_so, menu_so);
	}

	marcogrf_append_submenu(marco, dialogo_so, menu->texto, menubar_so, menu_so);
}

//---------------------------------------------------------

bool menu_hay_opciones_toolbar(const struct menu_t *menu)
{
    return array_Cumple(OpcMenu_t, menu->opciones, opcmenu_en_toolbar, NULL);
}

//---------------------------------------------------------

void menu_append_menu_toolbar(
                        struct menu_t *menu,
                        struct marcogrf_t *marco,
                        struct item_implementacion_dialogo_t *dialogo_so,
                        struct item_implementacion_toolbar_t *toolbar_so)
{
    bool hay_opciones_toolbar;
    unsigned long i, num_opciones;

    i_integridad(menu);

    hay_opciones_toolbar = false;

    num_opciones = array_numElementos(OpcMenu_t, menu->opciones);

    for (i = 0; i < num_opciones; i++)
    {
        struct OpcMenu_t *opcion;
            
        opcion = array_Get(OpcMenu_t, menu->opciones, i);
        opcmenu_append_opcion_toolbar(opcion, marco, dialogo_so, toolbar_so, &hay_opciones_toolbar);
    }
}
