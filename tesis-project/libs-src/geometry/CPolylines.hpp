/* -----------------------------------------------------------------------------
 *  CGeneratoPolilinesClosed.hpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

class CPolylines
{
    public:

        //Creations.
        static class CArrPoint2d *createArcEllipse(double rx, double ry, double angleIni, double angleFin, unsigned long numPoints);
        static class CArrPoint2d *createCircle(double radius, unsigned long numPoints, bool counterclockwise);
        static class CArrPoint2d *createRectangleCentredInOrigin(double width, double height);
        static class CArrPoint2d *createPoligonRegular(double radius, unsigned long numEdges);

        //2d to 3d.
        static class CArrPoint3d *createPolyline2dTo3d(const class CArrPoint2d *polyline, double z);

        //Transformations 2d.
        static void rotate2d(class CArrPoint2d *polyline, double xc, double yc, double Ux, double Uy);
        static void move2d(class CArrPoint2d *polyline, double dx, double dy);
        static void centerGravity2d(const class CArrPoint2d *polyline, double *xCenterGravity, double *yCenterGravity);

        //Operation.
        static double areaPolyline(const class CArrPoint2d *polyline);

        //Transformations 3d.
        static void move3d(class CArrPoint3d *polyline, double dx, double dy, double dz);
};
