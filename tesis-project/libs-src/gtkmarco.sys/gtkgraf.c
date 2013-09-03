// Dispositivo que dibuja para GTK.

#include "gtkgraf.inl"

#include "grafico.syh"

#include "idcolor.h"
#include "memory.h"
#include "asrtbas.h"

struct i_dispositivo_t
{
	GdkGC *gc;
	GdkWindow *ventana;
    
    GdkGC **colores;
    unsigned long ind_color_actual;
};

//---------------------------------------------------------

static GdkColor *i_crea_color(long red, long green, long blue)
{
    GdkColor *color;
    
    color = MALLOC(GdkColor);
    
    color->pixel = 0;
    color->red = red;
    color->green = green;
    color->blue = blue;

    gdk_color_alloc(gdk_colormap_get_system(), color);
    
    return color;
}

//---------------------------------------------------------

static GdkGC *i_crea_brocha(GdkWindow *ventana, long red, long green, long blue)
{
    GdkColor *color;
    GdkGC *brocha;
    
    color = i_crea_color(red, green, blue);
    
    brocha = gdk_gc_new(ventana);
    
    gdk_gc_set_foreground(brocha, color);
    
    return brocha;
}

//---------------------------------------------------------

static GdkGC **i_crea_colores(GdkWindow *ventana)
{
    GdkGC **colores;
    
    colores = CALLOC(IDCOLOR_NUM_COLORES, GdkGC *);
    
    colores[IDCOLOR_NEGRO] = i_crea_brocha(ventana, 0, 0, 0);
    colores[IDCOLOR_BLANCO] = i_crea_brocha(ventana, 0xffff, 0xffff, 0xffff);
    colores[IDCOLOR_AZUL] = i_crea_brocha(ventana, 0, 0, 0xffff);
    colores[IDCOLOR_ROJO] = i_crea_brocha(ventana, 0xffff, 0, 0);
    colores[IDCOLOR_VERDE] = i_crea_brocha(ventana, 0, 0xffff, 0);
    
    return colores;
}

//---------------------------------------------------------

static struct i_dispositivo_t *i_crea(GdkWindow *ventana, GdkGC *gc, GdkGC **colores, unsigned long ind_color_actual)
{
	struct i_dispositivo_t *dispositivo;
		
	dispositivo = MALLOC(struct i_dispositivo_t);
	
	dispositivo->ventana = ventana;
	dispositivo->gc = gc;
    dispositivo->colores = colores;
    dispositivo->ind_color_actual = ind_color_actual;
	
	return dispositivo;
}

//---------------------------------------------------------

static void i_destruye(struct i_dispositivo_t **dispositivo)
{
	assert_no_null(dispositivo);
    assert_no_null(*dispositivo);
    
    FREE_T(&(*dispositivo)->colores, GdkGC *);
	FREE_T(dispositivo, struct i_dispositivo_t);
}

//---------------------------------------------------------

static GdkGC *i_obten_brocha(GdkGC **colores, unsigned long ind_color)
{
    assert(ind_color < IDCOLOR_NUM_COLORES);
    return colores[ind_color];
}

//---------------------------------------------------------

static void i_dibuja_linea(struct i_dispositivo_t *dispositivo, double x1, double y1, double x2, double y2)
{
	gint gtk_x1, gtk_y1, gtk_x2, gtk_y2;
    GdkGC *brocha;
	
	assert_no_null(dispositivo);
	
	gtk_x1 = (gint)x1;
	gtk_y1 = (gint)y1;
	gtk_x2 = (gint)x2;
	gtk_y2 = (gint)y2;
    
    brocha = i_obten_brocha(dispositivo->colores, dispositivo->ind_color_actual);
	gdk_draw_line(dispositivo->ventana, brocha, gtk_x1, gtk_y1, gtk_x2, gtk_y2);
}

//---------------------------------------------------------

static void i_calcula_puntos_rectangulo(
						double x1, double y1, double x2, double y2,
						gint *gtk_x1, gint *gtk_y1, gint *gtk_ancho, gint *gtk_alto)
{
	double x_min, y_min, x_max, y_max;

	x_min = MIN(x1, x2);
	x_max = MAX(x1, x2);
	
	y_min = MIN(y1, y2);
	y_max = MAX(y1, y2);
	
	assert_no_null(gtk_x1);	assert_no_null(gtk_y1);
	assert_no_null(gtk_ancho);
	assert_no_null(gtk_alto);
	
	*gtk_x1 = (gint)x_min;	*gtk_y1 = (gint)y_min;
	*gtk_ancho = (gint)(x_max - x_min);
	*gtk_alto = (gint)(y_max - y_min);
}

//---------------------------------------------------------

static void i_dibuja_rectangulo(struct i_dispositivo_t *dispositivo, double x1, double y1, double x2, double y2)
{
	gint gtk_x1, gtk_y1, gtk_ancho, gtk_alto, gtk_filled;
    GdkGC *brocha;
	
	assert_no_null(dispositivo);
	
	i_calcula_puntos_rectangulo(x1, y1, x2, y2, &gtk_x1, &gtk_y1, &gtk_ancho, &gtk_alto);
	gtk_filled = FALSE;
    brocha = i_obten_brocha(dispositivo->colores, dispositivo->ind_color_actual);
        gdk_draw_rectangle(dispositivo->ventana, brocha, gtk_filled, gtk_x1, gtk_y1, gtk_ancho, gtk_alto);
}

//---------------------------------------------------------

static void i_dibuja_rectangulo_relleno(struct i_dispositivo_t *dispositivo, double x1, double y1, double x2, double y2)
{
	gint gtk_x1, gtk_y1, gtk_ancho, gtk_alto, gtk_filled;
    GdkGC *brocha;
	
	assert_no_null(dispositivo);
	
	i_calcula_puntos_rectangulo(x1, y1, x2, y2, &gtk_x1, &gtk_y1, &gtk_ancho, &gtk_alto);
	gtk_filled = TRUE;
    brocha = i_obten_brocha(dispositivo->colores, dispositivo->ind_color_actual);
	gdk_draw_rectangle(dispositivo->ventana, brocha, gtk_filled, gtk_x1, gtk_y1, gtk_ancho, gtk_alto);
}

//---------------------------------------------------------

static void i_cambia_color(struct i_dispositivo_t *dispositivo, unsigned long ind_color)
{
    assert_no_null(dispositivo);
    dispositivo->ind_color_actual = ind_color;
}

//---------------------------------------------------------

struct grafico_t *gtkgraf_crea(GdkWindow *ventana, GdkGC *gc)
{
	struct i_dispositivo_t *dispositivo;
    unsigned long ind_color_actual;
    GdkGC **colores;
	
    colores = i_crea_colores(ventana);
    ind_color_actual = 0;
	dispositivo = i_crea(ventana, gc, colores, ind_color_actual);

	return grafico_crea_dispositivo(
						i_dispositivo_t, &dispositivo,
						i_destruye, i_dibuja_linea, 
						i_dibuja_rectangulo,
						i_dibuja_rectangulo_relleno,  i_cambia_color);
}
