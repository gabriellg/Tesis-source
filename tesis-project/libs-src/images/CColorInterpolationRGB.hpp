// Genera colores interpolados entre un mínimo y un máximo.

class CColorInterpolationRGB
{
	public:
	
		CColorInterpolationRGB(double minimum, double maximum);
		virtual ~CColorInterpolationRGB() {;}
		
		void getColorValue(double value, double *fred, double *fgreen, double *fblue);
		
	private:
	
		double m_minimum, m_factor;
};
