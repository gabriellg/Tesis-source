// Definición de la vista.

#include "CView.hpp"

class CViewWorldVirtual : public CView
{
	public:
    
        CViewWorldVirtual();
                    
        virtual ~CViewWorldVirtual();
	
		struct panel_t *panel(class CDataWorkspace *dataWorkspace);
        
        void setRotationCamera(double rotXDegrees, double rotYDegrees, double rotZDegrees);
        void setInitialPositionCamera(class CDataWorkspaceWorldVirtual *dataWorkspaceWorldVirtual);

    private:
    
        struct SDataPrivateViewWorldVirtual *m_dataPrivate;
};
