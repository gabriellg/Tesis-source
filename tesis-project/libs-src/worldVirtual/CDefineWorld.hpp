class CDefineWorld
{
    public:
    
    	CDefineWorld(void) {;}
    	virtual ~CDefineWorld(void) {;}
        
        virtual void appendElementsToScene(class CScene *scene) = 0;
        virtual void defineLayers(class IGraphics *graphics) = 0;
        virtual void appendDisplays(class CGestorDisplays *displays) = 0;
};
