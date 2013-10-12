/* -----------------------------------------------------------------------------
 *  CGraphicsGL.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CGraphicsGL.hpp"

#include "opengl.inl"
#include "CTextureGL.inl"

#include "asrtbas.h"
#include "memory.h"

#include "CString.hpp"
#include "CImg.hpp"
#include "CMatrix4x4.hpp"
#include "CArray.hpp"
#include "CArraysTypes.hpp"
#include "CArrPoint3d.hpp"
#include "CArrPoint2d.hpp"
#include "CMath.hpp"
#include "CMaterial.hpp"

struct PrvLayer
{
    PrvLayer(class CMaterial **material_param, unsigned long idTexture_param);
    ~PrvLayer();

    unsigned long idTexture;
    class CMaterial *material;
};

struct prv_dataPrivateGraphicsGL_t
{
    class CArray<PrvLayer> *layers;

    bool isCameraPositioned;
    bool activeLight0;

    bool isInitializeText;
    unsigned long fontListBase;
    unsigned long fontHeightPixel;

    GLUtesselator *tesselator;
    GLUquadricObj *quadric;
    double r_background, b_background, g_background, a_background;

    unsigned long idBlockCurrent;
    class CArrayULong *ids_blocksRegisters;
};

//-----------------------------------------------------------------------
//-----------------------------------------------------------------------

PrvLayer::PrvLayer(class CMaterial **material_param, unsigned long idTexture_param)
{
    material = ASSIGN_PP_NO_NULL(material_param, class CMaterial);
    idTexture = idTexture_param;
}

//-----------------------------------------------------------------------

PrvLayer::~PrvLayer()
{
    assert_no_null(material);
    delete material;
}

//-----------------------------------------------------------------------

static bool prv_isSameMaterial(const struct PrvLayer *layer, const class CMaterial *material)
{
    assert_no_null(layer);
    assert_no_null(layer->material);
    return layer->material->isSameMaterial(material);
}

//-----------------------------------------------------------------------

static bool prv_isLayer(const struct PrvLayer *layer, const char *nameMaterial)
{
    assert_no_null(layer);
    assert_no_null(layer->material);
    return layer->material->isMaterial(nameMaterial);
}

//-----------------------------------------------------------------------

static struct prv_dataPrivateGraphicsGL_t *prv_create(
                        class CArray<PrvLayer> **layers,
                        bool isCameraPositioned,
                        bool activeLight0,
                        bool isInitializeText,
                        unsigned long fontListBase,
                        unsigned long fontHeightPixel,
                        GLUtesselator **tesselator,
                        GLUquadricObj **quadric,
                        double r_background, double b_background, double g_background, double a_background,
                        unsigned long idBlockCurrent,
                        class CArrayULong **ids_blocksRegisters)
{
    struct prv_dataPrivateGraphicsGL_t *dataPrivate;

    dataPrivate = MALLOC(struct prv_dataPrivateGraphicsGL_t);

    dataPrivate->layers = ASSIGN_PP_NO_NULL(layers, class CArray<PrvLayer>);

    dataPrivate->isCameraPositioned = isCameraPositioned;
    dataPrivate->activeLight0 = activeLight0;

    dataPrivate->isInitializeText = isInitializeText;
    dataPrivate->fontListBase = fontListBase;
    dataPrivate->fontHeightPixel = fontHeightPixel;

    dataPrivate->tesselator = ASSIGN_PP_NO_NULL(tesselator, GLUtesselator);
    dataPrivate->quadric = ASSIGN_PP_NO_NULL(quadric, GLUquadricObj);

    dataPrivate->r_background = r_background;
    dataPrivate->b_background = b_background;
    dataPrivate->g_background = g_background;
    dataPrivate->a_background = a_background;

    dataPrivate->idBlockCurrent = idBlockCurrent;
    dataPrivate->ids_blocksRegisters = ASSIGN_PP_NO_NULL(ids_blocksRegisters, class CArrayULong);

    return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroy(struct prv_dataPrivateGraphicsGL_t **dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(*dataPrivate);

    delete (*dataPrivate)->ids_blocksRegisters;
    delete (*dataPrivate)->layers;
    gluDeleteQuadric((*dataPrivate)->quadric);
    gluDeleteTess((*dataPrivate)->tesselator);

    FREE_T(dataPrivate, struct prv_dataPrivateGraphicsGL_t);
}

//---------------------------------------------------------------

GLvoid prv_handleError(GLenum errorCode)
{
    const char *stringError;

    stringError = (const char *)gluErrorString(errorCode);
    assert_message(stringError);
}

//---------------------------------------------------------------

GLvoid prv_combineCallback(GLdouble coords[3], GLdouble *vertex_data[4], GLfloat weight[4], GLdouble **dataOut)
{
    GLdouble *vertex;
    int i;

    vertex = CALLOC(6, GLdouble);

    vertex[0] = coords[0];
    vertex[1] = coords[1];
    vertex[2] = coords[2];

    for (i = 3; i < 6; i++)
        vertex[i] = 0;

    *dataOut = vertex;
}

//---------------------------------------------------------------

static void prv_installFunctionTesselator(GLUtesselator *tesselator)
{
    gluTessCallback(tesselator, GLU_TESS_VERTEX, (_GLUfuncptr)glVertex3dv);
    gluTessCallback(tesselator, GLU_TESS_BEGIN, (_GLUfuncptr)glBegin);
    gluTessCallback(tesselator, GLU_TESS_END, (_GLUfuncptr)glEnd);
    gluTessCallback(tesselator, GLU_TESS_ERROR, (_GLUfuncptr)prv_handleError);
    gluTessCallback(tesselator, GLU_TESS_COMBINE, (_GLUfuncptr)prv_combineCallback);
    gluTessProperty(tesselator, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_ODD);
}

//-----------------------------------------------------------------------

CGraphicsGL::CGraphicsGL()
{
    class CArray<PrvLayer> *layers;
    bool isCameraPositioned;
    bool activeLight0;
    bool isInitializeText;
    unsigned long fontListBase;
    unsigned long fontHeightPixel;
    GLUtesselator *tesselator;
    GLUquadricObj *quadric;
    double r_background, b_background, g_background, a_background;
    unsigned long idBlockCurrent;
    class CArrayULong *ids_blocksRegisters;

    layers = new CArray<PrvLayer>;
    isCameraPositioned = false;
    activeLight0 = false;

    tesselator = gluNewTess();
    prv_installFunctionTesselator(tesselator);

    quadric = gluNewQuadric();
    gluQuadricOrientation(quadric, GLU_OUTSIDE);

    r_background = 0.;
    g_background = 0.;
    b_background = 0.;
    a_background = 0.;

    isInitializeText = false;
    fontListBase = CMath::maxULong();
    fontHeightPixel = CMath::maxULong();

    idBlockCurrent = CMath::maxULong();
    ids_blocksRegisters = new CArrayULong;

    m_dataPrivate = prv_create(
                        &layers,
                        isCameraPositioned, activeLight0, isInitializeText,
                        fontListBase, fontHeightPixel,
                        &tesselator, &quadric,
                        r_background, b_background, g_background, a_background,
                        idBlockCurrent, &ids_blocksRegisters);
}

//-----------------------------------------------------------------------

CGraphicsGL::~CGraphicsGL()
{
    prv_destroy(&m_dataPrivate);
}

//---------------------------------------------------------------

static void prv_initFont(
                    unsigned long *fontListBase,
                    unsigned long *fontHeightPixel,
                    CGraphicsGL_FPtr_initFont func_initFont)
{
    assert_no_null(fontListBase);

    if (func_initFont != NULL)
    {
        unsigned long fontListBaseLoc;

        fontListBaseLoc = glGenLists(128);

        func_initFont(fontListBaseLoc, fontHeightPixel);

        *fontListBase = fontListBaseLoc;
    }
    else
    {
        *fontListBase = CMath::maxULong();
        *fontHeightPixel = CMath::maxULong();
    }
}

//---------------------------------------------------------------

void CGraphicsGL::initFont3D(CGraphicsGL_FPtr_initFont func_initFont)
{
    assert_no_null(m_dataPrivate);

    prv_initFont(&m_dataPrivate->fontListBase, &m_dataPrivate->fontHeightPixel, func_initFont);
    m_dataPrivate->isInitializeText = true;
}

//---------------------------------------------------------------

void CGraphicsGL::removeBlocksRegisters(void)
{
    unsigned long numBlocks;

    assert_no_null(m_dataPrivate);
    assert_no_null(m_dataPrivate->ids_blocksRegisters);

    numBlocks = m_dataPrivate->ids_blocksRegisters->size();

    for (unsigned long i = 0; i < numBlocks; i++)
    {
        unsigned long idBlock;

        idBlock = m_dataPrivate->ids_blocksRegisters->get(i);
        assert(glIsList(idBlock) == GL_TRUE);
        glDeleteLists(idBlock, 1);
    }

    delete m_dataPrivate->ids_blocksRegisters;
    m_dataPrivate->ids_blocksRegisters = new CArrayULong;
}

//---------------------------------------------------------------

static void prv_setLight(void)
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

//---------------------------------------------------------------

void CGraphicsGL::initDevice(void)
{
    assert_no_null(m_dataPrivate);

    glClearColor(m_dataPrivate->r_background, m_dataPrivate->g_background, m_dataPrivate->b_background, m_dataPrivate->a_background);

    glEnable(GL_DEPTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LESS);

    glShadeModel(GL_SMOOTH);

    prv_setLight();
}

//---------------------------------------------------------------

void CGraphicsGL::clear(void)
{
    GLfloat values[4] = {0.25f, 0.25f, 0.25f, 1.F};

    assert_no_null(m_dataPrivate);

    m_dataPrivate->isCameraPositioned = false;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMaterialfv(GL_FRONT, GL_AMBIENT, values);
}

//---------------------------------------------------------------

void CGraphicsGL::defineMaterial(const class CMaterial *material)
{
    struct PrvLayer *layer;
    unsigned long indFound;

    assert_no_null(m_dataPrivate);

    if (m_dataPrivate->layers->existOnlyOneElement<CMaterial>(material, prv_isSameMaterial, &indFound) == false)
    {
        class CMaterial *materialCopy;
        unsigned long idTexture;

        materialCopy = new CMaterial(material);

        if (materialCopy->hasTexture() == true)
        {
            const CImg *texture;

            texture = materialCopy->getTexture();
            idTexture = CTextureGL::createTexture(texture);
        }
        else
            idTexture = CMath::maxULong();

        layer = new PrvLayer(&materialCopy, idTexture);
        m_dataPrivate->layers->add(layer);
    }
}

//---------------------------------------------------------------

void CGraphicsGL::setMaterial(const char *nameLayer)
{
    struct PrvLayer *layer;
    enum CMaterial::EPriorityZFighting zFighting;

    assert_no_null(m_dataPrivate);

    layer = m_dataPrivate->layers->getOnlyOneElement<char>(nameLayer, prv_isLayer);
    assert_no_null(layer);
    assert_no_null(layer->material);

    if (layer->material->hasTexture() == true)
    {
        CTextureGL::textureOn(layer->idTexture);
    }
    else
    {
        float r, g, b, a;

        layer->material->getColorRGBA(&r, &g, &b, &a);
        CTextureGL::textureOff();
        glColor4f(r, g, b, a);
    }

    zFighting = layer->material->getPriorityZFighting();

    switch(zFighting)
    {
        case CMaterial::LOW:

            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(1., 1.);
            break;

        case CMaterial::MEDIUM:

            glDisable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(0., 0.);
            break;

        case CMaterial::HIGH:
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(-1., -1.);
        default_error();
    }
}

//---------------------------------------------------------------

void CGraphicsGL::resetTransformation(void)
{
    assert_no_null(m_dataPrivate);
    glLoadIdentity();
}

//---------------------------------------------------------------

void CGraphicsGL::pushTransformation(void)
{
    assert_no_null(m_dataPrivate);
    glPushMatrix();
}

//---------------------------------------------------------------

void CGraphicsGL::popTransformation(void)
{
    assert_no_null(m_dataPrivate);
    glPopMatrix();
}

//---------------------------------------------------------------

static GLenum prv_typeLightGL(enum IGraphics::ETypeLight typeLight)
{
    GLenum type;

    switch(typeLight)
    {
        case IGraphics::LIGHT_AMBIENT:

            type = GL_AMBIENT;
            break;

        case IGraphics::LIGHT_DIFUSSE:

            type = GL_DIFFUSE;
            break;

        case IGraphics::LIGHT_SPECULAR:

            type = GL_SPECULAR;
            break;

        default_error();
    }

    return type;
}

//---------------------------------------------------------------

void CGraphicsGL::defineLight(enum IGraphics::ELight light, enum IGraphics::ETypeLight typeLight, double r_color, double g_color, double b_color)
{
    GLenum typeLightGL;
    float values[4];

    assert_no_null(m_dataPrivate);

    values[0] = (float)r_color;
    values[1] = (float)g_color;
    values[2] = (float)b_color;
    values[3] = 1.f;

    typeLightGL = prv_typeLightGL(typeLight);

    switch(light)
    {
        case IGraphics::GENERAL:

            glMaterialfv(GL_FRONT, typeLightGL, values);
            break;

        case IGraphics::LIGHT0:

            if (m_dataPrivate->activeLight0 == false)
                m_dataPrivate->activeLight0 = true;

            glLightfv(GL_LIGHT0, typeLightGL, values);
            break;

        default_error();
    }
}

//---------------------------------------------------------------

void CGraphicsGL::positionLight(enum IGraphics::ELight light, double x, double y, double z)
{
    float values[4];

    assert_no_null(m_dataPrivate);

    values[0] = (float)x;
    values[1] = (float)y;
    values[2] = (float)z;
    values[3] = 1.f;

    switch(light)
    {
        case IGraphics::LIGHT0:

            glLightfv(GL_LIGHT0, GL_POSITION, values);
            break;

        case IGraphics::GENERAL:
        default_error();
    }
}

//---------------------------------------------------------------

void CGraphicsGL::viewport(int x, int y, unsigned long sizeX, unsigned long sizeY)
{
    assert_no_null(m_dataPrivate);
    glViewport(x, y, sizeX, sizeY);
}

//---------------------------------------------------------------

void CGraphicsGL::definePerpective(double fovy, double ratioXforY, double zNear, double zFar)
{
    assert_no_null(m_dataPrivate);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fovy, ratioXforY, zNear, zFar);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

//---------------------------------------------------------------

void CGraphicsGL::defineOrtho(
                    double xorg, double yorg, double zorg,
                    double xmin, double ymin, double zmin,
                    double xmax, double ymax, double zmax)
{
    assert_no_null(m_dataPrivate);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(xmin, xmax, ymin, ymax, zmin, zmax);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(xorg, yorg, zorg);
}

//---------------------------------------------------------------

void CGraphicsGL::positionCamera(
                    double eyeX, double eyeY, double eyeZ,
                    double pointReferenceX, double pointReferenceY, double pointReferenceZ,
                    double upX, double upY, double upZ)
{
    assert_no_null(m_dataPrivate);
    assert(m_dataPrivate->isCameraPositioned == false);

    m_dataPrivate->isCameraPositioned = true;

    glLoadIdentity();
    gluLookAt(eyeX, eyeY, eyeZ,  pointReferenceX, pointReferenceY, pointReferenceZ, upX, upY, upZ);
}

//---------------------------------------------------------------

void CGraphicsGL::traslation(double tx, double ty, double tz)
{
    assert_no_null(m_dataPrivate);
    glTranslated(tx, ty, tz);
}

//---------------------------------------------------------------

void CGraphicsGL::rotation(double angleDegrees, double Ux, double Uy, double Uz)
{
    assert_no_null(m_dataPrivate);
    glRotated(angleDegrees, Ux, Uy, Uz);
}

//---------------------------------------------------------------

void CGraphicsGL::scale(double sx, double sy, double sz)
{
    assert_no_null(m_dataPrivate);
    glScaled(sx, sy, sz);
}

//---------------------------------------------------------------

void CGraphicsGL::appendMatrix(const class CMatrix4x4 *matrix)
{
    double valuesMatrix[4][4];

    assert_no_null(m_dataPrivate);
    assert_no_null(matrix);

    matrix->getMatrix(valuesMatrix);

    glMultMatrixd((double *)valuesMatrix);
}

//---------------------------------------------------------------

void CGraphicsGL::setNormal(double Nx, double Ny, double Nz)
{
    assert_no_null(m_dataPrivate);
    glNormal3d(Nx, Ny, Nz);
}

//---------------------------------------------------------------

void CGraphicsGL::drawLine(double x1, double y1, double z1, double x2, double y2, double z2)
{
    assert_no_null(m_dataPrivate);

    glBegin(GL_LINES);
        glVertex3d(x1, y1, z1);
        glVertex3d(x2, y2, z2);
    glEnd();
}

//---------------------------------------------------------------

void CGraphicsGL::drawRect(double x1, double y1, double x2, double y2)
{
    assert_no_null(m_dataPrivate);

    glBegin(GL_LINE_LOOP);
        glVertex3d(x1, y1, 0.);
        glVertex3d(x2, y1, 0.);
        glVertex3d(x2, y2, 0.);
        glVertex3d(x1, y2, 0.);
    glEnd();
}

//---------------------------------------------------------------

static void prv_setVertexs(const class CArrPoint3d *points)
{
    unsigned long i, num;

    assert_no_null(points);

    num = points->size();

    for (i = 0; i < num; i++)
    {
        double x, y, z;

        points->get(i, &x, &y, &z);
        glVertex3d(x, y, z);
    }
}

//---------------------------------------------------------------

void CGraphicsGL::drawPolyline(const class CArrPoint3d *points)
{
    assert_no_null(m_dataPrivate);

    glBegin(GL_LINE_STRIP);
        prv_setVertexs(points);
    glEnd();
}

//---------------------------------------------------------------

void CGraphicsGL::drawPolygon(const class CArrPoint3d *points)
{
    glBegin(GL_POLYGON);
        prv_setVertexs(points);
    glEnd();
}

//---------------------------------------------------------------

static GLdouble *prv_insertVertexToTesselator(GLUtesselator *tesselator, const class CArrPoint3d *countour)
{
    GLdouble *vertexs;
    unsigned long i, num;

    assert_no_null(countour);

    num = countour->size();
    vertexs = CALLOC(3 * num, GLdouble);

    gluTessBeginContour(tesselator);

    for (i = 0; i < num; i++)
    {
        double x, y, z;

        countour->get(i, &x, &y, &z);

        vertexs[3 * i] = (GLdouble)x;
        vertexs[3 * i + 1] = (GLdouble)y;
        vertexs[3 * i + 2] = (GLdouble)z;

        gluTessVertex(tesselator, &(vertexs[3 * i]), &(vertexs[3 * i]));
    }

    gluTessEndContour(tesselator);

    return vertexs;
}

//---------------------------------------------------------------

static GLdouble **prv_insertToTesselator(
                        GLUtesselator *tesselator,
                        const class CArray<CArrPoint3d> *contours,
                        unsigned long *numCountours)
{
    GLdouble **allVertexs;
    unsigned long i, numContours_loc;

    assert_no_null(contours);
    assert_no_null(numCountours);

    numContours_loc = contours->size();

    allVertexs = CALLOC(numContours_loc, GLdouble *);

    gluTessBeginPolygon (tesselator, NULL);

    for (i = 0; i < numContours_loc; i++)
    {
        const class CArrPoint3d *countour;

        countour = contours->get(i);
        allVertexs[i] = prv_insertVertexToTesselator(tesselator, countour);
    }

    gluTessEndPolygon(tesselator);

    *numCountours = numContours_loc;

    return allVertexs;
}

//---------------------------------------------------------------

static void prv_freeVertexs(GLdouble ***allVertexs, unsigned long numCountour)
{
    unsigned long i;

    assert_no_null(allVertexs);

    for (i = 0; i < numCountour; i++)
        FREE_T(&((*allVertexs)[i]), GLdouble);

    FREE_T(allVertexs, GLdouble *);
}

//---------------------------------------------------------------

void CGraphicsGL::drawSurface(const class CArray<CArrPoint3d> *contours)
{
    GLdouble **allVertexs;
    unsigned long numCountour;

    assert_no_null(m_dataPrivate);

    allVertexs = prv_insertToTesselator(m_dataPrivate->tesselator, contours, &numCountour);
    prv_freeVertexs(&allVertexs, numCountour);
}

//---------------------------------------------------------------

void CGraphicsGL::drawQuads(
                double x1, double y1, double z1,
                double x2, double y2, double z2,
                double x3, double y3, double z3,
                double x4, double y4, double z4)
{
    glBegin(GL_QUADS);

        glVertex3d(x1, y1, z1);
        glVertex3d(x2, y2, z2);
        glVertex3d(x3, y3, z3);
        glVertex3d(x4, y4, z4);

    glEnd();
}

//---------------------------------------------------------------

static void prv_quadWithNormal(
                        double x1, double y1, double z1, double x2, double y2, double z2,
                        double x3, double y3, double z3, double x4, double y4, double z4,
                        double Nx, double Ny, double Nz)
{
    glBegin(GL_QUADS);

        glNormal3d(Nx, Ny, Nz);
        glVertex3d(x1, y1, z1);
        glVertex3d(x2, y2, z2);
        glVertex3d(x3, y3, z3);
        glVertex3d(x4, y4, z4);

    glEnd();
}

//---------------------------------------------------------------

static void prv_drawPrimitiveCube(void)
{
    prv_quadWithNormal(
                0., 0., 0.,
                1., 0., 0.,
                1., 0., 1.,
                0., 0., 1.,
                0., -1., 0.);

    prv_quadWithNormal(
                0., 1., 0.,
                0., 1., 1.,
                1., 1., 1.,
                1., 1., 0.,
                0., 1., 0.);

    prv_quadWithNormal(
                1., 1., 0.,
                1., 1., 1.,
                1., 0., 1.,
                1., 0., 0.,
                1., 0., 0.);

    prv_quadWithNormal(
                0., 1., 0.,
                0., 0., 0.,
                0., 0., 1.,
                0., 1., 1.,
                -1., 0., 0.);

    prv_quadWithNormal(
                1., 1., 1.,
                0., 1., 1.,
                0., 0., 1.,
                1., 0., 1.,
                0., 0., 1.);

    prv_quadWithNormal(
                0., 1., 0.,
                1., 1., 0.,
                1., 0., 0.,
                0., 0., 0.,
                0., 0., -1.);
}

//---------------------------------------------------------------

void CGraphicsGL::drawUnitCube(enum IGraphics::ETypeDrawSolid typeDraw)
{
    assert_no_null(m_dataPrivate);

    glPushAttrib(GL_ALL_ATTRIB_BITS);

    switch(typeDraw)
    {
        case IGraphics::TYPEDRAW_SMOOTH:
        case IGraphics::TYPEDRAW_SOLID:

            glPolygonMode(GL_FRONT, GL_FILL);
            break;

        case IGraphics::TYPEDRAW_WIRE:

            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;

        default_error();
    }

    prv_drawPrimitiveCube();

    glPopAttrib();
}

//---------------------------------------------------------------

static void prv_numSlideAndStacks(
                        enum IGraphics::ELevelDetail levelDetail,
                        unsigned long *numSlide,
                        unsigned long *numStacks)
{
    unsigned long numSlide_loc, numStacks_loc;

    assert_no_null(numSlide);
    assert_no_null(numStacks);

    switch(levelDetail)
    {
        case IGraphics::LEVELDETAIL_VERYHIGH:

            numSlide_loc = 40;
            numStacks_loc = 30;
            break;

        case IGraphics::LEVELDETAIL_HIGH:

            numSlide_loc = 30;
            numStacks_loc = 20;
            break;

        case IGraphics::LEVELDETAIL_NORMAL:

            numSlide_loc = 15;
            numStacks_loc = 10;
            break;

        case IGraphics::LEVELDETAIL_LOW:

            numSlide_loc = 10;
            numStacks_loc = 5;
            break;

        default_error();
    }

    *numSlide = numSlide_loc;
    *numStacks = numStacks_loc;
}

//---------------------------------------------------------------

static void prv_styleDraw(enum IGraphics::ETypeDrawSolid typeDraw, GLenum *drawStyle, GLenum *normals)
{
    GLenum drawStyle_loc, normals_loc;

    assert_no_null(drawStyle);
    assert_no_null(normals);

    switch(typeDraw)
    {
        case IGraphics::TYPEDRAW_SMOOTH:

            drawStyle_loc = GLU_FILL;
            normals_loc = GLU_SMOOTH;
            break;

        case IGraphics::TYPEDRAW_SOLID:

            drawStyle_loc = GLU_FILL;
            normals_loc = GLU_FLAT;
            break;

        case IGraphics::TYPEDRAW_WIRE:

            drawStyle_loc = GLU_LINE;
            normals_loc = GLU_NONE;
            break;

        default_error();
    }

    *drawStyle = drawStyle_loc;
    *normals = normals_loc;
}

//---------------------------------------------------------------

static void prv_prepareQuadric(
                        GLUquadricObj *quadric,
                        enum IGraphics::ELevelDetail levelDetail,
                        enum IGraphics::ETypeDrawSolid typeDraw)
{
    GLenum drawStyle, normals;

    prv_styleDraw(typeDraw, &drawStyle, &normals);

    gluQuadricDrawStyle(quadric, drawStyle);
    gluQuadricNormals(quadric, normals);
}

//---------------------------------------------------------------

void CGraphicsGL::drawUnitSphere(enum IGraphics::ELevelDetail levelDetail, enum IGraphics::ETypeDrawSolid typeDraw)
{
    unsigned long numSlide, numStacks;

    assert_no_null(m_dataPrivate);

    prv_prepareQuadric(m_dataPrivate->quadric, levelDetail, typeDraw);
    prv_numSlideAndStacks(levelDetail, &numSlide, &numStacks);
    gluSphere(m_dataPrivate->quadric, 1., numSlide, numStacks);
}

//---------------------------------------------------------------

void CGraphicsGL::drawUnitCylinder(enum IGraphics::ELevelDetail levelDetail, enum IGraphics::ETypeDrawSolid typeDraw)
{
    unsigned long numSlide, numStacks;

    assert_no_null(m_dataPrivate);

    prv_prepareQuadric(m_dataPrivate->quadric, levelDetail, typeDraw);
    prv_numSlideAndStacks(levelDetail, &numSlide, &numStacks);
    gluCylinder(m_dataPrivate->quadric, 1., 1., 1., numSlide, numStacks);
}

//---------------------------------------------------------------

void CGraphicsGL::drawUnitCone(enum IGraphics::ELevelDetail levelDetail, enum IGraphics::ETypeDrawSolid typeDraw)
{
    unsigned long numSlide, numStacks;

    assert_no_null(m_dataPrivate);

    prv_prepareQuadric(m_dataPrivate->quadric, levelDetail, typeDraw);
    prv_numSlideAndStacks(levelDetail, &numSlide, &numStacks);
    gluCylinder(m_dataPrivate->quadric, 1., 0., 1., numSlide, numStacks);
}

//---------------------------------------------------------------

void CGraphicsGL::drawUnitDisk(enum IGraphics::ELevelDetail levelDetail, enum IGraphics::ETypeDrawSolid typeDraw)
{
    unsigned long numSlide, numStacks;

    assert_no_null(m_dataPrivate);

    prv_prepareQuadric(m_dataPrivate->quadric, levelDetail, typeDraw);
    prv_numSlideAndStacks(levelDetail, &numSlide, &numStacks);
    gluDisk(m_dataPrivate->quadric, 0., 1., numSlide, numStacks);
}

//---------------------------------------------------------------

static void prv_transformSizeToWindows(
                        double width, double height,
                        double *widthWindows, double *heightWindows)
{
    GLdouble x1, y1, x2, y2, winx1, winy1, winz1, winx2, winy2, winz2;
    GLint viewport[4];
    GLdouble mvmatrix[16], projmatrix[16];

    assert_no_null(widthWindows);
    assert_no_null(heightWindows);

    x1 = 0.;
    y1 = 0.;
    x2 = width;
    y2 = height;

    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
    glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);

    gluProject(x1, y1, 0., mvmatrix, projmatrix, viewport, &winx1, &winy1, &winz1);
    gluProject(x2, y2, 0., mvmatrix, projmatrix, viewport, &winx2, &winy2, &winz2);

    *widthWindows = winx2 - winx1;
    *heightWindows = winy2 - winy1;
}

//---------------------------------------------------------------

void CGraphicsGL::drawImageRGB(double x, double y, double width, double height, const class CImg *image)
{
    GLenum format, type;
    double zoom_x, zoom_y;
    double widthWindows, heightWindows;
    unsigned long widthPixels, heightPixels, widthStep, nChannels;
    unsigned char *dataImage;

    assert_no_null(m_dataPrivate);
    assert_no_null(image);

    image->getDataRaw(&widthPixels, &heightPixels, &widthStep, &nChannels, &dataImage);

    if (nChannels == 4)
        format = GL_BGRA;
    else
    {
        assert(nChannels == 3);
        format = GL_BGR;
    }

    prv_transformSizeToWindows(width, height, &widthWindows, &heightWindows);

    zoom_x = widthWindows / widthPixels;
    zoom_y = heightWindows / heightPixels;

    glRasterPos2f(x, height + y);
    glPixelZoom(zoom_x, -zoom_y);

    type = GL_UNSIGNED_BYTE;
    glDrawPixels(widthPixels, heightPixels, format, type, dataImage);
}

//---------------------------------------------------------------

void CGraphicsGL::pointWindowToWorld(double xPixel, double yPixel, double *x, double *y)
{
    int realy;
    double xWorld_loc, yWorld_loc, zWorld_loc;
    GLint viewport[4];
    GLdouble mvmatrix[16], projmatrix[16];

    assert_no_null(m_dataPrivate);
    assert_no_null(x);
    assert_no_null(y);

    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
    glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);

    realy = viewport[3] - (GLint)yPixel - 1;
    gluUnProject((GLdouble) xPixel, (GLdouble)realy, 0., mvmatrix, projmatrix, viewport, &xWorld_loc, &yWorld_loc, &zWorld_loc);

    *x = xWorld_loc;
    *y = yWorld_loc;
}

//---------------------------------------------------------------

void CGraphicsGL::drawText(const char *text, double x, double y, double heigth)
{
    unsigned long lenghtText;

    assert_no_null(m_dataPrivate);
    assert(m_dataPrivate->isInitializeText == true);
    assert(m_dataPrivate->fontListBase != CMath::maxULong());

    lenghtText = CString::size(text);

    glPushMatrix();

    glRasterPos2f(x, y);

    glListBase(m_dataPrivate->fontListBase);
    glCallLists(lenghtText, GL_UNSIGNED_BYTE, text);

    glPopMatrix();
}

//---------------------------------------------------------------

unsigned long CGraphicsGL::beginBlock(void)
{
    assert_no_null(m_dataPrivate);
    assert(m_dataPrivate->idBlockCurrent == CMath::maxULong());

    m_dataPrivate->idBlockCurrent = glGenLists(1);
    glNewList(m_dataPrivate->idBlockCurrent, GL_COMPILE);

    return m_dataPrivate->idBlockCurrent;
}

//---------------------------------------------------------------

void CGraphicsGL::endBlock(unsigned long idBlock)
{
    assert_no_null(m_dataPrivate);
    assert(m_dataPrivate->idBlockCurrent == idBlock);
    glEndList();

    m_dataPrivate->ids_blocksRegisters->add(idBlock);

    m_dataPrivate->idBlockCurrent = CMath::maxULong();
}

//---------------------------------------------------------------

void CGraphicsGL::drawBlock(unsigned long idBlock)
{
    assert_no_null(m_dataPrivate);
    assert(m_dataPrivate->idBlockCurrent == CMath::maxULong());
    glCallList(idBlock);
}

//---------------------------------------------------------------

static void prv_drawElements(GLenum typePrimitive, const class CArray<CArrayULong> *indexsPrimitives)
{
    unsigned long num;

    num = indexsPrimitives->size();

    for (unsigned long i = 0; i < num; i++)
    {
        const class CArrayULong *indexs;
        unsigned int numIndexs;
        unsigned int *arrayIndexs;

        indexs = indexsPrimitives->get(i);
        assert_no_null(indexs);

        numIndexs = indexs->size();
        arrayIndexs = indexs->getArrayC();

        glDrawElements(typePrimitive, numIndexs, GL_UNSIGNED_INT, arrayIndexs);

        FREE_T(&arrayIndexs, unsigned int);
    }
}

//---------------------------------------------------------------

static void prv_drawNormalsDebug(
                        const class CArrPoint3d *points,
                        const class CArrPoint3d *normals)
{
    unsigned long numVertexs;
    static double PRV_DISTANCE_NORMAL = 0.5;

    numVertexs = points->size();
    assert(numVertexs == normals->size());

    glPushAttrib(GL_LINE_BIT | GL_ENABLE_BIT);
    glDisable(GL_TEXTURE_2D);
    glLineWidth(5.);

    glBegin(GL_LINES);

    for (unsigned long i = 0; i < numVertexs; i++)
    {
        double xorg, yorg, zorg, x1, y1, z1, x2, y2, z2;
        double Nx, Ny, Nz;

        points->get(i, &xorg, &yorg, &zorg);
        normals->get(i, &Nx, &Ny, &Nz);

        x1 = xorg - PRV_DISTANCE_NORMAL * Nx;
        y1 = yorg - PRV_DISTANCE_NORMAL * Ny;
        z1 = zorg - PRV_DISTANCE_NORMAL * Nz;

        x2 = xorg + PRV_DISTANCE_NORMAL * Nx;
        y2 = yorg + PRV_DISTANCE_NORMAL * Ny;
        z2 = zorg + PRV_DISTANCE_NORMAL * Nz;

        glColor4f(1.f, 0, 0, 1.);
        glVertex3d(xorg, yorg, zorg);
        glVertex3d(x1, y1, z1);
        glColor4f(0, 1.f, 0, 1.);
        glVertex3d(xorg, yorg, zorg);
        glVertex3d(x2, y2, z2);
    }

    glEnd();

    glPopAttrib();
}

//---------------------------------------------------------------

void CGraphicsGL::drawMesh(
                        const class CArrPoint3d *points,
                        const class CArrPoint3d *normals,
                        const class CArrPoint2d *textCoord_opt,
                        const class CArrayULong *indexTriangles,
                        const class CArray<CArrayULong> *indexTriangleFan,
                        const class CArray<CArrayULong> *indexTriangleStrip)
{
    unsigned long numVertexs, numIndexTriangles;
    float *arrayVertexs, *arrayNormals, *arrayTextCoord;
    static bool debug = false;

    assert_no_null(m_dataPrivate);

    numVertexs = points->size();
    assert(numVertexs == normals->size());
    if (textCoord_opt != NULL)
        assert(numVertexs == textCoord_opt->size());
    assert(numVertexs > 0);

    arrayVertexs = points->getArrayC();
    arrayNormals = normals->getArrayC();

    if (textCoord_opt != NULL)
        arrayTextCoord = textCoord_opt->getArrayC();
    else
        arrayTextCoord = NULL;

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, arrayVertexs);

    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, 0, arrayNormals);

    if (textCoord_opt != NULL)
    {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, 0, arrayTextCoord);
    }

    numIndexTriangles = indexTriangles->size();
    if (numIndexTriangles > 0)
    {
        unsigned int *arrayIndexTriangles;

        arrayIndexTriangles = indexTriangles->getArrayC();
        glDrawElements(GL_TRIANGLES, numIndexTriangles, GL_UNSIGNED_INT, arrayIndexTriangles);
        FREE_T(&arrayIndexTriangles, unsigned int);
    }

    prv_drawElements(GL_TRIANGLE_FAN, indexTriangleFan);
    prv_drawElements(GL_TRIANGLE_STRIP, indexTriangleStrip);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    if (textCoord_opt != NULL)
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    FREE_T(&arrayVertexs, float);
    FREE_T(&arrayNormals, float);

    if (textCoord_opt != NULL)
        FREE_T(&arrayTextCoord, float);

    if (debug == true)
        prv_drawNormalsDebug(points, normals);

}

//-----------------------------------------------------------------------

void CGraphicsGL::setModeTransparency(void)
{
    glAlphaFunc(GL_LEQUAL, 0.9);
    glEnable(GL_ALPHA_TEST);
    glDepthMask(GL_FALSE);
}

//-----------------------------------------------------------------------

void CGraphicsGL::setModeOpaque(void)
{
    glAlphaFunc(GL_GREATER, 0.9);
    glEnable(GL_ALPHA_TEST);
    glDepthMask(GL_TRUE);
}
