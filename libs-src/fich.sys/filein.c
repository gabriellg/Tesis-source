// filebas.c
// Fichero para lectura y escritura binaria.

#include "filein.h"

#include "filebas.inl"

#include "asrtbas.h"
#include "strbas.h"
#include "limits.h"
#include "memory.h"
#include "string.h"
#include "array.h"

// ----------------------------------------------------------------------------
struct FileIn_t *filein_fopen(const char *fichName)
{
	FILE *f;

	assert_no_null(fichName);

	f = filebas_fopen(fichName, "rb");

	return (struct FileIn_t *)f;
}

// ----------------------------------------------------------------------------
struct FileIn_t *filein_fopen_txt(const char *fichName)
{
	FILE *f;

	assert_no_null(fichName);

	f = filebas_fopen(fichName, "rt");

	return (struct FileIn_t *)f;
}

// ----------------------------------------------------------------------------
void filein_fclose(struct FileIn_t **f)
{
	filebas_fclose((FILE **)f);
}

// ----------------------------------------------------------------------------

char *filein_lee_buffer(struct FileIn_t *f, char *buffer, unsigned long sizeBuffer)
{
	return fgets(buffer, sizeBuffer, (FILE *)f);
}

// ----------------------------------------------------------------------------

static FILE *i_transforma_FILE(struct FileIn_t *f)
{
	assert_no_null(f);
	return (FILE *)f;
}

// ----------------------------------------------------------------------------

char *filein_lee_cadena(struct FileIn_t *f)
{
	size_t leidos, tam;
	char *cadena;
	FILE *file;

	file = i_transforma_FILE(f);
		
	leidos = fread(&tam,sizeof(size_t),1,file);
	assert(leidos == 1);
	
	cadena = strbas_create(tam+1);

	leidos = fread(cadena, sizeof(char), tam, file);
	assert(leidos == tam);

	return cadena;
}

// ----------------------------------------------------------------------------

char *filein_lee_cadena_opcional(struct FileIn_t *f)
{
	if (filein_lee_bool(f) == true)
		return filein_lee_cadena(f);
	else
		return NULL;
}

// ----------------------------------------------------------------------------

unsigned char filein_lee_byte(struct FileIn_t *f)
{
	unsigned char car;
	size_t leidos;
	FILE *file;
	
	file = i_transforma_FILE(f);

	leidos = fread(&car,sizeof(unsigned char),1,file);
	assert(leidos == 1);

	return car;
}

// ----------------------------------------------------------------------------

unsigned long filein_lee_ushort(struct FileIn_t *f)
{
	unsigned long ushort;
	size_t leidos;
	FILE *file;
	
	file = i_transforma_FILE(f);

	leidos = fread(&ushort, sizeof(unsigned long), 1, file);
	assert(leidos == 1);

	return ushort;
}

// ----------------------------------------------------------------------------

short filein_lee_short(struct FileIn_t *f)
{
	short shrt;
	size_t leidos;
	FILE *file;

	file = i_transforma_FILE(f);
	
	leidos = fread(&shrt,sizeof(short),1,file);
	assert(leidos == 1);

	return shrt;
}

// ----------------------------------------------------------------------------

double filein_lee_double(struct FileIn_t *f)
{
	double d;
	size_t leidos;
	FILE *file;

	file = i_transforma_FILE(f);
	leidos = fread(&d,sizeof(double),1,file);
	assert(leidos == 1);

	return d;
}

// ----------------------------------------------------------------------------

unsigned long filein_lee_ulong(struct FileIn_t *f)
{
	unsigned long dato;
	size_t leidos;
	FILE *file;

	file = i_transforma_FILE(f);
	leidos = fread(&dato,sizeof(unsigned long),1,file);
	assert(leidos == 1);

	return dato;
}

// ----------------------------------------------------------------------------

bool filein_lee_bool(struct FileIn_t *f)
{
	bool dato;
	size_t leidos;
	FILE *file;

	file = i_transforma_FILE(f);
	leidos = fread(&dato,sizeof(bool),1,file);
	assert(leidos == 1);
	assert(dato == true || dato == false);
	
	return dato;
}

// ----------------------------------------------------------------------------

char filein_lee_char(struct FileIn_t *f)
{
	char car;
	size_t leidos;
	FILE *file;

	file = i_transforma_FILE(f);
	leidos = fread(&car,sizeof(char),1,file);
	assert(leidos == 1);

	return car;
}

// ----------------------------------------------------------------------------

unsigned char filein_lee_uchar(struct FileIn_t *f)
{
	unsigned char car;
	size_t leidos;
	FILE *file;

	file = i_transforma_FILE(f);
	leidos = fread(&car,sizeof(unsigned char),1,file);
	assert(leidos == 1);

	return car;
}

// ----------------------------------------------------------------------------

struct Array_t *filein_nousar_lee_array(struct FileIn_t *f, const char *tipo, filein_FPtr_lee_elemento func_lee_elemento)
{
    struct Array_t *elementos;
    size_t leidos;
    FILE *file;
    unsigned long i,  num_elementos;
    
    assert_no_null(func_lee_elemento);
    
    file = i_transforma_FILE(f);
    
    leidos = fread(&num_elementos, sizeof(unsigned long), 1, file);
    assert(leidos == 1);
    
    elementos = array_CreaPrivada(tipo, num_elementos);
    
    for (i = 0; i < num_elementos; i++)
    {
        void *elemento;
        
        elemento = func_lee_elemento(f);
        array_PonerPrivada(tipo, elementos, i, elemento);
    }
    
    return elementos;
}
