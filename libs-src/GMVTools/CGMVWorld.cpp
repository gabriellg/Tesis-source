/* -----------------------------------------------------------------------------
*  CGMVReaderXML
*  Author: Gabriel López García (gabriellg AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include "CGMVWorld.hpp"

#include "asrtbas.h"
#include "memory.h"
#include "strbas.h"
#include "CArrayString.hpp"
#include "CArray.hpp"

#include "CDocXML.hpp"
#include "ILog.hpp"

enum prv_typeElementWorld_t
{
	prv_PRIMITIVE,
	prv_TRANSFORM,
	prv_ACTOR
};

class CPrivateElementWorld
{
    public:

        CPrivateElementWorld(
                enum prv_typeElementWorld_t element,
                char *id,
                class CArray<CPrivateElementWorld> **elements);

        ~CPrivateElementWorld();

        enum prv_typeElementWorld_t m_element;
        char *m_id;
        class CArray<CPrivateElementWorld> *m_elements;
};

struct prv_dataPrivateGMVWorld_t
{
    class CArrayString *ids_primitives;
	class CArrayString *ids_transforms;
	class CArrayString *ids_actors;
	class CArrayString *ids_generators;
	class CArrayString *ids_displays;

	class CArray<CPrivateElementWorld> *elementsWorld;
};

struct prv_dataPrivatePrvProcessNodeElementsContext_t
{
	char *tag;
	class CArrayString *ids;
	class ILog *log;
};

class CPrvProcessNodeElementsContext : public IProcessNodeXML
{
	public:

		CPrvProcessNodeElementsContext(const char *tag, class CArrayString *ids, class ILog *log);
		~CPrvProcessNodeElementsContext();

		virtual void processNode(const class CNodeXML *node);

	private:

		struct prv_dataPrivatePrvProcessNodeElementsContext_t *m_dataPrivate;
};

struct prv_dataProcessElements_t
{
	const class CArrayString *ids_primitives;
	const class CArrayString *ids_transforms;
	const class CArrayString *ids_actors;
	class CArray<CPrivateElementWorld> *elementsWorld;
	class ILog *log;
};

class CPrvProcessElements : public IProcessNodeXML
{
	public:

		CPrvProcessElements(unsigned long recursiveLevel, struct prv_dataProcessElements_t **dataProcess);
		~CPrvProcessElements();

		virtual void processNode(const class CNodeXML *node);

	private:

		unsigned long m_recursiveLevel;
		struct prv_dataProcessElements_t *m_dataProcess;
};

static const char *PRV_ID_CONTEXT_WORLD = "World";

static const char *PRV_ID_CONTEXT_PRIMITIVES = "Primitives";
static const char *PRV_ID_ELEMENT_PRIMITIVE = "Primitive";

static const char *PRV_ID_CONTEXT_TRANSFORMS = "Transforms";
static const char *PRV_ID_ELEMENT_TRANSFORM = "Transform";

static const char *PRV_ID_CONTEXT_ACTORS = "Actors";
static const char *PRV_ID_ELEMENT_ACTOR = "Actor";

static const char *PRV_ID_CONTEXT_GENERATORS = "Generators";
static const char *PRV_ID_ELEMENT_GENERATOR = "Generator";

static const char *PRV_ID_CONTEXT_DISPLAYS = "Displays";
static const char *PRV_ID_ELEMENT_DISPLAY = "Display";

static const char *PRV_ID_CONTEXT_SCENE = "Scene";
static const char *PRV_ID_ELEMENT_SCENE = "ElementWorld";

static const char *PRV_NO_CAN_OPEN_DOC = "It can't open GMV document";
static const char *PRV_MSG_NOT_EXIST_ATTRIBUTE = "It must be '%s' attribute";
static const char *PRV_MSG_NOT_EXIST_WORLD = "It must be 'World' tag";
static const char *PRV_MSG_NOT_EXIST_SCENE = "It must be 'Scene' tag";
static const char *PRV_ERROR_TYPE_CLASS = "'Class' attribute must be 'Actor' or 'Transform' or 'Primitive'";
static const char *PRV_ELEMENT_NOT_GMV = "It isn't GMV tag";
static const char *PRV_ELEMENT_NOT_EXIST = "The element world of type '%s' has '%s' id but it don't exists as '%s'";
static const char *PRV_WARNING_NO_ELEMENTS = "There are'nt elements";

//---------------------------------------------------------------

static void prv_warningNotAttribute(class ILog *log, unsigned long line, const char *attribute)
{
	char *messageWarning;

	assert_no_null(log);

	messageWarning = strbas_printf(PRV_MSG_NOT_EXIST_ATTRIBUTE, attribute);
	log->warningWithLine(line, messageWarning);
	strbas_destroy(&messageWarning);
}

//---------------------------------------------------------------

CPrivateElementWorld::CPrivateElementWorld(
        enum prv_typeElementWorld_t element,
        char *id,
        class CArray<CPrivateElementWorld> **elements)
{
	m_element = element;
	m_id = strbas_copyString(id);
	m_elements = ASSIGN_PP_NO_NULL(elements, class CArray<CPrivateElementWorld>);
}

//---------------------------------------------------------------

CPrivateElementWorld::~CPrivateElementWorld()
{
	strbas_destroy(&m_id);
	delete m_elements;
}

//---------------------------------------------------------------

static struct prv_dataPrivateGMVWorld_t *prv_create(
                class CArrayString **ids_primitives,
                class CArrayString **ids_transforms,
                class CArrayString **ids_actors,
                class CArrayString **ids_generators,
                class CArrayString **ids_displays,
				class CArray<CPrivateElementWorld> **elementsWorld)
{
	struct prv_dataPrivateGMVWorld_t *dataPrivate;

	dataPrivate = MALLOC(struct prv_dataPrivateGMVWorld_t);

	dataPrivate->ids_primitives = ASSIGN_PP_NO_NULL(ids_primitives, class CArrayString);
	dataPrivate->ids_transforms = ASSIGN_PP_NO_NULL(ids_transforms, class CArrayString);
	dataPrivate->ids_actors = ASSIGN_PP_NO_NULL(ids_actors, class CArrayString);
	dataPrivate->ids_generators = ASSIGN_PP_NO_NULL(ids_generators, class CArrayString);
	dataPrivate->ids_displays = ASSIGN_PP_NO_NULL(ids_displays, class CArrayString);
	dataPrivate->elementsWorld = ASSIGN_PP_NO_NULL(elementsWorld, class CArray<CPrivateElementWorld>);

	return dataPrivate;
}

//---------------------------------------------------------------

static void prv_destroy(struct prv_dataPrivateGMVWorld_t **dataPrivate)
{
	assert_no_null(dataPrivate);
	assert_no_null(*dataPrivate);

	delete (*dataPrivate)->ids_primitives;
	delete (*dataPrivate)->ids_transforms;
	delete (*dataPrivate)->ids_actors;
	delete (*dataPrivate)->ids_generators;
	delete (*dataPrivate)->ids_displays;

	delete (*dataPrivate)->elementsWorld;

	FREE_T(dataPrivate, struct prv_dataPrivateGMVWorld_t);
}

//-----------------------------------------------------------------------

static struct prv_dataProcessElements_t *prv_createDataProcessElement(
						const class CArrayString *ids_primitives,
						const class CArrayString *ids_transforms,
						const class CArrayString *ids_actors,
						class CArray<CPrivateElementWorld> *elementsWorld,
						class ILog *log)
{
	struct prv_dataProcessElements_t *dataProcess;

	dataProcess = MALLOC(struct prv_dataProcessElements_t);

	dataProcess->ids_primitives = ids_primitives;
	dataProcess->ids_transforms = ids_transforms;
	dataProcess->ids_actors = ids_actors;
	dataProcess->elementsWorld = elementsWorld;
	dataProcess->log = log;

	return dataProcess;
}

//-----------------------------------------------------------------------

static void prv_destroyDataProcess(struct prv_dataProcessElements_t **dataProcess)
{
	FREE_T(dataProcess, struct prv_dataProcessElements_t);
}

//-----------------------------------------------------------------------

static struct prv_dataPrivatePrvProcessNodeElementsContext_t *prv_createDataPrivatePrvProcessNodeElementsContext(
						const char *tag,
						class CArrayString *ids,
						ILog *log)
{
	struct prv_dataPrivatePrvProcessNodeElementsContext_t *dataPrivate;

	dataPrivate = MALLOC(struct prv_dataPrivatePrvProcessNodeElementsContext_t);

	dataPrivate->tag = strbas_copyString(tag);
	dataPrivate->ids = ids;
	dataPrivate->log = log;

	return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroyDataPrivatePrvProcessNodeElementsContext(struct prv_dataPrivatePrvProcessNodeElementsContext_t **dataPrivate)
{
	assert_no_null(dataPrivate);
	assert_no_null(*dataPrivate);

	strbas_destroy(&(*dataPrivate)->tag);

	FREE_T(dataPrivate, struct prv_dataPrivatePrvProcessNodeElementsContext_t);
}

//-----------------------------------------------------------------------

CPrvProcessNodeElementsContext::CPrvProcessNodeElementsContext(const char *tag, class CArrayString *ids, ILog *log)
{
	m_dataPrivate = prv_createDataPrivatePrvProcessNodeElementsContext(tag, ids, log);
}

//-----------------------------------------------------------------------

CPrvProcessNodeElementsContext::~CPrvProcessNodeElementsContext()
{
	prv_destroyDataPrivatePrvProcessNodeElementsContext(&m_dataPrivate);
}

//---------------------------------------------------------------

void CPrvProcessNodeElementsContext::processNode(const class CNodeXML *node)
{
	assert_no_null(m_dataPrivate);
	assert_no_null(m_dataPrivate->log);

	if (node->isTag(m_dataPrivate->tag) == true)
	{
		char *valueId;

		valueId = NULL;
		if (node->getId(&valueId) == true)
		    m_dataPrivate->ids->addDestroyingString(&valueId);
		else
			prv_warningNotAttribute(m_dataPrivate->log, node->getLine(), "id");
	}
	else
	{
		m_dataPrivate->log->warningWithLine(node->getLine(), PRV_ELEMENT_NOT_GMV);
	}
}

//---------------------------------------------------------------

CPrvProcessElements::CPrvProcessElements(unsigned long recursiveLevel, struct prv_dataProcessElements_t **dataProcess)
{
	assert(recursiveLevel < 100);
	m_recursiveLevel = recursiveLevel + 1;
	m_dataProcess = ASSIGN_PP_NO_NULL(dataProcess, struct prv_dataProcessElements_t);
}

//---------------------------------------------------------------

CPrvProcessElements::~CPrvProcessElements()
{
	prv_destroyDataProcess(&m_dataProcess);
}

//---------------------------------------------------------------

static bool prv_typeElement(const char *idClass, enum prv_typeElementWorld_t *typeElement)
{
	bool isTypeOk;
	enum prv_typeElementWorld_t typeElement_loc;

	if (strbas_equalsStringWithoutMatchCase(idClass, PRV_ID_ELEMENT_PRIMITIVE) == true)
	{
		typeElement_loc = prv_PRIMITIVE;
		isTypeOk = true;
	}
	else if (strbas_equalsStringWithoutMatchCase(idClass, PRV_ID_ELEMENT_TRANSFORM) == true)
	{
		typeElement_loc = prv_TRANSFORM;
		isTypeOk = true;
	}
	else if (strbas_equalsStringWithoutMatchCase(idClass, PRV_ID_ELEMENT_ACTOR) == true)
	{
		isTypeOk = true;
		typeElement_loc = prv_ACTOR;
	}
	else
	{
		isTypeOk = false;
		typeElement_loc = prv_PRIMITIVE;
	}

	if (isTypeOk == true && typeElement != NULL)
		*typeElement = typeElement_loc;

	return isTypeOk;
}

//---------------------------------------------------------------

static bool prv_existId(
					const class CArrayString *ids_primitives,
					const class CArrayString *ids_transforms,
					const class CArrayString *ids_actors,
					enum prv_typeElementWorld_t typeElement,
					const char *idStr,
					unsigned long line,
					class ILog *log)
{
	bool existId;
	const class CArrayString *ids;
	const char *typeElementStr;

	switch(typeElement)
	{
		case prv_PRIMITIVE:

			ids = ids_primitives;
			typeElementStr = "Primitive";
			break;

		case prv_TRANSFORM:

			ids = ids_transforms;
			typeElementStr = "Transform";
			break;

		case prv_ACTOR:

			ids = ids_actors;
			typeElementStr = "Actor";
			break;

		default_error();
	}

	existId = ids->searchString(idStr, NULL);
	if (existId == false)
	{
		char *messageError;

		messageError = strbas_printf(PRV_ELEMENT_NOT_EXIST, typeElementStr, idStr, typeElementStr);
		log->errorWithLine(line, messageError);
		strbas_destroy(&messageError);
	}

	return existId;
}

//---------------------------------------------------------------

void CPrvProcessElements::processNode(const class CNodeXML *node)
{
	assert_no_null(node);

	if (node->isTag(PRV_ID_ELEMENT_SCENE) == true)
	{
		char *idStr, *idClass;
		enum prv_typeElementWorld_t typeElement;
		struct CPrivateElementWorld *elementWorld;
		bool isElementOk, hasClass, hasId;

		idClass = NULL;
		hasClass = node->getClass(&idClass);

		idStr = NULL;
		hasId = node->getId(&idStr);

		if (hasClass == true)
		{
			isElementOk = prv_typeElement(idClass, &typeElement);

			if (isElementOk == false)
				m_dataProcess->log->warningWithLine(node->getLine(), PRV_ERROR_TYPE_CLASS);
		}
		else
		{
			isElementOk = false;
			prv_warningNotAttribute(m_dataProcess->log, node->getLine(), "class");
		}

		if (hasId == false)
		{
			isElementOk = false;
			prv_warningNotAttribute(m_dataProcess->log, node->getLine(), "id");
		}

		if (isElementOk == true)
		{
			isElementOk = prv_existId(
							m_dataProcess->ids_primitives, m_dataProcess->ids_transforms, m_dataProcess->ids_actors,
							typeElement, idStr, node->getLine(), m_dataProcess->log);
		}

		if (isElementOk == true)
		{
			class CArray<CPrivateElementWorld> *elementsTransform;

			elementsTransform = new CArray<CPrivateElementWorld>;

			switch(typeElement)
			{
				case prv_PRIMITIVE:
				case prv_ACTOR:

					break;

				case prv_TRANSFORM:
				{
					class CPrvProcessElements *process;
					struct prv_dataProcessElements_t *dataProcessTransform;

					dataProcessTransform = prv_createDataProcessElement(
							m_dataProcess->ids_primitives, m_dataProcess->ids_transforms,
							m_dataProcess->ids_actors,
							elementsTransform, m_dataProcess->log);

					process = new CPrvProcessElements(m_recursiveLevel, &dataProcessTransform);

					node->processAllChildren(process);

					delete process;
					break;
				}
				default_error();
			}

			if (isElementOk == true)
			{
				elementWorld = new CPrivateElementWorld(typeElement, idStr, &elementsTransform);
				m_dataProcess->elementsWorld->add(elementWorld);
			}
			else
			    delete elementsTransform;
		}

		if (hasId == true)
			strbas_destroy(&idStr);

		if (hasClass == true)
			strbas_destroy(&idClass);
	}
	else
	{
		m_dataProcess->log->warningWithLine(node->getLine(), PRV_ELEMENT_NOT_GMV);
	}
}

//---------------------------------------------------------------

static void prv_readContext(
				const class CNodeXML *world,
				const char *context,
				const char *elementContext,
				class ILog *log,
				class CArrayString *ids)
{
	class CNodeXML *nodeContext;

	nodeContext = NULL;
	if (world->findInChildren(context, &nodeContext) == true)
	{
		CPrvProcessNodeElementsContext *process;

		assert_no_null(nodeContext);

		process = new CPrvProcessNodeElementsContext(elementContext, ids, log);
		nodeContext->processAllChildren(process);
		delete process;
	}
}

//---------------------------------------------------------------

static void prv_readElements(
					const class CNodeXML *nodeScene,
					class ILog *log,
					const class CArrayString *ids_primitives,
					const class CArrayString *ids_transforms,
					const class CArrayString *ids_actors,
					class CArray<CPrivateElementWorld> *elementsWorld)
{
	struct prv_dataProcessElements_t *dataProcess;
	CPrvProcessElements *process;

	assert_no_null(elementsWorld);
    assert_no_null(log);

	dataProcess = prv_createDataProcessElement(
					ids_primitives, ids_transforms, ids_actors,
				    elementsWorld, log);

	process = new CPrvProcessElements(0, &dataProcess);

	nodeScene->processAllChildren(process);

	if (elementsWorld->size() == 0)
		log->warningWithLine(nodeScene->getLine(), PRV_WARNING_NO_ELEMENTS);

	delete process;
}

//---------------------------------------------------------------

static bool prv_readXML(
				const char *filename,
				class ILog *log,
				struct prv_dataPrivateGMVWorld_t **dataPrivate)
{
	bool isRead;
	class CDocXML *doc;

	doc = NULL;
	isRead = CDocXML::parseXML(filename, &doc);

	if (isRead == true)
	{
		class CNodeXML *world;

		assert_no_null(doc);
		world = doc->rootNode();

		isRead = world->isTag(PRV_ID_CONTEXT_WORLD);

		if (isRead == true)
		{
			class CArrayString *ids_primitives, *ids_transforms, *ids_actors;
			class CArrayString *ids_generators, *ids_displays;
			class CArray<CPrivateElementWorld> *elementsWorld;
			class CNodeXML *nodeTagScene;

			assert_no_null(dataPrivate);

			ids_primitives = new CArrayString();
			ids_transforms = new CArrayString();
			ids_actors = new CArrayString();
			ids_generators = new CArrayString();
			ids_displays = new CArrayString();
			elementsWorld = new CArray<CPrivateElementWorld>;

			prv_readContext(world, PRV_ID_CONTEXT_PRIMITIVES, PRV_ID_ELEMENT_PRIMITIVE, log, ids_primitives);
			prv_readContext(world, PRV_ID_CONTEXT_TRANSFORMS, PRV_ID_ELEMENT_TRANSFORM, log, ids_transforms);
			prv_readContext(world, PRV_ID_CONTEXT_ACTORS, PRV_ID_ELEMENT_ACTOR, log, ids_actors);
			prv_readContext(world, PRV_ID_CONTEXT_GENERATORS, PRV_ID_ELEMENT_GENERATOR, log, ids_generators);
			prv_readContext(world, PRV_ID_CONTEXT_DISPLAYS, PRV_ID_ELEMENT_DISPLAY, log, ids_displays);

			nodeTagScene = NULL;
			isRead = world->findInChildren(PRV_ID_CONTEXT_SCENE, &nodeTagScene);

			if (isRead == true)
				prv_readElements(nodeTagScene, log, ids_primitives, ids_transforms, ids_actors, elementsWorld);
			else
				log->error("%s", PRV_MSG_NOT_EXIST_SCENE);

			if (isRead == true)
				*dataPrivate = prv_create(&ids_primitives, &ids_transforms, &ids_actors, &ids_generators, &ids_displays, &elementsWorld);
			else
			{
				delete ids_primitives;
				delete ids_transforms;
				delete ids_actors;
				delete ids_generators;
				delete ids_displays;

				delete elementsWorld;
			}
		}
		else
			log->error("%s", PRV_MSG_NOT_EXIST_WORLD);

		delete doc;
	}
	else
		log->error("%s", PRV_NO_CAN_OPEN_DOC);

	return isRead;
}

//---------------------------------------------------------------

CGMVWorld::CGMVWorld(struct prv_dataPrivateGMVWorld_t **dataPrivate)
{
	m_dataPrivate = ASSIGN_PP_NO_NULL(dataPrivate, struct prv_dataPrivateGMVWorld_t);
}

//---------------------------------------------------------------

CGMVWorld::~CGMVWorld()
{
	prv_destroy(&m_dataPrivate);
}

//---------------------------------------------------------------

bool CGMVWorld::readXML(const char *fileNameGMV, class ILog *log, class CGMVWorld **world)
{
	bool isRead;
	struct prv_dataPrivateGMVWorld_t *dataPrivate;

	assert_no_null(world);

	log->setContext(fileNameGMV);

	isRead = prv_readXML(fileNameGMV, log, &dataPrivate);
	if (isRead == true)
	{
		log->line("File %s is OK", fileNameGMV);
		*world = new CGMVWorld(&dataPrivate);
	}
	else
		log->line("File %s has errors", fileNameGMV);

	log->setContext("");

	return isRead;
}
