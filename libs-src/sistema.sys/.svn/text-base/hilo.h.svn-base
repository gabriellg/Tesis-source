// Definicion de hilos.

typedef void (*hilo_FPtr_func_ejecuta)(struct hilo_item_dato_t *dato);
#define HILO_COMPRUEBA_FUNC_EJECUTA(tipo, funcion) (void)((void (*)(struct tipo *))funcion == funcion)

typedef void (*hilo_FPtr_func_destruye_datos)(struct hilo_item_dato_t **dato);
#define HILO_COMPRUEBA_FUNC_DESTRUYE(tipo, funcion) (void)((void (*)(struct tipo **))funcion == funcion)
    
struct hilo_t *hilo_nousar_crea(hilo_FPtr_func_ejecuta func_ejecuta, struct hilo_item_dato_t *dato);

#define hilo_crea(func_ejecuta, tipo, dato)\
(\
    (void)((struct tipo *)dato == dato),\
    HILO_COMPRUEBA_FUNC_EJECUTA(tipo, func_ejecuta),\
    hilo_nousar_crea((hilo_FPtr_func_ejecuta)func_ejecuta, (struct hilo_item_dato_t *)dato)\
)

void hilo_destruye(struct hilo_t **hilo);

void hilo_cancela(const struct hilo_t *hilo);

void hilo_no_puede_cancelar(void);

void hilo_puede_cancelar(void);

#define hilo_assert(x)			hilo_no_usar_assert(x, #x, __FILE__, __LINE__)
#define hilo_assert_error()		hilo_assert(0)
#define hilo_assert_no_null(x)	hilo_assert(x != NULL)

void hilo_no_usar_assert(int expr, const char *txtExpr, const char *file, int line);
