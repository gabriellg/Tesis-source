//CDisplay3D.hpp

#include "ITraslatorDisplay.hpp"

class CDisplay3D: public ITraslatorDisplay
{
	public:
		
		CDisplay3D(class IWorld *world, class CLight **light);
		virtual ~CDisplay3D();
		
        virtual void appendDescription(const char *symbol, class IDescription **description);
        virtual void drawScene(class IGraphics *graphics, const class CScene *scene);

	private:
		
		struct SPrvDataPrivateDisplay3D *m_dataPrivate;
};
