//opcion.h

typedef void (*opcion_FPtr_ejecutar)(struct marco_t *marco, struct opcion_item_datos_t *datos_opcion);
#define DECLARA_FUNCION_FPTR(tipo_dato, funcion) (void)((void (*)(struct marco_t *, struct tipo_dato *))funcion == funcion)
	
typedef void (*opcion_FPtr_ejecutar_con_parametro)(struct marco_t *, struct opcion_item_datos_t *datos_opcion, const char *parametro);
#define DECLARA_FUNCION_CON_PARAMETRO_FPTR(tipo_dato, funcion) (void)((void (*)(struct marco_t *, struct tipo_dato *, const char *))funcion == funcion)

typedef bool (*opcion_FPtr_se_puede_ejecutar)(const struct opcion_item_datos_t *datos_opcion, char **mensaje_error);
#define DECLARA_FUNCION_SE_PUEDE_EJECUTAR(tipo_dato, funcion) (void)((bool (*)(const struct tipo_dato *, char **mensaje_error))funcion == funcion)
	
struct opcion_t *opcion_no_usar_crea_opcion_simple(
						const char *id_ejecutar,
						const char *titulo_opcion,
						const char *help,
						opcion_FPtr_ejecutar funcion_ejecutar,
						opcion_FPtr_se_puede_ejecutar funcion_se_puede_ejecutar_opc);
#define opcion_crea_opcion_simple(\
			id_ejecutar, titulo_opcion, help,\
			tipo_dato, funcion_ejecutar, funcion_se_puede_ejecutar_opc)\
			(\
				DECLARA_FUNCION_FPTR(tipo_dato, funcion_ejecutar),\
				DECLARA_FUNCION_SE_PUEDE_EJECUTAR(tipo_dato, funcion_se_puede_ejecutar_opc),\
				opcion_no_usar_crea_opcion_simple(id_ejecutar, titulo_opcion, help,\
						(opcion_FPtr_ejecutar)funcion_ejecutar,\
						(opcion_FPtr_se_puede_ejecutar)funcion_se_puede_ejecutar_opc)\
			)

struct opcion_t *opcion_no_usar_crea_opcion_con_parametro(
						const char *id_ejecutar,
						const char *titulo_opcion,
						const char *help,
						opcion_FPtr_ejecutar_con_parametro funcion_ejecutar_con_parametro,
						opcion_FPtr_se_puede_ejecutar funcion_se_puede_ejecutar_opc);
#define opcion_crea_opcion_con_parametro(\
			id_ejecutar, titulo_opcion, help, tipo_dato, funcion_ejecutar_con_parametros, funcion_se_puede_ejecutar_opc)\
			(\
				DECLARA_FUNCION_CON_PARAMETRO_FPTR(tipo_dato, funcion_ejecutar_con_parametros),\
				DECLARA_FUNCION_SE_PUEDE_EJECUTAR(tipo_dato, funcion_se_puede_ejecutar_opc),\
				opcion_no_usar_crea_opcion_con_parametro(\
						id_ejecutar, titulo_opcion, help,\
						(opcion_FPtr_ejecutar_con_parametro)funcion_ejecutar_con_parametros,\
						(opcion_FPtr_se_puede_ejecutar)funcion_se_puede_ejecutar_opc)\
			)

void opcion_destruye(struct opcion_t **opcion);

