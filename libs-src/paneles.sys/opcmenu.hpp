// Define una opci�n del men�

struct OpcMenu_t *opcmenu_crea(const char *texto, struct rcpaccion_t **receptor_accion);

struct OpcMenu_t *opcmenu_crea_opcion_con_icono(const char *texto, class CImg **imageIcon, struct rcpaccion_t **receptor_accion);

struct OpcMenu_t *opcmenu_crea_separador();
