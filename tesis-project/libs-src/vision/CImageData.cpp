#include "CImageData.hpp"

#include "asrtbas.h"
#include "memory.h"
#include "CImg.hpp"

//---------------------------------------------------------
	
CImageData::CImageData(class CImg **image)
{
    m_image = ASSIGN_PP_NO_NULL(image, class CImg);
}

//---------------------------------------------------------
	
CImageData::CImageData(const char *filenameImage)
{
    m_image = new CImg(filenameImage);
}	

//---------------------------------------------------------
	
CImageData::~CImageData()
{
	delete m_image;
}

//---------------------------------------------------------
	
void CImageData::draw(class IGraphics *graphics) const
{
    ;
}
