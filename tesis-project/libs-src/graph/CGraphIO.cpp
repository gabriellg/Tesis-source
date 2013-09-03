/* -----------------------------------------------------------------------------
*  CGraphIO ( CGraph Input / Output file system )
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include "CGraphIO.hpp"

#include <iostream>

#include "CGraph.hpp"
#include "IVertex.hpp"
#include "CString.hpp"
#include "CArrayRef.hpp"
#include "CArrayString.hpp"
#include "CFileName.hpp"
#include "CFileInBin.hpp"
#include "CFileInTxt.hpp"
#include "CFileOutBin.hpp"
#include "CFileOutTxt.hpp"
//#include "CXMLParser.hpp"
#include "asrtbas.h"
using namespace std;

//static unsigned char PRV_VERSION_FILE = 0;

//------------------------------------------------------------------------------
//
static void prv_print_terminals( const char* separator,
                                 const class CArrayRef<IVertex> *arrayTerminals )
{
    unsigned long numTerminals;

    numTerminals = arrayTerminals->size();

    cout << separator;

    if( numTerminals > 0 )
    {
        for( unsigned long i=0; i < numTerminals; ++i )
        {
            const IVertex *vertex;

            vertex = arrayTerminals->get( i );

            vertex->print();

            if( i < numTerminals - 1 )
                cout << ",";
        }
    }
    else
        cout << "---";
}

//------------------------------------------------------------------------------
//
static void prv_print_family( const char* separator,
                              const class CArrayRef<IVertex> *arrayFamily )
{
    unsigned long numFamily;

    numFamily = arrayFamily->size();

    cout << separator;

    if ( numFamily == 0 )
        cout << "---";
    else
    {
        for ( unsigned long i = 0; i < numFamily; ++i )
        {
            const IVertex *relative;

            relative = arrayFamily->get( i );

            cout << ">";
            relative->print();
        }
    }
}

//------------------------------------------------------------------------------
//
static void prv_print_vertexList( const class CGraph<IVertex> *graph )
{
    class CArrayRef<IVertex> *arrayVertex;
    unsigned long numVertex;

    arrayVertex = graph->getVertex();
    numVertex = arrayVertex->size();

    for ( unsigned long ind = 0; ind < numVertex; ++ind )
    {
        class CArrayRef<IVertex> *arrayParents, *arrayChildren;
        const IVertex *vertex;

        vertex = arrayVertex->get( ind );
        arrayParents = graph->getParents( vertex );
        arrayChildren = graph->getChildren( vertex );

        cout << "\n   Vertex(";
        vertex->print();
        cout << ") ";

        prv_print_family( " In", arrayParents );

        cout << "\t";
        prv_print_family( " Out", arrayChildren );

        delete arrayParents;
        delete arrayChildren;
    }

    delete arrayVertex;
}

//------------------------------------------------------------------------------
//
void PrvBaseGraphIO :: prv_print( const class CGraph<class IVertex> *graph )
{
    unsigned int numVertex = graph->numVertex();

    cout << "# Graph - Num.Vertex: " << numVertex;

    if ( numVertex > 0 )
    {
        class CArrayRef<IVertex> *arrayInitials, *arrayFinals;

        arrayInitials = graph->getInitials();
        arrayFinals = graph->getFinals();

        prv_print_terminals( "  - I: ", arrayInitials );
        prv_print_terminals( "  - F: ", arrayFinals );

        prv_print_vertexList( graph );

        delete arrayInitials;
        delete arrayFinals;
    }
    cout << endl << endl;
}

//------------------------------------------------------------------------------
//
class CGraph<IVertex>* PrvBaseGraphIO :: prv_readGRF(
                                class CFileInTxt *file,
                                CGraphIO_FPtr_readVertex func_readVertex )
{
    class CGraph<IVertex> *graph;
    class CArrayRef<IVertex> *arrayVertex;
    class CArrayString *arrayData;
    unsigned long numVertex;
    char c;

    assert_no_null( file );
    assert_no_null( func_readVertex );

    graph = new CGraph<IVertex>();
    arrayData = new CArrayString();

    numVertex = file->readULong();
    c = file->readChar();
    assert( c == '\n' );

    while( graph->numVertex() < numVertex )
    {
        class IVertex *vertex;
        char *str;
        char c;

        str = file->readString();

        assert_with_message( CString::size( str ) == numVertex,
                             "Incorrect file format" );

        c = file->readChar();
        assert( c == ' ' );

        vertex = func_readVertex( file );
        graph->addVertex( vertex );

        c = file->readChar();

        assert( c == '\n' || file->eof() == true );

        arrayData->add( str );

        CString::free( &str );
    }

    assert_with_message( numVertex = graph->numVertex(),
                         "The file has an incorrect format." );

    arrayVertex = graph->getVertex();

    for( unsigned int i = 0; i < numVertex; ++i)
    {
        const class IVertex *vertex1;
        const char *str;

        str = arrayData->get( i );
        vertex1 = arrayVertex->get( i );

        for (unsigned int j = 0; j < numVertex; ++j)
        {
            if ( str[j] == '1' )
            {
                const class IVertex *vertex2 = arrayVertex->get( j );
                graph->addEdge( vertex1, vertex2 );
            }
        }
    }

    delete arrayData;
    delete arrayVertex;

    return graph;
}


//------------------------------------------------------------------------------
//
class CGraph<IVertex>* PrvBaseGraphIO :: prv_readDOT(
                                class CFileInTxt *file,
                                CGraphIO_FPtr_readVertex func_readVertex )
{
    class CGraph<IVertex> *graph;

    assert_message( "not implemented" );

    assert_no_null( file );
    assert_no_null( func_readVertex );

    graph = new CGraph<IVertex>();


    return graph;
}

/*
//------------------------------------------------------------------------------
//
static string prv_readGXL_GetGraphLabel( const string &graphLabelTag,
                    const vector<XMLElement_t>::const_iterator &itGXL )
{
    if ( graphLabelTag.empty() )
        return "";

    map<string, string>::const_iterator mapIt;

    mapIt = (*itGXL).content[0].attributes.find( graphLabelTag );

    if ( mapIt == (*itGXL).content[0].attributes.end() )
    {
        cerr << "ERROR (GXL-graph): Wrong tag label '" << graphLabelTag << "'" << endl;
        cerr << "Possible labels: ";

        if ( (*itGXL).content[0].attributes.empty() )
            cerr << "-empty-";
        else {
            for (mapIt = (*itGXL).content[0].attributes.begin();
                 mapIt != (*itGXL).content[0].attributes.end(); ++mapIt)
            {
                if (mapIt != (*itGXL).content[0].attributes.begin())
                    cerr << ", ";
                cerr << mapIt->first;
            }
        }
        cerr << endl;
        exit(-1);
    }

    return mapIt->second;
}

//------------------------------------------------------------------------------
//
static void prv_readGXL_ReadVertex( const string &nodeLabelTag,
                                        const vector<XMLElement_t>::iterator &itGXL,
                                        vector<XMLElement_t>::iterator &it,
                                        CGraph **graph )
{
    map<string, string>::const_iterator mapIt;
    string currentNodeLabel;
    int pos = 0;
    bool isInt;
    int vertexID, auxVertexID;

    for ( it = (*itGXL).content[0].content.begin();
          it != (*itGXL).content[0].content.end(); ++it, ++pos)
    {
        if ((*it).name != "node")
            break; // Continue with the edges

        assert((*it).attributes.size() >= 1);

        mapIt = (*it).attributes.find("id");
        assert(mapIt != (*it).attributes.end());

        isInt = CString::stringToInt( (mapIt->second).c_str(), &vertexID );

        assert( isInt );
        assert( vertexID - 1 == pos );



        // SET NODE LABEL
        CGraphVertex *vertex;

        if ( nodeLabelTag.empty() )
        {
            vertex = new CGraphVertex( '\0' );
            auxVertexID = (*graph)->addVertex( vertex );
            assert( auxVertexID == vertexID - 1 );
        }
        else
        {
            // Get Label of the node: //<attr name="type"><int>2</int>
            currentNodeLabel = "";
            for (vector<XMLElement_t>::iterator itNodeLabel = (*it).content.begin();
                    itNodeLabel != (*it).content.end(); ++itNodeLabel)
            {
                //assert( (*itNodeLabel).attributes.find("name") != (*itNodeLabel).attributes.end() );

                if ((*itNodeLabel).attributes["name"] == nodeLabelTag )
                {
                    assert((*itNodeLabel).content.size() == 1);

                    currentNodeLabel = (*itNodeLabel).content[0].value;
                    break;
                }
            }

            if (currentNodeLabel == "") // ERROR: wrong label
            {
                cerr << "ERROR (GXL-node): Wrong tag label '" << nodeLabelTag << "'" << endl;
                cerr << "Possible labels: ";

                if ((*it).content.empty())
                    cerr << "-empty-";
                else {
                    for (vector<XMLElement_t>::iterator itNodeLabel = (*it).content.begin();
                            itNodeLabel != (*it).content.end(); ++itNodeLabel)
                    {
                        if (itNodeLabel != (*it).content.begin())
                            cerr << ", ";
                        cerr << (*itNodeLabel).attributes["name"];
                    }
                }
                cerr << endl;
                exit(-1);
            }

            assert( currentNodeLabel.length() == 1 );

            vertex = new CGraphVertex( currentNodeLabel[0] );
            auxVertexID = (*graph)->addVertex( vertex );
            assert( auxVertexID == vertexID - 1 );
        }

    }
}

//------------------------------------------------------------------------------
//
static void prv_readGXL_ReadEdges( const vector<XMLElement_t>::const_iterator &itGXL,
                                       vector<XMLElement_t>::iterator &it,
                                       CGraph **graph )
{
    map<string, string>::const_iterator mapIt;
    int edgeFromID, edgeToID;
    bool isInt;

    for (; it != (*itGXL).content[0].content.end(); ++it )
    {
        assert( (*it).name == "edge" );
        assert( (*it).attributes.size() == 2 );

        mapIt = (*it).attributes.find("from");
        assert( mapIt != (*it).attributes.end() );

        isInt = CString::stringToInt( (mapIt->second).c_str(), &edgeFromID );
        assert( isInt );

        mapIt = (*it).attributes.find("to");
        assert( mapIt != (*it).attributes.end() );

        isInt = CString::stringToInt( (mapIt->second).c_str(), &edgeToID );
        assert( isInt );

        --edgeFromID;
        --edgeToID;
        assert( edgeFromID >= 0 &&
                edgeToID >= 0 &&
                edgeFromID < (signed)(*graph)->numVertex() &&
                edgeToID < (signed)(*graph)->numVertex() );

        (*graph)->addEdge( edgeFromID, edgeToID );
    }
}

//------------------------------------------------------------------------------
//
static class CGraph* prv_readGXL( const char* filename,
                                    string graphLabelTag, string nodeLabelTag )
{
    assert( CFileNameHelper::compareExtension( filename, "gxl" ) == true );

    vector<XMLElement_t>::iterator it;
    CGraph *graph;

    CXMLParser *xmlParser = new CXMLParser( filename );

    //xmlParser->print();

    vector<XMLElement_t> &xmlData = xmlParser->getXML();


    // Leemos hasta le etiqueta de <gxl>
    vector<XMLElement_t>::iterator itGXL;
    for ( itGXL = xmlData.begin(); itGXL != xmlData.end(); ++itGXL )
        if( (*itGXL).name == "gxl" )
            break;

    // Comprobaciones iniciales de formato
    if( xmlData.size() < 2 ||
        xmlData[0].name != "?xml" || // 1ª línea
        itGXL == xmlData.end() || // Se ha llegado al final
        (*itGXL).name != "gxl" || // No se ha encontrado <gxl>
        (*itGXL).content.size() != 1 || // SOLO SE PERMITE UN GRAFO POR FICHERO
        (*itGXL).content[0].name != "graph" || // Formato: <gxl> <graph>
        (*itGXL).content[0].content.empty()) // <graph> vacío
    {
        cerr << "ERROR: Wrong XML file '" << filename << "'" << endl;
        cerr << "<?xml version=\"1.0\"?>" << endl;
        cerr << "<gxl>" << endl;
        cerr << "\t<graph id=\"...\">" << endl;
        cerr << "\t\t<node id=\"1\"> ... </node>" << endl;
        cerr << "\t\t..." << endl;
        cerr << "\t\t<edge from=\"55\" to=\"54\"> ... </edge>" << endl;
        cerr << "\t\t..." << endl;
        cerr << "\t</graph>" << endl;
        cerr << "</gxl>" << endl;
        exit(-1);
    }

    // 1 - READ GRAPH LABEL

    string label = prv_LoadFromGXL_GetGraphLabel( graphLabelTag, itGXL );

    graph = new CGraph( label.c_str() );


    // 2 - READ NODES

    prv_LoadFromGXL_ReadVertex( nodeLabelTag, itGXL, it, &graph );


    // 3 - READ EDGES

    prv_LoadFromGXL_ReadEdges( itGXL, it, &graph );


    delete xmlParser;

    return graph;
}
*/


//------------------------------------------------------------------------------
//
class CGraph<class IVertex>* PrvBaseGraphIO :: prv_readGXL(
                                  class CFileInTxt *file,
                                  CGraphIO_FPtr_readVertex func_readVertex,
                                  const char* strGraphLabelTag, const char* strNodeLabelTag )
{
    return NULL;
    /*

    vector<XMLElement_t>::iterator it;
    CGraph *graph;

    CXMLParser *xmlParser = new CXMLParser( filename );

    //xmlParser->print();

    vector<XMLElement_t> &xmlData = xmlParser->getXML();


    // Leemos hasta le etiqueta de <gxl>
    vector<XMLElement_t>::iterator itGXL;
    for ( itGXL = xmlData.begin(); itGXL != xmlData.end(); ++itGXL )
        if( (*itGXL).name == "gxl" )
            break;

    // Comprobaciones iniciales de formato
    if( xmlData.size() < 2 ||
        xmlData[0].name != "?xml" || // 1ª línea
        itGXL == xmlData.end() || // Se ha llegado al final
        (*itGXL).name != "gxl" || // No se ha encontrado <gxl>
        (*itGXL).content.size() != 1 || // SOLO SE PERMITE UN GRAFO POR FICHERO
        (*itGXL).content[0].name != "graph" || // Formato: <gxl> <graph>
        (*itGXL).content[0].content.empty()) // <graph> vacío
    {
        cerr << "ERROR: Wrong XML file '" << filename << "'" << endl;
        cerr << "<?xml version=\"1.0\"?>" << endl;
        cerr << "<gxl>" << endl;
        cerr << "\t<graph id=\"...\">" << endl;
        cerr << "\t\t<node id=\"1\"> ... </node>" << endl;
        cerr << "\t\t..." << endl;
        cerr << "\t\t<edge from=\"55\" to=\"54\"> ... </edge>" << endl;
        cerr << "\t\t..." << endl;
        cerr << "\t</graph>" << endl;
        cerr << "</gxl>" << endl;
        exit(-1);
    }

    // 1 - READ GRAPH LABEL

    string label = prv_LoadFromGXL_GetGraphLabel( graphLabelTag, itGXL );

    graph = new CGraph( label.c_str() );


    // 2 - READ NODES

    prv_LoadFromGXL_ReadVertex( nodeLabelTag, itGXL, it, &graph );


    // 3 - READ EDGES

    prv_LoadFromGXL_ReadEdges( itGXL, it, &graph );


    delete xmlParser;

    return graph;*/
}

//------------------------------------------------------------------------------
//
class CGraph<class IVertex>* PrvBaseGraphIO :: prv_readBin(
                                class CFileInBin *file,
                                CGraphIO_FPtr_readVertex func_readVertex )
{
    class CArrayRef<IVertex> *auxVertexList;
    class CGraph<IVertex> *graph;
    unsigned long numVertex;

    assert_no_null( file );
    assert_no_null( func_readVertex );

    numVertex = file->readULong();
    auxVertexList = new CArrayRef<IVertex>( numVertex );
    graph = new CGraph<IVertex>();

    for( unsigned long ind = 0; ind < numVertex; ++ind )
    {
        class IVertex *vertex;

        vertex = func_readVertex( file );

        auxVertexList->set( ind, vertex );
        graph->addVertex( vertex );
    }

    for( unsigned long ind = 0; ind < numVertex; ++ind )
    {
        unsigned long numChildren;
        const class IVertex *vertex;

        numChildren = file->readULong();
        vertex = auxVertexList->get( ind );

        for( unsigned long nc = 0; nc < numChildren; ++nc )
        {
            unsigned long indChild;
            const class IVertex *child;

            indChild = file->readULong();
            assert( indChild < numVertex );

            child = auxVertexList->get( indChild );

            graph->addEdge( vertex, child );
        }
    }

    delete auxVertexList;

    return graph;
}

//------------------------------------------------------------------------------
//
static bool** prv_writeGRF_getMatrix( const class CGraph<IVertex> *graph,
                                      const class CArrayRef<IVertex> *arrayVertex )
{
    bool **matrix;
    unsigned long numVertex;

    numVertex = arrayVertex->size();

    matrix = new bool*[ numVertex ];

    for ( unsigned long ind = 0; ind < numVertex; ++ind )
    {
        class CArrayRef<IVertex> *arrayChildren;
        const class IVertex *vertex;
        unsigned long numChildren;

        matrix[ind] = new bool[ numVertex ];
        for ( unsigned int j = 0; j < numVertex; ++j )
            matrix[ind][j] = false;

        vertex = arrayVertex->get( ind );
        arrayChildren = graph->getChildren( vertex );
        numChildren = arrayChildren->size();

        for ( unsigned long nc = 0; nc < numChildren; ++nc )
        {
            const class IVertex *child;
            unsigned long child_id;
            bool existChild;

            child = arrayChildren->get( nc );
            existChild = arrayVertex->exist<IVertex>( child, NULL, &child_id );
            assert( existChild == true );

            matrix[ ind ][ child_id ] = true;
        }

        delete arrayChildren;
    }

    return matrix;
}

//------------------------------------------------------------------------------
//
void PrvBaseGraphIO :: prv_writeGRF(
                            const class CGraph<IVertex> *graph,
                            class CFileOutTxt *file,
                            CGraphIO_FPtr_writeVertex func_writeVertex )
{
    class CArrayRef<IVertex> *arrayVertex;
    bool **matrix;
    unsigned long numVertex;

    assert_no_null( graph );
    assert_no_null( file );
    assert_no_null( func_writeVertex );

    arrayVertex = graph->getVertex();
    numVertex = arrayVertex->size();

    matrix = prv_writeGRF_getMatrix( graph, arrayVertex );

    file->writeULong( numVertex );
    file->writeChar( '\n' );

    for (unsigned long i = 0; i < numVertex; ++i)
    {
        const class IVertex *vertex;

        for (unsigned long j = 0; j < numVertex; ++j)
            file->writeBool( matrix[i][j] );

        file->writeChar( ' ' );

        vertex = arrayVertex->get( i );
        func_writeVertex( vertex, file );

        file->writeChar( '\n' );
    }

    for (unsigned int i = 0; i < numVertex; ++i)
        delete [] matrix[i];
    delete [] matrix;

    delete arrayVertex;
}

//------------------------------------------------------------------------------
//
void PrvBaseGraphIO :: prv_writeGXL(
                            const class CGraph<IVertex> *graph,
                            class CFileOutTxt *file,
                            CGraphIO_FPtr_writeVertex func_writeVertex,
                            const char* graphTagLabel, const char* nodeTagLabel )
{
/*
    vector<XMLElement_t> xmlData;

    XMLElement_t elemHead;
    elemHead.name = "?xml";
    elemHead.attributes["version"] = "1.0";
    xmlData.push_back( elemHead );

    XMLElement_t elemGraph;

    // 1 - SET GRAPH LABEL
    elemGraph.name = "graph";

    if ( strcmp( graph->getLabel(), "" ) != 0 )
        elemGraph.attributes[ graphLabelTag ] = string( graph->getLabel() );

    elemGraph.attributes["edgeids"] = "true";
    elemGraph.attributes["edgemode"] = "directed";

    // 2 - SET NODES
    unsigned int numVertex = graph->numVertex();
    CGraphVertex *vertex;

    for ( unsigned int i = 0; i < numVertex; ++i )
    {
        vertex = graph->getVertex(i);

        XMLElement_t elemNode;
        elemNode.name = "node";
        elemNode.attributes["id"] = CString::IntToString( vertex->getID() + 1 );

        // Node - Label
        if( nodeLabelTag != "" &&
            vertex->getLabel() != '\0' )
        {
            XMLElement_t nodeLabel, nodeLabelValue;
            nodeLabel.name = "attr";
            nodeLabel.attributes[ "name" ] = nodeLabelTag;

            nodeLabelValue.name = "string";
            nodeLabelValue.value = CString::CharToString( vertex->getLabel() );

            nodeLabel.content.push_back( nodeLabelValue );
            elemNode.content.push_back( nodeLabel );
        }

        elemGraph.content.push_back( elemNode );
    }

    // 3 - SET EDGES
    for ( unsigned int i = 0; i < numVertex; ++i )
    {
        vertex = graph->getVertex(i);
        unsigned int numOutputs = vertex->numOutputs();

        for ( unsigned int j = 0; j < numOutputs; ++j )
        {
            XMLElement_t elemEdge;
            elemEdge.name = "edge";
            elemEdge.attributes["from"] = CString::IntToString( vertex->getID() + 1 );
            elemEdge.attributes["to"] = CString::IntToString( ( vertex->getChild(j) )->getID() + 1 );

            elemGraph.content.push_back(elemEdge);
        }
    }

    XMLElement_t elemGXL;
    elemGXL.name = "gxl";
    elemGXL.content.push_back(elemGraph);

    xmlData.push_back( elemGXL );

    XMLParser *xmlParser = new XMLParser();
    xmlParser->SetXML( xmlData );
    xmlParser->WriteToFile( filename );

    delete xmlParser;*/
}

//------------------------------------------------------------------------------
//
void PrvBaseGraphIO :: prv_writeDOT(
                            const class CGraph<IVertex> *graph,
                            class CFileOutTxt *file,
                            CGraphIO_FPtr_writeVertex func_writeVertex )
{
    class CArrayRef<IVertex> *arrayVertex;
    unsigned long numVertex;

    assert_no_null( graph );
    assert_no_null( file );
    assert_no_null( func_writeVertex );

    arrayVertex = graph->getVertex();
    numVertex = arrayVertex->size();

    file->writeString( "digraph gdot {\n" );

    for( unsigned long ind = 0; ind < numVertex; ++ind )
    {
        class CArrayRef<IVertex> *arrayChildren;
        const class IVertex *vertex;
        unsigned long numChildren;

        vertex = arrayVertex->get( ind );
        arrayChildren = graph->getChildren( vertex );
        numChildren = arrayChildren->size();

        for( unsigned long nc = 0; nc < numChildren; ++nc )
        {
            const class IVertex *child;
            unsigned long child_ind;
            bool existChild;

            child = arrayChildren->get( nc );
            existChild = arrayVertex->exist<IVertex>( child, NULL, &child_ind );
            assert( existChild == true );

            file->writeULong( ind );
            file->writeString( " -> " );
            file->writeULong( child_ind );
            file->writeString( ";" );
        }

        delete arrayChildren;
    }

    file->writeString( "}\n" );

    delete arrayVertex;
}

//------------------------------------------------------------------------------
//
void PrvBaseGraphIO :: prv_writeBin(
                                const class CGraph<class IVertex> *graph,
                                class CFileOutBin *file,
                                CGraphIO_FPtr_writeVertex func_writeVertex )
{
    class CArrayRef<IVertex> *auxVertexList;
    unsigned long numVertex;

    assert_no_null( graph );
    assert_no_null( file );
    assert_no_null( func_writeVertex );

    auxVertexList = graph->getVertex();
    numVertex = auxVertexList->size();

    file->writeULong( numVertex );

    for( unsigned long ind = 0; ind < numVertex; ++ind )
    {
        const class IVertex *vertex;

        vertex = auxVertexList->get( ind );

        func_writeVertex( vertex, file );
    }

    for( unsigned long ind = 0; ind < numVertex; ++ind )
    {
        const class IVertex *vertex;
        class CArrayRef<IVertex> *childrenList;
        unsigned long numChildren;

        vertex = auxVertexList->get( ind );
        childrenList = graph->getChildren( vertex );
        numChildren = childrenList->size();

        file->writeULong( numChildren );

        for( unsigned long nc = 0; nc < numChildren; ++nc )
        {
            const class IVertex *child;
            unsigned long ind_child;

            child = childrenList->get( nc );

            auxVertexList->exist<IVertex>( child, NULL, &ind_child );
            assert( ind_child < numVertex );

            file->writeULong( ind_child );
        }

        delete childrenList;
    }

    delete auxVertexList;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

/*
//------------------------------------------------------------------------------
//
static class CArray<PrvVertexData> *prv_readVertexs(
                            CGraph_FPtr_read func_readVertex,
                            class IFileIn *in)
{
    unsigned long numVertex;
    class CArray<PrvVertexData> *vertexs;

    assert_no_null(in);

    numVertex = in->readULong();
    vertexs = new CArray<PrvVertexData>(numVertex);

    for (unsigned long i = 0; i < numVertex; i++)
    {
        class IVertex *dataVertex;
        struct PrvVertexData *vertex;

        dataVertex = func_readVertex(in);
        vertex = new PrvVertexData(dataVertex);
        vertexs->set(i, vertex);
    }

    return vertexs;
}

//------------------------------------------------------------------------------
//
static void prv_readEdgesVertex(
                        class CArrayRef<PrvVertexData> *familyList,
                        class CArray<PrvVertexData> *vertexs,
                        class IFileIn *in)
{
    unsigned long numEdges;

    assert_no_null(in);

    numEdges = in->readULong();

    for (unsigned long i = 0; i < numEdges; i++)
    {
        unsigned long indVertex;
        const class PrvVertexData *vertexFamily;

        indVertex = in->readULong();
        vertexFamily = vertexs->get(indVertex);
        familyList->add(vertexFamily);
    }
}

//------------------------------------------------------------------------------
//
static void prv_readEdges(class CArray<PrvVertexData> *vertexs, class IFileIn *in)
{
    unsigned long numVertexs;

    assert_no_null(vertexs);

    numVertexs = vertexs->size();

    for (unsigned long i = 0; i < numVertexs; i++)
    {
        class PrvVertexData *vertex;

        vertex = vertexs->get(i);
        assert_no_null(vertex);

        prv_readEdgesVertex(vertex->parentsList, vertexs, in);
        prv_readEdgesVertex(vertex->childrenList, vertexs, in);
    }
}

//------------------------------------------------------------------------------
//
class CGraph<IVertex>* PrvBaseGraphIO :: prv_read(
                                CGraphIO_FPtr_readVertex func_readVertex,
                                class IFileIn *in)
{
    unsigned char version;
    struct prv_baseGraph_t *dataGraph;
    class CArray<PrvVertexData> *vertexs;

    assert_no_null(in);
    assert_no_null(func_readVertex);

    version = in->readByte();
    assert(version <= PRV_VERSION_FILE);

    vertexs = prv_readVertexs(func_readVertex, in);
    prv_readEdges(vertexs, in);

    dataGraph = prv_create(&vertexs);

    return new PrvBaseGraph(&dataGraph);
}

//------------------------------------------------------------------------------
//
static void prv_writeVertexs(
                    const class CArray<PrvVertexData> *vertexs,
                    CGraph_FPtr_write func_writeVertex,
                    class IFileOut *out)
{
    unsigned long numVertex;

    assert_no_null(vertexs);

    numVertex = vertexs->size();
    out->writeULong(numVertex);

    for (unsigned long i = 0; i < numVertex; i++)
    {
        struct PrvVertexData *vertex;

        vertex = vertexs->get(i);
        assert_no_null(vertex);

        func_writeVertex(vertex->vertex, out);
    }
}

//------------------------------------------------------------------------------
//
static void prv_writeEdgesVertex(
                    class CArrayRef<PrvVertexData> *familyList,
                    class CArray<PrvVertexData> *vertexDataList,
                    class IFileOut *out)
{
    unsigned long numEdges;

    assert_no_null(familyList);
    assert_no_null(out);

    numEdges = familyList->size();
    out->writeULong(numEdges);

    for (unsigned long i = 0; i < numEdges; i++)
    {
        unsigned long indVertex;
        const struct PrvVertexData *vertexFamily;
        bool exist;

        vertexFamily = familyList->get(i);

        exist = vertexDataList->existOnlyOneElement<PrvVertexData>(vertexFamily, NULL, &indVertex);
        assert(exist == true);

        out->writeULong(indVertex);
    }
}

//------------------------------------------------------------------------------
//
static void prv_writeEdges(class CArray<PrvVertexData> *vertexs, class IFileOut *out)
{
    unsigned long numVertexs;

    numVertexs = vertexs->size();

    for (unsigned long i = 0; i < numVertexs; i++)
    {
        class PrvVertexData *vertex;

        vertex = vertexs->get(i);

        prv_writeEdgesVertex(vertex->parentsList, vertexs, out);
        prv_writeEdgesVertex(vertex->childrenList, vertexs, out);
    }
}

//------------------------------------------------------------------------------
//
void PrvBaseGraphIO :: prv_write( const class CGraph<class IVertex> *graph,
                                CGraphIO_FPtr_writeVertex func_writeVertex,
                                class IFileOut *out )
{
    assert_no_null(graph);
    prv_integrity(graph->m_data);
    assert_no_null(func_writeVertex);
    assert_no_null(out);

    out->writeByte(PRV_VERSION_FILE);

    prv_writeVertexs(graph->m_data->vertexDataList, func_writeVertex, out);
    prv_writeEdges(graph->m_data->vertexDataList, out);
}
*/
