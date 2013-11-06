// Definicion area dibujo

#include "areadibujo.hpp"

#include "rcpdibujo.inl"
#include "componente.inl"
#include "panel.inl"
#include "rcpmouse.inl"
#include "rcpkey.inl"

#include "asrtbas.h"
#include "memory.h"
#include "array.h"
#include "marcogrf.syh"
#include "evtmouse.syh"
#include "callback.syh"

#include "evtmouse.h"
#include "CGraphicsRect.hpp"
#include "CGeometry.hpp"

struct i_limites_t
{
	bool hayLimites;
    
	double xmin, ymin, zmin;
	double xmax, ymax, zmax;
};

enum i_tipoDibujo_t
{
    i_DIBUJO_2D,
    i_DIBUJO_3D
};

enum i_zona_t
{
	i_ZONA_IZQ_INFERIOR,
	i_ZONA_DER_INFERIOR
};

struct i_zonaDibujo_t
{
	enum i_zona_t zona;
	
	unsigned long xSize, ySize;
	struct rcpdibujo_t *rcp_dibujoZona;
};

struct areaDibujo_t
{
    enum i_tipoDibujo_t tipoDibujo;
    
	unsigned long ancho, alto;

	struct i_limites_t limites;
	
    const struct areadibujo_item_data_camera_t *dataCamera;
    areadibujo_FPtr_positionCamera func_positionCamera;
    
    ArrTipo(rcpdibujo_t) *funciones_dibujo;
    ArrTipo(i_zonaDibujo_t) *zonasDibujo;
    
	struct callback_t *callback_iniciaGrafico; 
	struct callback_t *callback_definePerspectiva;
	struct callback_t *callback_dibuja;
    struct callback_t *funcion_eventos_raton;
        
    ArrTipo(RcpMouse_t) *receptores_raton;
        
    struct item_implementacion_t *item_area_dibujo;
    struct marcogrf_t *marco;
};

//---------------------------------------------------------

static struct i_zonaDibujo_t *i_creaZonaDibujo(
						enum i_zona_t zona, 
						unsigned long xSize, unsigned long ySize,
						struct rcpdibujo_t **rcp_dibujoZona)
{
	struct i_zonaDibujo_t *zonaDibujo;
	
	zonaDibujo = MALLOC(struct i_zonaDibujo_t);
	
	zonaDibujo->zona = zona;
	zonaDibujo->xSize = xSize; 
	zonaDibujo->ySize = ySize;
	zonaDibujo->rcp_dibujoZona = ASSIGN_PP_NO_NULL(rcp_dibujoZona, struct rcpdibujo_t);
	
	return zonaDibujo;
}

//---------------------------------------------------------

static void i_destruyeZonaDibujo(struct i_zonaDibujo_t **zonaDibujo)
{
	assert_no_null(zonaDibujo);
	assert_no_null(*zonaDibujo);

	rcpdibujo_destruye(&(*zonaDibujo)->rcp_dibujoZona);

	FREE_T(zonaDibujo, struct i_zonaDibujo_t);
}

//---------------------------------------------------------

static void i_putOrtho(
					unsigned long width, unsigned long heigth,
			        double xmin, double ymin, double zmin,
			        double xmax, double ymax, double zmax,
			        class IGraphics *graphics)
{
	double coefPorcBorder;
	const double i_MARGEN_EN_Z_POR_RAZONES_OPENGL = 100;
	
	coefPorcBorder = 0.05;
	CGeometry::rectangleWithProporcional(
				width, heigth, coefPorcBorder, 
				xmin, ymin, zmin,
				xmax, ymax, zmax,
				&xmin, &ymin, &zmin, 
				&xmax, &ymax, &zmax);
				
	if (zmax - zmin < 0.0001)
	{
		zmin = -1000.;
		zmax = 1000.;
	}
		
	graphics->defineOrtho(
				0., 0., 0.,
				xmin, ymin, zmin - i_MARGEN_EN_Z_POR_RAZONES_OPENGL,
				xmax, ymax, zmax + i_MARGEN_EN_Z_POR_RAZONES_OPENGL);
}

//---------------------------------------------------------

static void i_calculaPosicion(
							enum i_zona_t zona, 
							unsigned long xSizePantalla, 
							unsigned long ySizePantalla,
							unsigned long xSizeZona,
							unsigned long ySizeZona,
							int *xPos, int *yPos)
{
	assert_no_null(xPos);
	assert_no_null(yPos);
	assert(xSizePantalla > xSizeZona);
	assert(ySizePantalla > ySizeZona);
	
	switch(zona)
	{
		case i_ZONA_IZQ_INFERIOR:
			
			*xPos = 0;
			*yPos = 0;
			break;
			
		case i_ZONA_DER_INFERIOR:
			
			*xPos = xSizePantalla - xSizeZona;
			*yPos = 0;
			break;
			
		default_error();
	}
}

//---------------------------------------------------------

static void i_dibujaZonaDibujo(
							const struct i_zonaDibujo_t *zonaDibujo,
							unsigned long xSizePantalla, unsigned long ySizePantalla,
							class IGraphics *graphics)
{
	int xPos, yPos;
	double xmin, ymin, zmin, xmax, ymax, zmax;
	
	i_calculaPosicion(zonaDibujo->zona, xSizePantalla, ySizePantalla, zonaDibujo->xSize, zonaDibujo->ySize, &xPos, &yPos);
	graphics->viewport(xPos, yPos, zonaDibujo->xSize, zonaDibujo->ySize);
	
	if (rcpdibujo_calculaLimites(
							zonaDibujo->rcp_dibujoZona,
							&xmin, &ymin, &zmin, &xmax, &ymax, &zmax) == true)
	{
		i_putOrtho(zonaDibujo->xSize, zonaDibujo->ySize, 
					        xmin, ymin, zmin,
					        xmax, ymax, zmax,
					        graphics);
		
		rcpdibujo_redibujar(zonaDibujo->rcp_dibujoZona, graphics);
	}
}

//---------------------------------------------------------

static void i_recorreFuncionesDibujo(
						ArrTipo(rcpdibujo_t) *funciones_dibujo, 
						class IGraphics *grafico)
{
	unsigned long i, num_receptores;
	
	num_receptores = array_numElementos(rcpdibujo_t, funciones_dibujo);
	
	for (i = 0; i < num_receptores; i++)
	{
		struct rcpdibujo_t *receptor_dibujo;
			
		receptor_dibujo = array_Get(rcpdibujo_t, funciones_dibujo, i);
		rcpdibujo_redibujar(receptor_dibujo, grafico);
	}
} 

//---------------------------------------------------------

static void i_calculaCentro(const struct i_limites_t *limites, double *xcentro, double  *ycentro, double *zcentro)
{
    assert_no_null(limites);
    assert_no_null(xcentro);
    assert_no_null(ycentro);
    assert_no_null(zcentro);
    
    assert(limites->hayLimites == true);
    
    *xcentro = (limites->xmax + limites->xmin) / 2.; 
    *ycentro = (limites->ymax + limites->ymin) / 2.;
    *zcentro = (limites->zmax + limites->zmin) / 2.;
}

//---------------------------------------------------------

static void i_redibujarDibujos(
					enum i_tipoDibujo_t tipoDibujo,
					const struct i_limites_t *limites,
					const struct areadibujo_item_data_camera_t *dataCamera,
				    areadibujo_FPtr_positionCamera func_positionCamera,
				    ArrTipo(rcpdibujo_t) *funciones_dibujo,
					class IGraphics *graphics)
{
    bool se_dibuja;

    assert_no_null(limites);
    
    graphics->pushTransformation();
    
    switch (tipoDibujo)
    {
        case i_DIBUJO_2D:

            se_dibuja = limites->hayLimites;        
            break;
        
        case i_DIBUJO_3D:

            se_dibuja = true;
            assert_no_null(func_positionCamera);

            graphics->resetTransformation();

            func_positionCamera(dataCamera, graphics);
            
            if (limites->hayLimites == true)
            {
                double xcentro, ycentro, zcentro;
                
                i_calculaCentro(limites, &xcentro, &ycentro, &zcentro);
                graphics->traslation(-xcentro, -ycentro, -zcentro);
            }
            break;
        
        default_error();
    }

    if (se_dibuja == true)
	   i_recorreFuncionesDibujo(funciones_dibujo, graphics);
       
    graphics->popTransformation();
}

//---------------------------------------------------------

static void i_redibujarZonas(
					unsigned long xSize, 
					unsigned long ySize, 
					ArrTipo(i_zonaDibujo_t) *zonasDibujo, 
					class IGraphics *graphics)
{
	unsigned long i, num;
		
	num = array_numElementos(i_zonaDibujo_t, zonasDibujo);
		
	for (i = 0; i < num; i++)
	{
		const struct i_zonaDibujo_t *zonaDibujo;
			
		zonaDibujo = array_Get(i_zonaDibujo_t, zonasDibujo, i);
		i_dibujaZonaDibujo(zonaDibujo, xSize, ySize, graphics);
	}
}

//---------------------------------------------------------

static void i_redibujar(struct areaDibujo_t *area_dibujo, class IGraphics *graphics)
{
	unsigned long xSize, ySize;
	
	assert_no_null(area_dibujo);
	
    marcogrf_getDimensionAreaDib(area_dibujo->marco, area_dibujo->item_area_dibujo, &xSize, &ySize);
    
    graphics->viewport(0, 0, xSize, ySize);
	
	i_redibujarDibujos(area_dibujo->tipoDibujo,
						&area_dibujo->limites,
						area_dibujo->dataCamera,
						area_dibujo->func_positionCamera,
						area_dibujo->funciones_dibujo,
						graphics);
	
	i_redibujarZonas(xSize, ySize, area_dibujo->zonasDibujo, graphics);
}

//---------------------------------------------------------

static void i_recorreFuncionesDefineLayers(
						ArrTipo(rcpdibujo_t) *funciones_dibujo, 
						class IGraphics *grafico)
{
	unsigned long i, num_receptores;
	
	num_receptores = array_numElementos(rcpdibujo_t, funciones_dibujo);
	
	for (i = 0; i < num_receptores; i++)
	{
		struct rcpdibujo_t *receptor_dibujo;
			
		receptor_dibujo = array_Get(rcpdibujo_t, funciones_dibujo, i);
		rcpdibujo_defineLayers(receptor_dibujo, grafico);
	}
} 

//---------------------------------------------------------

static void i_recorreZonasDefineLayers(
						ArrTipo(i_zonaDibujo_t) *zonasDibujos, 
						class IGraphics *grafico)
{
	unsigned long i, num_receptores;
	
	num_receptores = array_numElementos(i_zonaDibujo_t, zonasDibujos);
	
	for (i = 0; i < num_receptores; i++)
	{
		struct i_zonaDibujo_t *zonaDibujo;
			
		zonaDibujo = array_Get(i_zonaDibujo_t, zonasDibujos, i);
		rcpdibujo_defineLayers(zonaDibujo->rcp_dibujoZona, grafico);
	}
} 

//---------------------------------------------------------

static void i_iniciaGrafico(struct areaDibujo_t* area_dibujo, class IGraphics * graphics)
{
    graphics->initDevice();
	i_recorreFuncionesDefineLayers(area_dibujo->funciones_dibujo, graphics);
	i_recorreZonasDefineLayers(area_dibujo->zonasDibujo, graphics);
}

//---------------------------------------------------------

static void i_calculaLimites(
						struct i_limites_t *limites,
						ArrTipo(rcpdibujo_t) *funciones_dibujo)
{
	class CGraphicsRect *graphicsLimits;
	class IGraphics *graphics;
	unsigned long i, num_receptores;
	
	assert_no_null(limites);
	
	graphicsLimits = new CGraphicsRect();
	graphics = graphicsLimits;

	num_receptores = array_numElementos(rcpdibujo_t, funciones_dibujo);
	
	for (i = 0; i < num_receptores; i++)
	{
		struct rcpdibujo_t *receptor_dibujo;
		double xmin, ymin, zmin, xmax, ymax, zmax;
			
		receptor_dibujo = array_Get(rcpdibujo_t, funciones_dibujo, i);
		
		if (rcpdibujo_calculaLimites(receptor_dibujo, &xmin, &ymin, &zmin, &xmax, &ymax, &zmax) == true)
		    graphics->drawLine(xmin, ymin, zmin, xmax, ymax, zmax);
	}
	
	limites->hayLimites = graphicsLimits->getLimits(
					&limites->xmin, &limites->ymin, &limites->zmin, 
					&limites->xmax, &limites->ymax, &limites->zmax);
					
	delete graphicsLimits;
}

//---------------------------------------------------------

static void i_definePerspectiva(
                    struct marcogrf_t *marco,
                    struct item_implementacion_t *item_area_dibujo,
                    class IGraphics *graphics)
{
    unsigned long width, heigth;
    double ratioXforY;
    double fovy, zNear, zFar;
    
    marcogrf_getDimensionAreaDib(marco, item_area_dibujo, &width, &heigth);
    
    assert(heigth > 0);

    fovy = 45.;
    zNear = 0.1;
    zFar = 1000.;
    ratioXforY = (double)width / (double)heigth;
    
    graphics->definePerpective(fovy, ratioXforY, zNear, zFar);
}

//---------------------------------------------------------

static void i_defineOrtho(
                        struct i_limites_t *limites,
                        ArrTipo(rcpdibujo_t) *funciones_dibujo,
                        struct item_implementacion_t *item_area_dibujo,
                        struct marcogrf_t *marco,
                        class IGraphics *graphics)
{
    assert_no_null(limites);

    if (limites->hayLimites == true)
    {
        unsigned long width, heigth;
        
        marcogrf_getDimensionAreaDib(marco, item_area_dibujo, &width, &heigth);
        
        i_putOrtho(width, heigth,
                limites->xmin, limites->ymin, limites->zmin,
                limites->xmax, limites->ymax, limites->zmax,
        		graphics);
    }
}

//---------------------------------------------------------

static void i_definePerspectivaSegunModel(struct areaDibujo_t* area_dibujo, class IGraphics *graphics)
{
    assert_no_null(area_dibujo);

    if (area_dibujo->limites.hayLimites == false)
        i_calculaLimites(&area_dibujo->limites, area_dibujo->funciones_dibujo);
    
    switch (area_dibujo->tipoDibujo) 
    {
        case i_DIBUJO_2D:
        
            i_defineOrtho(
                        &area_dibujo->limites,
                        area_dibujo->funciones_dibujo,
                        area_dibujo->item_area_dibujo,
                        area_dibujo->marco,
                        graphics);
            break;
        
        case i_DIBUJO_3D:
        {
            i_definePerspectiva(area_dibujo->marco, area_dibujo->item_area_dibujo, graphics);
            break;
        }
        default_error();
    }
}

//---------------------------------------------------------

static struct areaDibujo_t *i_crea(
                    enum i_tipoDibujo_t tipoDibujo,
                    unsigned long ancho, unsigned long alto,
                    bool hayLimites,
                    double xmin, double ymin, double zmin,
                    double xmax, double ymax, double zmax,
                    const struct areadibujo_item_data_camera_t *dataCamera,
                    areadibujo_FPtr_positionCamera func_positionCamera,
                    ArrTipo(rcpdibujo_t) **funciones_dibujo,
                    ArrTipo(i_zonaDibujo_t) **zonasDibujo,
                    struct callback_t **callback_iniciaGrafico, 
                    struct callback_t **callback_definePerspectiva,
                    struct callback_t **callback_dibuja,
                    struct callback_t **funcion_eventos_raton,
                    ArrTipo(RcpMouse_t) **receptores_raton,
                    struct item_implementacion_t **item_area_dibujo,
                    struct marcogrf_t **marco)
{
	struct areaDibujo_t *area_dibujo;
		
	area_dibujo = MALLOC(struct areaDibujo_t);

    area_dibujo->tipoDibujo = tipoDibujo;
        
    area_dibujo->ancho = ancho;
    area_dibujo->alto = alto;
    
    area_dibujo->limites.hayLimites = hayLimites;
     
    area_dibujo->limites.xmin = xmin; 
    area_dibujo->limites.ymin = ymin; 
    area_dibujo->limites.zmin = zmin;
    
    area_dibujo->limites.xmax = xmax; 
    area_dibujo->limites.ymax = ymax; 
    area_dibujo->limites.zmax = zmax;
    
    area_dibujo->dataCamera = dataCamera;
    area_dibujo->func_positionCamera = func_positionCamera;
    
    area_dibujo->funciones_dibujo = ASSIGN_PP_NO_NULL(funciones_dibujo, ArrTipo(rcpdibujo_t));
    area_dibujo->zonasDibujo = ASSIGN_PP_NO_NULL(zonasDibujo, ArrTipo(i_zonaDibujo_t));
    
    area_dibujo->callback_iniciaGrafico = ASSIGN_PP(callback_iniciaGrafico, struct callback_t); 
    area_dibujo->callback_definePerspectiva = ASSIGN_PP(callback_definePerspectiva,  struct callback_t);
    area_dibujo->callback_dibuja = ASSIGN_PP(callback_dibuja, struct callback_t);
    area_dibujo->funcion_eventos_raton = ASSIGN_PP(funcion_eventos_raton, struct callback_t);
        
    area_dibujo->receptores_raton = ASSIGN_PP_NO_NULL(receptores_raton, ArrTipo(RcpMouse_t));
        
    area_dibujo->item_area_dibujo = ASSIGN_PP(item_area_dibujo, struct item_implementacion_t);
    area_dibujo->marco = ASSIGN_PP(marco, struct marcogrf_t);
	
	return area_dibujo;
}

//---------------------------------------------------------

static void i_destruye(struct areaDibujo_t **area_dibujo)
{
	assert_no_null(area_dibujo);
	assert_no_null(*area_dibujo);
	
	array_Destruye(rcpdibujo_t, &(*area_dibujo)->funciones_dibujo, rcpdibujo_destruye);
	array_Destruye(i_zonaDibujo_t, &(*area_dibujo)->zonasDibujo, i_destruyeZonaDibujo);
	array_Destruye(RcpMouse_t, &(*area_dibujo)->receptores_raton, rcpmouse_destruye);
	
	callback_destruye(&(*area_dibujo)->callback_iniciaGrafico);
	callback_destruye(&(*area_dibujo)->callback_definePerspectiva);
	callback_destruye(&(*area_dibujo)->callback_dibuja);
    callback_destruye(&(*area_dibujo)->funcion_eventos_raton);
    
	FREE_T(area_dibujo, struct areaDibujo_t);
}

//---------------------------------------------------------

static void i_sendEventMouse(struct areaDibujo_t *area_dibujo, struct EvtMouse_t *evtMouse)
{
	unsigned long i, num_receptores;
    
    assert_no_null(area_dibujo);
    
	num_receptores = array_numElementos(RcpMouse_t, area_dibujo->receptores_raton);
	
	for (i = 0; i < num_receptores; i++)
	{
		struct RcpMouse_t *receptorMouse;
			
		receptorMouse = array_Get(RcpMouse_t, area_dibujo->receptores_raton, i);
		rcpmouse_sendEvent(receptorMouse, evtMouse);
	}
}

//---------------------------------------------------------

static struct item_implementacion_t *i_crea_implementacion(
						struct marcogrf_t *marco, 
						struct areaDibujo_t *area_dibujo,
                        struct item_implementacion_dialogo_t *dialogo_so)
{
    struct item_implementacion_t *item_implementacion;
    
	assert_no_null(area_dibujo);
    
	assert(area_dibujo->item_area_dibujo == NULL);
    assert(area_dibujo->marco == NULL);
    assert(area_dibujo->callback_iniciaGrafico == NULL);
    assert(area_dibujo->callback_definePerspectiva == NULL);
    assert(area_dibujo->callback_dibuja == NULL);
    assert(area_dibujo->funcion_eventos_raton == NULL);
    	
	area_dibujo->callback_iniciaGrafico = callback_crea_con_dato_externo(struct areaDibujo_t, area_dibujo, class IGraphics, i_iniciaGrafico, NULL);
	area_dibujo->callback_definePerspectiva = callback_crea_con_dato_externo(struct areaDibujo_t, area_dibujo, class IGraphics, i_definePerspectivaSegunModel, NULL);
	area_dibujo->callback_dibuja = callback_crea_con_dato_externo(struct areaDibujo_t, area_dibujo, class IGraphics, i_redibujar, NULL);
	
    area_dibujo->funcion_eventos_raton = callback_crea_con_dato_externo(struct areaDibujo_t, area_dibujo, struct EvtMouse_t, i_sendEventMouse, NULL);
    
	item_implementacion = marcogrf_crea_area_dibujo(
                        marco, dialogo_so,
					    area_dibujo->callback_iniciaGrafico,
					    area_dibujo->callback_definePerspectiva,
					    area_dibujo->callback_dibuja,
                        area_dibujo->funcion_eventos_raton,
                        area_dibujo->ancho, area_dibujo->alto);
    
    area_dibujo->item_area_dibujo = item_implementacion;
    area_dibujo->marco = marco;
    
    return area_dibujo->item_area_dibujo;
}

//---------------------------------------------------------
    
static struct panel_t *i_crea_panel_segun_tipo_dibujo(
                        enum i_tipoDibujo_t tipoDibujo,
                        unsigned long ancho, unsigned long alto,
                        const struct areadibujo_item_data_camera_t *dataCamera,
                        areadibujo_FPtr_positionCamera func_positionCamera,
                        struct rcpdibujo_t **funcion_dibujo, 
                        struct areaDibujo_t **area_dibujo_ptr)
{
    ArrTipo(rcpdibujo_t) *funciones_dibujo;
    struct areaDibujo_t *area_dibujo;
    struct componente_t *componente;
    struct callback_t *callback_iniciaGrafico; 
    struct callback_t *callback_definePerspectiva;
    struct callback_t *callback_dibuja;
    struct callback_t *funcion_eventos_raton;
    struct item_implementacion_t *item_area_dibujo;
    struct marcogrf_t *marco;
    bool hayLimites;
    double xmin, ymin, zmin;
    double xmax, ymax, zmax;
    ArrTipo(i_zonaDibujo_t) *zonasDibujo;
    ArrTipo(RcpMouse_t) *receptores_raton;
    
    assert_no_null(funcion_dibujo);
    assert_no_null(*funcion_dibujo);
    
    zonasDibujo = array_CreaPuntero(0, i_zonaDibujo_t);
    funciones_dibujo = array_CreaPuntero(1, rcpdibujo_t);
    array_Set(rcpdibujo_t, funciones_dibujo, 0, *funcion_dibujo);
    
    hayLimites = false;
    xmin = 0.; ymin = 0.; zmin = 0.;
    xmax = 0.; ymax = 0.; zmax = 0.;
    
    *funcion_dibujo = NULL;
    callback_iniciaGrafico = NULL; 
    callback_definePerspectiva = NULL;
    callback_dibuja = NULL;
    funcion_eventos_raton = NULL;

    item_area_dibujo = NULL;
    marco = NULL;
    
    receptores_raton = array_CreaPuntero(0, RcpMouse_t);
    
    area_dibujo = i_crea(
                    tipoDibujo, ancho, alto,
                    hayLimites,
                    xmin, ymin, zmin,
                    xmax, ymax, zmax,
                    dataCamera, func_positionCamera,
                    &funciones_dibujo,
                    &zonasDibujo,
                    &callback_iniciaGrafico, &callback_definePerspectiva,
                    &callback_dibuja, &funcion_eventos_raton,
                    &receptores_raton,
                    &item_area_dibujo,
                    &marco);
    
    if (area_dibujo_ptr != NULL)
        *area_dibujo_ptr = area_dibujo;
    
    componente = componente_crea_defecto(areaDibujo_t, &area_dibujo, i_crea_implementacion, i_destruye, NULL, NULL, NULL, NULL);
    
    return panel_crea_componente(&componente);
}

//---------------------------------------------------------
	
struct panel_t *areadibujo_crea_modelo2d(
                    unsigned long ancho, unsigned long alto,
                    struct rcpdibujo_t **funcion_dibujo,
                    struct areaDibujo_t **area_dibujo_ptr)
{
    enum i_tipoDibujo_t tipoDibujo;
    const struct areadibujo_item_data_camera_t *dataCamera;
    areadibujo_FPtr_positionCamera func_positionCamera;

    tipoDibujo = i_DIBUJO_2D; 

    dataCamera = NULL;
    func_positionCamera = NULL;

    return i_crea_panel_segun_tipo_dibujo(
                        tipoDibujo, ancho, alto,
                        dataCamera, func_positionCamera,
                        funcion_dibujo, area_dibujo_ptr);
}

//---------------------------------------------------------
    
struct panel_t *areadibujo_nousar_crea_modelo3d(
                        unsigned long ancho, unsigned long alto,
                        const struct areadibujo_item_data_camera_t *dataCamera,
                        areadibujo_FPtr_positionCamera func_positionCamera,
                        struct rcpdibujo_t **funcion_dibujo,
                        struct areaDibujo_t **area_dibujo_ptr)
{
    enum i_tipoDibujo_t tipoDibujo;
                        
    tipoDibujo = i_DIBUJO_3D; 
    
    return i_crea_panel_segun_tipo_dibujo(
                        tipoDibujo, ancho, alto,
                        dataCamera, func_positionCamera,
                        funcion_dibujo, area_dibujo_ptr);
}

//---------------------------------------------------------
	
void areadibujo_appendReceptorMouse(
                        struct areaDibujo_t *areaDibujo,
                        struct RcpMouse_t **receptorMouse) 
{
    assert_no_null(areaDibujo);
    assert_no_null(receptorMouse);
    assert_no_null(*receptorMouse);

	array_Anyadir(RcpMouse_t, areaDibujo->receptores_raton, *receptorMouse);
	*receptorMouse = NULL;    
}

//---------------------------------------------------------

void areadibujo_redraw(struct areaDibujo_t *areaDibujo)
{
	assert_no_null(areaDibujo);
	marcogrf_redraw(areaDibujo->marco, areaDibujo->item_area_dibujo);
}

//---------------------------------------------------------

void areadibujo_size(struct areaDibujo_t *areaDibujo, unsigned long *width, unsigned long *heigth)
{
    assert_no_null(areaDibujo);
    marcogrf_getDimensionAreaDib(areaDibujo->marco, areaDibujo->item_area_dibujo, width, heigth);
}

//---------------------------------------------------------

void areadibujo_appendZonaDibujoIzqInferior(
							struct areaDibujo_t *areaDibujo, 
							unsigned long xSizePx, 
							unsigned long ySizePx, 
							struct rcpdibujo_t **rcp_dibujoZona)
{
	struct i_zonaDibujo_t *zonaDibujo;
	
	assert_no_null(areaDibujo);
	
	zonaDibujo = i_creaZonaDibujo(i_ZONA_IZQ_INFERIOR, xSizePx, ySizePx, rcp_dibujoZona);
	array_Anyadir(i_zonaDibujo_t, areaDibujo->zonasDibujo, zonaDibujo);
}

//---------------------------------------------------------

void areadibujo_appendZonaDibujoDerInferior(
							struct areaDibujo_t *areaDibujo, 
							unsigned long xSizePx, 
							unsigned long ySizePx, 
							struct rcpdibujo_t **rcp_dibujoZona)
{
	struct i_zonaDibujo_t *zonaDibujo;
	
	assert_no_null(areaDibujo);
	
	zonaDibujo = i_creaZonaDibujo(i_ZONA_DER_INFERIOR, xSizePx, ySizePx, rcp_dibujoZona);
	array_Anyadir(i_zonaDibujo_t, areaDibujo->zonasDibujo, zonaDibujo);
}
