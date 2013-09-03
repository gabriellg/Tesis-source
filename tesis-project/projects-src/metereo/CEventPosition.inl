// Define un evento con una posición 

#ifndef CEVENTPOSITION_HPP_
#define CEVENTPOSITION_HPP_

#include "CEventSystem.hpp"

class CEventPosition : public CEventSystem
{
    public:
    
        bool isSamePosition(double xPos, double yPos) const;

        void getPosition(double *xPos, double *yPos) const;
        
    protected:

        CEventPosition(double xPos, double yPos, const char *id_event);
        virtual ~CEventPosition();
        
    private:
    
        struct prv_dataPrivate_t *m_dataPrivate;
};

#endif /*CEVENTPOSITION_HPP_*/
