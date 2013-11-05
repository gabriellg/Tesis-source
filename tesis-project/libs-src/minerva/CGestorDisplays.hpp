//CGestorDisplay.hpp

class CGestorDisplays
{
	public:
		
		CGestorDisplays();
		virtual ~CGestorDisplays();
		
		void appendDisplay(class ITraslatorDisplay **display);
		void drawScene(class IGraphics *graphics, const class CScene *scene);
		
	private:
		
		struct prv_dataPrivateGestorDisplays_t *m_dataPrivate;
};
