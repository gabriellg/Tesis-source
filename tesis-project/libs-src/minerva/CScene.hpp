// Clase C++ que define una escena.

class CScene
{
	public:
	
        CScene();
		virtual ~CScene();
		
		void appendAgent(class CAgent **agent);
        
        void appendKey(const struct EvtKey_t *evtKey);
        void nextFrame();
        void processDraw(class CTypeDescription *evtDraw) const;
		
	private:
    
        struct SPrvDataPrivateScene *m_dataPrivate;
};
