//CMapSprite.hpp

#include "CAgentWithRepresentation.hpp"

class CMapSprite : public CAgentWithRepresentation
{
	public:

		CMapSprite(const char *idFigureScenario);
		virtual ~CMapSprite();
		
    protected:

        virtual void appendEvent(class CCollectionEventsSystem *allEvents) {;}

        virtual class CAgent *evolution(class CCollectionEventsSystem *allEvents);
        virtual class CAgent *representation(class CTypeDescription *evtDescription);

    private:

        struct prv_dataPrivateMapSprite_t *m_dataPrivate;
};
