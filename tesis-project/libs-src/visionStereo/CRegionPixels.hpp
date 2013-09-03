// Definici�n de una region de pixels.

class CRegionPixels
{
	public:
	
		CRegionPixels(unsigned long idRegion);
		virtual ~CRegionPixels();

		void appendPixel(
						long xPixel, long yPixel,
						class CDataPixelRegion *dataPixelRegion);
		
		bool isRegion(const class IDataPixel *dataPixel) const;
		
		bool searchRegionToImage(
						const class CImg *image,
						unsigned long numPixelsExploration,
						double tolerance,
						unsigned long *numPixelsDisparity_opt) const;

		class CArrPixel *pixels() const;

		void calculateRegionColor();
				
		void colorRegion(double *fred, double *fgreen, double *fblue) const;
		
	private:
	
		struct i_region_t *m_regionData;
};
