// Evento que gestiona teclas.

#include "evtkey.h"

#include "asrtbas.h"
#include "memory.h"

struct EvtKey_t
{
    unsigned int key;
    bool shiftPress;
    bool controlPress;
    
    bool isEnded;
};

//---------------------------------------------------------------

static struct EvtKey_t *i_create(unsigned int key, bool shiftPress, bool controlPress, bool isEnded)
{
    struct EvtKey_t *evtKey;
    
    evtKey = MALLOC(struct EvtKey_t);

    evtKey->key = key;
    evtKey->shiftPress = shiftPress;
    evtKey->controlPress = controlPress;
    evtKey->isEnded = isEnded;
    
    return evtKey;
}

//---------------------------------------------------------------

struct EvtKey_t *evtkey_create(unsigned int key, bool shiftPress, bool controlPress)
{
	bool isEnded;
	
	isEnded = false;
	return i_create(key, shiftPress, controlPress, isEnded);
}

//---------------------------------------------------------------

struct EvtKey_t *evtkey_copy(const struct EvtKey_t *evtKey)
{
	assert_no_null(evtKey);
	return i_create(evtKey->key, evtKey->shiftPress, evtKey->controlPress, evtKey->isEnded);
}

//---------------------------------------------------------------

void evtkey_destroy(struct EvtKey_t **evtKey)
{
    FREE_T(evtKey, struct EvtKey_t);  
}

//---------------------------------------------------------------

unsigned int evtkey_key(const struct EvtKey_t *evtKey)
{
    assert_no_null(evtKey);
    return evtKey->key;
}

//---------------------------------------------------------------

bool evtkey_isControlPress(const struct EvtKey_t *evtKey, unsigned int *key)
{
    assert_no_null(evtKey);
    
    if (key != NULL && evtKey->controlPress == true)
        *key = evtKey->key;
        
    return evtKey->controlPress; 
}

//---------------------------------------------------------------

bool evtkey_isShiftPress(const struct EvtKey_t *evtKey, unsigned int *key)
{
    assert_no_null(evtKey);
    
    if (key != NULL && evtKey->shiftPress == true)
        *key = evtKey->key;
        
    return evtKey->shiftPress; 
}

//---------------------------------------------------------------

void evtkey_endEvent(struct EvtKey_t *evtKey)
{
	assert_no_null(evtKey);
	evtKey->isEnded = true;
}

//---------------------------------------------------------------

bool evtkey_isEnded(const struct EvtKey_t *evtKey)
{
	assert_no_null(evtKey);
    return evtKey->isEnded; 
}
