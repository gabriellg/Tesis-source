/* -----------------------------------------------------------------------------
*  CArray
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include "PrvBaseArray.inl"

template<class Type> class CArray : private PrvBaseArray
{
    public:
        CArray( void );
        CArray( unsigned long numElements );
        CArray( const class CArray *array );

        inline void clear();
        inline unsigned long size() const;

        inline Type* get( unsigned long pos ) const;
        inline Type* getLast( void ) const;

        inline void set( unsigned long pos, Type *item );
        inline void add( Type *item );
        inline void insert( unsigned long pos, Type *item );

        inline void erase(unsigned long pos);
        inline void eraseRef(unsigned long pos);

        inline void eraseLast();
        inline void eraseRefLast();

        inline void concatenate( const class CArray *array );
        inline void concatenateDestroying( class CArray **array );

        template<class TypeSearch> inline class CArrayRef<Type>* search(
                                const TypeSearch *itemSearch,
                                bool (*func_search_optional)(const Type*, const TypeSearch*) ) const;
        template<class TypeSearch> inline bool exist(
                                const TypeSearch *itemSearch,
                                bool (*func_search_optional)(const Type*, const TypeSearch*),
                                unsigned long *indFirst_optional) const;
        template<class TypeSearch> inline bool existFrom(
                                const TypeSearch *itemSearch,
                                unsigned long fromIndex,
                                bool (*func_search_optional)(const Type*, const TypeSearch*),
                                unsigned long *indNext_optional ) const;
        template<class TypeSearch> inline bool existOnlyOneElement(
                                const TypeSearch *itemSearch,
                                bool (*func_search_optional)(const Type*, const TypeSearch*),
                                unsigned long *ind_optional) const;
        template<class TypeSearch> inline Type* getOnlyOneElement(
                                const TypeSearch *itemSearch,
                                bool (*func_search_optional)(const Type*, const TypeSearch*)) const;

    private:
        inline static void prv_destroyElement( struct prv_item_t **item );
        inline static struct prv_item_t* prv_copyElement( const struct prv_item_t *item );
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//
template<class Type> CArray<Type>::CArray(void)
: PrvBaseArray( 0, prv_destroyElement )
{
    ;
}

//------------------------------------------------------------------------------
//
template<class Type> CArray<Type>::CArray(unsigned long numElements)
: PrvBaseArray( numElements, prv_destroyElement )
{
    ;
}

//------------------------------------------------------------------------------
//
template<class Type> CArray<Type>::CArray(const class CArray *array)
: PrvBaseArray( array, prv_copyElement )
{
    ;
}

//------------------------------------------------------------------------------
//
template<class Type> void CArray<Type>::clear()
{
    PrvBaseArray::clear();
}

//------------------------------------------------------------------------------
//
template<class Type> unsigned long CArray<Type>::size() const
{
    return PrvBaseArray::size();
}

//------------------------------------------------------------------------------
//
template<class Type> Type* CArray<Type>::get( unsigned long pos ) const
{
    return (Type*)PrvBaseArray::get( pos );
}

//------------------------------------------------------------------------------
//
template<class Type> Type* CArray<Type>::getLast( void ) const
{
    return (Type*)PrvBaseArray::getLast();
}

//------------------------------------------------------------------------------
//
template<class Type> void CArray<Type>::set( unsigned long pos, Type *item )
{
    PrvBaseArray::set( pos, (struct prv_item_t *)item );
}

//------------------------------------------------------------------------------
//
template<class Type> void CArray<Type>::add( Type *item )
{
    PrvBaseArray::add( (struct prv_item_t *)item );
}

//------------------------------------------------------------------------------
//
template<class Type> void CArray<Type>::insert( unsigned long pos, Type *item )
{
    PrvBaseArray::insert( pos, (struct prv_item_t *)item );
}

//------------------------------------------------------------------------------
//
template<class Type> void CArray<Type>::erase( unsigned long ind )
{
    PrvBaseArray::erase( ind );
}

//------------------------------------------------------------------------------
//
template<class Type> void CArray<Type>::eraseRef(unsigned long ind)
{
    PrvBaseArray::eraseRef(ind);
}

//------------------------------------------------------------------------------
//
template<class Type> void CArray<Type>::eraseLast()
{
    PrvBaseArray::eraseLast();
}

//------------------------------------------------------------------------------
//
template<class Type> void CArray<Type>::eraseRefLast()
{
    PrvBaseArray::eraseRefLast();
}

//------------------------------------------------------------------------------
//
template<class Type> void CArray<Type>::concatenate( const class CArray *array )
{
    PrvBaseArray::concatenate(array, prv_copyElement);
}

//------------------------------------------------------------------------------
//
template<class Type> void CArray<Type>::concatenateDestroying(class CArray **array)
{
    class PrvBaseArray **arrayPrivate;

    arrayPrivate = (class PrvBaseArray **)array;
    PrvBaseArray::concatenateDestroying(arrayPrivate);
}

//------------------------------------------------------------------------------
//
template<class Type> template<class TypeSearch>
class CArrayRef<Type>* CArray<Type>::search(
                            const TypeSearch *itemSearch,
                            bool (*func_search_optional)(const Type*, const TypeSearch*) ) const
{
    return (class CArrayRef<Type>*)PrvBaseArray::search(
                    (const struct prv_itemSearch_t *)itemSearch,
                    (CArray_FPtr_search)func_search_optional);
}

//------------------------------------------------------------------------------
//
template<class Type> template<class TypeSearch>
bool CArray<Type>::exist(
                            const TypeSearch *itemSearch,
                            bool (*func_search_optional)(const Type*, const TypeSearch*),
                            unsigned long *indFirst_optional) const
{
    return PrvBaseArray::exist(
                            (const struct prv_itemSearch_t*)itemSearch,
                            (CArray_FPtr_search)func_search_optional, 
                            indFirst_optional) ;
}

//------------------------------------------------------------------------------
//
template<class Type> template<class TypeSearch>
bool CArray<Type>::existFrom(
                            const TypeSearch *itemSearch,
                            unsigned long fromIndex,
                            bool (*func_search_optional)(const Type*, const TypeSearch*),
                            unsigned long *indNext_optional) const
{
    return PrvBaseArray::existFrom(
                        (const struct prv_itemSearch_t*)itemSearch,
                        fromIndex,
                        (CArray_FPtr_search)func_search_optional,
                        indNext_optional );
}

//------------------------------------------------------------------------------
//
template<class Type> template<class TypeSearch>
bool CArray<Type>::existOnlyOneElement(
                            const TypeSearch *itemSearch,
                            bool (*func_search_optional)(const Type*, const TypeSearch*),
                            unsigned long *ind_optional) const
{
    return PrvBaseArray::existOnlyOneElement(
                            (const struct prv_itemSearch_t*)itemSearch,
                            (CArray_FPtr_search)func_search_optional,
                            ind_optional);
}

//------------------------------------------------------------------------------
//
template<class Type> template<class TypeSearch>
Type* CArray<Type>::getOnlyOneElement(
                            const TypeSearch *itemSearch,
                            bool (*func_search_optional)(const Type*, const TypeSearch*)) const
{
    return (Type*)PrvBaseArray::getOnlyOneElement(
                            (const struct prv_itemSearch_t*)itemSearch,
                            (CArray_FPtr_search)func_search_optional);
}

//------------------------------------------------------------------------------
//
template<class Type> void CArray<Type>::prv_destroyElement(struct prv_item_t **item)
{
    delete (Type*)(*item);
    *item = 0L;
}

//------------------------------------------------------------------------------
//
template<class Type> struct prv_item_t *CArray<Type>::prv_copyElement( const struct prv_item_t *item )
{
    return (struct prv_item_t *)new Type((const Type*)item);
}
