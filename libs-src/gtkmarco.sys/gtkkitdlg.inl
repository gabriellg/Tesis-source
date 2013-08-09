// Utilizadades para lanzar un di�logo

#include "gtkinc.inl"

void gtkkitdlg_cierra_dialogo(GtkWidget *dialogo);
                   
void gtkkitdlg_cierra_aplicacion(GtkWidget *dialogo);
                   
void gtkkitdlg_lanza_dialogo(GtkWidget *dialogo);

GtkWidget *gtkkitdlg_crea_aplicacion(void);

void gtkkitdlg_lanza_aplicacion(
                    GtkWidget *aplicacion,
                    const char *titulo,
                    struct item_implementacion_menubar_t **menubar, 
                    struct item_implementacion_toolbar_t **toolbar,
                    struct item_implementacion_t **area_aplicacion);

