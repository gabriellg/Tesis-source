// Definici�n de una barra de men�s

#include "menubar.h"
#include "menubar.inl"

#include "menu.inl"

#include "marcogrf.syh"

#include "array.h"
#include "asrtbas.h"
#include "memory.h"

struct menubar_t
{
    ArrTipo(menu_t) *menus;
};

//---------------------------------------------------------

static void i_integridad(const struct menubar_t *menubar)
{
    assert_no_null(menubar);
    assert_no_null(menubar->menus);
};

//---------------------------------------------------------

static struct menubar_t *i_crea(ArrTipo(menu_t) **menus)
{
    struct menubar_t *menubar;
        
    menubar = MALLOC(struct menubar_t);
        
    menubar->menus = ASSIGN_PP(menus, ArrTipo(menu_t));
    
    i_integridad(menubar);
        
    return menubar;
}

//---------------------------------------------------------

struct menubar_t *menubar_crea(void)
{
    ArrTipo(menu_t) *menus;
	
	menus = array_CreaPuntero(0, menu_t);
	
	return i_crea(&menus);
}

//---------------------------------------------------------

void menubar_destruye(struct menubar_t **menubar)
{
	assert_no_null(menubar);
	i_integridad(*menubar);
	
    array_Destruye(menu_t, &(*menubar)->menus, menu_destruye);
	
	FREE_T(menubar, struct menubar_t);
}

//---------------------------------------------------------

void menubar_append_menu(struct menubar_t *menubar, struct menu_t **menu)
{
    i_integridad(menubar);
    assert_no_null(menu);
    
	array_Anyadir(menu_t, menubar->menus, *menu);
    *menu = NULL;
}

//---------------------------------------------------------

struct item_implementacion_menubar_t *menubar_crea_menu_bar(
                        struct menubar_t *menubar,
                        struct marcogrf_t *marco,
                        struct dialogo_t *dialogo_padre,
                        struct item_implementacion_dialogo_t *dialogo_so)
{
	struct item_implementacion_menubar_t *menubar_so;
	unsigned long i, num_menus;
	
	i_integridad(menubar);
	
	num_menus = array_numElementos(menu_t, menubar->menus);
	menubar_so = marcogrf_crea_barra_menu(marco, dialogo_so);

	for (i = 0; i < num_menus; i++)
	{
		const struct menu_t *menu;
			
		menu = array_Get(menu_t, menubar->menus, i);
        menu_append_menubar(menu, marco, dialogo_padre, dialogo_so, menubar_so);
	}
	
	return menubar_so;
}

//---------------------------------------------------------

struct item_implementacion_toolbar_t *menubar_crea_toolbar(
                        struct menubar_t *menubar,
                        struct marcogrf_t *marco,
                        struct item_implementacion_dialogo_t *dialogo_so)
{
    struct item_implementacion_toolbar_t *toolbar_so;
        
    i_integridad(menubar);
    
    if (array_Cumple(menu_t, menubar->menus, menu_hay_opciones_toolbar, NULL) == true)
    {
        unsigned long i, num_menus;
        
        num_menus = array_numElementos(menu_t, menubar->menus);
        
        toolbar_so = marcogrf_crea_toolbar(marco, dialogo_so);
    
        for (i = 0; i < num_menus; i++)
        {
            struct menu_t *menu;
                
            menu = array_Get(menu_t, menubar->menus, i);
            menu_append_menu_toolbar(menu, marco, dialogo_so, toolbar_so);
        }
    }
    else
        toolbar_so = NULL;
    
    return toolbar_so;
}
