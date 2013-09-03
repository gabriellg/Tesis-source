// Fichero para escritura binaria.

#include "fileout.h"

#include "filebas.inl"

#include "asrtbas.h"
#include "strbas.h"
#include "array.h"

// ----------------------------------------------------------------------------

struct FileOut_t *fileout_fopen(const char *fichName)
{
	FILE *file;

	file = filebas_fopen(fichName, "wb");

	return (struct FileOut_t *)file;
}

// ----------------------------------------------------------------------------

void fileout_fclose(struct FileOut_t **f)
{
	filebas_fclose((FILE **)f);
}

// ----------------------------------------------------------------------------

static FILE *i_transforma_FILE(struct FileOut_t *f)
{
	assert_no_null(f);
	return (FILE *)f;
}

// ----------------------------------------------------------------------------

void fileout_escr_bool(struct FileOut_t *f, bool b)
{
	size_t escrito;
	FILE *file;
	
	file = i_transforma_FILE(f);
	escrito = fwrite(&b,sizeof(bool),1,file);
	assert(escrito == 1);
	assert(b == false || b == true);
}

// ----------------------------------------------------------------------------

void fileout_escr_cadena(struct FileOut_t *f,const char *cadena)
{
	size_t escrito, tam_cadena;
	FILE *file;
	
	file = i_transforma_FILE(f);

	tam_cadena = strbas_length(cadena);

	escrito = fwrite(&tam_cadena, sizeof(size_t), 1, file);
	assert(escrito == 1);
	
	escrito = fwrite(cadena, sizeof(char), tam_cadena, file);
	assert(escrito == tam_cadena);
}

// ----------------------------------------------------------------------------

void fileout_escr_cadena_opcional(struct FileOut_t *f,const char *cadena)
{
	if(cadena != NULL)
    {
    	fileout_escr_bool(f, true);
    	fileout_escr_cadena(f, cadena);
    }
    else
    	fileout_escr_bool(f, false);
}

// ----------------------------------------------------------------------------

void fileout_escr_byte(struct FileOut_t *f,unsigned char car)
{
	size_t escrito;
	FILE *file;
	
	file = i_transforma_FILE(f);
	escrito = fwrite(&car,sizeof(unsigned char),1,file);
	assert(escrito == 1);
}

// ----------------------------------------------------------------------------
void fileout_escr_ushort(struct FileOut_t *f,unsigned long car)
{
	size_t escrito;
	FILE *file;
	
	file = i_transforma_FILE(f);
	escrito = fwrite(&car,sizeof(unsigned long),1,file);
	assert(escrito == 1);
}

// ----------------------------------------------------------------------------
void fileout_escr_short(struct FileOut_t *f,short car)
{
	size_t escrito;
	FILE *file;
	
	file = i_transforma_FILE(f);
	escrito = fwrite(&car,sizeof(short),1,file);
	assert(escrito == 1);
}

// ----------------------------------------------------------------------------
void fileout_escr_double(struct FileOut_t *f,double d)
{
	size_t escrito;
	FILE *file;
	
	file = i_transforma_FILE(f);
	escrito = fwrite(&d,sizeof(double),1,file);
	assert(escrito == 1);
}

// ----------------------------------------------------------------------------

void fileout_escr_uchar(struct FileOut_t *f,unsigned char d)
{
	size_t escrito;
	FILE *file;
	
	file = i_transforma_FILE(f);
	escrito = fwrite(&d,sizeof(unsigned char),1,file);
	assert(escrito == 1);
}

// ----------------------------------------------------------------------------

void fileout_escr_ulong(struct FileOut_t *f, unsigned long d)
{
	size_t escrito;
	FILE *file;
	
	file = i_transforma_FILE(f);
	escrito = fwrite(&d,sizeof(unsigned long),1,file);
	assert(escrito == 1);
}

// ----------------------------------------------------------------------------

void fileout_nousar_graba_array(
                        struct FileOut_t *f, 
                        const char *tipo, 
                        const struct Array_t *array, 
                        fileout_FPtr_graba_elemento func_graba_elemento)
{
    size_t escritos;
    FILE *file;
    unsigned long i,  num_elementos;
    
    assert_no_null(func_graba_elemento);
    
    file = i_transforma_FILE(f);
    
    num_elementos = array_numElementosPrivada(tipo, array);
    escritos = fwrite(&num_elementos, sizeof(unsigned long), 1, file);
    assert(escritos == 1);
    
    for (i = 0; i < num_elementos; i++)
    {
        void *elemento;
        
        elemento = array_ObtenerPrivada(tipo, array, i);
        func_graba_elemento(elemento, f);
    }
}
