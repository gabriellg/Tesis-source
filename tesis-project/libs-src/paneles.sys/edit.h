// Definicion de edit

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef bool (*edit_FPtr_filtra)(struct i_item_datos_t *datos, const char *texto, char **mensaje_error);
#define EDIT_COMPROBAR_FPTR_FILTRA(funcion, tipo) ((bool (*)(struct tipo *, const char *, const char *))funcion == funcion)

typedef void (*edit_FPtr_destruye)(struct i_item_datos_t **datos);
#define EDIT_COMPROBAR_FPTR_DESTRUYE(funcion, tipo) ((void (*)(struct tipo **))funcion == funcion)

struct panel_t *edit_crea_numero(const char *titulo, double *valor, double inicio, double final, unsigned long num_decimales);
	
struct panel_t *edit_crea_texto(const char *titulo, char **texto);

struct panel_t *edit_crea_texto_multilinea(const char *titulo, char **texto, unsigned long ancho, unsigned long alto, bool editable);

void edit_nousar_anade_filtro(struct edit_t *edit, struct i_item_datos_t **datos, edit_FPtr_filtra func_filtra, edit_FPtr_destruye func_destruye);
#define edit_anade_filtro(edit, tipo, datos, func_filtra, func_destruye)\
(\
    (struct tipo **)tipo == tipo,\
    EDIT_COMPROBAR_FPTR_FILTRA(func_filtra, tipo),\
    EDIT_COMPROBAR_FPTR_DESTRUYE(func_destruye, tipo),\
    edit_nousar_anade_filtro(edit, (struct i_item_datos_t **)datos, (edit_FPtr_filtra)func_filtra,(edit_FPtr_destruye)func_destruye)\
)

const char *edit_get_texto(const struct edit_t *edit);

#ifdef __cplusplus
}
#endif /* __cplusplus */
