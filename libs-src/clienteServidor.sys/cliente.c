// Definicion de cliente

#include "cliente.h"

#include "servicio.inl"
#include "sckstream.inl"

#include "asrtbas.h"
#include "memory.h"
#include "array.h"
#include "strbas.h"

struct cliente_t
{
    int puerto;
    char *direccion_servidor;
    
    ArrTipo(servicio_t) *servicios;
};

//---------------------------------------------------------

struct cliente_t *cliente_crea(
                    int puerto,
                    const char *direccion_servidor,
                    ArrTipo(servicio_t) **servicios)
{
    struct cliente_t *cliente;
        
    cliente = MALLOC(struct cliente_t);
        
    cliente->puerto = puerto;
    cliente->direccion_servidor = strbas_copyString(direccion_servidor);
    cliente->servicios = ASSIGN_PP_NO_NULL(servicios, ArrTipo(servicio_t));
        
    return cliente;
}
    
//---------------------------------------------------------

void cliente_destruye(struct cliente_t **cliente)
{
    assert_no_null(cliente);
    assert_no_null(*cliente);
 
    strbas_destroy(&(*cliente)->direccion_servidor);
    array_Destruye(servicio_t, &(*cliente)->servicios, servicio_destruye);
    
    FREE_T(cliente, struct cliente_t);
}

//---------------------------------------------------------

static void i_ejecuta_servicio(ArrTipo(servicio_t) *servicios, unsigned long ind_servicio, int handle_socket)
{
    struct servicio_t *servicio;

    servicio = array_Get(servicio_t, servicios, ind_servicio);
    
    servicio_envia_nombre_servicio(servicio, handle_socket);
    servicio_ejecuta(servicio, handle_socket);
    
    sckstream_close(handle_socket);
}

//---------------------------------------------------------

void cliente_ejecuta(const struct cliente_t *cliente, const char *nombre_servicio)
{
    int handle_socket;
    unsigned long ind_servicio;
    bool existe, abierto;
    
    assert_no_null(cliente);
    
    existe = array_Buscar(
                    servicio_t, cliente->servicios, char, nombre_servicio, 
                    servicio_es_nombre_servicio, &ind_servicio);
    assert(existe == true);
        
	abierto = sckstream_abrir_cliente(
					cliente->puerto, cliente->direccion_servidor,
					&handle_socket, NULL);
    assert(abierto == true);
    
    i_ejecuta_servicio(cliente->servicios, ind_servicio, handle_socket);
}
