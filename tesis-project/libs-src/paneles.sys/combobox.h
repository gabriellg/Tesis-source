// Definicion de combobox
	
typedef void (*combobox_ha_cambiado_opcion)(struct item_datos_cambio_t *datos_cambio, unsigned long ind_opcion);
#define DECLARAR_FUNC_HA_CAMBIADO(func, tipo) (void)((void (*)(struct tipo *, unsigned long))func == func)


struct panel_t *combobox_crea(const char *titulo, unsigned long *valor, struct combobox_t **combobox);

#define combobox_crea_enum(titulo, tipo_enum, valor, combobox)\
    (\
        assert(sizeof(enum tipo_enum) == sizeof(unsigned long)),\
        combobox_crea(titulo, (unsigned long *)valor, combobox)\
    )

void combobox_anade_opcion(struct combobox_t *combobox, unsigned long id_valor, const char *texto_opcion);

void combobox_nousar_set_ha_cambiado_opcion(struct combobox_t *combobox, struct item_datos_cambio_t *datos_cambio, combobox_ha_cambiado_opcion func_ha_cambiado_opcion);

#define combobox_set_ha_cambiado_opcion(combobox, tipo, datos_cambio, func_ha_cambiado_opcion)\
    (\
        (void)((struct tipo *)datos_cambio == datos_cambio),\
        DECLARAR_FUNC_HA_CAMBIADO(func_ha_cambiado_opcion, tipo),\
        combobox_nousar_set_ha_cambiado_opcion(combobox, (struct item_datos_cambio_t *)datos_cambio, (combobox_ha_cambiado_opcion)func_ha_cambiado_opcion)\
    )
