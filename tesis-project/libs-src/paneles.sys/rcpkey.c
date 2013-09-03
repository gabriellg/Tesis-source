// Receptores para teclados

#include "rcpkey.h"
#include "rcpkey.inl"

#include "asrtbas.h"
#include "memory.h"
#include "evtkey.h"

#include "float.h"

struct RcpKey_t
{
    struct rcpkey_item_data_t *data;

    rcpkey_FPtr_evtKey func_evtKey;
    rcpkey_FPtr_destroy func_destroy;
};

//---------------------------------------------------------

static struct RcpKey_t *i_create(
                        struct rcpkey_item_data_t **data,
                        rcpkey_FPtr_evtKey func_evtKey,
                        rcpkey_FPtr_destroy func_destroy)
{
    struct RcpKey_t *rcpKey;
    
    rcpKey = MALLOC(struct RcpKey_t);
    
    rcpKey->data = ASSIGN_PP(data, struct rcpkey_item_data_t);
    
    rcpKey->func_evtKey = func_evtKey;
    rcpKey->func_destroy = func_destroy;
    
    return rcpKey;
}

//---------------------------------------------------------

struct RcpKey_t *rcpkey_createPrivate(
                            struct rcpkey_item_data_t *data,
                            rcpkey_FPtr_evtKey func_evtKey)
{
    return i_create(&data, func_evtKey, NULL);
}

//---------------------------------------------------------

struct RcpKey_t *rcpkey_createWithDestroyPrivate(
                            struct rcpkey_item_data_t **data,
                            rcpkey_FPtr_evtKey func_evtKey,
                            rcpkey_FPtr_destroy func_destroy)
{
    return i_create(data, func_evtKey, func_destroy);
}

//---------------------------------------------------------

void rcpkey_destruye(struct RcpKey_t **rcpKey)
{
    assert_no_null(rcpKey);
    assert_no_null(*rcpKey);
    
    if ((*rcpKey)->data != NULL && (*rcpKey)->func_destroy != NULL)
        (*rcpKey)->func_destroy(&(*rcpKey)->data);
    
    FREE_T(rcpKey, struct RcpKey_t);    
}

//---------------------------------------------------------

void rcpkey_sendEvent(struct RcpKey_t *rcpKey, struct EvtKey_t *evtKey)
{
    assert_no_null(rcpKey);
    assert_no_null(rcpKey->func_evtKey);
    
    rcpKey->func_evtKey(evtKey, rcpKey->data);
}
