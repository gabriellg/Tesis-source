//CTypeDescDrawSprite: Type description it is drawed on a bitmap.

#include "CTypeDescrPhysics.hpp"

class CTypeDescDrawSprite: public CTypeDescrPhysics
{
	public:

		static const char *ID_EVENT;

		CTypeDescDrawSprite(
                    class CGraphicsSprite *graphicsSprite,
                    class IWorld *world,
                    const class CDictionaryDescription *dictionaryDraw);

		virtual ~CTypeDescDrawSprite();

        virtual void beginSymbol(const char *symbol, const class IDataSymbol *dataFigure);
        virtual void drawSymbol(const char *symbol, const class IDataSymbol *dataFigure);
        virtual void endSymbol(const char *symbol, const class IDataSymbol *dataFigure);

		class IObjectDraw *move(const char *idElement, class IObjectDraw **objMove);
		
	private:
		
		struct SPrvDataPrivateTypeDescDrawSprite *m_dataPrivate;
};
