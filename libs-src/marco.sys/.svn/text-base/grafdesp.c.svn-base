// Grafico de desplazamiento.

#include "grafdesp.h"

#include "grafico.h"
#include "grafico.syh"
#include "memory.h"
#include "asrtbas.h"

struct i_desplazamiento_t
{
	struct grafico_t *grafico;
    double dx, dy;
};

//---------------------------------------------------------

static struct i_desplazamiento_t *i_crea(struct grafico_t *grafico, double dx, double dy)
{
	struct i_desplazamiento_t *desplazamiento;
		
	desplazamiento = MALLOC(struct i_desplazamiento_t);
		
	desplazamiento->grafico = grafico;
	desplazamiento->dx = dx;
    desplazamiento->dy = dy;
	
	return desplazamiento;
}

//---------------------------------------------------------

static void i_destruye(struct i_desplazamiento_t **desplazamiento)
{
	assert_no_null(desplazamiento);
	assert_no_null(*desplazamiento);
	
	FREE_T(desplazamiento, struct i_desplazamiento_t);
}

//---------------------------------------------------------

static void i_transforma_punto(
						const struct i_desplazamiento_t *desplazamiento, 
						double x, double y, double *xt, double *yt)
{
	assert_no_null(desplazamiento);
	assert_no_null(xt);
	assert_no_null(yt);
	
	*xt = x + desplazamiento->dx;
	*yt = y + desplazamiento->dy;
}

//---------------------------------------------------------

static void i_dibuja_linea(struct i_desplazamiento_t *desplazamiento, double x1, double y1, double x2, double y2)
{
	double x1t, y1t, x2t, y2t;
	
	assert_no_null(desplazamiento);
	
	i_transforma_punto(desplazamiento, x1, y1, &x1t, &y1t);
	i_transforma_punto(desplazamiento, x2, y2, &x2t, &y2t);

	grafico_dibuja_linea(desplazamiento->grafico, x1t, y1t, x2t, y2t);
}

//---------------------------------------------------------

static void i_dibuja_rectangulo(struct i_desplazamiento_t *desplazamiento, double x1, double y1, double x2, double y2)
{
	double x1t, y1t, x2t, y2t;
	
	assert_no_null(desplazamiento);
	
	i_transforma_punto(desplazamiento, x1, y1, &x1t, &y1t);
	i_transforma_punto(desplazamiento, x2, y2, &x2t, &y2t);

	grafico_dibuja_rectangulo(desplazamiento->grafico, x1t, y1t, x2t, y2t);
}

//---------------------------------------------------------

static void i_dibuja_rectangulo_relleno(struct i_desplazamiento_t *desplazamiento, double x1, double y1, double x2, double y2)
{
	double x1t, y1t, x2t, y2t;
	
	assert_no_null(desplazamiento);
	
	i_transforma_punto(desplazamiento, x1, y1, &x1t, &y1t);
	i_transforma_punto(desplazamiento, x2, y2, &x2t, &y2t);

	grafico_dibuja_rectangulo_relleno(desplazamiento->grafico, x1t, y1t, x2t, y2t);
}

//---------------------------------------------------------

static void i_cambia_color(struct i_desplazamiento_t *desplazamiento, unsigned long color)
{
	assert_no_null(desplazamiento);
	grafico_cambia_color(desplazamiento->grafico, color);
}

//---------------------------------------------------------

struct grafico_t *grafdesp_crea(struct grafico_t *grafico, double dx, double dy)
{
	struct i_desplazamiento_t *desplazamiento;
	
	desplazamiento = i_crea(grafico, dx, dy);
	
	return grafico_crea_dispositivo(
						i_desplazamiento_t, &desplazamiento, 
						i_destruye, i_dibuja_linea, i_dibuja_rectangulo, 
						i_dibuja_rectangulo_relleno, i_cambia_color);
}
