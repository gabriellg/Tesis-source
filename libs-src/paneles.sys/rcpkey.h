// Receptores para teclados

typedef void (*rcpkey_FPtr_evtKey)(struct EvtKey_t *evtKey, struct rcpkey_item_data_t *data);
#define RCPKEY_VERIFY_FUNC_ON_BOTON(function, typeData) (void)((void (*)(struct EvtKey_t *, typeData *))function == function)
    
typedef void (*rcpkey_FPtr_destroy)(struct rcpkey_item_data_t **data);
#define RCPKEY_VERIFY_FUNC_DESTRUYE(function, typeData) (void)((void (*)(typeData **datos_opcion))function == function)

struct RcpKey_t *rcpkey_createPrivate(
                            struct rcpkey_item_data_t *data,
                            rcpkey_FPtr_evtKey func_evtKey);

struct RcpKey_t *rcpkey_createWithDestroyPrivate(
                            struct rcpkey_item_data_t **data,
                            rcpkey_FPtr_evtKey func_evtKey,
                            rcpkey_FPtr_destroy func_destroy);

#define rcpkey_create(typeData, data, func_evtKey)\
(\
    (void)((typeData *)data == data),\
    RCPKEY_VERIFY_FUNC_ON_BOTON(func_evtKey, typeData),\
    rcpkey_createPrivate(\
                            (struct rcpkey_item_data_t *)data,\
                            (rcpkey_FPtr_evtKey)func_evtKey)\
)
                            
#define rcpkey_createWithDestroy(typeData, data, func_evtKey, func_destroy)\
(\
    (void)((typeData **)data == data),\
    RCPKEY_VERIFY_FUNC_ON_BOTON(func_evtMouse, typeData),\
    RCPKEY_VERIFY_FUNC_DESTRUYE(func_destroy, typeData),\
    rcpkey_createWithDestroyPrivate(\
                            (struct rcpkey_item_data_t **)data,\
                            (rpckey_FPtr_evtMouse)func_evtMouse,\
                            (rcpkey_FPtr_destroy)func_destroy)\
)
