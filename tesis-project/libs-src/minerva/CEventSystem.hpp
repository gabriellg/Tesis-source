// Define evento de un sistema.

#ifndef CEVENTSYSTEM_HPP_
#define CEVENTSYSTEM_HPP_

class CEventSystem
{
    public:
    
    	CEventSystem(const char *nameEvent);
    	virtual ~CEventSystem();
        
        bool isTypeEvent(const char *nameTypeEvent) const;
        
    private:
    
        char *m_nameTypeEvent;
};

#endif /*CEVENTSYSTEM_HPP_*/
