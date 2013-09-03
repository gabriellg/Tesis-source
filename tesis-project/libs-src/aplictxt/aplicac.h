//aplicac.h

typedef void (*aplicac_FPtr_destruye)(struct opcion_item_datos_t **datos);
typedef struct opcion_item_datos_t *(*aplicac_FPtr_lee_datos)(struct marco_t *marco, int argc, char **argv, const ArrString *parametros);
#define DECLARAR_FUNC_LEE_DATOS(func, tipo) (void)((struct tipo *(*)(struct marco_t *marco, int argc, char **argv, const ArrString *))func == func)
#define DECLARAR_FUNC_DESTRUYE(func, tipo) (void)((void (*)(struct tipo **))func == func)

int aplicac_no_usar_lanzar(
				int argc, 
				char *argv[], 
				const char *titulo_aplicacion,
				const char *explicacion,
				const char *txt_version,
				aplicac_FPtr_lee_datos funcion_lee_datos,
				aplicac_FPtr_destruye funcion_destruye_datos,
				const ArrTipo(opcion_t) *opciones);
					
#define aplicac_lanzar(argc, argv, titulo, explicacion, txt_version, func_lee_datos, func_destruye_datos, opciones, tipo_datos) (DECLARAR_FUNC_DESTRUYE(func_destruye_datos, tipo_datos), DECLARAR_FUNC_LEE_DATOS(func_lee_datos, tipo_datos), aplicac_no_usar_lanzar(argc, argv, titulo, explicacion, txt_version, (aplicac_FPtr_lee_datos)func_lee_datos,(aplicac_FPtr_destruye)func_destruye_datos,opciones))
				
				
