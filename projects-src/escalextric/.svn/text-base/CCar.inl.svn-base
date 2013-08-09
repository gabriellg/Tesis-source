/* -----------------------------------------------------------------------------
 *  CCar.inl
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CAgentWithRepresentation.hpp"

class CCar: public CAgentWithRepresentation
{
    public:

        static const char *LEFT;
        static const char *RIGHT;

        CCar(const char *idCar);
        virtual ~CCar();
        
    private:

        virtual class CAgent *evolution(class CCollectionEventsSystem *allEvents);
        virtual class CAgent *representation(class CTypeDescription *evtDescription);

        struct prv_dataPrivateCar_t *m_dataPrivate;
};
