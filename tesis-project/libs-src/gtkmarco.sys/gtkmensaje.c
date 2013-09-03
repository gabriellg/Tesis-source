// Dialogos para mensajes y preguntas

#include "gtkmensaje.inl"

#include "asrtbas.h"
#include "memory.h"
#include "gtkboton.inl"
#include "gtkkitdlg.inl"

#include <stdlib.h>

struct i_datos_panel_t
{
    GtkWidget *dialogo;
    bool *aceptado;
};

//---------------------------------------------------------

static struct i_datos_panel_t *i_crea_datos_panel(
                        GtkWidget *dialogo,
                        bool **aceptado)
{
    struct i_datos_panel_t *datos_panel;
        
    datos_panel = MALLOC(struct i_datos_panel_t);
        
    datos_panel->dialogo = dialogo;
    datos_panel->aceptado = ASSIGN_PP(aceptado, bool);
    
    return datos_panel;
}

//---------------------------------------------------------

static void i_destruye_datos_panel(struct i_datos_panel_t **datos_panel)
{
    FREE_T(datos_panel, struct i_datos_panel_t);
}

//---------------------------------------------------------

static void i_destruir_componente(GtkWidget *widget, gpointer *data)
{
    struct i_datos_panel_t *datos_panel;
    
    datos_panel = (struct i_datos_panel_t *)data;
    i_destruye_datos_panel(&datos_panel);
}

//---------------------------------------------------------

static void i_cierra(GtkWidget *widget, gpointer data)
{
    struct i_datos_panel_t *datos_panel;
        
    datos_panel = (struct i_datos_panel_t *)data;
    assert_no_null(datos_panel);
    gtkkitdlg_cierra_dialogo(GTK_WIDGET(datos_panel->dialogo));
}

//---------------------------------------------------------

static void i_acepta(GtkWidget *widget, gpointer data)
{
    struct i_datos_panel_t *datos_panel;
        
    datos_panel = (struct i_datos_panel_t *)data;
    assert_no_null(datos_panel);
    
    *(datos_panel->aceptado) = true;
    gtkkitdlg_cierra_dialogo(GTK_WIDGET(datos_panel->dialogo));
}

//---------------------------------------------------------

static GtkWidget *i_dialogo_mensaje(
                            const char *mensaje_error, 
                            const char *titulo,
                            const char *texto_aceptar,
                            const char *texto_cancelar,
                            bool *aceptado)
{
    struct i_datos_panel_t *datos_panel;
    GtkWidget *dialogo;
    GtkWidget *label_mensaje, *boton_cancelar, *boton_aceptar;
    
    dialogo = gtk_dialog_new();
    datos_panel = i_crea_datos_panel(dialogo, &aceptado);
    
    label_mensaje = gtk_label_new(mensaje_error);
    gtk_widget_show(label_mensaje);
    
    gtk_window_set_title(GTK_WINDOW(dialogo), titulo);    

    if (texto_aceptar != NULL)
    {
        boton_aceptar = gtkboton_crea_boton(texto_aceptar, GTK_SIGNAL_FUNC(i_acepta), datos_panel);
        gtk_widget_show(boton_aceptar);
    }
    else
        boton_aceptar = NULL;
 
    boton_cancelar = gtkboton_crea_boton(texto_cancelar, GTK_SIGNAL_FUNC(i_cierra), datos_panel);
    gtk_widget_show(boton_cancelar);
     
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialogo)->vbox), label_mensaje, TRUE, TRUE, 0);
    
    if (boton_aceptar != NULL)
        gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialogo)->action_area), boton_aceptar, TRUE, TRUE, 0);
    
    gtk_box_pack_end(GTK_BOX(GTK_DIALOG(dialogo)->action_area), boton_cancelar, TRUE, TRUE, 0);
    
    gtk_signal_connect(GTK_OBJECT(dialogo), "destroy", GTK_SIGNAL_FUNC(i_destruir_componente), (gpointer)datos_panel);

    return dialogo;
}

//---------------------------------------------------------

static void i_lanza_mensaje_error_y_aviso(GtkWidget *dlg_padre, const char *titulo, const char *mensaje_error)
{
    GtkWidget *dialogo;
    bool aceptado;
    
    dialogo = i_dialogo_mensaje(mensaje_error, titulo, NULL, "Terminar", &aceptado);
    
    if (dlg_padre != NULL)
        gtk_widget_set_parent(dialogo, dlg_padre);
    
    gtkkitdlg_lanza_dialogo(dialogo);
}

//---------------------------------------------------------

void gtkmensaje_lanza_mensaje_error(GtkWidget *dlg_padre, const char *mensaje_error)
{
    i_lanza_mensaje_error_y_aviso(NULL, "Error", mensaje_error);
}

//---------------------------------------------------------

void gtkmensaje_lanza_mensaje_aviso(GtkWidget *dlg_padre, const char *mensaje_error)
{
    i_lanza_mensaje_error_y_aviso(NULL, "Aviso", mensaje_error);
}

//---------------------------------------------------------

void gtkmensaje_lanza_pregunta(GtkWidget *dlg_padre, const char *mensaje, bool *aceptar)
{
    GtkWidget *dialogo;
    
    dialogo = i_dialogo_mensaje(mensaje, "Pregunta", "Si", "No", aceptar);
    
    if (dlg_padre != NULL)
        gtk_widget_set_parent(dialogo, dlg_padre);
    
    gtkkitdlg_lanza_dialogo(dialogo);
}
