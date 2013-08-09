// Definicion area dibujo

#include "areadibujo.hxx"

struct panel_t *areadibujo_crea_modelo2d(
						unsigned long ancho, unsigned long alto,
                        struct rcpdibujo_t **funcion_dibujo,
                        struct areaDibujo_t **area_dibujo_ptr);

struct panel_t *areadibujo_nousar_crea_modelo3d(
                        unsigned long ancho, unsigned long alto,
                        const struct areadibujo_item_data_camera_t *dataCamera,
                        areadibujo_FPtr_positionCamera func_positionCamera,
                        struct rcpdibujo_t **funcion_dibujo,
                        struct areaDibujo_t **area_dibujo_ptr);
                        
#define areadibujo_crea_modelo3d(\
                        ancho, alto,\
                        typeDataCamera, dataCamera,\
                        func_positionCamera,\
                        funcion_dibujo, area_dibujo_ptr)\
(\
    (void)((const typeDataCamera *)dataCamera == dataCamera),\
    AREADIBUJO_FUNC_POSITION_CAMERA(func_positionCamera, typeDataCamera),\
    areadibujo_nousar_crea_modelo3d(\
                        ancho, alto,\
                        (const struct areadibujo_item_data_camera_t *)dataCamera,\
                        (areadibujo_FPtr_positionCamera)func_positionCamera,\
                        funcion_dibujo, area_dibujo_ptr)\
)

void areadibujo_appendReceptorMouse(struct areaDibujo_t *areaDibujo, struct RcpMouse_t **receptorMouse);

void areadibujo_redraw(struct areaDibujo_t *areaDibujo);

void areadibujo_size(struct areaDibujo_t *areaDibujo, unsigned long *width, unsigned long *heigth);

void areadibujo_appendZonaDibujoIzqInferior(
							struct areaDibujo_t *areaDibujo, 
							unsigned long xSizePx, 
							unsigned long ySizePx, 
							struct rcpdibujo_t **rcp_dibujoZona);

void areadibujo_appendZonaDibujoDerInferior(
							struct areaDibujo_t *areaDibujo, 
							unsigned long xSizePx, 
							unsigned long ySizePx, 
							struct rcpdibujo_t **rcp_dibujoZona);
