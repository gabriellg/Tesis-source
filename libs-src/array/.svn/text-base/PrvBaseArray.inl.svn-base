/* -----------------------------------------------------------------------------
*  PrvBaseArray
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*          Gabriel López García (gabriellg AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#ifndef PRVBASEARRAY_HPP
#define PRVBASEARRAY_HPP

typedef struct prv_item_t* (*CArray_FPtr_copyElement)(const struct prv_item_t *);
typedef void (*CArray_FPtr_destroyElement)(struct prv_item_t **);
typedef bool (*CArray_FPtr_search)(const struct prv_item_t *, const struct prv_itemSearch_t *);

class PrvBaseArray
{
    public:

        PrvBaseArray(unsigned long numElements, CArray_FPtr_destroyElement funcReleasePtr);
        PrvBaseArray(const class PrvBaseArray *array, CArray_FPtr_copyElement funcCopy);
        ~PrvBaseArray();

        class PrvBaseArray *invert(CArray_FPtr_copyElement funcCopy) const;

        void clear();
        unsigned long size() const;

        struct prv_item_t *get(unsigned long ind) const;
        struct prv_item_t *getLast(void) const;

        void set(unsigned long ind, struct prv_item_t *item);
        void add(struct prv_item_t *item);
        void insert(unsigned long ind, struct prv_item_t *item);

        void erase(unsigned long ind);
        void eraseRef(unsigned long ind);

        void eraseLast();
        void eraseRefLast();

        void concatenate(const class PrvBaseArray *array, CArray_FPtr_copyElement funcCopy);

        class PrvBaseArray* search( const struct prv_itemSearch_t *itemSearch,
                            CArray_FPtr_search funcSearch_optional ) const;
        bool exist( const struct prv_itemSearch_t *itemSearch,
                            CArray_FPtr_search func_search_optional,
                            unsigned long *indFirst_optional ) const;
        bool existFrom( const struct prv_itemSearch_t *itemSearch,
                            unsigned long fromIndex,
                            CArray_FPtr_search func_search_opc,
                            unsigned long *indNext_optional ) const;
        bool existOnlyOneElement( const struct prv_itemSearch_t *itemSearch,
                            CArray_FPtr_search func_search_optional,
                            unsigned long *ind_optional ) const;
        struct prv_item_t *getOnlyOneElement( const struct prv_itemSearch_t *itemSearch,
                            CArray_FPtr_search func_search_optional ) const;

    private:

        PrvBaseArray(struct prv_privateArray_t **data);

        struct prv_privateArray_t *m_data;
};

#endif
