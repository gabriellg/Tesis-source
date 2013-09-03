//CEventCollision: Event to check if there is a collision with background or other element.

#include "CEventSystem.hpp"

class CEventCollision: public CEventSystem
{
	public:

		static const char *ID_EVENT;

		CEventCollision(class CWorldGeometry2DPixels *worldGeometry2D);

		virtual ~CEventCollision();

		bool hasCollisionBackground(const char *id);

		bool hasCollisionWithElement(const char *id, char **idBlockCollisioned_opt);

	private:

		struct prv_dataPrivateEventCollision_t *m_dataPrivate;
};
