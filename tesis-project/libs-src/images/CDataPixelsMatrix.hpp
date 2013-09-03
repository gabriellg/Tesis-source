// Define matriz de pixels con informaci�n adicional.

class CDataPixelsMatrix
{
	public:
	
		CDataPixelsMatrix(unsigned long width, unsigned long heigth);
		virtual ~CDataPixelsMatrix();
		
		bool hasDataPixel(long xPixel, long yPixel) const;
		bool isPosition(long xPixel, long yPixel) const;
		class IDataPixel *getDataPixel(long xPixel, long yPixel) const;
		void setDataPixel(long xPixel, long yPixel, class IDataPixel *dataPixel);
		
		void getSize(unsigned long *width, unsigned long *heigth) const;
		
		void processDataPixels(class IProcessDataPixel *processDataPixel) const;

	private:
	
		struct i_DataPixelsMatrix_t *m_dataPixelMatrix;
};
