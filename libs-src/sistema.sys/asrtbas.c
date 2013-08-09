/* -----------------------------------------------------------------------------
*  asrtbas.h (Layer of assert.h)
*  Author: Gabriel López García (gabriellg AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include "asrtbas.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

//------------------------------------------------------------------------------
//
static void prv_error_log( const char *message, const char *file, int line )
{
    const char *file_assert_msg;

    file_assert_msg = getenv("ASSERT_FILE");

    if (file_assert_msg != NULL)
    {
        FILE *f;

        f = fopen(file_assert_msg, "a");

        fprintf(f, "%s:%d:%s\n", file, line, message);

        fclose(f);
    }
}

//------------------------------------------------------------------------------
//
static void prv_print_message( const char* txtExp, const char* msg, 
                               const char* file, int line )
{
    if (getenv("TEST_ON") == NULL)
    {
        printf("\n# Assert\n");
        printf("# Expression: %s\n", txtExp);

        if( strlen( msg ) > 0 )
            printf("# Message: %s\n", msg);

        printf("# File: %s\n", file);
        printf("# Line: %d\n", line);
    }
	
    prv_error_log(txtExp, file, line);
    
    abort();
}

//------------------------------------------------------------------------------
//
void asrtbas_assert( int expr, const char* txtExpr, const char* msg,
                     const char* file, int line )
{
    if (expr == 0)
        prv_print_message( txtExpr, msg, file, line );
}

//------------------------------------------------------------------------------
//
void asrtbas_internal_warning( const char* message )
{
    printf("\nInternal Warning: %s\n", message);
    exit(EXIT_FAILURE);
}
