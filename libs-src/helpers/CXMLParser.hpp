/* -----------------------------------------------------------------------------
*  CXMLParser
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/
template<class> class CArrayRef;

class CXMLParser
{
    public:
        static class CNodeXML* load( const char* filename );
        static void save( const char* filename, const class CNodeXML *rootXML );
};

class CNodeXML          // <name attribute1='' att2=''>value</name>
{                       // <name> <child1></child1> </name>
    public:
        CNodeXML();
        CNodeXML( const char* name, const char* value );
        ~CNodeXML();

        const char* name() const;
        const char* value() const;

        const char* attribute( const char* key ) const;
        class CMapString* getAttributes() const;

        class CArrayRef<CNodeXML>* getChildren() const;

        void addChildNode( class CNodeXML *node );
        void addAttribute( const char* attribute, const char* value );

        void processChildren( void (*func)(const class CNodeXML *node) ) const;

        void processChildren_recursive( void (*func)(const class CNodeXML *node) ) const;

        //bool findInChildren(const char *tagChild, class CNodeXML **nodeXML) const;*/

    protected:
        struct prv_nodeXML_t *m_data;
};
