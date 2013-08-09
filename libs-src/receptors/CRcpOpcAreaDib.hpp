#ifndef CRCPOPCAREADIB_HPP_
#define CRCPOPCAREADIB_HPP_

class CRcpOpcAreaDib
{
	public:
	
		enum Result_t
		{
			END, CONTINUE 
		};
	
		CRcpOpcAreaDib(const char *nameOption);
		virtual ~CRcpOpcAreaDib();
		
		enum Result_t sendEvent(class CEvtOpcAreaDib *evtAreaDib);
		
		const char *getTextMenu(void);
		
	private:
		
		virtual enum Result_t processEvtTime(class CEvtOpcAreaDib *evtAreaDib);
		virtual enum Result_t processEvtKey(class CEvtOpcAreaDib *evtAreaDib);
		virtual enum Result_t processEvtMouse(class CEvtOpcAreaDib *evtAreaDib);
		virtual enum Result_t processStart(class CEvtOpcAreaDib *evtAreaDib);
		virtual enum Result_t processEnd(class CEvtOpcAreaDib *evtAreaDib);
		
		struct prv_dataPrivateOpcAreaDib_t *m_dataPrivate;
};

#endif /*CRCPOPCAREADIB_HPP_*/
