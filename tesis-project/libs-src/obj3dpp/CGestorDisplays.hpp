//CGestorDisplay.hpp

class CGestorDisplays
{
	public:
		
		CGestorDisplays();
		virtual ~CGestorDisplays();
		
		void appendDisplay(class IDisplay **display);
		
		void setInitialPositionCamera(void);
		
	    void makeRotationCurrentDisplay(class IGraphics *graphics);
	    void setRotationCameraCurrentDisplay(struct areaDibujo_t *areaDibujo, double rotXDegrees, double rotYDegrees, double rotZDegrees);
	    void incrRotateCameraCurrentDisplay(struct areaDibujo_t *areaDibujo, double incrRotateX, double incrRotateY, double incrRotateZ);
	    void frontCameraCurrentDisplay(struct areaDibujo_t *areaDibujo, double step);
	    void backCameraCurrentDisplay(struct areaDibujo_t *areaDibujo, double step);
		
		void positionCameraCurrentDisplay(class IGraphics *graphics) const;
		void drawScene(class IGraphics *graphics, const class CScene *scene);
		
		bool isArea3DCurrentDisplay(void) const;
		
	private:
		
		struct prv_dataPrivateGestorDisplays_t *m_dataPrivate;
};
