/* -----------------------------------------------------------------------------
*  memory (Layer of malloc.h)
*  Author: Gabriel López García (gabriellg AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include "memory.h"
#include "asrtbas.h"
#include <stdlib.h>
#include <string.h>

//------------------------------------------------------------------------------
//
void *memory_malloc(size_t size)
{
    void *pointer;

    assert(size > 0);

    pointer = malloc(size);

    assert_no_null(pointer);

    return pointer;
}

//------------------------------------------------------------------------------
//
void *memory_calloc(unsigned int numElements, size_t size)
{
    void *ptr;

    assert(size > 0);
    assert(numElements > 0);

    ptr = calloc(numElements,size);

    assert_no_null(ptr);

    return ptr;
}

//------------------------------------------------------------------------------
//
void memory_free(void **ptr, size_t size)
{
    assert_no_null(ptr);
    assert_no_null(*ptr);

    memset(*ptr, 0, size);

    free(*ptr);

    *ptr = NULL;
}

//------------------------------------------------------------------------------
//
void memory_realloc(void **ptr,unsigned int numElements,size_t size)
{
    assert_no_null(ptr);
    assert(numElements > 0);
    assert(size > 0);

    if (*ptr != NULL)
        *ptr = realloc(*ptr, numElements * size);
    else
        *ptr = calloc((size_t)numElements, size);

    assert_no_null(*ptr);
}

//------------------------------------------------------------------------------
//
void *memory_assign_double_pointer(void **ptr)
{
    void *rvalue;

    if (ptr != NULL)
    {
        rvalue = *ptr;
        *ptr = NULL;
    }
    else
        rvalue = NULL;

    return rvalue;
}

//------------------------------------------------------------------------------
//
void *memory_assign_double_pointer_no_null(void **ptr)
{
    void *rvalue;
	
    assert_no_null(ptr);
    
    rvalue = *ptr;
    *ptr = NULL;
	
    return rvalue;
}

//------------------------------------------------------------------------------
//
void memory_destroy_optional_pointer(void **p, memory_FPtr_destroy func_destroy)
{
    assert_no_null(p);
    assert_no_null(func_destroy);

    if (*p != NULL)
        func_destroy(p);
}

//------------------------------------------------------------------------------
//
void *memory_copy(const void *ptr, unsigned long num, size_t size)
{
    void *ptr_copy;

    assert_no_null(ptr);
    assert(size > 0);
    assert(num > 0);

    ptr_copy = calloc(num, size);
    assert_no_null(ptr_copy);
    memcpy(ptr_copy, ptr, num * size);

    return ptr_copy;
}

//-----------------------------------------------------------------------

void memory_integrity_pointer_to_destroy(void **ptr)
{
    assert_no_null(ptr);
    assert_no_null(*ptr);
}

//-----------------------------------------------------------------------

void memory_crush_pointer(void **ptr)
{
    assert_no_null(ptr);
    *ptr = NULL;
}
