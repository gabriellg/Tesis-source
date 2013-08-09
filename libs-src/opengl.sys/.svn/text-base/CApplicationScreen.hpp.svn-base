// Define una aplicación con pantalla.

class CApplicationScreen
{
	public:
	
		CApplicationScreen(const char *title);
		virtual ~CApplicationScreen();
		
		void appendOption(const char *nameOption, char key, class IOptionOGL **option);
		bool hasOptions(void) const;

		void run(int argc, char **argv);

		void changeWindows(int width, int height);
		void draw(void);
		void executeKey(char keyPressed);
		void executeIdOption(unsigned long idOption);
		void mouseClick(int button, int state, int x, int y);
        void moveMouseWithButtonPulsed(int x, int y);

	protected:

		virtual class CLight *createLight0(void);
        virtual void initAplication(void);

		virtual void initMaterialsAndBlocks(class IGraphics *graphics) = 0;
		virtual void drawScreen(class IGraphics *graphics) = 0;
		
	private:

        void calculateLimits(void);
        void executeOption(class IOptionOGL *option);
        void initialization(void);

		struct SPrvDataPrivateApplicationScreen *m_dataPrivate;
};
