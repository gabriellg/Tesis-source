// Definici�n de Componente

#include "componente.inl"

#include "asrtbas.h"
#include "memory.h"
#include "strbas.h"

#include "marcogrf.syh"

struct componente_t
{
	char *tipo;

	struct componente_datos_item_t *datos;

	componente_FPtr_crear_implementacion func_crea_implementacion;
	componente_FPtr_destruye func_destruye;

    componente_FPtr_se_puede_aceptar func_se_puede_aceptar;
    componente_FPtr_aceptar func_aceptar;

    componente_FPtr_cadena func_titulo;
    componente_FPtr_cadena func_dato;

    struct dialogo_t *dialogo_padre;
	struct item_implementacion_t *datos_implementacion;
};

//---------------------------------------------------------

static void i_integridad(const struct componente_t *componente)
{
	assert_no_null(componente);
	assert_no_null(componente->tipo);
	assert_no_null(componente->datos);
	assert_no_null(componente->func_crea_implementacion);

	if (componente->datos != NULL)
		assert_no_null(componente->func_destruye);
	else
		assert(componente->func_destruye == NULL);
}

//---------------------------------------------------------

static struct componente_t *i_crea(
						const char *tipo,
						struct componente_datos_item_t **datos,
					    componente_FPtr_crear_implementacion func_crea_implementacion,
						componente_FPtr_destruye func_destruye,
                        componente_FPtr_se_puede_aceptar func_se_puede_aceptar,
                        componente_FPtr_aceptar func_aceptar,
                        componente_FPtr_cadena func_titulo,
                        componente_FPtr_cadena func_dato,
                        struct dialogo_t **dialogo_padre,
						struct item_implementacion_t **datos_implementacion)
{
	struct componente_t *componente;

	assert_no_null(func_crea_implementacion);

	componente = MALLOC(struct componente_t);

	componente->tipo = strbas_copyString(tipo);
	componente->datos = ASSIGN_PP(datos, struct componente_datos_item_t);
	componente->func_crea_implementacion = func_crea_implementacion;
	componente->func_destruye = func_destruye;
    componente->dialogo_padre = ASSIGN_PP(dialogo_padre, struct dialogo_t);
	componente->datos_implementacion = ASSIGN_PP(datos_implementacion, struct item_implementacion_t);

    componente->func_se_puede_aceptar = func_se_puede_aceptar;
    componente->func_aceptar = func_aceptar;

    componente->func_titulo = func_titulo;
    componente->func_dato = func_dato;

	i_integridad(componente);

	return componente;
}


//---------------------------------------------------------

struct componente_t *componente_no_usar_crea_defecto(
						const char *tipo,
						struct componente_datos_item_t **datos,
					    componente_FPtr_crear_implementacion func_crea_implementacion,
						componente_FPtr_destruye func_destruye,
                        componente_FPtr_se_puede_aceptar func_se_puede_aceptar,
                        componente_FPtr_aceptar func_aceptar,
                        componente_FPtr_cadena func_titulo,
                        componente_FPtr_cadena func_dato)
{
	struct item_implementacion_t *datos_implementacion;
    struct dialogo_t *dialogo_padre;

	datos_implementacion = NULL;
	dialogo_padre = NULL;
    return i_crea(tipo, datos, func_crea_implementacion, func_destruye, func_se_puede_aceptar, func_aceptar, func_titulo, func_dato, &dialogo_padre, &datos_implementacion);
}

//---------------------------------------------------------

void componente_destruye(struct componente_t **componente)
{
	assert_no_null(componente);
	i_integridad(*componente);

	strbas_destroy(&(*componente)->tipo);

	if ((*componente)->datos != NULL)
	{
		assert_no_null((*componente)->func_destruye);
		(*componente)->func_destruye(&(*componente)->datos);
	}

	FREE_T(componente, struct componente_t);
}

//---------------------------------------------------------

struct item_implementacion_t *componente_generar_implementacion(
                        struct marcogrf_t *marco,
                        struct componente_t *componente,
                        struct item_implementacion_dialogo_t *dialogo_so)
{
    struct item_implementacion_t *implementacion;

	i_integridad(componente);
	assert(componente->datos_implementacion == NULL);

	implementacion = componente->func_crea_implementacion(marco, componente->datos, dialogo_so);
    componente->datos_implementacion = implementacion;

	return implementacion;
}

//---------------------------------------------------------

struct componente_datos_item_t *componente_no_usar_datos(
						struct componente_t *componente, const char *tipo)
{
	i_integridad(componente);
	assert(strbas_equalsStrings(componente->tipo, tipo) == true);
	return componente->datos;
}

//---------------------------------------------------------

bool componente_se_puede_finalizar(const struct marcogrf_t *marco, const struct componente_t *componente, char **mensaje_error)
{
    i_integridad(componente);

    if (componente->func_se_puede_aceptar != NULL)
        return componente->func_se_puede_aceptar(marco, componente->datos, mensaje_error);
    else
        return true;
}

//---------------------------------------------------------

void componente_aceptar(const struct marcogrf_t *marco, const struct componente_t *componente)
{
    i_integridad(componente);

    if (componente->func_aceptar != NULL)
        componente->func_aceptar(marco, componente->datos);
}

//---------------------------------------------------------

ArrString *componente_titulo(const struct componente_t *componente)
{
    i_integridad(componente);
   if (componente->func_titulo != NULL)
       return componente->func_titulo(componente->datos);
   else
       return NULL;
}

//---------------------------------------------------------

ArrString *componente_dato(const struct componente_t *componente)
{
    i_integridad(componente);
   if (componente->func_dato != NULL)
       return componente->func_dato(componente->datos);
   else
       return NULL;
}

//---------------------------------------------------------

void componente_set_dialogo_padre(struct componente_t *componente, struct dialogo_t *dialogo)
{
    i_integridad(componente);
    componente->dialogo_padre = dialogo;
}

//---------------------------------------------------------

struct dialogo_t *componente_get_dialogo_padre(struct componente_t *componente)
{
    i_integridad(componente);
    return componente->dialogo_padre;
}

//---------------------------------------------------------

bool componente_isA(
					const struct componente_t *componente,
					const char *type,
					struct componente_datos_item_t **datos)
{
	i_integridad(componente);

	if (strbas_equalsStrings(componente->tipo, type) == true)
	{
		if (datos != NULL)
			*datos = componente->datos;

		return true;
	}
	else
		return false;
}

//---------------------------------------------------------

void componente_setTime(
                    struct componente_t *componente,
                    const struct marcogrf_t *marco,
                    unsigned long timeout_miliseconds,
                    struct callback_t **callback)
{
    assert_no_null(componente);
    assert_no_null(componente->datos_implementacion);

    marcogrf_setTime(marco, componente->datos_implementacion, timeout_miliseconds, callback);
}
