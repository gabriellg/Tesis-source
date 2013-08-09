//CLight.hpp

#include "IGraphics.hpp"

class CLight
{
	public:

		CLight(class CColor **colorAmbient, class CColor **colorDiffuse);
	
		CLight(class CColor **colorAmbient, class CColor **colorDiffuse, class CColor **colorSpecular);
	
		CLight(class CColor **colorAmbient, class CColor **colorDiffuse, class CColor **colorSpecular, double positionX, double positionY, double positionZ);
		
		virtual ~CLight();
		
		void putLight(class IGraphics *graphics, enum IGraphics::ELight light) const;
		
	private:
	
		struct prv_dataPrivateCLight_t *m_dataPrivate;
};
