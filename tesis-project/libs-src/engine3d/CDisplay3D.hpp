//CDisplay3D.hpp

#include "ITraslatorDisplay.hpp"

class CDisplay3D: public ITraslatorDisplay
{
	public:
		
		CDisplay3D(class IWorld *world, class CLight **light, class CPositionCamera **positionCameraInitial);
		virtual ~CDisplay3D();
		
        virtual void appendDescription(const char *symbol, class IDescription **description);
        virtual void drawScene(class IGraphics *graphics, const class CScene *scene);

		virtual void setInitialPositionCamera(void);
		virtual void positionCamera(class IGraphics *graphics) const;
		virtual void makeRotationCamera(class IGraphics *graphics) const;
		virtual void setRotationCamera(struct areaDibujo_t *areaDibujo, double rotXDegrees, double rotYDegrees, double rotZDegrees);
		virtual void incrRotateCamera(struct areaDibujo_t *areaDibujo, double incrRotateX, double incrRotateY, double incrRotateZ);
		virtual void frontCamera(struct areaDibujo_t *areaDibujo, double step);
		virtual void backCamera(struct areaDibujo_t *areaDibujo, double step);
		
		virtual bool isArea3D(void) const { return true; };
		virtual bool withSize(unsigned long *width, unsigned long *heigth) const { return false;}

	private:
		
		struct SPrvDataPrivateDisplay3D *m_dataPrivate;
};
