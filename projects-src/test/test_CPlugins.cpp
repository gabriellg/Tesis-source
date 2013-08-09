/* -----------------------------------------------------------------------------
*  test_CPlugins
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include "CPlugins.hpp"
#include "CString.hpp"

#include "stdio.h"
#include "stdlib.h"

typedef void (*fnc_main)(void);

//------------------------------------------------------------------------------
//
int main(int argc, char** argv)
{
    class CPlugins *plugins;
    char *messageError;
    bool existSymbol;
    const char *nameLibrary = "/home/gabi/Proyectos-C/srvirtual/srvirtual/plugins-bin/deb/test-plugins.so";
    void *symbol;
    fnc_main gmv_main;

    plugins = new CPlugins();

    existSymbol = plugins->readFunction(
                        nameLibrary,
                        "gmv_main",
                        &symbol,
                        &messageError);

    gmv_main = (fnc_main)symbol;

    if (existSymbol == true)
        gmv_main();
    else
    {
        printf("Error: %s\n", messageError);
        CString::free(&messageError);
    }

    delete plugins;

    return EXIT_SUCCESS;
}
