// Define el mundo para las tormentas.

#include "CDefineWorld.hpp"

class CWorldStorm : public CDefineWorld
{
    public:
    
    	CWorldStorm(void);
    	virtual ~CWorldStorm(void);
        
        virtual void defineLayers(class IGraphics *graphics);
        virtual void appendElementsToScene(class CScene *scene); 
        virtual void appendDisplays(class CGestorDisplays *displays);
};
