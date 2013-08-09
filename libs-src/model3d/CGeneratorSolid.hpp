/* -----------------------------------------------------------------------------
 *  CGeneratorSolid.hpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

class CGeneratorSolid
{
    public:

        static class CMesh *createSolidByExtrusion(
                            const class CArrPoint2d *polyline,
                            double height);

        static class CMesh *createSolidWithSurfaceByExtrusion(
                            const class CSurface2d *surface,
                            double height);

        static class CMesh *createUnitCube(void);

        static class CMesh *createCylinder(
                            double radiusInf, double radiusSup, double height,
                            unsigned long numPointsCilinder);
};
