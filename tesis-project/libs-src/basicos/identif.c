// Gestión de identificador

#include "identif.h"

#include "asrtbas.h"

//-----------------------------------------------------------------------

unsigned long identif_nuevo_identificador(unsigned long *id_nuevo)
{
	unsigned long id;
	
	assert_no_null(id_nuevo);
	
	id = *id_nuevo;
	(*id_nuevo)++;
	
	return id;
}

