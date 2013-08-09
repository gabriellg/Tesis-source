// Definimos un evento para cuando se ejecute un relámpago.

#include "CEventPosition.inl"

class CEventStorm : public CEventPosition
{
    public:
    
        static const char *ID_EVENT;
    
    	CEventStorm(double xPos, double yPos);
    	virtual ~CEventStorm() {;}
};
