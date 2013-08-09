// Herramienta para dibujar en un area de dibujo

#include "grafico.hxx"
#include "grafico.h"
#include "grafico.syh"

#include "memory.h"
#include "asrtbas.h"
#include "strbas.h"

struct grafico_t
{
	struct grafico_item_datos_t *datos;
	char *tipo_dato;
	
	grafico_FPtr_destruye func_destruye;
	grafico_FPtr_dibuja_linea func_dibuja_linea;
	grafico_FPtr_dibuja_rectangulo func_dibuja_rectangulo;
	grafico_FPtr_dibuja_rectangulo_relleno func_dibuja_rectangulo_relleno;
	grafico_FPtr_cambia_color func_cambia_color;
};

//---------------------------------------------------------

struct grafico_t *grafico_no_usar_crea_dispositivo(
						struct grafico_item_datos_t **datos,
						const char *tipo_dato,
						grafico_FPtr_destruye func_destruye,
						grafico_FPtr_dibuja_linea func_dibuja_linea,
						grafico_FPtr_dibuja_rectangulo func_dibuja_rectangulo,
						grafico_FPtr_dibuja_rectangulo_relleno func_dibuja_rectangulo_relleno,
						grafico_FPtr_cambia_color func_cambia_color)
{
	struct grafico_t *grafico;
		
	grafico = MALLOC(struct grafico_t);

	grafico->datos = ASSIGN_PP(datos, struct grafico_item_datos_t);
	grafico->tipo_dato = strbas_copyString(tipo_dato);
		
	grafico->func_destruye = func_destruye;
	grafico->func_dibuja_linea = func_dibuja_linea;
	grafico->func_dibuja_rectangulo = func_dibuja_rectangulo;
	grafico->func_dibuja_rectangulo_relleno = func_dibuja_rectangulo_relleno;
	grafico->func_cambia_color = func_cambia_color;
	
	return grafico;
}
	
//---------------------------------------------------------

void grafico_destruye(struct grafico_t **grafico)
{
	assert_no_null(grafico);
	assert_no_null(*grafico);
	assert_no_null((*grafico)->func_destruye);
	(*grafico)->func_destruye(&(*grafico)->datos);
	
	strbas_destroy(&(*grafico)->tipo_dato);
	
	FREE_T(grafico, struct grafico_t);
}

//---------------------------------------------------------

void grafico_dibuja_linea(struct grafico_t *grafico, double x1, double y1, double x2, double y2)
{
	assert_no_null(grafico);
	assert_no_null(grafico->func_dibuja_linea);
	grafico->func_dibuja_linea(grafico->datos, x1, y1, x2, y2);
}

//---------------------------------------------------------

void grafico_dibuja_rectangulo(struct grafico_t *grafico, double x1, double y1, double x2, double y2)
{
	assert_no_null(grafico);
	assert_no_null(grafico->func_dibuja_rectangulo);
	grafico->func_dibuja_rectangulo(grafico->datos, x1, y1, x2, y2);
}

//---------------------------------------------------------

void grafico_dibuja_rectangulo_relleno(struct grafico_t *grafico, double x1, double y1, double x2, double y2)
{
	assert_no_null(grafico);
	assert_no_null(grafico->func_dibuja_rectangulo_relleno);
	grafico->func_dibuja_rectangulo_relleno(grafico->datos, x1, y1, x2, y2);
}

//---------------------------------------------------------

void grafico_cambia_color(struct grafico_t *grafico, unsigned long color)
{
	assert_no_null(grafico);
	if (grafico->func_cambia_color != NULL)
		grafico->func_cambia_color(grafico->datos, color);
}

//---------------------------------------------------------

struct grafico_item_datos_t *grafico_no_usar_obten_datos_grafico(
						struct grafico_t *grafico, 
						const char *tipo_dato)
{
	assert_no_null(grafico);
	assert(strbas_equalsStrings(grafico->tipo_dato, tipo_dato) == true);
	
	return grafico->datos;
}

//---------------------------------------------------------

void grafico_amplia_margen(double *ox, double *oy, double *ex, double *ey)
{
    double margen_x, margen_y;
    
    assert_no_null(ox);
    assert_no_null(oy);
    assert_no_null(ex);
    assert_no_null(ey);
    
    margen_x = 0.05 * (*ex - *ox);
    margen_y = 0.05 * (*ey - *oy);
    
    *ox -= margen_x;
    *oy -= margen_y;
    *ex += margen_x;
    *ey += margen_y;
}



