//IDisplay.hpp

#pragma once

class ITraslatorDisplay
{
	public:
		
		virtual ~ITraslatorDisplay() {;}
		
        virtual void appendDescription(const char *symbol, class IDescription **description) = 0;
        virtual void drawScene(class IGraphics *graphics, const class CScene *scene) = 0;

		virtual void setInitialPositionCamera(void) = 0;
		virtual void positionCamera(class IGraphics *graphics) const  = 0 ;
		virtual void makeRotationCamera(class IGraphics *graphics) const  = 0 ;
		virtual void setRotationCamera(struct areaDibujo_t *areaDibujo, double rotXDegrees, double rotYDegrees, double rotZDegrees) = 0;
		virtual void incrRotateCamera(struct areaDibujo_t *areaDibujo, double incrRotateX, double incrRotateY, double incrRotateZ) = 0;
		virtual void frontCamera(struct areaDibujo_t *areaDibujo, double step) = 0;
		virtual void backCamera(struct areaDibujo_t *areaDibujo, double step) = 0;

		virtual bool isArea3D(void) const = 0;
        virtual bool withSize(unsigned long *width, unsigned long *height) const = 0;
};
