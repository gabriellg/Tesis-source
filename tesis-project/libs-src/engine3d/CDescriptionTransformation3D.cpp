/* -----------------------------------------------------------------------------
 *  CDescriptionTransformation3d.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CDescriptionTransformation3D.hpp"

#include "CTransform3D.hpp"

#include "IGraphics.hpp"
#include "asrtbas.h"

//-----------------------------------------------------------------------

void CDescriptionTransformation3D::beginSymbol(class IGraphics *graphics, const class IDataSymbol *dataFigure)
{
    assert_no_null(dataFigure);
    graphics->pushTransformation();
}

//-----------------------------------------------------------------------

void CDescriptionTransformation3D::drawSymbol(class IGraphics *graphics, const class IDataSymbol *dataFigure)
{
    const class IDataTransformation3D *dataTransformation;

    dataTransformation = dynamic_cast<const class IDataTransformation3D *>(dataFigure);
    assert_no_null(dataTransformation);

    dataTransformation->applyTransformacion(graphics);
}

//-----------------------------------------------------------------------

void CDescriptionTransformation3D::endSymbol(class IGraphics *graphics, const class IDataSymbol *dataFigure)
{
    assert_no_null(dataFigure);
    graphics->popTransformation();
}
