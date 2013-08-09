//CEventTraslate.hpp

#include "CEventSystem.hpp"

class CEventTraslate : public CEventSystem
{
	public:
		
		static const char *ID_EVENT;
		
		static class CEventTraslate *createLeft(void);
		static class CEventTraslate *createRight(void);
		static class CEventTraslate *createDown(void);
		static class CEventTraslate *createUp(void);
		
		virtual ~CEventTraslate();
		
		char *applyTraslate(
						const char *idDesireRight,
						const char *idDesireLeft,
						const char *idDesireUp,
						const char *idDesireDown) const;
		
	private:
		
		CEventTraslate(struct prv_dataPrivateTraslate_t **dataPrivate);
		
		struct prv_dataPrivateTraslate_t *m_dataPrivate; 
};
