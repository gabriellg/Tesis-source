//CEscalextric.hpp

#include "CDefineWorld.hpp"

class CEscalextric: public CDefineWorld
{
	public:
		
		CEscalextric(bool is3d, const char *fileNameXML);
		virtual ~CEscalextric();
		
		virtual void defineLayers(class IGraphics *graphics);
		virtual void appendElementsToScene(class CScene *scene);
		virtual void appendDisplays(class CGestorDisplays *displays);

	private:

		struct SPrvDataPrivateEscalextric *m_dataPrivate;
};
