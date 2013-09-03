// Componente que nos da el area de dibujo.

struct item_implementacion_t *gtkdibujo_crea(
						struct item_implementacion_dialogo_t *dlg_padre, 
						struct callback_t *callback_iniciaGrafico, 
						struct callback_t *callback_definePerspectiva,
						struct callback_t *callback_dibuja,
						struct callback_t *funcion_evento_raton,
						unsigned long ancho, unsigned long alto);

class IGraphics *gtkdibujo_getGraphics(struct item_implementacion_t *area_dibujo);

void gtkdibujo_getDimension(struct item_implementacion_t *area_dibujo, unsigned long *width, unsigned long *height);

void gtkdibujo_redraw(struct item_implementacion_t *area_dibujo);
