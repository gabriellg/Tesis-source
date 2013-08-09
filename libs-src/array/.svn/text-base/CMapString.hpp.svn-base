/* -----------------------------------------------------------------------------
*  CMapString
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

class CMapString
{
    public:
        CMapString();
        CMapString( const class CMapString *map );
        ~CMapString();

        void clear();
        unsigned long size() const;

        const char* get( const char* key ) const;
        class CArrayString* getArrayKeys() const;

        void set( const char* key, const char* value );
        void add( const char* key, const char* value );

        void erase( const char* key );

        void concatenate( const class CMapString *map );

        bool exist( const char* key ) const;

    private:
        struct prv_mapString_t *m_data;
};
