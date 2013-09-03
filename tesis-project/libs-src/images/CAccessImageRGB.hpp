//CAccessImageRGB.hpp:

class CAccessImageRGB
{
	public:

		CAccessImageRGB(unsigned char *imageData, unsigned long widthStep, unsigned long width, unsigned long height);
		virtual ~CAccessImageRGB();

		unsigned long getWidth(void) const;
		unsigned long getHeight(void) const;

		void getRGB(unsigned long f, unsigned long c, unsigned char *r, unsigned char *g, unsigned char *b) const;
		void setRGB(unsigned long f, unsigned long c, unsigned char r, unsigned char g, unsigned char b);

	private:

		struct prv_dataPrivateAccessImageRGB_t *m_dataPrivate;
};
