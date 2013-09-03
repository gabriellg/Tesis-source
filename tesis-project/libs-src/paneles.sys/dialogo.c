// Dialogos de aceptar y cancelar

#include "dialogo.h"

#include "rcpaccion.inl"
#include "rcpaccion.h"
#include "panel.inl"
#include "menubar.inl"

#include "callback.syh"
#include "marcogrf.syh"

#include "boton.h"
#include "asrtbas.h"
#include "memory.h"
#include "strbas.h"

enum i_tipo_dialogo_t
{
    i_DIALOGO_DATOS,
    i_DIALOGO_FRAME
};

struct dialogo_t 
{
    enum i_tipo_dialogo_t tipo_dialogo;
    char *titulo;
    
    struct item_implementacion_dialogo_t *dialogo_so;
    
	struct marcogrf_t *marco;
    
	struct panel_t *panel;
	
	struct callback_t *funcion_eventos_teclado;
};

//---------------------------------------------------------

static void i_integridad(const struct dialogo_t *dialogo)
{
	assert_no_null(dialogo);
	assert_no_null(dialogo->panel);
    assert_no_null(dialogo->titulo);
}

//---------------------------------------------------------

static struct dialogo_t *i_crea(
                enum i_tipo_dialogo_t tipo_dialogo,
                const char *titulo,
				struct marcogrf_t *marco,
                struct item_implementacion_dialogo_t **dialogo_so,
				struct panel_t **panel,
				struct callback_t **funcion_eventos_teclado)
{
	struct dialogo_t *dialogo;
	
	dialogo = MALLOC(struct dialogo_t);
	
    dialogo->tipo_dialogo = tipo_dialogo;
    
    dialogo->titulo = strbas_copyString(titulo);
    
	dialogo->marco = marco;
    
    dialogo->panel = ASSIGN_PP_NO_NULL(panel, struct panel_t);
    dialogo->funcion_eventos_teclado = ASSIGN_PP(funcion_eventos_teclado, struct callback_t);
    dialogo->dialogo_so = ASSIGN_PP(dialogo_so, struct item_implementacion_dialogo_t);
        
	i_integridad(dialogo);
		
	return dialogo;
}

//---------------------------------------------------------

static struct callback_t *i_crea_callback_teclas_si_es_necesario(struct panel_t *panel)
{
	assert_no_null(panel);
	if (panel_hay_receptores_tecla(panel) == true)
		return callback_crea_con_dato_externo(struct panel_t, panel, struct EvtKey_t, panel_sendEventKey, NULL);
	else
		return NULL;
}

//---------------------------------------------------------

struct dialogo_t *dialogo_crea_frame(
						struct marcogrf_t *marco,
						const char *titulo, 
						struct panel_t **panel)
{
    enum i_tipo_dialogo_t tipo_dialogo;
    struct item_implementacion_dialogo_t *dialogo_so;
    struct dialogo_t *dialogo;
    struct callback_t *funcion_eventos_teclado;
    
    assert_no_null(panel);
    
    tipo_dialogo = i_DIALOGO_FRAME;
    dialogo_so = NULL;
    funcion_eventos_teclado = i_crea_callback_teclas_si_es_necesario(*panel);
    
	dialogo = i_crea(tipo_dialogo, titulo, marco, &dialogo_so, panel, &funcion_eventos_teclado);
    
    panel_set_dialogo_padre(dialogo->panel, dialogo);
    
    return dialogo;
}

//---------------------------------------------------------

struct dialogo_t *dialogo_crea_dialogo(struct dialogo_t *dialogo_padre, const char *titulo, struct panel_t **panel)
{
    struct dialogo_t *dialogo;
    enum i_tipo_dialogo_t tipo_dialogo;
    struct item_implementacion_dialogo_t *dialogo_so;
    struct callback_t *funcion_eventos_teclado;
	
	i_integridad(dialogo_padre);
	assert_no_null(panel);
    
    dialogo_so = marcogrf_crea_dialogo(dialogo_padre->marco, dialogo_padre->dialogo_so);
    tipo_dialogo = i_DIALOGO_DATOS;
    
    funcion_eventos_teclado = i_crea_callback_teclas_si_es_necesario(*panel);
    
	dialogo = i_crea(tipo_dialogo, titulo, dialogo_padre->marco, &dialogo_so, panel, &funcion_eventos_teclado);
    
    panel_set_dialogo_padre(dialogo->panel, dialogo);
    
    return dialogo;
}

//---------------------------------------------------------

void dialogo_destruye(struct dialogo_t **dialogo)
{
	assert_no_null(dialogo);
	i_integridad(*dialogo);

	strbas_destroy(&(*dialogo)->titulo);
	panel_destruye(&(*dialogo)->panel);
	
	if ((*dialogo)->funcion_eventos_teclado != NULL)
		callback_destruye(&(*dialogo)->funcion_eventos_teclado);
	
	FREE_T(dialogo, struct dialogo_t);
}

//---------------------------------------------------------

static bool i_se_puede_aceptar(struct dialogo_t *dialogo, char **mensaje_error)
{
    assert_no_null(dialogo);
    return panel_se_puede_finalizar(dialogo->panel, dialogo->marco, mensaje_error);
}

//---------------------------------------------------------

void i_aceptar_dialogo(struct dialogo_t *dialogo)
{
    panel_aceptar(dialogo->panel, dialogo->marco);
}

//---------------------------------------------------------

static void i_lanzar_dialogo(
                        struct marcogrf_t *marco, 
                        struct item_implementacion_dialogo_t **datos_so,
                        enum i_tipo_dialogo_t tipo_dialogo,
                        const char *titulo,
                        struct panel_t *panel,
                        struct callback_t *funcion_eventos_teclado, 
                        struct dialogo_t *dialogo,
                        bool *aceptado,
                        bool con_boton_aceptar)
{
    struct item_implementacion_t *implementacion_panel;

    assert_no_null(datos_so);
    
    implementacion_panel = panel_implementacion(panel, marco, *datos_so);
    
    switch(tipo_dialogo)
    {
        case i_DIALOGO_DATOS:
        
            marcogrf_lanzar_dialogo_aceptar(
                        marco, datos_so, titulo,
                        &implementacion_panel,
                        dialogo, 
                        funcion_eventos_teclado,
                        con_boton_aceptar, aceptado,
                        i_se_puede_aceptar, i_aceptar_dialogo);
            break;
        
        case i_DIALOGO_FRAME:
        default_error();
    }
}


//---------------------------------------------------------

void dialogo_lanzar(struct dialogo_t *dialogo)
{
    bool *aceptado;
    
    i_integridad(dialogo);
    
    aceptado = NULL;

    i_lanzar_dialogo(
            dialogo->marco, &dialogo->dialogo_so,
            dialogo->tipo_dialogo,
            dialogo->titulo,
            dialogo->panel,
            dialogo->funcion_eventos_teclado,
            dialogo,
            aceptado, true);
}

//---------------------------------------------------------

void dialogo_lanzar_sin_aceptar(struct dialogo_t *dialogo)
{
    bool *aceptado;
    
    i_integridad(dialogo);
    
    aceptado = NULL;

    i_lanzar_dialogo(
            dialogo->marco, &dialogo->dialogo_so,
            dialogo->tipo_dialogo,
            dialogo->titulo,
            dialogo->panel,
            dialogo->funcion_eventos_teclado,
            dialogo,
            aceptado, false);
}

//---------------------------------------------------------

bool dialogo_lanzar_aceptado(struct dialogo_t *dialogo)
{
    bool aceptado;
    
    i_integridad(dialogo);
    
    aceptado = false;
    
    i_lanzar_dialogo(
            dialogo->marco, &dialogo->dialogo_so,
            dialogo->tipo_dialogo,
            dialogo->titulo,
            dialogo->panel,
            dialogo->funcion_eventos_teclado,
            dialogo,
            &aceptado, true);
    
    return aceptado;
}

//---------------------------------------------------------

bool dialogo_lanza_seleccion_fichero(
                        struct dialogo_t *dlg_padre, 
                        const char *titulo_dialogo, 
                        char **fichero_seleccionado){
    i_integridad(dlg_padre);
    return marcogrf_lanzar_seleccion_fichero(dlg_padre->marco, titulo_dialogo, fichero_seleccionado, true);
}

//---------------------------------------------------------

bool dialogo_lanza_seleccion_fichero_grabar(
                        struct dialogo_t *dlg_padre, 
                        const char *titulo_dialogo, 
                        char **fichero_seleccionado){
    i_integridad(dlg_padre);
    return marcogrf_lanzar_seleccion_fichero(dlg_padre->marco, titulo_dialogo, fichero_seleccionado, false);
}

//---------------------------------------------------------

void dialogo_lanza_frame(struct dialogo_t *dlg_frame, struct menubar_t **menubar)
{
    struct item_implementacion_menubar_t *implementacion_menubar;
    struct item_implementacion_t *implementacion_area;
    struct item_implementacion_dialogo_t *implementacion_dialogo_frame;
    struct item_implementacion_toolbar_t *implementacion_toolbar; 
                                    
    i_integridad(dlg_frame);

    implementacion_dialogo_frame = marcogrf_get_dialogo(dlg_frame->marco);
    
	assert(dlg_frame->tipo_dialogo == i_DIALOGO_FRAME);

    if (menubar != NULL && *menubar != NULL)
    {
        implementacion_menubar = menubar_crea_menu_bar(*menubar, dlg_frame->marco, dlg_frame, implementacion_dialogo_frame);
        implementacion_toolbar = menubar_crea_toolbar(*menubar, dlg_frame->marco, implementacion_dialogo_frame);
    }
    else
    {
        implementacion_menubar = NULL;
        implementacion_toolbar = NULL;
    }

    implementacion_area = panel_implementacion(dlg_frame->panel, dlg_frame->marco, implementacion_dialogo_frame);
    
    marcogrf_lanzar_aplicacion(
                            dlg_frame->marco,
                            &implementacion_dialogo_frame,
                            dlg_frame->titulo,
                            dlg_frame->funcion_eventos_teclado,
                            &implementacion_menubar,
                            &implementacion_toolbar,
                            &implementacion_area);
                                    
    if (menubar != NULL && *menubar != NULL)
        menubar_destruye(menubar);
}

//---------------------------------------------------------

void dialogo_cierra_frame(struct dialogo_t *dlg_frame)
{
    i_integridad(dlg_frame);
    assert(dlg_frame->tipo_dialogo == i_DIALOGO_FRAME);
    marcogrf_cierra_aplicacion(dlg_frame->marco);
}

//---------------------------------------------------------

bool dialogo_pregunta(struct dialogo_t *dlg_padre, const char *mensaje)
{
    bool aceptado;
    
    i_integridad(dlg_padre);
    marcogrf_lanza_aviso_pregunta(
                    dlg_padre->marco, 
                    dlg_padre->dialogo_so, 
                    MARCOGRF_PREGUNTA, 
                    mensaje, &aceptado);

    return aceptado;
}

//---------------------------------------------------------

void dialogo_aviso(struct dialogo_t *dlg_padre, const char *mensaje)
{
    bool aceptado;
    
    i_integridad(dlg_padre);
    marcogrf_lanza_aviso_pregunta(
                    dlg_padre->marco, 
                    dlg_padre->dialogo_so, 
                    MARCOGRF_MENSAJE, 
                    mensaje, &aceptado);
}
