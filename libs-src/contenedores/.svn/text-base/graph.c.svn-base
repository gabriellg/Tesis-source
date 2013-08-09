// Implementación de grafos.

#include "graph.h"

#include "memory.h"
#include "array.h"
#include "asrtbas.h"
#include "strbas.h"
#include "arrgroup.h"

struct i_edge_t
{
	const struct ItemDataVertex_t *dataOrigin;
    const struct ItemDataVertex_t *dataDestiny;
};

struct graph_t
{
	ArrTipo(ItemDataVertex_t) *vertexs;
	ArrTipo(i_edge_t) *edges;
	
	graph_fptr_vertexEqual func_vertexEqual;
	graph_fptr_vertexDestroy func_vertexDestroy;
};

//---------------------------------------------------------

static struct i_edge_t *i_createEdge(
						const struct ItemDataVertex_t *dataOrigin,
    					const struct ItemDataVertex_t *dataDestiny)
{
	struct  i_edge_t *edge;
		
	assert_no_null(dataOrigin);
	assert_no_null(dataDestiny);
		
	edge = MALLOC(struct  i_edge_t);
		
	edge->dataOrigin = dataOrigin;
	edge->dataDestiny = dataDestiny;
		
	return edge;
}

//---------------------------------------------------------

static void i_destroyEdge(struct i_edge_t **edge)
{
	FREE_T(edge, struct i_edge_t);
}

//---------------------------------------------------------

static bool i_isFather(
						const struct i_edge_t *edge,
						const struct ItemDataVertex_t *vertex,
						graph_fptr_vertexEqual func_vertexEqual)
{
	assert_no_null(edge);
	assert_no_null(func_vertexEqual);
	
	return func_vertexEqual(edge->dataOrigin, vertex);
}

//---------------------------------------------------------

static bool i_isSon(
						const struct i_edge_t *edge, 
						const struct ItemDataVertex_t *vertex,
						graph_fptr_vertexEqual func_vertexEqual)
{
	assert_no_null(edge);
	assert_no_null(func_vertexEqual);
	
	return func_vertexEqual(edge->dataDestiny, vertex);
}

//---------------------------------------------------------

static struct graph_t *i_create(
						ArrTipo(ItemDataVertex_t) **vertexs,
						ArrTipo(i_edge_t) **edges,
						graph_fptr_vertexEqual func_vertexEqual,
						graph_fptr_vertexDestroy func_vertexDestroy)
{
	struct graph_t *graph;
		
	graph = MALLOC(struct graph_t);
	
	graph->func_vertexEqual = func_vertexEqual;
	graph->func_vertexDestroy =  func_vertexDestroy;
	
	graph->vertexs = ASSIGN_PP_NO_NULL(vertexs, ArrTipo(ItemDataVertex_t));
	graph->edges = ASSIGN_PP_NO_NULL(edges, ArrTipo(i_edge_t));
	
	return graph;
};

//---------------------------------------------------------

struct graph_t *graph_createPrivate(
						graph_fptr_vertexEqual funcVertexEqual, 
						graph_fptr_vertexDestroy funcVertexDestroy)
{
	ArrTipo(ItemDataVertex_t) *vertexs;
	ArrTipo(i_edge_t) *edges;

	vertexs = array_CreaPuntero(0, ItemDataVertex_t);
	edges = array_CreaPuntero(0, i_edge_t);
	
	return i_create(&vertexs, &edges, funcVertexEqual, funcVertexDestroy);
}

//---------------------------------------------------------

void graph_destroyPrivate(struct graph_t **graph)
{
	assert_no_null(graph);
	assert_no_null(*graph);
	
	array_Destruye(ItemDataVertex_t, &(*graph)->vertexs, (*graph)->func_vertexDestroy);
	array_Destruye(i_edge_t, &(*graph)->edges, i_destroyEdge);
	
	FREE_T(graph, struct graph_t);
}

//---------------------------------------------------------

void graph_appendVetexPrivate(struct graph_t *graph, struct ItemDataVertex_t *dataVertex)
{
	assert_no_null(graph);
	assert_no_null(dataVertex);
	assert(array_BuscarUnicoPuntero(ItemDataVertex_t, graph->vertexs, dataVertex, NULL) == false);
	assert(array_Buscar(ItemDataVertex_t, graph->vertexs, ItemDataVertex_t, dataVertex, graph->func_vertexEqual, NULL) == false);
	
	array_Anyadir(ItemDataVertex_t, graph->vertexs, dataVertex);
}

//---------------------------------------------------------

void graph_appendEdgePrivate(struct graph_t *graph, const struct ItemDataVertex_t *vertexOrigin, const struct ItemDataVertex_t *vertexDestiny)
{
	struct i_edge_t *edge;

	assert_no_null(graph);
	assert(array_BuscarUnicoPuntero(ItemDataVertex_t, graph->vertexs, vertexOrigin, NULL) == true);
	assert(array_BuscarUnicoPuntero(ItemDataVertex_t, graph->vertexs, vertexDestiny, NULL) == true);
	
	edge = i_createEdge(vertexOrigin, vertexDestiny);
	array_Anyadir(i_edge_t, graph->edges, edge);
}

//---------------------------------------------------------

static const ArrTipo(ItemDataVertex_t) *i_fathersVertex(
					const ArrTipo(i_edge_t) *edges, 
					const struct ItemDataVertex_t *vertex,
					graph_fptr_vertexEqual func_vertexEqual)
{
	const ArrTipo(ItemDataVertex_t) *fathers;
    unsigned long i, numEdges;
    
    fathers = array_CreaPunteroConst(0, ItemDataVertex_t);
    numEdges = array_numElementos(i_edge_t, edges);
    
    for (i = 0; i < numEdges; i++)
    {
        struct i_edge_t *edge;
        
        edge = array_Get(i_edge_t, edges, i);
        
        if (i_isSon(edge, vertex, func_vertexEqual) == true)
	        array_AnyadirConst(ItemDataVertex_t, fathers, edge->dataOrigin);
    }
    
    return fathers;
}

//---------------------------------------------------------

static const ArrTipo(ItemDataVertex_t) *i_sonsVertex(
					const ArrTipo(i_edge_t) *edges, 
					const struct ItemDataVertex_t *vertex,
					graph_fptr_vertexEqual func_vertexEqual)
{
	const ArrTipo(ItemDataVertex_t) *sons;
    unsigned long i, numEdges;
    
    sons = array_CreaPunteroConst(0, ItemDataVertex_t);
    numEdges = array_numElementos(i_edge_t, edges);
    
    for (i = 0; i < numEdges; i++)
    {
        struct i_edge_t *edge;
        
        edge = array_Get(i_edge_t, edges, i);
        
        if (i_isFather(edge, vertex, func_vertexEqual) == true)
	        array_AnyadirConst(ItemDataVertex_t, sons, edge->dataDestiny);
    }
    
    return sons;
}

//---------------------------------------------------------

static struct Array_t *i_transformationType(
				    const char *typeVertex,
					const ArrTipo(ItemDataVertex_t) *itemVertexs)
{
	struct Array_t *dataVertexs;
    unsigned long i, numVertexs;
    
    numVertexs = array_numElementos(ItemDataVertex_t, itemVertexs);
	dataVertexs = array_CreaPrivada(typeVertex, numVertexs);
    
    for (i = 0; i < numVertexs; i++)
    {
        struct ItemDataVertex_t *itemVertex;
        
        itemVertex = array_Get(ItemDataVertex_t, itemVertexs, i);
        array_PonerPrivada(typeVertex, dataVertexs, i, itemVertex);
    }
    
    return dataVertexs;
}

//---------------------------------------------------------

struct Array_t *graph_fathersPrivate(
					const struct graph_t *graph, 
					const struct ItemDataVertex_t *vertex,
					const char *type_vertex)
{
	struct Array_t *dataVertexFathers;
	const ArrTipo(ItemDataVertex_t) *itemFathers;
	
	assert_no_null(graph);
	assert(array_Buscar(ItemDataVertex_t, graph->vertexs, ItemDataVertex_t, vertex, graph->func_vertexEqual, NULL) == true);
	
	itemFathers = i_fathersVertex(graph->edges, vertex, graph->func_vertexEqual);
	dataVertexFathers = i_transformationType(type_vertex, itemFathers);
	
	array_DestruyeConst(ItemDataVertex_t, &itemFathers);
	
	return dataVertexFathers;
}

//---------------------------------------------------------

struct Array_t *graph_sonsPrivate(
					const struct graph_t *graph, 
					const struct ItemDataVertex_t *vertex,
					const char *type_vertex)
{
	struct Array_t *dataVertexSons;
	const ArrTipo(ItemDataVertex_t) *itemSons;
	
	assert_no_null(graph);
	assert(array_Buscar(ItemDataVertex_t, graph->vertexs, ItemDataVertex_t, vertex, graph->func_vertexEqual, NULL) == true);
	
	itemSons = i_sonsVertex(graph->edges, vertex, graph->func_vertexEqual);
	dataVertexSons = i_transformationType(type_vertex, itemSons);
	
	array_DestruyeConst(ItemDataVertex_t, &itemSons);
	
	return dataVertexSons;
}

//---------------------------------------------------------

bool graph_FindVertexPrivate(
						const struct graph_t *graph,
						const char *typeVertex, 
						struct item_data_find_t *item_data_find, 
						graph_fptr_vertexFind func_vertexFind,
						struct ItemDataVertex_t **vertexFind)
{
	bool existe;
	unsigned long ind_pos;

	existe = array_Buscar(
						ItemDataVertex_t, graph->vertexs, 
						struct item_data_find_t, item_data_find, 
						func_vertexFind, 
						&ind_pos);
						
	if (existe == true && vertexFind != NULL)
		*vertexFind = array_Get(ItemDataVertex_t, graph->vertexs, ind_pos);
						
	return existe;
} 

//---------------------------------------------------------

static bool i_hasFathers(
						const ArrTipo(i_edge_t) *edges, 
						const struct ItemDataVertex_t *vertex,
						graph_fptr_vertexEqual func_vertexEqual)
{
	bool hasFathers;
	const ArrTipo(ItemDataVertex_t) *vertexFathers;
	
	vertexFathers = i_fathersVertex(edges, vertex, func_vertexEqual);
	
	if (array_numElementos(ItemDataVertex_t, vertexFathers) > 0)
		hasFathers = true;
	else
		hasFathers = false;
		
	array_DestruyeConst(ItemDataVertex_t, &vertexFathers);		
		
	return hasFathers;
}

//---------------------------------------------------------

static const ArrTipo(ItemDataVertex_t) *i_vertexsWithoutFathers(
							const char *type_vertex,
							ArrTipo(ItemDataVertex_t) *vertexs,
							ArrTipo(i_edge_t) *edges,
							graph_fptr_vertexEqual func_vertexEqual)
{
	const ArrTipo(ItemDataVertex_t) *vertexsWithoutFathers;
	unsigned long i, num;
	
	vertexsWithoutFathers = array_CreaPunteroConst(0, ItemDataVertex_t);
	num = array_numElementos(ItemDataVertex_t, vertexs);
	
	for (i = 0; i < num; i++) 
	{
		const struct ItemDataVertex_t *vertex;

		vertex = array_Get(ItemDataVertex_t, vertexs, i);
		
		if (i_hasFathers(edges, vertex, func_vertexEqual) == false)
			array_AnyadirConst(ItemDataVertex_t, vertexsWithoutFathers, vertex);
	}
	
	return vertexsWithoutFathers;
}

//---------------------------------------------------------

static bool i_existSlopes(const ArrTipo(ItemDataVertex_t) *slopes)
{
	if (array_numElementos(ItemDataVertex_t, slopes) > 0)
		return true;
	else
		return false;
}

//---------------------------------------------------------

static bool i_allVertexsVisited(
						const ArrTipo(ItemDataVertex_t) *vertexs, 
						const ArrTipo(ItemDataVertex_t) *visited)
{
    unsigned long i, num;
    
    num = array_numElementos(ItemDataVertex_t, vertexs);

    for (i = 0; i < num; i++)
    {
        struct ItemDataVertex_t *vertex;
        
        vertex = array_Get(ItemDataVertex_t, vertexs, i);
        if (array_Buscar(ItemDataVertex_t, visited, struct ItemDataVertex_t, vertex, NULL, NULL) == false)
            return false;
    }
    
    return true;
}

//---------------------------------------------------------

static const ArrTipo(ItemDataVertex_t) *i_vertexWithAllFathersVisited(
							const ArrTipo(ItemDataVertex_t) *vertexs,
							const ArrTipo(i_edge_t) *edges,
							const ArrTipo(ItemDataVertex_t) *visited,
							graph_fptr_vertexEqual func_vertexEqual)
{
    const ArrTipo(ItemDataVertex_t) *vertexWithAllFathersVisited;
    unsigned long i, num;
    
    vertexWithAllFathersVisited = array_CreaPunteroConst(0, ItemDataVertex_t);
    num = array_numElementos(ItemDataVertex_t, vertexs);

    for (i = 0; i < num; i++)
    {
        const struct ItemDataVertex_t *itemVertex;
        const ArrTipo(ItemDataVertex_t) *fathers;
        
        itemVertex = array_Get(ItemDataVertex_t, vertexs, i); 

        fathers = i_fathersVertex(edges, itemVertex, func_vertexEqual);
        
        if (i_allVertexsVisited(fathers, visited) == true)
        	array_AnyadirConst(ItemDataVertex_t, vertexWithAllFathersVisited, itemVertex);
        	
        array_DestruyeConst(ItemDataVertex_t, &fathers);
    }
    
    return vertexWithAllFathersVisited;
}
 
//---------------------------------------------------------

static void i_appendNotExist(
						const ArrTipo(ItemDataVertex_t) *vertexs, 
						const ArrTipo(ItemDataVertex_t) *vertexNotRepeated)
{
	unsigned long i, num;
	
	num = array_numElementos(ItemDataVertex_t, vertexs);

	for (i = 0; i < num; i++)
	{
		struct ItemDataVertex_t *vertex;
		
		vertex = array_Get(ItemDataVertex_t, vertexs, i);
		
		if (array_Buscar(ItemDataVertex_t, vertexNotRepeated, struct ItemDataVertex_t, vertex, NULL, NULL) == false)
			array_Anyadir(ItemDataVertex_t, vertexNotRepeated, vertex);
	}
}    

//---------------------------------------------------------

static const ArrTipo(ItemDataVertex_t) *i_sonsNodeLevelSlopes(
						const ArrTipo(i_edge_t) *edges,
						const ArrTipo(ItemDataVertex_t) *levelSlopes,
						const ArrTipo(ItemDataVertex_t) *visited,
						graph_fptr_vertexEqual func_vertexEqual)
{
    const ArrTipo(ItemDataVertex_t) *sonsLevelSlopes;
    unsigned long i, num;
    
    sonsLevelSlopes = array_CreaPunteroConst(0, ItemDataVertex_t);
    num = array_numElementos(ItemDataVertex_t, levelSlopes);

    for (i = 0; i < num; i++)
    {
        struct ItemDataVertex_t *itemVertex;
        const ArrTipo(ItemDataVertex_t) *sonsVertex, *sonsWithAllFathersVisited;
        
        itemVertex = array_Get(ItemDataVertex_t, levelSlopes, i);
        
        assert(array_Buscar(ItemDataVertex_t, visited, struct ItemDataVertex_t, itemVertex, NULL, NULL) == false);
        array_Anyadir(ItemDataVertex_t, visited, itemVertex);
        
        sonsVertex = i_sonsVertex(edges, itemVertex, func_vertexEqual);
        sonsWithAllFathersVisited = i_vertexWithAllFathersVisited(sonsVertex, edges, visited, func_vertexEqual);

		i_appendNotExist(sonsWithAllFathersVisited, sonsLevelSlopes);

		array_DestruyeConst(ItemDataVertex_t, &sonsVertex);
		array_DestruyeConst(ItemDataVertex_t, &sonsWithAllFathersVisited);
    }
        
    return sonsLevelSlopes;
}

//---------------------------------------------------------

struct ArrGroup_t *graph_levelsGroupsPrivate(struct graph_t *graph, const char *typeVertex)
{
	const ArrTipo(ItemDataVertex_t) *visited, *levelSlopes;
	struct ArrGroup_t *arrGroupLevels;
	
	assert_no_null(graph);
	
	arrGroupLevels = arrgroup_createEmptyPrivate(typeVertex); 
	
	visited = array_CreaPunteroConst(0, ItemDataVertex_t);
	
	levelSlopes = i_vertexsWithoutFathers(typeVertex, graph->vertexs, graph->edges, graph->func_vertexEqual);
	assert(array_numElementos(ItemDataVertex_t, levelSlopes) > 0);

	while (i_existSlopes(levelSlopes) == true)
	{
        const ArrTipo(ItemDataVertex_t) *sonsLevel;
        struct Array_t *vertexsLevel;
        
        sonsLevel = i_sonsNodeLevelSlopes(
						graph->edges,
						levelSlopes,
						visited,
						graph->func_vertexEqual);

        vertexsLevel = i_transformationType(typeVertex, levelSlopes);
        arrgroup_appendGroupPrivate(typeVertex, arrGroupLevels, &vertexsLevel);
        
        array_DestruyeConst(ItemDataVertex_t, &levelSlopes);
        levelSlopes = sonsLevel;
	}
	
	array_DestruyeConst(ItemDataVertex_t, &levelSlopes);
	array_DestruyeConst(ItemDataVertex_t, &visited);
	
	return arrGroupLevels;
}
