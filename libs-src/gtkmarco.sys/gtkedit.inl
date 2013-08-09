// Boton para gtk

#include "marcogrf.hxx"

struct item_implementacion_t *gtkedit_crea_implementacion(
                        struct item_implementacion_dialogo_t *dlg_padre, 
                        const char *texto, 
                        const char *filtro_teclas, 
                        struct edit_t *dato, 
                        bool es_multilinea, bool editable,
                        unsigned long ancho, unsigned long alto,
                        marco_FPtr_comprobar func_filtra);

char *gtkedit_get_texto(struct item_implementacion_t *edit);
