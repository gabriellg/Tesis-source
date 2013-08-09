// Boton para gtk

#include "gtkedit.inl"

#include "gtkmensaje.inl"

#include "asrtbas.h"
#include "memory.h"
#include "strbas.h"
#include "gtkinc.inl"

struct i_edit_t
{
    marco_FPtr_comprobar func_filtra;
    struct edit_t *dato;
    char *filtro_tecla;
    bool es_multilinea;
};

//---------------------------------------------------------

static struct i_edit_t *i_crea_datos_edit(
                        marco_FPtr_comprobar func_filtra,
                        const char *filtro_tecla,
                        struct edit_t *dato,
                        bool es_multilinea)
{
    struct i_edit_t *edit_datos;
        
    edit_datos = MALLOC(struct i_edit_t);
    
    edit_datos->func_filtra = func_filtra;
    edit_datos->filtro_tecla = strbas_copyOptionalNull(filtro_tecla);
    edit_datos->dato = dato;
    edit_datos->es_multilinea = es_multilinea;
    
    return edit_datos;
}

//---------------------------------------------------------

static void i_destruye_datos_edit(struct i_edit_t **datos_edit)
{
    assert_no_null(datos_edit);
    assert_no_null(*datos_edit);

    if ((*datos_edit)->filtro_tecla != NULL)
        strbas_destroy(&(*datos_edit)->filtro_tecla);
    
    FREE_T(datos_edit, struct i_edit_t);
}

//---------------------------------------------------------

static gboolean i_cambia_foco(GtkWidget *widget, GdkEventFocus *event, gpointer user_data)
{
    static bool hay_mensaje = false;

    if (hay_mensaje == false)
    {
        struct i_edit_t *datos_edit;
        const char *texto_edit;
        bool aceptado;
        char *mensaje_error;
        
        datos_edit = (struct i_edit_t *)user_data;
        texto_edit = gtk_entry_get_text(GTK_ENTRY(widget));
        
        assert_no_null(datos_edit);
        assert_no_null(datos_edit->func_filtra);
        
        mensaje_error = NULL;
        aceptado = datos_edit->func_filtra(texto_edit, datos_edit->dato, &mensaje_error);
        
        if (aceptado == false)
        {
            hay_mensaje = true;
            gtkmensaje_lanza_mensaje_error(widget, mensaje_error);
            hay_mensaje = false;
            strbas_destroy(&mensaje_error);
            return FALSE;
        }
        else
            return TRUE;
    }
    else
        return TRUE;
}

//---------------------------------------------------------

static gint i_filtra_tecla(GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
    assert_no_null(event);
    assert_no_null(user_data);
    
    if (event->length > 0)
    {
        struct i_edit_t *datos_edit;
        
        datos_edit = (struct i_edit_t *)user_data;
        
        if (strbas_charIsInString(datos_edit->filtro_tecla, event->keyval) == false)
        {
            event->string[event->length - 1] = '\0';
            event->length--;
            
            return TRUE;
        }
    }
    
    return FALSE;
}

//---------------------------------------------------------

static void i_destruir_componente(GtkWidget *widget, gpointer *data)
{
    struct i_edit_t *datos_edit;
    
    datos_edit = (struct i_edit_t *)data;
    i_destruye_datos_edit(&datos_edit);
}

//---------------------------------------------------------

static GtkWidget *i_crea_edit_simple(const char *texto, struct i_edit_t *datos_edit)
{
	GtkWidget *entry;
    
    assert_no_null(datos_edit);
    
	entry = gtk_entry_new();
    
    gtk_entry_set_text(GTK_ENTRY(entry), texto);
    
    gtk_signal_connect(GTK_OBJECT(entry), "focus_out_event", GTK_SIGNAL_FUNC(i_cambia_foco), (gpointer)datos_edit);
    
    if (datos_edit->filtro_tecla != NULL)
        gtk_signal_connect(GTK_OBJECT(entry), "key_press_event", GTK_SIGNAL_FUNC(i_filtra_tecla), (gpointer)datos_edit);
    
    return entry;
}

//---------------------------------------------------------

struct item_implementacion_t *gtkedit_crea_implementacion(
                        struct item_implementacion_dialogo_t *dlg_padre, 
                        const char *texto, 
                        const char *filtro_teclas, 
                        struct edit_t *dato, 
                        bool es_multilinea, bool editable,
                        unsigned long ancho, unsigned long alto,
                        marco_FPtr_comprobar func_filtra)
{
    GtkWidget *control;
    struct i_edit_t *datos_edit;
    
    assert(es_multilinea == false);

    datos_edit = i_crea_datos_edit(func_filtra, filtro_teclas, dato, es_multilinea);

	control = i_crea_edit_simple(texto, datos_edit);

    gtk_object_set_data(GTK_OBJECT(control), "datos_edit", (gpointer)datos_edit);
    gtk_signal_connect(GTK_OBJECT(control), "destroy", GTK_SIGNAL_FUNC(i_destruir_componente), (gpointer)datos_edit);
	
	return (struct item_implementacion_t *)control;
}

//---------------------------------------------------------

char *gtkedit_get_texto(struct item_implementacion_t *edit)
{
    GtkWidget *gtk_edit;
    struct i_edit_t *datos_edit;
    const char *texto;
    
    gtk_edit = (GtkWidget *)edit;
    datos_edit = (struct i_edit_t *)gtk_object_get_data(GTK_OBJECT(gtk_edit), "datos_edit");
    
    if (datos_edit->es_multilinea == false)
        texto = gtk_entry_get_text(GTK_ENTRY(gtk_edit));
    else
        texto = gtk_editable_get_chars(GTK_EDITABLE(gtk_edit), 0, -1);
    
    return strbas_copyString(texto);
}
