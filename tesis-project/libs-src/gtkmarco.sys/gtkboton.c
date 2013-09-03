// Boton para gtk

#include "gtkboton.inl"

#include "gtkmensaje.inl"

#include "callback.syh"
#include "asrtbas.h"
#include "strbas.h"
#include "memory.h"

//---------------------------------------------------------

static void i_pulsar(GtkWidget *button, gpointer data)
{
	struct callback_t *callback;
		
	callback = (struct callback_t *)data;
	callback_ejecuta(callback);
}

//---------------------------------------------------------

struct item_implementacion_t *gtkboton_crea_implementacion(
                        struct item_implementacion_dialogo_t *dlg_padre,
						const char *texto,
						struct callback_t *callback)
{
	GtkWidget *button;

    assert_no_null(dlg_padre);
    assert_no_null(texto);
	assert_no_null(callback);
	
	button = gtk_button_new_with_label(texto);
	gtk_signal_connect(GTK_OBJECT(button), "clicked", GTK_SIGNAL_FUNC(i_pulsar), (gpointer)callback);
	
	return (struct item_implementacion_t *)button;
}

//---------------------------------------------------------

GtkWidget *gtkboton_crea_boton(const char *texto, GtkSignalFunc funcion, gpointer data)
{
	GtkWidget *button;

    assert_no_null(texto);
    
	button = gtk_button_new_with_label(texto);
	gtk_signal_connect(GTK_OBJECT(button), "clicked", funcion, data);
    
    GTK_WIDGET_SET_FLAGS(button, GTK_CAN_DEFAULT);
	
	return button;
}
