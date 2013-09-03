// Hace segmentaci�n.

class CRegions
{
	public:
	
		CRegions(const class CImg *image);
		
		~CRegions();
		
		class CMatrixDisparity *calculateMatrixDisparity(const class CImg *theOtherImage) const;

		class CImg *imageAllRegionsCalculated(void) const;
	
	private:
	
		class CDataPixelsMatrix *m_pixelsMatrix;
		ArrClass(CRegionPixels) *m_regions;	
};
