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

		virtual void setInitialPositionCamera(void) {;}
		virtual void positionCamera(class IGraphics *graphics) const {;}
		virtual void makeRotationCamera(class IGraphics *graphics) const {;}
		virtual void setRotationCamera(struct areaDibujo_t *areaDibujo, double rotXDegrees, double rotYDegrees, double rotZDegrees) {;}
		virtual void incrRotateCamera(struct areaDibujo_t *areaDibujo, double incrRotateX, double incrRotateY, double incrRotateZ) {;}
		virtual void frontCamera(struct areaDibujo_t *areaDibujo, double step) {;}
		virtual void backCamera(struct areaDibujo_t *areaDibujo, double step) {;}
		
		bool isArea3D(void) const {return false;};
        bool withSize(unsigned long *width, unsigned long *height) const;

		
	private:
		
		struct prv_dataPrivateDisplaySprite_t *m_dataPrivate;
};
