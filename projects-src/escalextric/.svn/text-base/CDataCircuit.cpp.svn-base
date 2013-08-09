//CDataCircuit.hpp: Definición del circuito del escalextric.

#include "CDataCircuit.inl"

#include "CArray.hpp"
#include "CDocXML.hpp"
#include "CString.hpp"
#include "CMath.hpp"

#include "asrtbas.h"
#include "memory.h"
#include "CGeometry.hpp"
#include "CVector.hpp"
#include "CGraphicsSprite.hpp"
#include "CArrPoint2d.hpp"
#include "CArrPoint3d.hpp"
#include "CGraphicsRect.hpp"
#include "CTransform2D.hpp"
#include "CPolylines.hpp"
#include "CMatrix4x4.hpp"
#include "CStackTransformation.hpp"
#include "CMesh.hpp"
#include "CMaterial.hpp"
#include "CModel3d.hpp"
#include "CGeneratorModel.hpp"
#include "CGeneratorSolid.hpp"

#include "CImg.hpp"
#include "CDataCircuit__straight_horizontal.iig"
#include "CDataCircuit__straight_vertical.iig"
#include "CDataCircuit__curve0_90.iig"
#include "CDataCircuit__curve90_180.iig"
#include "CDataCircuit__curve180_270.iig"
#include "CDataCircuit__curve270_360.iig"

enum EPrvTypeRotation
{
	PRV_ROAD_STRAIGHT, PRV_ROAD_CURVE_ROTATE_LEFT, PRV_ROAD_CURVE_ROTATE_RIGHT
};

enum EPrvTypeTrack
{
    PRV_HORIZONTAL, PRV_VERTICAL, PRV_CURVE_0_90, PRV_CURVE_90_180, PRV_CURVE_180_270, PRV_CURVE_270_360
};

enum EPrvDirection
{
    PRV_RIGHT, PRV_UP, PRV_LEFT, PRV_DOWN
};

struct SPrvTrack
{
	SPrvTrack(enum EPrvTypeTrack type, enum EPrvDirection directionEnd);
	~SPrvTrack(){;}

	void getDxDy(double *dx, double *dy) const;

	enum EPrvTypeTrack m_typeTrack;
	enum EPrvDirection m_directionEnd;
};

class CPrvProcessTracks: public IProcessNodeXML
{
	public:

		CPrvProcessTracks();
		~CPrvProcessTracks();
		virtual void processNode(const class CNodeXML *node);

		class CArray<SPrvTrack> *getTracks(void);

	private:

		class CArray<SPrvTrack> *m_tracks;
		enum EPrvDirection m_direction;
};

struct SPrvTransformTrack
{
    enum EPrvTypeRotation typeRotation;
    enum EPrvDirection directionBegin;
    enum EPrvDirection directionEnd;
    enum EPrvTypeTrack typeTrack;
};

struct SPrvRotateDirecion
{
    enum EPrvTypeRotation typeRotation;
    enum EPrvDirection directionBegin;
    enum EPrvDirection directionEnd;
};

static struct SPrvTransformTrack prv_TRANSFORMATION_TRACK[] =
{
   {PRV_ROAD_STRAIGHT, PRV_RIGHT, PRV_RIGHT, PRV_HORIZONTAL},
   {PRV_ROAD_STRAIGHT, PRV_UP, PRV_UP, PRV_VERTICAL},
   {PRV_ROAD_STRAIGHT, PRV_LEFT, PRV_LEFT, PRV_HORIZONTAL},
   {PRV_ROAD_STRAIGHT, PRV_DOWN, PRV_DOWN, PRV_VERTICAL},

   {PRV_ROAD_CURVE_ROTATE_LEFT, PRV_RIGHT, PRV_UP, PRV_CURVE_270_360},
   {PRV_ROAD_CURVE_ROTATE_LEFT, PRV_UP, PRV_LEFT, PRV_CURVE_0_90},
   {PRV_ROAD_CURVE_ROTATE_LEFT, PRV_LEFT, PRV_DOWN, PRV_CURVE_90_180},
   {PRV_ROAD_CURVE_ROTATE_LEFT, PRV_DOWN, PRV_RIGHT, PRV_CURVE_180_270},

   {PRV_ROAD_CURVE_ROTATE_RIGHT, PRV_RIGHT, PRV_DOWN, PRV_CURVE_0_90},
   {PRV_ROAD_CURVE_ROTATE_RIGHT, PRV_UP, PRV_RIGHT, PRV_CURVE_90_180},
   {PRV_ROAD_CURVE_ROTATE_RIGHT, PRV_LEFT, PRV_UP, PRV_CURVE_180_270},
   {PRV_ROAD_CURVE_ROTATE_RIGHT, PRV_DOWN, PRV_LEFT, PRV_CURVE_270_360}
};

struct SPrvDataPrivateCircuit
{
    double sizeXCircuit, sizeYCircuit;
    class CArray<SPrvTrack> *tracks;
    class CArrPoint2d *positionsTracks;
};

static unsigned long PRV_STEP_BY_TRACK = 16;
static unsigned long PRV_SIZE_X_TRACK_SPRITE = 32;
static unsigned long PRV_SIZE_Y_TRACK_SPRITE = 32;
static double PRV_SIZE_X_TRACK_MODEL3D = 1.;
static double PRV_SIZE_Y_TRACK_MODEL3D = 1.;
static double PRV_SIZE_Z_TRACK_MODEL3D = 0.1;

//-----------------------------------------------------------------------
//
SPrvTrack::SPrvTrack(enum EPrvTypeTrack type, enum EPrvDirection directionEnd)
{
	m_typeTrack = type;
	m_directionEnd = directionEnd;
}

//-----------------------------------------------------------------------
//
void SPrvTrack::getDxDy(double *dx, double *dy) const
{
    double dx_loc, dy_loc;

    assert_no_null(dx);
    assert_no_null(dy);

    switch(m_directionEnd)
    {
        case PRV_RIGHT:

            dx_loc = 1.;
            dy_loc = 0.;
            break;

        case PRV_UP:

            dx_loc = 0.;
            dy_loc = 1.;
            break;

        case PRV_LEFT:

            dx_loc = -1.;
            dy_loc = 0.;
            break;

        case PRV_DOWN:

            dx_loc = 0.;
            dy_loc = -1.;
            break;

        default_error();
    }

    *dx = dx_loc;
    *dy = dy_loc;
}

//-----------------------------------------------------------------------
//
static bool prv_isCounterClockWise(const struct SPrvTrack *track)
{
    bool isCounterClockwise;

    assert_no_null(track);

    switch(track->m_typeTrack)
    {
        case PRV_CURVE_0_90:
        {
            if (track->m_directionEnd == PRV_DOWN)
                isCounterClockwise = false;
            else
            {
                assert(track->m_directionEnd == PRV_LEFT);
                isCounterClockwise = true;
            }
            break;
        }
        case PRV_CURVE_90_180:
        {
            if (track->m_directionEnd == PRV_RIGHT)
                isCounterClockwise = false;
            else
            {
                assert(track->m_directionEnd == PRV_DOWN);
                isCounterClockwise = true;
            }
            break;
        }
        case PRV_CURVE_180_270:
        {
            if (track->m_directionEnd == PRV_UP)
                isCounterClockwise = false;
            else
            {
                assert(track->m_directionEnd == PRV_RIGHT);
                isCounterClockwise = true;
            }
            break;
        }
        case PRV_CURVE_270_360:
        {
            if (track->m_directionEnd == PRV_LEFT)
                isCounterClockwise = false;
            else
            {
                assert(track->m_directionEnd == PRV_UP);
                isCounterClockwise = true;
            }
            break;
        }
        case PRV_HORIZONTAL:
        {
            if (track->m_directionEnd == PRV_LEFT)
                isCounterClockwise = false;
            else
            {
                assert(track->m_directionEnd == PRV_RIGHT);
                isCounterClockwise = true;
            }
            break;
        }
        case PRV_VERTICAL:
        {
            if (track->m_directionEnd == PRV_UP)
                isCounterClockwise = true;
            else
            {
                assert(track->m_directionEnd == PRV_DOWN);
                isCounterClockwise = false;
            }
            break;
        }
        default_error();
    }

    return isCounterClockwise;
}

//-----------------------------------------------------------------------
//
static struct SPrvDataPrivateCircuit *prv_create(
                    double sizeXCircuit, double sizeYCircuit,
                    class CArray<SPrvTrack> **tracks,
                    class CArrPoint2d **positionsTracks)
{
	struct SPrvDataPrivateCircuit *data;

	data = MALLOC(struct SPrvDataPrivateCircuit);

	data->sizeXCircuit = sizeXCircuit;
	data->sizeYCircuit = sizeYCircuit;

    data->tracks = ASSIGN_PP_NO_NULL(tracks, class CArray<SPrvTrack>);
	data->positionsTracks = ASSIGN_PP_NO_NULL(positionsTracks, CArrPoint2d);

	return data;
}

//-----------------------------------------------------------------------
//
static void prv_destroy(struct SPrvDataPrivateCircuit **data)
{
	assert_no_null(data);
	assert_no_null(*data);

	delete (*data)->positionsTracks;
	delete (*data)->tracks;

	FREE_T(data, struct SPrvDataPrivateCircuit);
}

//-----------------------------------------------------------------------
//
CPrvProcessTracks::CPrvProcessTracks()
{
    m_tracks = new CArray<SPrvTrack>;
    m_direction = PRV_RIGHT;
}

//-----------------------------------------------------------------------
//
CPrvProcessTracks::~CPrvProcessTracks()
{
    assert(m_tracks == NULL);
}

//-----------------------------------------------------------------------
//
class CArray<SPrvTrack> *CPrvProcessTracks::getTracks(void)
{
    class CArray<SPrvTrack> *tracks;

    assert_no_null(m_tracks);

    tracks = m_tracks;
    m_tracks = NULL;

    return tracks;
}

//-----------------------------------------------------------------------
//
static enum EPrvTypeRotation prv_typeRotation(const class CNodeXML *node)
{
    enum EPrvTypeRotation typeRotation;
    char *idClass;
    bool existClass;

    assert_no_null(node);
    assert(node->isTag("Track") == true);

    idClass = NULL;
    existClass = node->getClass(&idClass);
    assert(existClass == true);

    if (CString::equal(idClass, "straight") == true)
        typeRotation = PRV_ROAD_STRAIGHT;
    else if (CString::equal(idClass, "RotateLeft") == true)
        typeRotation = PRV_ROAD_CURVE_ROTATE_LEFT;
    else
    {
        assert(CString::equal(idClass, "RotateRight") == true);
        typeRotation = PRV_ROAD_CURVE_ROTATE_RIGHT;
    }

    CString::free(&idClass);

    return typeRotation;
}

//-----------------------------------------------------------------------
//
static void prv_applyRotation(
                enum EPrvTypeRotation typeRotation,
                enum EPrvDirection directionBegin,
                enum EPrvTypeTrack *typeTrack,
                enum EPrvDirection *directionEnd)
{
    unsigned long i, num;

    assert_no_null(typeTrack);
    assert_no_null(directionEnd);

    num = sizeof(prv_TRANSFORMATION_TRACK) / sizeof(prv_TRANSFORMATION_TRACK[0]);
    assert(num > 0);

    for (i = 0; i < num; i++)
    {
        if (prv_TRANSFORMATION_TRACK[i].typeRotation == typeRotation
                && prv_TRANSFORMATION_TRACK[i].directionBegin == directionBegin)
        {
            break;
        }
    }

    assert(i < num);

    *typeTrack = prv_TRANSFORMATION_TRACK[i].typeTrack;
    *directionEnd = prv_TRANSFORMATION_TRACK[i].directionEnd;
}

//-----------------------------------------------------------------------
//
void CPrvProcessTracks::processNode(const class CNodeXML *node)
{
	class SPrvTrack *track;
	enum EPrvTypeRotation typeRotation;
	enum EPrvDirection nextDirection;
	enum EPrvTypeTrack typeTrack;

	typeRotation = prv_typeRotation(node);
	prv_applyRotation(typeRotation, m_direction, &typeTrack, &nextDirection);

	track = new SPrvTrack(typeTrack, nextDirection);
	m_tracks->add(track);

	m_direction = nextDirection;
}

//-----------------------------------------------------------------------
//
static class CArray<SPrvTrack> *prv_readTracks(class CNodeXML *nodeCircuit)
{
	class CPrvProcessTracks *processTracks;
	class CArray<SPrvTrack> *tracks;

	processTracks = new CPrvProcessTracks();

	nodeCircuit->processAllChildren(processTracks);

	tracks = processTracks->getTracks();

    delete processTracks;

	return tracks;
}

//-----------------------------------------------------------------------
//
static void prv_OriginAndSizeCircuit(
                    const CArray<SPrvTrack> *tracks,
                    double *xMin, double *yMin, double *xMax, double *yMax)
{
    double x, y;
    unsigned long numTrack;
    class CGraphicsRect *graphicsRect;
    class IGraphics *graphics;
    bool hasLimits;
    double zMin, zMax;

    assert_no_null(tracks);
    assert_no_null(xMin);
    assert_no_null(yMin);
    assert_no_null(xMax);
    assert_no_null(yMax);

    x = 0.;
    y = 0.;

    graphicsRect = new CGraphicsRect();
    graphics = graphicsRect;

    numTrack = tracks->size();
    assert(numTrack > 0);

    for (unsigned long i = 0; i < numTrack; i++)
    {
        double dx, dy;
        const struct SPrvTrack *track;

        track = tracks->get(i);
        assert_no_null(track);

        graphics->drawLine(x, y, 0., x, y, 0.);

        track->getDxDy(&dx, &dy);

        x += dx;
        y += dy;
    }

    hasLimits = graphicsRect->getLimits(xMin, yMin, &zMin, xMax, yMax, &zMax);
    assert(hasLimits == true);

    delete graphicsRect;
}

//-----------------------------------------------------------------------
//
static class CArrPoint2d *prv_calculatePositionTracks(const class CArray<SPrvTrack> *tracks, double *sizeXCircuit, double *sizeYCircuit)
{
    class CArrPoint2d *positionsTracks;
    unsigned long numTracks;
    double xMin, yMin, xMax, yMax;
    double x, y;

    assert_no_null(tracks);
    assert_no_null(sizeXCircuit);
    assert_no_null(sizeYCircuit);

    prv_OriginAndSizeCircuit(tracks, &xMin, &yMin, &xMax, &yMax);

    x = -xMin;
    y = -yMin;

    numTracks = tracks->size();
    positionsTracks = new CArrPoint2d(numTracks);

    for (unsigned long i = 0; i < numTracks; i++)
    {
        const struct SPrvTrack *track;
        double dx, dy;

        track = tracks->get(i);
        track->getDxDy(&dx, &dy);

        positionsTracks->set(i, x, y);

        x += dx;
        y += dy;
    }

    *sizeXCircuit = xMax - xMin;
    *sizeYCircuit = yMax - yMin;

    return positionsTracks;
}

//-----------------------------------------------------------------------
//
CDataCircuit::CDataCircuit(const char *fileNameXML)
{
    class CArrPoint2d *positionsTracks;
	class CArray<SPrvTrack> *tracks;
	class CDocXML *doc;
	class CNodeXML *nodeCircuit;
	bool isRead;
	double sizeXCircuit, sizeYCircuit;

	doc = NULL;
	isRead = CDocXML::parseXML(fileNameXML, &doc);
	assert(isRead == true);

	nodeCircuit = doc->rootNode();
	assert(nodeCircuit->isTag("Circuit") == true);

	tracks = prv_readTracks(nodeCircuit);
	positionsTracks = prv_calculatePositionTracks(tracks, &sizeXCircuit, &sizeYCircuit);

	m_dataPrivate = prv_create(sizeXCircuit, sizeYCircuit, &tracks, &positionsTracks);

	delete doc;
}

//-----------------------------------------------------------------------
//
CDataCircuit::~CDataCircuit()
{
	assert_no_null(m_dataPrivate);
	prv_destroy(&m_dataPrivate);
}

//-----------------------------------------------------------------------
//
static class CImg *prv_getImage(enum EPrvTypeTrack typeTrack)
{
    class CImg *imageTrack;

    switch (typeTrack)
    {
        case PRV_HORIZONTAL:

            imageTrack = cimg_load_local_png(horizontal);
            break;

        case PRV_VERTICAL:

            imageTrack = cimg_load_local_png(vertical);
            break;

        case PRV_CURVE_0_90:

            imageTrack = cimg_load_local_png(90);
            break;

        case PRV_CURVE_90_180:

            imageTrack = cimg_load_local_png(180);
            break;

        case PRV_CURVE_180_270:

            imageTrack = cimg_load_local_png(270);
            break;

        case PRV_CURVE_270_360:

            imageTrack = cimg_load_local_png(360);
            break;

        default_error();
    }

    return imageTrack;
}

//-----------------------------------------------------------------------
//
class CImg *CDataCircuit::image(unsigned long *sizeXTrack, unsigned long *sizeYTrack) const
{
    class CImg *imageCircuit;
    double sizeXPixels, sizeYPixels;
    class CGraphicsSprite *graphicsSprite;
    unsigned long numTracks;

    assert_no_null(m_dataPrivate);
    assert_no_null(m_dataPrivate->tracks);
    assert_no_null(sizeXTrack);
    assert_no_null(sizeYTrack);

    sizeXPixels = PRV_SIZE_X_TRACK_SPRITE * (m_dataPrivate->sizeXCircuit + 1.);
    sizeYPixels = PRV_SIZE_Y_TRACK_SPRITE * (m_dataPrivate->sizeYCircuit + 1.);

    graphicsSprite = new CGraphicsSprite((unsigned long)sizeXPixels, (unsigned long)sizeYPixels, 0, 0, 0);

    numTracks = m_dataPrivate->tracks->size();
    assert(numTracks == m_dataPrivate->positionsTracks->size());

    graphicsSprite->setOriginCartesian();
    graphicsSprite->pushTransformation();
    graphicsSprite->scale(PRV_SIZE_X_TRACK_SPRITE, PRV_SIZE_Y_TRACK_SPRITE);

    for (unsigned long i = 0; i < numTracks; i++)
    {
        const struct SPrvTrack *track;
        double x, y;
        class CImg *image;

        track = m_dataPrivate->tracks->get(i);
        m_dataPrivate->positionsTracks->get(i, &x, &y);

        graphicsSprite->pushTransformation();

        graphicsSprite->translate(x, y);

        image = prv_getImage(track->m_typeTrack);
        graphicsSprite->drawImage(image);
        delete image;

        graphicsSprite->popTransformation();
    }

    imageCircuit = graphicsSprite->getImage();
    graphicsSprite->popTransformation();

    delete graphicsSprite;

    *sizeXTrack = PRV_SIZE_X_TRACK_SPRITE;
    *sizeYTrack = PRV_SIZE_Y_TRACK_SPRITE;

    return imageCircuit;
}

//-----------------------------------------------------------------------

static class CArrPoint3d *prv_polylineCurveToPath(double width, double height)
{
    class CArrPoint3d *points;

    points = new CArrPoint3d(5);

    points->set(0, width, 0., height);
    points->set(1, width, 0., 0.);
    points->set(2, 0., 0., 0.);
    points->set(3, 0., 0., height);
    points->set(4, width, 0., height);

    return points;
}

//-----------------------------------------------------------------------

static class CMesh *prv_polylinesCurveToPath(double angleCurve, double angleRotation, double dx, double dy)
{
    class CMesh *meshCurve;
    unsigned long numPolylines;
    class CArrPoint3d *polyline;
    class CArray<CArrPoint3d> *polylinesInPathWithSamePoints;
    class CStackTransformation *stackTransformation;
    double incrAngleDegree;

    numPolylines = 32;
    polyline = prv_polylineCurveToPath(PRV_SIZE_X_TRACK_MODEL3D - 0.2, PRV_SIZE_Z_TRACK_MODEL3D);

    stackTransformation = new CStackTransformation();

    stackTransformation->pushTransformation();
    stackTransformation->appendTranslation(0.1, 0., 0.);

    stackTransformation->transformationPoints3d(polyline);

    stackTransformation->popTransformation();

    polylinesInPathWithSamePoints = new CArray<CArrPoint3d>(numPolylines);

    stackTransformation->pushTransformation();

    incrAngleDegree = angleCurve / (numPolylines - 1);

    for (unsigned long i = 0; i < numPolylines; i++)
    {
        class CArrPoint3d *polyline3d;

        polyline3d = new CArrPoint3d(polyline);
        stackTransformation->transformationPoints3d(polyline3d);
        polylinesInPathWithSamePoints->set(i, polyline3d);

        stackTransformation->appendRotation(incrAngleDegree, 0., 0., 1.);
    }

    stackTransformation->popTransformation();

    meshCurve = CGeneratorModel::createWithPathOpen((const class CArrayRef<CArrPoint3d> *)polylinesInPathWithSamePoints);

    stackTransformation->appendTranslation(dx, dy, 0.);
    stackTransformation->appendRotation(angleRotation, 0., 0., 1.);

    meshCurve->applyTransformation(stackTransformation);

    delete stackTransformation;
    delete polyline;
    delete polylinesInPathWithSamePoints;

    return meshCurve;
}

//-----------------------------------------------------------------------

static class CMesh *prv_polylinesStraight(double width, double height, double thickness, bool isHorizontal)
{
    class CMesh *meshStraight;
    class CStackTransformation *stackTransformation;

    stackTransformation = new CStackTransformation();

    if (isHorizontal == true)
    {
        stackTransformation->appendTranslation(0., 0.1, 0.);
        stackTransformation->appendScale(width, height - 0.2, thickness);
    }
    else
    {
        stackTransformation->appendTranslation(0.1, 0., 0.);
        stackTransformation->appendScale(width - 0.2, height, thickness);
    }

    meshStraight = CGeneratorSolid::createUnitCube();

    meshStraight->applyTransformation(stackTransformation);

    delete stackTransformation;

    return meshStraight;
}

//-----------------------------------------------------------------------
//
static class CMesh *prv_createMeshTrack(enum EPrvTypeTrack typeTrack)
{
    class CMesh *meshTrack;

    switch(typeTrack)
    {
        case PRV_HORIZONTAL:

            meshTrack = prv_polylinesStraight(PRV_SIZE_X_TRACK_MODEL3D, PRV_SIZE_Y_TRACK_MODEL3D, PRV_SIZE_Z_TRACK_MODEL3D, true);
            break;

        case PRV_VERTICAL:

            meshTrack = prv_polylinesStraight(PRV_SIZE_Y_TRACK_MODEL3D, PRV_SIZE_X_TRACK_MODEL3D, PRV_SIZE_Z_TRACK_MODEL3D, false);
            break;

        case PRV_CURVE_0_90:

            meshTrack = prv_polylinesCurveToPath(90., 0., 0., 0.);
            break;

        case PRV_CURVE_90_180:

            meshTrack = prv_polylinesCurveToPath(90., 90., 1., 0.);
            break;

        case PRV_CURVE_180_270:

            meshTrack = prv_polylinesCurveToPath(90., 180., 1., 1.);
            break;

        case PRV_CURVE_270_360:

            meshTrack = prv_polylinesCurveToPath(90., 270., 0., 1.);
            break;

        default_error();
    }

    return meshTrack;
}

//-----------------------------------------------------------------------
//
class CModel3d *CDataCircuit::model3d(void) const
{
    class CModel3d *modelCircuit;
    class CMaterial *materialCircuit;
    class CStackTransformation *stackTransformation;
    unsigned long numTracks;
    const char *PRV_MATERIAL_CIRCUIT = "Circuit";

    assert_no_null(m_dataPrivate);
    assert_no_null(m_dataPrivate->tracks);

    materialCircuit = new CMaterial(PRV_MATERIAL_CIRCUIT, 0.5, 0.5, 0.5, 1.);

    modelCircuit = new CModel3d;
    modelCircuit->appendMaterial(&materialCircuit);

    stackTransformation = new CStackTransformation;

    numTracks = m_dataPrivate->tracks->size();
    assert(numTracks == m_dataPrivate->positionsTracks->size());

    for (unsigned long i = 0; i < numTracks; i++)
    {
        const struct SPrvTrack *track;
        double x, y;
        class CMesh *meshTrack;

        track = m_dataPrivate->tracks->get(i);
        m_dataPrivate->positionsTracks->get(i, &x, &y);

        meshTrack = prv_createMeshTrack(track->m_typeTrack);

        stackTransformation->pushTransformation();

        stackTransformation->appendTranslation(x, y, -PRV_SIZE_Z_TRACK_MODEL3D);

        meshTrack->applyTransformation(stackTransformation);

        modelCircuit->appendMesh(PRV_MATERIAL_CIRCUIT, meshTrack);
        delete meshTrack;

        stackTransformation->popTransformation();
    }

    delete stackTransformation;

    return modelCircuit;
}

//-----------------------------------------------------------------------
//
void CDataCircuit::applyVelocity(unsigned long velocity, double radius, unsigned long *indTrack, double *stepInTrack) const
{
    unsigned long numTracks;
    unsigned long incrTrack;
    double distance, incrByStep;
    const struct SPrvTrack *track;

    assert_no_null(m_dataPrivate);
    assert_no_null(indTrack);
    assert_no_null(stepInTrack);
    assert(radius > 0.);

    incrByStep = 1. / PRV_STEP_BY_TRACK;
    numTracks = m_dataPrivate->tracks->size();
    track = m_dataPrivate->tracks->get(*indTrack);

    switch(track->m_typeTrack)
    {
        case PRV_HORIZONTAL:
        case PRV_VERTICAL:

            distance = *stepInTrack + velocity * incrByStep;
            break;

        case PRV_CURVE_0_90:
        case PRV_CURVE_90_180:
        case PRV_CURVE_180_270:
        case PRV_CURVE_270_360:
        {
            double angularVelocity, radiusTrack;

            if (prv_isCounterClockWise(track) == true)
                radiusTrack = (1. - radius);
            else
                radiusTrack = radius;

            angularVelocity = velocity / radiusTrack;
            distance = *stepInTrack + angularVelocity * incrByStep;
            break;
        }
        default_error();
    }

    if (distance > 1.)
    {
        incrTrack = CMath::roundu(distance);
        distance = distance - (double)incrTrack;
    }
    else
        incrTrack = 0;

    *indTrack = (*indTrack + incrTrack) % numTracks;
    *stepInTrack = distance;
}

//-----------------------------------------------------------------------
//
static void prv_anglesTrack(
                    const struct SPrvTrack *track,
                    double *angleBeginDegree, double *angleEndDegree)
{
    double angleBeginDegree_loc, angleEndDegree_loc;

    switch(track->m_typeTrack)
    {
        case PRV_CURVE_0_90:
        {
            angleBeginDegree_loc = 0.;
            angleEndDegree_loc = 90.;
            break;
        }
        case PRV_CURVE_90_180:
        {
            angleBeginDegree_loc = 90.;
            angleEndDegree_loc = 180.;
            break;
        }
        case PRV_CURVE_180_270:
        {
            angleBeginDegree_loc = 180.;
            angleEndDegree_loc = 270.;
            break;
        }
        case PRV_CURVE_270_360:
        {
            angleBeginDegree_loc = 270.;
            angleEndDegree_loc = 360.;
            break;
        }
        case PRV_HORIZONTAL:
        case PRV_VERTICAL:
        default_error();
    }

    *angleBeginDegree =  angleBeginDegree_loc;
    *angleEndDegree = angleEndDegree_loc;
}

//-----------------------------------------------------------------------

static void prv_traslateOrigin(const struct SPrvTrack *track, double *dx, double *dy)
{
    double dx_loc, dy_loc;

    assert_no_null(track);
    assert_no_null(dx);
    assert_no_null(dy);

    switch(track->m_typeTrack)
    {
        case PRV_CURVE_0_90:
        case PRV_HORIZONTAL:
        {
            dx_loc = 0.;
            dy_loc = 0.;
            break;
        }
        case PRV_CURVE_90_180:
        case PRV_VERTICAL:
        {
            dx_loc = 1.;
            dy_loc = 0.;
            break;
        }
        case PRV_CURVE_180_270:
        {
            dx_loc = 1.;
            dy_loc = 1.;
            break;
        }
        case PRV_CURVE_270_360:
        {
            dx_loc = 0.;
            dy_loc = 1.;
            break;
        }
        default_error();
    }

    *dx = dx_loc;
    *dy = dy_loc;
}

//-----------------------------------------------------------------------

static void prv_getPositionLane(
                        const class CArray<SPrvTrack> *tracks,
                        const class CArrPoint2d *positionsTracks,
                        double sizeYCircuit,
                        unsigned long indTrack,
                        double tx, double ty,
                        double *xPos, double *yPos, double *angleRotation)
{
    double x, y;
    double Ux, Uy;
    struct SPrvTrack *track;
    double dx, dy;
    double xRelative, yRelative;
    double dxOrigin, dyOrigin, angleRotation_loc;

    assert_no_null(xPos);
    assert_no_null(yPos);
    assert_no_null(angleRotation);

    track = tracks->get(indTrack);

    if (prv_isCounterClockWise(track) == true)
    {
        dx = tx;
        dy = ty;
    }
    else
    {
        dx = (1. - tx);
        dy = (1. - ty);
    }

    switch(track->m_typeTrack)
    {
        case PRV_HORIZONTAL:

            Ux = 1.;
            Uy = 0.;

            angleRotation_loc = 0.;
            break;

        case PRV_VERTICAL:

            Ux = 0.;
            Uy = 1.;

            angleRotation_loc = CMath::PI / 2.;
            break;

        case PRV_CURVE_90_180:
        case PRV_CURVE_180_270:
        case PRV_CURVE_270_360:
        case PRV_CURVE_0_90:
        {
            double angleDegree, angle, angleBeginDegree, angleEndDegree;

            prv_anglesTrack(track, &angleBeginDegree, &angleEndDegree);
            angleDegree = dx * (angleEndDegree - angleBeginDegree) + angleBeginDegree;
            angle = CMath::degreeToRadian(angleDegree);
            CVector::directorVectorAngle(angle, &Ux, &Uy);

            angleRotation_loc = CMath::arctan(Ux, -Uy);

            dx = (1. - dy);
            dy = 0.;
            break;
        }
        default_error();
    }

    CGeometry::traslatePoint2D(0., 0., dx, dy, Ux, Uy, &xRelative, &yRelative);

    positionsTracks->get(indTrack, &x, &y);
    prv_traslateOrigin(track, &dxOrigin, &dyOrigin);

    if (prv_isCounterClockWise(track) == false)
        angleRotation_loc += CMath::PI;

    *angleRotation = angleRotation_loc;

    *xPos = x + dxOrigin + xRelative;
    *yPos = y + dyOrigin + yRelative;
}

//-----------------------------------------------------------------------
//
void CDataCircuit::getPositionLane(
                    unsigned long indTrack,
                    double stepInTrack,
                    double traslateLane,
                    double *xPos, double *yPos,
                    double *angleRotation) const
{
    assert_no_null(m_dataPrivate);
    prv_getPositionLane(
                    m_dataPrivate->tracks,
                    m_dataPrivate->positionsTracks,
                    m_dataPrivate->sizeYCircuit,
                    indTrack,
                    stepInTrack, traslateLane,
                    xPos, yPos,
                    angleRotation);
}
