// Definici�n del marco para gtk.

#include "marcogtk.hpp"

#include "gtkinc.inl"
#include "gtkboton.inl"
#include "gtkpanel.inl"
#include "gtkdialogo.inl"
#include "gtkdibujo.inl"
#include "gtkmenu.inl"
#include "gtketiqueta.inl"
#include "gtkcombobox.inl"
#include "gtkedit.inl"
#include "gtktime.inl"
#include "gtkimage.inl"

#include "mrpanel.syh"
#include "mrareadib.syh"
#include "mrmenu.syh"
#include "marcogrf.syh"

#include "asrtbas.h"

static bool i_INICIA_GTK = false;

//---------------------------------------------------------

static void i_inicia_gtk(int argc, char *argv[])
{
	assert(i_INICIA_GTK == false);
	i_INICIA_GTK = true;
    gtk_init(&argc, &argv);
}

//---------------------------------------------------------

static struct item_aplicacion_t *prv_inicia_aplicacion(int argc, char *argv[])
{
	i_inicia_gtk(argc, argv);
    gtk_gl_init (&argc, &argv);

	return (struct item_aplicacion_t *)NULL;
}

//---------------------------------------------------------

static void prv_cierra_aplicacion(struct item_aplicacion_t *datos_aplicacion)
{
    gtk_main_quit();
}

//---------------------------------------------------------

struct marcogrf_t *gtkmarco_crea_marco_gtk()
{
	struct mrpanel_t *gst_panel;
	struct mrareadib_t *gst_areaDibujo;
    struct mrmenu_t *gst_menu;
	
	gst_areaDibujo = mrareadib_create(
                            gtkdibujo_crea, 
                            gtkdibujo_getGraphics, 
                            gtkdibujo_getDimension, 
                            gtkdibujo_redraw);
                            
    gst_menu = mrmenu_create(                            
                        gtkmenu_crea_barra_menu,
                        gtkmenu_crea_menu,
                        gtkmenu_append_submenu,
                        gtkmenu_append_opcion,
                        gtkmenu_append_separador,
                        gtkmenu_crea_toolbar,
                        gtkmenu_appen_opcion_toolbar,
                        gtkmenu_appen_separator_toolbar);
    
    gst_panel = mrpanel_create(
						gtkpanel_crea_panel,
						gtkpanel_append_panel,
						gtkpanel_append_panel,
						gtkpanel_remove_panel,
						gtkpanel_remove_panel);
	
    return marcogrf_crea(
                        prv_inicia_aplicacion,
                        gtkdialogo_get_dialogo,
                        gtkdialogo_lanza_aplicacion,
                        prv_cierra_aplicacion,
                        (marcogrf_FPtr_destruye_datos_aplicacion)NULL, //func_destruye_aplicacion,
                        gtkboton_crea_implementacion,
                        gtketiqueta_crea_implementacion,
                        gtketiqueta_set_texto,
                        gtketiqueta_crea_implementacion_separador,
                        gtkcombobox_crea_implementacion, gtkcombobox_get_opcion_combox,
                        gtkedit_crea_implementacion, gtkedit_get_texto,
                        &gst_panel,
                        &gst_areaDibujo,
                        &gst_menu,
                        gtktime_setTime,
                        NULL, //gtklista_crea,
                        NULL, //gtklista_append_lista,
                        NULL, //gtklista_remove_lista,
                        NULL, //gtklista_cambia_fila,
                        NULL, //gtklista_hay_fila_activa,
                        NULL, //gtklista_set_fila_activa,
                        gtkpanel_mostrar,  
                        gtkdialogo_crea_dialogo,
                        gtkdialogo_lanza_dialogo,
                        gtkdialogo_lanzar_seleccion_fichero,
                        gtkdialogo_lanza_mensaje_aviso,
                        NULL, //gtkfecha_crea_fecha, 
                        NULL); //gtkfecha_fecha);
}

//---------------------------------------------------------

void marcogtk_inicia_imagenes_gtk(int argc, char *argv[])
{
	i_inicia_gtk(argc, argv);
}

//---------------------------------------------------------

class CImg *marcogtk_read_image_from_file(const char *filename)
{
	assert(i_INICIA_GTK == true);
	return gtkimage_read_image_from_file(filename);
}
