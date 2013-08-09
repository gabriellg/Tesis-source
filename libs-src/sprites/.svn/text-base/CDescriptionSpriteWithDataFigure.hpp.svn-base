/* -----------------------------------------------------------------------------
 *  CDescriptionSpriteWithDataFigure.hpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "IDescriptionSprite.hpp"
#include "IDataSymbol.hpp"

class CDataImageFigure : public IDataSymbol
{
    public:

        CDataImageFigure(class CImg **image);
        CDataImageFigure(const class CImg *image);
        virtual ~CDataImageFigure(void);

        void drawImage(class CGraphicsSprite *graphicsSprite) const;
        void size(unsigned long *sizeX, unsigned long *sizeY) const;

    private:

        struct prv_dataPrivateDataImageFigure_t *m_dataPrivate;
};

class CDescriptionSpriteWithDataFigure: public IDescriptionSprite
{
    public:

        CDescriptionSpriteWithDataFigure(long dx, long dy);
        virtual ~CDescriptionSpriteWithDataFigure();

        virtual void beginSymbol(class CGraphicsSprite *graphicsSprite, const class IDataSymbol *dataFigure) {;}
        virtual void drawSymbol(class CGraphicsSprite *graphicsSprite, const class IDataSymbol *dataFigure);
        virtual void endSymbol(class CGraphicsSprite *graphicsSprite, const class IDataSymbol *dataFigure) {;}

        virtual void size(const class IDataSymbol *dataFigure, unsigned long *sizeX, unsigned long *sizeY) const;

    private:

        struct prv_dataPrivateDescriptionSpriteWithDataFigure_t *m_dataPrivate;
};
