/* -----------------------------------------------------------------------------
 *  CEventAcceleration.inl
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CEventSystem.hpp"

class CEventAcceleration: public CEventSystem
{
    public:

        static const char *ID_EVENT;

        CEventAcceleration(const char *idCar, double incrAcceleration);
        virtual ~CEventAcceleration();

        void applyAcceleration(const char *idCar, double *acceleration);
    private:

        struct prv_dataPrivateEventAcceleration_t *m_dataPrivate;
};
