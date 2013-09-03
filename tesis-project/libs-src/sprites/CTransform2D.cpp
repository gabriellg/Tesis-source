//CTransform2D.cpp

#include "CTransform2D.hpp"

#include "asrtbas.h"
#include "IDataSymbol.hpp"
#include "CTransform.hpp"
#include "CGraphicsSprite.hpp"

const char *CTransform2D::ID_SYMBOL_ROTATE2D = "ROTATE2D";
const char *CTransform2D::ID_SYMBOL_TRASLATE2D = "TRASLATE2D";

class IDataTransformation2D : public IDataSymbol
{
    public:

        virtual void applyTransformacion(class CGraphicsSprite *graphicsSprite) const = 0;
};

class CDataTraslate2D : public IDataTransformation2D
{
    public:

        CDataTraslate2D(double dx, double dy);

        virtual void applyTransformacion(class CGraphicsSprite *graphicsSprite) const;

    private:

        double m_dx, m_dy;
};

class CDataRotate2D : public IDataTransformation2D
{
    public:

        CDataRotate2D(double angle);

        virtual void applyTransformacion(class CGraphicsSprite *graphicsSprite) const;

    private:

        double m_angle;
};

//-----------------------------------------------------------------------

CDataTraslate2D::CDataTraslate2D(double dx, double dy)
{
    m_dx = dx;
    m_dy = dy;
}

//-----------------------------------------------------------------------

void CDataTraslate2D::applyTransformacion(class CGraphicsSprite *graphicsSprite) const
{
    assert_no_null(graphicsSprite);
	graphicsSprite->translate(m_dx, m_dy);
}

//-----------------------------------------------------------------------

CDataRotate2D::CDataRotate2D(double angle)
{
    m_angle = angle;
}

//-----------------------------------------------------------------------

void CDataRotate2D::applyTransformacion(class CGraphicsSprite *graphicsSprite) const
{
    assert_no_null(graphicsSprite);
    graphicsSprite->rotate(m_angle);
}

//-----------------------------------------------------------------------

void CDescriptionTransformation::beginSymbol(
                        class CGraphicsSprite *graphicsSprite,
                        const class IDataSymbol *dataFigure)
{
    assert_no_null(graphicsSprite);
    assert_no_null(dataFigure);
    graphicsSprite->pushTransformation();
}

//-----------------------------------------------------------------------

void CDescriptionTransformation::drawSymbol(class CGraphicsSprite *graphicsSprite, const class IDataSymbol *dataFigure)
{
    const class IDataTransformation2D *dataTransformation2D;

    assert_no_null(graphicsSprite);
    assert_no_null(dataFigure);

    dataTransformation2D = dynamic_cast<const class IDataTransformation2D *>(dataFigure);
    assert_no_null(dataTransformation2D);

    dataTransformation2D->applyTransformacion(graphicsSprite);
}

//-----------------------------------------------------------------------

void CDescriptionTransformation::endSymbol(class CGraphicsSprite *graphicsSprite, const class IDataSymbol *dataFigure)
{
    assert_no_null(graphicsSprite);
    assert_no_null(dataFigure);

    graphicsSprite->popTransformation();
}

//-----------------------------------------------------------------------

class CTransform *CTransform2D::createRotate2D(class CAgent **actorToTransform, double angleRotation)
{
    class IDataSymbol *dataTransform;

    dataTransform = new CDataRotate2D(angleRotation);
    return new CTransform(ID_SYMBOL_ROTATE2D, &dataTransform, actorToTransform);
}

//-----------------------------------------------------------------------

class CTransform *CTransform2D::createTraslate2D(class CAgent **actorToTransform, double dx, double dy)
{
    class IDataSymbol *dataTransform;

    dataTransform = new CDataTraslate2D(dx, dy);
    return new CTransform(ID_SYMBOL_TRASLATE2D, &dataTransform, actorToTransform);
}
