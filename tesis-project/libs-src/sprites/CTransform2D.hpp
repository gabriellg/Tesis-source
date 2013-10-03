//CTransform2D.hpp

#include "IDescriptionSprite.hpp"

class CDescriptionTransformation : public IDescriptionSprite
{
    public:
        virtual void beginSymbol(class CGraphicsSprite *graphicsSprite, const class IDataSymbol *dataFigure);
        virtual void drawSymbol(class CGraphicsSprite *graphicsSprite, const class IDataSymbol *dataFigure);
        virtual void endSymbol(class CGraphicsSprite *graphicsSprite, const class IDataSymbol *dataFigure);
        virtual void size(const class IDataSymbol *dataFigure, unsigned long *sizeX, unsigned long *sizeY) const {;}
};

class CTransform2D
{
	public:
		
        static const char *ID_SYMBOL_ROTATE2D;
        static const char *ID_SYMBOL_TRASLATE2D;

        static class CTransform *createRotate2D(class IObjectDraw **figure, double angleRotation);
		static class CTransform *createTraslate2D(class IObjectDraw **figure, double dx, double dy);
};
