// Workspace para vision.

#include "CWorkspace.hpp"

class CVisionWorkspace : public CWorkspace
{
	public:
	
		CVisionWorkspace(const class CFactoryProcessGraph *factoryProcessGraph);
		virtual ~CVisionWorkspace();
		
	protected:
	
		class CDataWorkspace *createDataWorkspace(void);
		class CView *createView(class CDataWorkspace *dataWorkspace);
		void defineMenu(void);
		
	private:
	
		const class CFactoryProcessGraph *m_factoryProcessGraph;
};
