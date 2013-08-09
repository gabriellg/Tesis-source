/* -----------------------------------------------------------------------------
 *  CDescriptionSprite.hpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "IDescriptionSprite.hpp"

class CDescriptionSpriteAnimated : public IDescriptionSprite
{
    public:

        CDescriptionSpriteAnimated(class CArray<class CImg> **images);
        virtual ~CDescriptionSpriteAnimated();

        virtual void beginSymbol(class CGraphicsSprite *graphicsSprite, const class IDataSymbol *dataFigure) {;}
        virtual void drawSymbol(class CGraphicsSprite *graphicsSprite, const class IDataSymbol *dataFigure);
        virtual void endSymbol(class CGraphicsSprite *graphicsSprite, const class IDataSymbol *dataFigure) {;}

        virtual void size(const class IDataSymbol *dataFigure, unsigned long *sizeX, unsigned long *sizeY) const;

    private:

        struct prv_dataPrivateDescriptionSpriteAnimated_t *m_dataPrivate;
};
