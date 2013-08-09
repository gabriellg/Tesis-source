// Array de cadenas.

typedef bool (*arrcad_FPtr_es_cadena)(const char *cad1, const char *cad2);

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

ArrString *arrcad_CreaString(unsigned long numElementos);

void arrcad_Destruye(ArrString **);

ArrString *arrcad_Copia(const ArrString *arr);

const char *arrcad_Get(const ArrString *arr, unsigned long pos);

char *arrcad_GetCopia(const ArrString *arr, unsigned long pos);

void arrcad_Set(ArrString *arr, unsigned long pos, const char *cad);

void arrcad_SetDestruyendoCadena(ArrString *arr, unsigned long pos, char **cad);

void arrcad_Anyadir(ArrString *arr, const char *cad);

void arrcad_AnyadirDestruyendoCadena(ArrString *arr, char **cad);

void arrcad_Borrar(ArrString *arr, unsigned long pos);
				  
char *arrcad_popCadena(ArrString *arr);

const char *arrcad_ultimaCadena(const ArrString *arr);

bool arrcad_Buscar(const ArrString *arr, const char *cad, arrcad_FPtr_es_cadena func_buscar, unsigned long *pos);

unsigned long arrcad_numElementos(const ArrString *arr);

void arrcad_Concatena(ArrString *arr, const ArrString *array_a_anyadir);

char *arrcad_texto_separado_por_espacios(const ArrString *array);
    
char *arrcad_texto_separado_por_comas(const ArrString *arr);
    
ArrString *arrcad_separa_palabras_por_comas(char *cadena);

	
#ifdef __cplusplus
}
#endif /* __cplusplus */

