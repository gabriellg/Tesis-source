// Define una tabla que para identificar cada elemento tenemos una cadena.

#include "table.h"

#include "asrtbas.h"
#include "memory.h"
#include "array.h"
#include "strbas.h"

struct i_node_t
{
	char *key;
	
	struct table_item_data_t *data;
	table_FPtr_destroyData func_destroy;
};

struct Table_t
{
	ArrTipo(i_node_t) *nodes;
};

//---------------------------------------------------------

static struct i_node_t *i_createNode(
						const char *key,
						struct table_item_data_t **data,
						table_FPtr_destroyData func_destroy)
{
	struct i_node_t *node;
	
	node = MALLOC(struct i_node_t);
	
	node->key = strbas_copyString(key);
	node->data = ASSIGN_PP_NO_NULL(data, struct table_item_data_t);
	node->func_destroy = func_destroy;
	
	return node;
}

//---------------------------------------------------------

static void i_destroyNode(struct i_node_t **node)
{
	assert_no_null(node);
	assert_no_null(*node);

	strbas_destroy(&(*node)->key);
	
	if ((*node)->func_destroy != NULL)
		(*node)->func_destroy(&(*node)->data);
	
	FREE_T(node, struct i_node_t);
}

//---------------------------------------------------------

static bool i_isKey(const struct i_node_t *node, const char *key)
{
	assert_no_null(node);
	return strbas_equalsStrings(node->key, key);
}

//---------------------------------------------------------

static struct Table_t *i_create(ArrTipo(i_node_t) **nodes)
{
	struct Table_t *table;
	
	table = MALLOC(struct Table_t);
	
	table->nodes = ASSIGN_PP_NO_NULL(nodes, ArrTipo(i_node_t));
	
	return table;
}

//---------------------------------------------------------

struct Table_t *table_create(void)
{
	ArrTipo(i_node_t) *nodes;
	
	nodes = array_CreaPuntero(0, i_node_t);
	
	return i_create(&nodes);
}

//---------------------------------------------------------

void table_destroy(struct Table_t **table)
{
	assert_no_null(table);
	assert_no_null(*table);
	
	array_Destruye(i_node_t, &(*table)->nodes, i_destroyNode);
	
	FREE_T(table, struct Table_t);
}

//---------------------------------------------------------

struct table_item_data_t *table_getPrivate(const struct Table_t *table, const char *key)
{
	unsigned long ind;
	bool isNodeFound;
	struct i_node_t *node;
	
	assert_no_null(table);
	
	isNodeFound = array_BuscarUnica(i_node_t, table->nodes, char, key, i_isKey, &ind);
	assert(isNodeFound == true);

	node = array_Get(i_node_t, table->nodes, ind);
	assert_no_null(node);
	
	return node->data;
}

//---------------------------------------------------------

void table_setPrivate(
					struct Table_t *table, const char *key, 
					struct table_item_data_t **data,
					table_FPtr_destroyData func_destroy)
{
	unsigned long ind;
	bool isNodeFound;
	struct i_node_t *node;
	
	assert_no_null(table);
	
	isNodeFound = array_BuscarUnica(i_node_t, table->nodes, char, key, i_isKey, &ind);
	
	if (isNodeFound == true)
	{
		node = array_Get(i_node_t, table->nodes, ind);
		
		if (node->func_destroy != NULL)
			node->func_destroy(&node->data);
			
		node->data = ASSIGN_PP(data, struct table_item_data_t);
		node->func_destroy = func_destroy;
	}
	else
	{
		node = i_createNode(key, data, func_destroy);
		array_Anyadir(i_node_t, table->nodes, node);
	}
}

//---------------------------------------------------------

bool table_hasKey(const struct Table_t *table, const char *key)
{
	assert_no_null(table);
	return array_BuscarUnica(i_node_t, table->nodes, char, key, i_isKey, NULL);
}
