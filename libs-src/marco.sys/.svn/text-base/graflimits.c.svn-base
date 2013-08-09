// Calcula los limites de un dibujo.

#include "graflimits.h"

#include "grafico.syh"
#include "grafico.h"

#include "memory.h"
#include "asrtbas.h"
#include "CMath.hpp"

struct i_limites_t
{
	bool hay_limites;
    bool con_margen;
	double ox, oy, ex, ey;
};

//---------------------------------------------------------

static void i_integridad(const struct i_limites_t *limites)
{
	assert_no_null(limites);
	if (limites->hay_limites == true)
	{
		assert(limites->ox < limites->ex);
		assert(limites->oy < limites->ey);
	}
}


//---------------------------------------------------------

static struct i_limites_t *i_crea(
						bool hay_limites,
						bool con_margen,
						double ox, double oy, double ex, double ey)
{
	struct i_limites_t *limites;
		
	limites = MALLOC(struct i_limites_t);
	
	limites->hay_limites = hay_limites;
    limites->con_margen = con_margen;
	limites->ox = ox;
	limites->oy = oy;
	limites->ex = ex;
	limites->ey = ey;
	
	i_integridad(limites);
	
	return limites;
}

//---------------------------------------------------------

static void i_destruye(struct i_limites_t **limites)
{
	assert_no_null(limites);
	FREE_T(limites, struct i_limites_t);
}

//---------------------------------------------------------

static void i_maximiza(
					bool *hay_limites,
					double *ox, double *oy,
					double *ex, double *ey,
					double x, double y)
{
	assert_no_null(hay_limites);
	assert_no_null(ox);
	assert_no_null(oy);
	assert_no_null(ex);
	assert_no_null(ey);
	
	*ox = MIN(*ox, x);
	*oy = MIN(*oy, y);
	
	*ex = MAX(*ex, x);
	*ey = MAX(*ey, y);
	
	*hay_limites = true;
}

//---------------------------------------------------------

static void i_dibuja_linea(struct i_limites_t *limites, double x1, double y1, double x2, double y2)
{
	assert_no_null(limites);
	
	i_maximiza(&limites->hay_limites, &limites->ox, &limites->oy, &limites->ex, &limites->ey, x1, y1);
	i_maximiza(&limites->hay_limites, &limites->ox, &limites->oy, &limites->ex, &limites->ey, x2, y2);
}

//---------------------------------------------------------

static void i_dibuja_rectangulo(struct i_limites_t *limites, double x1, double y1, double x2, double y2)
{
	assert_no_null(limites);
	
	i_maximiza(&limites->hay_limites, &limites->ox, &limites->oy, &limites->ex, &limites->ey, x1, y1);
	i_maximiza(&limites->hay_limites, &limites->ox, &limites->oy, &limites->ex, &limites->ey, x2, y2);
}

//---------------------------------------------------------

static struct grafico_t *i_crea_defecto(bool con_margen)
{
	struct i_limites_t *limites;
	bool hay_limites;
	double ox, oy, ex, ey;
	
	hay_limites = false;
	ox = 100000.; 
	oy = 100000.; 
	ex = -100000.; 
	ey = -100000.; 
	
	limites = i_crea(hay_limites, con_margen, ox, oy, ex, ey);
	
	return grafico_crea_dispositivo(
						i_limites_t, &limites,
						i_destruye, i_dibuja_linea, 
						i_dibuja_rectangulo,
						i_dibuja_rectangulo,  NULL);
}

//---------------------------------------------------------

struct grafico_t *graflimits_crea(void)
{
    bool con_margen;
    
    con_margen = false;
    return i_crea_defecto(con_margen);
}

//---------------------------------------------------------

struct grafico_t *graflimits_crea_con_margen(void)
{
    bool con_margen;
    
    con_margen = true;
    return i_crea_defecto(con_margen);
}

//---------------------------------------------------------

void graflimits_obten_limites(
						struct grafico_t *grafico, 
						double *ox, double *oy, double *ex, double *ey)
{
	struct i_limites_t *limites;
	
	assert_no_null(ox);
	assert_no_null(oy);
	assert_no_null(ex);
	assert_no_null(ey);
	
	limites = grafico_obten_datos_grafico(grafico, i_limites_t);
	
	i_integridad(limites);
    
	assert(limites->hay_limites == true);
    
    if (limites->con_margen == true)
        grafico_amplia_margen(&(limites->ox), &(limites->oy), &(limites->ex), &(limites->ey));
	
	*ox = limites->ox;
	*oy = limites->oy;
	*ex = limites->ex;
	*ey = limites->ey;
}
