/* -----------------------------------------------------------------------------
 *  CTransformRobot2D.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CTransformRobot2D.inl"

#include "CWorldSimulationRobot.inl"

#include "asrtbas.h"
#include "IDataSymbol.hpp"
#include "CTransform.hpp"

const char *CTransformRobot::ID_ROTATE_ROBOT = "ROTATE_ROBOT";
const char *CTransformRobot::ID_TRASLATE_ROBOT = "TRASLATE_ROBOT";

class IDataTransformRobot : public IDataSymbol
{
    public:

        virtual void applyTransformacion(class CWorldSimulationRobot *worldSimulation) const = 0;
};

class CRotateRobot : public IDataTransformRobot
{
    public:

        CRotateRobot(double angle);

        virtual void applyTransformacion(class CWorldSimulationRobot *worldSimulation) const;

    private:

        double m_angle;

};

class CTraslateRobot : public IDataTransformRobot
{
    public:

        CTraslateRobot(double dx, double dy);

        virtual void applyTransformacion(class CWorldSimulationRobot *worldSimulation) const;

    private:

        double m_dx, m_dy;

};

//-----------------------------------------------------------------------

CRotateRobot::CRotateRobot(double angle)
{
    m_angle = angle;
}

//-----------------------------------------------------------------------

void CRotateRobot::applyTransformacion(class CWorldSimulationRobot *worldSimulation) const
{
    assert_no_null(worldSimulation);
    worldSimulation->rotate(m_angle);
}

//-----------------------------------------------------------------------

CTraslateRobot::CTraslateRobot(double dx, double dy)
{
    m_dx = dx;
    m_dy = dy;
}

//-----------------------------------------------------------------------

void CTraslateRobot::applyTransformacion(class CWorldSimulationRobot *worldSimulation) const
{
    assert_no_null(worldSimulation);
    worldSimulation->traslate(m_dx, m_dy);
}

//-----------------------------------------------------------------------

void CDescriptionExecuteRobot::drawSymbol(
                        class CWorldSimulationRobot *worldSimulation,
                        const class IDataSymbol *dataFigure)
{
    assert_no_null(worldSimulation);
    assert(dataFigure == NULL);
    worldSimulation->applyTraslate();
}

//-----------------------------------------------------------------------

void CDescriptionExecuteTransformation::beginSymbol(
                    class CWorldSimulationRobot *worldRobot,
                    const class IDataSymbol *dataFigure)
{
    assert_no_null(worldRobot);
    assert_no_null(dataFigure);
    worldRobot->pushTransformation();
}

//-----------------------------------------------------------------------

void CDescriptionExecuteTransformation::drawSymbol(
                    class CWorldSimulationRobot *worldRobot,
                    const class IDataSymbol *dataFigure)
{
    const class IDataTransformRobot *dataTransform;

    assert_no_null(dataFigure);

    dataTransform = dynamic_cast<const class IDataTransformRobot *>(dataFigure);
    assert_no_null(dataTransform);
    dataTransform->applyTransformacion(worldRobot);
}
//-----------------------------------------------------------------------

void CDescriptionExecuteTransformation::endSymbol(class CWorldSimulationRobot *worldRobot, const class IDataSymbol *dataFigure)
{
    assert_no_null(worldRobot);
    assert_no_null(dataFigure);
    worldRobot->popTransformation();
}

//-----------------------------------------------------------------------

class CTransform *CTransformRobot::createRotateRobot(class CAgent **actorToTransform, double angleRotation)
{
    class IDataSymbol *dataRotate;

    dataRotate = new CRotateRobot(angleRotation);
    return new CTransform(ID_ROTATE_ROBOT, &dataRotate, actorToTransform);
}

//-----------------------------------------------------------------------

class CTransform *CTransformRobot::createTraslateRobot(class CAgent **actorToTransform, double dx, double dy)
{
    class IDataSymbol *dataTraslate;

    dataTraslate = new CTraslateRobot(dx, dy);
    return new CTransform(ID_TRASLATE_ROBOT, &dataTraslate, actorToTransform);
}
