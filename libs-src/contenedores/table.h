// Define una tabla que para identificar cada elemento tenemos una cadena.

typedef void (*table_FPtr_destroyData)(struct table_item_data_t **data);
#define TABLE_VERIFY_FUNC_DESTROY(typeData, function) (void)((void (*)(typeData **))function == function)

struct Table_t *table_create(void);

void table_destroy(struct Table_t **table);

struct table_item_data_t *table_getPrivate(const struct Table_t *table, const char *key);
#define table_get(table, key, typeData) (typeData *)table_getPrivate(table, key)

void table_setPrivate(struct Table_t *table, const char *key, struct table_item_data_t **data, table_FPtr_destroyData func_destroy);

#define table_set(table, key, data, func_destroy, typeData)\
(\
	(void)((typeData **)data == data),\
	TABLE_VERIFY_FUNC_DESTROY(typeData, func_destroy),\
	table_setPrivate(table, key, (struct table_item_data_t **)data, (table_FPtr_destroyData)func_destroy)\
)

bool table_hasKey(const struct Table_t *table, const char *key);
