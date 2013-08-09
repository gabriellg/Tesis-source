/* -----------------------------------------------------------------------------
 *  CShaderGL.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CShaderGL.hpp"

#include "opengl.inl"

#include "CString.hpp"
#include "asrtbas.h"
#include "memory.h"
#include "CArray.hpp"
#include "CArrayString.hpp"
#include "CFileName.hpp"
#include "filetxt.hpp"

static const GLuint PRV_SHADER_NULL = 0;

struct SPrvShader
{
    ~SPrvShader();

    char *idModule;

    GLenum shaderType;
    GLuint idShader;
};

struct SPrvDataPrivateShaderGL
{
    char *idProgram;

    GLuint idProgramShader;
    bool isLinked;
    bool isExecuted;

    class CArray<SPrvShader> *shaders;
};

//-----------------------------------------------------------------------

SPrvShader::~SPrvShader()
{
    CString::free(&idModule);
}

//-----------------------------------------------------------------------

static struct SPrvShader *prv_createPrvShader(const char *idModule, GLenum shaderType, GLuint idShader)
{
    struct SPrvShader *shader;

    shader = new SPrvShader;

    shader->idModule = CString::copy(idModule);
    shader->shaderType = shaderType;
    shader->idShader = idShader;

    return shader;
}

// ---------------------------------------------------------------------------

static struct SPrvShader *prv_createShaderWithLineSource(const char *idModule, GLenum shaderType, const char *shaderCode)
{
    GLuint idShader;
    const char *shaderMultipleCodes[1];

    idShader = glCreateShader(shaderType);

    shaderMultipleCodes[0] = shaderCode;
    glShaderSource(idShader, 1, shaderMultipleCodes, NULL);

    return prv_createPrvShader(idModule, shaderType, idShader);
}

// ---------------------------------------------------------------------------

static struct SPrvShader *prv_createShaderArrayTextCode(const char *idModule, GLenum shaderType, const CArrayString *code)
{
    GLuint idShader;
    char **strCode;
    unsigned long numStrings;

    assert_no_null(code);

    strCode = code->toArrayC(&numStrings);

    idShader = glCreateShader(shaderType);
    assert(idShader != PRV_SHADER_NULL);

    glShaderSource(idShader, numStrings, (const char **)strCode, NULL);

    CString::deleteArrayStrings(&strCode, numStrings);

    return prv_createPrvShader(idModule, shaderType, idShader);
}


//-----------------------------------------------------------------------

static void prv_integriyShaderGL(const struct SPrvDataPrivateShaderGL *dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(dataPrivate->idProgram);
    assert_no_null(dataPrivate->shaders);
}

//-----------------------------------------------------------------------

static struct SPrvDataPrivateShaderGL *prv_createShaderGL(
                    const char *idProgram,
                    GLuint idProgramShader,
                    bool isLinked,
                    bool isExecuted,
                    class CArray<SPrvShader> **shaders)
{
    struct SPrvDataPrivateShaderGL *dataPrivate;

    dataPrivate = new SPrvDataPrivateShaderGL;

    dataPrivate->idProgram = CString::copy(idProgram);
    dataPrivate->idProgramShader = idProgramShader;
    dataPrivate->isLinked = isLinked;
    dataPrivate->isExecuted = isExecuted;

    dataPrivate->shaders = ASSIGN_PP_NO_NULL(shaders, class CArray<SPrvShader>);

    return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroyShaderGL(struct SPrvDataPrivateShaderGL **dataPrivate)
{
    assert_no_null(dataPrivate);
    prv_integriyShaderGL(*dataPrivate);

    CString::free(&(*dataPrivate)->idProgram);

    if ((*dataPrivate)->idProgramShader != PRV_SHADER_NULL)
        glDeleteProgram((*dataPrivate)->idProgramShader);

    DELETE_OBJECT(&(*dataPrivate)->shaders, class CArray<SPrvShader>);

    DELETE_OBJECT(dataPrivate, struct SPrvDataPrivateShaderGL);
}

//-----------------------------------------------------------------------

CShaderGL::CShaderGL(const char *idProgram)
{
    class CArray<SPrvShader> *shaders;
    GLuint idProgramShader;
    bool isLinked, isExecuted;

    shaders = new CArray<SPrvShader>();
    idProgramShader = PRV_SHADER_NULL;
    isLinked = false;
    isExecuted = false;

    m_dataPrivate = prv_createShaderGL(idProgram, idProgramShader, isLinked, isExecuted, &shaders);
}

//-----------------------------------------------------------------------

CShaderGL::~CShaderGL()
{
    prv_destroyShaderGL(&m_dataPrivate);
}

//-----------------------------------------------------------------------

void CShaderGL::appendVertexShader(const char *idVertexShader, const char *codeVertexShader)
{
    struct SPrvShader *shader;

    prv_integriyShaderGL(m_dataPrivate);
    assert(m_dataPrivate->isLinked == false);

    shader = prv_createShaderWithLineSource(idVertexShader, GL_VERTEX_SHADER, codeVertexShader);
    m_dataPrivate->shaders->add(shader);
}

//-----------------------------------------------------------------------

void CShaderGL::appendFragmentShader(const char *idFragmedShader, const char *codeFragmentShader)
{
    struct SPrvShader *shader;

    prv_integriyShaderGL(m_dataPrivate);
    assert(m_dataPrivate->isLinked == false);

    shader = prv_createShaderWithLineSource(idFragmedShader, GL_FRAGMENT_SHADER, codeFragmentShader);
    m_dataPrivate->shaders->add(shader);
}

// ---------------------------------------------------------------------------

static GLenum prv_typeShaderByExtension(const char *nameFileShaderVSorFS)
{
    GLenum typeShader;
    char *extension;

    extension = CFileName::getExtension(nameFileShaderVSorFS);

    if (CString::equalWithoutMatchCase(extension, "vs") == true)
        typeShader = GL_VERTEX_SHADER;
    else
    {
        assert(CString::equalWithoutMatchCase(extension, "fs") == true);
        typeShader = GL_FRAGMENT_SHADER;
    }

    return typeShader;
}

// ---------------------------------------------------------------------------

void CShaderGL::appendFileShader(const char *nameFileShaderVSorFS)
{
    struct SPrvShader *shader;
    class CArrayString *txtShader;
    GLenum shaderType;

    prv_integriyShaderGL(m_dataPrivate);
    assert(m_dataPrivate->isLinked == false);

    txtShader = filetxt_readFileTxt(nameFileShaderVSorFS);

    shaderType = prv_typeShaderByExtension(nameFileShaderVSorFS);
    shader = prv_createShaderArrayTextCode(nameFileShaderVSorFS, shaderType, txtShader);

    m_dataPrivate->shaders->add(shader);

    DELETE_OBJECT(&txtShader, class CArrayString);
}

// ---------------------------------------------------------------------------

static char *prv_getMsgError(GLuint idShader)
{
    GLint errorLogLength, actualErrorLogLength;
    char *errorLogText;

    glGetShaderiv(idShader, GL_INFO_LOG_LENGTH, &errorLogLength);

    if (errorLogLength > 1)
    {
        errorLogText = CString::create(errorLogLength);
        glGetShaderInfoLog(idShader, errorLogLength, &actualErrorLogLength, errorLogText);
    }
    else
        errorLogText = CString::copy("");

    return errorLogText;
}

// ---------------------------------------------------------------------------

static bool prv_compileShader(struct SPrvShader *shader, char **msgError)
{
    bool isOk;
    GLint result;

    glCompileShader(shader->idShader);

    glGetShaderiv(shader->idShader, GL_COMPILE_STATUS, &result);

    if (result != 0)
        isOk = true;
    else
    {
        char *msgError_loc;

        msgError_loc = prv_getMsgError(shader->idShader);
        CString::assignIfNoNull(msgError, msgError_loc);
        isOk = false;
        CString::free(&msgError_loc);
    }

    return isOk;
}

//-----------------------------------------------------------------------

static bool prv_compileShaders(
                    GLuint id_program_shader,
                    const class CArray<SPrvShader> *shaders,
                    class CArrayString *msgsError)
{
    bool isCanLink;
    unsigned long numShaders;

    isCanLink = true;

    numShaders = shaders->size();

    for (unsigned long i = 0; i < numShaders; i++)
    {
        struct SPrvShader *shader;
        char *msgError;

        shader = shaders->get(i);
        assert_no_null(shader);

        msgError = NULL;
        if (prv_compileShader(shader, &msgError) == true)
            glAttachShader(id_program_shader, shader->idShader);
        else
        {
            msgsError->add(shader->idModule);
            msgsError->addDestroyingString(&msgError);
            isCanLink = false;
        }
    }

    return isCanLink;
}

//-----------------------------------------------------------------------

bool CShaderGL::compileAndLinkProgram(class CArrayString **msgsErrorOpt)
{
    bool isOk;
    class CArrayString *msgsError_loc;
    GLuint idProgramShader;

    prv_integriyShaderGL(m_dataPrivate);
    assert(m_dataPrivate->isLinked == false);
    assert(m_dataPrivate->idProgramShader == PRV_SHADER_NULL);

    idProgramShader = glCreateProgram();
    assert(idProgramShader != PRV_SHADER_NULL);

    msgsError_loc = new CArrayString();

    if (prv_compileShaders(idProgramShader, m_dataPrivate->shaders, msgsError_loc) == true)
    {
        GLint result;

        glLinkProgram(idProgramShader);

        glGetProgramiv(idProgramShader, GL_LINK_STATUS, &result);

        if (result == 0)
        {
            char *msgErrorLink;

            msgErrorLink = prv_getMsgError(idProgramShader);
            msgsError_loc->add(m_dataPrivate->idProgram);
            msgsError_loc->addDestroyingString(&msgErrorLink);
        }
    }

    if (msgsError_loc->size() > 0)
    {
        char *headError;

        headError = CString::printf("Errors program shader: %s", m_dataPrivate->idProgram);

        msgsError_loc->insert(0, headError);
        glDeleteProgram(idProgramShader);
        CString::free(&headError);

        if (msgsErrorOpt != NULL)
            *msgsErrorOpt = msgsError_loc;

        isOk = false;
    }
    else
    {
        m_dataPrivate->isLinked = true;
        m_dataPrivate->idProgramShader = idProgramShader;
        isOk = true;
        DELETE_OBJECT(&msgsError_loc, class CArrayString);
    }

    return isOk;
}

//-----------------------------------------------------------------------

void CShaderGL::run(void)
{
    prv_integriyShaderGL(m_dataPrivate);

    assert(m_dataPrivate->isLinked == true);
    assert(m_dataPrivate->isExecuted == false);

    glUseProgram(m_dataPrivate->idProgramShader);

    m_dataPrivate->isExecuted = true;
}

//-----------------------------------------------------------------------

void CShaderGL::stop(void)
{
    prv_integriyShaderGL(m_dataPrivate);

    assert(m_dataPrivate->isLinked == true);
    assert(m_dataPrivate->isExecuted == true);

    glUseProgram(0);

    m_dataPrivate->isExecuted = false;
}
