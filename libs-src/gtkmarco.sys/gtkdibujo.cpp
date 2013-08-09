// Componente que nos da el area de dibujo.

#include "gtkdibujo.inl"

#include "gtkgraf.inl"
#include "gtkinc.inl"

#include "graflimits.h"
#include "grafico.h"
#include "asrtbas.h"
#include "memory.h"

#include "callback.syh"
#include "evtmouse.syh"

#include "CGraphicsGL.hpp"

struct i_area_dibujo_t
{
	struct callback_t *callback_iniciaGrafico;
	struct callback_t *callback_definePerspectiva;
	struct callback_t *callback_dibuja;
    struct callback_t *funcion_raton;
    class IGraphics *graphics;
    
    bool hasPreviousPosition;
    double xPrevious, yPrevious;
};

//---------------------------------------------------------

static struct i_area_dibujo_t *i_crea_dibujo(
						struct callback_t *callback_iniciaGrafico, 
						struct callback_t *callback_definePerspectiva,
						struct callback_t *callback_dibuja,
                        struct callback_t *funcion_raton,
                        bool hasPreviousPosition,
                        double xPrevious, double yPrevious,
                        class IGraphics **graphics)
{
    struct i_area_dibujo_t *area_dibujo;
        
    area_dibujo = MALLOC(i_area_dibujo_t);
    
	area_dibujo->callback_iniciaGrafico = callback_iniciaGrafico;  
	area_dibujo->callback_definePerspectiva = callback_definePerspectiva; 
	area_dibujo->callback_dibuja = callback_dibuja; 
    area_dibujo->funcion_raton = funcion_raton;
    
    area_dibujo->graphics = ASSIGN_PP_NO_NULL(graphics, class IGraphics);
    
    area_dibujo->hasPreviousPosition = hasPreviousPosition;
    area_dibujo->xPrevious = xPrevious;  
    area_dibujo->yPrevious = yPrevious;  
    
    return area_dibujo;
}

//---------------------------------------------------------

static void i_destruye_area_dibujo(struct i_area_dibujo_t **area_dibujo)
{
    assert_no_null(area_dibujo);
    assert_no_null(*area_dibujo);
    
    delete (*area_dibujo)->graphics;
    
    FREE_T(area_dibujo, struct i_area_dibujo_t);
}

//---------------------------------------------------------

static void i_coordenadas_pantalla(GtkWidget *widget, int *ox, int *oy, int *ex, int *ey)
{
    assert_no_null(ox);
    assert_no_null(oy);
    assert_no_null(ex);
    assert_no_null(ey);
    
    *ox = 0;
    *oy = 0; 
    *ex = widget->allocation.width - 2;
    *ey = widget->allocation.height - 2;
}

//---------------------------------------------------------

static void i_pon_marco(GtkWidget *widget)
{
	GdkGC *gc;
	GdkWindow *ventana;
    int ox, oy, ex, ey;

	assert_no_null(widget);
	assert_no_null(widget->style);
	assert_no_null(widget->style->fg_gc);
    
	gc = widget->style->fg_gc[GTK_STATE_NORMAL];
	ventana = widget->window;
    
    i_coordenadas_pantalla(widget, &ox, &oy, &ex, &ey);
    
    return gdk_draw_rectangle(ventana, gc, FALSE, ox, oy, (ex - ox), (ey - oy));
}

//---------------------------------------------------------------

static void i_initFont(unsigned long fontListBase, unsigned long *fontHeightPixel)
{
	PangoFontDescription *font_desc;
	PangoFont *font;
	unsigned long fontHeightPixelLoc;
	
	assert_no_null(fontHeightPixel);
	
  	font_desc = pango_font_description_from_string("Courier 12");

  	font = gdk_gl_font_use_pango_font(font_desc, 0, 128, fontListBase);

  	if (font != NULL)
  	{
  	    PangoFontMetrics *font_metrics;

		font_metrics = pango_font_get_metrics (font, NULL);

		fontHeightPixelLoc = pango_font_metrics_get_ascent (font_metrics) +
						pango_font_metrics_get_descent (font_metrics);
		fontHeightPixelLoc = PANGO_PIXELS(fontHeightPixelLoc);
	
		pango_font_metrics_unref (font_metrics);
  	}
  	else
  	  fontHeightPixelLoc = 12;

    *fontHeightPixel = fontHeightPixelLoc;

    pango_font_description_free(font_desc);
}


//---------------------------------------------------------

static void i_iniciaDibujo(GtkWidget *widget, gpointer data)
{
	GdkGLContext *glcontext;
  	GdkGLDrawable *gldrawable;
    struct i_area_dibujo_t *area_dibujo;
    bool resp;
    class CGraphicsGL *graphicsGL;

	area_dibujo = (struct i_area_dibujo_t *)data;

  	glcontext = gtk_widget_get_gl_context(widget);
  	gldrawable = gtk_widget_get_gl_drawable(widget);

  	resp = gdk_gl_drawable_gl_begin(gldrawable, glcontext);
  	assert(resp == TRUE);
  	
  	graphicsGL = (class CGraphicsGL *)area_dibujo->graphics;
  	graphicsGL->initFont3D(i_initFont);
  	
  	callback_ejecuta_con_dato_externo(area_dibujo->callback_iniciaGrafico, class IGraphics, area_dibujo->graphics);
  	
  	gdk_gl_drawable_gl_end(gldrawable);
}

//---------------------------------------------------------

static gboolean i_configuraEvento(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
	GdkGLContext *glcontext;
  	GdkGLDrawable *gldrawable;
  	unsigned long w, h;
    struct i_area_dibujo_t *area_dibujo;
    bool resp;

	area_dibujo = (struct i_area_dibujo_t *)data;

  	glcontext = gtk_widget_get_gl_context(widget);
  	gldrawable = gtk_widget_get_gl_drawable(widget);

  	w = widget->allocation.width;
  	h = widget->allocation.height;

  	resp = gdk_gl_drawable_gl_begin(gldrawable, glcontext);
  	assert(resp == TRUE);
  
  	area_dibujo->graphics->viewport(0, 0, w, h);
  	
	callback_ejecuta_con_dato_externo(area_dibujo->callback_definePerspectiva, class IGraphics, area_dibujo->graphics);

  	gdk_gl_drawable_gl_end(gldrawable);

  	return TRUE;
}

//---------------------------------------------------------

static gboolean i_redibujar(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
	GdkGLContext *glcontext;
  	GdkGLDrawable *gldrawable;
    struct i_area_dibujo_t *area_dibujo;
    bool resp;
 	unsigned long w, h;
 
	area_dibujo = (struct i_area_dibujo_t *)data;

  	glcontext = gtk_widget_get_gl_context(widget);
  	gldrawable = gtk_widget_get_gl_drawable(widget);

  	resp = gdk_gl_drawable_gl_begin(gldrawable, glcontext);
  	assert(resp == TRUE);
    
  	w = widget->allocation.width;
  	h = widget->allocation.height;
  	
  	area_dibujo->graphics->viewport(0, 0, w, h);
 	
	callback_ejecuta_con_dato_externo(area_dibujo->callback_definePerspectiva, class IGraphics, area_dibujo->graphics);
  	
	area_dibujo->graphics->clear();

	callback_ejecuta_con_dato_externo(area_dibujo->callback_dibuja, class IGraphics, area_dibujo->graphics);

	gdk_gl_drawable_swap_buffers(gldrawable);
	
	gdk_gl_drawable_gl_end(gldrawable);
	
	i_pon_marco(widget);
	
	return TRUE;
}

//---------------------------------------------------------

static void i_envia_evento_raton_y_destruye(struct i_area_dibujo_t *area_dibujo, struct EvtMouse_t **evento)
{
    assert_no_null(evento);
    assert_no_null(area_dibujo);
    
    callback_ejecuta_con_dato_externo(area_dibujo->funcion_raton, struct EvtMouse_t, *evento);
        
    evtmouse_destroy(evento);
}

//---------------------------------------------------------

static bool i_hasButton(GdkEventMotion *event, enum evtmouse_button_t *button)
{
    if ((event->state & GDK_BUTTON1_MASK) != 0)
    {
        *button = EVTMOUSE_LEFT;
        return true;
    }
    else if ((event->state & GDK_BUTTON3_MASK) != 0)
    {
        *button = EVTMOUSE_RIGHT;
        return true;
    }
    else if ((event->state & GDK_BUTTON2_MASK) != 0) 
    {
        *button = EVTMOUSE_MIDDLE;
        return true;
    }
    else
        return false;
}

//---------------------------------------------------------

static void i_get_modificadores(guint state, bool *isShiftPulsed, bool *isControlPulsed)
{
    bool isShiftPulsed_loc, isControlPulsed_loc;

    assert_no_null(isShiftPulsed);
    assert_no_null(isControlPulsed);

    if ((state & GDK_SHIFT_MASK) != 0)
        isShiftPulsed_loc = true;
    else
        isShiftPulsed_loc = false;

    if ((state & GDK_CONTROL_MASK) != 0)
        isControlPulsed_loc = true;
    else
        isControlPulsed_loc = false;

    *isShiftPulsed = isShiftPulsed_loc;
    *isControlPulsed = isControlPulsed_loc;
}

//---------------------------------------------------------

static gboolean i_evento_raton(GtkWidget *widget, GdkEventMotion *event, gpointer user_data)
{
    enum evtmouse_button_t button;
    struct i_area_dibujo_t *area_dibujo;
    struct EvtMouse_t *evtmouse;
    bool isShiftPulsed, isControlPulsed;

    assert_no_null(event);
    
	area_dibujo = (struct i_area_dibujo_t *)user_data;

	i_get_modificadores(event->state, &isShiftPulsed, &isControlPulsed);

    if (i_hasButton(event, &button) == true)
    {
        evtmouse = evtmouse_createPulse(
                        button,
                        area_dibujo->hasPreviousPosition,
                        isShiftPulsed, isControlPulsed,
                        area_dibujo->xPrevious, area_dibujo->yPrevious,
                        event->x, event->y, 
                        area_dibujo->graphics);
                        
        area_dibujo->hasPreviousPosition = true;
        area_dibujo->xPrevious = event->x; 
        area_dibujo->yPrevious = event->y;
    }
    else
        evtmouse = evtmouse_createMove(isShiftPulsed, isControlPulsed, event->x, event->y, area_dibujo->graphics);
    
    i_envia_evento_raton_y_destruye(area_dibujo, &evtmouse);
    
    area_dibujo->hasPreviousPosition = true;
    area_dibujo->xPrevious = event->x; 
    area_dibujo->yPrevious = event->y;

    return TRUE;
}

//---------------------------------------------------------

static bool i_se_procesa_wheel_raton(GdkScrollDirection direction, enum evtmouse_type_t *typeMouse)
{
    bool se_procesa_wheel;
    enum evtmouse_type_t typeMouse_loc;

    assert_no_null(typeMouse);

    switch(direction)
    {
        case GDK_SCROLL_UP:

            se_procesa_wheel = true;
            typeMouse_loc = EVTMOUSE_WHEEL_UP;
            break;

        case GDK_SCROLL_DOWN:

            se_procesa_wheel = true;
            typeMouse_loc = EVTMOUSE_WHEEL_DOWN;
            break;

        case GDK_SCROLL_LEFT:
        case GDK_SCROLL_RIGHT:

            se_procesa_wheel = false;
            break;

        default_error();
    }

    *typeMouse = typeMouse_loc;

    return se_procesa_wheel;
}

//---------------------------------------------------------

static gboolean i_event_scroll(GtkWidget *widget, GdkEventScroll *event, gpointer user_data)
{
    gboolean processed;
    struct i_area_dibujo_t *area_dibujo;
    enum evtmouse_type_t typeMouse;
    bool isShiftPulsed, isControlPulsed;

    assert_no_null(event);
    area_dibujo = (struct i_area_dibujo_t *)user_data;

    i_get_modificadores(event->state, &isShiftPulsed, &isControlPulsed);

    if (i_se_procesa_wheel_raton(event->direction, &typeMouse) == true)
    {
        struct EvtMouse_t *evtMouse;

        evtMouse = evtmouse_createWheel(typeMouse, isShiftPulsed, isControlPulsed, area_dibujo->graphics);
        i_envia_evento_raton_y_destruye(area_dibujo, &evtMouse);

        processed = TRUE;
    }
    else
        processed = FALSE;

    return processed;
}

//---------------------------------------------------------

static gboolean i_entra_en_area(GtkWidget *widget, GdkEventCrossing *event, gpointer user_data)
{
    struct i_area_dibujo_t *area_dibujo;
    struct EvtMouse_t *evtMouse;
    
    area_dibujo = (struct i_area_dibujo_t *)user_data;
    
    evtMouse = evtmouse_createIn(area_dibujo->graphics);
    
    i_envia_evento_raton_y_destruye(area_dibujo, &evtMouse);
    
    return TRUE;
}

//---------------------------------------------------------

static gboolean i_sale_de_area(GtkWidget *widget, GdkEventCrossing *event, gpointer user_data)
{
    struct i_area_dibujo_t *area_dibujo;
    struct EvtMouse_t *evtmouse;
    
    area_dibujo = (struct i_area_dibujo_t *)user_data;
    
    evtmouse = evtmouse_createOut(area_dibujo->graphics);
    
    i_envia_evento_raton_y_destruye(area_dibujo, &evtmouse);
    
    return TRUE;
}

//---------------------------------------------------------

static void i_destruir_componente(GtkWidget *widget, gpointer *data)
{
    struct i_area_dibujo_t *area_dibujo;

    area_dibujo = (struct i_area_dibujo_t *)data;
    i_destruye_area_dibujo(&area_dibujo);
}

//---------------------------------------------------------

static enum evtmouse_button_t i_type_button(GdkEventButton *event)
{
    if (event->button == 1)
    {
        return EVTMOUSE_LEFT;
    }
    else if (event->button == 3)
    {
        return EVTMOUSE_RIGHT;
    }
    else 
    {
        assert(event->button == 2);
        return EVTMOUSE_MIDDLE;
    }
}

//---------------------------------------------------------

static gboolean i_pulsa_boton(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
    struct i_area_dibujo_t *area_dibujo;
    struct EvtMouse_t *evtMouse;
    enum evtmouse_button_t button;
    bool isShiftPulsed, isControlPulsed;
        
    assert_no_null(event);
    
	area_dibujo = (struct i_area_dibujo_t *)data;
    button = i_type_button(event);
    
    i_get_modificadores(event->state, &isShiftPulsed, &isControlPulsed);

    evtMouse = evtmouse_createPulse(
                        button,
                        false,
                        isShiftPulsed, isControlPulsed,
                        0., 0.,
                        event->x, event->y, 
                        area_dibujo->graphics);
                        
    i_envia_evento_raton_y_destruye(area_dibujo, &evtMouse);
    
    return TRUE;
}

//---------------------------------------------------------

static gboolean i_suelta_boton(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
    struct i_area_dibujo_t *area_dibujo;
    struct EvtMouse_t *evtMouse;
    enum evtmouse_button_t button;
    bool isShiftPulsed, isControlPulsed;
        
    assert_no_null(event);
    
    area_dibujo = (struct i_area_dibujo_t *)data;
    button = i_type_button(event);

    i_get_modificadores(event->state, &isShiftPulsed, &isControlPulsed);

    evtMouse = evtmouse_createRelease(
                        button,
                        isShiftPulsed, isControlPulsed,
                        event->x, event->y, 
                        area_dibujo->graphics);
                        
    i_envia_evento_raton_y_destruye(area_dibujo, &evtMouse);

    area_dibujo->hasPreviousPosition = false;
    area_dibujo->xPrevious = 0; 
    area_dibujo->yPrevious = 0;

    return TRUE;
}

//---------------------------------------------------------

static GdkGLConfig *i_createConfigGL(void)
{
	GdkGLConfig *glconfig;

  	glconfig = gdk_gl_config_new_by_mode(
  					(GdkGLConfigMode)(
  							GDK_GL_MODE_RGB    |
							GDK_GL_MODE_DEPTH  |
							GDK_GL_MODE_DOUBLE));

	assert_no_null(glconfig);
	
	return glconfig;
}

//---------------------------------------------------------

struct item_implementacion_t *gtkdibujo_crea(
                        struct item_implementacion_dialogo_t *dlg_padre, 
						struct callback_t *callback_iniciaGrafico, 
						struct callback_t *callback_definePerspectiva,
						struct callback_t *callback_dibuja,
                        struct callback_t *funcion_raton,
                        unsigned long ancho, unsigned long alto)
{
	GtkWidget *area_dibujo;
    struct i_area_dibujo_t *datos_area_dibujo;
    class IGraphics *graphics3d;
    GdkGLConfig *configGL;
    bool hasPreviousPosition;
    double xPrevious, yPrevious;
    
	area_dibujo = gtk_drawing_area_new();
	
	configGL = i_createConfigGL();
  	gtk_widget_set_gl_capability(area_dibujo,
				configGL,
				NULL, TRUE,
				GDK_GL_RGBA_TYPE);

    hasPreviousPosition = false;
    xPrevious = 0.; 
    yPrevious = 0.;
    
    graphics3d = new CGraphicsGL();

    datos_area_dibujo = i_crea_dibujo(
    					callback_iniciaGrafico,
    					callback_definePerspectiva,
    					callback_dibuja,
    					funcion_raton,
                        hasPreviousPosition,
                        xPrevious, yPrevious,
    					&graphics3d);
	
	gtk_drawing_area_size(GTK_DRAWING_AREA(area_dibujo), (gint)ancho, (gint)alto);
    
    gtk_widget_add_events(area_dibujo, GDK_ENTER_NOTIFY_MASK);
    gtk_widget_add_events(area_dibujo, GDK_LEAVE_NOTIFY_MASK);
    gtk_widget_add_events(area_dibujo, GDK_POINTER_MOTION_MASK);
    gtk_widget_add_events(area_dibujo, GDK_POINTER_MOTION_HINT_MASK);
    gtk_widget_add_events(area_dibujo, GDK_BUTTON_PRESS_MASK);
    gtk_widget_add_events(area_dibujo, GDK_SCROLL_MASK);
    gtk_widget_add_events(area_dibujo, GDK_BUTTON_RELEASE_MASK);
    
  	gtk_signal_connect_after(GTK_OBJECT(area_dibujo), "realize", GTK_SIGNAL_FUNC(i_iniciaDibujo), (gpointer)datos_area_dibujo);
	gtk_signal_connect(GTK_OBJECT(area_dibujo), "expose_event", GTK_SIGNAL_FUNC(i_redibujar), (gpointer)datos_area_dibujo);
  	gtk_signal_connect(GTK_OBJECT(area_dibujo), "configure_event", GTK_SIGNAL_FUNC(i_configuraEvento), (gpointer)datos_area_dibujo);
	
    gtk_signal_connect(GTK_OBJECT(area_dibujo), "motion_notify_event", GTK_SIGNAL_FUNC(i_evento_raton), (gpointer)datos_area_dibujo);
    gtk_signal_connect(GTK_OBJECT(area_dibujo), "scroll-event", GTK_SIGNAL_FUNC(i_event_scroll), (gpointer)datos_area_dibujo);
    gtk_signal_connect(GTK_OBJECT(area_dibujo), "enter_notify_event", GTK_SIGNAL_FUNC(i_entra_en_area), (gpointer)datos_area_dibujo);
    gtk_signal_connect(GTK_OBJECT(area_dibujo), "leave_notify_event", GTK_SIGNAL_FUNC(i_sale_de_area), (gpointer)datos_area_dibujo);
    gtk_signal_connect(GTK_OBJECT(area_dibujo), "destroy", GTK_SIGNAL_FUNC(i_destruir_componente), (gpointer)datos_area_dibujo);
    gtk_signal_connect(GTK_OBJECT(area_dibujo), "button_press_event", GTK_SIGNAL_FUNC(i_pulsa_boton), (gpointer)datos_area_dibujo);
    gtk_signal_connect(GTK_OBJECT(area_dibujo), "button_release_event", GTK_SIGNAL_FUNC(i_suelta_boton), (gpointer)datos_area_dibujo);

    GTK_WIDGET_SET_FLAGS(area_dibujo, GTK_CAN_FOCUS);
                                                 
    gtk_object_set_data(GTK_OBJECT(area_dibujo), "datos", datos_area_dibujo);
	
	return (struct item_implementacion_t *)area_dibujo;
}

//---------------------------------------------------------

class IGraphics *gtkdibujo_getGraphics(struct item_implementacion_t *area_dibujo)
{
    GtkWidget *widget;
    struct i_area_dibujo_t *datos_area_dibujo;
    
    widget = (GtkWidget *)area_dibujo;
    
    datos_area_dibujo = (struct i_area_dibujo_t *)gtk_object_get_data(GTK_OBJECT(widget), "datos");
    assert_no_null(datos_area_dibujo);

    return datos_area_dibujo->graphics;  
}

//---------------------------------------------------------

void gtkdibujo_getDimension(struct item_implementacion_t *area_dibujo, unsigned long *width, unsigned long *height)
{
    GtkWidget *widget;
    
    assert_no_null(width);
    assert_no_null(height);
    
    widget = (GtkWidget *)area_dibujo;
    assert_no_null(area_dibujo);

    *width = widget->allocation.width;
  	*height = widget->allocation.height;
}

//---------------------------------------------------------

void gtkdibujo_redraw(struct item_implementacion_t *area_dibujo)
{
    GtkWidget *widget;
    
    widget = (GtkWidget *)area_dibujo;
    gdk_window_invalidate_rect(widget->window, &widget->allocation, FALSE);
    gdk_window_process_updates(widget->window, FALSE);
}
