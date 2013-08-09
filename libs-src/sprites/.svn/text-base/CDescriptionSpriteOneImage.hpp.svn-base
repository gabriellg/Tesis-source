/* -----------------------------------------------------------------------------
 *  CDescriptionSpriteOneImage.hpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "IDescriptionSprite.hpp"

class CDescriptionSpriteOneImage: public IDescriptionSprite
{
    public:

        CDescriptionSpriteOneImage(long dx, long dy, class CImg **image);
        CDescriptionSpriteOneImage(class CImg **image);
        static class CDescriptionSpriteOneImage *createSpriteOneImageInCenter(class CImg **image);

        virtual ~CDescriptionSpriteOneImage();

        virtual void beginSymbol(class CGraphicsSprite *graphicsSprite, const class IDataSymbol *dataFigure) {;}
        virtual void drawSymbol(class CGraphicsSprite *graphicsSprite, const class IDataSymbol *dataFigure);
        virtual void endSymbol(class CGraphicsSprite *graphicsSprite, const class IDataSymbol *dataFigure) {;}

        virtual void size(const class IDataSymbol *dataFigure, unsigned long *sizeX, unsigned long *sizeY) const;

    private:

        CDescriptionSpriteOneImage(struct prv_dataPrivateDescriptionSpriteOneImage_t **dataPrivate);

        struct prv_dataPrivateDescriptionSpriteOneImage_t *m_dataPrivate;
};
