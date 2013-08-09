/* -----------------------------------------------------------------------------
 *  CPhysicLaws.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CWorldEscalextric.inl"

#include "CDataCircuit.inl"

#include "asrtbas.h"
#include "memory.h"

#include "CMath.hpp"
#include "CTransform.hpp"
#include "CTransform2D.hpp"
#include "CTransform3D.hpp"
#include "CArray.hpp"
#include "CString.hpp"

enum EPrvDimensionWorld
{
    PRV_NO_DIMENSION,
    PRV_DIMENSION_2D,
    PRV_DIMENSION_3D
};

enum EPrvLane
{
    PRV_LANE_LEFT,
    PRV_LANE_RIGHT
};

struct SPrvCar
{
    SPrvCar(const char *idCar, enum EPrvLane lane, unsigned long indTrack, double stepInTrack, unsigned long velocity);
    ~SPrvCar();

    char *m_idCar;
    enum EPrvLane m_lane;
    unsigned long m_indTrack;
    double m_stepInTrack;
    unsigned long m_velocity;
};

struct SPrvDataPrivateWorldEscalextric
{
    enum EPrvDimensionWorld dimensionWorld;
    class CArray<SPrvCar> *cars;
    const class CDataCircuit *dataCircuit;
};

//-----------------------------------------------------------------------

SPrvCar::SPrvCar(const char *idCar, enum EPrvLane lane, unsigned long indTrack, double stepInTrack, unsigned long velocity)
{
    m_idCar = CString::copy(idCar);
    m_lane = lane;
    m_indTrack = indTrack;
    m_stepInTrack = stepInTrack;
    m_velocity = velocity;
}

//-----------------------------------------------------------------------

SPrvCar::~SPrvCar()
{
    CString::free(&m_idCar);
}

//-----------------------------------------------------------------------

static bool prv_isCar(const struct SPrvCar *car, const char *idCar)
{
    return CString::equal(car->m_idCar, idCar);
}

//-----------------------------------------------------------------------

static void prv_integrity(const struct SPrvDataPrivateWorldEscalextric *dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(dataPrivate->cars);
    assert_no_null(dataPrivate->dataCircuit);
}

//-----------------------------------------------------------------------

static struct SPrvDataPrivateWorldEscalextric *prv_createDataPrivateWorldEscalextric(enum EPrvDimensionWorld dimensionWorld, const class CDataCircuit *dataCircuit, class CArray<SPrvCar> **cars)
{
    struct SPrvDataPrivateWorldEscalextric *dataPrivate;

    dataPrivate = MALLOC(struct SPrvDataPrivateWorldEscalextric);

    dataPrivate->dimensionWorld = dimensionWorld;
    dataPrivate->dataCircuit = dataCircuit;
    dataPrivate->cars = ASSIGN_PP_NO_NULL(cars, class CArray<SPrvCar>);

    return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroyDataPrivateWorldEscalextric(struct SPrvDataPrivateWorldEscalextric **dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(*dataPrivate);

    delete (*dataPrivate)->cars;

    FREE_T(dataPrivate, struct SPrvDataPrivateWorldEscalextric);
}

//-----------------------------------------------------------------------

CWorldEscalextric::CWorldEscalextric(const class CDataCircuit *dataCircuit)
{
    class CArray<SPrvCar> *cars;

    cars = new CArray<SPrvCar>;
    m_dataPrivate = prv_createDataPrivateWorldEscalextric(PRV_NO_DIMENSION, dataCircuit, &cars);
}

//-----------------------------------------------------------------------

CWorldEscalextric::~CWorldEscalextric()
{
    prv_destroyDataPrivateWorldEscalextric(&m_dataPrivate);
}

//-----------------------------------------------------------------------

void CWorldEscalextric::setWorld2d(void)
{
    prv_integrity(m_dataPrivate);
    assert(m_dataPrivate->dimensionWorld == PRV_NO_DIMENSION);
    m_dataPrivate->dimensionWorld = PRV_DIMENSION_2D;
}

//-----------------------------------------------------------------------

void CWorldEscalextric::setWorld3d(void)
{
    prv_integrity(m_dataPrivate);
    assert(m_dataPrivate->dimensionWorld == PRV_NO_DIMENSION);
    m_dataPrivate->dimensionWorld = PRV_DIMENSION_3D;
}

//-----------------------------------------------------------------------

static void prv_appendCar(const char *idCar, enum EPrvLane lane, class CArray<SPrvCar> *cars)
{
    struct SPrvCar *car;
    unsigned long indTrack, velocity;
    double stepInTrack;

    indTrack = 0;
    stepInTrack = 0.;
    velocity = 0;

    car = new SPrvCar(idCar, lane, indTrack, stepInTrack, velocity);
    cars->add(car);
}

//-----------------------------------------------------------------------

void CWorldEscalextric::appendCarLaneRight(const char *idCar)
{
    prv_integrity(m_dataPrivate);
    prv_appendCar(idCar, PRV_LANE_RIGHT, m_dataPrivate->cars);
}

//-----------------------------------------------------------------------

void CWorldEscalextric::appendCarLaneLeft(const char *idCar)
{
    prv_integrity(m_dataPrivate);
    prv_appendCar(idCar, PRV_LANE_LEFT, m_dataPrivate->cars);
}

//-----------------------------------------------------------------------

static double prv_radiusCurveByLane(enum EPrvLane lane)
{
    double radius;

    switch(lane)
    {
        case PRV_LANE_LEFT:

            radius = .75;
            break;

        case PRV_LANE_RIGHT:

            radius = .25;
            break;

        default_error();
    }

    return radius;
}

//-----------------------------------------------------------------------

void CWorldEscalextric::applyVelocity(const char *idCar, double acceleration)
{
    struct SPrvCar *car;
    double velocity, radius;

    prv_integrity(m_dataPrivate);

    car = m_dataPrivate->cars->getOnlyOneElement<char>(idCar, prv_isCar);
    assert_no_null(car);

    radius = prv_radiusCurveByLane(car->m_lane);

    velocity = car->m_velocity + acceleration;
    velocity = MAX(0., velocity);

    car->m_velocity = CMath::roundu(velocity);

    m_dataPrivate->dataCircuit->applyVelocity(car->m_velocity, radius, &car->m_indTrack, &car->m_stepInTrack);
}

//-----------------------------------------------------------------------

class CAgent *CWorldEscalextric::move(const char *idCar, class CAgent **figure) const
{
    class CAgent *carMoved;
    class SPrvCar *car;
    double xPos, yPos, angleRotation, radius;

    car = m_dataPrivate->cars->getOnlyOneElement<char>(idCar, prv_isCar);

    radius = prv_radiusCurveByLane(car->m_lane);
    m_dataPrivate->dataCircuit->getPositionLane(car->m_indTrack, car->m_stepInTrack, radius, &xPos, &yPos, &angleRotation);

    switch(m_dataPrivate->dimensionWorld)
    {
        case PRV_DIMENSION_2D:
        {
            class CAgent *agentRotated;

            agentRotated = CTransform2D::createRotate2D(figure, angleRotation);
            carMoved = CTransform2D::createTraslate2D(&agentRotated, xPos, yPos);
            break;
        }
        case PRV_DIMENSION_3D:
        {
            class CAgent *agentRotated;
            double angleRotationDegree;

            angleRotationDegree = CMath::radianToDegree(angleRotation);

            agentRotated = CTransform3D::createRotate3D(figure, angleRotationDegree, 0., 0., 1.);
            carMoved = CTransform3D::createTraslate3D(&agentRotated, xPos, yPos, 0.);
            break;
        }
        case PRV_NO_DIMENSION:
        default_error();
    };

    return carMoved;
}
