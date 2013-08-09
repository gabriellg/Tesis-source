/* -----------------------------------------------------------------------------
 *  CModel3d.hpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

class CModel3d
{
    public:

        CModel3d(void);
        virtual ~CModel3d();

        void appendMaterial(class CMaterial **material);

        void appendMesh(const char *nameMaterial, const class CMesh *mesh);

        void appendModel(const class CModel3d *model);

        void appendTriangle(
                        const char *nameMaterial,
                        double x1, double y1, double z1,
                        double Nx1, double Ny1, double Nz1,
                        double x2, double y2, double z2,
                        double Nx2, double Ny2, double Nz2,
                        double x3, double y3, double z3,
                        double Nx3, double Ny3, double Nz3);

        void appendTriangleWithTexture(
                        const char *nameMaterial,
                        double x1, double y1, double z1,
                        double Nx1, double Ny1, double Nz1,
                        double texx1, double texy1,
                        double x2, double y2, double z2,
                        double Nx2, double Ny2, double Nz2,
                        double texx2, double texy2,
                        double x3, double y3, double z3,
                        double Nx3, double Ny3, double Nz3,
                        double texx3, double texy3);

        void InsideCube(double dimensionX, double dimensionY, double dimensionZ);

        void applyTransformation(const class CStackTransformation *transformation);

        void defineMaterialsInGraphics(class IGraphics *graphics);

        void defineBlock(class IGraphics *graphics);

        void draw(class IGraphics *graphics) const;

        void drawIdBlock(class IGraphics *graphics) const;

    private:

        struct SDataPrivateModel3d *m_dataPrivate;
};
