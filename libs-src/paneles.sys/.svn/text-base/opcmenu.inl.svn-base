// Define una opci�n del men�

struct marcogrf_t;

void opcmenu_destruye(struct OpcMenu_t **opcion);

void opcmenu_append_opcion_implementacion(
                        struct OpcMenu_t *opcion,
                        struct marcogrf_t *marco,
                        struct dialogo_t *dialogo_padre,
                        struct item_implementacion_dialogo_t *dialogo_so,
                        struct item_implementacion_menu_t *menu_so);

bool opcmenu_en_toolbar(const struct OpcMenu_t *opcion);

void opcmenu_append_opcion_toolbar(
                        struct OpcMenu_t *opcion,
                        struct marcogrf_t *marco,
                        struct item_implementacion_dialogo_t *dialogo_so, 
                        struct item_implementacion_toolbar_t *toolbar_so,
                        bool *hay_opciones_toolbar);
