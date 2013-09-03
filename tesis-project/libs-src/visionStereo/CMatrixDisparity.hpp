// Definimos matriz de disparidad.

class CMatrixDisparity
{
	public:
	
		CMatrixDisparity(
					const class CImg *image,
					const ArrClass(CRegionPixels) *regions,
					unsigned long numPixelsExploration,
					double toleranceDistanceRGB);
		
		~CMatrixDisparity() {;}
		
		class CImg *imageRepresentationDisparity();
		
	private:
		
		class CDataPixelsMatrix *m_matrixDisparity;
};
