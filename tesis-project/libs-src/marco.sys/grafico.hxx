#ifndef __GRAFICO__
#define __GRAFICO__

struct grafico_item_datos_t;
	
typedef void (*grafico_FPtr_destruye)(struct grafico_item_datos_t **datos_graficos);
#define GRAFICO_COMPRUEBA_FPTR_DESTRUYE(funcion, tipo) (void)((void (*)(struct tipo **))funcion == funcion)
	
typedef void (*grafico_FPtr_dibuja_linea)(struct grafico_item_datos_t *datos_graficos, double x1, double y1, double x2, double y2);
#define GRAFICO_COMPRUEBA_FPTR_DIBUJA_LINEA(funcion, tipo) (void)((void (*)(struct tipo *, double , double , double , double ))funcion == funcion)
	
typedef void (*grafico_FPtr_dibuja_rectangulo)(struct grafico_item_datos_t *datos_graficos, double x1, double y1, double x2, double y2);
#define GRAFICO_COMPRUEBA_FPTR_DIBUJA_RECTANGULO(funcion, tipo) (void)((void (*)(struct tipo *, double , double , double , double ))funcion == funcion)

typedef void (*grafico_FPtr_dibuja_rectangulo_relleno)(struct grafico_item_datos_t *datos_graficos, double x1, double y1, double x2, double y2);
#define GRAFICO_COMPRUEBA_FPTR_DIBUJA_RECTANGULO_RELLENO(funcion, tipo) (void)((void (*)(struct tipo *, double , double , double , double ))funcion == funcion)

typedef void (*grafico_FPtr_cambia_color)(struct grafico_item_datos_t *datos_graficos, unsigned long color);
#define GRAFICO_COMPRUEBA_FPTR_CAMBIA_COLOR(funcion, tipo) (void)((void (*)(struct tipo *, unsigned long))funcion == funcion)
					
	
#endif	
					
