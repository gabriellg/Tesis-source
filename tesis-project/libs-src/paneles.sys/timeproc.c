// Procesos en un determinado tiempo.

#include "timeproc.inl"

#include "componente.inl"
#include "rcpaccion.inl"

#include "asrtbas.h"
#include "memory.h"
#include "callback.syh"
#include "marcogrf.syh"

struct timeproc_t
{
    unsigned long timeout_miliseconds;
    
    struct rcpaccion_t *rcp_process_time;
     
    struct componente_t *componente;
};

//---------------------------------------------------------

static struct timeproc_t *i_create(
                        unsigned long timeout_miliseconds,
                        struct rcpaccion_t **rcp_process_time,
                        struct componente_t *componente)
{
    struct timeproc_t *time_process;
    
    time_process = MALLOC(struct timeproc_t);
    
    time_process->timeout_miliseconds = timeout_miliseconds;
    time_process->rcp_process_time = ASSIGN_PP_NO_NULL(rcp_process_time, struct rcpaccion_t);
    
    time_process->componente = componente;
    
    return time_process;
} 

//---------------------------------------------------------

struct timeproc_t *timeproc_create_process_time(
                        unsigned long timeout_miliseconds,
                        struct rcpaccion_t **rcp_process_time)
{
    struct componente_t *componente;
    
    componente = NULL;
    
    return i_create(timeout_miliseconds, rcp_process_time, componente);
}

//---------------------------------------------------------

void timeproc_destroy(struct timeproc_t **time_process)
{
    assert_no_null(time_process);
    assert_no_null(*time_process);
    
    rcpaccion_destruye(&(*time_process)->rcp_process_time);
    
    FREE_T(time_process, struct timeproc_t);
}

//---------------------------------------------------------

static void i_run_receptor(struct timeproc_t *time_proc)
{
    struct dialogo_t *dialogo_padre;
    
    assert_no_null(time_proc);
    
    dialogo_padre = componente_get_dialogo_padre(time_proc->componente);
    rcpaccion_envia(time_proc->rcp_process_time, time_proc->componente, dialogo_padre);
}

//---------------------------------------------------------

void timeproc_set_time(struct timeproc_t *time_proc, struct componente_t *componente, const struct marcogrf_t *marco)
{
    struct callback_t *callback;
    
    assert_no_null(time_proc);
    assert(time_proc->componente == NULL);
    
    callback = callback_crea(timeproc_t, time_proc, i_run_receptor, NULL);
    
    time_proc->componente = componente;
    componente_setTime(componente, marco, time_proc->timeout_miliseconds, &callback);
}
