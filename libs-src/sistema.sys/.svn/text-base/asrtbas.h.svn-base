/* -----------------------------------------------------------------------------
*  asrtbas.h (Layer of assert.h)
*  Author: Gabriel López García (gabriellg AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include "stddef.h"

#ifdef assert
    #undef assert
#endif

#define assert(x)                   asrtbas_assert(x, #x, "", __FILE__, __LINE__)
#define assert_with_message(x, msg) asrtbas_assert(x, #x, msg, __FILE__, __LINE__);

#define assert_no_null(x)           assert(x != NULL)

#define assert_error()              assert(0)
#define assert_message(msg)         asrtbas_assert(0, msg, "", __FILE__, __LINE__);

#define default_error()             default: assert(0)
#define default_message(msg)        default: assert_message(msg)
#define assert_return_error(type)   assert(0); return (type)0


#ifdef __cplusplus
extern "C" {
#endif

void asrtbas_assert( int expr, const char* txtExpr, const char* msg, const char* file, int line);
void asrtbas_internal_warning( const char* message );

#ifdef __cplusplus
}
#endif
