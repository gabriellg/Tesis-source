// Definición de menu

void menu_append_menubar(
                        const struct menu_t *menu,
                        struct marcogrf_t *marco,
                        struct dialogo_t *dialogo_padre,
                        struct item_implementacion_dialogo_t *dialogo_so,
                        struct item_implementacion_menubar_t *menubar_so);
						
void menu_destruye(struct menu_t **menu);

bool menu_hay_opciones_toolbar(const struct menu_t *menu);

void menu_append_menu_toolbar(
                        struct menu_t *menu,
                        struct marcogrf_t *marco,
                        struct item_implementacion_dialogo_t *dialogo_so,
                        struct item_implementacion_toolbar_t *toolbar_so);
