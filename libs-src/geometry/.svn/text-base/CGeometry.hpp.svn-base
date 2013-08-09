//CGeometry: Geometry Functions.

class CGeometry
{
    public:

        static double module2D(double x1, double y1, double x2, double y2);

        static void rotate2D(double x, double y, double xc, double yc, double Ux, double Uy, double *x_rotate, double *y_rotate);

        static bool isSamePoint2D(double x1, double y1, double x2, double y2, double precision);

        static void traslatePoint2D(double x, double y, double dx, double dy, double Ux, double Uy, double *xtraslate, double *ytraslate);

        static double angleVectorZeroTo2PiRadians(double x, double y);

        static double module3D(double x1, double y1, double z1, double x2, double y2, double z2);

        static bool isSamePoint3D(double x1, double y1, double z1, double x2, double y2, double z2, double precision);

        static bool areCollinearPoints(
                    double x1, double y1, double z1,
                    double x2, double y2, double z2,
                    double x3, double y3, double z3, double precision);

        static void calculateParametricPoint(
                    double x1, double y1, double z1,
                    double x2, double y2, double z2,
                    double t,
                    double *x, double *y, double *z);

        static void extrusion(
                    double Nx, double Ny, double Nz,
                    double *Ux, double *Uy, double *Uz,
                    double *Vx, double *Vy, double *Vz);

        static void horizontalAxisFromThePlaneNormal(
                    double A, double B, double C,
                    double *Ux, double *Uy, double *Uz,
                    double *Vx, double *Vy, double *Vz);

        static void rectangleWithProporcional(
                    double widthWindow, double heigthWindow,
                    double coefPorcBorder,
                    double xmin, double ymin, double zmin,
                    double xmax, double ymax, double zmax,
                    double *xminOut, double *yminOut, double *zminOut,
                    double *xmaxOut, double *ymaxOut, double *zmaxOut);

        static void calculateCenter(
                            double xmin, double ymin, double zmin,
                            double xmax, double ymax, double zmax,
                            double *xcenter, double *ycenter, double *zcenter);

        //Plane functions

        static void calculateCoefEcuationPlane(
                        double x1, double y1, double z1,
                        double x2, double y2, double z2,
                        double x3, double y3, double z3,
                        double *A, double *B, double *C, double *D);

        static double distanceToPlane(double A, double B, double C, double D, double x, double y, double z);

        static void point3dProjectedTo2D(
                        double Xo, double Yo, double Zo,
                        double Ux, double Uy, double Uz,
                        double Vx, double Vy, double Vz,
                        double x3D, double y3D, double z3D,
                        double *x2D, double *y2D);
};
