// filebas.c
// Fichero para lectura y escritura binaria.

#include "filebas.inl"

#include "asrtbas.h"
#include "strbas.h"
#include "memory.h"
#include "array.h"

struct filebas_t
{
	char *nombreFich;
	FILE *file;
};

static ArrTipo(filebas_t) *s_ficherosAbiertos = NULL;

// ----------------------------------------------------------------------------

static void i_integridad(const struct filebas_t *filebas)
{
	assert_no_null(filebas);
	assert_no_null(filebas->nombreFich);
	assert_no_null(filebas->file);
}

// ----------------------------------------------------------------------------

static struct filebas_t *i_crea(const char *nombreFich, FILE *file)
{
	struct filebas_t *fich = MALLOC(struct filebas_t);

	fich->nombreFich = strbas_copyString(nombreFich);
	fich->file = file;

	i_integridad(fich);
		
	return fich;
}

// ----------------------------------------------------------------------------

static void i_destruye_filebas(struct filebas_t **filebas)
{
    assert_no_null(filebas);
    i_integridad(*filebas);
    
    strbas_destroy(&(*filebas)->nombreFich);
    
    FREE_T(filebas, struct filebas_t);
}

// ----------------------------------------------------------------------------

static bool i_existe_nombre(const struct filebas_t *fich, const char *fichName)
{
    assert_no_null(fich);
    
    if (strbas_equalsStrings(fich->nombreFich, fichName) == true)
        return true;
    else
        return false;
}

// ----------------------------------------------------------------------------

static bool i_existe_FILE(const struct filebas_t *fich, const FILE *file)
{
    assert_no_null(fich);
    
    if (fich->file == file)
        return true;
    else
        return false;
}

// ----------------------------------------------------------------------------

static void i_registra_fichero(
                            ArrTipo(filebas_t) *ficheros_abiertos,
							const char *nombreFich,
							FILE *file)
{
	struct filebas_t *fich;

	fich = i_crea(nombreFich, file);
	array_Anyadir(filebas_t, ficheros_abiertos, fich);
}

// ----------------------------------------------------------------------------

static void i_elimina_del_registro_fichero(
							ArrTipo(filebas_t) *ficheros_registrados,
							FILE *file)
{
    unsigned long pos;

    if (array_BuscarUnica(filebas_t, ficheros_registrados, FILE, file, i_existe_FILE, &pos) == true)
    {
        array_Borrar(filebas_t, ficheros_registrados, pos, i_destruye_filebas);
    }
	else
		asrtbas_internal_warning("El fichero ya estaba cerrado");
}

// ----------------------------------------------------------------------------

FILE *filebas_fopen(const char *fichName,const char *modo)
{
	FILE *file;

	assert_no_null(fichName);
	assert_no_null(modo);
    
    if (s_ficherosAbiertos == NULL)
        s_ficherosAbiertos = array_CreaPuntero(0, filebas_t);

    if (array_BuscarUnica(filebas_t, s_ficherosAbiertos, char, fichName, i_existe_nombre, NULL) == true)
		asrtbas_internal_warning("El fichero ya esta abierto");

	file = fopen(fichName, modo);

    i_registra_fichero(s_ficherosAbiertos, fichName, file);

	return file;
}

// ----------------------------------------------------------------------------

void filebas_fclose(FILE **file)
{
	int ret;
	
	assert_no_null(file);
	assert_no_null(*file);
	
    i_elimina_del_registro_fichero(s_ficherosAbiertos, *file);
    
    if (array_numElementos(filebas_t, s_ficherosAbiertos) == 0)
        array_Destruye(filebas_t, &s_ficherosAbiertos, i_destruye_filebas);
    
	ret = fclose(*file);
	assert(ret == 0);

	*file = NULL;
}
