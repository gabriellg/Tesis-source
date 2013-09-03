// Implementacion para gtk de menu

#include "gtkmenu.inl"

#include "callback.syh"
#include "gtkinc.inl"
#include "gtkimage.inl"


//---------------------------------------------------------

struct item_implementacion_menubar_t *gtkmenu_crea_barra_menu(struct item_implementacion_dialogo_t *dlg_padre)
{
	GtkWidget *menu_bar;
	
	menu_bar = gtk_menu_bar_new();
	gtk_widget_show(menu_bar); 
    
	return (struct item_implementacion_menubar_t *)menu_bar;
}

//---------------------------------------------------------

struct item_implementacion_menu_t *gtkmenu_crea_menu(struct item_implementacion_dialogo_t *dlg_padre)
{
	return (struct item_implementacion_menu_t *)gtk_menu_new();
}

//---------------------------------------------------------

void gtkmenu_append_submenu(
                        struct item_implementacion_dialogo_t *dlg_padre, 
                        const char *texto,
                        struct item_implementacion_menubar_t *menubar,
                        struct item_implementacion_menu_t *submenu)
{
	GtkWidget *gtk_menubar, *gtk_submenu, *gtk_menu;
	
    gtk_menubar = (GtkWidget *)menubar; 
	gtk_menu = (GtkWidget *)submenu;
	
	gtk_submenu = gtk_menu_item_new_with_label(texto);
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(gtk_submenu), gtk_menu);
	gtk_widget_show(gtk_submenu);
    
    gtk_menu_bar_append(GTK_MENU_BAR(gtk_menubar), gtk_submenu);
}

//---------------------------------------------------------

static void i_ejecuta_opcion(GtkWidget *button, gpointer data)
{
	struct callback_t *callback;
		
	callback = (struct callback_t *)data;
	callback_ejecuta(callback);
}

//---------------------------------------------------------

static void i_setImage(GtkWidget *gtk_opcion, const class CImg *imageIcon)
{
    GtkWidget *gtk_image;
    GdkPixbuf *pixbuf;
    
    pixbuf = (GdkPixbuf *)gtkimage_createPixbufFromImage(imageIcon);
    
    gtk_image =  gtk_image_new_from_pixbuf(pixbuf);
    gtk_image_menu_item_set_image((GtkImageMenuItem *)gtk_opcion, gtk_image);
}

//---------------------------------------------------------

void gtkmenu_append_opcion(
                        struct item_implementacion_dialogo_t *dlg_padre, 
                        struct item_implementacion_menu_t *menu_so,
                        const char *texto,
                        const class CImg *imageIcon_opc,
                        struct callback_t *callback)
{
	GtkWidget *gtk_menu, *gtk_opcion;
	
    gtk_menu = (GtkWidget *)menu_so; 
    
    if (imageIcon_opc != NULL)
    {
        gtk_opcion = gtk_image_menu_item_new_with_label(texto);
        i_setImage(gtk_opcion, imageIcon_opc);
    }
    else
        gtk_opcion = gtk_menu_item_new_with_label(texto);
    
	gtk_signal_connect(GTK_OBJECT(gtk_opcion), "activate",  GTK_SIGNAL_FUNC(i_ejecuta_opcion), (gpointer)callback);
	
	gtk_widget_show(gtk_opcion);
    gtk_menu_append(GTK_MENU(gtk_menu), gtk_opcion);
}

//---------------------------------------------------------

void gtkmenu_append_separador(
                        struct item_implementacion_dialogo_t *dlg_padre, 
                        struct item_implementacion_menu_t *menu_so)
{
	GtkWidget *gtk_menu, *gtk_opcion;
	
    gtk_menu = (GtkWidget *)menu_so;
    
	gtk_opcion = gtk_menu_item_new();
    gtk_widget_show(gtk_opcion);
    
    gtk_menu_append(GTK_MENU(gtk_menu), gtk_opcion);
}

//---------------------------------------------------------

struct item_implementacion_toolbar_t *gtkmenu_crea_toolbar(struct item_implementacion_dialogo_t *dlg_padre)
{
    GtkWidget *gtk_toolbar;
    
    gtk_toolbar = gtk_toolbar_new();
    gtk_toolbar_set_tooltips(GTK_TOOLBAR(gtk_toolbar), true);
    gtk_toolbar_set_orientation(GTK_TOOLBAR(gtk_toolbar), GTK_ORIENTATION_HORIZONTAL);
    gtk_toolbar_set_style(GTK_TOOLBAR(gtk_toolbar), GTK_TOOLBAR_ICONS);
    gtk_toolbar_set_icon_size(GTK_TOOLBAR(gtk_toolbar), GTK_ICON_SIZE_SMALL_TOOLBAR);
    gtk_widget_show(gtk_toolbar);
    
    return (struct item_implementacion_toolbar_t *)gtk_toolbar;
}

//---------------------------------------------------------

void gtkmenu_appen_opcion_toolbar(
                        struct item_implementacion_dialogo_t *dlg_padre, 
                        struct item_implementacion_toolbar_t *toolbar_so,
                        const char *textTooltip, 
                        const class CImg *imageIcon,
                        struct callback_t *callback)
{
    GtkWidget *gtk_toolbar, *gtk_image;
    GtkToolItem *gtk_opcion;
    GdkPixbuf *pixbuf;
    
    gtk_toolbar = (GtkWidget *)toolbar_so;
    
    pixbuf = (GdkPixbuf *)gtkimage_createPixbufFromImage(imageIcon);
    gtk_image =  gtk_image_new_from_pixbuf(pixbuf);

    gtk_opcion = gtk_tool_button_new(gtk_image, NULL); //label
    gtk_tool_item_set_tooltip_text(gtk_opcion, textTooltip);
    gtk_signal_connect(GTK_OBJECT(gtk_opcion), "clicked", GTK_SIGNAL_FUNC(i_ejecuta_opcion), (gpointer)callback);
    gtk_widget_show(GTK_WIDGET(gtk_opcion));
    gtk_widget_show(GTK_WIDGET(gtk_image));
    
    gtk_toolbar_insert(GTK_TOOLBAR(gtk_toolbar), gtk_opcion, -1);
}

//---------------------------------------------------------

void gtkmenu_appen_separator_toolbar(
                        struct item_implementacion_dialogo_t *dlg_padre, 
                        struct item_implementacion_toolbar_t *toolbar_so)
{
    GtkToolItem *gtk_separator;
    GtkWidget *gtk_toolbar;
    
    gtk_toolbar = (GtkWidget *)toolbar_so;
    gtk_separator = gtk_separator_tool_item_new();    
    gtk_toolbar_insert(GTK_TOOLBAR(gtk_toolbar), gtk_separator, -1);
    gtk_widget_show(GTK_WIDGET(gtk_separator));
}

