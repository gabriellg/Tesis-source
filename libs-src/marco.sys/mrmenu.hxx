#ifndef MRMENU_HXX_
#define MRMENU_HXX_

typedef struct item_implementacion_menubar_t *(*mrmenu_FPtr_crea_barra_menu)(
                        struct item_implementacion_dialogo_t *dlg_padre);
                        
typedef struct item_implementacion_menu_t *(*mrmenu_FPtr_crea_menu)(
                        struct item_implementacion_dialogo_t *dlg_padre);
                        
typedef void (*mrmenu_FPtr_append_submenu)(
                        struct item_implementacion_dialogo_t *dlg_padre, 
                        const char *texto,
                        struct item_implementacion_menubar_t *menubar,
                        struct item_implementacion_menu_t *submenu);
                        
typedef void (*mrmenu_FPtr_append_opcion)(
                        struct item_implementacion_dialogo_t *dlg_padre, 
                        struct item_implementacion_menu_t *menu_so,
                        const char *texto,
                        const class CImg *imageIcon_opc,
                        struct callback_t *callback);
                        
typedef void (*mrmenu_FPtr_append_separador)(
                        struct item_implementacion_dialogo_t *dlg_padre,
                        struct item_implementacion_menu_t *menu_so);

typedef struct item_implementacion_toolbar_t *(*mrmenu_FPtr_crea_toolbar)(struct item_implementacion_dialogo_t *dlg_padre);

typedef void (*mrmenu_FPtr_append_toolbar_opcion)(
                        struct item_implementacion_dialogo_t *dlg_padre, 
                        struct item_implementacion_toolbar_t *toolbar, 
                        const char *textoTooltip, const class CImg *imageIcon_opc,
                        struct callback_t *callback);

typedef void (*mrmenu_FPtr_append_toolbar_separador)(
                        struct item_implementacion_dialogo_t *dlg_padre,
                        struct item_implementacion_toolbar_t *toolbar); 

#endif /*MRMENU_HXX_*/
