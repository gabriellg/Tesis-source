// Lectura de ficheros de texto

#include "filetxt.hpp"

#include "filebas.inl"

#include "asrtbas.h"
#include "arrcad.h"
#include "stdio.h"

#include "CArrayString.hpp"
#include "CString.hpp"

// ----------------------------------------------------------------------------

static void prv_deleteEndLine(char *buffer)
{
	unsigned long sizeString;

	sizeString = CString::size(buffer);
	
	if (buffer[sizeString-1] == '\n')
	{
		buffer[sizeString-1] = '\0';
		sizeString--;
	}

	if (buffer[sizeString-1] == '\r')
		buffer[sizeString-1] = '\0';
}

// ----------------------------------------------------------------------------

ArrString *filetxt_leer_fichero_texto(const char *fichero_txt)
{
	FILE *fich;
	ArrString *lineas;
	char *ret, buffer[2048];
	
	fich = filebas_fopen(fichero_txt, "rt");
	lineas = arrcad_CreaString(0);

	do
	{
		ret = fgets(buffer, 2048, fich);
		prv_deleteEndLine(buffer);
		if (ret != NULL)
			arrcad_Anyadir(lineas, buffer);
	}
	while(ret != NULL);
	
	filebas_fclose(&fich);
	
	return lineas;
}

// ----------------------------------------------------------------------------

void filetxt_escribe_fichero_texto(const char *fichero_txt, const ArrString *lineas)
{
	FILE *fich;
	unsigned long i, num_lineas;
	
	fich = filebas_fopen(fichero_txt, "wt");
	num_lineas = arrcad_numElementos(lineas);

	for (i = 0; i < num_lineas; i++)
	{
		const char *cadena;
		int ret;
		
		cadena = arrcad_Get(lineas, i);
		ret = fputs(cadena, fich);
		ret = fputs("\n", fich);
		assert(ret != EOF);
	}
	
	filebas_fclose(&fich);
}

// ----------------------------------------------------------------------------

class CArrayString *filetxt_readFileTxt(const char *filenameTxt)
{
    FILE *file;
    class CArrayString *lines;
    char *ret, buffer[2048];

    file = filebas_fopen(filenameTxt, "rt");
    lines = new CArrayString();

    do
    {
        ret = fgets(buffer, 2048, file);

        prv_deleteEndLine(buffer);
        if (ret != NULL)
            lines->add(buffer);
    }
    while(ret != NULL);

    filebas_fclose(&file);

    return lines;
}

// ----------------------------------------------------------------------------

class CArrayString *filetxt_readFileTxtNumLines(const char *filenameTxt, unsigned long numLines)
{
    FILE *file;
    class CArrayString *lines;
    char *ret, buffer[2048];

    file = filebas_fopen(filenameTxt, "rt");
    lines = new CArrayString();

    for (unsigned long i = 0; i < numLines; i++)
    {
        ret = fgets(buffer, 2048, file);

        prv_deleteEndLine(buffer);

        if (ret != NULL)
            lines->add(buffer);
        else
            break;
    }

    filebas_fclose(&file);

    return lines;
}


// ----------------------------------------------------------------------------

void filetxt_writeFileTxt(const char *filename, const class CArrayString *lineas)
{
    FILE *fich;
    unsigned long num_lineas;

    assert_no_null(lineas);

    fich = filebas_fopen(filename, "wt");
    num_lineas = lineas->size();

    for (unsigned long i = 0; i < num_lineas; i++)
    {
        const char *cadena;
        int ret;

        cadena = lineas->get(i);
        ret = fputs(cadena, fich);
        ret = fputs("\n", fich);
        assert(ret != EOF);
    }

    filebas_fclose(&fich);
}
