// Clase C++ que define una escena.

class CScene
{
	public:
	
        CScene();
		virtual ~CScene();
		
		void appendAgent(class CAgent **agent);
        
        void appendEvent(class CEventSystem **evtSystem);
        void nextFrame();
        void processDraw(class CTypeDescription *evtDraw) const;
		
	private:
    
        struct SPrvDataPrivateScene *m_dataPrivate;
};
