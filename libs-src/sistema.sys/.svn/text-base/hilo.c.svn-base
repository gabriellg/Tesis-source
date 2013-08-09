// Definicion de hilos.

#include "hilo.h"

#include "asrtbas.h"
#include "memory.h"
#include <stdio.h>
#include <pthread.h>

typedef void *(*func_fptr_ejecuta)(void *);

struct hilo_t
{
    pthread_t *tarea;
    struct hilo_item_dato_t *dato;
    hilo_FPtr_func_destruye_datos func_destruye;
};

//---------------------------------------------------------

static struct hilo_t *i_crea(
                pthread_t **tarea,
                struct hilo_item_dato_t **dato,
                hilo_FPtr_func_destruye_datos func_destruye)
{
    struct hilo_t *hilo;
        
    hilo = MALLOC(struct hilo_t);
        
    hilo->tarea = ASSIGN_PP_NO_NULL(tarea, pthread_t);
    hilo->dato = ASSIGN_PP(dato, struct hilo_item_dato_t);
    hilo->func_destruye = func_destruye;
        
    return hilo;
}

//---------------------------------------------------------

pthread_t *i_lanza_hilo(hilo_FPtr_func_ejecuta func_ejecuta, struct hilo_item_dato_t *dato)
{
    pthread_t *tarea;
    int respuesta;
    
    tarea = MALLOC(pthread_t);
    respuesta = pthread_create(tarea, NULL, (func_fptr_ejecuta)func_ejecuta, dato);
    assert(respuesta == 0);
    
    return tarea;
}

//---------------------------------------------------------

struct hilo_t *hilo_nousar_crea(hilo_FPtr_func_ejecuta func_ejecuta, struct hilo_item_dato_t *dato)
{
    pthread_t *tarea;
    
    tarea = i_lanza_hilo(func_ejecuta, dato);
    
    return i_crea(&tarea, &dato, NULL);
}

//---------------------------------------------------------

void hilo_destruye(struct hilo_t **hilo)
{
    assert_no_null(hilo);
    assert_no_null(*hilo);
    assert_no_null((*hilo)->tarea);
    
    pthread_join(*((*hilo)->tarea), NULL);
    
    if ((*hilo)->func_destruye != NULL)
        (*hilo)->func_destruye(&(*hilo)->dato);

    FREE_T(&(*hilo)->tarea, pthread_t);
    FREE_T(hilo, struct hilo_t);
}

//---------------------------------------------------------

void hilo_cancela(const struct hilo_t *hilo)
{
    assert_no_null(hilo);
    assert_no_null(hilo->tarea);
    pthread_cancel(*(hilo->tarea));
}
//---------------------------------------------------------

void hilo_no_puede_cancelar(void)
{
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
}

//---------------------------------------------------------

void hilo_puede_cancelar(void)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
}

//////////////////////////////////////////////////////////////////////////////

static void i_imprimir_mensaje(const char *mensaje, const char *file, int line)
{
    printf("\nExpresi�n incorrecta:\n");
    printf("Expresion: %s\n", mensaje);
    printf("Fichero: %s\n", file);
    printf("Linea: %d\n", line);
}

//---------------------------------------------------------

void hilo_no_usar_assert(int expr, const char *txtExpr, const char *file, int line)
{
    if (expr == 0)
    {
        i_imprimir_mensaje(txtExpr, file, line);
        pthread_exit(NULL);
    }
}
