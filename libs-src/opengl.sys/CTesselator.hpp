/* -----------------------------------------------------------------------------
 *  CTesselator.hpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

class CTesselator
{
    public:

        //Definition: A contour is a set of polygon where there are polylines with negative area (hole) and positive area too.

        static void tesselationContour(
                        const class CArrayRef<CArrPoint3d> *contour,
                        double Nx, double Ny, double Nz,
                        double tolerancePoints,
                        class CArrPoint3d **points,
                        class CArrPoint3d **normals,
                        class CArrayULong **indexsTriangles,
                        class CArray<CArrayULong> **indexsTriangleStrip,
                        class CArray<CArrayULong> **indexsTriangleFan);
};
