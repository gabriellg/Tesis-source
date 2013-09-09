/* -----------------------------------------------------------------------------
*  CDocXML
*  Author: Gabriel López García (gabriel.lg AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

class IProcessNodeXML
{
	public:

		virtual ~IProcessNodeXML() {;}

		virtual void processNode(const class CNodeXML *node) = 0;
};

class CDocXML
{
	public:

		static bool parseXML(const char *fileNameXML, class CDocXML **docXML);
		virtual ~CDocXML();

		class CNodeXML *rootNode() const;

	private:

		CDocXML(struct prv_dataPrivateDocXML_t **dataPrivate);

		struct prv_dataPrivateDocXML_t *m_dataPrivate;
};

class CNodeXML
{
	public:

		virtual ~CNodeXML();

		void processAllChildren(IProcessNodeXML *process) const;
		bool findInChildren(const char *tagChild, class CNodeXML **nodeXML) const;
		bool isTag(const char *tag) const;
		unsigned long getLine() const;

		bool getId(char **valueId) const;
		bool getClass(char **valueClass) const;
		bool getProp(const char *nameProp, char **valueId) const;

	private:

		CNodeXML(struct prv_dataPrivateNodeXML_t *xmlNode);

		struct prv_dataPrivateNodeXML_t *m_dataPrivate;

		friend class CDocXML;
};
