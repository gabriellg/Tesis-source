// Utilidades para generar codigo.

#include "CGenUtil.inl"

#include "asrtbas.h"

#include "CArrayString.hpp"
#include "CString.hpp"

//-----------------------------------------------------------------------

void CGenUtil::appendComentary(class CArrayString *sourceCode)
{
    assert_no_null(sourceCode);
    sourceCode->add( "//-----------------------------------------------------------------------");
}

//-----------------------------------------------------------------------

void CGenUtil::appendComentaryTextDestroying(class CArrayString *sourceCode, char **textComentary)
{
    char *lineComentary;

    assert_no_null(textComentary);
    assert_no_null(*textComentary);

    lineComentary = CString::printf("// %s", *textComentary);
    sourceCode->addDestroyingString(&lineComentary);
    CString::free(textComentary);
}

//-----------------------------------------------------------------------

void CGenUtil::appendLineWhite(class CArrayString *sourceCode)
{
    assert_no_null(sourceCode);
	sourceCode->add("");
}

//-----------------------------------------------------------------------

void CGenUtil::appendStaticFunctionDefinition(
				const char *typeReturn,
				const char *nameFuncion, 
				const char *parametersWithoutParenthesis, 
				class CArrayString *sourceCode)
{
	char *strDefinition;
	
    assert_no_null(sourceCode);

	strDefinition = CString::printf("static %s %s(%s)", typeReturn, nameFuncion, parametersWithoutParenthesis);
	sourceCode->addDestroyingString(&strDefinition);
}

//-----------------------------------------------------------------------

static void prv_appendWithLevelTab(unsigned long levelTab, const char *text, class CArrayString *sourceCode)
{
	char *strTextWithLevelTab;
	
	if (levelTab > 0)
	{
		size_t numChar;
		char *strTab;
		
		numChar = levelTab * 4;
		strTab = CString::createWithInitialitation(' ', numChar);
		strTextWithLevelTab = CString::printf("%s%s", strTab, text);
		
		CString::free(&strTab);
	}
	else
		strTextWithLevelTab = CString::copy(text);
	
	sourceCode->addDestroyingString(&strTextWithLevelTab);
}

//-----------------------------------------------------------------------

static void prv_appendBodyCode(
					unsigned long levelTab, 
					const class CArrayString *bodyCode,
					class CArrayString *sourceCode)
{
    unsigned long num;

    num = bodyCode->size();

    for (unsigned long i = 0; i < num; i++)
    {
        const char *strCode;

        strCode = bodyCode->get(i);
        prv_appendWithLevelTab(levelTab, strCode, sourceCode);
    }
}

//-----------------------------------------------------------------------

void CGenUtil::appendDefinitionStaticArrayData(
				unsigned long levelTab,
				const char *typeVariable,
				const char *nameVariable,
				unsigned long numElements,
				class CArrayString **dataCode,
				class CArrayString *sourceCode)
{
	char *definitionVariable;
	
	assert_no_null(dataCode);
	
    definitionVariable = CString::printf("static %s %s[%d]=", typeVariable, nameVariable, numElements);
	prv_appendWithLevelTab(levelTab, definitionVariable, sourceCode);
	CString::free(&definitionVariable);

	prv_appendWithLevelTab(levelTab, "{", sourceCode);

	prv_appendBodyCode(levelTab + 1, *dataCode, sourceCode);

	prv_appendWithLevelTab(levelTab, "};", sourceCode);

	delete *dataCode;
	*dataCode = NULL;
}

//-----------------------------------------------------------------------

void CGenUtil::appendBodyCode(
				unsigned long levelTab,
				class CArrayString **bodyCode,
				class CArrayString *sourceCode)
{
	assert_no_null(bodyCode);
	
	prv_appendWithLevelTab(levelTab, "{", sourceCode);
	
	prv_appendBodyCode(levelTab + 1, *bodyCode, sourceCode);
	
	prv_appendWithLevelTab(levelTab, "}", sourceCode);
	
	delete *bodyCode;
	*bodyCode = NULL;
}
