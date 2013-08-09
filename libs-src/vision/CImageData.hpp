#include "CResultData.hpp"

class CImageData : public CResultData
{
	public:
	
		CImageData(class CImg **image);
		CImageData(const char *filenameImage);
		
		CRESULTDATA_DECLARE_TYPE(CImageData);

		virtual ~CImageData();
		
		void draw(class IGraphics *graphics) const;
		
		const class CImg *getImage() const { return m_image; }
		
	private:
	
		class CImg *m_image;
};
