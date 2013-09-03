// Dialogos de aceptar y cancelar

typedef void (*dialogo_FPtr_fichero_seleccionado)(struct dialogo_item_dato_t *dato, const char *fichero_seleccion);
#define DIALOGO_COMPRUEBA_FUNC_FICHERO_SELECCIONADO(funcion, tipo) (void)((void (*)(tipo

struct dialogo_t *dialogo_crea_frame(
							struct marcogrf_t *marco,
							const char *titulo, 
							struct panel_t **panel);

struct dialogo_t *dialogo_crea_dialogo(struct dialogo_t *dialogo_padre, const char *titulo, struct panel_t **panel);

void dialogo_destruye(struct dialogo_t **dialogo);

void dialogo_lanzar(struct dialogo_t *dialogo);
    
bool dialogo_lanzar_aceptado(struct dialogo_t *dialogo);

void dialogo_lanzar_sin_aceptar(struct dialogo_t *dialogo);
    
bool dialogo_lanza_seleccion_fichero(struct dialogo_t *dlg_padre, const char *titulo_dialogo, char **fichero_seleccionado);

bool dialogo_lanza_seleccion_fichero_grabar(struct dialogo_t *dlg_padre, const char *titulo_dialogo, char **fichero_seleccionado);
                        
void dialogo_lanza_frame(struct dialogo_t *dlg_frame, struct menubar_t **menubar);

void dialogo_cierra_frame(struct dialogo_t *dlg_frame);

bool dialogo_pregunta(struct dialogo_t *dlg_padre, const char *mensaje);

void dialogo_aviso(struct dialogo_t *dlg_padre, const char *mensaje);
