/* -----------------------------------------------------------------------------
 *  CDisplay3dMetereo.inl
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

class CDisplay3DMetereo
{
    public:

        static const char *SYMBOL_FORREST;
        static const char *SYMBOL_TREE;
        static const char *SYMBOL_LIGHTING;

        static class IDisplay *createDisplay3d(unsigned long numRows, unsigned long numCols);
        static void defineLayers(class IGraphics *graphics);
};

