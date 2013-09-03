// Procesos en un determinado tiempo.

struct timeproc_t *timeproc_create_process_time(unsigned long timeout_miliseconds, struct rcpaccion_t **rcp_process_time);

void timeproc_destroy(struct timeproc_t **time_process);

void timeproc_set_time(struct timeproc_t *time_proc, struct componente_t *componente, const struct marcogrf_t *marco);
