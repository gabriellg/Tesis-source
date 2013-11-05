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
        
        void setRotationCamera(double rotXDegrees, double rotYDegrees, double rotZDegrees);
        void incrRotateCamera(double incrRotateX, double incrRotateY, double incrRotateZ);
        void frontCamera(double step);
        void backCamera(double step);
        
        void draw(class IGraphics *graphics) const;
        
        
    private:
    
        struct SPrvDataWorkspaceWorldVirtual *m_dataPrivate;
};
