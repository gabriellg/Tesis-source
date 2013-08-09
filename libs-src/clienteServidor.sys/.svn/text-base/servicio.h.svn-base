// Definicion de un servicio.

typedef void (*servidor_fptr_funcion_recibe)(struct servidor_item_t *datos_servicios, struct filesocketIn_t *file);
#define SERVICIO_COMPROBAR_FUNC_RECIBE(tipo, funcion) (void)((void (*)(struct tipo *, struct filesocketIn_t *))funcion == funcion)    

typedef void (*servidor_fptr_funcion_envia)(struct servidor_item_t *datos_servicios, struct filesocketOut_t *file);
#define SERVICIO_COMPROBAR_FUNC_ENVIA(tipo, funcion) (void)((void (*)(struct tipo *, struct filesocketOut_t *))funcion == funcion)                        
                        
//---------------------------------------------------------

struct servicio_t *servicio_nousar_crea_envia(
                        const char *nombre_servicio,
                        struct servidor_item_t *datos_servicio,
                        servidor_fptr_funcion_envia func_envia);
#define servicio_crea_envia(tipo_dato, datos_servicio, nombre_servicio, func_envia)\
(\
    (void)((struct tipo_dato *)datos_servicio == datos_servicio),\
    SERVICIO_COMPROBAR_FUNC_ENVIA(tipo_dato, func_envia),\
    servicio_nousar_crea_envia(\
                        nombre_servicio,\
                        (struct servidor_item_t *)datos_servicio,\
                        (servidor_fptr_funcion_envia)func_envia)\
)

struct servicio_t *servicio_nousar_crea_recibe(
                        const char *nombre_servicio,
                        struct servidor_item_t *datos_servicio,
                        servidor_fptr_funcion_recibe func_recibe);
#define servicio_crea_recibe(tipo_dato, datos_servicio, nombre_servicio, func_recibe)\
(\
    (void)((struct tipo_dato *)datos_servicio == datos_servicio),\
    SERVICIO_COMPROBAR_FUNC_RECIBE(tipo_dato, func_recibe),\
    servicio_nousar_crea_recibe(\
                        nombre_servicio,\
                        (struct servidor_item_t *)datos_servicio,\
                        (servidor_fptr_funcion_recibe)func_recibe)\
)
