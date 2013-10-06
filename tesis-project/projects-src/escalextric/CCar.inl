/* -----------------------------------------------------------------------------
 *  CCar.inl
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CAgent.hpp"

class CCar: public CAgent
{
    public:

        static const char *LEFT;
        static const char *RIGHT;

        CCar(const char *idCar);
        virtual ~CCar();
        
    private:

        virtual void beginEvolution(class CCollectionEventsSystem *allEvents) {;}
        virtual class CAgent *evolution(class CCollectionEventsSystem *allEvents, class CArrayRef<CAgent> **sons);

        virtual class CArray<IObjectDraw> *createRepresentation(class CTypeDescription *evtDescription,
                class CArray<IObjectDraw> **childsOpt);


        struct prv_dataPrivateCar_t *m_dataPrivate;
};
