// Definición abstracta de una figura.

#include "CAgent.hpp"

class CFigure : public CAgent
{
	public:
    
		CFigure(const char *symbol);
		CFigure(const char *symbol, class IDataSymbol **dataFigure);
		virtual ~CFigure();

    private:

        virtual void beginEvolution(class CCollectionEventsSystem *allEvents) {;}
        virtual void endEvolution(class CCollectionEventsSystem *allEvents) {;}

        virtual class CAgent *evolution(class CCollectionEventsSystem *allEvents);
        virtual class CAgent *representation(class CTypeDescription *evtDescription);

        virtual void beginRepresentation(class CTypeDescription *evtDescription);
        virtual void drawRepresentation(class CTypeDescription *evtDescription);
        virtual void endRepresentation(class CTypeDescription *evtDescription);
        
        struct SPrvDataPrivateFigure *m_dataPrivate;
};
