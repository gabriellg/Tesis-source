// Define el workspace para la aplicación de mundos virutales.

#include "CWorkspace.hpp"

class CWorkspaceWorldVirtual : public CWorkspace
{
	public:
	
		CWorkspaceWorldVirtual(class CDefineWorld **defineWorld);
		virtual ~CWorkspaceWorldVirtual();
		
		class CDataWorkspace *createDataWorkspace(void);
		class CView *createView(class CDataWorkspace *dataWorkspace);
		void defineMenu(void);
        
    private:
    
        class CDefineWorld *m_defineWorld;
};
