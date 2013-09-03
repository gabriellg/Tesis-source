//CMatrix4x4.hpp:

class CMatrix4x4
{
    public:

        CMatrix4x4(const class CMatrix4x4 *matrix);
        CMatrix4x4(const double matrix[16]);
        CMatrix4x4(const float matrix[16]);

        virtual ~CMatrix4x4();

        static class CMatrix4x4 *createIdentity(void);

        static class CMatrix4x4 *createTraslation(double tx, double ty, double tz);

        static class CMatrix4x4 *createScale(double sx, double sy, double sz);

        static class CMatrix4x4 *createAxis(double Ux, double Uy, double Uz, double Vx, double Vy, double Vz);

        static class CMatrix4x4 *createExtrusion(double Nx, double Ny, double Nz);

        static class CMatrix4x4 *createRotate(double angleRotate, double Ux, double Uy, double Uz);

        static class CMatrix4x4 *createRotatePlaneXY(double angleRotateAxisZ);

        static class CMatrix4x4 *createRotatePlaneYZ(double angleRotateAxisX);

        static class CMatrix4x4 *createRotatePlaneXZ(double angleRotateAxisY);

        class CMatrix4x4 *multiplication(const class CMatrix4x4 *matrix) const;

        class CArrPoint3d *transformationPoints2d(const class CArrPoint2d *points) const;

        void transformationPoints3d(class CArrPoint3d *points) const;

        void transformationPoint3d(
                                double x, double y, double z,
                                double *xTrans, double *yTrans, double *zTrans) const;

        void transformationPointF3d(
                                float x, float y, float z,
                                float *xTrans, float *yTrans, float *zTrans) const;

        void transformationUnitVector(
                                double Ux, double Uy, double Uz,
                                double *UxTrans, double *UyTrans, double *UzTrans) const;

        void transformationUnitVectorF(
                                float Ux, float Uy, float Uz,
                                float *UxTrans, float *UyTrans, float *UzTrans) const;

        void getMatrix(double valuesMatrix[4][4]) const;

    private:

        CMatrix4x4(struct SDataPrivateMatrix4x4 **dataPrivate);

        struct SDataPrivateMatrix4x4 *m_dataPrivate;
};
