//CEventCreateActor: Event to create actors.

#include "CEventSystem.hpp"

class CEventCreateActor: public CEventSystem
{
	public:

		static const char *ID_EVENT;

		CEventCreateActor(
						const char *typeActor,
						long x, long y,
						class ICreatorActors *createActors,
						class CWorldGeometry2DPixels *worldGeometry2DPixels);
		virtual ~CEventCreateActor();

		bool createActor(class CAgent **actorCreate);

	private:

		struct SPrvDataPrivateEventCreateActor *m_dataPrivate;
};
