// Marco de aplicaci�n

#include "marco.h"
#include "marco.syh"

#include "asrtbas.h"
#include "memory.h"

struct marco_t
{
	struct marco_item_t *datos;

	marco_FPtr_lanza_mensaje func_lanza_mensaje;
	marco_FPtr_lanza_explicacion func_lanza_explicacion;
	marco_FPtr_lanza_pregunta func_lanza_pregunta;
};

//-----------------------------------------------------------------------

struct marco_t *macro_nousar_crea(
						struct marco_item_t **datos,
						marco_FPtr_lanza_mensaje func_lanza_mensaje,
						marco_FPtr_lanza_explicacion func_lanza_explicacion,
						marco_FPtr_lanza_pregunta func_lanza_pregunta)
{
	struct marco_t *marco;
		
	marco = MALLOC(struct marco_t);
		
	marco->datos = ASSIGN_PP(datos, struct marco_item_t);

	marco->func_lanza_mensaje = func_lanza_mensaje;
	marco->func_lanza_explicacion = func_lanza_explicacion;
	marco->func_lanza_pregunta = func_lanza_pregunta;
	
	return marco;
}

//-----------------------------------------------------------------------

void marco_destruye(struct marco_t **marco)
{
	assert_no_null(marco);
	FREE_T(marco, struct marco_t);
}

//-----------------------------------------------------------------------

void marco_lanza_mensaje(const struct marco_t *marco, enum marco_tipo_mensaje_t tipo_mensaje, const char *mensaje)
{
	assert_no_null(marco);
	assert_no_null(marco->func_lanza_mensaje);
	marco->func_lanza_mensaje(marco->datos, tipo_mensaje, mensaje);
}

//-----------------------------------------------------------------------

void marco_lanza_explicacion(const struct marco_t *marco, enum marco_tipo_mensaje_t tipo_mensaje, const ArrString *explicacion)
{
	assert_no_null(marco);
	assert_no_null(marco->func_lanza_explicacion);
	marco->func_lanza_explicacion(marco->datos, tipo_mensaje, explicacion);
}

//-----------------------------------------------------------------------

bool marco_lanza_pregunta(const struct marco_t *marco, const char *pregunta)
{
	assert_no_null(marco);
	assert_no_null(marco->func_lanza_pregunta);
	return marco->func_lanza_pregunta(marco->datos, pregunta);
}


