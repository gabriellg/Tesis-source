//mrpanel.c

#include "mrpanel.syh"
#include "mrpanel.tli"

#include "memory.h"

//---------------------------------------------------------

struct mrpanel_t *mrpanel_create(
	    mrpanel_FPtr_crea_panel func_crea_panel,
	    mrpanel_FPtr_append_panel func_append_panel,
	    mrpanel_FPtr_append_componente func_append_componente,    
	    mrpanel_FPtr_remove_panel func_remove_panel,
	    mrpanel_FPtr_remove_componente func_remove_componente)
{
	struct mrpanel_t *mrpanel;
	
	mrpanel = MALLOC(struct mrpanel_t);

	mrpanel->func_crea_panel = func_crea_panel; 
	mrpanel->func_append_panel = func_append_panel; 
	mrpanel->func_append_componente = func_append_componente;     
	mrpanel->func_remove_panel = func_remove_panel; 
	mrpanel->func_remove_componente = func_remove_componente; 
	
	return mrpanel;
}

//---------------------------------------------------------

void mrpanel_destroy(struct mrpanel_t **mrpanel)
{
	FREE_T(mrpanel, struct mrpanel_t);
}
