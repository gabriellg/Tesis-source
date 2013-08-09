// Evento que gestiona teclas.

struct EvtKey_t *evtkey_create(unsigned int key, bool shiftPress, bool controlPress);

struct EvtKey_t *evtkey_copy(const struct EvtKey_t *evtKey);

void evtkey_destroy(struct EvtKey_t **evtKey);

unsigned int evtkey_key(const struct EvtKey_t *evtKey);

bool evtkey_isControlPress(const struct EvtKey_t *evtKey, unsigned int *key);

bool evtkey_isShiftPress(const struct EvtKey_t *evtKey, unsigned int *key);

void evtkey_endEvent(struct EvtKey_t *evtKey);
