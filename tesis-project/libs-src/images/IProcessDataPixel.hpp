class IProcessDataPixel
{
	public:
	
		IProcessDataPixel() {;}
		virtual ~IProcessDataPixel() {;}
		
		virtual void processDataPixel(unsigned long x, unsigned long y, IDataPixel *dataPixel) = 0;
};

