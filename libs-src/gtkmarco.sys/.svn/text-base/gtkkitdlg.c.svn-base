// Utilizadades para lanzar un di�logo

#include "gtkkitdlg.inl"

#include "asrtbas.h"

#define i_TAMANYO_BORDE 5

//---------------------------------------------------------

static GtkWidget *i_zona_menus(
                    struct item_implementacion_menubar_t **menubar,
                    struct item_implementacion_toolbar_t **toolbar)
{
    GtkWidget *gtk_toolbar, *gtk_menubar, *gtk_menu_and_toolbar;
    int espacio_entre_ellos;
    
    assert_no_null(menubar);
    assert_no_null(toolbar);
    
    gtk_menubar = (GtkWidget *)*menubar;
    gtk_toolbar = (GtkWidget *)*toolbar;
    
    espacio_entre_ellos = 1;
    gtk_menu_and_toolbar = gtk_vbox_new(FALSE, espacio_entre_ellos);
    gtk_box_pack_start(GTK_BOX(gtk_menu_and_toolbar), gtk_menubar, FALSE, TRUE, 0);
    
    if (gtk_toolbar != NULL)
        gtk_box_pack_start(GTK_BOX(gtk_menu_and_toolbar), gtk_toolbar, FALSE, TRUE, 0);
    
    *menubar = NULL;
    *toolbar = NULL; 
    
    return gtk_menu_and_toolbar;
}

//---------------------------------------------------------

static GtkWidget *i_area_aplicacion(
                    struct item_implementacion_menubar_t **menubar,
                    struct item_implementacion_toolbar_t **toolbar, 
                    struct item_implementacion_t **area_aplicacion)
{
    GtkWidget *gtk_aplicacion, *gtk_menubar, *gtk_area;
    int espacio_entre_ellos;
    
    assert_no_null(menubar);
    assert_no_null(area_aplicacion);
    
    gtk_menubar = i_zona_menus(menubar, toolbar);
    gtk_area = (GtkWidget *)*area_aplicacion;
    assert_no_null(gtk_area);
    
    espacio_entre_ellos = 3;
    gtk_aplicacion = gtk_vbox_new(FALSE, espacio_entre_ellos);
 
    if (gtk_menubar != NULL)
    {
        gtk_widget_show(gtk_menubar);
        gtk_box_pack_start(GTK_BOX(gtk_aplicacion), gtk_menubar, FALSE, TRUE, 0);
    }
    
    gtk_widget_show(gtk_area);
    gtk_box_pack_start(GTK_BOX(gtk_aplicacion), gtk_area, TRUE, TRUE, 0);

    gtk_widget_show(gtk_aplicacion);

    *menubar = NULL; 
    *area_aplicacion = NULL; 
    
    return gtk_aplicacion;
}

//---------------------------------------------------------

void gtkkitdlg_cierra_dialogo(GtkWidget *dialogo)
{
    assert_no_null(dialogo);
    
    gtk_widget_hide(dialogo);
    gtk_widget_destroy(dialogo);
    
    gtk_main_quit();
}

//---------------------------------------------------------

static void i_cierra_dialogo(GtkWidget *dialogo, gpointer data)
{
    gtk_widget_hide(dialogo);
    gtk_main_quit();
}

//---------------------------------------------------------

void gtkkitdlg_lanza_dialogo(GtkWidget *dialogo)
{
    assert_no_null(dialogo);
    
    gtk_container_border_width(GTK_CONTAINER(dialogo), i_TAMANYO_BORDE);
    
    gtk_window_set_modal(GTK_WINDOW(dialogo), TRUE);
    
    gtk_signal_connect(GTK_OBJECT(dialogo), "delete_event", GTK_SIGNAL_FUNC(i_cierra_dialogo), NULL);
    
    gtk_widget_show(dialogo);
    gtk_main();
}

//---------------------------------------------------------

GtkWidget *gtkkitdlg_crea_aplicacion(void)
{
    GtkWidget *widget;
    
	widget = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(widget), 500, 400);
    
    return widget; 
}

//---------------------------------------------------------

void gtkkitdlg_lanza_aplicacion(
                    GtkWidget *aplicacion,
                    const char *titulo,
                    struct item_implementacion_menubar_t **menubar, 
                    struct item_implementacion_toolbar_t **toolbar,
                    struct item_implementacion_t **area_aplicacion)
{
    GtkWidget *gtk_area_aplicacion;

    gtk_area_aplicacion = i_area_aplicacion(menubar, toolbar, area_aplicacion);  
    
	gtk_container_border_width(GTK_CONTAINER(aplicacion), i_TAMANYO_BORDE);
	gtk_window_set_title(GTK_WINDOW(aplicacion), titulo);
	gtk_container_add(GTK_CONTAINER(aplicacion), gtk_area_aplicacion);
    
    gtk_signal_connect(GTK_OBJECT(aplicacion), "delete_event", GTK_SIGNAL_FUNC(i_cierra_dialogo), NULL);
	gtk_widget_show(aplicacion);
    
    gtk_main();
}

