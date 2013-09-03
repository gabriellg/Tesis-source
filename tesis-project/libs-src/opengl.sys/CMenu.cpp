#include "CMenu.inl"

#include "IOptionOGL.hpp"

#include "opengl.inl"

#include "asrtbas.h"
#include "memory.h"

#include "CArray.hpp"
#include "CMath.hpp"
#include "CString.hpp"

struct SPrvOption
{
    SPrvOption(const char *nameOption_param, unsigned long idOption_param, char key_param, class IOptionOGL **option_param);
    ~SPrvOption();

    char *nameOption;
    unsigned long idOption;
    char key;
    class IOptionOGL *option;
};

struct SDataPrivateMenu
{
    unsigned long idMenuOGL;

    class CArray<SPrvOption> *options;
};

//-----------------------------------------------------------------------

SPrvOption::SPrvOption(const char *nameOption_param, unsigned long idOption_param, char key_param, class IOptionOGL **option_param)
{
    nameOption = CString::copy(nameOption_param);
    idOption = idOption_param;
    key = key_param;
    option = ASSIGN_PP_NO_NULL(option_param, class IOptionOGL);
}

//-----------------------------------------------------------------------

SPrvOption::~SPrvOption()
{
    assert_no_null(option);

    CString::free(&nameOption);
    delete option;
}

//-----------------------------------------------------------------------

static struct SDataPrivateMenu *prv_createMenu(unsigned long idMenuOGL, class CArray<SPrvOption> **options)
{
    struct SDataPrivateMenu *dataPrivate;

    dataPrivate = MALLOC(struct SDataPrivateMenu);

    dataPrivate->idMenuOGL = idMenuOGL;
    dataPrivate->options = ASSIGN_PP_NO_NULL(options, class CArray<SPrvOption>);

    return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroyMenu(struct SDataPrivateMenu **dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(*dataPrivate);

    if ((*dataPrivate)->idMenuOGL != CMath::maxULong())
        glutDestroyMenu((*dataPrivate)->idMenuOGL);

    delete (*dataPrivate)->options;

    FREE_T(dataPrivate, struct SDataPrivateMenu);
}

//-----------------------------------------------------------------------

CMenu::CMenu()
{
    class CArray<SPrvOption> *options;
    unsigned long idMenuOGL;

    idMenuOGL = CMath::maxULong();
    options = new CArray<SPrvOption>;

    m_dataPrivate = prv_createMenu(idMenuOGL, &options);
}

//-----------------------------------------------------------------------

CMenu::~CMenu()
{
    prv_destroyMenu(&m_dataPrivate);
}

//---------------------------------------------------------------

bool CMenu::hasOptions(void) const
{
    if (m_dataPrivate->options->size() > 0)
        return true;
    else
        return false;
}

//---------------------------------------------------------------

static bool prv_isOptionKeyPressed(const struct SPrvOption *option, const char *keyPressed)
{
	assert_no_null(option);
	assert_no_null(keyPressed);
	
	if (option->key == *keyPressed)
	    return true;
	else
	    return false;
}

//---------------------------------------------------------------

static bool prv_isOptionId(const struct SPrvOption *option, const unsigned long *id)
{
    assert_no_null(id);

    if (option->idOption == *id)
        return true;
    else
        return false;
}

//---------------------------------------------------------------

void CMenu::appendOption(const char *nameOption, unsigned long idOption, char key, class IOptionOGL **option)
{
    struct SPrvOption *optionMenu;

    assert_no_null(m_dataPrivate);

    optionMenu = new SPrvOption(nameOption, idOption, key, option);
	m_dataPrivate->options->add(optionMenu);
}

//---------------------------------------------------------------

bool CMenu::searchOptionKeyPressed(char keyPressed, class IOptionOGL **optionFound)
{
	bool exist;
	unsigned long ind;
	
	assert_no_null(optionFound);

	exist = m_dataPrivate->options->existOnlyOneElement<char>(&keyPressed, prv_isOptionKeyPressed, &ind);

	if (exist == true)
	{
	    struct SPrvOption *option;

	    option = m_dataPrivate->options->get(ind);
		*optionFound = option->option;
	}
		
	return exist;
}

//---------------------------------------------------------------

class IOptionOGL *CMenu::getOptionId(unsigned long idOption)
{
    struct SPrvOption *option;

    option = m_dataPrivate->options->getOnlyOneElement<unsigned long>(&idOption, prv_isOptionId);
    return option->option;
}

//---------------------------------------------------------------

void CMenu::appendOptionsToMenuOGL(unsigned long idMenuOGL)
{
    unsigned long num;

    assert_no_null(m_dataPrivate);
    assert(m_dataPrivate->idMenuOGL == CMath::maxULong());

    m_dataPrivate->idMenuOGL = idMenuOGL;

    num = m_dataPrivate->options->size();

    for (unsigned long i = 0; i < num; i++)
    {
        const struct SPrvOption *option;

        option = m_dataPrivate->options->get(i);
        assert_no_null(option);

        glutAddMenuEntry(option->nameOption, option->idOption);
    }
}
