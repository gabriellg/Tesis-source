//mrpanel.hxx

#ifndef _MRPANEL_HXX
#define _MRPANEL_HXX

enum mrpanel_tipo_disposicion_t
{
	MRPANEL_HORIZONTAL,
	MRPANEL_VERTICAL,
	MRPANEL_GRID
};

typedef struct item_implementacion_t *(*mrpanel_FPtr_crea_panel)(enum mrpanel_tipo_disposicion_t tipo_disposicion, unsigned long num_filas, unsigned long num_columnas, bool homogeneus, unsigned long pixelsSpacing);
typedef void (*mrpanel_FPtr_append_panel)(struct item_implementacion_t *panel,  enum mrpanel_tipo_disposicion_t tipo_disposicion, unsigned long ind_fila, unsigned long ind_columna, bool expand, bool fill, struct item_implementacion_t *panel_hijo);
typedef void (*mrpanel_FPtr_append_componente)(struct item_implementacion_t *panel, enum mrpanel_tipo_disposicion_t tipo_disposicion, unsigned long ind_fila, unsigned long ind_columna, bool expand, bool fill, struct item_implementacion_t *componente_hijo);
typedef void (*mrpanel_FPtr_remove_panel)(struct item_implementacion_t *panel, struct item_implementacion_t *panel_hijo);
typedef void (*mrpanel_FPtr_remove_componente)(struct item_implementacion_t *panel, struct item_implementacion_t *componente_hijo);

#endif
