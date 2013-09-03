// Grafo de procesamiento de im�genes.

class CProcessGraphImages
{
	public:
	
		CProcessGraphImages();
		virtual ~CProcessGraphImages();
		
		void initDefineGraphProcess();
		
		void addProcessImage(const char *idProcess, class CProcessImage **processImage);
		
		void addDependProcess(const char *idProcessIn, const char *idProcessOut);
		
		void closeDefineGraphProcess();
		
		void run();
		void runNextLevel();
		
		void defineLayers(class IGraphics *graphics) const;

		void draw(class IGraphics *graphics) const;
		
	private:
	
		struct SchedulerTask_t *m_schedulerImages;
		ArrClass(CProcessImage) *m_allProcess; 
};
