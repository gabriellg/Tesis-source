//CDisplaySprite.hpp

#include "ITraslatorDisplay.hpp"

class CDisplaySprite: public ITraslatorDisplay
{
	public:

		CDisplaySprite(unsigned long width, unsigned long height, class IWorld *worldGeometry2D);
		CDisplaySprite(class CImg **imageBackground, class IWorld *world);
	    CDisplaySprite(bool originCartesian, unsigned long scaleX, unsigned long scaleY, unsigned long width, unsigned long height, class IWorld *world);
		virtual ~CDisplaySprite();

        virtual void appendDescription(const char *symbol, class IDescription **description);
        virtual void drawScene(class IGraphics *graphics, const class CScene *scene);
		
	private:
		
		struct prv_dataPrivateDisplaySprite_t *m_dataPrivate;
};
