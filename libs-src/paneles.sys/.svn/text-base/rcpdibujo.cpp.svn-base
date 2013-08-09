// Receptores de acciones

#include "rcpdibujo.hpp"
#include "rcpdibujo.inl"

#include "asrtbas.h"
#include "memory.h"

#include "CGraphicsRect.hpp"

struct rcpdibujo_t
{
	rcpdibujo_FPtr_defineLayers func_defineLayers;
	rcpdibujo_FPtr_dibujo func_dibujo;
	rcpdibujo_FPtr_limites func_limites;
	
	const struct item_datos_dibujo_t *datos_dibujo;
};

//---------------------------------------------------------

struct rcpdibujo_t *rcpdibujo_no_usar_crea(
						rcpdibujo_FPtr_defineLayers func_defineLayers,
						rcpdibujo_FPtr_dibujo func_dibujo,
						rcpdibujo_FPtr_limites func_limites,
						const struct item_datos_dibujo_t *datos_dibujo)
{
	struct rcpdibujo_t *receptor_accion;
	
	receptor_accion = MALLOC(struct rcpdibujo_t);
	
	receptor_accion->func_defineLayers = func_defineLayers; 
	receptor_accion->func_dibujo = func_dibujo;
	receptor_accion->func_limites = func_limites;
	receptor_accion->datos_dibujo = datos_dibujo;
		
	return receptor_accion;
}
	
//---------------------------------------------------------

void rcpdibujo_destruye(struct rcpdibujo_t **receptor_accion)
{
	assert_no_null(receptor_accion);
	FREE_T(receptor_accion, struct rcpdibujo_t);
}

//---------------------------------------------------------

void rcpdibujo_defineLayers(struct rcpdibujo_t *receptor_dibujo, class IGraphics *grafico)
{
	assert_no_null(receptor_dibujo);
	assert_no_null(receptor_dibujo->func_defineLayers);
	receptor_dibujo->func_defineLayers(receptor_dibujo->datos_dibujo, grafico);
}

//---------------------------------------------------------

void rcpdibujo_redibujar(struct rcpdibujo_t *receptor_dibujo, class IGraphics *grafico)
{
	assert_no_null(receptor_dibujo);
	assert_no_null(receptor_dibujo->func_dibujo);
	receptor_dibujo->func_dibujo(receptor_dibujo->datos_dibujo, grafico);
}

//---------------------------------------------------------

bool rcpdibujo_calculaLimites(
						struct rcpdibujo_t *receptor_dibujo, 
						double *xmin, double *ymin, double *zmin, 
						double *xmax, double *ymax, double *zmax)
{
	bool hayLimites;
	
	assert_no_null(receptor_dibujo);
	
	if (receptor_dibujo->func_limites != NULL)
		hayLimites = receptor_dibujo->func_limites(receptor_dibujo->datos_dibujo, xmin, ymin, zmin, xmax, ymax, zmax);
	else
	{
		class CGraphicsRect *graphicsLimits;
		
		graphicsLimits = new CGraphicsRect();
		
		receptor_dibujo->func_dibujo(receptor_dibujo->datos_dibujo, graphicsLimits);
		
		hayLimites = graphicsLimits->getLimits(xmin, ymin, zmin, xmax, ymax, zmax);
						
		delete graphicsLimits;
	}
	
	return hayLimites;
}
