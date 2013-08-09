// Ejecutable que transforma los acentos en acentos de latex.

#include "asrtbas.h"
#include "arrcad.h"
#include "strbas.h"
#include "filetxt.hpp"
#include "nomfich.h"
#include "stdio.h"

struct i_acentos_t
{
	const char *acento;
	const char *cadenaSustituir;
};

static struct i_acentos_t i_ACENTOS[] = 
{
	{"á", "\\'a"},
	{"é", "\\'e"},
	{"í", "\\'i"},
	{"ó", "\\'o"},
	{"ú", "\\'u"},
	{"Á", "\\'A"},
	{"É", "\\'E"}, 	 	  	
    {"Í", "\\'I"},	  	
    {"Ó", "\\'O"},
    {"Ú", "\\'U"}, 	 	 
    {"ü", "\\\"u"},
	{"Ü"  "\\\"U"},
	{"ñ", "\\~n"},
	{"Ñ", "\\~N"}
};

//-----------------------------------------------------------------------

static char *i_sustituir_acentos(const char *linea)
{
	char *linea_aux;
	unsigned long numAcentos;
	
	linea_aux = strbas_copyString(linea);
	
	numAcentos = sizeof(i_ACENTOS) / sizeof(i_ACENTOS[0]);
	
	for (unsigned long i = 0; i < numAcentos; i++)
	{
		if (strbas_isSubstring(linea_aux, i_ACENTOS[i].acento) == true)
		{
			char *linea_sustituida;
		
			linea_sustituida = strbas_replaceString(
							linea_aux, 
							i_ACENTOS[i].acento, 
							i_ACENTOS[i].cadenaSustituir);
			strbas_destroy(&linea_aux);
		
			linea_aux = linea_sustituida;
		}
	}
	
	return linea_aux;
}

//-----------------------------------------------------------------------

static ArrString *i_cambiaAcentos(ArrString *lineasFichero)
{
	unsigned long numLineas;
	ArrString *lineasFicheroSalida;
	
	numLineas = arrcad_numElementos(lineasFichero);
	lineasFicheroSalida = arrcad_CreaString(numLineas);
	
	for (unsigned long i = 0; i < numLineas; i++)
	{
		const char *linea;
		
		linea = arrcad_Get(lineasFichero, i);
		
		if (strbas_length(linea) > 0)
		{
			char *lineaSustituida;
			
			lineaSustituida = i_sustituir_acentos(linea);
			arrcad_SetDestruyendoCadena(lineasFicheroSalida, i, &lineaSustituida);
		}
		else
			arrcad_Set(lineasFicheroSalida, i, linea);
	}
	
	return lineasFicheroSalida;
}

//-----------------------------------------------------------------------

int main(int argc, char* argv[])
{
	if (argc == 3)
	{
		const char *nomFichEntrada, *nomFichSalida;
		
		nomFichEntrada = argv[1]; 
		nomFichSalida = argv[2];
		
		if (nomfich_existe_fichero(nomFichEntrada) == true)
		{
			ArrString *lineasFichero, *lineasFicheroSalida;
			
			lineasFichero = filetxt_leer_fichero_texto(nomFichEntrada);
			
			lineasFicheroSalida = i_cambiaAcentos(lineasFichero);
			filetxt_escribe_fichero_texto(nomFichSalida, lineasFicheroSalida);
			
			arrcad_Destruye(&lineasFichero);
			arrcad_Destruye(&lineasFicheroSalida);
		}
		else
			printf("No existe el fichero %s\n", nomFichEntrada); 
	}
	else
	{
		printf("Uso: acentosLatex ficheroEntrada ficheroSalida\n"); 
	}
	
	return 0;
}
