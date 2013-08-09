// Di�logo para gtk

#include "gtkdialogo.inl"

#include "gtkbtnacp.inl"
#include "gtkkitdlg.inl"
#include "gtkmensaje.inl"
#include "gtkinc.inl"

#include "callback.syh"
#include "evtkey.h"
#include "asrtbas.h"
#include "strbas.h"
#include "memory.h"

#include <gtk/gtk.h>

//---------------------------------------------------------

struct item_implementacion_dialogo_t *gtkdialogo_crea_dialogo(struct item_implementacion_dialogo_t *dlg_padre)
{
    GtkWidget *dialogo;
    
    dialogo = gtk_dialog_new();
    
    if (dlg_padre != NULL)
        gtk_widget_set_parent(dialogo, (GtkWidget *)dlg_padre);
    
    return (struct item_implementacion_dialogo_t *)dialogo;
}

//---------------------------------------------------------

static bool i_isPressedModifier(GdkEventKey *event, GdkModifierType type)
{
    if (event->state & type)
        return true;
    else
        return false;
}

//---------------------------------------------------------

static void i_pulsa_tecla(GtkWidget *widget, GdkEventKey *event, gpointer data)
{
	struct callback_t *funcion_eventos_teclado;
    struct EvtKey_t *evtKey;
    bool shiftPress, controlPress;
        
    assert_no_null(event);
    
    funcion_eventos_teclado = (struct callback_t *)data;
    
    shiftPress = i_isPressedModifier(event, GDK_SHIFT_MASK);
    controlPress = i_isPressedModifier(event, GDK_CONTROL_MASK);
    
    evtKey = evtkey_create(event->keyval, shiftPress, controlPress);
    
    callback_ejecuta_con_dato_externo(funcion_eventos_teclado, struct EvtKey_t, evtKey);
        
    evtkey_destroy(&evtKey);    
}

//---------------------------------------------------------

static void i_registraGestionTeclaSiProcede(
							GtkWidget *gtk_dialogo, 
							struct callback_t *funcion_eventos_teclado) 
{
	if (funcion_eventos_teclado != NULL)
	{
		gtk_widget_add_events(gtk_dialogo, GDK_KEY_PRESS_MASK);
		gtk_signal_connect(GTK_OBJECT(gtk_dialogo), "key_press_event", GTK_SIGNAL_FUNC(i_pulsa_tecla), (gpointer)funcion_eventos_teclado);
	}
}

//---------------------------------------------------------

void gtkdialogo_lanza_dialogo(
                        struct item_aplicacion_t *item_aplicacion,
                        struct item_implementacion_dialogo_t **implementacion_dialogo,
                        const char *titulo,
                        struct item_implementacion_t **area_trabajo,
                        struct dialogo_t *dialogo,
                        struct callback_t *funcion_eventos_teclado,
                        bool con_boton_aceptar,
                        bool *aceptado,
                        marcogrf_FPtr_se_puede_aceptar func_se_puede_aceptar, 
                        marcogrf_FPtr_aceptar func_aceptar)
{
    bool respuesta_aceptado;
    GtkWidget *gtk_dialogo;
    GtkWidget *gtk_aceptar, *gtk_cancelar, **gtk_area_trabajo;
    struct gtkbtnacp_t *datos_dialogo;
        
    assert_no_null(area_trabajo);
    
    gtk_dialogo = (GtkWidget *)(*implementacion_dialogo);
    
    i_registraGestionTeclaSiProcede(gtk_dialogo, funcion_eventos_teclado); 
    
    datos_dialogo = gtkbtnacp_crea_datos_dialogo(dialogo, func_se_puede_aceptar, func_aceptar);
    
    gtk_area_trabajo = (GtkWidget **)area_trabajo;
    if (con_boton_aceptar == true)
        gtk_aceptar = gtkbtnacp_boton_aceptar("Aceptar", datos_dialogo);
    else
        gtk_aceptar = NULL;
    
    gtk_cancelar = gtkbtnacp_boton_cancelar("Cancelar", datos_dialogo);
    
    respuesta_aceptado = gtkbtnacp_lanza_dialogo_con_botones(gtk_dialogo, titulo, gtk_area_trabajo, &gtk_aceptar, &gtk_cancelar, &datos_dialogo);
    
    if (aceptado != NULL)
        *aceptado = respuesta_aceptado;
    
    *implementacion_dialogo = NULL;
    *area_trabajo = NULL;
}

//---------------------------------------------------------

bool gtkdialogo_lanzar_seleccion_fichero(const char *titulo_dialogo, char **fichero_seleccionado, bool abrirFichero)
{
    GtkWidget *dialogo_seleccion;
    GtkFileSelection *file_selection;
    struct gtkbtnacp_t *datos_dialogo;
    bool aceptado;
    
    assert_no_null(fichero_seleccionado);
    
    dialogo_seleccion = gtk_file_selection_new(titulo_dialogo);
    file_selection = GTK_FILE_SELECTION(dialogo_seleccion);
    aceptado = false;
    
    datos_dialogo = gtkbtnacp_crea_datos_dialogo_para_file(fichero_seleccionado);
    
    gtkbtnacp_click_aceptar(file_selection->ok_button, datos_dialogo);
    gtkbtnacp_click_cancelar(file_selection->cancel_button, datos_dialogo);
    
    if (*fichero_seleccionado != NULL)
        gtk_file_selection_set_filename(file_selection, *fichero_seleccionado);

    aceptado = gtkbtnacp_lanza_dialogo(dialogo_seleccion, &datos_dialogo);
    
    return aceptado;
}

//---------------------------------------------------------

void gtkdialogo_lanza_mensaje_aviso(
                                struct item_implementacion_dialogo_t *dlg_padre,
                                enum marcogrf_tipo_aviso_t tipo_aviso,
                                const char *mensaje, bool *aceptado)
{
    assert_no_null(aceptado);
    *aceptado = false;
    
    switch(tipo_aviso)
    {
        case MARCOGRF_PREGUNTA:
        
            gtkmensaje_lanza_pregunta((GtkWidget *)dlg_padre, mensaje, aceptado);
            break;
        
        case MARCOGRF_MENSAJE:
            
            gtkmensaje_lanza_mensaje_error((GtkWidget *)dlg_padre, mensaje);
            break;
        
        default_error();
    }
}

//---------------------------------------------------------

struct item_implementacion_dialogo_t *gtkdialogo_get_dialogo(struct item_aplicacion_t *datos_aplicacion)
{
    assert(datos_aplicacion == NULL);
    return (struct item_implementacion_dialogo_t *)gtkkitdlg_crea_aplicacion();
}

//---------------------------------------------------------

void gtkdialogo_lanza_aplicacion(
                                struct item_aplicacion_t *datos_aplicacion,
                                struct item_implementacion_dialogo_t **dialogo_aplicacion,
                                const char *titulo,
                                struct callback_t *funcion_eventos_teclado,
                                struct item_implementacion_menubar_t **menubar,
                                struct item_implementacion_toolbar_t **toolbar,
                                struct item_implementacion_t **area_aplicacion)
{
	GtkWidget *gtk_dialogo_aplicacion;
	
    assert(datos_aplicacion == NULL);
    
    gtk_dialogo_aplicacion = (GtkWidget *)(*dialogo_aplicacion); 
    
    i_registraGestionTeclaSiProcede(gtk_dialogo_aplicacion, funcion_eventos_teclado);
    
    gtkkitdlg_lanza_aplicacion(gtk_dialogo_aplicacion, titulo, menubar, toolbar, area_aplicacion);
    *dialogo_aplicacion = NULL;
}
