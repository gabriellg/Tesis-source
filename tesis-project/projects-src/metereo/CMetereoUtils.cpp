/* -----------------------------------------------------------------------------
 *  CMetereoUtils.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CMetereoUtils.inl"

#include "CDisplay3DMetereo.inl"
#include "CDescriptionsGL.inl"

#include "asrtbas.h"

#include "CMath.hpp"
#include "CGraphicsRect.hpp"
#include "CFigure.hpp"
#include "CTransform.hpp"
#include "CTransform3D.hpp"

//---------------------------------------------------------------

void CMetereoUtils::sizeTree(double *sizeXTree, double *sizeYTree)
{
    class CDescriptionTreeGL *descriptionTree;
    class CGraphicsRect *graphicsRect;
    bool hasLimits;
    double xmin, ymin, zmin, xmax, ymax, zmax;

    assert_no_null(sizeXTree);
    assert_no_null(sizeYTree);

    graphicsRect = new CGraphicsRect();

    descriptionTree = new CDescriptionTreeGL();
    descriptionTree->drawSymbol(graphicsRect, NULL);
    delete descriptionTree;

    hasLimits = graphicsRect->getLimits(&xmin, &ymin, &zmin, &xmax, &ymax, &zmax);
    assert(hasLimits == true);

    delete graphicsRect;

    *sizeXTree = 2. * (xmax - xmin);
    *sizeYTree = 2. * (ymax - ymin);

    assert(*sizeXTree > 0.0001);
    assert(*sizeYTree > 0.0001);
}

//---------------------------------------------------------------

class CAgent *CMetereoUtils::descriptionTree(double xPos, double yPos, double scale)
{
    class CAgent *scaleFrame, *figureTree;

    figureTree = new CFigure(CDisplay3DMetereo::SYMBOL_TREE);

    if (CMath::isEqual(scale, 1., 0.00001) == false)
        scaleFrame = CTransform3D::createScale3D(&figureTree, scale, scale, scale);
    else
        scaleFrame = figureTree;

    return CTransform3D::createTraslate3D(&scaleFrame, xPos, yPos, 0.);
}
