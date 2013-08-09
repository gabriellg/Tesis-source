// Procesa una imagen.

class CProcessImage
{
	public:
	
		virtual ~CProcessImage() {;}
		
		virtual class CResultData *processImage(const ArrClass(CResultData) *imagesIn) = 0;
		virtual void draw(const class CResultData *, class IGraphics *graphics) const;
};
