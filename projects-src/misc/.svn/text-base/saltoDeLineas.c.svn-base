#include "arrcad.h"
#include "filetxt.hpp"
#include "stdio.h"

//-----------------------------------------------------------------------
int main(int argc, char *argv[])
{
    if (argc == 3)
    {
        ArrString *lineas;
        const char *nombre_fichero_salida, *nombre_fichero_entrada;
        
        nombre_fichero_entrada = argv[1];
        nombre_fichero_salida = argv[2];
        
        lineas = filetxt_leer_fichero_texto(nombre_fichero_entrada);
        filetxt_escribe_fichero_texto(nombre_fichero_salida, lineas);
        arrcad_Destruye(&lineas);
    }
    else
        printf("saltoDeLineas nombre_fichero_entrada nombre_fichero_salida\n");
}
