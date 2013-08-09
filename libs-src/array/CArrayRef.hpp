/* -----------------------------------------------------------------------------
*  CArrayRef
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include "PrvBaseArray.inl"

template<class Type> class CArrayRef : public PrvBaseArray
{
    public:

        CArrayRef(void);
        CArrayRef(unsigned long numElements);
        CArrayRef(const CArrayRef *array);

        inline void clear();
        inline unsigned long size() const;

        inline Type* get(unsigned long pos) const;
        inline Type* getLast(void) const;

        inline void set(unsigned long pos, Type *item);
        inline void set(unsigned long pos, const Type *item);
        inline void add(Type *item);
        inline void add(const Type *item);
        inline void insert(unsigned long pos, Type *item);
        inline void insert(unsigned long pos, const Type *item);

        inline void erase(unsigned long pos);
        inline void eraseLast();

        inline void concatenate(const class CArrayRef *array);

        template<typename TypeSearch> inline class CArrayRef<Type>* search(
                                const TypeSearch *itemSearch,
                                bool (*func_search_optional)(const Type*, const TypeSearch*)) const;

        template<typename TypeSearch> inline bool exist(
                                const TypeSearch *itemSearch,
                                bool (*func_search_optional)(const Type*, const TypeSearch*),
                                unsigned long *indFirst_optional) const;

        template<typename TypeSearch> inline bool existFrom(
                                const TypeSearch *itemSearch,
                                unsigned long fromIndex,
                                bool (*func_search_optional)(const Type*, const TypeSearch*),
                                unsigned long *indNext_optional) const;

        template<typename TypeSearch> inline bool existOnlyOneElement(
                                const TypeSearch *itemSearch,
                                bool (*func_search_optional)(const Type*, const TypeSearch*),
                                unsigned long *ind_optional) const;

        template<typename TypeSearch> inline Type* getOnlyOneElement(
                                const TypeSearch *itemSearch,
                                bool (*func_search_optional)(const Type*, const TypeSearch*)) const;
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//
template<class Type> CArrayRef<Type>::CArrayRef(void)
: PrvBaseArray((unsigned long)0, 0l)
{
    ;
}

//------------------------------------------------------------------------------
//
template<class Type> CArrayRef<Type>::CArrayRef(unsigned long numElements)
: PrvBaseArray(numElements, 0l)
{
    ;
}

//------------------------------------------------------------------------------
//
template<class Type> CArrayRef<Type>::CArrayRef(const CArrayRef *array)
: PrvBaseArray(array, 0l)
{
    ;
}

//------------------------------------------------------------------------------
//
template<class Type> void CArrayRef<Type>::clear()
{
    PrvBaseArray::clear();
}

//------------------------------------------------------------------------------
//
template<class Type> unsigned long CArrayRef<Type>::size() const
{
    return PrvBaseArray::size();
}

//------------------------------------------------------------------------------
//
template<class Type> Type* CArrayRef<Type>::get(unsigned long ind) const
{
    return (Type *)PrvBaseArray::get(ind);
}

//------------------------------------------------------------------------------
//
template<class Type> Type* CArrayRef<Type>::getLast(void) const
{
    return (const Type *)PrvBaseArray::getLast();
}

//------------------------------------------------------------------------------
//
template<class Type> void CArrayRef<Type>::set(unsigned long ind, Type *item)
{
    PrvBaseArray::set(ind, (struct prv_item_t *)item);
}

//------------------------------------------------------------------------------
//
template<class Type> void CArrayRef<Type>::set(unsigned long ind, const Type *item)
{
    PrvBaseArray::set(ind, (struct prv_item_t *)item);
}

//------------------------------------------------------------------------------
//
template<class Type> void CArrayRef<Type>::add( Type *item )
{
    PrvBaseArray::add((struct prv_item_t *)item);
}

//------------------------------------------------------------------------------
//
template<class Type> void CArrayRef<Type>::add(const Type *item )
{
    PrvBaseArray::add((struct prv_item_t *)item);
}

//------------------------------------------------------------------------------
//
template<class Type> void CArrayRef<Type>::insert( unsigned long pos, Type *item )
{
    PrvBaseArray::insert( pos, (struct prv_item_t *)item );
}

//------------------------------------------------------------------------------
//
template<class Type> void CArrayRef<Type>::insert( unsigned long pos, const Type *item )
{
    PrvBaseArray::insert( pos, (struct prv_item_t *)item );
}

//------------------------------------------------------------------------------
//
template<class Type> void CArrayRef<Type>::erase(unsigned long ind)
{
    PrvBaseArray::erase(ind);
}

//------------------------------------------------------------------------------
//
template<class Type> void CArrayRef<Type>::eraseLast()
{
    PrvBaseArray::eraseLast();
}

//------------------------------------------------------------------------------
//
template<class Type> void CArrayRef<Type>::concatenate(const class CArrayRef *array)
{
    PrvBaseArray::concatenate(array, 0l);
}

//------------------------------------------------------------------------------
//
template<class Type> template<class TypeSearch>
class CArrayRef<Type>* CArrayRef<Type>::search(
                            const TypeSearch *itemSearch,
                            bool (*func_search_optional)(const Type*, const TypeSearch*) ) const
{
    return PrvBaseArray::search(
                    (const struct prv_itemSearch_t *)itemSearch,
                    (CArray_FPtr_search)func_search_optional);
}

//------------------------------------------------------------------------------
//
template<class Type> template<class TypeSearch>
inline bool CArrayRef<Type>::exist(
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
inline bool CArrayRef<Type>::existFrom(
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
inline bool CArrayRef<Type>::existOnlyOneElement(
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
inline Type* CArrayRef<Type>::getOnlyOneElement(
                            const TypeSearch *itemSearch,
                            bool (*func_search_optional)(const Type*, const TypeSearch*)) const
{
    return (Type*)PrvBaseArray::getOnlyOneElement(
                            (const struct prv_itemSearch_t*)itemSearch,
                            (CArray_FPtr_search)func_search_optional);
}
