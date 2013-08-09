// Definici�n del marco para gtk.

#include "mrpanel.hxx"

struct item_implementacion_t *gtkpanel_crea_panel(
                    enum mrpanel_tipo_disposicion_t tipo_disposicion, 
                    unsigned long num_filas, unsigned long num_columnas,
                    bool homogeneus, unsigned long pixelsSpacing);

void gtkpanel_append_panel(
                    struct item_implementacion_t *panel,  
                    enum mrpanel_tipo_disposicion_t tipo_disposicion, 
                    unsigned long ind_fila, unsigned long ind_columna,
                    bool expand, bool fill,
                    struct item_implementacion_t *panel_hijo);
                    
void gtkpanel_mostrar(struct item_implementacion_t *panel);

void gtkpanel_remove_panel(
                    struct item_implementacion_t *panel, 
                    struct item_implementacion_t *panel_a_borrar);
