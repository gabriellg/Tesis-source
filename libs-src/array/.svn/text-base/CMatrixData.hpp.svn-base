/* -----------------------------------------------------------------------------
 *  CMatrixData.hpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

typedef void (*CMatrixData_FPtr_destroyElement)(struct prv_itemDataMatrix_t **);

class PrvNotUseMatrixData
{
    public:

        bool hasData(unsigned long row, unsigned long col) const;
        void size(unsigned long *width, unsigned long *heigth) const;

    protected:

        PrvNotUseMatrixData(unsigned long width, unsigned long heigth, CMatrixData_FPtr_destroyElement func_destroyElement);
        virtual ~PrvNotUseMatrixData();

        struct prv_itemDataMatrix_t *prv_getData(unsigned long row, unsigned long col);
        const struct prv_itemDataMatrix_t *prv_getDataConst(unsigned long row, unsigned long col) const;
        void prv_setData(unsigned long row, unsigned long col, struct prv_itemDataMatrix_t *data);


    private:

        struct prv_dataPrivateMatrixData_t *m_dataPrivate;
};

template<class Type> class CMatrixData : public PrvNotUseMatrixData
{
    public:
        CMatrixData(unsigned long width, unsigned long heigth);

        inline Type *get(unsigned long row, unsigned long col);
        inline const Type *getConst(unsigned long row, unsigned long col) const;

        inline void set(unsigned long row, unsigned long col, Type *item);

    private:

        inline static void prv_destroyElement(struct prv_itemDataMatrix_t **item);
};

//------------------------------------------------------------------------------

template<class Type> CMatrixData<Type>::CMatrixData(unsigned long width, unsigned long heigth)
        : PrvNotUseMatrixData(width, heigth, prv_destroyElement)
{
    ;
}

//------------------------------------------------------------------------------

template<class Type> Type *CMatrixData<Type>::get(unsigned long row, unsigned long col)
{
    return (Type *)PrvNotUseMatrixData::prv_getData(row, col);
}

//------------------------------------------------------------------------------

template<class Type> const Type *CMatrixData<Type>::getConst(unsigned long row, unsigned long col) const
{
    return (const Type *)PrvNotUseMatrixData::prv_getDataConst(row, col);
}

//------------------------------------------------------------------------------

template<class Type> void CMatrixData<Type>::set(unsigned long row, unsigned long col, Type *item)
{
    PrvNotUseMatrixData::prv_setData(row, col, (struct prv_itemDataMatrix_t *)item);
}

//------------------------------------------------------------------------------

template<class Type> void CMatrixData<Type>::prv_destroyElement(struct prv_itemDataMatrix_t **item)
{
    delete (Type*)(*item);
    *item = 0L;
}
