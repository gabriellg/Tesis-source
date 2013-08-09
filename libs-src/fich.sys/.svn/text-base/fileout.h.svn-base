// Fichero para escritura binaria.

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef void *(*fileout_FPtr_graba_elemento)(void *elemento, struct FileOut_t *file);
#define FILEOUT_COMPROBAR_FPTR_GRABA_ELEMENTO(tipo_elemento, funcion) (void)((void (*)(const struct tipo_elemento *, struct FileOut_t *))funcion == funcion)

struct FileOut_t *fileout_fopen(const char *fichName);

void fileout_fclose(struct FileOut_t **f);

void fileout_escr_bool(struct FileOut_t *f, bool b);

void fileout_escr_cadena(struct FileOut_t *f,const char *cadena);

void fileout_escr_cadena_opcional(struct FileOut_t *f,const char *cadena);

void fileout_escr_byte(struct FileOut_t *f,unsigned char car);

void fileout_escr_ushort(struct FileOut_t *f,unsigned long car);
    
#define fileout_escr_enum(f, tipo_enum, car) ((void)((tipo_enum)car == car), fileout_escr_ushort(f, (unsigned long)car))

void fileout_escr_short(struct FileOut_t *f,short car);

void fileout_escr_double(struct FileOut_t *f,double d);

void fileout_escr_uchar(struct FileOut_t *f,unsigned char d);

void fileout_escr_ulong(struct FileOut_t *f, unsigned long d);
    
void fileout_nousar_graba_array(struct FileOut_t *f, const char *tipo, const struct Array_t *array, fileout_FPtr_graba_elemento func_graba_elemento);

#define fileout_graba_array(tipo_elemento, f, elementos, func_graba)\
    (\
        (void)((ArrTipo(tipo_elemento) *)elementos == elementos),\
        FILEOUT_COMPROBAR_FPTR_GRABA_ELEMENTO(tipo_elemento, func_graba),\
        fileout_nousar_graba_array(\
                    f, #tipo_elemento, (const struct Array_t *)elementos,\
                    (fileout_FPtr_graba_elemento)func_graba)\
    )

#ifdef __cplusplus
}
#endif /* __cplusplus */
