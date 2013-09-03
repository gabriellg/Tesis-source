/* -----------------------------------------------------------------------------
 *  IGraphics.hpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#ifndef IGRAPHICS_HPP_
#define IGRAPHICS_HPP_

class IGraphics
{
    public:

        enum ELevelDetail
        {
            LEVELDETAIL_VERYHIGH,
            LEVELDETAIL_HIGH,
            LEVELDETAIL_NORMAL,
            LEVELDETAIL_LOW,
        };

        enum ETypeDrawSolid
        {
            TYPEDRAW_SMOOTH,
            TYPEDRAW_SOLID,
            TYPEDRAW_WIRE
        };

        enum ETypeLight
        {
            LIGHT_AMBIENT,
            LIGHT_DIFUSSE,
            LIGHT_SPECULAR
        };

        enum ELight
        {
            GENERAL,
            LIGHT0
        };

        virtual ~IGraphics() {;};

        virtual void initDevice(void) = 0;
        virtual void clear(void) = 0;

        virtual void defineMaterial(const class CMaterial *material) = 0;

        virtual void setMaterial(const char *nameLayer) = 0;

        virtual void resetTransformation(void) = 0;

        virtual void pushTransformation(void) = 0;

        virtual void defineLight(enum ELight light, enum ETypeLight typeLight, double r_color, double g_color, double b_color) = 0;

        virtual void positionLight(enum ELight light, double x, double y, double z) = 0;

        virtual void viewport(int x, int y, unsigned long sizeX, unsigned long sizeY) = 0;

        virtual void definePerpective(double fovy, double ratioXforY, double zNear, double zFar) = 0;

        virtual void defineOrtho(double xorg, double yorg, double zorg, double xmin, double ymin, double zmin, double xmax, double ymax, double zmax) = 0;

        virtual void positionCamera(
                        double eyeX, double eyeY, double eyeZ,
                        double pointReferenceX, double pointReferenceY, double pointReferenceZ,
                        double upX, double upY, double upZ) = 0;

        virtual void traslation(double tx, double ty, double tz) = 0;

        virtual void rotation(double angleDegrees, double Ux, double Uy, double Uz) = 0;

        virtual void scale(double sx, double sy, double sz) = 0;

        virtual void appendMatrix(const class CMatrix4x4 *matrix) = 0;

        virtual void setNormal(double Nx, double Ny, double Nz) = 0;

        virtual void popTransformation(void) = 0;

        virtual void drawLine(double x1, double y1, double z1, double x2, double y2, double z2) = 0;

        virtual void drawRect(double x1, double y1, double x2, double y2) = 0;

        virtual void drawPolyline(const class CArrPoint3d *points) = 0;

        virtual void drawPolygon(const class CArrPoint3d *points) = 0;

        virtual void drawSurface(const class CArray<CArrPoint3d> *contours) = 0;

        virtual void drawQuads(
                        double x1, double y1, double z1,
                        double x2, double y2, double z2,
                        double x3, double y3, double z3,
                        double x4, double y4, double z4) = 0;

        virtual void drawUnitCube(enum ETypeDrawSolid typeDraw) = 0;

        virtual void drawUnitSphere(enum ELevelDetail levelDetail, enum ETypeDrawSolid typeDraw) = 0;

        virtual void drawUnitCylinder(enum ELevelDetail levelDetail, enum ETypeDrawSolid typeDraw) = 0;

        virtual void drawUnitCone(enum ELevelDetail levelDetail, enum ETypeDrawSolid typeDraw) = 0;

        virtual void drawUnitDisk(enum ELevelDetail levelDetail, enum ETypeDrawSolid typeDraw) = 0;

        virtual void drawImageRGB(double x, double y, double width, double height, const class CImg *image) = 0;

        virtual void pointWindowToWorld(double xPixel, double yPixel, double *x, double *y) = 0;

        virtual void drawText(const char *text, double x, double y, double heigth) = 0;

        virtual bool hasManagerBlocks(void) const = 0;

        virtual unsigned long beginBlock(void) = 0;

        virtual void endBlock(unsigned long id_block) = 0;

        virtual void drawBlock(unsigned long id_block) = 0;

        virtual void drawMesh(
                            const class CArrPoint3d *points,
                            const class CArrPoint3d *normals,
                            const class CArrPoint2d *textCoord_opt,
                            const class CArrayULong *indexTriangles,
                            const class CArray<CArrayULong> *indexTriangleFan,
                            const class CArray<CArrayULong> *indexTriangleStrip) = 0;
};

#endif /* IGRAPHICS_HPP_ */
