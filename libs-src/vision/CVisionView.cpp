// Vista para vision artificial.

#include "CVisionView.hpp"

#include "asrtbas.h"
#include "panel.h"
#include "etiqueta.h"
#include "edit.h"
#include "idcolor.h"
#include "boton.h"
#include "rcpaccion.h"
#include "evtaccion.h"
#include "memory.h"

#include "rcpdibujo.hpp"
#include "areadibujo.hpp"
#include "CVisionDataWorkspace.hpp"
#include "IGraphics.hpp"

//-----------------------------------------------------------------------

static void i_defineLayers(const class CVisionDataWorkspace *visionDataWorkspace, class IGraphics *graphics)
{
	assert_no_null(visionDataWorkspace);
	visionDataWorkspace->defineLayers(graphics);
}

//-----------------------------------------------------------------------

static void i_dibuja_vista(
					const class CVisionDataWorkspace *visionDataWorksapce, 
					class IGraphics *graphics)
{
	assert_no_null(visionDataWorksapce);
	visionDataWorksapce->draw(graphics);
} 

//-----------------------------------------------------------------------

static struct panel_t *i_viewGraph(class CVisionDataWorkspace *visionDataWorkspace)
{
	struct panel_t *area_dibujo;
	unsigned long ancho, alto;
	struct rcpdibujo_t *funcion_dibujo;
	
	ancho = 200; 
	alto = 400;
	
	funcion_dibujo = rcpdibujo_crea_class(i_defineLayers, i_dibuja_vista, CVisionDataWorkspace, visionDataWorkspace);
			
	area_dibujo = areadibujo_crea_modelo2d(ancho, alto, &funcion_dibujo, NULL);
			
	return area_dibujo;				
}

//-----------------------------------------------------------------------

struct panel_t *CVisionView::panel(class CDataWorkspace *dataWorkspace)
{
	class CVisionDataWorkspace *visionDataWorkspace;

	visionDataWorkspace = (class CVisionDataWorkspace *)dataWorkspace;
	return i_viewGraph(visionDataWorkspace);  
}
