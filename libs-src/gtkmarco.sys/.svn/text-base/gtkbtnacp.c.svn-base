// Boton de aceptar y cancelar.

#include "gtkbtnacp.inl"

#include "gtkboton.inl"
#include "gtkmensaje.inl"
#include "gtkkitdlg.inl"

#include "asrtbas.h"
#include "memory.h"
#include "strbas.h"
#include "marcogrf.hxx"

struct gtkbtnacp_t
{
    bool aceptado;

    GtkWidget *widget_dialogo;
    struct dialogo_t *dialogo;
    char **nombre_fichero;
    marcogrf_FPtr_se_puede_aceptar func_se_puede_aceptar;
    marcogrf_FPtr_aceptar func_aceptar;
};

//---------------------------------------------------------

struct gtkbtnacp_t *i_crea(
                    bool aceptado,
                    GtkWidget **widget_dialogo,
                    struct dialogo_t *dialogo,
                    char **nombre_fichero,
                    marcogrf_FPtr_se_puede_aceptar func_se_puede_aceptar,
                    marcogrf_FPtr_aceptar func_aceptar)
{
    struct gtkbtnacp_t *datos_dialogo;
    
    datos_dialogo = MALLOC(struct gtkbtnacp_t);
        
    datos_dialogo->aceptado = aceptado;
    
    datos_dialogo->widget_dialogo = ASSIGN_PP(widget_dialogo, GtkWidget);
    datos_dialogo->dialogo = dialogo;
    datos_dialogo->nombre_fichero = nombre_fichero;
    datos_dialogo->func_se_puede_aceptar = func_se_puede_aceptar;
    datos_dialogo->func_aceptar = func_aceptar;
        
    return datos_dialogo;
}


//---------------------------------------------------------

struct gtkbtnacp_t *gtkbtnacp_crea_datos_dialogo(
                    struct dialogo_t *dialogo,
                    marcogrf_FPtr_se_puede_aceptar func_se_puede_aceptar,
                    marcogrf_FPtr_aceptar func_aceptar)
{
    bool aceptado;
    GtkWidget *widget_dialogo;
    
    aceptado = false;
    widget_dialogo = NULL;
    return i_crea(aceptado, &widget_dialogo, dialogo, NULL, func_se_puede_aceptar, func_aceptar);
}

//---------------------------------------------------------

struct gtkbtnacp_t *gtkbtnacp_crea_datos_dialogo_para_file(char **nombre_fichero)
{
    bool aceptado;
    GtkWidget *widget_dialogo;
    struct dialogo_t *dialogo;
    marcogrf_FPtr_se_puede_aceptar func_se_puede_aceptar;
    marcogrf_FPtr_aceptar func_aceptar;
    
    aceptado = false;
    widget_dialogo = NULL;
    dialogo = NULL;
    func_se_puede_aceptar = NULL;
    func_aceptar = NULL;
    
    return i_crea(aceptado, &widget_dialogo, dialogo, nombre_fichero, func_se_puede_aceptar, func_aceptar);
}

//---------------------------------------------------------

static void i_destruye(struct gtkbtnacp_t **datos_dialogo)
{
    FREE_T(datos_dialogo, struct gtkbtnacp_t);
}

//---------------------------------------------------------

static bool i_se_puede_finalizar(struct dialogo_t *dialogo, marcogrf_FPtr_se_puede_aceptar func_se_puede_aceptar, char **mensaje_error)
{
    if (func_se_puede_aceptar != NULL)
        return func_se_puede_aceptar(dialogo, mensaje_error);
    else
        return true;
}

//---------------------------------------------------------

static bool i_acepta_dialogo_si_puede(struct gtkbtnacp_t *datos_dialogo)
{
    bool acepta_dialogo;
    char *mensaje_error;
    
    assert_no_null(datos_dialogo);
    
    mensaje_error = NULL;
    if (i_se_puede_finalizar(datos_dialogo->dialogo, datos_dialogo->func_se_puede_aceptar, &mensaje_error) == true)
    {
        if (datos_dialogo->func_aceptar != NULL)
            datos_dialogo->func_aceptar(datos_dialogo->dialogo);
        
        acepta_dialogo = true;
    }
    else
    {
        gtkmensaje_lanza_mensaje_error(datos_dialogo->widget_dialogo, mensaje_error);
        strbas_destroy(&mensaje_error);
        acepta_dialogo = false;
    }
    
    return acepta_dialogo;
}

//---------------------------------------------------------

static void i_acepta_cancela_dialogo(gpointer data, bool aceptado)
{
    struct gtkbtnacp_t *datos_dialogo;
    bool cierra_dialogo;
        
    assert_no_null(data);
    datos_dialogo = (struct gtkbtnacp_t *)data;
        
    datos_dialogo->aceptado = aceptado;
    
    if (aceptado == true)
    {
        if (datos_dialogo->nombre_fichero != NULL)
        {
            GtkFileSelection *file_selection;
    
            file_selection = GTK_FILE_SELECTION(datos_dialogo->widget_dialogo);
            
            if (*(datos_dialogo->nombre_fichero) != NULL)
                strbas_destroy(datos_dialogo->nombre_fichero);
            
            *(datos_dialogo->nombre_fichero) = strbas_copyString(gtk_file_selection_get_filename(file_selection));
        }
        
        cierra_dialogo = i_acepta_dialogo_si_puede(datos_dialogo);
    }
    else
        cierra_dialogo = true;
    
    if (cierra_dialogo == true)
        gtkkitdlg_cierra_dialogo(datos_dialogo->widget_dialogo);
}
   
//---------------------------------------------------------

static void i_acepta_dialogo_fichero(GtkWidget *widget, gpointer data)
{
    i_acepta_cancela_dialogo(data, true);
}

//---------------------------------------------------------

static void i_cancelar_dialogo_fichero(GtkWidget *widget, gpointer data)
{
    i_acepta_cancela_dialogo(data, false);
}

//---------------------------------------------------------

GtkWidget *gtkbtnacp_boton_aceptar(const char *texto, struct gtkbtnacp_t *datos_dialogo)
{
    return gtkboton_crea_boton(texto, GTK_SIGNAL_FUNC(i_acepta_dialogo_fichero), datos_dialogo);
}

//---------------------------------------------------------

GtkWidget *gtkbtnacp_boton_cancelar(const char *texto, struct gtkbtnacp_t *datos_dialogo)
{
    return gtkboton_crea_boton(texto, GTK_SIGNAL_FUNC(i_cancelar_dialogo_fichero), datos_dialogo);
}

//---------------------------------------------------------

void gtkbtnacp_click_aceptar(GtkWidget *boton, struct gtkbtnacp_t *datos_dialogo)
{
    gtk_signal_connect(GTK_OBJECT(boton), "clicked", GTK_SIGNAL_FUNC(i_acepta_dialogo_fichero), datos_dialogo);
}

//---------------------------------------------------------

void gtkbtnacp_click_cancelar(GtkWidget *boton, struct gtkbtnacp_t *datos_dialogo)
{
    gtk_signal_connect(GTK_OBJECT(boton), "clicked", GTK_SIGNAL_FUNC(i_cancelar_dialogo_fichero), datos_dialogo);
}

//---------------------------------------------------------

static void i_prepara_dialogo_aceptar_cancelar(
                    GtkWidget *dialogo,
                    const char *titulo,
                    GtkWidget **gtk_area_trabajo,
                    GtkWidget **gtk_boton_aceptar, 
                    GtkWidget **gtk_boton_cancelar)
{
    assert_no_null(gtk_area_trabajo);
    assert_no_null(*gtk_area_trabajo);

    assert_no_null(gtk_boton_aceptar);

    assert_no_null(gtk_boton_cancelar);
    assert_no_null(*gtk_boton_cancelar);
    
    gtk_window_set_title(GTK_WINDOW(dialogo), titulo);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialogo)->vbox), *gtk_area_trabajo, TRUE, TRUE, 0);
    
    if (*gtk_boton_aceptar != NULL)
    {
        gtk_widget_show(*gtk_boton_aceptar);
        gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialogo)->action_area), *gtk_boton_aceptar, TRUE, TRUE, 0);
    }
    
    gtk_widget_show(*gtk_boton_cancelar);
    gtk_box_pack_end(GTK_BOX(GTK_DIALOG(dialogo)->action_area), *gtk_boton_cancelar, TRUE, TRUE, 0);

    *gtk_area_trabajo = NULL; 
    *gtk_boton_aceptar = NULL;
    *gtk_boton_cancelar = NULL;
}

//---------------------------------------------------------

bool gtkbtnacp_lanza_dialogo_con_botones(
                    GtkWidget *dialogo, const char *titulo, 
                    GtkWidget **gtk_area_trabajo, 
                    GtkWidget **gtk_boton_aceptar, GtkWidget **gtk_boton_cancelar,
                    struct gtkbtnacp_t **datos_dialogo)
{
    bool aceptado;
    
    assert_no_null(datos_dialogo);
    assert_no_null(*datos_dialogo);
    assert_no_null(dialogo);

    i_prepara_dialogo_aceptar_cancelar(
                    dialogo, titulo,
                    gtk_area_trabajo, gtk_boton_aceptar, gtk_boton_cancelar);
    
    (*datos_dialogo)->widget_dialogo = dialogo;
   
    gtkkitdlg_lanza_dialogo(dialogo);
    
    aceptado = (*datos_dialogo)->aceptado;
    
    (*datos_dialogo)->widget_dialogo = NULL;
    
    i_destruye(datos_dialogo);
    
    return aceptado;
}

//---------------------------------------------------------

bool gtkbtnacp_lanza_dialogo(GtkWidget *dialogo, struct gtkbtnacp_t **datos_dialogo)
{
    bool aceptado;
    
    assert_no_null(datos_dialogo);
    assert_no_null(*datos_dialogo);
    assert_no_null(dialogo);

    (*datos_dialogo)->widget_dialogo = dialogo;
   
    gtkkitdlg_lanza_dialogo(dialogo);
    
    aceptado = (*datos_dialogo)->aceptado;
    
    (*datos_dialogo)->widget_dialogo = NULL;
    
    i_destruye(datos_dialogo);
    
    return aceptado;
}
