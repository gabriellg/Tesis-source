// Definición de menu

struct menu_t *menu_crea(const char *texto);

void menu_append_opcion(struct menu_t *menu, struct OpcMenu_t **opcion);
