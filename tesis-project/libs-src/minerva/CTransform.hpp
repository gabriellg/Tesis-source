// Define objeto transformacion

#include "IObjectDraw.hpp"

class CTransform : public IObjectDraw
{
	public:
	
        CTransform(const char *idSymbol, class IDataSymbol **dataTransform, class CArray<IObjectDraw> **sons);
        CTransform(const char *idSymbol, class IDataSymbol **dataTransform, IObjectDraw **son);
		virtual ~CTransform();
		
    private:

        virtual void drawRepresentation(class CTypeDescription *evtDescription);


        struct SPrvDataPrivateTransform *m_dataPrivate;
};
