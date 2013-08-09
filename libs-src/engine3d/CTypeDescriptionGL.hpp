//CEventGL.hpp

#include "CTypeDescrPhysics.hpp"

class CTypeDescriptionGL: public CTypeDescrPhysics
{
	public:
		
        static const char *ID_EVENT;

		CTypeDescriptionGL(class IWorld *world, class IGraphics *graphics, const class CDictionaryDescription *dictionaryDraw);
		virtual ~CTypeDescriptionGL();
		
    protected:

        virtual void beginSymbol(const char *symbol, const class IDataSymbol *dataFigure);
        virtual void drawSymbol(const char *symbol, const class IDataSymbol *dataFigure);
        virtual void endSymbol(const char *symbol, const class IDataSymbol *dataFigure);

        virtual class CAgent *move(const char *idElement, class CAgent **figure);

	private:
		
		struct SPrvDataPrivateTypeDescriptionGL *m_dataPrivate;
};
