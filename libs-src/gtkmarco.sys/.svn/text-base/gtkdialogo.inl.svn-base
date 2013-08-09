// Di�logo para gtk

#include "marcogrf.hxx"

struct item_implementacion_dialogo_t *gtkdialogo_crea_dialogo(struct item_implementacion_dialogo_t *dlg_padre);

void gtkdialogo_lanza_dialogo(
                        struct item_aplicacion_t *item_aplicacion,
                        struct item_implementacion_dialogo_t **dialogo,
                        const char *titulo,
                        struct item_implementacion_t **area_trabajo,
                        struct dialogo_t *dlg_padre,
                        struct callback_t *funcion_eventos_teclado,
                        bool con_boton_aceptar,
                        bool *aceptado,
                        marcogrf_FPtr_se_puede_aceptar func_se_puede_aceptar, 
                        marcogrf_FPtr_aceptar func_aceptar);

bool gtkdialogo_lanzar_seleccion_fichero(const char *titulo_dialogo, char **fichero_seleccionado, bool abrirFichero);

void gtkdialogo_lanza_mensaje_aviso(
                        struct item_implementacion_dialogo_t *dlg_padre,
                        enum marcogrf_tipo_aviso_t tipo_aviso,
                        const char *mensaje, bool *aceptado);

struct item_implementacion_dialogo_t *gtkdialogo_get_dialogo(struct item_aplicacion_t *datos_aplicacion);

void gtkdialogo_lanza_aplicacion(
                        struct item_aplicacion_t *datos_aplicacion,
                        struct item_implementacion_dialogo_t **dialogo_aplicacion,
                        const char *titulo,
                        struct callback_t *funcion_eventos_teclado,
                        struct item_implementacion_menubar_t **menubar,
                        struct item_implementacion_toolbar_t **toolbar,
                        struct item_implementacion_t **area_aplicacion);
