/* -----------------------------------------------------------------------------
 *  CWorldRobot.inl
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "IWorld.hpp"

class CWorldSimulationRobot: public IWorld
{
    public:

        CWorldSimulationRobot(bool is3d, class CImg **map);
        virtual ~CWorldSimulationRobot();

        void getObstaclesSimulation(
                double angleSpectrum, double incrAngleSpectrum,
                double distanceSpectrum, double incrDistanceSpectrum,
                class CArrayDouble **distances, class CArrayDouble **angles) const;

        void getMarkSimulation(double angleFocus, double distanceFocus, class CArrayString **marks) const;

        void pushTransformation(void);

        void rotate(double angle);
        void traslate(double dx, double dy);
        void applyTraslate(void);

        void popTransformation(void);

    private:

        struct prv_dataPrivateWorldSimulationRobot_t *m_dataPrivate;

        class CAgent *move(const char *idElement, class CAgent **figure) const;
};
