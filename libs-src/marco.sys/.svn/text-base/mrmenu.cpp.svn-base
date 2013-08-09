// Marco para generación de menus.

#include "mrmenu.syh"
#include "mrmenu.inl"
#include "mrmenu.tli"

#include "memory.h"

//---------------------------------------------------------------

struct mrmenu_t *mrmenu_create(
                        mrmenu_FPtr_crea_barra_menu func_crea_barra_menu,
                        mrmenu_FPtr_crea_menu func_crea_menu,
                        mrmenu_FPtr_append_submenu func_append_submenu,
                        mrmenu_FPtr_append_opcion func_append_opcion,
                        mrmenu_FPtr_append_separador func_append_separador,
                        mrmenu_FPtr_crea_toolbar func_crea_toolbar,
                        mrmenu_FPtr_append_toolbar_opcion func_append_toolbar_opcion,
                        mrmenu_FPtr_append_toolbar_separador func_append_toolbar_separador)
{
    struct mrmenu_t *menu;
    
    menu = MALLOC(struct mrmenu_t);

    menu->func_crea_barra_menu = func_crea_barra_menu;
    menu->func_crea_menu = func_crea_menu;
    menu->func_append_submenu = func_append_submenu;
    menu->func_append_opcion = func_append_opcion; 
    menu->func_append_separador = func_append_separador; 
    
    menu->func_crea_toolbar = func_crea_toolbar;
    menu->func_append_toolbar_opcion = func_append_toolbar_opcion;
    menu->func_append_toolbar_separador = func_append_toolbar_separador; 
    
    return menu;
}

//---------------------------------------------------------------

void mrmenu_destroy(struct mrmenu_t **gst_menu)
{
    FREE_T(gst_menu, struct mrmenu_t);
}
