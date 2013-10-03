// Definición abstracta de una figura.

#include "IObjectDraw.hpp"

class CFigure : public IObjectDraw
{
	public:
    
		CFigure(const char *symbol);
		CFigure(const char *symbol, class IDataSymbol **dataFigure);
		virtual ~CFigure();

    private:

        virtual void drawRepresentation(class CTypeDescription *evtDescription);
        
        struct SPrvDataPrivateFigure *m_dataPrivate;
};
