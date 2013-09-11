// Procesa el evento del sistema si ha sucedido.

#ifndef _CPROCESSEVENTSYSTEM_HPP
#define _CPROCESSEVENTSYSTEM_HPP

class CProcessEventSystem
{
    public:
    
    	CProcessEventSystem(const char *nameTypeEvent);
        
    	virtual ~CProcessEventSystem();
        
        virtual class CAgent *nextGeneration(class CActor *actor, class CEventSystem *eventSystem, class CDataActor *dataActor) = 0;
        
        bool canProcessEvent(const class CEventSystem *evt) const;

    private:
    
    	struct prv_dataPrivateProcessEventSystem_t *m_dataPrivate;
};

#endif //_CPROCESSEVENTSYSTEM_HPP
