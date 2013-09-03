/* -----------------------------------------------------------------------------
 *  CDisplaySimulationRobot.inl
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

class CDisplaySimulationRobot
{
    public:
        static const char *SYMBOL_ROBOT;
        static const char *SYMBOL_SCENARIO;
        static const char *SYMBOL_MEMORY;

        static class IDisplay *createDisplaySprite(class CWorldSimulationRobot *world);
        static class IDisplay *createDisplayGL(class CWorldSimulationRobot *world);
};
