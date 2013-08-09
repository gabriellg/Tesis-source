// Sistema de times para gtk.

#include "gtktime.inl"

#include "gtkinc.inl"

#include "asrtbas.h"
#include "memory.h"
#include "callback.syh"

struct gtktime_t
{
    guint id_time;
    unsigned long timeout_miliseconds;
    struct callback_t *callback;
};

//---------------------------------------------------------
    
static gboolean prv_timeout(gpointer data)
{
    struct callback_t *callback;
    
    assert_no_null(data);
    
    callback = (struct callback_t *)data;
    callback_ejecuta(callback);

    return TRUE;
}

//---------------------------------------------------------
    
static struct gtktime_t *i_create(
                        guint id_time,
                        unsigned long timeout_miliseconds,
                        struct callback_t **callback)
{
    struct gtktime_t *gtktime;
    
    gtktime = MALLOC(struct gtktime_t);
    
    gtktime->id_time = id_time;
    gtktime->timeout_miliseconds = timeout_miliseconds;  
    gtktime->callback = ASSIGN_PP_NO_NULL(callback, struct callback_t);
    
    return gtktime;
}

//---------------------------------------------------------
    
static void i_destroy(struct gtktime_t **gtktime)
{
    assert_no_null(gtktime);
    assert_no_null(*gtktime);    
    assert((*gtktime)->id_time == 0);
       
    callback_destruye(&(*gtktime)->callback);
    
    FREE_T(gtktime, struct gtktime_t);
}

//---------------------------------------------------------

static gboolean prv_lanzar_run(GtkWidget *widget, GdkEventAny *event, gpointer data)
{
    struct gtktime_t *gtktime;

    assert_no_null(data);
    gtktime = (struct gtktime_t *)data;
    
    gtktime->id_time = g_timeout_add(gtktime->timeout_miliseconds, prv_timeout, gtktime->callback);
    
    return TRUE;
}

//---------------------------------------------------------

static void prv_removeIdTime(guint *id_time)
{
	assert_no_null(id_time);
    assert(*id_time != 0);
    g_source_remove(*id_time);
    *id_time = 0;
}

//---------------------------------------------------------

static gboolean prv_lanzar_stop(GtkWidget *widget, GdkEventAny *event, gpointer data)
{
    struct gtktime_t *gtktime;

    assert_no_null(data);
    gtktime = (struct gtktime_t *)data;

    prv_removeIdTime(&gtktime->id_time);
    
    return TRUE;
}

//---------------------------------------------------------

static void prv_destroyTime(GtkWidget *widget, gpointer *data)
{
	struct gtktime_t *gtktime;

	gtktime = (struct gtktime_t *)data;

	if (gtktime->id_time != 0)
	    prv_removeIdTime(&gtktime->id_time);

	i_destroy(&gtktime);
}

//---------------------------------------------------------
    
void gtktime_setTime(struct item_implementacion_t *item_widget,
                        unsigned long timeout_miliseconds,
                        struct callback_t **callback)
{
    guint id_time;
    struct gtktime_t *gtktime;
    GtkWidget *gtk_componente;
    
    gtk_componente = (GtkWidget *)item_widget;
    
    assert_no_null(callback);
    assert_no_null(callback);
    
    id_time = 0;
    gtktime = i_create(id_time, timeout_miliseconds, callback);
    
    gtk_componente = (GtkWidget *)item_widget;
    
    g_signal_connect(
                G_OBJECT(gtk_componente),
                "map_event",
                G_CALLBACK(prv_lanzar_run),
                gtktime);

    g_signal_connect(
                G_OBJECT(gtk_componente),
                "unmap_event",
                G_CALLBACK(prv_lanzar_stop),
                gtktime);

    gtk_signal_connect(
                GTK_OBJECT(gtk_componente), "destroy", 
                GTK_SIGNAL_FUNC(prv_destroyTime), (gpointer)gtktime);
}
