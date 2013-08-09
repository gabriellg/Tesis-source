// Definici�n de funciones callback

#include "callback.syh"

#include "asrtbas.h"
#include "memory.h"
#include "strbas.h"

enum i_tipo_callback_t
{
	i_CALLBACK_SIMPLE,
	i_CALLBACK_CON_DATOS_EXTERNOS
};

struct callback_t
{
	enum i_tipo_callback_t tipo_callback;

	callback_FPtr_accion funcion;
	char *tipo_dato_externo;
	callback_FPtr_accion_externo funcion_con_datos_externos;
	struct callback_item_dato_t *dato;
    callback_FPtr_destruye funcion_destruye_dato;
};

//---------------------------------------------------------

static struct callback_t *i_crea(
						enum i_tipo_callback_t tipo_callback,
						callback_FPtr_accion funcion,
						const char *tipo_dato_externo,
						callback_FPtr_accion_externo funcion_con_datos_externos,
						struct callback_item_dato_t *dato,
                        callback_FPtr_destruye funcion_destruye_dato)
{
	struct callback_t *callback;
		
	callback = MALLOC(struct callback_t);
	
	callback->tipo_callback = tipo_callback;
	
	callback->funcion = funcion;
	callback->dato = dato;
	
	if (tipo_dato_externo != NULL)
		callback->tipo_dato_externo = strbas_copyString(tipo_dato_externo);
	else
		callback->tipo_dato_externo = NULL;
	
	callback->funcion_con_datos_externos = funcion_con_datos_externos;
    
    callback->funcion_destruye_dato = funcion_destruye_dato;
	
	return callback;
}

//---------------------------------------------------------

struct callback_t *callback_no_usar_crea(
                        callback_FPtr_accion funcion, 
                        struct callback_item_dato_t *dato, 
                        callback_FPtr_destruye funcion_destruye_dato)
{
	enum i_tipo_callback_t tipo_callback;
	callback_FPtr_accion_externo funcion_con_datos_externos;
	const char *tipo_dato_externo;
		
	tipo_callback = i_CALLBACK_SIMPLE;
	funcion_con_datos_externos = NULL;
	tipo_dato_externo = NULL;
		
	return i_crea(tipo_callback, funcion, tipo_dato_externo, funcion_con_datos_externos, dato, funcion_destruye_dato);
}

//---------------------------------------------------------

struct callback_t *callback_no_usar_crea_con_dato_externo(
                        callback_FPtr_accion_externo funcion_con_datos_externos, 
                        const char *tipo_dato_externo, struct callback_item_dato_t *dato,
                        callback_FPtr_destruye funcion_destruye_dato)
{
	enum i_tipo_callback_t tipo_callback;
	callback_FPtr_accion funcion;
		
	tipo_callback = i_CALLBACK_CON_DATOS_EXTERNOS;
	funcion = NULL;
		
	return i_crea(tipo_callback, funcion, tipo_dato_externo, funcion_con_datos_externos, dato, funcion_destruye_dato);
}

//---------------------------------------------------------

void callback_destruye(struct callback_t **callback)
{
    assert_no_null(callback);

    if ((*callback)->tipo_dato_externo != NULL)
        strbas_destroy(&(*callback)->tipo_dato_externo);
    
    if ((*callback)->funcion_destruye_dato != NULL)
        (*callback)->funcion_destruye_dato(&(*callback)->dato);
    
	FREE_T(callback, struct callback_t);
}

//---------------------------------------------------------

void callback_ejecuta(struct callback_t *callback)
{
	assert_no_null(callback);
	switch(callback->tipo_callback)
	{
		case i_CALLBACK_SIMPLE:
			callback->funcion(callback->dato);
			break;
		case i_CALLBACK_CON_DATOS_EXTERNOS:
		default_error();
	}
}

//---------------------------------------------------------

void callback_no_usar_ejecuta_con_dato_externo(
						struct callback_t *callback, 
						const char *tipo_dato_externo, 
						struct callback_item_dato_extreno_t *dato_externo)
{
	assert_no_null(callback);
	assert(strbas_equalsStrings(callback->tipo_dato_externo, tipo_dato_externo) == true);
	
	switch(callback->tipo_callback)
	{
		case i_CALLBACK_CON_DATOS_EXTERNOS:
			callback->funcion_con_datos_externos(callback->dato, dato_externo);
			break;
		case i_CALLBACK_SIMPLE:
		default_error();
	}
}
