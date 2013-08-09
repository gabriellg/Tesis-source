// Boton para gtk

#include "gtkinc.inl"
#include "marcogrf.hxx"

struct item_implementacion_t *gtkboton_crea_implementacion(
                        struct item_implementacion_dialogo_t *dlg_padre, 
                        const char *texto, 
                        struct callback_t *callback);

GtkWidget *gtkboton_crea_boton(const char *texto, GtkSignalFunc funcion, gpointer data);
