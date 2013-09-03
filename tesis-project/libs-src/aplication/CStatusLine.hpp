//CStatusLine.hpp

class CStatusLine
{
	public:
		
		CStatusLine();
		virtual ~CStatusLine();
		
		void setIcon(const struct Image_t *image);
		void setTextLeft(const char *text);
		void setTextRigth(const char *text);
		
		struct panel_t *panel(void);
		
	private:
		
		struct prv_privateDataStatusLine_t *m_dataPrivate;
};
