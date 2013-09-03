//CStackTransformation.hpp:

class CStackTransformation
{
    public:

        CStackTransformation();
        CStackTransformation(const class CStackTransformation *stackTransformation);
        virtual ~CStackTransformation();

        void resetTransformation(void);

        void pushTransformation(void);

        void popTransformation(void);

        void appendTranslation(double tx, double ty, double tz);

        void appendRotation(double angleDegrees, double Ux, double Uy, double Uz);

        void appendAxis(double Ux, double Uy, double Uz, double Vx, double Vy, double Vz);

        void appendExtrusion(double Nx, double Ny, double Nz);

        void appendScale(double sx, double sy, double sz);

        void appendMatrix(const class CMatrix4x4 *matrix);

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

    private:

        struct prv_dataPrivateStackTransformation_t *m_dataPrivate;
};
