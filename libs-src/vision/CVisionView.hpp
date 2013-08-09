// Vista para vision artificial.

#include "CView.hpp"

class CVisionView : public CView
{
	public:
		CVisionView() {;}
		virtual ~CVisionView() {;}
		
		struct panel_t *panel(class CDataWorkspace *dataWorkspace);
};
