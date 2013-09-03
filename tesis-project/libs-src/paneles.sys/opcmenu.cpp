// Define una opción del menú

#include "opcmenu.hpp"
#include "opcmenu.inl"

#include "rcpaccion.inl"

#include "callback.syh"
#include "marcogrf.syh"

#include "asrtbas.h"
#include "memory.h"

#include "CImg.hpp"
#include "CString.hpp"

enum i_tipo_opcion_t
{
    i_SEPARADOR,
    i_OPCION
};

struct OpcMenu_t
{
    enum i_tipo_opcion_t tipo_opcion;
    
	char *texto;
	class CImg *imageIcon_opc;
    struct callback_t *callback;
	struct rcpaccion_t *receptor_accion;
	
	struct dialogo_t *dialogo_padre;
};

//---------------------------------------------------------

static void i_integridad(const struct OpcMenu_t *opcion)
{
	assert_no_null(opcion);
}

//---------------------------------------------------------

static struct OpcMenu_t *i_crea(
                        enum i_tipo_opcion_t tipo_opcion,
						const char *texto,
						class CImg **imageIcon_opc,
                        struct callback_t **callback,
						struct rcpaccion_t **receptor_accion,
						struct dialogo_t **dialogo_padre)
{
	struct OpcMenu_t *opcion;
		
	opcion = MALLOC(struct OpcMenu_t);
        
    opcion->tipo_opcion = tipo_opcion;
	
    if (texto != NULL)
        opcion->texto = CString::copy(texto);
    else
        opcion->texto = NULL;

    if (imageIcon_opc != NULL)
        opcion->imageIcon_opc = ASSIGN_PP_NO_NULL(imageIcon_opc, class CImg);
    else
        opcion->imageIcon_opc = NULL;
    
    opcion->callback = ASSIGN_PP(callback, struct callback_t);
	opcion->receptor_accion = ASSIGN_PP(receptor_accion, struct rcpaccion_t);
	opcion->dialogo_padre = ASSIGN_PP(dialogo_padre, struct dialogo_t);
	
	i_integridad(opcion);

	return opcion;
}

//---------------------------------------------------------

struct OpcMenu_t *opcmenu_crea(const char *texto, struct rcpaccion_t **receptor_accion)
{
	struct callback_t *callback;
    enum i_tipo_opcion_t tipo_opcion;
    struct dialogo_t *dialogo_padre;
    
    assert_no_null(texto);
    assert_no_null(receptor_accion);
    assert_no_null(*receptor_accion);
	
    tipo_opcion = i_OPCION;
	callback = NULL;
	dialogo_padre = NULL;
	
    return i_crea(
				tipo_opcion, texto, 
				NULL, //imageIcon 
				&callback, receptor_accion, &dialogo_padre);
}

//---------------------------------------------------------

struct OpcMenu_t *opcmenu_crea_opcion_con_icono(const char *texto, class CImg **imageIcon, struct rcpaccion_t **receptor_accion)
{
    struct callback_t *callback;
    enum i_tipo_opcion_t tipo_opcion;
    struct dialogo_t *dialogo_padre;
    
    assert_no_null(texto);
    assert_no_null(imageIcon);
    assert_no_null(receptor_accion);
    assert_no_null(*receptor_accion);
    
    tipo_opcion = i_OPCION;
    callback = NULL;
    dialogo_padre = NULL;
    
    return i_crea(tipo_opcion, texto, imageIcon, &callback, receptor_accion, &dialogo_padre);
}

//---------------------------------------------------------

struct OpcMenu_t *opcmenu_crea_separador()
{
    enum i_tipo_opcion_t tipo_opcion;
    const char *texto;
    struct rcpaccion_t *receptor_accion;
	struct callback_t *callback;
    struct dialogo_t *dialogo_padre;
	
    tipo_opcion = i_SEPARADOR;
    texto = NULL;
    receptor_accion = NULL;
	callback = NULL;
	dialogo_padre = NULL;
	
    return i_crea(
				tipo_opcion, texto, 
				NULL, //imageIcon 
				&callback, &receptor_accion, &dialogo_padre);
}

//---------------------------------------------------------

void opcmenu_destruye(struct OpcMenu_t **opcion)
{
	assert_no_null(opcion);
	i_integridad(*opcion);

	if ((*opcion)->texto != NULL)
        CString::free(&(*opcion)->texto);
        
    if ((*opcion)->imageIcon_opc != NULL)
        delete (*opcion)->imageIcon_opc;
    
    if ((*opcion)->callback != NULL)
        callback_destruye(&(*opcion)->callback);
    
    if ((*opcion)->receptor_accion != NULL)
        rcpaccion_destruye(&(*opcion)->receptor_accion);
	
	FREE_T(opcion, struct OpcMenu_t);
}

//---------------------------------------------------------

static void i_envia_accion(struct OpcMenu_t *opcion)
{
    assert_no_null(opcion);
    rcpaccion_envia(opcion->receptor_accion, NULL, opcion->dialogo_padre);
}

//---------------------------------------------------------

void opcmenu_append_opcion_implementacion(
                        struct OpcMenu_t *opcion,
                        struct marcogrf_t *marco,
                        struct dialogo_t *dialogo_padre,
                        struct item_implementacion_dialogo_t *dialogo_so,
                        struct item_implementacion_menu_t *menu_so)
{
    i_integridad(opcion);
    
    assert(opcion->callback == NULL);
    assert(opcion->dialogo_padre == NULL);
    
    opcion->dialogo_padre = dialogo_padre;
    
    switch(opcion->tipo_opcion)
    {
        case i_OPCION:
            
            opcion->callback = callback_crea(struct OpcMenu_t, opcion, i_envia_accion, NULL);
            marcogrf_append_opcion(marco, dialogo_so, menu_so, opcion->texto, opcion->imageIcon_opc, opcion->callback);
            break;
            
        case i_SEPARADOR:
            
            marcogrf_append_separador(marco, dialogo_so, menu_so);
            break;
            
        default_error();
    }
}

//---------------------------------------------------------

bool opcmenu_en_toolbar(const struct OpcMenu_t *opcion)
{
    i_integridad(opcion);
    
    if (opcion->imageIcon_opc != NULL)
        return true;
    else
        return false;
}

//---------------------------------------------------------

void opcmenu_append_opcion_toolbar(
                        struct OpcMenu_t *opcion,
                        struct marcogrf_t *marco,
                        struct item_implementacion_dialogo_t *dialogo_so, 
                        struct item_implementacion_toolbar_t *toolbar_so,
                        bool *hay_opciones_toolbar)
{
    i_integridad(opcion);

    assert_no_null(opcion->dialogo_padre);
    
    switch(opcion->tipo_opcion)
    {
        case i_OPCION:
            
            assert_no_null(opcion->callback);
    
            opcion->callback = callback_crea(struct OpcMenu_t, opcion, i_envia_accion, NULL);
            if (opcion->imageIcon_opc != NULL)
            {
                marcogrf_append_toolbar_opcion(
                        marco, dialogo_so, toolbar_so, 
                        opcion->texto,
                        opcion->imageIcon_opc, 
                        opcion->callback);
                *hay_opciones_toolbar = true;
            }
            break;
            
        case i_SEPARADOR:
            
            assert(opcion->callback == NULL);
            
            marcogrf_append_toolbar_separator(marco, dialogo_so, toolbar_so);
            break;
            
        default_error();
    }
}
