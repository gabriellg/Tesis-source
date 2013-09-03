//CEventDesired.hpp: Event defines desire

#include "CEventSystem.hpp"

class CEventDesire: public CEventSystem
{
	public:

		static const char *ID_EVENT;

		static const char *DESIRE_GO_LEFT;
		static const char *DESIRE_GO_RIGHT;
		static const char *DESIRE_GO_FORWARD;
		static const char *DESIRE_GO_BACK;

		CEventDesire(class CWorldGeometry2DPixels *worldGeometry2D);
		virtual ~CEventDesire();

		void setDesire(const char *idBlock, const char *idDesire);

		static bool isDesire(const char *idDesire);

	private:

		struct prv_dataPrivateEventDesire_t *m_dataPrivate;
};
