// Boton de aceptar y cancelar.

#include "marcogrf.hxx"
#include "gtkinc.inl"

struct gtkbtnacp_t *gtkbtnacp_crea_datos_dialogo(
                    struct dialogo_t *dialogo,
                    marcogrf_FPtr_se_puede_aceptar func_se_puede_aceptar,
                    marcogrf_FPtr_aceptar func_aceptar);

struct gtkbtnacp_t *gtkbtnacp_crea_datos_dialogo_para_file(char **nombre_fichero);

GtkWidget *gtkbtnacp_boton_aceptar(const char *texto, struct gtkbtnacp_t *datos_dialogo);

GtkWidget *gtkbtnacp_boton_cancelar(const char *texto, struct gtkbtnacp_t *datos_dialogo);

void gtkbtnacp_click_aceptar(GtkWidget *boton, struct gtkbtnacp_t *datos_dialogo);

void gtkbtnacp_click_cancelar(GtkWidget *boton, struct gtkbtnacp_t *datos_dialogo);

bool gtkbtnacp_lanza_dialogo_con_botones(
                    GtkWidget *dialogo, const char *titulo, 
                    GtkWidget **gtk_area_trabajo, 
                    GtkWidget **gtk_boton_aceptar, GtkWidget **gtk_boton_cancelar,
                    struct gtkbtnacp_t **datos_dialogo);

bool gtkbtnacp_lanza_dialogo(GtkWidget *dialogo, struct gtkbtnacp_t **datos_dialogo);
