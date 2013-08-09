// Receptor de eventos raton

typedef void (*rpcmouse_FPtr_evtMouse)(const struct EvtMouse_t *evtMouse, struct rcpmouse_item_data_t *data);
#define VERIFY_FUNC_ON_BOTON(function, typeData) (void)((void (*)(const struct EvtMouse_t *, typeData *))function == function)
    
typedef void (*rcpmouse_FPtr_destroy)(struct rcpmouse_item_data_t **data);
#define VERIFY_FUNC_DESTRUYE(function, typeData) (void)((void (*)(typeData **datos_opcion))function == function)

struct RcpMouse_t *rcpmouse_createPrivate(
                            struct rcpmouse_item_data_t *data,
                            rpcmouse_FPtr_evtMouse func_evtMouse);

struct RcpMouse_t *rcpmouse_createWithDestroyPrivate(
                            struct rcpmouse_item_data_t **data,
                            rpcmouse_FPtr_evtMouse func_evtMouse,
                            rcpmouse_FPtr_destroy func_destroy);

#define rcpmouse_create(typeData, data, func_evtMouse)\
(\
    (void)((typeData *)data == data),\
	VERIFY_FUNC_ON_BOTON(func_evtMouse, typeData),\
    rcpmouse_createPrivate(\
                            (struct rcpmouse_item_data_t *)data,\
                            (rpcmouse_FPtr_evtMouse)func_evtMouse)\
)
                            
#define rcpmouse_createWithDestroy(typeData, data, func_evtMouse, func_destroy)\
(\
    (void)((typeData **)data == data),\
	VERIFY_FUNC_ON_BOTON(func_evtMouse, typeData),\
	VERIFY_FUNC_DESTRUYE(func_destroy, typeData),\
    rcpmouse_createWithDestroyPrivate(\
                            (struct rcpmouse_item_data_t **)data,\
                            (rpcmouse_FPtr_evtMouse)func_evtMouse,\
                            (rcpmouse_FPtr_destroy)func_destroy)\
)
