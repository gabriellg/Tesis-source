//CPlanetRobots.hpp

#include "CDefineWorld.hpp"

class CSimulationRobot : public CDefineWorld
{
	public:
		
        CSimulationRobot(bool is3d);
		virtual ~CSimulationRobot(void);
		
		virtual void defineLayers(class IGraphics *graphics);
		virtual void appendElementsToScene(class CScene *scene);
		virtual void appendDisplays(class CGestorDisplays *displays);

	private:

		struct prv_dataPrivateSimulationRobot_t *m_dataPrivate;
};
