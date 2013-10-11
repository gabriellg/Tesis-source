/* -----------------------------------------------------------------------------
 *  CGeneratorModel.hpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

class CGeneratorModel
{
    public:

        static class CMesh *createWithGrid3d(const class CArrayRef<CArrPoint3d> *controlPoints);
        static class CMesh *createSky(double height, double holeDome, unsigned long numIterations);
        static class CMesh *createDome(double height, double holeDome, unsigned long numIterations);

        static class CMesh *createWithPathOpenWithoutCaps(const class CArrayRef<CArrPoint3d> *polylinesInPathWithSamePoints);
        static class CMesh *createWithPathOpen(const class CArrayRef<CArrPoint3d> *polylinesInPathWithSamePoints);
        static class CMesh *createWithPathClosed(const class CArrayRef<CArrPoint3d> *polylinesInPathWithSamePoints);
        static class CMesh *createGridWithImage(const class CImg *image, const class CScaleColorWithHeight *colorsAndHeights, double widthGrid, double heightGrid);
};
