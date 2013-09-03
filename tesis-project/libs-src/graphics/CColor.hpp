//CColor.hpp

class CColor
{
	public:

		enum Color_t
		{
			Black, White
		};
		
		CColor(enum Color_t color);
		CColor(double r, double g, double b);
		CColor(unsigned char r, unsigned char g, unsigned char b);
		virtual ~CColor();
		
		void getColorDouble(double *r, double *g, double *b) const;
		void getColor(unsigned char *r, unsigned char *g, unsigned char *b) const;
		
		bool isWhite(void) const;
		bool isBlack(void) const;

		static void transformColorRGB(
                        unsigned char r, unsigned char g, unsigned char b,
                        double *fr, double *fg, double *fb);

		static void transformToRGB(
                        double fred, double fgreen, double fblue,
		                unsigned char *red, unsigned char *green, unsigned char *blue);

		static bool isSameColorWithTolerance(
		                double fr1, double fg1, double fb1,
		                double fr2, double fg2, double fb2, double tolerance);

	private:
		
		struct prv_dataPrivateCColor_t *m_dataPrivate;
};
