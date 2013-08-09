// Definici�n de paneles

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef void (*panel_FPtr_destruye_datos_panel)(struct item_datos_panel_t **datos_panel);
#define PANEL_COMPRUEBA_FPTR_DESTRUYE_DATOS_PANEL(tipo, funcion) (void)((void (*)(tipo **))funcion == funcion)

typedef void (*panel_FPtr_aceptado)(struct item_datos_panel_t *datos_panel);
#define PANEL_COMPRUEBA_FPTR_ACEPTADO(tipo, funcion) (void)((void (*)(struct tipo *))funcion == funcion)

struct panel_t *panel_crea_con_disposicion_horizontal(void);
	
struct panel_t *panel_crea_con_disposicion_vertical(void);
    
struct panel_t *panel_crea_con_disposicion_grid(unsigned long num_filas, unsigned long num_columnas);

void panel_append_panel(struct panel_t *padre, struct panel_t **hijo);
    
void panel_set_en_grid(struct panel_t *padre, unsigned long fila, unsigned long columna, struct panel_t **hijo);

void panel_nousar_append_datos_panel(struct panel_t *panel, struct item_datos_panel_t **datos_panel, panel_FPtr_destruye_datos_panel func_destruye_datos_panel);

#define panel_append_datos_panel(panel, tipo_datos_panel, datos_panel, funcion_destruye_datos_panel)\
(\
    (void)((tipo_datos_panel **)datos_panel == datos_panel),\
    PANEL_COMPRUEBA_FPTR_DESTRUYE_DATOS_PANEL(tipo_datos_panel, funcion_destruye_datos_panel),\
    panel_nousar_append_datos_panel(panel, (struct item_datos_panel_t **)datos_panel, (panel_FPtr_destruye_datos_panel)funcion_destruye_datos_panel)\
)

void panel_nousar_set_funcion_aceptar(
                struct panel_t *panel, 
                struct item_datos_panel_t *datos_aceptar, 
                panel_FPtr_aceptado func_aceptar);

#define panel_set_funcion_aceptado(panel, tipo_datos_panel, datos_panel_aceptar, func_aceptar)\
(\
    (void)((struct tipo_datos_panel *)datos_panel_aceptar == datos_panel_aceptar),\
    PANEL_COMPRUEBA_FPTR_ACEPTADO(tipo_datos_panel, func_aceptar),\
    panel_nousar_set_funcion_aceptar(panel, (struct item_datos_panel_t *)datos_panel_aceptar, (panel_FPtr_aceptado)func_aceptar)\
)

bool panel_acepta_si_se_puede(struct panel_t *panel, char **mensaje);

struct Array_t *panel_componentsOfPrivate(struct panel_t *panel, const char *type);
#define panel_componentsOf(panel, type) ((ArrTipo(type) *)panel_componentsOfPrivate(panel, #type))

void panel_append_time(struct panel_t *panel, unsigned long timeout_miliseconds, struct rcpaccion_t **rcp_process_time);  

void panel_appendReceptorKeys(struct panel_t *panel, struct RcpKey_t **rcpKey);

void panel_set_expand(struct panel_t *panel);

void panel_set_fill(struct panel_t *panel);

#ifdef __cplusplus
}
#endif /* __cplusplus */
