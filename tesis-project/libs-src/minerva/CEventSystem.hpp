// Define evento de un sistema.

#pragma once

class CEventSystem
{
    public:
    
    	CEventSystem(const char *nameEvent);
    	virtual ~CEventSystem();
        
        bool isTypeEvent(const char *nameTypeEvent) const;
        
    private:
    
        char *m_nameTypeEvent;
};
