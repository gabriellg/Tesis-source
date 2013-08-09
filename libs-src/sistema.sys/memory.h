/* -----------------------------------------------------------------------------
*  memory (Layer of malloc.h)
*  Author: Gabriel López García (gabriellg AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include "stddef.h"

typedef void (*memory_FPtr_destroy)(void **);

#define MALLOC(x) (x *)memory_malloc(sizeof(x))
#define CALLOC(n,x) (x *)memory_calloc(n,sizeof(x))
#define FREE_T(x, type) ((void)((type *) *x == *x), memory_free((void **)x, sizeof(x)))
#define REALLOC(ptr,n,x) ((void)(*ptr == (x)*ptr), memory_realloc((void **)ptr,n,sizeof(x)))
#define ASSIGN_PP(ptr, x) ((void)(*ptr == (x *)*ptr), (x *)memory_assign_double_pointer((void **)ptr))
#define ASSIGN_PP_NO_NULL(ptr, x) ((void)(*ptr == (x *)*ptr), (x *)memory_assign_double_pointer_no_null((void **)ptr))
#define DESTROY_OPTIONAL_POINTER(ptr, x, func_destroy) ((void)(ptr == (x **)ptr), memory_destroy_optional_pointer((void **)ptr, (memory_FPtr_destroy)func_destroy))
#define COPY_MEMORY(ptr, n, type) ((void)((const type *)ptr == ptr), (type *)(memory_copy(ptr, n, sizeof(type))))
#define DELETE_OBJECT(x, type) ((void)((type *) *x == *x), memory_integrity_pointer_to_destroy((void **)x), delete *x, memory_crush_pointer((void **)x))

#ifdef __cplusplus
extern "C" {
#endif

void *memory_malloc(size_t size);

void *memory_calloc(unsigned int numElements, size_t size);

void memory_free(void **ptr, size_t size);

void memory_realloc(void **ptr,unsigned int numElements, size_t size);

void *memory_assign_double_pointer(void **ptr);
    
void *memory_assign_double_pointer_no_null(void **ptr);

void memory_destroy_optional_pointer(void **p, memory_FPtr_destroy func_destroy);

void *memory_copy(const void *p, unsigned long n, size_t size);

void memory_integrity_pointer_to_destroy(void **ptr);

void memory_crush_pointer(void **ptr);

#ifdef __cplusplus
}
#endif
