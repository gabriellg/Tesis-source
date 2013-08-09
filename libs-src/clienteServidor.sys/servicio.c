// Definicion de un servicio.

#include "servicio.h"
#include "servicio.inl"

#include "sckstream.inl"
#include "filesocket.inl"

#include "asrtbas.h"
#include "strbas.h"
#include "arrcad.h"
#include "memory.h"

enum i_tipo_servicio_t
{
    i_SERVICIO_RECIBE,
    i_SERVICIO_ENVIA
};

struct servicio_t
{
    char *nombre_servicio;
    struct servidor_item_t *datos_servicios;
        
    enum i_tipo_servicio_t tipo_servicio;
    
    servidor_fptr_funcion_recibe func_recibe;
    servidor_fptr_funcion_envia func_envia;
};

//---------------------------------------------------------

struct servicio_t *i_crea(
                        const char *nombre_servicio,
                        struct servidor_item_t *datos_servicio,
                        enum i_tipo_servicio_t tipo_servicio,
                        servidor_fptr_funcion_recibe func_recibe,
                        servidor_fptr_funcion_envia func_envia)
{
    struct servicio_t *servicio;
        
    servicio = MALLOC(struct servicio_t);
        
    servicio->nombre_servicio = strbas_copyString(nombre_servicio);
    servicio->datos_servicios = datos_servicio;
    
    servicio->tipo_servicio = tipo_servicio;
    servicio->func_recibe = func_recibe;
    servicio->func_envia = func_envia;
        
    return servicio;
}

//---------------------------------------------------------

struct servicio_t *servicio_nousar_crea_envia(
                        const char *nombre_servicio,
                        struct servidor_item_t *datos_servicios,
                        servidor_fptr_funcion_envia func_envia)
{
    servidor_fptr_funcion_recibe func_recibe;
    
    func_recibe = NULL;
    
    return i_crea(nombre_servicio, datos_servicios, i_SERVICIO_ENVIA, func_recibe, func_envia);
}

//---------------------------------------------------------

struct servicio_t *servicio_nousar_crea_recibe(
                        const char *nombre_servicio,
                        struct servidor_item_t *datos_servicios,
                        servidor_fptr_funcion_recibe func_recibe)
                        
{
    servidor_fptr_funcion_envia func_envia;
    
    func_envia = NULL;
    
    return i_crea(nombre_servicio, datos_servicios, i_SERVICIO_RECIBE, func_recibe, func_envia);
}

//---------------------------------------------------------

void servicio_destruye(struct servicio_t **servicio)
{
    assert_no_null(servicio);
    assert_no_null(*servicio);
    
    strbas_destroy(&(*servicio)->nombre_servicio);
    FREE_T(servicio, struct servicio_t);
}

//---------------------------------------------------------

static void i_recibe_datos(
                        struct servidor_item_t *datos_servicios,
                        int hand_socket,
                        servidor_fptr_funcion_recibe func_recibe)
{
    char *cadena_recibida;
    ArrString *parametros;
    struct filesocket_t *file;
    
    assert_no_null(func_recibe);
    
    cadena_recibida = sckstream_recibir_cadena(hand_socket);
    parametros = arrcad_separa_palabras_por_comas(cadena_recibida);
    
    file = filesocket_abre_lectura(&parametros);
    
    func_recibe(datos_servicios, (struct filesocketIn_t *)file);
    
    strbas_destroy(&cadena_recibida);
    filesocket_destruye(&file);
}

//---------------------------------------------------------

static void i_envia_datos(
                        struct servidor_item_t *datos_servicios,
                        int hand_socket,
                        servidor_fptr_funcion_envia func_envia)
{
    char *cadena_a_enviar;
    const ArrString *parametros;
    struct filesocket_t *file;
    
    assert_no_null(func_envia);
    
    file = filesocket_abre_escritura();
    func_envia(datos_servicios, (struct filesocketOut_t *)file);
    parametros = filesocket_contenido(file);
    cadena_a_enviar = arrcad_texto_separado_por_comas(parametros);
    
    sckstream_enviar_cadena(hand_socket, cadena_a_enviar);
    
    strbas_destroy(&cadena_a_enviar);
    filesocket_destruye(&file);
}

//---------------------------------------------------------

void servicio_ejecuta(const struct servicio_t *servicio, int hand_socket)
{
    assert_no_null(servicio);
    
    switch(servicio->tipo_servicio)
    {
        case i_SERVICIO_RECIBE:
        {
            i_recibe_datos(servicio->datos_servicios, hand_socket, servicio->func_recibe);
            break;
        }
        case i_SERVICIO_ENVIA:
        {
            i_envia_datos(servicio->datos_servicios, hand_socket, servicio->func_envia);
            break;
        }
        default_error();
    }
}

//---------------------------------------------------------

bool servicio_es_nombre_servicio(const struct servicio_t *servicio, const char *nombre_servicio)
{
    assert_no_null(servicio);
    return strbas_equalsStringWithoutMatchCase(servicio->nombre_servicio, nombre_servicio);
}

//---------------------------------------------------------

void servicio_envia_nombre_servicio(const struct servicio_t *servicio, int hand_socket)
{
    assert_no_null(servicio);
    sckstream_enviar_cadena(hand_socket, servicio->nombre_servicio);
    sckstream_recibir_confirmacion(hand_socket);
}
