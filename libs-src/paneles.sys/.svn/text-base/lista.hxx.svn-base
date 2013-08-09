// Control lista.c

#ifndef __LISTA_HXX__
#define __LISTA_HXX__

#define Lista(tipo) struct lista_##tipo

typedef struct lista_item_dato_t *(*lista_FPtr_crea_defecto)(unsigned long *id_nuevo);
typedef struct panel_t *(*lista_FPtr_panel)(struct lista_item_dato_t *dato);
typedef struct panel_t *(*lista_FPtr_panel_con_datos_extra)(const struct lista_item_datos_extra_t *datos_extra, struct lista_item_dato_t *dato);
typedef struct lista_item_dato_t *(*lista_FPtr_copia)(const struct lista_item_dato_t *dato);
typedef void (*lista_FPtr_destruye)(struct lista_item_dato_t **dato);
    
#define LISTA_COMPRUEBA_CREA_DEFECTO(funcion, tipo) (void)((struct tipo *(*)(unsigned long *id_nuevo))funcion == funcion)
#define LISTA_COMPRUEBA_PANEL(funcion, tipo) (void)((struct panel_t *(*)(struct tipo *))funcion == funcion)
#define LISTA_COMPRUEBA_PANEL_DATOS_EXTRA(func, tipo_datos_extra, tipo_dato) (void)((struct panel_t *(*)(const struct tipo_datos_extra *, struct tipo_dato *))func == func)
#define LISTA_COMPRUEBA_COPIA(funcion, tipo) (void)((struct tipo *(*)(const struct tipo *))funcion == funcion)
#define LISTA_COMPRUEBA_DESTRUYE(funcion, tipo) (void)((void (*)(struct tipo **))funcion == funcion)

#define LISTA_COMPRUEBA_CREA_DEFECTO_CLASS(funcion, tipo) (void)((class tipo *(*)(unsigned long *))funcion == funcion)
#define LISTA_COMPRUEBA_PANEL_CLASS(funcion, tipo) (void)((struct panel_t *(*)(class tipo *))funcion == funcion)
#define LISTA_COMPRUEBA_COPIA_CLASS(funcion, tipo) (void)((class tipo *(*)(const class tipo *))funcion == funcion)
#define LISTA_COMPRUEBA_DESTRUYE_CLASS(funcion, tipo) (void)((void (*)(class tipo **))funcion == funcion)

#endif //__LISTA_HXX__
