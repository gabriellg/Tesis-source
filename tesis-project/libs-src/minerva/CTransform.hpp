// Define objeto transformacion

#include "CAgent.hpp"

class CTransform : public CAgent
{
	public:
	
        CTransform(const char *idSymbol, class IDataSymbol **dataTransform, class CAgent **actorToTransform);
		virtual ~CTransform();
		
    private:

        virtual void beginEvolution(class CCollectionEventsSystem *allEvents) {;}
        virtual void endEvolution(class CCollectionEventsSystem *allEvents) {;}

        virtual class CAgent *evolution(class CCollectionEventsSystem *allEvents);
        virtual class CAgent *representation(class CTypeDescription *evtDescription);

        virtual void beginRepresentation(class CTypeDescription *evtDescription);
        virtual void drawRepresentation(class CTypeDescription *evtDescription);
        virtual void endRepresentation(class CTypeDescription *evtDescription);


        struct SPrvDataPrivateTransform *m_dataPrivate;
};
