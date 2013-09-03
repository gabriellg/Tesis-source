// Implementacion para gtk de menu

struct item_implementacion_menubar_t *gtkmenu_crea_barra_menu(struct item_implementacion_dialogo_t *dlg_padre);

struct item_implementacion_menu_t *gtkmenu_crea_menu(struct item_implementacion_dialogo_t *dlg_padre);

void gtkmenu_append_submenu(
                        struct item_implementacion_dialogo_t *dlg_padre, 
                        const char *texto,
                        struct item_implementacion_menubar_t *menubar,
                        struct item_implementacion_menu_t *submenu);

void gtkmenu_append_opcion(
                        struct item_implementacion_dialogo_t *dlg_padre, 
                        struct item_implementacion_menu_t *menu_so,
                        const char *texto,
                        const class CImg *imageIcon_opc,
                        struct callback_t *callback);

void gtkmenu_append_separador(
                        struct item_implementacion_dialogo_t *dlg_padre, 
                        struct item_implementacion_menu_t *menu_so);

struct item_implementacion_toolbar_t *gtkmenu_crea_toolbar(struct item_implementacion_dialogo_t *dlg_padre);

void gtkmenu_appen_opcion_toolbar(
                        struct item_implementacion_dialogo_t *dlg_padre, 
                        struct item_implementacion_toolbar_t *toolbar_so,
                        const char *textTooltip, 
                        const class CImg *imageIcon,
                        struct callback_t *callback);

void gtkmenu_appen_separator_toolbar(
                        struct item_implementacion_dialogo_t *dlg_padre, 
                        struct item_implementacion_toolbar_t *toolbar_so);
