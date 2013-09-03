/* -----------------------------------------------------------------------------
*  CDocXML
*  Author: Gabriel López García (gabriel.lg AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include "CDocXML.hpp"

#include "libxml.inl"

#include "asrtbas.h"
#include "memory.h"
#include "strbas.h"

//-----------------------------------------------------------------------

CNodeXML::CNodeXML(struct prv_dataPrivateNodeXML_t *xmlNode)
{
	assert_no_null(xmlNode);
	m_dataPrivate = xmlNode;
}

//-----------------------------------------------------------------------

CNodeXML::~CNodeXML()
{
	;
}

//-----------------------------------------------------------------------

static xmlNode *prv_castToXmlNode(struct prv_dataPrivateNodeXML_t *dataPrivate)
{
	assert_no_null(dataPrivate);
	return (xmlNode *)dataPrivate;
}

//---------------------------------------------------------------

static bool prv_isTag(const xmlNode *node, const char *tagString)
{
	assert_no_null(node);

	if (xmlStrEqual(node->name, (xmlChar *)tagString) == 1)
		return true;
	else
		return false;
}

//-----------------------------------------------------------------------

void CNodeXML::processAllChildren(class IProcessNodeXML *process) const
{
	xmlNode *node, *nodePrivate;

	assert_no_null(process);

	nodePrivate = prv_castToXmlNode(m_dataPrivate);

	for (node = nodePrivate->xmlChildrenNode; node != NULL;  node = node->next)
	{
		if (prv_isTag(node, "text") == false)
		{
			class CNodeXML nodeXML((struct prv_dataPrivateNodeXML_t *)node);

			process->processNode(&nodeXML);
		}
	}
}

//-----------------------------------------------------------------------

bool CNodeXML::findInChildren(const char *tagChild, class CNodeXML **nodeXML) const
{
	xmlNode *parent, *node;

	assert_no_null(nodeXML);

	parent = prv_castToXmlNode(m_dataPrivate);

	for (node = parent->xmlChildrenNode; node != NULL;  node = node->next)
	{
		if (prv_isTag(node, tagChild) == true)
		{
			*nodeXML = new CNodeXML((struct prv_dataPrivateNodeXML_t *)node);
			return true;
		}
	}

	return false;
}

//-----------------------------------------------------------------------

bool CNodeXML::isTag(const char *tag) const
{
	xmlNode *node;

	node = 	prv_castToXmlNode(m_dataPrivate);
	return prv_isTag(node, tag);
}

//-----------------------------------------------------------------------

unsigned long CNodeXML::getLine() const
{
	xmlNode *node;

	node = 	prv_castToXmlNode(m_dataPrivate);
	return node->line;
}

//-----------------------------------------------------------------------

static bool prv_getProp(xmlNode *node, const char *nameProp, char **valueProp)
{
	bool hasProp;
	xmlChar *idStr;

	assert_no_null(valueProp);

	idStr = xmlGetProp(node, (xmlChar *)nameProp);

	if (idStr != NULL)
	{
		hasProp = true;
		*valueProp = strbas_copyString((char *)idStr);
		xmlFree(idStr);
	}
	else
		hasProp = false;

	return hasProp;
}

//-----------------------------------------------------------------------

bool CNodeXML::getId(char **valueId) const
{
	xmlNode *node;

	node = 	prv_castToXmlNode(m_dataPrivate);
	return prv_getProp(node, "id", valueId);
}

//-----------------------------------------------------------------------

bool CNodeXML::getClass(char **valueClass) const
{
	xmlNode *node;

	node = 	prv_castToXmlNode(m_dataPrivate);
	return prv_getProp(node, "class", valueClass);
}

//-----------------------------------------------------------------------

bool CNodeXML::getProp(const char *nameProp, char **valueId) const
{
	xmlNode *node;

	node = 	prv_castToXmlNode(m_dataPrivate);
	return prv_getProp(node, "class", valueId);
}

//-----------------------------------------------------------------------

bool CDocXML::parseXML(const char *fileNameXML, class CDocXML **docXML)
{
	bool isParsed;
	struct prv_dataPrivateDocXML_t *doc;

	assert_no_null(docXML);

	doc = (struct prv_dataPrivateDocXML_t *)xmlParseFile(fileNameXML);

	if (doc != NULL)
	{
		*docXML = new CDocXML(&doc);
		isParsed = true;
	}
	else
		isParsed = false;

	return isParsed;
}

//-----------------------------------------------------------------------

CDocXML::CDocXML(struct prv_dataPrivateDocXML_t **dataPrivate)
{
	m_dataPrivate = ASSIGN_PP_NO_NULL(dataPrivate, struct prv_dataPrivateDocXML_t);
}

//-----------------------------------------------------------------------

CDocXML::~CDocXML()
{
	xmlDoc *doc;

	assert_no_null(m_dataPrivate);
	doc = (xmlDoc *)m_dataPrivate;

	xmlFreeDoc(doc);
	m_dataPrivate = NULL;
}

//-----------------------------------------------------------------------

class CNodeXML *CDocXML::rootNode() const
{
	struct prv_dataPrivateNodeXML_t *nodeRoot;
	xmlDoc *doc;

	assert_no_null(m_dataPrivate);
	doc = (xmlDoc *)m_dataPrivate;

	nodeRoot = (struct prv_dataPrivateNodeXML_t *)xmlDocGetRootElement(doc);
	return new CNodeXML(nodeRoot);
}

