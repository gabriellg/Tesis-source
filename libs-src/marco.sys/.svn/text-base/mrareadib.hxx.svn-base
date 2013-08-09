// Define las funciones necesarias para las �rea de dibujo.
#ifndef MRAREADIB_HXX_
#define MRAREADIB_HXX_

typedef struct item_implementacion_t *(*mrareadib_FPtr_crea_area_dibujo)(
						struct item_implementacion_dialogo_t *dlg_padre,
						struct callback_t *callback_iniciaGrafico, 
						struct callback_t *callback_definePerspectiva,
						struct callback_t *callback_dibuja,
						struct callback_t *funcion_evento_raton, 
						unsigned long ancho, unsigned long alto);
						
typedef class IGraphics *(*mrareadib_FPtr_getGraphics)(struct item_implementacion_t *area_dibujo);

typedef void (*mrareadib_FPtr_getDimension)(struct item_implementacion_t *area_dibujo, unsigned long *width, unsigned long *heigth);

typedef void (*mrareadib_FPtr_redraw)(struct item_implementacion_t *area_dibujo);

#endif /*MRAREADIB_HXX_*/
