// Definicion Servidor.

#include "servidor.h"

#include "servicio.inl"
#include "sckstream.inl"

#include "asrtbas.h"
#include "array.h"
#include "strbas.h"
#include "memory.h"
#include "hilo.h"

#define i_MAXIMAS_PETICIONES 100

struct i_peticion_t
{
    unsigned long ind_peticion;
    int handle_socket;
    struct servidor_t *servidor;
    struct hilo_t *hilo_peticion;
    bool terminado;
};

struct servidor_t
{
    int puerto;
    bool terminar;
    ArrTipo(servicio_t) *servicios;
    struct hilo_t *hilo_ejecutando;
    int handle_socket_escucha;
    struct i_peticion_t *peticiones_pendientes[i_MAXIMAS_PETICIONES];
};

//---------------------------------------------------------

static void i_inicializa_array_de_peticiones(struct i_peticion_t **peticiones_pendientes)
{
    unsigned long i;
    
    assert_no_null(peticiones_pendientes);
    
    for (i = 0; i < i_MAXIMAS_PETICIONES; i++)
        peticiones_pendientes[i] = NULL;
}

//---------------------------------------------------------

static struct i_peticion_t *i_crea_peticion(
                            unsigned long ind_peticion,
                            int handle_socket,
                            struct servidor_t *servidor,
                            struct hilo_t **hilo_peticion)
{
    struct i_peticion_t *peticion;
        
    peticion = MALLOC(struct i_peticion_t);
        
    peticion->ind_peticion = ind_peticion;
    peticion->handle_socket = handle_socket;
    peticion->servidor = servidor;
    peticion->hilo_peticion = ASSIGN_PP(hilo_peticion, struct hilo_t);;
    peticion->ind_peticion = ind_peticion;
    peticion->terminado = false;
        
    return peticion;
}

//---------------------------------------------------------

static void i_destruye_peticion(struct i_peticion_t **peticion)
{
    assert_no_null(*peticion);
    assert_no_null(peticion);
    hilo_destruye(&(*peticion)->hilo_peticion);
    FREE_T(peticion, struct i_peticion_t);
}

//---------------------------------------------------------

static bool i_esta_pendiente(const struct i_peticion_t *peticion)
{
    if (peticion != NULL && peticion->terminado == false)
        return true;
    else
        return false;
}

//---------------------------------------------------------

static struct servidor_t *i_crea(
                        int puerto,
                        bool terminar,
                        ArrTipo(servicio_t) **servicios,
                        struct hilo_t **hilo_ejecutando,
                        int handle_socket_escucha)
{
    struct servidor_t *servidor;
        
    servidor = MALLOC(struct servidor_t);
        
    servidor->puerto = puerto;
    servidor->terminar = terminar;
    servidor->servicios = ASSIGN_PP_NO_NULL(servicios, ArrTipo(servicio_t));
    servidor->hilo_ejecutando = ASSIGN_PP(hilo_ejecutando, struct hilo_t);
    servidor->handle_socket_escucha = handle_socket_escucha;
    i_inicializa_array_de_peticiones(servidor->peticiones_pendientes);
        
    return servidor;
}
                    
//---------------------------------------------------------

struct servidor_t *servidor_crea(
                        int puerto,
                        ArrTipo(servicio_t) **servicios)
{
    bool terminar;
    struct hilo_t *hilo_ejecutando;
    int handle_socket_escucha;

    terminar = false;
    hilo_ejecutando = NULL;
    handle_socket_escucha = 0;
    
    return i_crea(puerto, terminar, servicios, &hilo_ejecutando, handle_socket_escucha);
}

//---------------------------------------------------------

static void i_destruye_pendientes(struct i_peticion_t **peticiones_pendientes)
{
    unsigned long i;
    
    assert_no_null(peticiones_pendientes);
    
    for (i = 0; i < i_MAXIMAS_PETICIONES; i++)
    {
        if (i_esta_pendiente(peticiones_pendientes[i]) == true)
            hilo_cancela(peticiones_pendientes[i]->hilo_peticion);
        
        if (peticiones_pendientes[i] != NULL)
            i_destruye_peticion(&peticiones_pendientes[i]);
    }
}

//---------------------------------------------------------

void servidor_destruye(struct servidor_t **servidor)
{
    assert_no_null(servidor);
    assert_no_null(*servidor);
    
    (*servidor)->terminar = true;
    hilo_cancela((*servidor)->hilo_ejecutando);
    
    i_destruye_pendientes((*servidor)->peticiones_pendientes);
    array_Destruye(servicio_t, &(*servidor)->servicios, servicio_destruye);
    
    FREE_T(servidor, struct servidor_t);
}

//---------------------------------------------------------

static void i_ejecutar_peticion(
					int hand_socket_aceptado,
					const ArrTipo(servicio_t) *servicios)
{
	char *nombre_servicio;
    unsigned long ind_servicio;
	
	nombre_servicio = sckstream_recibir_cadena(hand_socket_aceptado);
    
    if (array_Buscar(
                    servicio_t, servicios, char, nombre_servicio, 
                    servicio_es_nombre_servicio, &ind_servicio) == true)
	{
        const struct servicio_t *servicio;
            
        sckstream_enviar_confirmacion(hand_socket_aceptado);
        
        servicio = array_Get(servicio_t, servicios, ind_servicio);
        servicio_ejecuta(servicio, hand_socket_aceptado);
	}
    
    strbas_destroy(&nombre_servicio);
}

//---------------------------------------------------------

static void i_lanza_peticion(struct i_peticion_t *peticion)
{
    assert_no_null(peticion);
    assert_no_null(peticion->servidor);
    
    i_ejecutar_peticion(peticion->handle_socket, peticion->servidor->servicios);
    peticion->terminado = true;
}

//---------------------------------------------------------

bool i_busca_hueco_pendiente(struct i_peticion_t **peticiones_pendientes, unsigned long *ind_hueco_peticion)
{
    unsigned long i;
    
    assert_no_null(peticiones_pendientes);
    assert_no_null(ind_hueco_peticion);
    
    for (i = 0; i < i_MAXIMAS_PETICIONES; i++)
    {
        if (i_esta_pendiente(peticiones_pendientes[i]) == false)
        {
            if (peticiones_pendientes[i] != NULL)
                i_destruye_peticion(&peticiones_pendientes[i]);
            
            *ind_hueco_peticion = i;
            return true;
        }
    }
    
    return false;
}

//---------------------------------------------------------

static void i_escuchando(struct servidor_t *servidor)
{
    assert_no_null(servidor);
    
	while (servidor->terminar == false)
	{
        unsigned long ind_hueco_peticion;
		int hand_socket_aceptado;
        struct i_peticion_t *peticion;
        struct hilo_t *hilo_peticion;
		
		hand_socket_aceptado = sckstream_aceptar_peticion(servidor->handle_socket_escucha, NULL);

        if (i_busca_hueco_pendiente(servidor->peticiones_pendientes, &ind_hueco_peticion) == true)
        {
            hilo_no_puede_cancelar();
        
            peticion = i_crea_peticion(ind_hueco_peticion, hand_socket_aceptado, servidor, &hilo_peticion);
            servidor->peticiones_pendientes[ind_hueco_peticion] = peticion;
            peticion->hilo_peticion = hilo_crea(i_lanza_peticion, i_peticion_t, peticion);

            hilo_puede_cancelar();
        }
	}
        
	sckstream_close(servidor->handle_socket_escucha);
}

//---------------------------------------------------------

void servidor_ejecuta(struct servidor_t *servidor)
{
    assert_no_null(servidor);
    assert(servidor->hilo_ejecutando == NULL);
    
	servidor->handle_socket_escucha = sckstream_abrir_servidor(servidor->puerto);
    servidor->hilo_ejecutando = hilo_crea(i_escuchando, servidor_t, servidor);
}
