//Definición de shader para sombras.

CYBOOL cyshadow_soporta_shaders_sombras(void);

CONSTRUCTOR(struct cyshadow_t *, cyshadow_crea_shadow, (void));

void cyshadow_destruye(struct cyshadow_t **shadow);

void cyshadow_inicia_generacion_sombra(struct cyshadow_t *shadow);

void cyshadow_fin_generacion_sombra(struct cyshadow_t *shadow);

void cyshadow_inicia_dibujo_sombra(struct cyshadow_t *shadow, unsigned long unidad_texturas_materiales, CYBOOL activa_luz_1, CYBOOL activa_luz_2);

void cyshadow_fin_dibujo_sombra(struct cyshadow_t *shadow);

void cyshadow_se_aplican_materiales(struct cyshadow_t *shadow, CYBOOL se_aplica_materiales);

void cyshadow_dibuja_mapa_sombras_debug(const struct cyshadow_t *shadow, long x_viewport, long y_viewport, unsigned long ancho_viewport, unsigned long alto_viewport);
    
void cyshadow_es_de_dia_o_de_noche(struct cyshadow_t *shadow, CYBOOL es_de_dia);
