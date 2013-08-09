/* -----------------------------------------------------------------------------
 *  CWorldRobot.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CWorldSimulationRobot.inl"

#include "CRobotSprites.inl"

#include "asrtbas.h"
#include "memory.h"

#include "CImg.hpp"
#include "CMath.hpp"
#include "CStackTransformation.hpp"
#include "CArraysTypes.hpp"
#include "CArrayString.hpp"
#include "CTransform.hpp"
#include "CTransform2D.hpp"
#include "CTransform3D.hpp"

enum prv_typeCell_t
{
    PRV_OBSTACLE, PRV_FREE, PRV_MARK1, PRV_MARK2, PRV_MARK3, PRV_MARK4
};

struct prv_dataPrivateWorldSimulationRobot_t
{
    bool is3d;

    long x, y;
    double angleRobot;

    class CImg *map;

    double angleRobotComand;
    class CArrayDouble *anglesRobot;
    class CStackTransformation *stackTransformations;
};

static double PRV_EPSILON = 1e-5;

//-----------------------------------------------------------------------

static void prv_integrityWorldSimulationRobot(const struct prv_dataPrivateWorldSimulationRobot_t *dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(dataPrivate->map);
    assert_no_null(dataPrivate->anglesRobot);
    assert_no_null(dataPrivate->stackTransformations);
}

//-----------------------------------------------------------------------

static struct prv_dataPrivateWorldSimulationRobot_t *prv_createWorldRobot(
                    bool is3d,
                    long x, long y,
                    double angleRobot, class CImg **map,
                    double angleRobotComand,
                    class CArrayDouble **anglesRobot,
                    class CStackTransformation **stackTransformations)
{
    struct prv_dataPrivateWorldSimulationRobot_t *dataPrivate;

    dataPrivate = MALLOC(struct prv_dataPrivateWorldSimulationRobot_t);

    dataPrivate->is3d = is3d;
    dataPrivate->x = x;
    dataPrivate->y = y;
    dataPrivate->angleRobot = angleRobot;
    dataPrivate->map = ASSIGN_PP_NO_NULL(map, class CImg);

    dataPrivate->angleRobotComand = angleRobotComand;
    dataPrivate->anglesRobot = ASSIGN_PP_NO_NULL(anglesRobot, class CArrayDouble);
    dataPrivate->stackTransformations = ASSIGN_PP_NO_NULL(stackTransformations, class CStackTransformation);

    prv_integrityWorldSimulationRobot(dataPrivate);

    return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroyWorldRobot(struct prv_dataPrivateWorldSimulationRobot_t **dataPrivate)
{
    assert_no_null(dataPrivate);
    prv_integrityWorldSimulationRobot(*dataPrivate);

    DELETE_OBJECT(&(*dataPrivate)->map, class CImg);
    DELETE_OBJECT(&(*dataPrivate)->anglesRobot, class CArrayDouble);
    DELETE_OBJECT(&(*dataPrivate)->stackTransformations, class CStackTransformation);

    FREE_T(dataPrivate, struct prv_dataPrivateWorldSimulationRobot_t);
}

//-----------------------------------------------------------------------

CWorldSimulationRobot::CWorldSimulationRobot(bool is3d, class CImg **map)
{
    long x, y;
    double angleRobot;
    double angleRobotComand;
    class CArrayDouble *anglesRobot;
    class CStackTransformation *stackTransformations;

    x = 4;
    y = 3;
    angleRobot = 0.;

    angleRobotComand = 0.;
    anglesRobot = new CArrayDouble;
    stackTransformations = new class CStackTransformation;

    m_dataPrivate = prv_createWorldRobot(is3d, x, y, angleRobot, map, angleRobotComand, &anglesRobot, &stackTransformations);
}

//-----------------------------------------------------------------------

CWorldSimulationRobot::~CWorldSimulationRobot()
{
    prv_destroyWorldRobot(&m_dataPrivate);
}

//-----------------------------------------------------------------------

class CAgent *CWorldSimulationRobot::move(const char *idElement, class CAgent **figure) const
{
    class CAgent *robotTraslated;
    class CAgent *rotation;
    double xPixel, yPixel;

    prv_integrityWorldSimulationRobot(m_dataPrivate);

    xPixel = m_dataPrivate->x;
    yPixel = m_dataPrivate->y;

    if (m_dataPrivate->is3d == true)
    {
        double angleRobotDegree;

        angleRobotDegree = CMath::radianToDegree(m_dataPrivate->angleRobot);

        rotation = CTransform3D::createRotate3D(figure, angleRobotDegree, 0., 0., 1.);
        robotTraslated = CTransform3D::createTraslate3D(&rotation, xPixel, yPixel, 0.);
    }
    else
    {
        rotation = CTransform2D::createRotate2D(figure, m_dataPrivate->angleRobot);
        robotTraslated = CTransform2D::createTraslate2D(&rotation, xPixel, yPixel);
    }

    return robotTraslated;
}

//-----------------------------------------------------------------------

static void prv_getPivotPoint(double xRobot, double yRobot, double anglePivot, double distance, long *xPivot, long *yPivot)
{
    double x, y;

    assert_no_null(xPivot);
    assert_no_null(yPivot);

    x = xRobot + CMath::cosine(anglePivot) * distance;
    y = yRobot - CMath::sine(anglePivot) * distance;

    *xPivot = CMath::round(x);
    *yPivot = CMath::round(y);
}

//-----------------------------------------------------------------------

void CWorldSimulationRobot::getObstaclesSimulation(
                        double angleSpectrum, double incrAngleSpectrum,
                        double distanceSpectrum, double incrDistanceSpectrum,
                        class CArrayDouble **distances, class CArrayDouble **angles) const
{
    double angleEnd, distanceEnd;
    class CArrayDouble *distances_loc, *angles_loc;

    prv_integrityWorldSimulationRobot(m_dataPrivate);
    assert_no_null(distances);
    assert_no_null(angles);

    distances_loc = new CArrayDouble;
    angles_loc = new CArrayDouble;

    angleEnd = angleSpectrum + PRV_EPSILON;
    distanceEnd = distanceSpectrum + PRV_EPSILON;

    for (double angle = 0.; angle < angleEnd; angle += incrAngleSpectrum)
    {
        double anglePivot;

        anglePivot = m_dataPrivate->angleRobot - angleSpectrum / 2. + angle;

        for (double distance = incrDistanceSpectrum; distance < distanceEnd; distance += incrDistanceSpectrum)
        {
            long xPivot, yPivot;

            prv_getPivotPoint(m_dataPrivate->x, m_dataPrivate->y, anglePivot, distance, &xPivot, &yPivot);

            if (CRobotSprites::isObstable(m_dataPrivate->map, xPivot, yPivot) == true)
            {
                distances_loc->add(distance);
                angles_loc->add(anglePivot - m_dataPrivate->angleRobot);
            }
        }
    }

    *distances = distances_loc;
    *angles = angles_loc;
}

//-----------------------------------------------------------------------

void CWorldSimulationRobot::getMarkSimulation(
                        double angleFocus, double distanceFocus,
                        class CArrayString **marks) const
{
    class CArrayString *marks_loc;
    double oneDegreeInRadians;
    double angleEnd, distanceEnd;

    prv_integrityWorldSimulationRobot(m_dataPrivate);
    assert_no_null(marks);

    marks_loc = new CArrayString;
    angleEnd = angleFocus + PRV_EPSILON;
    distanceEnd = distanceFocus + PRV_EPSILON;

    oneDegreeInRadians = CMath::degreeToRadian(1.);

    for (double angle = 0.; angle < angleEnd; angle += oneDegreeInRadians)
    {
        for (double distance = 1.; distance < distanceEnd; distance += 1.)
        {
            long xPivot, yPivot;
            double anglePivot;
            char *markObjective;

            anglePivot = m_dataPrivate->angleRobot - angleFocus / 2. + angle;
            prv_getPivotPoint(m_dataPrivate->x, m_dataPrivate->y, anglePivot, distance, &xPivot, &yPivot);

            markObjective = NULL;
            if (CRobotSprites::cellWithMark(m_dataPrivate->map, xPivot, yPivot, &markObjective) == true)
                marks_loc->addDestroyingString(&markObjective);
        }
    }

    *marks = marks_loc;
}

//-----------------------------------------------------------------------

void CWorldSimulationRobot::pushTransformation(void)
{
    prv_integrityWorldSimulationRobot(m_dataPrivate);

    m_dataPrivate->stackTransformations->pushTransformation();
    m_dataPrivate->anglesRobot->add(m_dataPrivate->angleRobotComand);
}

//-----------------------------------------------------------------------

void CWorldSimulationRobot::rotate(double angle)
{
    double angleDegree;

    prv_integrityWorldSimulationRobot(m_dataPrivate);

    m_dataPrivate->angleRobotComand += angle;

    angleDegree = CMath::radianToDegree(angle);
    m_dataPrivate->stackTransformations->appendRotation(angleDegree, 0., 0., 1.);
}

//-----------------------------------------------------------------------

void CWorldSimulationRobot::traslate(double dx, double dy)
{
    prv_integrityWorldSimulationRobot(m_dataPrivate);
    m_dataPrivate->stackTransformations->appendTranslation(dx, dy, 0.);
}

//-----------------------------------------------------------------------

void CWorldSimulationRobot::applyTraslate(void)
{
    double cosineRobot, sineRobot;
    double dxRobot, dyRobot;
    long dxRobotRounded, dyRobotRounded;
    double dx, dy, dz;

    prv_integrityWorldSimulationRobot(m_dataPrivate);

    m_dataPrivate->stackTransformations->transformationPoint3d(0., 0., 0., &dx, &dy, &dz);
    m_dataPrivate->angleRobot += m_dataPrivate->angleRobotComand;

    cosineRobot = CMath::cosine(m_dataPrivate->angleRobot);
    sineRobot = -CMath::sine(m_dataPrivate->angleRobot);

    dxRobot = dx * cosineRobot - dy * sineRobot;
    dyRobot = dx * sineRobot   + dy * cosineRobot;
    dxRobotRounded = (long)dxRobot;
    dyRobotRounded = (long)dyRobot;

    m_dataPrivate->x += dxRobotRounded;
    m_dataPrivate->y += dyRobotRounded;
}

//-----------------------------------------------------------------------

void CWorldSimulationRobot::popTransformation(void)
{
    prv_integrityWorldSimulationRobot(m_dataPrivate);

    m_dataPrivate->stackTransformations->popTransformation();
    m_dataPrivate->angleRobotComand = m_dataPrivate->anglesRobot->getLast();
    m_dataPrivate->anglesRobot->eraseLast();
}

