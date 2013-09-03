// Implementación de grafos.

#include "graph.hxx"

typedef void (*graph_fptr_vertexDestroy)(struct ItemDataVertex_t **data);
#define VERIFY_FUNC_VERTEX_DESTROY(func, type) (void)((void (*)(struct type **))func == func)
	
typedef bool (*graph_fptr_vertexEqual)(const struct ItemDataVertex_t *data1,  const struct ItemDataVertex_t *data2);
#define VERIFY_FUNC_VERTEX_EQUAL(func, type) (void)((bool (*)(const struct type *, const struct type *))func == func)

typedef bool (*graph_fptr_vertexFind)(const struct ItemDataVertex_t *dataVertex, const struct item_data_find_t *item_data_find);
#define VERIFY_FUNC_FIND_VERTEX(func, typeVertex, typeDataFind) (void)((bool (*)(const struct typeVertex *, const typeDataFind *))func == func)
	
#define VERIFY_GRAPH(graph, type) (void)((Graph(type) *)graph == graph)
#define VERIFY_GRAPH_PP(graph, type) (void)((Graph(type) **)graph == graph)
#define VERIFY_TYPE(data, type) (void)((struct type *)data == data)
#define VERIFY_TYPE_CONST(data, type) (void)((const struct type *)data == data)

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct graph_t *graph_createPrivate(graph_fptr_vertexEqual func_vertexEqual, graph_fptr_vertexDestroy func_vertexDestroy);
	
void graph_destroyPrivate(struct graph_t **graph);

void graph_appendVetexPrivate(struct graph_t *graph, struct ItemDataVertex_t *dataVertex);

void graph_appendEdgePrivate(struct graph_t *graph, const struct ItemDataVertex_t *vertexOrigin, const struct ItemDataVertex_t *vertexDestiny);

struct Array_t *graph_fathersPrivate(const struct graph_t *graph, const struct ItemDataVertex_t *vertex, const char *type_vertex);

struct Array_t *graph_sonsPrivate(const struct graph_t *graph, const struct ItemDataVertex_t *vertex, const char *type_vertex);

bool graph_FindVertexPrivate(
						const struct graph_t *graph,
						const char *typeVertex, 
						struct item_data_find_t *item_data_find, 
						graph_fptr_vertexFind func_vertexFind,
						struct ItemDataVertex_t **vertexFind);

struct ArrGroup_t *graph_levelsGroupsPrivate(struct graph_t *graph, const char *typeVertex);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#define graph_create(type, funcVertexEqual, funcVertexDestroy)\
	(\
	VERIFY_FUNC_VERTEX_EQUAL(funcVertexEqual, type),\
	VERIFY_FUNC_VERTEX_DESTROY(funcVertexDestroy, type),\
	(Graph(type) *)graph_createPrivate(\
				(graph_fptr_vertexEqual)funcVertexEqual,\
				(graph_fptr_vertexDestroy)funcVertexDestroy)\
	)

#define graph_destroy(type, graph)\
	(\
	VERIFY_GRAPH_PP(graph, type),\
	graph_destroyPrivate((struct graph_t **)graph)\
	)

#define graph_appendVertex(type, graph, dataVertex)\
	(\
	VERIFY_GRAPH(graph, type),\
	VERIFY_TYPE(dataVertex, type),\
	graph_appendVetexPrivate((struct graph_t *)graph, (struct ItemDataVertex_t *)dataVertex)\
	)

#define graph_appendEdge(type, graph, dataVertexOrigin, dataVertexDestiny)\
	(\
	VERIFY_GRAPH(graph, type),\
	VERIFY_TYPE_CONST(dataVertexOrigin, type),\
	VERIFY_TYPE_CONST(dataVertexDestiny, type),\
	graph_appendEdgePrivate(\
								(struct graph_t *)graph,\
								( const struct ItemDataVertex_t *)dataVertexOrigin,\
								( const struct ItemDataVertex_t *)dataVertexDestiny)\
	)

#define graph_fathers(type, graph, vertex)\
	(\
	VERIFY_GRAPH(graph, type),\
	VERIFY_TYPE_CONST(vertex, type),\
	(ArrTipo(type)*)graph_fathersPrivate(\
						(const struct graph_t *)graph,\
						(const struct ItemDataVertex_t *)vertex,\
						#type)\
	)

#define graph_sons(type, graph, vertex)\
	(\
	VERIFY_GRAPH(graph, type),\
	VERIFY_TYPE_CONST(vertex, type),\
	(ArrTipo(type)*)graph_sonsPrivate(\
						(const struct graph_t *)graph,\
						(const struct ItemDataVertex_t *)vertex,\
						#type)\
	)

#define graph_findVertex(type, graph, typeDataFind, dataFind, func_findVertex, vertexFind)\
	(\
	(void)((const typeDataFind *)dataFind == dataFind),\
	(void)((struct type **)vertexFind == vertexFind),\
	VERIFY_GRAPH(graph, type),\
	VERIFY_FUNC_FIND_VERTEX(func_findVertex, type, typeDataFind),\
	graph_FindVertexPrivate(\
						(const struct graph_t *)graph,\
						#type,\
						(struct item_data_find_t *)dataFind,\
						(graph_fptr_vertexFind)func_findVertex,\
						(struct ItemDataVertex_t **)vertexFind)\
	)

#define graph_levelsGroups(type, graph)\
	(\
		VERIFY_GRAPH(graph, type),\
		(ArrGroup(type) *)graph_levelsGroupsPrivate((struct graph_t *)graph, #type)\
	)
