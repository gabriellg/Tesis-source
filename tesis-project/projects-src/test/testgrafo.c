// Test para los grafos.

#include "graph.h"

#include "asrtbas.h"
#include "memory.h"
#include "array.h"

struct i_vertice_t
{
	unsigned long idVertice;
};

//-----------------------------------------------------------------------

static struct i_vertice_t *i_creaVertice(unsigned long idVertice)
{
	struct i_vertice_t *vertice;
	
	vertice = MALLOC(struct i_vertice_t);
	
	vertice->idVertice = idVertice;
	
	return vertice;
}

//-----------------------------------------------------------------------

static void i_destruyeVertice(struct i_vertice_t **vertice)
{
	FREE_T(vertice, struct i_vertice_t);
}

//-----------------------------------------------------------------------

static bool i_sonIguales(const struct i_vertice_t *v1, const struct i_vertice_t *v2)
{
	assert_no_null(v1);
	assert_no_null(v2);	
	
	if (v1->idVertice == v2->idVertice)
		return true;
	else
		return false;
}

//-----------------------------------------------------------------------

static void i_creaVertices(
				struct i_vertice_t **v1, 
				struct i_vertice_t **v2, 
				struct i_vertice_t **v3, 
				struct i_vertice_t **v4, 
				struct i_vertice_t **v5)
{
	assert_no_null(v1);
	assert_no_null(v2);
	assert_no_null(v3);
	assert_no_null(v4);
	assert_no_null(v5);
	
	*v1 = i_creaVertice(1);
	*v2 = i_creaVertice(2);
	*v3 = i_creaVertice(3);
	*v4 = i_creaVertice(4);	
	*v5 = i_creaVertice(5);	
}

//-----------------------------------------------------------------------

static Graph(i_vertice_t) *i_creaGrafo(void)
{
	Graph(i_vertice_t) *grafo;
	struct i_vertice_t *v1, *v2, *v3, *v4, *v5;
	
	i_creaVertices(&v1, &v2, &v3, &v4, &v5);
	
	grafo =	graph_create(i_vertice_t, i_sonIguales, i_destruyeVertice);
	
	graph_appendVertex(i_vertice_t, grafo, v1);
	graph_appendVertex(i_vertice_t, grafo, v2);
	graph_appendVertex(i_vertice_t, grafo, v3);
	graph_appendVertex(i_vertice_t, grafo, v4);
	graph_appendVertex(i_vertice_t, grafo, v5);
	
	graph_appendEdge(i_vertice_t, grafo, v1, v3);
	graph_appendEdge(i_vertice_t, grafo, v2, v3);
	graph_appendEdge(i_vertice_t, grafo, v3, v4);
	graph_appendEdge(i_vertice_t, grafo, v4, v5);
	
	return grafo;
}

//-----------------------------------------------------------------------

static void i_compruebaVertices(
						const ArrTipo(i_vertice_t) *vertices,
						unsigned long numVertices,
						const unsigned long *ids_vertices)
{
	unsigned long i, num;
	
	num = array_numElementos(i_vertice_t, vertices);
	assert(num == numVertices);
	
	if (numVertices > 0)
		assert_no_null(ids_vertices);
	
	for (i = 0; i < num; i++)
	{
		const struct i_vertice_t *v;
		
		v = array_Get(i_vertice_t, vertices, i);
		assert_no_null(v);
		assert(v->idVertice == ids_vertices[i]);
	}
}

//-----------------------------------------------------------------------

static void i_compruebaPadres(
						const Graph(i_vertice_t) *grafo, 
						const struct i_vertice_t *v, 
						unsigned long numPadres, 
						const unsigned long *ids_padres)
{
	ArrTipo(i_vertice_t) *padres;

	padres = graph_fathers(i_vertice_t, grafo, v);
	i_compruebaVertices(padres, numPadres, ids_padres);
	array_Destruye(i_vertice_t, &padres, NULL);
}

//-----------------------------------------------------------------------

static void	i_test_padres(const Graph(i_vertice_t) *grafo)
{
	struct i_vertice_t *v1, *v2, *v3, *v4, *v5;
	const unsigned long idsPadresV3[2] = {1, 2};
	const unsigned long idsPadresV4[1] = {3};
	const unsigned long idsPadresV5[1] = {4};
	unsigned long numPadres;

	i_creaVertices(&v1, &v2, &v3, &v4, &v5);
	
	i_compruebaPadres(grafo, v1, 0, NULL);
	i_compruebaPadres(grafo, v2, 0, NULL);
	
	numPadres = sizeof(idsPadresV3) / sizeof(idsPadresV3[0]);
	i_compruebaPadres(grafo, v3, numPadres, idsPadresV3);

	numPadres = sizeof(idsPadresV4) / sizeof(idsPadresV4[0]);
	i_compruebaPadres(grafo, v4, numPadres, idsPadresV4);

	numPadres = sizeof(idsPadresV5) / sizeof(idsPadresV5[0]);
	i_compruebaPadres(grafo, v5, numPadres, idsPadresV5);

	i_destruyeVertice(&v1);
	i_destruyeVertice(&v2);
	i_destruyeVertice(&v3);
	i_destruyeVertice(&v4);
	i_destruyeVertice(&v5);
}

//-----------------------------------------------------------------------

static void i_compruebaHijos(
						const Graph(i_vertice_t) *grafo, 
						const struct i_vertice_t *v, 
						unsigned long numHijos, 
						const unsigned long *idsHijos)
{
	ArrTipo(i_vertice_t) *hijos;

	hijos = graph_sons(i_vertice_t, grafo, v);
	
	i_compruebaVertices(hijos, numHijos, idsHijos);
	array_Destruye(i_vertice_t, &hijos, NULL);
}

//-----------------------------------------------------------------------

static void i_test_hijos(const Graph(i_vertice_t) *grafo)
{
	struct i_vertice_t *v1, *v2, *v3, *v4, *v5;
	const unsigned long idsHijosV1[1] = {3};
	const unsigned long idsHijosV2[1] = {3};
	const unsigned long idsHijosV3[1] = {4};
	const unsigned long idsHijosV4[1] = {5};
	unsigned long numHijos;

	i_creaVertices(&v1, &v2, &v3, &v4, &v5);
	
	i_compruebaHijos(grafo, v5, 0, NULL);
	
	numHijos = sizeof(idsHijosV1) / sizeof(idsHijosV1[0]);
	i_compruebaHijos(grafo, v1, numHijos, idsHijosV1);

	numHijos = sizeof(idsHijosV2) / sizeof(idsHijosV2[0]);
	i_compruebaHijos(grafo, v2, numHijos, idsHijosV2);

	numHijos = sizeof(idsHijosV3) / sizeof(idsHijosV3[0]);
	i_compruebaHijos(grafo, v3, numHijos, idsHijosV3);

	numHijos = sizeof(idsHijosV4) / sizeof(idsHijosV4[0]);
	i_compruebaHijos(grafo, v4, numHijos, idsHijosV4);

	i_destruyeVertice(&v1);
	i_destruyeVertice(&v2);
	i_destruyeVertice(&v3);
	i_destruyeVertice(&v4);
	i_destruyeVertice(&v5);
}

//-----------------------------------------------------------------------

int main(int argc, char *argv[])
{
	Graph(i_vertice_t) *grafo;
	
	grafo = i_creaGrafo();
	
	i_test_padres(grafo);
	i_test_hijos(grafo);
	
	graph_destroy(i_vertice_t, &grafo);
	
	return 0;
}
