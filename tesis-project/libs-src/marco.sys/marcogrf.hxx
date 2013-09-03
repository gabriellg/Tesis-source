#ifndef __MARCOGRF__
#define __MARCOGRF__

enum marcogrf_justificacion_horizontal_t
{
    MARCOGRF_IZQUIERDA,
    MARCOGRF_DERECHA,
    MARCOGRF_CENTRO
};

enum marcogrf_tipo_separador_t
{
    MARCOGRF_SEPARADOR_HORIZONTAL,
    MARCOGRF_SEPARADOR_VERTICAL
};

enum marcogrf_tipo_fecha_t
{
    MARCOGRF_FECHA,
    MARCOGRF_HORA,
    MARCOGRF_FECHA_Y_HORA
};

enum marcogrf_tipo_aviso_t
{
    MARCOGRF_PREGUNTA,
    MARCOGRF_MENSAJE
};

typedef struct item_aplicacion_t *(*marcogrf_FPtr_inicia_aplicacion)(int argc, char *argv[]);

typedef struct item_implementacion_dialogo_t *(*marcogrf_FPtr_get_dialogo)(struct item_aplicacion_t *item_aplicacion);
typedef void (*marcogrf_FPtr_lanzar_aplicacion)(
                        struct item_aplicacion_t *item_aplicacion,
                        struct item_implementacion_dialogo_t **,
                        const char *titulo_aplicacion,
                        struct callback_t *funcion_eventos_teclado,
                        struct item_implementacion_menubar_t **menubar,
                        struct item_implementacion_toolbar_t **toolbar,
                        struct item_implementacion_t **area);
typedef void (*marcogrf_FPtr_cerra_aplicacion)(struct item_aplicacion_t *datos_aplicacion);
typedef void (*marcogrf_FPtr_destruye_datos_aplicacion)(struct item_aplicacion_t **datos_aplicacion);

typedef struct item_implementacion_t *(*marcogrf_FPtr_crea_boton)(struct item_implementacion_dialogo_t *dlg_padre, const char *texto, struct callback_t *callback);
typedef struct item_implementacion_t *(*marcogrf_FPtr_crea_etiqueta)(struct item_implementacion_dialogo_t *dlg_padre, const char *texto, enum marcogrf_justificacion_horizontal_t tipo_justificacion);
typedef void (*marcogrf_FPtr_set_texto_etiqueta)(struct item_implementacion_t *item_implementacion, const char *texto);
typedef struct item_implementacion_t *(*marcogrf_FPtr_crea_linea_separacion)(struct item_implementacion_dialogo_t *dlg_padre, enum marcogrf_tipo_separador_t tipo_seperador);

typedef void (*marcogrf_ha_cambiado_opcion)(struct combobox_t *combobox, unsigned long opcion_ha_cambiado);
typedef struct item_implementacion_t *(*marcogrf_FPtr_crea_combobox)(
                        struct item_implementacion_dialogo_t *dlg_padre,
                        struct combobox_t *combobox,
                        marcogrf_ha_cambiado_opcion func_ha_cambiado,
                        const ArrString *texto_opciones,
                        unsigned long ind_valor);
typedef unsigned long (*marcogrf_FPtr_get_opcion_combobox)(struct item_implementacion_t *combobox);

typedef bool (*marco_FPtr_comprobar)(const char *texto, struct edit_t *dato, char **mensaje_error);
typedef struct item_implementacion_t *(*marcogrf_FPtr_crea_edit)(
                                                    struct item_implementacion_dialogo_t *dlg_padre,
                                                    const char *texto, const char *filtro_teclas,
                                                    struct edit_t *dato,
                                                    bool es_multilinea, bool editable,
                                                    unsigned long ancho, unsigned long alto,
                                                    marco_FPtr_comprobar func_filtra);
typedef char *(*marcogrf_FPtr_get_texto)(struct item_implementacion_t *edit);

typedef void (*marcogrf_FPtr_setTime)(struct item_implementacion_t *item_widget, unsigned long timeout_miliseconds, struct callback_t **callback);

typedef struct item_implementacion_t *(*marcogrf_FPtr_crea_lista)(
                            struct item_implementacion_dialogo_t *dlg_padre,
                            const ArrString *titulos,
                            const ArrPointer(ArrString) *datos,
                            bool hay_seleccion,
                            unsigned long indice_seleccionado);
typedef void (*marcogrf_FPtr_append_lista)(struct item_implementacion_t *implementacion, const ArrString *datos_fila);
typedef void (*marcogrf_FPtr_remove_lista)(struct item_implementacion_t *implementacion, unsigned long fila);
typedef void (*marcogrf_FPtr_cambia_fila)(struct item_implementacion_t *implementacion, unsigned long fila, const ArrString *datos_fila);
typedef bool (*marcogrf_FPtr_get_fila_activa)(struct item_implementacion_t *implementacion, unsigned long *fila);
typedef void (*marcogrf_FPtr_set_fila_activa)(struct item_implementacion_t *implementacion, unsigned long fila);

typedef void (*marcogrf_FPtr_mostrar)(struct item_implementacion_t *componente);

typedef bool (*marcogrf_FPtr_se_puede_aceptar)(struct dialogo_t *dialogo, char **mensaje);
typedef void (*marcogrf_FPtr_aceptar)(struct dialogo_t *dialogo);

typedef struct item_implementacion_dialogo_t *(*marcogrf_FPtr_crea_dialogo)(struct item_implementacion_dialogo_t *dlg_padre);

typedef void (*marcogrf_FPtr_lanzar_dialogo_aceptar)(
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

typedef bool (*marcogrf_FPtr_lanzar_seleccion_fichero)(const char *titulo_dialogo, char **fichero_seleccionado, bool abrirFichero);

typedef void (*marcogrf_FPtr_lanza_aviso_pregunta)(
                                struct item_implementacion_dialogo_t *dialogo,
                                enum marcogrf_tipo_aviso_t tipo_aviso,
                                const char *mensaje, bool *aceptado);

typedef struct item_implementacion_t *(*marcogrf_FPtr_crea_fecha)(struct item_implementacion_dialogo_t *dlg_padre, enum marcogrf_tipo_fecha_t tipo_fecha, const struct fecha_t *fecha);
typedef struct fecha_t *(*marcogrf_FPtr_fecha)(struct item_implementacion_t *fecha);


#endif

