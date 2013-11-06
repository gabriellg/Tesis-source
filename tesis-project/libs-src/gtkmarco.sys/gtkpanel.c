// Gesti�n de paneles para gtk
#include "gtkpanel.inl"

#include "gtkinc.inl"

#include "asrtbas.h"
#include "array.h"
#include "limits.h"

#define i_MARGEN_ENTRE_ITEMS 3

//---------------------------------------------------------

static void i_append_hijos(GtkWidget *box, GtkWidget *gtk_hijo, bool expand, bool fill)
{
    gtk_widget_show(gtk_hijo);
    gtk_box_pack_start(GTK_BOX(box), gtk_hijo, expand, fill, i_MARGEN_ENTRE_ITEMS);
}

//---------------------------------------------------------

static void i_append_grid(GtkWidget *box, unsigned long f, unsigned long c, GtkWidget *gtk_hijo, bool expand, bool fill)
{
	GtkAttachOptions options;
	
	options = GTK_SHRINK;
	
	if (expand == true)
		options = (GtkAttachOptions)(options | GTK_EXPAND);

	if (fill == true)
		options = (GtkAttachOptions)(options | GTK_FILL);
	
    gtk_widget_show(gtk_hijo);
    gtk_table_attach(
                    GTK_TABLE(box), gtk_hijo, 
                    c, c + 1, f, f + 1, 
                    options, options,
                    i_MARGEN_ENTRE_ITEMS, i_MARGEN_ENTRE_ITEMS);
}

//---------------------------------------------------------

struct item_implementacion_t *gtkpanel_crea_panel(
                                    enum mrpanel_tipo_disposicion_t tipo_disposicion, 
                                    unsigned long num_filas, unsigned long num_columnas,
                                    bool homogeneus, unsigned long pixelsSpacing)
{
	GtkWidget *box;

	switch(tipo_disposicion)
	{
		case MRPANEL_HORIZONTAL:
			
            assert(num_filas == 1);
            assert(num_columnas > 0);
			box = gtk_hbox_new(homogeneus, pixelsSpacing);
			break;
		
		case MRPANEL_VERTICAL:
			
            assert(num_filas > 0);
            assert(num_columnas == 1);
			box = gtk_vbox_new(homogeneus, pixelsSpacing);
			break;
		
        case MRPANEL_GRID:
            
            assert(num_filas != USHRT_MAX);
            assert(num_columnas != USHRT_MAX);
            box = gtk_table_new(num_filas, num_columnas, pixelsSpacing);
            break;
        
		default_error();
	}
	
    gtk_widget_show(box);
    
	return (struct item_implementacion_t *)box;
}

//---------------------------------------------------------

static void i_append_widget(
                    GtkWidget *gtk_panel,  
                    enum mrpanel_tipo_disposicion_t tipo_disposicion, 
                    unsigned long ind_fila, unsigned long ind_columna,
                    bool expand, bool fill,
                    GtkWidget *gtk_hijo)
{
	switch(tipo_disposicion)
	{
		case MRPANEL_HORIZONTAL:
		case MRPANEL_VERTICAL:
        
            i_append_hijos(gtk_panel, gtk_hijo, expand, fill);
			break;
        
        case MRPANEL_GRID:
            
            assert(ind_fila != USHRT_MAX);
            assert(ind_columna != USHRT_MAX);
            i_append_grid(gtk_panel, ind_fila, ind_columna, gtk_hijo, expand, fill);
            break;
        
		default_error();
	}
}

//---------------------------------------------------------

void gtkpanel_append_panel(
                    struct item_implementacion_t *panel,  
                    enum mrpanel_tipo_disposicion_t tipo_disposicion, 
                    unsigned long ind_fila, unsigned long ind_columna,
                    bool expand, bool fill,
                    struct item_implementacion_t *panel_hijo)
{
    GtkWidget *gtk_panel, *gtk_hijo;
    
    gtk_panel = (GtkWidget *)panel; 
    gtk_hijo = (GtkWidget *)panel_hijo;
    
    i_append_widget(gtk_panel, tipo_disposicion, ind_fila, ind_columna, expand, fill, gtk_hijo);
}
                    
//---------------------------------------------------------

void gtkpanel_mostrar(struct item_implementacion_t *panel)
{
	GtkWidget *gtk_panel;
	
	gtk_panel = (GtkWidget *)panel;
	gtk_widget_show(gtk_panel);
}

//---------------------------------------------------------

void gtkpanel_remove_panel(struct item_implementacion_t *panel, struct item_implementacion_t *panel_a_borrar)
{
    GtkWidget *gtk_panel, *gtk_panel_a_borrar;
    
    assert_no_null(panel);
    assert_no_null(panel_a_borrar);
    
    gtk_panel = (GtkWidget *)panel;
    gtk_panel_a_borrar = (GtkWidget *)panel_a_borrar;
    
    gtk_container_remove(GTK_CONTAINER(gtk_panel), gtk_panel_a_borrar);
}
