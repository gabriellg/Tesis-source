/* -----------------------------------------------------------------------------
 *  CDescriptionSpriteRotated.hpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "IDescriptionSprite.hpp"

class CDescriptionSpriteRotated: public IDescriptionSprite
{
    public:

        CDescriptionSpriteRotated(class CArray<class CImg> **images);
        virtual ~CDescriptionSpriteRotated();

        virtual void beginSymbol(class CGraphicsSprite *graphicsSprite, const class IDataSymbol *dataFigure) {;}
        virtual void drawSymbol(class CGraphicsSprite *graphicsSprite, const class IDataSymbol *dataFigure);
        virtual void endSymbol(class CGraphicsSprite *graphicsSprite, const class IDataSymbol *dataFigure) {;}

        virtual void size(const class IDataSymbol *dataFigure, unsigned long *sizeX, unsigned long *sizeY) const;

    private:

        struct prv_dataPrivateDescriptionSpriteRotated_t *m_dataPrivate;

};
