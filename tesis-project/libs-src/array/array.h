// M�dulo:array.h
// Descripci�n: Definici�n de un contenedor de punteros.

typedef void (*FuncLiberaPtr)(void **);
typedef void * (*FuncCopiaPtr)(const void *);
typedef bool (*FuncBuscarPtr)(const void *,const void *);
typedef bool (*FuncCumplePtr)(const void *);

#define DECLARAR_FUNC_LIBERA(func, tipo) (void)((void (*)(struct tipo **))func == func)
#define DECLARAR_FUNC_LIBERA_POINTER(func, tipo) (void)((void (*)(tipo **))func == func)
#define DECLARAR_FUNC_LIBERA_CLASS(func, tipo) (void)((void (*)(class tipo **))func == func)

#define DECLARAR_FUNC_COPIA(func, tipo) (void)((struct tipo * (*)(const struct tipo *)) func == func)
#define DECLARAR_FUNC_COPIA_POINTER(func, tipo) (void)((tipo * (*)(const tipo *)) func == func)
#define DECLARAR_FUNC_COPIA_CLASS(func, tipo) (void)((tipo * (*)(const class tipo *)) func == func)

#define DECLARAR_FUNC_BUSCAR(func, tipo, tipoElem) (void)((bool (*)(const struct tipo *,const tipoElem *)) func == func)
#define DECLARAR_FUNC_BUSCAR_POINTER(func, tipo, tipoElem) (void)((bool (*)(const tipo *,const tipoElem *)) func == func)
#define DECLARAR_FUNC_BUSCAR_CLASS(func, tipo, tipoElem) (void)((bool (*)(const class tipo *,const tipoElem *)) func == func)

#define DECLARAR_FUNC_CUMPLE(func, tipo) (void)((bool (*)(const struct tipo *)) func == func)
#define DECLARAR_FUNC_CUMPLE_CLASS(func, tipo) (void)((bool (*)(const class tipo *)) func == func)

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct Array_t *array_CreaPrivada(const char *tipo, unsigned long numElementos);

void array_DestruyePrivada(const char *tipo, struct Array_t **arr,FuncLiberaPtr func);

struct Array_t *array_CopiaPrivada(const char *tipo, const struct Array_t *arr,FuncCopiaPtr func);

void *array_ObtenerPrivada(const char *tipo, const struct Array_t *arr,unsigned long pos);

void *array_UltimoPrivada(const char *tipo, const struct Array_t *arr);

void array_PonerPrivada(const char *tipo, struct Array_t *arr,unsigned long pos,void *ptr);

void array_AnyadirPrivada(const char *tipo, struct Array_t *arr,void *ptr);

void array_AumentarPrivada(const char *tipo, struct Array_t *arr, unsigned long numNuevosElementos);

void array_InsertarPrivada(const char *tipo, struct Array_t *arr, unsigned long pos, void *ptr);

void array_BorrarPrivada(const char *tipo, struct Array_t *arr,unsigned long pos, FuncLiberaPtr func);

bool array_BuscarPrivada(const char *tipo, const struct Array_t *arr,const void *elem, FuncBuscarPtr func, unsigned long *pos);

bool array_BuscarUnicaPrivada(const char *tipo, struct Array_t *arr,const void *elem, FuncBuscarPtr func, unsigned long *pos);

bool array_cumplePrivada(const char *tipo, struct Array_t *arr, FuncCumplePtr func, unsigned long *ind);

unsigned long array_numElementosPrivada(const char *tipo, const struct Array_t *arr);

void array_ConcatenarPrivada(const char *tipo, struct Array_t *arr, const struct Array_t *array_a_anyadir, FuncCopiaPtr func);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#define array_CreaPuntero(n, tipo) ((ArrTipo(tipo) *)array_CreaPrivada(#tipo, n))
#define array_CreaPunteroConst(n, tipo) ((const ArrTipo(tipo) *)array_CreaPrivada(#tipo, n))
#define array_CreaArrayPuntero(n, tipo) ((ArrPointer(tipo) *)array_CreaPrivada(#tipo, n))
#define array_CreaClassPuntero(n, tipo) ((ArrClass(tipo) *)array_CreaPrivada(#tipo, n))

#define array_Destruye(tipo, arr, func) (DECLARAR_FUNC_LIBERA(func, tipo), (void)((ArrTipo(tipo) **)arr == arr), array_DestruyePrivada(#tipo, (struct Array_t **)arr,(FuncLiberaPtr)func))
#define array_DestruyeConst(tipo, arr) ((void)((const ArrTipo(tipo) **)arr == arr), array_DestruyePrivada(#tipo, (struct Array_t **)arr,(FuncLiberaPtr)NULL))
#define array_DestruyePointer(tipo, arr, func) (DECLARAR_FUNC_LIBERA_POINTER(func, tipo), (void)((ArrPointer(tipo) **)arr == arr), array_DestruyePrivada(#tipo, (struct Array_t **)arr,(FuncLiberaPtr)func))
#define array_DestruyeClass(tipo, arr, func) (DECLARAR_FUNC_LIBERA_CLASS(func, tipo), (void)((ArrClass(tipo) **)arr == arr), array_DestruyePrivada(#tipo, (struct Array_t **)arr,(FuncLiberaPtr)func))
	
#define array_Copia(tipo, arr, func) (DECLARAR_FUNC_COPIA(func, tipo), (void)((ArrTipo(tipo) *)arr == arr), (ArrTipo(tipo) *)array_CopiaPrivada(#tipo, (const struct Array_t *)arr,(FuncCopiaPtr)func))
#define array_CopiaPointer(tipo, arr, func) (DECLARAR_FUNC_COPIA_POINTER(func, tipo), (void)((ArrPointer(tipo) *)arr == arr), (ArrPointer(tipo) *)array_CopiaPrivada(#tipo, (const struct Array_t *)arr,(FuncCopiaPtr)func))
#define array_CopiaClass(tipo, arr, func) (DECLARAR_FUNC_COPIA_CLASS(func, tipo), (void)((ArrClass(tipo) *)arr == arr), (ArrClass(tipo) *)array_CopiaPrivada(#tipo, (const struct Array_t *)arr,(FuncCopiaPtr)func))

#define array_Get(tipo, arr, pos) ((void)((ArrTipo(tipo) *)arr == arr), (struct tipo *)array_ObtenerPrivada(#tipo, (const struct Array_t *)arr, pos))
#define array_GetConst(tipo, arr, pos) ((void)((const ArrTipo(tipo) *)arr == arr), (const struct tipo *)array_ObtenerPrivada(#tipo, (const struct Array_t *)arr, pos))
#define array_GetPointer(tipo, arr, pos) ((void)((const ArrPointer(tipo) *)arr == arr), (tipo *)array_ObtenerPrivada(#tipo, (const struct Array_t *)arr, pos))
#define array_GetClass(tipo, arr, pos) ((void)((ArrClass(tipo) *)arr == arr), (tipo *)array_ObtenerPrivada(#tipo, (const struct Array_t *)arr, pos))

#define array_GetLast(tipo, arr) ((void)((ArrTipo(tipo) *)arr == arr), (struct tipo *)array_UltimoPrivada(#tipo, (const struct Array_t *)arr))
#define array_GetLastConst(tipo, arr) ((void)((const ArrTipo(tipo) *)arr == arr), (const struct tipo *)array_UltimoPrivada(#tipo, (const struct Array_t *)arr))
#define array_GetLastPointer(tipo, arr, pos) ((void)((const ArrPointer(tipo) *)arr == arr), (tipo *)array_UltimoPrivada(#tipo, (const struct Array_t *)arr))
#define array_GetLastClass(tipo, arr, pos) ((void)((ArrClass(tipo) *)arr == arr), (tipo *)array_UltimoPrivada(#tipo, (const struct Array_t *)arr))

#define array_Set(tipo, arr, pos, obj) ((void)((struct tipo *)obj == obj), (void)((ArrTipo(tipo) *)arr == arr), array_PonerPrivada(#tipo, (struct Array_t *)arr, pos, obj))
#define array_SetConst(tipo, arr, pos, obj) ((void)((const struct tipo *)obj == obj), (void)((const ArrTipo(tipo) *)arr == arr), array_PonerPrivada(#tipo, (struct Array_t *)arr, pos, obj))
#define array_SetPointer(tipo, arr, pos, obj) ((void)((tipo *)obj == obj), (void)((ArrPointer(tipo) *)arr == arr), array_PonerPrivada(#tipo, (struct Array_t *)arr, pos, obj))
#define array_SetClass(tipo, arr, pos, obj) ((void)((tipo *)obj == obj), (void)((ArrClass(tipo) *)arr == arr), array_PonerPrivada(#tipo, (struct Array_t *)arr, pos, obj))

#define array_Anyadir(tipo, arr, obj) ((void)((struct tipo *)obj == obj), (void)((ArrTipo(tipo) *)arr == arr), array_AnyadirPrivada(#tipo, (struct Array_t *)arr, obj))
#define array_AnyadirConst(tipo, arr, obj) ((void)((const struct tipo *)obj == obj), (void)((const ArrTipo(tipo) *)arr == arr), array_AnyadirPrivada(#tipo, (struct Array_t *)arr, (void *)obj))
#define array_AnyadirPointer(tipo, arr, obj) ((void)((tipo *)obj == obj), (void)((ArrPointer(tipo) *)arr == arr), array_AnyadirPrivada(#tipo, (struct Array_t *)arr, obj))
#define array_AnyadirClass(tipo, arr, obj) ((void)((tipo *)obj == obj), (void)((ArrClass(tipo) *)arr == arr), array_AnyadirPrivada(#tipo, (struct Array_t *)arr, obj))

#define array_Aumentar(tipo, arr, numNuevosElementos) ((void)((ArrTipo(tipo) *)arr == arr),array_AumentarPrivada(#tipo, (struct Array_t *)arr, numNuevosElementos)) 
#define array_AumentarPointer(tipo, arr, numNuevosElementos) ((void)((ArrPointer(tipo) *)arr == arr),array_AumentarPrivada(#tipo, (struct Array_t *)arr, numNuevosElementos)) 
#define array_AumentarClass(tipo, arr, numNuevosElementos) ((void)((ArrClass(tipo) *)arr == arr),array_AumentarPrivada(#tipo, (struct Array_t *)arr, numNuevosElementos))

#define array_Insertar(tipo, arr, pos, obj) ((void)((struct tipo *)obj == obj),(void)((ArrTipo(tipo) *)arr == arr),array_InsertarPrivada(#tipo, (struct Array_t *)arr, pos, obj))
#define array_InsertarPointer(tipo, arr, pos, obj) ((void)((tipo *)obj == obj),(void)((ArrPointer(tipo) *)arr == arr),array_InsertarPrivada(#tipo, (struct Array_t *)arr, pos, obj))
#define array_InsertarClass(tipo, arr, pos, obj) ((void)((tipo *)obj == obj),(void)((ArrClass(tipo) *)arr == arr),array_InsertarPrivada(#tipo, (struct Array_t *)arr, pos, obj))

#define array_Borrar(tipo, arr, pos, func) (DECLARAR_FUNC_LIBERA(func, tipo), (void)((ArrTipo(tipo) *)arr == arr),array_BorrarPrivada(#tipo, (struct Array_t *)arr, pos, (FuncLiberaPtr)func))
#define array_BorrarPointer(tipo, arr, pos, func) (DECLARAR_FUNC_LIBERA_POINTER(func, tipo), (void)((ArrPointer(tipo) *)arr == arr),array_BorrarPrivada(#tipo, (struct Array_t *)arr, pos, (FuncLiberaPtr)func))
#define array_BorrarClass(tipo, arr, pos, func) (DECLARAR_FUNC_LIBERA_CLASS(func, tipo), (void)((ArrClass(tipo) *)arr == arr),array_BorrarPrivada(#tipo, (struct Array_t *)arr, pos, (FuncLiberaPtr)func))
				  
#define array_Buscar(tipo, arr, tipoElem, elem, func, pos) (DECLARAR_FUNC_BUSCAR(func, tipo, tipoElem), (void)((tipoElem *)elem == elem), (void)((const ArrTipo(tipo) *)arr == arr), array_BuscarPrivada(#tipo, (const struct Array_t *)arr, elem, (FuncBuscarPtr)func, pos))
#define array_BuscarPointer(tipo, arr, tipoElem, elem, func, pos) (DECLARAR_FUNC_BUSCAR_POINTER(func, tipo, tipoElem), (void)((tipoElem *)elem == elem), (void)((const ArrPointer(tipo) *)arr == arr), array_BuscarPrivada(#tipo, (const struct Array_t *)arr, elem, (FuncBuscarPtr)func, pos))
#define array_BuscarClass(tipo, arr, tipoElem, elem, func, pos) (DECLARAR_FUNC_BUSCAR_CLASS(func, tipo, tipoElem), (void)((tipoElem *)elem == elem), (void)((const ArrClass(tipo) *)arr == arr), array_BuscarPrivada(#tipo, (const struct Array_t *)arr, elem, (FuncBuscarPtr)func, pos))

#define array_BuscarUnica(tipo, arr, tipoElem, elem, func, pos) (DECLARAR_FUNC_BUSCAR(func, tipo, tipoElem), (void)((tipoElem *)elem == elem), (void)((ArrTipo(tipo) *)arr == arr), array_BuscarUnicaPrivada(#tipo, (struct Array_t *)arr, elem, (FuncBuscarPtr)func, pos))
#define array_BuscarUnicaPointer(tipo, arr, tipoElem, elem, func, pos) (DECLARAR_FUNC_BUSCAR_POINTER(func, tipo, tipoElem), (void)((tipoElem *)elem == elem), (void)((ArrPointer(tipo) *)arr == arr), array_BuscarUnicaPrivada(#tipo, (struct Array_t *)arr, elem, (FuncBuscarPtr)func, pos))
#define array_BuscarUnicaClass(tipo, arr, tipoElem, elem, func, pos) (DECLARAR_FUNC_BUSCAR_CLASS(func, tipo, tipoElem), (void)((tipoElem *)elem == elem), (void)((ArrClass(tipo) *)arr == arr), array_BuscarUnicaPrivada(#tipo, (struct Array_t *)arr, elem, (FuncBuscarPtr)func, pos))

#define array_Cumple(tipo, arr, func, pos) (DECLARAR_FUNC_CUMPLE(func, tipo), array_cumplePrivada(#tipo, (struct Array_t *)arr, (FuncCumplePtr)func, pos))
#define array_CumpleClass(tipo, arr, func, pos) (DECLARAR_FUNC_CUMPLE_CLASS(func, tipo), array_cumplePrivada(#tipo, (struct Array_t *)arr, (FuncCumplePtr)func, ind))

#define array_BuscarUnicoPuntero(tipo, arr, elem, pos) ((void)((struct tipo *)elem == elem), (void)((ArrTipo(tipo) *)arr == arr), array_BuscarUnicaPrivada(#tipo, (struct Array_t *)arr, elem, NULL, pos))
#define array_BuscarUnicoPunteroPointer(tipo, arr, elem, pos) ((void)((tipo *)elem == elem), (void)((ArrPointer(tipo) *)arr == arr), array_BuscarUnicaPrivada(#tipo, (struct Array_t *)arr, elem, NULL, pos))
#define array_BuscarUnicoPunteroClass(tipo, arr, elem, pos) ((void)((tipo *)elem == elem), (void)((ArrClass(tipo) *)arr == arr), array_BuscarUnicaPrivada(#tipo, (struct Array_t *)arr, elem, NULL, pos))

#define array_numElementos(tipo, arr) ((void)((const ArrTipo(tipo) *)arr == arr), array_numElementosPrivada(#tipo, (struct Array_t *)arr))
#define array_numElementosPointer(tipo, arr) ((void)((const ArrPointer(tipo) *)arr == arr), array_numElementosPrivada(#tipo, (struct Array_t *)arr))
#define array_numElementosClass(tipo, arr) ((void)((const ArrClass(tipo) *)arr == arr), array_numElementosPrivada(#tipo, (struct Array_t *)arr))

#define array_Concatena(tipo, arr, array_a_anyadir, func) ((void)((ArrTipo(tipo) *)arr == arr), (void)((const ArrTipo(tipo) *)array_a_anyadir == array_a_anyadir), DECLARAR_FUNC_COPIA(func, tipo),array_ConcatenarPrivada(#tipo, (struct Array_t *)arr, (const struct Array_t *)array_a_anyadir, (FuncCopiaPtr)func))
#define array_ConcatenaConst(tipo, arr, array_a_anyadir) ((void)((const ArrTipo(tipo) *)arr == arr), (void)((const ArrTipo(tipo) *)array_a_anyadir == array_a_anyadir), array_ConcatenarPrivada(#tipo, (struct Array_t *)arr, (const struct Array_t *)array_a_anyadir, NULL))
#define array_ConcatenaPointer(tipo, arr, array_a_anyadir, func) ((void)((const ArrPointer(tipo) *)arr == arr), (void)((const ArrTipo(tipo) *)array_a_anyadir == array_a_anyadir), DECLARAR_FUNC_COPIA(func, tipo),array_ConcatenarPrivada(#tipo, (struct Array_t *)arr, (const struct Array_t *)array_a_anyadir, (FuncCopiaPtr)func))
#define array_ConcatenaClass(tipo, arr, array_a_anyadir, func) ((void)((const ArrClass(tipo) *)arr == arr), (void)((const ArrClass(tipo) *)array_a_anyadir == array_a_anyadir), DECLARAR_FUNC_COPIA_CLASS(func, tipo),array_ConcatenarPrivada(#tipo, (struct Array_t *)arr, (const struct Array_t *)array_a_anyadir, (FuncCopiaPtr)func))
