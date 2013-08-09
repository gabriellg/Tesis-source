// Define los datos del mundo virtual.

#include "CDataWorkspace.hpp"

class CDataWorkspaceWorldVirtual : public CDataWorkspace
{
	public:
	
		CDataWorkspaceWorldVirtual(class CDefineWorld **defineWorld);
		virtual ~CDataWorkspaceWorldVirtual();

        void defineLayers(class IGraphics *graphics) const;
        
        void appendKeyToScene(const struct EvtKey_t *evtKey);
        void nextFrame(void);
        
        void stop(void);
        
		void setInitialPositionCamera(void);
        void positionCamera(class IGraphics *graphics) const;
        void makeRotationCamera(class IGraphics *graphics) const;
        
        void setRotationCamera(struct areaDibujo_t *areaDibujo, double rotXDegrees, double rotYDegrees, double rotZDegrees);
        void incrRotateCamera(struct areaDibujo_t *areaDibujo, double incrRotateX, double incrRotateY, double incrRotateZ);
        void frontCamera(struct areaDibujo_t *areaDibujo, double step);
        void backCamera(struct areaDibujo_t *areaDibujo, double step);
        bool isArea3D(void) const;
        
        void draw(class IGraphics *graphics) const;
        
        
    private:
    
        struct prv_dataPrivate_t *m_dataPrivate;
};
