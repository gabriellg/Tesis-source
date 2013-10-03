//CTypeDescLimitsSprite: Type description it is calculated sprites's size to look for collision.

#include "CTypeDescrPhysics.hpp"

class CTypeDescLimitsSprite : public CTypeDescrPhysics
{
	public:

		static const char *ID_EVENT;

		CTypeDescLimitsSprite(class CWorldGeometry2DPixels *worldGeometry2D, const class CDictionaryDescription *dictionaryDraw);
		virtual ~CTypeDescLimitsSprite();

		virtual class IObjectDraw *move(const char *idElement, class IObjectDraw **actorToTransform);

        virtual void beginSymbol(const char *symbol, const class IDataSymbol *dataFigure);
        virtual void drawSymbol(const char *symbol, const class IDataSymbol *dataFigure);
        virtual void endSymbol(const char *symbol, const class IDataSymbol *dataFigure);

	private:

		struct SPrvDataPrivateTypeDescLimits *m_dataPrivate;
};
