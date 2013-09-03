// Gtk etiqueta.

#include "gtketiqueta.inl"

#include "gtktools.inl"

#include "asrtbas.h"
#include "marcogrf.hxx"
#include "gtkinc.inl"

//---------------------------------------------------------

static GtkJustification i_gtk_justificacion(enum marcogrf_justificacion_horizontal_t tipo_justificacion)
{
	GtkJustification gtkJustification;
	
	switch (tipo_justificacion) 
	{
		case MARCOGRF_CENTRO:
			
			gtkJustification = GTK_JUSTIFY_CENTER;
			break;
			
		case MARCOGRF_IZQUIERDA:
			
			gtkJustification = GTK_JUSTIFY_LEFT;
			break;
			
		case MARCOGRF_DERECHA:
			
			gtkJustification = GTK_JUSTIFY_RIGHT;
			break;
			
		default_error();
	}
	
	return gtkJustification;
}

//---------------------------------------------------------

static void i_gtk_alignment_factors(
							enum marcogrf_justificacion_horizontal_t tipo_justificacion, 
							gfloat *factorX, gfloat *factorY)
{
	assert_no_null(factorX);
	assert_no_null(factorY);
	
	switch (tipo_justificacion) 
	{
		case MARCOGRF_CENTRO:
			
			*factorX = 0.5;
			break;
			
		case MARCOGRF_IZQUIERDA:
			
			*factorX = 0.;
			break;
			
		case MARCOGRF_DERECHA:
			
			*factorX = 1.;
			break;
			
		default_error();
	}
	
	*factorY = .5;
}

//---------------------------------------------------------

struct item_implementacion_t *gtketiqueta_crea_implementacion(
                    struct item_implementacion_dialogo_t *dlg_padre, 
                    const char *texto,
                    enum marcogrf_justificacion_horizontal_t tipo_justificacion)
{
	GtkWidget *label;
	GtkJustification gtkJustificacion;
	gfloat factorX, factorY;

    assert_no_null(dlg_padre);
    assert_no_null(texto);
    
	label = gtk_label_new(texto);

	i_gtk_alignment_factors(tipo_justificacion, &factorX, &factorY);
	gtk_misc_set_alignment(GTK_MISC(label), factorX, factorY);
	
	gtkJustificacion = i_gtk_justificacion(tipo_justificacion);
	gtk_label_set_justify(GTK_LABEL(label), gtkJustificacion);
	
	return (struct item_implementacion_t *)label;
}

//---------------------------------------------------------

void gtketiqueta_set_texto(struct item_implementacion_t *item_etiqueta, const char *texto)
{
	GtkWidget *label;
	
	label = (GtkWidget *)item_etiqueta;
	gtk_label_set_text(GTK_LABEL(label), texto);
	gtktools_update();
}

//---------------------------------------------------------

struct item_implementacion_t *gtketiqueta_crea_implementacion_separador(
                    struct item_implementacion_dialogo_t *dlg_padre, 
                    enum marcogrf_tipo_separador_t tipo_separador)
{
    GtkWidget *separador;
    
    assert_no_null(dlg_padre);
    
    switch(tipo_separador)
    {
        case MARCOGRF_SEPARADOR_HORIZONTAL:
            
            separador = gtk_hseparator_new();

            break;
        
        case MARCOGRF_SEPARADOR_VERTICAL:
            
            separador = gtk_vseparator_new();
            break;
        
        default_error();
    }
    
    return (struct item_implementacion_t *)separador;
}
