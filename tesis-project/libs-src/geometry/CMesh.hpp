/* -----------------------------------------------------------------------------
 *  CMesh.hpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

class CMesh
{
    public:

        enum ETypeTexture
        {
            TEXTURE_REPEAT, TEXTURE_DECAL
        };

        CMesh(void);
        virtual ~CMesh();

        void concatenate(const class CMesh *mesh);

        void appendQuad(
                        double x1, double y1, double z1,
                        double Nx1, double Ny1, double Nz1,
                        double x2, double y2, double z2,
                        double Nx2, double Ny2, double Nz2,
                        double x3, double y3, double z3,
                        double Nx3, double Ny3, double Nz3,
                        double x4, double y4, double z4,
                        double Nx4, double Ny4, double Nz4);

        void appendTriangle(
                        double x1, double y1, double z1,
                        double Nx1, double Ny1, double Nz1,
                        double x2, double y2, double z2,
                        double Nx2, double Ny2, double Nz2,
                        double x3, double y3, double z3,
                        double Nx3, double Ny3, double Nz3);

        void appendTriangleWithTexture(
                        double x1, double y1, double z1,
                        double Nx1, double Ny1, double Nz1,
                        double texx1, double texy1,
                        double x2, double y2, double z2,
                        double Nx2, double Ny2, double Nz2,
                        double texx2, double texy2,
                        double x3, double y3, double z3,
                        double Nx3, double Ny3, double Nz3,
                        double texx3, double texy3);

        void appendTessetation(
                        class CArrPoint3d **points,
                        class CArrPoint3d **normals,
                        class CArrayULong **indexsTriangles,
                        class CArray<CArrayULong> **indexsTriangleStrip,
                        class CArray<CArrayULong> **indexsTriangleFan);

        void appendTriangleFan(
                        class CArrPoint3d **points,
                        class CArrPoint3d **normals,
                        class CArrayULong **indexsTriangleFan);

        void appendTriangleFanWithTexture(
                        class CArrPoint3d **points,
                        class CArrPoint3d **normals,
                        class CArrPoint2d **texture,
                        class CArrayULong **indexsTriangleFan);

        void appendTriangleStrip(
                        class CArrPoint3d **points,
                        class CArrPoint3d **normals,
                        class CArrayULong **indexsTriangleStrip);

        void appendMultipleTriangleStripBands(
                        class CArrPoint3d **points,
                        class CArrPoint3d **normals,
                        class CArray<CArrayULong> **indexsTriangleStrip);

        void appendTriangleStripWithTexture(
                        class CArrPoint3d **points,
                        class CArrPoint3d **normals,
                        class CArrPoint2d **texture,
                        class CArrayULong **indexsTriangleStrip);

        void applyTransformation(const class CStackTransformation *transformation);

        void applyMatrix(class CMatrix4x4 *matrixTransformation);

        void calculateCoordsTexturesInPlane(enum CMesh::ETypeTexture typeTexture);

        void draw(class IGraphics *graphics) const;

    private:

        struct SDataPrivateMesh *m_dataPrivate;
};
