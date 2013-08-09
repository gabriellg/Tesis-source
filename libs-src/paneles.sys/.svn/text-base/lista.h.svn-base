// Control lista.c

#include "lista.hxx"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct panel_t *lista_nousar_crea(
                    bool forzar_seleccion,
                    unsigned long *ind_elemento_seleccionado,
                    const char *tipo,
                    unsigned long *id_nuevo,
                    unsigned long ancho, unsigned long alto,
                    bool trata_con_datos_originales,
                    struct Array_t **datos,
                    lista_FPtr_crea_defecto func_crea_defecto,
                    lista_FPtr_panel func_panel,
                    lista_FPtr_copia func_copia,
                    lista_FPtr_destruye func_destruye,
                    bool con_datos_extra,
                    const struct lista_item_datos_extra_t *datos_extra,
                    lista_FPtr_panel_con_datos_extra func_panel_con_datos_extra,
                    struct lista_t **plista);

#define lista_crea_con_datos_originales(tipo, datos, ancho, alto, id_nuevo, func_crea_defecto, func_panel, func_copia, func_destruye, lista)\
    (\
        (void)((ArrTipo(tipo) **)datos == datos),\
        (void)((Lista(tipo) **)lista == lista),\
        LISTA_COMPRUEBA_CREA_DEFECTO(func_crea_defecto, tipo),\
        LISTA_COMPRUEBA_PANEL(func_panel, tipo),\
        LISTA_COMPRUEBA_COPIA(func_copia, tipo),\
        LISTA_COMPRUEBA_DESTRUYE(func_destruye, tipo),\
        lista_nousar_crea(\
                    false, NULL, #tipo, id_nuevo, ancho, alto, true,\
                    (struct Array_t **)datos,\
                    (lista_FPtr_crea_defecto)func_crea_defecto,\
                    (lista_FPtr_panel)func_panel,\
                    (lista_FPtr_copia)func_copia,\
                    (lista_FPtr_destruye)func_destruye,\
                    false, NULL, NULL,\
                    (struct lista_t **)lista)\
    )

#define lista_crea_con_datos_originales_y_dato_extra(\
            tipo, datos, tipo_datos_extra, datos_extra, ancho, alto, id_nuevo,\
            func_crea_defecto, func_panel,\
            func_copia, func_destruye, lista)\
    (\
        (void)((ArrTipo(tipo) **)datos == datos),\
        (void)((Lista(tipo) **)lista == lista),\
        LISTA_COMPRUEBA_CREA_DEFECTO(func_crea_defecto, tipo),\
        LISTA_COMPRUEBA_PANEL_DATOS_EXTRA(func_panel, tipo_datos_extra, tipo),\
        LISTA_COMPRUEBA_COPIA(func_copia, tipo),\
        LISTA_COMPRUEBA_DESTRUYE(func_destruye, tipo),\
        lista_nousar_crea(\
                    false, NULL, #tipo, id_nuevo, ancho, alto, true,\
                    (struct Array_t **)datos,\
                    (lista_FPtr_crea_defecto)func_crea_defecto,\
                    NULL,\
                    (lista_FPtr_copia)func_copia,\
                    (lista_FPtr_destruye)func_destruye,\
                    true,\
                    (const struct lista_item_datos_extra_t *)datos_extra,\
                    (lista_FPtr_panel_con_datos_extra)func_panel,\
                    (struct lista_t **)lista)\
    )

#define lista_crea(tipo, datos, ancho, alto, id_nuevo, func_crea_defecto, func_panel, func_copia, func_destruye, lista)\
    (\
        (void)((ArrTipo(tipo) **)datos == datos),\
        (void)((Lista(tipo) **)lista == lista),\
        LISTA_COMPRUEBA_CREA_DEFECTO(func_crea_defecto, tipo),\
        LISTA_COMPRUEBA_PANEL(func_panel, tipo),\
        LISTA_COMPRUEBA_COPIA(func_copia, tipo),\
        LISTA_COMPRUEBA_DESTRUYE(func_destruye, tipo),\
        lista_nousar_crea(\
                    false, NULL, #tipo, id_nuevo, ancho, alto, false,\
                    (struct Array_t **)datos,\
                    (lista_FPtr_crea_defecto)func_crea_defecto,\
                    (lista_FPtr_panel)func_panel,\
                    (lista_FPtr_copia)func_copia,\
                    (lista_FPtr_destruye)func_destruye,\
                    false, NULL, NULL,\
                    (struct lista_t **)lista)\
    )

#define lista_crea_class(tipo, datos, id_nuevo, func_crea_defecto, func_panel, func_copia, func_destruye)\
    (\
        (void)((ArrClass(tipo) **)datos == datos),\
        (void)((Lista(tipo) **)lista == lista),\
        LISTA_COMPRUEBA_CREA_DEFECTO_CLASS(func_crea_defecto, tipo),\
        LISTA_COMPRUEBA_PANEL_CLASS(func_panel, tipo),\
        LISTA_COMPRUEBA_COPIA_CLASS(func_copia, tipo),\
        LISTA_COMPRUEBA_DESTRUYE_CLASS(func_destruye, tipo),\
        (Lista(tipo) *)lista_nousar_crea(\
                    false, NULL, #tipo, id_nuevo,\
                    (struct Array_t **)datos,\
                    (lista_FPtr_crea_defecto)func_crea_defecto,\
                    (lista_FPtr_panel)func_panel,\
                    (lista_FPtr_copia)func_copia,\
                    (lista_FPtr_destruye)func_destruye,\
                    false, NULL, NULL,\
                    (struct lista_t **)lista)\
    )

#define lista_crea_seleccion(tipo, datos, ancho, alto, ind_seleccion, func_panel, lista)\
    (\
        (void)((ArrTipo(tipo) **)datos == datos),\
        (void)((Lista(tipo) **)lista == lista),\
        LISTA_COMPRUEBA_PANEL(func_panel, tipo),\
        lista_nousar_crea(\
                    true, ind_seleccion, #tipo, 0, ancho, alto,\
                    true, (struct Array_t **)datos,\
                    NULL,\
                    (lista_FPtr_panel)func_panel,\
                    NULL, NULL,\
                    false, NULL, NULL,\
                    (struct lista_t **)lista)\
    )

struct lista_item_dato_t *lista_nousar_nuevo_elemento(struct lista_t *lista, const char *tipo);
    
#define lista_nuevo_elemento(tipo, lista)\
(\
    (void)((Lista(tipo) *)lista == lista),\
    (struct tipo *)lista_nousar_nuevo_elemento((struct lista_t *)lista, #tipo)\
)
#define lista_nuevo_elemento_class(tipo, lista)\
(\
    (void)((Lista(tipo) *)lista == lista),\
    (class tipo *)lista_nousar_nuevo_elemento((struct lista_t *)lista, #tipo)\
)

void lista_nousar_append_elemento(struct lista_t *lista, const char *tipo, struct lista_item_dato_t **dato);

#define lista_append_elemento(tipo, lista, dato)\
(\
    (void)((Lista(tipo) *)lista == lista),\
    (void)((struct tipo **)dato == dato),\
    lista_nousar_append_elemento((struct lista_t *)lista, #tipo, (struct lista_item_dato_t **)dato)\
)
#define lista_append_elemento_class(tipo, lista, dato)\
(\
    (void)((Lista(tipo) *)lista == lista),\
    (void)((class tipo **)dato == dato),\
    lista_nousar_append_elemento((struct lista_t *)lista, #tipo, (struct lista_item_dato_t **)dato)\
)

void lista_nousar_borra_elemento_seleccionado(struct lista_t *lista, const char *tipo);
#define lista_borra_elemento_seleccionado(tipo, lista)\
(\
    (void)((Lista(tipo) *)lista == lista),\
    lista_nousar_borra_elemento_seleccionado((struct lista_t *)lista, #tipo)\
)
    
bool lista_nousar_hay_elementos_seleccionados(struct lista_t *lista, const char *tipo);
    
#define lista_hay_elementos_seleccionados(tipo, lista)\
(\
    (void)((Lista(tipo) *)lista == lista),\
    lista_nousar_hay_elementos_seleccionados((struct lista_t *)lista, #tipo)\
)

struct panel_t *lista_nousar_panel_elemento_activo(struct lista_t *lista, const char *tipo);
#define lista_panel_elemento_activo(tipo, lista)\
(\
    (void)((Lista(tipo) *)lista == lista),\
    lista_nousar_panel_elemento_activo((struct lista_t *)lista, #tipo)\
)

struct lista_item_dato_t *lista_nousar_elemento_activo(struct lista_t *lista, const char *tipo);
#define lista_elemento_activo(tipo, lista)\
(\
    (void)((Lista(tipo) *)lista == lista),\
    (struct tipo *)lista_nousar_elemento_activo((struct lista_t *)lista, #tipo)\
)

void lista_nousar_actualiza_activo(struct lista_t *lista, const char *tipo);
#define lista_actualiza_activo(tipo, lista)\
(\
    (void)((Lista(tipo) *)lista == lista),\
    (struct tipo *)lista_nousar_actualiza_activo((struct lista_t *)lista, #tipo)\
)

    
#ifdef __cplusplus
}
#endif /* __cplusplus */
