/* -----------------------------------------------------------------------------
*  CXMLParser
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include "CXMLParser.hpp"

#include <iostream>
#include <stdlib.h>

#include "CString.hpp"
#include "CArray.hpp"
#include "CArrayRef.hpp"
#include "CArrayString.hpp"
#include "CMapString.hpp"
#include "CFileInTxt.hpp"
#include "CFileOutTxt.hpp"
#include "asrtbas.h"
using namespace std;

struct prv_nodeXML_t
{
    char* name;
    class CMapString *attributes;
    class CArray<CNodeXML> *content;
    char* value;
};

class PrvNodeXML : public CNodeXML
{
    public:
        PrvNodeXML( struct prv_nodeXML_t *data )
        {
            assert_no_null(data);
            m_data = data;
        }
};

//------------------------------------------------------------------------------
//
static struct prv_nodeXML_t* prv_create( const char* name, const char* value )
{
    struct prv_nodeXML_t *data;

    data = new prv_nodeXML_t;
    assert_no_null( data );

    data->name = CString::copy( name );
    data->value = CString::copy( value );

    data->attributes = new CMapString();
    data->content = new CArray<CNodeXML>();

    return data;
}

//------------------------------------------------------------------------------
//
CNodeXML :: CNodeXML()
{
    m_data = prv_create( "", "" );
}

//------------------------------------------------------------------------------
//
CNodeXML :: CNodeXML( const char* name, const char* value )
{
    m_data = prv_create( name, value );
}

//------------------------------------------------------------------------------
//
CNodeXML :: ~CNodeXML()
{
    CString::free( &m_data->name );
    CString::free( &m_data->value );
    delete m_data->attributes;
    delete m_data->content;

    delete m_data;
}

//------------------------------------------------------------------------------
//
const char* CNodeXML :: name() const
{
    return m_data->name;
}

//------------------------------------------------------------------------------
//
const char* CNodeXML :: value() const
{
    return m_data->value;
}

//------------------------------------------------------------------------------
//
const char* CNodeXML :: attribute( const char* key ) const
{
    return m_data->attributes->get( key );
}

//------------------------------------------------------------------------------
//
class CMapString* CNodeXML :: getAttributes() const
{
    return new CMapString( m_data->attributes );
}

//------------------------------------------------------------------------------
//
class CArrayRef<CNodeXML>* CNodeXML :: getChildren() const
{
    return new CArrayRef<CNodeXML>( (const class CArrayRef<CNodeXML>*)((class PrvBaseArray*)m_data->content) );
}

//------------------------------------------------------------------------------
//
void CNodeXML :: addChildNode( class CNodeXML *node )
{
    assert_no_null( node );

    m_data->content->add(node);
}

//------------------------------------------------------------------------------
//
void CNodeXML :: addAttribute( const char* attribute, const char* value )
{
    m_data->attributes->add( attribute, value );
}

//------------------------------------------------------------------------------
//
void CNodeXML :: processChildren( void (*func)(const class CNodeXML *node) ) const
{
    unsigned long size;

    size = m_data->content->size();

    for( unsigned long ind = 0; ind < size; ++ind )
    {
        func( m_data->content->get( ind ) );
    }
}

//------------------------------------------------------------------------------
//
void CNodeXML :: processChildren_recursive( void (*func)(const class CNodeXML *node) ) const
{
    unsigned long size;

    size = m_data->content->size();

    for( unsigned long ind = 0; ind < size; ++ind )
    {
        const class CNodeXML *node;

        node = m_data->content->get( ind );

        func( node );

        if( node->m_data->content->size() > 0 )
            node->processChildren_recursive( func );
    }
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//
static char* prv_readToken( class CFileInTxt *file )
{
    char* str;
    char BR, c;

    assert_no_null( file );

    str = CString::copy( "" );

    c = file->readChar();

    if( c == '\"' || c == '\'')
    {
        BR = c;
        c = file->readChar();
    }
    else
        BR = ' ';

    while( c != '<' && c != '>' && 
           c != BR  && c != '=' &&
           file->eof() == false )
    {
        CString::concatenateChar( &str, c );

        c = file->readChar();
    }

    if( c != BR )
        file->ungetChar();

    return str;
}

//------------------------------------------------------------------------------
//
static char prv_trimXML( class CFileInTxt *fic )
{
    char c;

    assert_no_null( fic );

    c = fic->readChar();

    while( (c == ' ' || c == '\n' || c == '\t') && fic->eof()==false )
        c = fic->readChar();

    return c;
}

//------------------------------------------------------------------------------
// ATTRIBUTES	<tag attribute=value ...>
static void prv_getNode_readAttributes( class CFileInTxt *file,
                                        struct prv_nodeXML_t *node )
{
    char c, *strAttribute, *strValue;

    file->ungetChar();
    strAttribute = prv_readToken( file );

    c = file->readChar();

    assert_with_message( c == '=',
            "Malformed XML: Unexpected Symbol when reading attributes" );

    strValue = prv_readToken( file );

    node->attributes->add( strAttribute, strValue );
    //cout << "   " << strAttribute << " = " << strValue << endl;
}

//------------------------------------------------------------------------------
// INNER VALUE  <tag>Inner-value</tag>
static void prv_getNode_readInnverValue( class CFileInTxt *file,
                                         struct prv_nodeXML_t *node )
{
    char c;

    c = prv_trimXML( file );
    file->ungetChar();

    if( c != '<' )
    {
        assert_with_message( CString::size( node->value ) == 0,
                             "Malformed XML, two inner values" );

        node->value = prv_readToken( file );
        //cout << "   Inner value: " << node->value << endl;
    }
}

//------------------------------------------------------------------------------
//
static void prv_getNode_readEndTag( class CFileInTxt *file, struct prv_nodeXML_t *node )
{
    char* finalName;
    char c;

    finalName = prv_readToken( file );

    assert_with_message( CString::equal( finalName, node->name ) == true,
            "Malformed XML: end tag mismatch" );

    c = file->readChar();

    assert_with_message( c == '>',
            "Malformed XML: expected close symbol '>'" );

    CString::free( &finalName );
}

//------------------------------------------------------------------------------
//
static char* prv_readTagName( class CFileInTxt *file )
{
    char c, *tagName;

    c = prv_trimXML( file );

    assert_with_message( c == '<', "Malformed XML: Not found the open symbol '<'." );

    tagName = prv_readToken( file );

    return tagName;
}

//------------------------------------------------------------------------------
//
static class CNodeXML* prv_getNode( class CFileInTxt *file )
{
    class CNodeXML *nodeXML;
    struct prv_nodeXML_t *nodeData;
    char *strTagName;

    assert_no_null( file );

    strTagName = prv_readTagName( file );
    //cout << "NODE: " << nodeData->name << endl;

    assert( CString::equal( strTagName, "?xml" ) == false &&
            CString::equal( strTagName, "!DOCTYPE" ) == false );

    nodeData = prv_create( strTagName, "" );

    while( file->eof() == false )
    {
        char c;

        c = prv_trimXML( file );

        if( c == '>' )
        {
            // INNER VALUE <tag>Inner-value</tag>
            prv_getNode_readInnverValue( file, nodeData );
        }
        else if( c == '<' )
        {
            // CLOSE SYMBOL, there are 2 possibilities:
            //  1 - END TAG </TAG>
            //  2 - NEW TAG	-> RECURSIVE CALL
            if( file->readChar() == '/' )
            {
                prv_getNode_readEndTag( file, nodeData );
                break;
            }
            else
            {
                class CNodeXML *child;

                file->ungetChar();
                file->ungetChar();

                child = prv_getNode( file );

                nodeData->content->add( child );
            }                    
        }
        else
        {
            // ATTRIBUTES <tag attribute=value ...>
            prv_getNode_readAttributes( file, nodeData );
        }
    }

    nodeXML = (CNodeXML*)new PrvNodeXML( nodeData );

    CString::free( &strTagName );

    return nodeXML;
}

//------------------------------------------------------------------------------
//
static void prv_removeXMLHeader( class CFileInTxt *file )
{
    char c, *tagName;

    tagName = prv_readTagName( file );

    assert( CString::equal( tagName, "?xml" ) == true );

    while( c != '>' && file->eof() == false )
    {
        c = file->readChar();
    }

    c = prv_trimXML( file );
    assert_with_message( c == '<', "Malformed XML: Not found the open symbol '<'." );

    if( file->readChar() == '!' )
    {
        while( c != '>' && file->eof() == false )
        {
            c = file->readChar();
        }
    }
    else
    {
        file->ungetChar();
        file->ungetChar();
    }

    CString::free( &tagName );
}

//------------------------------------------------------------------------------
//
class CNodeXML* CXMLParser :: load( const char* filename )
{
    class CFileInTxt *file;
    class CNodeXML *rootXML;

    file = new CFileInTxt( filename );

    prv_removeXMLHeader( file );

    rootXML = prv_getNode( file );

//    if( node != NULL )
//        arrayXML->add( node );

    // Clear empty final lines in the file
//    if( CString::size( arrayXML->getLast()->name ) == 0 )
//        arrayXML->eraseLast();

//    assert( arrayXML->size() > 0 );
//    assert( CString::equal( arrayXML->get(0)->name, "?xml" ) == true );

    delete file;

    return rootXML;
}

//----------------------------------------------------------
//
static void prv_write_attributes( class CFileOutTxt *file,
                                const class CNodeXML *node )
{
    class CMapString *mapAttributes;
    class CArrayString *arrayKeys;
    unsigned long size;

    mapAttributes = node->getAttributes();
    arrayKeys = mapAttributes->getArrayKeys();
    size = arrayKeys->size();

    for( unsigned long ind = 0; ind < size; ++ind )
    {
        const char* key;

        key = arrayKeys->get( ind );

        file->writeChar( ' ' );
        file->writeString( key );
        file->writeString( "=\"" );
        file->writeString( mapAttributes->get( key ) );
        file->writeString( "\"" );
    }

    delete mapAttributes;
    delete arrayKeys;
}

//------------------------------------------------------------------------------
//
static void prv_write_indentation(class CFileOutTxt *file, unsigned long indentation)
{
    for(unsigned int i = 0; i < indentation; ++i)
        file->writeChar(' ');
}

//------------------------------------------------------------------------------
//
static void prv_write_recursive( class CFileOutTxt *file,
                                const class CNodeXML *node,
                                unsigned long indentation )
{
    class CArrayRef<CNodeXML> *arrayChildren;
    unsigned long numChildren;

    assert_no_null( file );
    assert( CString::equal( node->name(), "?xml" ) == false );

    arrayChildren = node->getChildren();
    numChildren = arrayChildren->size();

    prv_write_indentation( file, indentation );
    file->writeChar( '<' );
    file->writeString( node->name() );

    prv_write_attributes( file, node );

    file->writeChar( '>' );

    if( numChildren > 0 )
        file->writeChar( '\n' );

    for( unsigned long i = 0; i < numChildren; ++i )
    {
        prv_write_recursive( file, arrayChildren->get( i ), indentation + 1 );
    }

    if( CString::size( node->value() ) > 0 )
        file->writeString( node->value() );

    if( numChildren > 0 )
        prv_write_indentation( file, indentation );

    file->writeString( "</" );
    file->writeString( node->name() );
    file->writeString( ">\n" );

    delete arrayChildren;
}

//------------------------------------------------------------------------------
//
static void prv_write_header( class CFileOutTxt *file )
{
    file->writeString( "<?xml version=\"1.0\"?>\n" );
}

//------------------------------------------------------------------------------
//
void CXMLParser :: save( const char* filename, const class CNodeXML *rootNode )
{
    class CFileOutTxt *file;

    assert_no_null( rootNode );
    assert( CString::equal( rootNode->name(), "?xml" ) == false &&
            CString::equal( rootNode->name(), "!DOCTYPE" ) == false );

    file = new CFileOutTxt( filename );

    prv_write_header( file );

    prv_write_recursive( file, rootNode, 0 );

    delete file;
}
