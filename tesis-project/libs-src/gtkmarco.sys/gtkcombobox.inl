// Combobox para gtk

#include "marcogrf.hxx"

struct item_implementacion_t *gtkcombobox_crea_implementacion(
                        struct item_implementacion_dialogo_t *dlg_padre, 
                        struct combobox_t *combobox,
                        marcogrf_ha_cambiado_opcion func_ha_cambiado,
                        const ArrString *texto_opciones,
                        unsigned long ind_valor);

unsigned long gtkcombobox_get_opcion_combox(struct item_implementacion_t *combobox);
