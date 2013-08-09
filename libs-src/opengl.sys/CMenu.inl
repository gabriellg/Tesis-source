class CMenu
{
	public:
	
		CMenu();
		virtual ~CMenu();

		bool hasOptions(void) const;
		
		void appendOption(const char *nameOption, unsigned long idOption, char key, class IOptionOGL **option);
		bool searchOptionKeyPressed(char keyPressed, class IOptionOGL **optionFound);
		class IOptionOGL *getOptionId(unsigned long idOption);
		
		void appendOptionsToMenuOGL(unsigned long idMenuOGL);

	private:
	
		struct SDataPrivateMenu *m_dataPrivate;
};
