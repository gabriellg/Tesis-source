/* -----------------------------------------------------------------------------
 *  CGraphicsRect.hpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "IGraphics.hpp"

class CGraphicsRect: public IGraphics
{
    public:

        CGraphicsRect();
        CGraphicsRect(const class IGraphics *graphicsTypeRect);
        virtual ~CGraphicsRect();

        bool getLimits(double *xmin, double *ymin, double *zmin, double *xmax, double *ymax, double *zmax);

    private:

        virtual void initDevice(void) {;}
        virtual void clear(void) {;}

        virtual void defineMaterial(const class CMaterial *material) {;}

        virtual void setMaterial(const char *nameMaterial) {;}
        virtual void setColor(float r, float g, float b) {;}

        virtual void resetTransformation(void);
        virtual void pushTransformation(void);

        virtual void defineLight(enum ELight light, enum ETypeLight typeLight, double r_color, double g_color, double b_color) {;}

        virtual void positionLight(enum ELight light, double x, double y, double z) {;}

        virtual void viewport(int x, int y, unsigned long sizeX, unsigned long sizeY) {;}

        virtual void definePerpective(double fovy, double ratioXforY, double zNear, double zFar) {;}

        virtual void defineOrtho(
                        double xorg, double yorg, double zorg,
                        double xmin, double ymin, double zmin,
                        double xmax, double ymax, double zmax) {;}

        virtual void positionCamera(
                        double eyeX, double eyeY, double eyeZ,
                        double pointReferenceX, double pointReferenceY, double pointReferenceZ,
                        double upX, double upY, double upZ) {;}

        virtual void traslation(double tx, double ty, double tz);

        virtual void rotation(double angleDegrees, double Ux, double Uy, double Uz);

        virtual void scale(double sx, double sy, double sz);

        virtual void appendMatrix(const class CMatrix4x4 *matrix);


        virtual void setNormal(double Nx, double Ny, double Nz) {;}

        virtual void popTransformation(void);

        virtual void drawLine(double x1, double y1, double z1, double x2, double y2, double z2);

        virtual void drawRect(double x1, double y1, double x2, double y2);

        virtual void drawPolyline(const class CArrPoint3d *points);

        virtual void drawPolygon(const class CArrPoint3d *points);

        virtual void drawSurface(const class CArray<CArrPoint3d> *contours);

        virtual void drawQuads(
                        double x1, double y1, double z1,
                        double x2, double y2, double z2,
                        double x3, double y3, double z3,
                        double x4, double y4, double z4);

        virtual void drawUnitCube(enum ETypeDrawSolid typeDraw);

        virtual void drawUnitSphere(enum ELevelDetail levelDetail, enum ETypeDrawSolid typeDraw);

        virtual void drawUnitCylinder(enum ELevelDetail levelDetail, enum ETypeDrawSolid typeDraw);

        virtual void drawUnitCone(enum ELevelDetail levelDetail, enum ETypeDrawSolid typeDraw);

        virtual void drawUnitDisk(enum ELevelDetail levelDetail, enum ETypeDrawSolid typeDraw);

        virtual void drawImageRGB(double x, double y, double width, double height, const class CImg *image);

        virtual void pointWindowToWorld(double xPixel, double yPixel, double *x, double *y) {;}

        virtual void drawText(const char *text, double x, double y, double heigth);

        virtual bool hasManagerBlocks(void) const { return false; };

        virtual unsigned long beginBlock(void) { return 0;}

        virtual void endBlock(unsigned long id_block){;}

        virtual void drawBlock(unsigned long id_block){;}

        virtual void drawMesh(
                            const class CArrPoint3d *points,
                            const class CArrPoint3d *normals,
                            const class CArrPoint2d *textCoord_opt,
                            const class CArrayULong *indexTriangles,
                            const class CArray<CArrayULong> *indexTriangleFan,
                            const class CArray<CArrayULong> *indexTriangleStrip);

        struct prv_dataPrivateGraphicsRect_t *m_dataPrivate;
};
