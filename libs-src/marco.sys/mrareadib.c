// Define las funciones necesarias para las �rea de dibujo.

#include "mrareadib.syh"
#include "mrareadib.inl"
#include "mrareadib.tli"

#include "memory.h"

//---------------------------------------------------------

struct mrareadib_t *mrareadib_create(
						mrareadib_FPtr_crea_area_dibujo func_creaAreaDibujo,
						mrareadib_FPtr_getGraphics func_getGraphics,
						mrareadib_FPtr_getDimension func_getDimension,
						mrareadib_FPtr_redraw func_redraw)
{
	struct mrareadib_t *mr_area_dibujo;
	
	mr_area_dibujo = MALLOC(struct mrareadib_t);
	
	mr_area_dibujo->func_creaAreaDibujo = func_creaAreaDibujo;
	mr_area_dibujo->func_getGraphics = func_getGraphics;
	mr_area_dibujo->func_getDimension = func_getDimension;
	mr_area_dibujo->func_redraw = func_redraw;
	
	return mr_area_dibujo;	
}

//---------------------------------------------------------

void mrareadib_destroy(struct mrareadib_t **mr_area_dibujo)
{
	FREE_T(mr_area_dibujo, struct mrareadib_t);
}
