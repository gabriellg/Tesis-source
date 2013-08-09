#ifndef CEVTOPCAREADIB_HPP_
#define CEVTOPCAREADIB_HPP_

class CEvtOpcAreaDib
{
	public:

		enum TypeEvent_t
		{
			START, END, MOUSE, TIME, KEY
		};
		
		enum TypeRedraw_t
		{
			NO_REDRAW, REDRAW
		};
	
		CEvtOpcAreaDib(
				enum TypeEvent_t type,
				class CDataWorkspace *dataWorkspace,
				class CStatusLine *statusLine,
                class CView *viewCurrent,
				struct dialogo_t *dlgFather,
				const struct EvtMouse_t *evtMouse,
				const struct EvtKey_t *evtKey);

		virtual ~CEvtOpcAreaDib();
		
		enum TypeEvent_t typeEvent(void);
		
		class CDataWorkspace *getDataWorkspace(void);
        class CView *getViewCurrent(void);
        const struct EvtKey_t *getEvtKey(void);
		
		void setRedraw(void);
		bool redrawAllViews(void);
		bool isStart(void);
		bool isEnd(void);
		
		void setMessageLeft(const char *text);
		void setMessageRight(const char *text);
		
	private:

		struct prv_dataPrivateEvtOpcAreaDib_t *m_dataPrivate;
};

#endif /*CEVTOPCAREADIB_HPP_*/
