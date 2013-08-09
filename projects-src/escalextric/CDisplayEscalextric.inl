/* -----------------------------------------------------------------------------
 *  CDisplayEscalextric.inl
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

class CDisplayEscalextric
{
    public:

        static const char *SYMBOL_CIRCUIT;
        static const char *SYMBOL_CAR;

        static class IDisplay *createDisplaySprite(const class CDataCircuit *dataCircuit, class CWorldEscalextric *worldEscalextric);
        static class IDisplay *createDisplayGL(const class CDataCircuit *dataCircuit, class CWorldEscalextric *worldEscalextric);
};
