/* -----------------------------------------------------------------------------
 *  CDisplayEscalextric.inl
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

class CDisplayEscalextric
{
    public:

        static const char *SYMBOL_CIRCUIT;
        static const char *SYMBOL_CAR;
        static const char *SYMBOL_WORLD;
        static const char *SYMBOL_LIGHT_AMBIENT;

        static class ITraslatorDisplay *createDisplaySprite(const class CDataCircuit *dataCircuit, class CWorldEscalextric *worldEscalextric);
        static class ITraslatorDisplay *createDisplay3D(const class CDataCircuit *dataCircuit, class CWorldEscalextric *worldEscalextric);
};
