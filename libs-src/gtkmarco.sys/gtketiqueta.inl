// Gtk etiqueta.

#include "marcogrf.hxx"

struct item_implementacion_t *gtketiqueta_crea_implementacion(struct item_implementacion_dialogo_t *dlg_padre, const char *texto, enum marcogrf_justificacion_horizontal_t tipo_justificacion);

struct item_implementacion_t *gtketiqueta_crea_implementacion_separador(struct item_implementacion_dialogo_t *dlg_padre,  enum marcogrf_tipo_separador_t tipo_separador);

void gtketiqueta_set_texto(struct item_implementacion_t *item_etiqueta, const char *texto);
