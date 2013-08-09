// Define una aplicacion.

class CAplication
{
	public:
		CAplication(const char *titleAplication);
		virtual ~CAplication();
		
		virtual class CWorkspace *createWorkspace() = 0;
		virtual struct marcogrf_t *getFrameworkAplication(void) = 0;
		
		void run(int argc, char **argv);
			
	private:
	
		char *m_titleAplication;
        bool m_withHandleArchive;
};
