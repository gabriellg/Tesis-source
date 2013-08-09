// Marco de aplicaciones para aplicaciones gráficas.

#include "marcogrf.syh"
#include "marcogrf.hxx"

#include "mrareadib.inl"
#include "mrmenu.inl"
#include "mrareadib.tli"
#include "mrmenu.tli"
#include "mrpanel.inl"
#include "mrpanel.tli"

#include "asrtbas.h"
#include "memory.h"

struct marcogrf_t
{
	struct item_aplicacion_t *datos_aplicacion;
	
    marcogrf_FPtr_inicia_aplicacion func_inicia_aplicacion;
    marcogrf_FPtr_get_dialogo func_get_dialogo;
    marcogrf_FPtr_lanzar_aplicacion func_lanzar_aplicacion;
    marcogrf_FPtr_cerra_aplicacion func_cerrar_aplicacion;
    marcogrf_FPtr_destruye_datos_aplicacion func_destruye_aplicacion;

    marcogrf_FPtr_crea_boton func_crea_boton;
    
    marcogrf_FPtr_crea_etiqueta func_crea_etiqueta;
    marcogrf_FPtr_set_texto_etiqueta func_set_texto_etiqueta;
    marcogrf_FPtr_crea_linea_separacion func_crea_linea_separacion;
   
    marcogrf_FPtr_crea_combobox func_crea_combobox;
    marcogrf_FPtr_get_opcion_combobox func_get_opcion_combobox;

    marcogrf_FPtr_crea_edit func_crea_edit;
    marcogrf_FPtr_get_texto func_get_texto;

    struct mrpanel_t *gst_panel;
    
	struct mrareadib_t *gst_areaDibujo;

    struct mrmenu_t *gst_menus;	

    marcogrf_FPtr_setTime func_setTime;    

    marcogrf_FPtr_crea_lista func_crea_lista;
    marcogrf_FPtr_append_lista func_append_lista;
    marcogrf_FPtr_remove_lista func_remove_lista;
    marcogrf_FPtr_cambia_fila func_cambia_fila;
    marcogrf_FPtr_get_fila_activa func_get_fila_activa;
    marcogrf_FPtr_set_fila_activa func_set_fila_activa;

    marcogrf_FPtr_mostrar func_mostrar;
    
    marcogrf_FPtr_crea_dialogo func_crea_dialogo;
    marcogrf_FPtr_lanzar_dialogo_aceptar func_lanzar_dialogo_aceptar;
    marcogrf_FPtr_lanzar_seleccion_fichero func_lanzar_seleccion_fichero;
    marcogrf_FPtr_lanza_aviso_pregunta func_lanza_aviso_pregunta;
    
    marcogrf_FPtr_crea_fecha func_crea_fecha;
    marcogrf_FPtr_fecha func_fecha;
};

//---------------------------------------------------------

struct marcogrf_t *marcogrf_crea(
                        marcogrf_FPtr_inicia_aplicacion func_inicia_aplicacion,
                        marcogrf_FPtr_get_dialogo func_get_dialogo,
                        marcogrf_FPtr_lanzar_aplicacion func_lanzar_aplicacion,
                        marcogrf_FPtr_cerra_aplicacion func_cerrar_aplicacion,
                        marcogrf_FPtr_destruye_datos_aplicacion func_destruye_aplicacion,
                        marcogrf_FPtr_crea_boton func_crea_boton,
                        marcogrf_FPtr_crea_etiqueta func_crea_etiqueta,
                        marcogrf_FPtr_set_texto_etiqueta func_set_texto_etiqueta,
                        marcogrf_FPtr_crea_linea_separacion func_crea_linea_separacion,   
                        marcogrf_FPtr_crea_combobox func_crea_combobox,
                        marcogrf_FPtr_get_opcion_combobox func_get_opcion_combobox,
                        marcogrf_FPtr_crea_edit func_crea_edit,
                        marcogrf_FPtr_get_texto func_get_texto,
                        struct mrpanel_t **gst_panel,
                        struct mrareadib_t **gst_areaDibujo,
                        struct mrmenu_t **gst_menus,
                        marcogrf_FPtr_setTime func_setTime,
                        marcogrf_FPtr_crea_lista func_crea_lista,
                        marcogrf_FPtr_append_lista func_append_lista,
                        marcogrf_FPtr_remove_lista func_remove_lista,
                        marcogrf_FPtr_cambia_fila func_cambia_fila,
                        marcogrf_FPtr_get_fila_activa func_get_fila_activa,
                        marcogrf_FPtr_set_fila_activa func_set_fila_activa,
                        marcogrf_FPtr_mostrar func_mostrar,  
                        marcogrf_FPtr_crea_dialogo func_crea_dialogo,
                        marcogrf_FPtr_lanzar_dialogo_aceptar func_lanzar_dialogo_aceptar,
                        marcogrf_FPtr_lanzar_seleccion_fichero func_lanzar_seleccion_fichero,
                        marcogrf_FPtr_lanza_aviso_pregunta func_lanza_aviso_pregunta,
                        marcogrf_FPtr_crea_fecha func_crea_fecha,
                        marcogrf_FPtr_fecha func_fecha)
{
	struct marcogrf_t *marco;
		
	marco = MALLOC(struct marcogrf_t);
	
	marco->datos_aplicacion = NULL;
    
    marco->func_inicia_aplicacion = func_inicia_aplicacion;
    marco->func_get_dialogo = func_get_dialogo;
    marco->func_lanzar_aplicacion = func_lanzar_aplicacion;
    marco->func_cerrar_aplicacion = func_cerrar_aplicacion;
    marco->func_destruye_aplicacion = func_destruye_aplicacion;

    marco->func_crea_boton = func_crea_boton;
    marco->func_crea_etiqueta = func_crea_etiqueta;
    marco->func_set_texto_etiqueta = func_set_texto_etiqueta;     
    marco->func_crea_linea_separacion = func_crea_linea_separacion;
   
    marco->func_crea_combobox = func_crea_combobox;
    marco->func_get_opcion_combobox = func_get_opcion_combobox;

    marco->func_crea_edit = func_crea_edit;
    marco->func_get_texto = func_get_texto;

    marco->gst_panel = ASSIGN_PP_NO_NULL(gst_panel, struct mrpanel_t);
	marco->gst_areaDibujo = ASSIGN_PP_NO_NULL(gst_areaDibujo, struct mrareadib_t);
    marco->gst_menus = ASSIGN_PP_NO_NULL(gst_menus, struct mrmenu_t);     
	
    marco->func_setTime = func_setTime;

    marco->func_crea_lista = func_crea_lista;
    marco->func_append_lista = func_append_lista;
    marco->func_remove_lista = func_remove_lista;
    marco->func_cambia_fila = func_cambia_fila;
    marco->func_get_fila_activa = func_get_fila_activa;
    marco->func_set_fila_activa = func_set_fila_activa;

    marco->func_mostrar = func_mostrar;
    
    marco->func_crea_dialogo = func_crea_dialogo;
    marco->func_lanzar_dialogo_aceptar = func_lanzar_dialogo_aceptar;
    marco->func_lanzar_seleccion_fichero = func_lanzar_seleccion_fichero;
    marco->func_lanza_aviso_pregunta = func_lanza_aviso_pregunta;

    marco->func_crea_fecha = func_crea_fecha;
    marco->func_fecha = func_fecha;

	return marco;
}

//---------------------------------------------------------

void marcogrf_destruye(struct marcogrf_t **marco)
{
    assert_no_null(marco);
    assert_no_null(*marco);
    
    if ((*marco)->datos_aplicacion != NULL)
    {
        assert_no_null((*marco)->func_destruye_aplicacion);
        (*marco)->func_destruye_aplicacion(&((*marco)->datos_aplicacion));
    }
    
    mrpanel_destroy(&(*marco)->gst_panel);
    mrareadib_destroy(&(*marco)->gst_areaDibujo);
    mrmenu_destroy(&(*marco)->gst_menus);
    
	FREE_T(marco, struct marcogrf_t);
}

//---------------------------------------------------------

void marcogrf_inicia_aplicacion(struct marcogrf_t *marco, int argc, char *argv[])
{
    assert_no_null(marco);
	assert_no_null(marco->func_inicia_aplicacion);
	assert(marco->datos_aplicacion == NULL);
	
	marco->datos_aplicacion = marco->func_inicia_aplicacion(argc, argv);
}
    
//---------------------------------------------------------

struct item_implementacion_dialogo_t *marcogrf_get_dialogo(struct marcogrf_t *marco)
{
    assert_no_null(marco);
    assert_no_null(marco->func_get_dialogo);
    return marco->func_get_dialogo(marco->datos_aplicacion);
}

//---------------------------------------------------------

void marcogrf_lanzar_aplicacion(
                            struct marcogrf_t *marco,
                            struct item_implementacion_dialogo_t **dialogo_aplicacion,
                            const char *titulo_aplicacion,
                            struct callback_t *funcion_eventos_teclado,
                            struct item_implementacion_menubar_t **menubar,
                            struct item_implementacion_toolbar_t **toolbar,
                            struct item_implementacion_t **area)
{
    assert_no_null(marco);
    assert_no_null(marco->func_lanzar_aplicacion);
    marco->func_lanzar_aplicacion(
                            marco->datos_aplicacion, 
                            dialogo_aplicacion, 
                            titulo_aplicacion, 
                            funcion_eventos_teclado,
                            menubar, toolbar, area);
}

//---------------------------------------------------------

void marcogrf_cierra_aplicacion(struct marcogrf_t *marco)
{
    assert_no_null(marco);
    assert_no_null(marco->func_cerrar_aplicacion);
    marco->func_cerrar_aplicacion(marco->datos_aplicacion);
}

//---------------------------------------------------------

struct item_implementacion_t *marcogrf_crea_boton(
                            struct marcogrf_t *marco, 
                            struct item_implementacion_dialogo_t *dlg_padre, 
                            const char *texto, struct callback_t *callback)
{
    assert_no_null(marco);
    assert_no_null(marco->func_crea_boton);
    
    return marco->func_crea_boton(dlg_padre, texto, callback);
}

//---------------------------------------------------------

struct item_implementacion_t *marcogrf_crea_etiqueta(
                            struct marcogrf_t *marco,
                            struct item_implementacion_dialogo_t *dlg_padre, 
                            const char *texto,
                            enum marcogrf_justificacion_horizontal_t tipo_justificacion_horizontal)
{
    assert_no_null(marco);
    assert_no_null(marco->func_crea_etiqueta);
    
    return marco->func_crea_etiqueta(dlg_padre, texto, tipo_justificacion_horizontal);
}

//---------------------------------------------------------

void marcogrf_set_texto(
					const struct marcogrf_t *marco, 
					struct item_implementacion_t *item_implementacion,
					const char *texto)
{
    assert_no_null(marco);
    assert_no_null(marco->func_set_texto_etiqueta);
    
    return marco->func_set_texto_etiqueta(item_implementacion, texto);
}

//---------------------------------------------------------

struct item_implementacion_t *marcogrf_crea_linea_separacion(
                            struct marcogrf_t *marco,
                            struct item_implementacion_dialogo_t *dlg_padre, 
                            enum marcogrf_tipo_separador_t tipo_seperador)
{
    assert_no_null(marco);
    assert_no_null(marco->func_crea_linea_separacion);
    
    return marco->func_crea_linea_separacion(dlg_padre, tipo_seperador);
}
   
//---------------------------------------------------------

struct item_implementacion_t *marcogrf_crea_combobox(
                            struct marcogrf_t *marco, 
                            struct item_implementacion_dialogo_t *dlg_padre,
                            struct combobox_t *combobox,
                            marcogrf_ha_cambiado_opcion func_ha_cambiado, 
                            const ArrString *texto_opciones,
                            unsigned long ind_valor)
{
    assert_no_null(marco);
    assert_no_null(marco->func_crea_combobox);
    return marco->func_crea_combobox(dlg_padre, combobox, func_ha_cambiado, texto_opciones, ind_valor);
}

//---------------------------------------------------------

unsigned long marcogrf_get_opcion_combobox(
                        const struct marcogrf_t *marco, 
                        struct item_implementacion_t *combobox)
{
    assert_no_null(marco);
    assert_no_null(marco->func_get_opcion_combobox);
    return marco->func_get_opcion_combobox(combobox);
}

//---------------------------------------------------------

struct item_implementacion_t *marcogrf_crea_edit(
                        struct marcogrf_t *marco, 
                        struct item_implementacion_dialogo_t *dlg_padre, 
                        const char *texto, 
                        const char *filtro_teclas, 
                        struct edit_t *dato, 
                        bool es_multilinea, bool editable,
                        unsigned long ancho, unsigned long alto,
                        marco_FPtr_comprobar func_filtra)
{
    assert_no_null(marco);
    assert_no_null(marco->func_crea_edit);
    return marco->func_crea_edit(dlg_padre, texto, filtro_teclas, dato, 
                            es_multilinea, editable,
                            ancho, alto, func_filtra);
}

//---------------------------------------------------------

char *marcogrf_get_texto(const struct marcogrf_t *marco, struct item_implementacion_t *edit)
{
    assert_no_null(marco);
    assert_no_null(marco->func_get_texto);
    return marco->func_get_texto(edit);
}

//---------------------------------------------------------

struct item_implementacion_t *marcogrf_crea_panel(
                        struct marcogrf_t *marco,
                        enum mrpanel_tipo_disposicion_t tipo_disposicion, 
                        unsigned long num_filas, unsigned long num_columnas,
                        bool homogeneus, unsigned long pixelsSpacing)
{
    assert_no_null(marco);
    assert_no_null(marco->gst_panel);
    assert_no_null(marco->gst_panel->func_crea_panel);
    
    return marco->gst_panel->func_crea_panel(tipo_disposicion, num_filas, num_columnas, homogeneus, pixelsSpacing);
}

//---------------------------------------------------------

void marcogrf_append_panel(
                        struct marcogrf_t *marco,
                        struct item_implementacion_t *panel,
                        enum mrpanel_tipo_disposicion_t tipo_disposicion, 
                        unsigned long ind_fila, unsigned long ind_columna,
                        bool expand, bool fill,
                        struct item_implementacion_t *panel_hijo)
{
    assert_no_null(marco);
    assert_no_null(marco->gst_panel);
    assert_no_null(marco->gst_panel->func_append_panel);
    
    marco->gst_panel->func_append_panel(panel, tipo_disposicion, ind_fila, ind_columna, expand, fill, panel_hijo);
}

//---------------------------------------------------------

void marcogrf_append_componente(
                        struct marcogrf_t *marco, 
                        struct item_implementacion_t *panel, 
                        enum mrpanel_tipo_disposicion_t tipo_disposicion, 
                        unsigned long ind_fila, unsigned long ind_columna,
                        bool expand, bool fill,
                        struct item_implementacion_t *componente_hijo)
{
    assert_no_null(marco);
    assert_no_null(marco->gst_panel);
    assert_no_null(marco->gst_panel->func_append_componente);
    marco->gst_panel->func_append_componente(panel, tipo_disposicion, ind_fila, ind_columna, expand, fill, componente_hijo);
}

//---------------------------------------------------------

void marcogrf_remove_panel(
                        struct marcogrf_t *marco,
                        struct item_implementacion_t *panel,
                        struct item_implementacion_t *panel_a_borrar)
{
    assert_no_null(marco);
    assert_no_null(marco->gst_panel);
    assert_no_null(marco->gst_panel->func_remove_panel);
    marco->gst_panel->func_remove_panel(panel, panel_a_borrar);
}

//---------------------------------------------------------

void marcogrf_remove_componente(
                        struct marcogrf_t *marco,
                        struct item_implementacion_t *panel,
                        struct item_implementacion_t *panel_a_borrar)
{
    assert_no_null(marco);
    assert_no_null(marco->gst_panel);
    assert_no_null(marco->gst_panel->func_remove_panel);
    marco->gst_panel->func_remove_componente(panel, panel_a_borrar);
}

//---------------------------------------------------------
  
struct item_implementacion_t *marcogrf_crea_area_dibujo(
                        struct marcogrf_t *marco, 
                        struct item_implementacion_dialogo_t *dlg_padre, 
						struct callback_t *callback_iniciaGrafico, 
						struct callback_t *callback_definePerspectiva,
						struct callback_t *callback_dibuja,
                        struct callback_t *funcion_evento_raton, 
                        unsigned long ancho, unsigned long alto)
{
    assert_no_null(marco);
    assert_no_null(marco->gst_areaDibujo);
    assert_no_null(marco->gst_areaDibujo->func_creaAreaDibujo);
    return marco->gst_areaDibujo->func_creaAreaDibujo(
    					dlg_padre, 
						callback_iniciaGrafico,
						callback_definePerspectiva,
						callback_dibuja,
    					funcion_evento_raton,
    					ancho, alto);
}

//---------------------------------------------------------

void marcogrf_getDimensionAreaDib(
                        struct marcogrf_t *marco, 
                        struct item_implementacion_t *area_dibujo,
                        unsigned long *width, unsigned long *heigth)
{
    assert_no_null(marco);
    assert_no_null(marco->gst_areaDibujo->func_getDimension);
    marco->gst_areaDibujo->func_getDimension(area_dibujo, width, heigth);
}

//---------------------------------------------------------

class IGraphics *marcogrf_getGraphics(
                        struct marcogrf_t *marco, 
                        struct item_implementacion_t *area_dibujo)
{
    assert_no_null(marco);
    assert_no_null(marco->gst_areaDibujo->func_getGraphics);
    return marco->gst_areaDibujo->func_getGraphics(area_dibujo);
}

//---------------------------------------------------------

void marcogrf_redraw(struct marcogrf_t *marco, struct item_implementacion_t *area_dibujo)
{
	assert_no_null(marco);	
	assert_no_null(marco->gst_areaDibujo->func_redraw);
	
	marco->gst_areaDibujo->func_redraw(area_dibujo);
}
	
//---------------------------------------------------------

struct item_implementacion_menubar_t *marcogrf_crea_barra_menu(
                        struct marcogrf_t *marco, 
                        struct item_implementacion_dialogo_t *dlg_padre)
{
    assert_no_null(marco);
    assert_no_null(marco->gst_menus);
    assert_no_null(marco->gst_menus->func_crea_barra_menu);
    return marco->gst_menus->func_crea_barra_menu(dlg_padre);
}

//---------------------------------------------------------

struct item_implementacion_menu_t *marcogrf_crea_menu(
                        const struct marcogrf_t *marco, 
                        struct item_implementacion_dialogo_t *dlg_padre)
{
    assert_no_null(marco);
    assert_no_null(marco->gst_menus);
    assert_no_null(marco->gst_menus->func_crea_menu);
    
    return marco->gst_menus->func_crea_menu(dlg_padre);
}    
    
//---------------------------------------------------------

void marcogrf_append_submenu(
                        const struct marcogrf_t *marco, 
                        struct item_implementacion_dialogo_t *dlg_padre, 
                        const char *texto,
                        struct item_implementacion_menubar_t *menubar_so,
                        struct item_implementacion_menu_t *submenu_so)
{
    assert_no_null(marco);
    assert_no_null(marco->gst_menus);
    assert_no_null(marco->gst_menus->func_append_submenu);
    
    marco->gst_menus->func_append_submenu(dlg_padre, texto, menubar_so, submenu_so);
}

//---------------------------------------------------------

void marcogrf_append_opcion(
                        struct marcogrf_t *marco, 
                        struct item_implementacion_dialogo_t *dlg_padre, 
                        struct item_implementacion_menu_t *menu_so,
                        const char *texto,
                        const class CImg *imageIcon_opc,
                        struct callback_t *callback)
{
    assert_no_null(marco);
    assert_no_null(marco->gst_menus);
    assert_no_null(marco->gst_menus->func_append_opcion);
    
    marco->gst_menus->func_append_opcion(dlg_padre, menu_so, texto, imageIcon_opc, callback);
                        
}
    
//---------------------------------------------------------

void marcogrf_append_separador(
                        struct marcogrf_t *marco,
                        struct item_implementacion_dialogo_t *dlg_padre,
                        struct item_implementacion_menu_t *menu_so)
{
    assert_no_null(marco);
    assert_no_null(marco->gst_menus);
    assert_no_null(marco->gst_menus->func_append_separador);
    
    return marco->gst_menus->func_append_separador(dlg_padre, menu_so);
}

//---------------------------------------------------------

struct item_implementacion_toolbar_t *marcogrf_crea_toolbar( 
                        struct marcogrf_t *marco,
                        struct item_implementacion_dialogo_t *dlg_padre_so)
{
    assert_no_null(marco);
    assert_no_null(marco->gst_menus);
    assert_no_null(marco->gst_menus->func_crea_toolbar);
    
    return marco->gst_menus->func_crea_toolbar(dlg_padre_so);
}

//---------------------------------------------------------

void marcogrf_append_toolbar_opcion(
                        struct marcogrf_t *marco,
                        struct item_implementacion_dialogo_t *dlg_padre, 
                        struct item_implementacion_toolbar_t *toolbar, 
                        const char *textoTooltip, 
                        const class CImg *imageIcon,
                        struct callback_t *callback)
{
    assert_no_null(marco);
    assert_no_null(marco->gst_menus);
    assert_no_null(marco->gst_menus->func_append_toolbar_opcion);
    
    return marco->gst_menus->func_append_toolbar_opcion(dlg_padre, toolbar, textoTooltip, imageIcon, callback);
}

//---------------------------------------------------------

void marcogrf_append_toolbar_separator(
                        struct marcogrf_t *marco,
                        struct item_implementacion_dialogo_t *dlg_padre, 
                        struct item_implementacion_toolbar_t *toolbar)
{
    assert_no_null(marco);
    assert_no_null(marco->gst_menus);
    assert_no_null(marco->gst_menus->func_append_toolbar_separador);
    
    return marco->gst_menus->func_append_toolbar_separador(dlg_padre, toolbar);
}
	
//---------------------------------------------------------

void marcogrf_setTime(
                    const struct marcogrf_t *marco,
                    struct item_implementacion_t *item_widget,
                    unsigned long timeout_miliseconds, 
                    struct callback_t **callback) 
{
    assert_no_null(marco);
    assert_no_null(marco->func_setTime);
    
    marco->func_setTime(item_widget, timeout_miliseconds, callback);
}

//---------------------------------------------------------

struct item_implementacion_t *marcogrf_crea_lista(
                    struct marcogrf_t *marco,
                    struct item_implementacion_dialogo_t *dlg_padre, 
                    const ArrString *titulos,
                    const ArrPointer(ArrString) *datos,
                    bool hay_seleccion,
                    unsigned long indice_seleccionado)
{
    assert_no_null(marco);
    assert_no_null(marco->func_crea_lista);
    
    return marco->func_crea_lista(dlg_padre, titulos, datos, hay_seleccion, indice_seleccionado);
}

//---------------------------------------------------------

void marcogrf_append_lista(
                    struct marcogrf_t *marco,
                    struct item_implementacion_t *implementacion, 
                    const ArrString *datos_fila)
{
    assert_no_null(marco);
    assert_no_null(marco->func_append_lista);
    
    marco->func_append_lista(implementacion, datos_fila);
}

//---------------------------------------------------------

void marcogrf_remove_lista(
                    struct marcogrf_t *marco, 
                    struct item_implementacion_t *implementacion, 
                    unsigned long fila)
{
    assert_no_null(marco);
    assert_no_null(marco->func_remove_lista);
    
    marco->func_remove_lista(implementacion, fila);
}

//---------------------------------------------------------

void marcogrf_cambia_fila(
                    struct marcogrf_t *marco, 
                    struct item_implementacion_t *implementacion, 
                    unsigned long fila, 
                    const ArrString *datos_fila)
{
    assert_no_null(marco);
    assert_no_null(marco->func_cambia_fila);
    
    marco->func_cambia_fila(implementacion, fila, datos_fila);
}

//---------------------------------------------------------

bool marcogrf_get_fila_activa(
                    struct marcogrf_t *marco,
                    struct item_implementacion_t *implementacion, 
                    unsigned long *fila)
{
    assert_no_null(marco);
    assert_no_null(marco->func_get_fila_activa);
    
    return marco->func_get_fila_activa(implementacion, fila);
}

//---------------------------------------------------------

void marcogrf_set_fila_activa(
                    struct marcogrf_t *marco,
                    struct item_implementacion_t *implementacion, 
                    unsigned long fila)
{
    assert_no_null(marco);
    assert_no_null(marco->func_set_fila_activa);
    
    marco->func_set_fila_activa(implementacion, fila);
}

//---------------------------------------------------------

void marcogrf_mostrar(struct marcogrf_t *marco, struct item_implementacion_t *componente)
{
    assert_no_null(marco);
    assert_no_null(marco->func_mostrar);
    
    marco->func_mostrar(componente);
}
    
//---------------------------------------------------------

struct item_implementacion_dialogo_t *marcogrf_crea_dialogo(
                    struct marcogrf_t *marco,
                    struct item_implementacion_dialogo_t *dlg_padre)
{
    assert_no_null(marco);
    assert_no_null(marco->func_crea_dialogo);
    
    return marco->func_crea_dialogo(dlg_padre);
}

//---------------------------------------------------------

void marcogrf_lanzar_dialogo_aceptar(
                    struct marcogrf_t *marco,
                    struct item_implementacion_dialogo_t **implementacion_dialogo,
                    const char *titulo,
                    struct item_implementacion_t **area_trabajo,
                    struct dialogo_t *dialogo,
                    struct callback_t *funcion_eventos_teclado,
                    bool con_boton_aceptar,
                    bool *aceptado,
                    marcogrf_FPtr_se_puede_aceptar func_se_puede_aceptar,
                    marcogrf_FPtr_aceptar func_aceptar)
{
    assert_no_null(marco);
    assert_no_null(marco->func_lanzar_dialogo_aceptar);
    
    marco->func_lanzar_dialogo_aceptar(
                    marco->datos_aplicacion,
                    implementacion_dialogo, titulo, area_trabajo,
                    dialogo, funcion_eventos_teclado, con_boton_aceptar, aceptado, 
                    func_se_puede_aceptar, func_aceptar);
}
                            
//---------------------------------------------------------

bool marcogrf_lanzar_seleccion_fichero(
                    struct marcogrf_t *marco,
                    const char *titulo_dialogo, 
                    char **fichero_seleccionado,
                    bool abrirFichero)
{
    assert_no_null(marco);
    assert_no_null(marco->func_lanzar_seleccion_fichero);
    
    return marco->func_lanzar_seleccion_fichero(titulo_dialogo, fichero_seleccionado, abrirFichero);
}

//---------------------------------------------------------

void marcogrf_lanza_aviso_pregunta(
                    struct marcogrf_t *marco, 
                    struct item_implementacion_dialogo_t *dlg_padre, 
                    enum marcogrf_tipo_aviso_t tipo_aviso, 
                    const char *mensaje, bool *aceptado)
{
    assert_no_null(marco);
    assert_no_null(marco->func_lanza_aviso_pregunta);
    
    return marco->func_lanza_aviso_pregunta(dlg_padre, tipo_aviso, mensaje, aceptado);
}

//---------------------------------------------------------

struct item_implementacion_t *marcogrf_crea_fecha(
                    struct marcogrf_t *marco,
                    struct item_implementacion_dialogo_t *dlg_padre, 
                    enum marcogrf_tipo_fecha_t tipo_fecha, 
                    const struct fecha_t *fecha)
{
    assert_no_null(marco);
    assert_no_null(marco->func_crea_fecha);
    
    return marco->func_crea_fecha(dlg_padre, tipo_fecha, fecha);
}
    
//---------------------------------------------------------

struct fecha_t *marcogrf_fecha(struct marcogrf_t *marco, struct item_implementacion_t *fecha)
{
    assert_no_null(marco);
    assert_no_null(marco->func_fecha);
    
    return marco->func_fecha(fecha);
}
