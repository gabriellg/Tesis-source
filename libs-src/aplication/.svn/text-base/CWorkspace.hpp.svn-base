// Define workspace de la aplicaci�n.

class CWorkspace
{
	public:
		
		CWorkspace();
		virtual ~CWorkspace();
		
		virtual class CDataWorkspace *createDataWorkspace(void) = 0;
		virtual class CView *createView(class CDataWorkspace *dataWorkspace) = 0;
		virtual void defineMenu(void) = 0;
		
		void appendMenu(const char *textMenu);
		void appendOptionIntoLastMenu(class CRcpOpcAreaDib *rcpOption);
        void appendOptionWithIconIntoLastMenu(class CImg **imageIcon, class CRcpOpcAreaDib *rcpOption);
        void appendSeparatorIntoLastMenu(void);
        
	// Estas funciones deber�as estar fuera de aqu�.
		void createData(void);
		void close(void);
		
		struct panel_t *panel(void);
		void appendMenus(struct menubar_t *menubar);
		
		void startOption(struct dialogo_t *dlgFather, class CRcpOpcAreaDib *rcpOption);
		void setStatusLine(class CStatusLine *statusLine);
		
	private:

		struct prv_dataPrivateWorspace_t *m_dataPrivate;
};
