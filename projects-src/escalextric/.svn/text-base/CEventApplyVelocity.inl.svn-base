/* -----------------------------------------------------------------------------
 *  CEventApplyVelocity.inl
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CEventSystem.hpp"

class CEventApplyVelocity: public CEventSystem
{
    public:

        static const char *ID_EVENT;

        CEventApplyVelocity(class CWorldEscalextric *world);
        virtual ~CEventApplyVelocity() {;}

        void applyVelocity(const char *idCar, double acceleration);

    private:

        struct prv_dataPrivateEventApplyVelocity_t *m_dataPrivate;
};
