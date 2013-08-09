/* -----------------------------------------------------------------------------
 *  IDescriptionSprite.inl
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#ifndef IDESCRIPTIONSPRITE_INL_
#define IDESCRIPTIONSPRITE_INL_

#include "IDescription.hpp"

class IDescriptionSprite : public IDescription
{
    public:

        virtual void beginSymbol(class CGraphicsSprite *graphicsSprite, const class IDataSymbol *dataFigure) = 0;
        virtual void drawSymbol(class CGraphicsSprite *graphicsSprite, const class IDataSymbol *dataFigure) = 0;
        virtual void endSymbol(class CGraphicsSprite *graphicsSprite, const class IDataSymbol *dataFigure) = 0;

        virtual void size(const class IDataSymbol *dataFigure, unsigned long *sizeX, unsigned long *sizeY) const = 0;
};

#endif /* IDESCRIPTIONSPRITE_INL_ */
