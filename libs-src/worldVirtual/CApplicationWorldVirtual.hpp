// Define la aplicación para mundos virtuales. 

#include "CAplicationGTK.hpp"

class CApplicationWorldVirtual : public CAplicationGTK
{
	public:
	
		CApplicationWorldVirtual(class CDefineWorld *defineWorld, const char *titleAplication);
        virtual ~CApplicationWorldVirtual();
		
	private:
    
        class CDefineWorld *m_defineWorld;
	
		class CWorkspace *createWorkspace();
		
};
