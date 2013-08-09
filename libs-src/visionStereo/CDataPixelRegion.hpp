#include "IDataPixel.hpp"

class CDataPixelRegion : public IDataPixel
{
	public:
	
		CDataPixelRegion(
						unsigned long idRegion,
						unsigned long xPixel, unsigned long yPixel,
						double fred, double fgreen, double fblue);
						
		~CDataPixelRegion();				
						
		bool isPixelAtDisparityWithColorByDistance(
						const class CImg *image,
						unsigned long disparity,
						double tolerance) const;
						
		void appendPixel(class CArrPixel *pixels);
		
		void acumulateColor(double *fred, double *fgreen, double *fblue) const;

//TODO: Mal rollo set y get. Mirar.		
		void setColorRegion(double fred, double fgreen, double fblue);
		
		void getColorRegion(double *fred, double *fgreen, double *fblue) const;

		void selectBorder(const class CDataPixelRegion *dataPixel);

		class CRegionPixels *selectRegion(ArrClass(CRegionPixels) *regions) const;

	private:

		struct i_dataPixelRegion_t *m_dataPixelRegion;	
};
