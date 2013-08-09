// Receptores de redibujado

typedef void (*rcpdibujo_FPtr_dibujo)(const struct item_datos_dibujo_t *datos_dibujo, class IGraphics *grafico);
#define COMPROBAR_FUNC_DIBUJO(funcion, tipo) (void)((void (*)(const struct tipo *, class IGraphics *))funcion == funcion)
#define COMPROBAR_FUNC_DIBUJO_CLASS(funcion, tipo) (void)((void (*)(const class tipo *, class IGraphics *))funcion == funcion)

typedef void (*rcpdibujo_FPtr_defineLayers)(const struct item_datos_dibujo_t *datos_dibujo, class IGraphics *grafico);
#define COMPROBAR_FUNC_DEFINE_LAYERS(funcion, tipo) (void)((void (*)(const struct tipo *, class IGraphics *))funcion == funcion)
#define COMPROBAR_FUNC_DEFINE_LAYERS_CLASS(funcion, tipo) (void)((void (*)(const class tipo *, class IGraphics *))funcion == funcion)

typedef bool (*rcpdibujo_FPtr_limites)(const struct item_datos_dibujo_t *datos_dibujo, double *ox, double *oy, double *oz, double *ex, double *ey, double *ez);
#define COMPROBAR_FUNC_DEFINE_LIMITES(funcion, tipo) (void)((bool (*)(const struct tipo *, double *ox, double *oy, double *oz, double *ex, double *ey, double *ez))funcion == funcion)
#define COMPROBAR_FUNC_DEFINE_LIMITES_CLASS(funcion, tipo) (void)((bool (*)(const class tipo *, double *ox, double *oy, double *oz, double *ex, double *ey, double *ez))funcion == funcion)
	
struct rcpdibujo_t *rcpdibujo_no_usar_crea(
					rcpdibujo_FPtr_defineLayers func_defineLayers,
					rcpdibujo_FPtr_dibujo func_dibujo,
					rcpdibujo_FPtr_limites func_limites,
					const struct item_datos_dibujo_t *datos_dibujo);
						
#define rcpdibujo_crea(func_defineLayers, func_dibujo, tipo, datos_dibujo)\
					(\
					(void)((const struct tipo *)datos_dibujo == datos_dibujo),\
					COMPROBAR_FUNC_DEFINE_LAYERS(func_defineLayers, tipo),\
					COMPROBAR_FUNC_DIBUJO(func_dibujo, tipo),\
					rcpdibujo_no_usar_crea(\
						(rcpdibujo_FPtr_defineLayers)func_defineLayers,\
						(rcpdibujo_FPtr_dibujo)func_dibujo,\
						(rcpdibujo_FPtr_limites)NULL,\
						(const struct item_datos_dibujo_t *)datos_dibujo)\
					)

#define rcpdibujo_crea_con_limites(func_defineLayers, func_dibujo, func_limites, tipo, datos_dibujo)\
					(\
					(void)((const struct tipo *)datos_dibujo == datos_dibujo),\
					COMPROBAR_FUNC_DEFINE_LAYERS(func_defineLayers, tipo),\
					COMPROBAR_FUNC_DIBUJO(func_dibujo, tipo),\
					COMPROBAR_FUNC_DEFINE_LIMITES(func_limites, tipo),\
					rcpdibujo_no_usar_crea(\
						(rcpdibujo_FPtr_defineLayers)func_defineLayers,\
						(rcpdibujo_FPtr_dibujo)func_dibujo,\
						(rcpdibujo_FPtr_limites)func_limites,\
						(const struct item_datos_dibujo_t *)datos_dibujo)\
					)

#define rcpdibujo_crea_class(func_defineLayers, func_dibujo, tipo, datos_dibujo)\
					(\
					(void)((const class tipo *)datos_dibujo == datos_dibujo),\
					COMPROBAR_FUNC_DEFINE_LAYERS_CLASS(func_defineLayers, tipo),\
					COMPROBAR_FUNC_DIBUJO_CLASS(func_dibujo, tipo),\
					rcpdibujo_no_usar_crea(\
						(rcpdibujo_FPtr_defineLayers)func_defineLayers,\
						(rcpdibujo_FPtr_dibujo)func_dibujo,\
						(rcpdibujo_FPtr_limites)NULL,\
						(const struct item_datos_dibujo_t *)datos_dibujo)\
					)
						
					
			
					
			
