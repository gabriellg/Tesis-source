// Definici�n de Componente

typedef struct item_implementacion_t *(*componente_FPtr_crear_implementacion)(struct marcogrf_t *marco, struct componente_datos_item_t *datos, struct item_implementacion_dialogo_t *dialogo_so);
typedef void (*componente_FPtr_destruye)(struct componente_datos_item_t **datos);
typedef bool (*componente_FPtr_se_puede_aceptar)(const struct marcogrf_t *marco, struct componente_datos_item_t *datos, char **mensaje_error);
typedef void (*componente_FPtr_aceptar)(const struct marcogrf_t *marco, struct componente_datos_item_t *datos);
typedef ArrString *(*componente_FPtr_cadena)(const struct componente_datos_item_t *datos);

#define COMPROBAR_FUNC_CREA_IMPLEMENTACION(funcion, tipo) (void)((struct item_implementacion_t *(*)(struct marcogrf_t *, struct tipo *, struct item_implementacion_dialogo_t *)) funcion == funcion)
#define COMPROBAR_FUNC_DESTRUYE(funcion, tipo) (void)((void (*)(struct tipo **)) funcion == funcion)
#define COMPROBAR_FUNC_SE_PUEDE_ACEPTAR(funcion, tipo) (void)((bool (*)(const struct marcogrf_t *marco, struct tipo *, char **mensaje_error)) funcion == funcion)
#define COMPROBAR_FUNC_ACEPTAR(funcion, tipo) (void)((void (*)(const struct marcogrf_t *marco, struct tipo *)) funcion == funcion)
#define COMPROBAR_FUNC_CADENA(funcion, tipo) (void)((ArrString *(*)(const struct tipo *)) funcion == funcion)

typedef ArrString *(*componente_FPtr_cadenas)(const struct componente_datos_item_t *datos);


struct componente_t *componente_no_usar_crea_defecto(
						const char *tipo,
						struct componente_datos_item_t **datos,
					    componente_FPtr_crear_implementacion func_crea_implementacion,
						componente_FPtr_destruye func_destruye,
                        componente_FPtr_se_puede_aceptar func_se_puede_aceptar,
                        componente_FPtr_aceptar func_aceptar,
                        componente_FPtr_cadena func_titulo,
                        componente_FPtr_cadena func_dato);
#define componente_crea_defecto(tipo, datos, func_crea_implementacion, func_destruye, func_se_puede_aceptar, func_aceptar, func_titulo, func_dato)\
	(\
	(void)((struct tipo **)datos == datos),\
	COMPROBAR_FUNC_CREA_IMPLEMENTACION(func_crea_implementacion, tipo),\
	COMPROBAR_FUNC_DESTRUYE(func_destruye, tipo),\
    COMPROBAR_FUNC_SE_PUEDE_ACEPTAR(func_se_puede_aceptar, tipo),\
    COMPROBAR_FUNC_ACEPTAR(func_aceptar, tipo),\
    COMPROBAR_FUNC_CADENA(func_titulo, tipo),\
    COMPROBAR_FUNC_CADENA(func_dato, tipo),\
	componente_no_usar_crea_defecto(\
						#tipo,\
						(struct componente_datos_item_t **)datos,\
					    (componente_FPtr_crear_implementacion)func_crea_implementacion,\
						(componente_FPtr_destruye)func_destruye,\
                        (componente_FPtr_se_puede_aceptar)func_se_puede_aceptar,\
                        (componente_FPtr_aceptar)func_aceptar,\
                        (componente_FPtr_cadena)func_titulo,\
                        (componente_FPtr_cadena)func_dato)\
	)\
	
void componente_destruye(struct componente_t **componente);

struct item_implementacion_t *componente_generar_implementacion(struct marcogrf_t *marco, struct componente_t *componente, struct item_implementacion_dialogo_t *dialogo_so);

struct componente_datos_item_t *componente_no_usar_datos(struct componente_t *componente, const char *tipo);
#define componente_datos(tipo, componente) (struct tipo *)componente_no_usar_datos(componente, #tipo)

bool componente_se_puede_finalizar(const struct marcogrf_t *marco, const struct componente_t *componente, char **mensaje_error);

void componente_aceptar(const struct marcogrf_t *marco, const struct componente_t *componente);

ArrString *componente_titulo(const struct componente_t *componente);

ArrString *componente_dato(const struct componente_t *componente);

void componente_set_dialogo_padre(struct componente_t *componente, struct dialogo_t *dialogo);

struct dialogo_t *componente_get_dialogo_padre(struct componente_t *componente);

bool componente_isA(const struct componente_t *componente, const char *type, struct componente_datos_item_t **datos);

void componente_setTime(
                    struct componente_t *componente,
                    const struct marcogrf_t *marco, 
                    unsigned long timeout_miliseconds, 
                    struct callback_t **callback);
