#include "CDataWorkspace.hpp"

class CVisionDataWorkspace : public CDataWorkspace
{
	public:
	
		CVisionDataWorkspace(class CProcessGraphImages **processGraphImage);
		virtual ~CVisionDataWorkspace();

		void runProcessImages();
		void runNextLevel();
		
		void defineLayers(class IGraphics *graphics) const;
		void draw(class IGraphics *graphics) const;
		
	private:
	
		class CProcessGraphImages *m_processGraphImage;
};
