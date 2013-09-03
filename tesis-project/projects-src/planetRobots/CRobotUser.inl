//CRobotUser.hpp: Robot is moved by user using keyboard. It is defined as a sprite.

#include "CAgentWithRepresentation.hpp"

class CRobotUser: public CAgentWithRepresentation
{
	public:

		CRobotUser();
		virtual ~CRobotUser();

        const char *getIdRobot(void);

        static void appendRobotToCollision(class CGeneratorCollisionSprite *generatorCollision);
        static void appendSymbolRobot(class IDisplay *display);

	private:

        virtual class CAgent *evolution(class CCollectionEventsSystem *allEvents);
        virtual class CAgent *representation(class CTypeDescription *evtDescription);

		struct SPrvDataPrivateRobotUser *m_dataPrivate;
};
