// Descripci�n: Funciones para la lectura.

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef void *(*filein_FPtr_lee_elemento)(struct FileIn_t *file);
#define FILEIN_COMPROBAR_FPTR_LEE_ELEMENTO(tipo_elemento, funcion) (void)((struct tipo_elemento *(*)(struct FileIn_t *))funcion == funcion)
    
struct FileIn_t *filein_fopen(const char *fichName);

struct FileIn_t *filein_fopen_txt(const char *fichName);

void filein_fclose(struct FileIn_t **f);

char *filein_lee_buffer(struct FileIn_t *f, char *buffer, unsigned long sizeBuffer);

char *filein_lee_cadena(struct FileIn_t *f);

char *filein_lee_cadena_opcional(struct FileIn_t *f);

unsigned char filein_lee_byte(struct FileIn_t *f);

unsigned long filein_lee_ushort(struct FileIn_t *f);
    
#define filein_lee_enum(tipo_enum, f) ((tipo_enum)filein_lee_ushort(f))

short filein_lee_short(struct FileIn_t *f);

double filein_lee_double(struct FileIn_t *f);

unsigned long filein_lee_ulong(struct FileIn_t *f);

bool filein_lee_bool(struct FileIn_t *f);

char filein_lee_char(struct FileIn_t *f);

unsigned char filein_lee_uchar(struct FileIn_t *f);
    
struct Array_t *filein_nousar_lee_array(struct FileIn_t *f, const char *tipo, filein_FPtr_lee_elemento func_lee_elemento);

#define filein_lee_array(tipo_elemento, f, func_lee)\
    (\
        FILEIN_COMPROBAR_FPTR_LEE_ELEMENTO(tipo_elemento, func_lee),\
        (ArrTipo(tipo_elemento) *)filein_nousar_lee_array(f, #tipo_elemento, (filein_FPtr_lee_elemento)func_lee)\
    )


#ifdef __cplusplus
}
#endif /* __cplusplus */

