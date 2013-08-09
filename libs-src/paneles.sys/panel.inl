// Definici�n de paneles

void panel_destruye(struct panel_t **panel);

struct panel_t *panel_crea_componente(struct componente_t **componente);

struct item_implementacion_t *panel_implementacion(struct panel_t *panel, struct marcogrf_t *marco, struct item_implementacion_dialogo_t *dialogo_so);

bool panel_se_puede_finalizar(struct panel_t *panel, const struct marcogrf_t *marco, char **mensaje_error);

void panel_aceptar(struct panel_t *panel, const struct marcogrf_t *marco);

ArrString *panel_titulos_componentes(struct panel_t *panel);

ArrString *panel_datos_componentes(struct panel_t *panel);

void panel_set_dialogo_padre(struct panel_t *panel, struct dialogo_t *dialogo);

bool panel_hay_receptores_tecla(struct panel_t *panel);

void panel_sendEventKey(struct panel_t *panel, struct EvtKey_t *evtKey);
