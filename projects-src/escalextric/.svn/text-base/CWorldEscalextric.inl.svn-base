/* -----------------------------------------------------------------------------
 *  CPhysicLaws.inl
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "IWorld.hpp"

class CWorldEscalextric: public IWorld
{
    public:

        CWorldEscalextric(const class CDataCircuit *dataCircuit);
        virtual ~CWorldEscalextric();

        void setWorld2d(void);
        void setWorld3d(void);

        void appendCarLaneRight(const char *idCar);
        void appendCarLaneLeft(const char *idCar);

        void applyVelocity(const char *idCar, double acceleration);

        virtual class CAgent *move(const char *idCar, class CAgent **figure) const;

    private:

        struct SPrvDataPrivateWorldEscalextric *m_dataPrivate;
};
