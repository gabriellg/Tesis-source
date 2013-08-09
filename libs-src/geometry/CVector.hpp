/* -----------------------------------------------------------------------------
 *  CVector.hpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

class CVector
{
    public:

        static void twoPointstoVector(
                        double x1, double y1, double z1,
                        double x2, double y2, double z2,
                        double *Ux, double *Uy, double *Uz);

        static void toUnitVector(double *Ux, double *Uy, double *Uz);
        static bool isUnitVector(double Ux, double Uy, double Uz, double precision);

        static double module3d(double Ux, double Uy, double Uz);

        static void directorVectorAngle(double angle, double *Ux, double *Uy);

        static double dotProduct(
                            double Ux, double Uy, double Uz,
                            double Vx, double Vy, double Vz);

        static void crossProduct(
                            double Ux, double Uy, double Uz,
                            double Vx, double Vy, double Vz,
                            double *Nx, double *Ny, double *Nz);
};
