/* -----------------------------------------------------------------------------
*  CGMVReaderXML
*  Author: Gabriel López García (gabriellg AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

class CGMVWorld
{
	public:

		~CGMVWorld(void);

		static bool readXML(const char *fileNameGMV, class ILog *log, class CGMVWorld **world);

	private:

		CGMVWorld(struct prv_dataPrivateGMVWorld_t **dataPrivate);
		struct prv_dataPrivateGMVWorld_t *m_dataPrivate;
};
