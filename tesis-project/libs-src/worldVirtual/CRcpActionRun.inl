//CRcpActionRun.hpp

#include "CRcpOpcAreaDib.hpp"

class CRcpActionRun : public CRcpOpcAreaDib
{
    public:

    	CRcpActionRun(void);
    	~CRcpActionRun();

    private:

    	enum CRcpOpcAreaDib::Result_t processEvtTime(class CEvtOpcAreaDib *evtAreaDib);
    	enum CRcpOpcAreaDib::Result_t processEvtKey(class CEvtOpcAreaDib *evtAreaDib);
    	enum CRcpOpcAreaDib::Result_t processStart(class CEvtOpcAreaDib *evtAreaDib);
    	enum CRcpOpcAreaDib::Result_t processEnd(class CEvtOpcAreaDib *evtAreaDib);

    	struct prv_dataPrivateCRcpActionRun_t *m_dataPrivate;
};
