/* -----------------------------------------------------------------------------
 *  CTransformRobot2D.hpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "IDescriptionCommandRobot.inl"

class CDescriptionExecuteTransformation : public IDescriptionCommandRobot
{
    public:

        virtual void beginSymbol(class CWorldSimulationRobot *graphicsSprite, const class IDataSymbol *dataFigure);
        virtual void drawSymbol(class CWorldSimulationRobot *graphicsSprite, const class IDataSymbol *dataFigure);
        virtual void endSymbol(class CWorldSimulationRobot *graphicsSprite, const class IDataSymbol *dataFigure);
};

class CDescriptionExecuteRobot : public IDescriptionCommandRobot
{
    public:

        virtual void beginSymbol(class CWorldSimulationRobot *graphicsSprite, const class IDataSymbol *dataFigure) {;}
        virtual void drawSymbol(class CWorldSimulationRobot *graphicsSprite, const class IDataSymbol *dataFigure);
        virtual void endSymbol(class CWorldSimulationRobot *graphicsSprite, const class IDataSymbol *dataFigure) {;}
};

class CTransformRobot
{
    public:

        static const char *ID_ROTATE_ROBOT;
        static const char *ID_TRASLATE_ROBOT;

        static class CTransform *createRotateRobot(class CAgent **actorToTransform, double angleRotation);
        static class CTransform *createTraslateRobot(class CAgent **actorToTransform, double dx, double dy);
};
