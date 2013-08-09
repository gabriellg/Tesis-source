// Combobox para gtk

#include "gtkcombobox.inl"

#include "gtkinc.inl"

#include "asrtbas.h"
#include "array.h"
#include "arrcad.h"
#include "strbas.h"
#include "memory.h"

struct i_gtk_combo_t
{
    GtkWidget *gtk_widget_combobox;
    
    struct combobox_t *combobox;
    marcogrf_ha_cambiado_opcion func_ha_cambiado;
    ArrTipo(item_implementacion_t) *items_menus;
};

//---------------------------------------------------------

static struct i_gtk_combo_t *i_crea_gtk_combo(
                GtkWidget *gtk_widget_combobox,
                struct combobox_t *combobox,
                marcogrf_ha_cambiado_opcion func_ha_cambiado,
                ArrTipo(item_implementacion_t) **items_menus)
{
    struct i_gtk_combo_t *gtk_combobox;
        
    gtk_combobox = MALLOC(struct i_gtk_combo_t);

    gtk_combobox->gtk_widget_combobox = gtk_widget_combobox;
    gtk_combobox->combobox = combobox;
    gtk_combobox->func_ha_cambiado = func_ha_cambiado;
    gtk_combobox->items_menus = ASSIGN_PP_NO_NULL(items_menus, ArrTipo(item_implementacion_t));
    
    return gtk_combobox;
}

//---------------------------------------------------------

void i_destruye_gtk_combo(struct i_gtk_combo_t **gtk_combobox)
{
    assert_no_null(gtk_combobox);
    assert_no_null(*gtk_combobox);
    
    array_Destruye(item_implementacion_t, &(*gtk_combobox)->items_menus, NULL);
    
    FREE_T(gtk_combobox, struct i_gtk_combo_t);
}

//---------------------------------------------------------

static void i_destruir_componente(GtkWidget *gtk_combobox, gpointer data)
{
    struct i_gtk_combo_t *datos_gtk_combobox;
        
    datos_gtk_combobox = (struct i_gtk_combo_t *)data;
    i_destruye_gtk_combo(&datos_gtk_combobox);
}

//---------------------------------------------------------

static ArrTipo(item_implementacion_t) *i_crea_items(const ArrString *texto_opciones)
{
    ArrTipo(item_implementacion_t) *items;
    unsigned long i, num_textos;
    
    num_textos = arrcad_numElementos(texto_opciones);
    items = array_CreaPuntero(num_textos, item_implementacion_t);
    
    for (i = 0; i < num_textos; i++)
    {
        const char *texto;
        GtkWidget *gtk_item_menu;
        
        texto = arrcad_Get(texto_opciones, i);
        
        gtk_item_menu = gtk_menu_item_new_with_label(texto); 
        gtk_widget_show(gtk_item_menu);
        array_Set(item_implementacion_t, items, i,(struct item_implementacion_t *)gtk_item_menu);
    }
    
    return items;
}

//---------------------------------------------------------

static GtkWidget *i_crea_menu(ArrTipo(item_implementacion_t) *items_menus)
{
    GtkWidget *menu;
    unsigned long i, num_items;
    
    menu = gtk_menu_new();
    num_items = array_numElementos(item_implementacion_t, items_menus);
    
    for (i = 0; i < num_items; i++)
    {
        GtkWidget *gtk_item_menu;
        
        gtk_item_menu = (GtkWidget *)array_Get(item_implementacion_t, items_menus, i);
        gtk_menu_append(GTK_MENU(menu), gtk_item_menu);
    }
    
    return menu;
}

//---------------------------------------------------------

static unsigned long i_get_opcion_combox(
                        GtkWidget *gtk_combobox, 
                        const ArrTipo(item_implementacion_t) *items_menus)
{
    bool existe;
    unsigned long posicion;
    GtkWidget *gtk_menu, *gtk_opcion_activa;
    
    gtk_menu = gtk_option_menu_get_menu(GTK_OPTION_MENU(gtk_combobox));
    gtk_opcion_activa = gtk_menu_get_active(GTK_MENU(gtk_menu));
    
    existe = array_BuscarUnica(item_implementacion_t, items_menus, item_implementacion_t, (struct item_implementacion_t *)gtk_opcion_activa, NULL, &posicion);
    assert(existe == true);
    
    return posicion;
}

//---------------------------------------------------------

static void i_cambia_opcion(GtkMenuShell *menushell, gpointer user_data)
{
    struct i_gtk_combo_t *datos_gtk_combobox;
    unsigned long opcion_seleccionada;
    
    assert_no_null(user_data);
    
    datos_gtk_combobox = (struct i_gtk_combo_t *)user_data;
    
    assert_no_null(datos_gtk_combobox->func_ha_cambiado);
    
    opcion_seleccionada = i_get_opcion_combox(datos_gtk_combobox->gtk_widget_combobox, datos_gtk_combobox->items_menus);
    datos_gtk_combobox->func_ha_cambiado(datos_gtk_combobox->combobox, opcion_seleccionada);
}

//---------------------------------------------------------

struct item_implementacion_t *gtkcombobox_crea_implementacion(
                        struct item_implementacion_dialogo_t *dlg_padre, 
                        struct combobox_t *combobox,
                        marcogrf_ha_cambiado_opcion func_ha_cambiado,
                        const ArrString *texto_opciones,
                        unsigned long ind_valor)
{
    struct i_gtk_combo_t *datos_gtk_combo;
	GtkWidget *gtk_combobox, *gtk_menu;
    ArrTipo(item_implementacion_t) *items;

    items = i_crea_items(texto_opciones);
    gtk_menu = i_crea_menu(items);
	
	gtk_combobox = gtk_option_menu_new();
    gtk_option_menu_set_menu(GTK_OPTION_MENU(gtk_combobox), gtk_menu);
    
    datos_gtk_combo = i_crea_gtk_combo(gtk_combobox, combobox, func_ha_cambiado, &items);
    
    gtk_object_set_data(GTK_OBJECT(gtk_combobox), "datos", datos_gtk_combo);
    gtk_signal_connect(GTK_OBJECT(gtk_combobox), "destroy", GTK_SIGNAL_FUNC(i_destruir_componente), (gpointer)datos_gtk_combo);
    
    if (func_ha_cambiado != NULL)
        gtk_signal_connect(GTK_OBJECT(gtk_menu), "selection-done", GTK_SIGNAL_FUNC(i_cambia_opcion), (gpointer)datos_gtk_combo);
	
	return (struct item_implementacion_t *)gtk_combobox;
}

//---------------------------------------------------------

unsigned long gtkcombobox_get_opcion_combox(struct item_implementacion_t *combobox)
{
    struct i_gtk_combo_t *datos_gtk_combobox;
    GtkWidget *gtk_combobox;

    gtk_combobox = (GtkWidget *)combobox;
    datos_gtk_combobox = (struct i_gtk_combo_t *)gtk_object_get_data(GTK_OBJECT(gtk_combobox), "datos");
    assert_no_null(datos_gtk_combobox);
    
    return i_get_opcion_combox(gtk_combobox, datos_gtk_combobox->items_menus);
}
