// Gestión de shaders de opengl.

#include "cyoglshader.inl"
#include "cyoglshader.win"

#include "cyassert.h"
#include "cypespy.h"
#include "cypestr.h"
#include "cypedir.h"
#include "cypeftxt.h"
#include "arraystr.h"
#include "a_punter.h"
#include "a_cadena.h"
#include "cambtipo.h"

static const GLhandleARB I_HANDLE_ARB_NULL = 0;

static PFNGLCREATEPROGRAMOBJECTARBPROC  s_glCreateProgramObjectARB = NULL;
static PFNGLUSEPROGRAMOBJECTARBPROC     s_glUseProgramObjectARB = NULL;
static PFNGLCREATESHADEROBJECTARBPROC   s_glCreateShaderObjectARB = NULL;
static PFNGLDELETEOBJECTARBPROC         s_glDeleteObjectARB = NULL;
static PFNGLSHADERSOURCEARBPROC         s_glShaderSourceARB = NULL;
static PFNGLCOMPILESHADERARBPROC        s_glCompileShaderARB = NULL;
static PFNGLGETOBJECTPARAMETERIVARBPROC s_glGetObjectParameterivARB = NULL;
static PFNGLATTACHOBJECTARBPROC         s_glAttachObjectARB = NULL;
static PFNGLLINKPROGRAMARBPROC          s_glLinkProgramARB = NULL;
static PFNGLGETUNIFORMLOCATIONARBPROC   s_glGetUniformLocationARB = NULL;
static PFNGLUNIFORM1IARBPROC            s_glUniform1iARB = NULL;
static PFNGLUNIFORMMATRIX4FVPROC        s_glUniformMatrix4fvARB = NULL;
static PFNGLUNIFORM1FARBPROC            s_glUniform1fARB = NULL;
static PFNGLGETINFOLOGARBPROC           s_glGetInfoLogARB = NULL;
static PFNGLGETACTIVEUNIFORMARBPROC     s_glGetActiveUniformARB = NULL;
struct i_handler_shader_t
{
    GLhandleARB handle_shader_opengl;
};

struct i_variable_t
{
    char *nombre_variable;
    GLint id_variable_shader;
};

struct cyoglshader_t
{
    char *id_program;

    GLhandleARB id_program_shader;
    CYBOOL esta_linkada;
    CYBOOL esta_en_ejecucion;

    ArrEstructura(i_variable_t) *variables;
    ArrEstructura(i_handler_shader_t) *shaders;
};

// ---------------------------------------------------------------------------

static GLhandleARB glCreateProgramObjectARB(void)
{
    assert_no_null(s_glCreateProgramObjectARB);
    return s_glCreateProgramObjectARB();
}

// ---------------------------------------------------------------------------

static void glUseProgramObjectARB(GLhandleARB programObj)
{
    assert_no_null(s_glUseProgramObjectARB);
    s_glUseProgramObjectARB(programObj);
}

// ---------------------------------------------------------------------------

static GLhandleARB glCreateShaderObjectARB(GLenum shaderType)
{
    assert_no_null(s_glCreateShaderObjectARB);
    return s_glCreateShaderObjectARB(shaderType);
}

// ---------------------------------------------------------------------------

static void glDeleteShaderARB(GLhandleARB obj)
{
    assert_no_null(s_glDeleteObjectARB);
    s_glDeleteObjectARB(obj);
}

// ---------------------------------------------------------------------------

static void glDeleteProgramARB(GLhandleARB obj)
{
    assert_no_null(s_glDeleteObjectARB);
    s_glDeleteObjectARB(obj);
}

// ---------------------------------------------------------------------------

static void glShaderSourceARB(GLhandleARB shaderObj, GLsizei count, const GLcharARB* *string, const GLint *length)
{
    assert_no_null(s_glShaderSourceARB);
    s_glShaderSourceARB(shaderObj, count, string, length);
}

// ---------------------------------------------------------------------------

static void glCompileShaderARB(GLhandleARB shaderObj)
{
    assert_no_null(s_glCompileShaderARB);
    s_glCompileShaderARB(shaderObj);
}

// ---------------------------------------------------------------------------

static void glGetObjectParameterivARB(GLhandleARB obj, GLenum pname, GLint *params)
{
    assert_no_null(s_glGetObjectParameterivARB);
    s_glGetObjectParameterivARB(obj, pname, params);
}

// ---------------------------------------------------------------------------

static void glAttachObjectARB(GLhandleARB containerObj, GLhandleARB obj)
{
    assert_no_null(s_glAttachObjectARB);
    s_glAttachObjectARB(containerObj, obj);
}

// ---------------------------------------------------------------------------

static void glLinkProgramARB(GLhandleARB programObj)
{
    assert_no_null(s_glLinkProgramARB);
    s_glLinkProgramARB(programObj);
}

// ---------------------------------------------------------------------------

static GLint glGetUniformLocationARB(GLhandleARB programObj, const GLcharARB *name)
{
    assert_no_null(s_glGetUniformLocationARB);
    return s_glGetUniformLocationARB(programObj, name);
}

// ---------------------------------------------------------------------------

static void glUniform1iARB(GLint location, GLint v0)
{
    assert_no_null(s_glUniform1iARB);
    s_glUniform1iARB(location, v0);
}

// ---------------------------------------------------------------------------

static void glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    assert_no_null(s_glUniformMatrix4fvARB);
    s_glUniformMatrix4fvARB(location, count, transpose, value);
}

// ---------------------------------------------------------------------------

static void glUniform1fARB(GLint location, GLfloat v0)
{
    assert_no_null(s_glUniform1fARB);
    s_glUniform1fARB(location, v0);
}

// ---------------------------------------------------------------------------

static void glGetInfoLogARB(GLhandleARB obj, GLsizei maxLength, GLsizei *length, GLcharARB *infoLog)
{
    assert_no_null(s_glGetInfoLogARB);
    s_glGetInfoLogARB(obj, maxLength, length, infoLog);
}

// ---------------------------------------------------------------------------

static void glGetActiveUniformARB(GLhandleARB programObj, GLuint index, GLsizei maxLength, GLsizei *length, GLint *size, GLenum *type, GLcharARB *name)
{
    assert_no_null(s_glGetActiveUniformARB);
    s_glGetActiveUniformARB(programObj, index, maxLength, length, size, type, name);
}

// ---------------------------------------------------------------------------

static CYBOOL i_soporta_shaders(void)
{
    if (s_glCreateProgramObjectARB == NULL)
        return FALSO;

    if (s_glUseProgramObjectARB == NULL)
        return FALSO;

    if (s_glCreateShaderObjectARB == NULL)
        return FALSO;

    if (s_glDeleteObjectARB == NULL)
        return FALSO;

    if (s_glShaderSourceARB == NULL)
        return FALSO;

    if (s_glCompileShaderARB == NULL)
        return FALSO;

    if (s_glGetObjectParameterivARB == NULL)
        return FALSO;

    if (s_glAttachObjectARB == NULL)
        return FALSO;

    if (s_glLinkProgramARB == NULL)
        return FALSO;

    if (s_glGetUniformLocationARB == NULL)
        return FALSO;

    if (s_glUniform1iARB == NULL)
        return FALSO;

    if (s_glUniformMatrix4fvARB == NULL)
        return FALSO;

    if (s_glUniform1fARB == NULL)
        return FALSO;

    if (s_glGetInfoLogARB == NULL)
        return FALSO;

    if (s_glGetActiveUniformARB == NULL)
        return FALSO;

    return CIERTO;
}

// ---------------------------------------------------------------------------

CYBOOL cyoglshader_soporta_shaders(void)
{
    static CYBOOL s_cargados_shaders = FALSO;
    static CYBOOL s_shaders_soportados = FALSO;

    if (s_cargados_shaders == FALSO)
    {
	    s_glCreateProgramObjectARB = (PFNGLCREATEPROGRAMOBJECTARBPROC)wglGetProcAddress("glCreateProgramObjectARB");
        s_glDeleteObjectARB = (PFNGLDELETEOBJECTARBPROC)wglGetProcAddress("glDeleteObjectARB");
	    s_glUseProgramObjectARB = (PFNGLUSEPROGRAMOBJECTARBPROC)wglGetProcAddress("glUseProgramObjectARB");
	    s_glCreateShaderObjectARB = (PFNGLCREATESHADEROBJECTARBPROC)wglGetProcAddress("glCreateShaderObjectARB");
	    s_glShaderSourceARB = (PFNGLSHADERSOURCEARBPROC)wglGetProcAddress("glShaderSourceARB"); 
	    s_glCompileShaderARB = (PFNGLCOMPILESHADERARBPROC)wglGetProcAddress("glCompileShaderARB"); 
	    s_glGetObjectParameterivARB = (PFNGLGETOBJECTPARAMETERIVARBPROC)wglGetProcAddress("glGetObjectParameterivARB"); 
	    s_glAttachObjectARB = (PFNGLATTACHOBJECTARBPROC)wglGetProcAddress("glAttachObjectARB"); 
	    s_glLinkProgramARB = (PFNGLLINKPROGRAMARBPROC)wglGetProcAddress("glLinkProgramARB");
	    s_glGetUniformLocationARB = (PFNGLGETUNIFORMLOCATIONARBPROC)wglGetProcAddress("glGetUniformLocationARB");
	    s_glUniform1iARB = (PFNGLUNIFORM1IARBPROC)wglGetProcAddress("glUniform1iARB");
        s_glUniformMatrix4fvARB = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fvARB");
        s_glUniform1fARB = (PFNGLUNIFORM1FARBPROC)wglGetProcAddress("glUniform1fARB");
        s_glGetInfoLogARB = (PFNGLGETINFOLOGARBPROC)wglGetProcAddress("glGetInfoLogARB");
        s_glGetActiveUniformARB = (PFNGLGETACTIVEUNIFORMARBPROC)wglGetProcAddress("glGetActiveUniformARB");

        s_shaders_soportados = i_soporta_shaders();
        s_cargados_shaders = CIERTO;
    }

    return s_shaders_soportados;
}

// ---------------------------------------------------------------------------

CONSTRUCTOR(static struct i_handler_shader_t *, i_crea_handler_shader, (GLhandleARB handle_shader_opengl))
{
    struct i_handler_shader_t *handle_shader;

    handle_shader = MALLOC(struct i_handler_shader_t);

    handle_shader->handle_shader_opengl = handle_shader_opengl;

    return handle_shader;
}

// ---------------------------------------------------------------------------

CONSTRUCTOR(static struct i_variable_t *, i_crea_variable, (const char *nombre_variable, GLint id_variable_shader))
{
    struct i_variable_t *variable;

    variable = MALLOC(struct i_variable_t);

    variable->nombre_variable = cad_copia_cadena(nombre_variable);
    variable->id_variable_shader = id_variable_shader;

    return variable;
}

// ---------------------------------------------------------------------------

static void i_destruye_variable(struct i_variable_t **variable)
{
    assert_no_null(variable);
    assert_no_null(*variable);

    cypestr_destruye(&(*variable)->nombre_variable);

    FREE_PP(variable, struct i_variable_t);
}

// ---------------------------------------------------------------------------

static CYBOOL i_es_variable(const struct i_variable_t *variable, const char *nombre_variable)
{
    assert_no_null(variable);
    return cad_cadenas_iguales(variable->nombre_variable, nombre_variable);
}

// ---------------------------------------------------------------------------

static void i_destruye_handler_shader(struct i_handler_shader_t **handle_shader)
{
    assert_no_null(handle_shader);
    assert_no_null(*handle_shader);

    glDeleteShaderARB((*handle_shader)->handle_shader_opengl);

    FREE_PP(handle_shader, struct i_handler_shader_t);
}

// ---------------------------------------------------------------------------

CONSTRUCTOR(static struct cyoglshader_t *, i_crea, (
                        const char *id_program,
                        GLhandleARB id_program_shader,
                        CYBOOL esta_linkada,
                        CYBOOL esta_en_ejecucion,
                        ArrEstructura(i_variable_t) **variables,
                        ArrEstructura(i_handler_shader_t) **shaders))
{
    struct cyoglshader_t *cyoglshader;

    cyoglshader = MALLOC(struct cyoglshader_t);

    cyoglshader->id_program = cad_copia_cadena(id_program);

    cyoglshader->id_program_shader = id_program_shader;
    cyoglshader->esta_linkada = esta_linkada;
    cyoglshader->esta_en_ejecucion = esta_en_ejecucion;

    cyoglshader->variables = ASIGNA_PUNTERO_PP_NO_NULL(variables, ArrEstructura(i_variable_t));
    cyoglshader->shaders = ASIGNA_PUNTERO_PP_NO_NULL(shaders, ArrEstructura(i_handler_shader_t));

    return cyoglshader;
}

// ---------------------------------------------------------------------------

struct cyoglshader_t *cyoglshader_crea_defecto(const char *id_program)
{
    GLhandleARB id_program_shader;
    CYBOOL esta_linkada;
    CYBOOL esta_en_ejecucion;
    ArrEstructura(i_variable_t) *variables;
    ArrEstructura(i_handler_shader_t) *shaders;

    id_program_shader = I_HANDLE_ARB_NULL;
    esta_linkada = FALSO;
    esta_en_ejecucion = FALSO;
    variables = arr_CreaPunteroST(0, i_variable_t);
    shaders = arr_CreaPunteroST(0, i_handler_shader_t);

    return i_crea(id_program, id_program_shader, esta_linkada, esta_en_ejecucion, &variables, &shaders);
}

// ---------------------------------------------------------------------------

void cyoglshader_destruye(struct cyoglshader_t **cyoglshader)
{
    assert_no_null(cyoglshader);
    assert_no_null(*cyoglshader);
    assert((*cyoglshader)->esta_en_ejecucion == FALSO);

    cypestr_destruye(&(*cyoglshader)->id_program);

    if ((*cyoglshader)->esta_linkada == CIERTO)
        glDeleteProgramARB((*cyoglshader)->id_program_shader);
    else
        assert((*cyoglshader)->id_program_shader == I_HANDLE_ARB_NULL);

    arr_DestruyeEstructurasST(&(*cyoglshader)->shaders, i_handler_shader_t, i_destruye_handler_shader);
    arr_DestruyeEstructurasST(&(*cyoglshader)->variables, i_variable_t, i_destruye_variable);

    FREE_PP(cyoglshader, struct cyoglshader_t);
}

// ---------------------------------------------------------------------------

CONSTRUCTOR(static char *, i_get_error, (GLhandleARB handle))
{
    GLint errorLoglength, actualErrorLogLength;
    char *errorLogText;
	
	glGetObjectParameterivARB(handle, GL_OBJECT_INFO_LOG_LENGTH_ARB, &errorLoglength);

    if (errorLoglength > 0)
    {
        errorLogText = CALLOC(errorLoglength, char);
		    
        glGetInfoLogARB(handle, errorLoglength, &actualErrorLogLength, errorLogText);
    }
    else
        errorLogText = cad_copia_cadena("");

    return errorLogText;
}

// ---------------------------------------------------------------------------

static CYBOOL i_compile_shader(GLhandleARB handle, char **mensaje_error)
{
    CYBOOL es_ok;
    GLint result;
    char *mensaje_error_loc;

    glCompileShaderARB(handle);

    glGetObjectParameterivARB(handle, GL_OBJECT_COMPILE_STATUS_ARB, &result);
    mensaje_error_loc = i_get_error(handle);

    if (result != 0)
        es_ok = CIERTO;
    else
        es_ok = FALSO;

    cypestr_copia_si_no_null(mensaje_error, mensaje_error_loc);

    cypestr_destruye(&mensaje_error_loc);

    return es_ok;
}

// ---------------------------------------------------------------------------

static GLhandleARB i_create_shader(GLenum shaderType, const char *shaderCode)
{
    GLhandleARB handle;
    const GLcharARB *shaderMultipleCodes[1];

	handle = glCreateShaderObjectARB(shaderType);

    shaderMultipleCodes[0] = (const GLcharARB *)shaderCode;
    glShaderSourceARB(handle, 1, shaderMultipleCodes, NULL);

    return handle;
}

// ---------------------------------------------------------------------------

static void i_append_type_shader(
                        GLenum shaderType,
                        const char *shader_code,
                        ArrEstructura(i_handler_shader_t) *shaders)
{
    GLhandleARB handle_shader_opengl;
    struct i_handler_shader_t *shader;

    handle_shader_opengl = i_create_shader(shaderType, shader_code);
    shader = i_crea_handler_shader(handle_shader_opengl);
    arr_AppendPunteroST(shaders, shader, i_handler_shader_t);
}

// ---------------------------------------------------------------------------

void cyoglshader_append_vertex_shader(struct cyoglshader_t *cyoglshader, const char *shader_code)
{
    assert_no_null(cyoglshader);
    i_append_type_shader(GL_VERTEX_SHADER, shader_code, cyoglshader->shaders);
}

// ---------------------------------------------------------------------------

void cyoglshader_append_fragment_shader(struct cyoglshader_t *cyoglshader, const char *shader_code)
{
    assert_no_null(cyoglshader);
    i_append_type_shader(GL_FRAGMENT_SHADER, shader_code, cyoglshader->shaders);
}

// ---------------------------------------------------------------------------

static void i_log_uniform(GLuint id_program_shader)
{
    GLint i, num_variables_uniform;
    GLcharARB nombre_variable[512];

    num_variables_uniform = 0;
    glGetObjectParameterivARB(id_program_shader, GL_OBJECT_ACTIVE_UNIFORMS_ARB, &num_variables_uniform);

    for (i = 0; i < num_variables_uniform; i++)
    {
        GLenum type;
        GLint size;

        nombre_variable[0] = '\0';
        glGetActiveUniformARB(id_program_shader, (GLuint)i, 512, NULL, &size, &type, nombre_variable);
    }
}

// ---------------------------------------------------------------------------

CYBOOL cyoglshader_compila_y_linka(struct cyoglshader_t *cyoglshader, char **mensaje_error)
{
	GLint result;
    unsigned long i, num;
    CYBOOL se_puede_linkar;
    static CYBOOL s_debug = FALSO;

    assert_no_null(cyoglshader);
    assert(cyoglshader->esta_linkada == FALSO);
    assert(cyoglshader->id_program_shader == I_HANDLE_ARB_NULL);

	cyoglshader->id_program_shader = glCreateProgramObjectARB();

    num = arr_NumElemsPunteroST(cyoglshader->shaders, i_handler_shader_t);

    se_puede_linkar = CIERTO;

    for (i = 0; i < num && se_puede_linkar == CIERTO; i++)
    {
        const struct i_handler_shader_t *handle_shader;
        char *mensaje_error_loc;

        handle_shader = arr_GetPunteroConstST(cyoglshader->shaders, i, i_handler_shader_t);
        assert_no_null(handle_shader);

        if (i_compile_shader(handle_shader->handle_shader_opengl, &mensaje_error_loc) == CIERTO)
    	    glAttachObjectARB(cyoglshader->id_program_shader, handle_shader->handle_shader_opengl);
        else
        {
            se_puede_linkar = FALSO;
            cypestr_copia_si_no_null(mensaje_error, mensaje_error_loc);
        }

        cypestr_destruye(&mensaje_error_loc);
    }

    if (se_puede_linkar == CIERTO)
    {
	    glLinkProgramARB(cyoglshader->id_program_shader);

        glGetObjectParameterivARB(cyoglshader->id_program_shader, GL_OBJECT_LINK_STATUS_ARB, &result);

        if (result == 0)
        {
            se_puede_linkar = FALSO;
            if (mensaje_error != NULL)
                *mensaje_error = i_get_error(cyoglshader->id_program_shader);
        }
    }

    if (s_debug == CIERTO)
        i_log_uniform(cyoglshader->id_program_shader);

    cyoglshader->esta_linkada = CIERTO;

    return se_puede_linkar;
}

// ---------------------------------------------------------------------------

void cyoglshader_append_variable_uniform(struct cyoglshader_t *cyoglshader, const char *nombre_variable)
{
    struct i_variable_t *variable;
    GLint id_variable_shader;

    assert_no_null(cyoglshader);
    assert(cyoglshader->esta_linkada == CIERTO);

    id_variable_shader = glGetUniformLocationARB(cyoglshader->id_program_shader, nombre_variable);
    assert_msg(id_variable_shader >= 0, nombre_variable);

    variable = i_crea_variable(nombre_variable, id_variable_shader);
    arr_AppendPunteroST(cyoglshader->variables, variable, i_variable_t);
}

// ---------------------------------------------------------------------------

CYBOOL cyoglshader_se_puede_definir_variable(struct cyoglshader_t *cyoglshader, const char *nombre_variable)
{
    GLint id_variable_shader;

    assert_no_null(cyoglshader);
    assert(cyoglshader->esta_linkada == CIERTO);

    id_variable_shader = glGetUniformLocationARB(cyoglshader->id_program_shader, nombre_variable);

    if (id_variable_shader >= 0)
        return CIERTO;
    else
        return FALSO;
}

// ---------------------------------------------------------------------------

void cyoglshader_ejecuta_shader(struct cyoglshader_t *cyoglshader)
{
    assert_no_null(cyoglshader);
    assert(cyoglshader->esta_linkada == CIERTO);
    assert(cyoglshader->esta_en_ejecucion == FALSO);

    glUseProgramObjectARB(cyoglshader->id_program_shader);

    cyoglshader->esta_en_ejecucion = CIERTO;
}

// ---------------------------------------------------------------------------

void cyoglshader_parar_shader(struct cyoglshader_t *cyoglshader)
{
    assert_no_null(cyoglshader);
    assert(cyoglshader->esta_linkada == CIERTO);
    assert(cyoglshader->esta_en_ejecucion == CIERTO);

    glUseProgramObjectARB(0);

    cyoglshader->esta_en_ejecucion = FALSO;
}

// ---------------------------------------------------------------------------

void cyoglshader_set_variable_uniform_int(struct cyoglshader_t *cyoglshader, const char *nombre_variable, int valor)
{
    struct i_variable_t *variable;

    assert_no_null(cyoglshader);
    assert(cyoglshader->esta_linkada == CIERTO);
    assert(cyoglshader->esta_en_ejecucion == CIERTO);

    variable = arr_DameUnicaEstructuraST(cyoglshader->variables, i_variable_t, nombre_variable, char, i_es_variable);
    glUniform1iARB(variable->id_variable_shader, valor);
}

// ---------------------------------------------------------------------------

void cyoglshader_set_variable_uniform_uint(struct cyoglshader_t *cyoglshader, const char *nombre_variable, unsigned int valor)
{
    struct i_variable_t *variable;
    int valor_int;

    assert_no_null(cyoglshader);
    assert(cyoglshader->esta_linkada == CIERTO);
    assert(cyoglshader->esta_en_ejecucion == CIERTO);

    valor_int = cambtipo_unsigned_int_a_int(valor);

    variable = arr_DameUnicaEstructuraST(cyoglshader->variables, i_variable_t, nombre_variable, char, i_es_variable);
    glUniform1iARB(variable->id_variable_shader, valor_int);
}

// ---------------------------------------------------------------------------

void cyoglshader_set_variable_uniform_CYBOOL(struct cyoglshader_t *cyoglshader, const char *nombre_variable, CYBOOL valor)
{
    struct i_variable_t *variable;

    assert_no_null(cyoglshader);
    assert(cyoglshader->esta_linkada == CIERTO);
    assert(cyoglshader->esta_en_ejecucion == CIERTO);

    variable = arr_DameUnicaEstructuraST(cyoglshader->variables, i_variable_t, nombre_variable, char, i_es_variable);
    if (valor == CIERTO)
        glUniform1iARB(variable->id_variable_shader, 1);
    else
        glUniform1iARB(variable->id_variable_shader, 0);
}

// ---------------------------------------------------------------------------

void cyoglshader_set_variable_uniform_matriz4x4(struct cyoglshader_t *cyoglshader, const char *nombre_variable, const float matriz[16])
{
    struct i_variable_t *variable;

    assert_no_null(cyoglshader);
    assert(cyoglshader->esta_linkada == CIERTO);
    assert(cyoglshader->esta_en_ejecucion == CIERTO);

    variable = arr_DameUnicaEstructuraST(cyoglshader->variables, i_variable_t, nombre_variable, char, i_es_variable);
    glUniformMatrix4fv(variable->id_variable_shader, 1, GL_FALSE, matriz);
}

// ---------------------------------------------------------------------------

void cyoglshader_set_variable_uniform_float(struct cyoglshader_t *cyoglshader, const char *nombre_variable, float valor)
{
    struct i_variable_t *variable;

    assert_no_null(cyoglshader);
    assert(cyoglshader->esta_linkada == CIERTO);
    assert(cyoglshader->esta_en_ejecucion == CIERTO);

    variable = arr_DameUnicaEstructuraST(cyoglshader->variables, i_variable_t, nombre_variable, char, i_es_variable);
    glUniform1fARB(variable->id_variable_shader, valor);
}

// ---------------------------------------------------------------------------

static GLenum i_type_shader_segun_extension(const char *fichero_shader)
{
    GLenum type_shader;
    char *extension;

    extension = cdir_extension_fichero(fichero_shader);

    if (cad_cadenas_iguales_may_o_min(extension, "fsh") == CIERTO)
        type_shader = GL_FRAGMENT_SHADER;
    else
    {
        assert(cad_cadenas_iguales_may_o_min(extension, "vsh") == CIERTO);
        type_shader = GL_VERTEX_SHADER;
    }

    cypestr_destruye(&extension);

    return type_shader;
}

// ---------------------------------------------------------------------------

CONSTRUCTOR(static char *, i_lee_shader, (const char *fichero_shader))
{
    char *codigo_shader;
    ArrCadena *lineas_codigo_shader;

    lineas_codigo_shader = lc_LeeFicheroTxt_con_lineas_largas(fichero_shader);
    codigo_shader = arraystr_union_de_textos_separados_por_salto_de_linea(lineas_codigo_shader);
    arr_DestruyeCADENA(&lineas_codigo_shader);

    return codigo_shader;
}

// ---------------------------------------------------------------------------

CYBOOL cyoglshader_compila_codigo_shader(const char *fichero_shader, char **mensaje_error)
{
    CYBOOL se_ha_compilado;
    GLhandleARB handle_shader;
    GLenum shaderType;
    char *codigo_shader;

    shaderType = i_type_shader_segun_extension(fichero_shader);
    codigo_shader = i_lee_shader(fichero_shader);

	handle_shader = i_create_shader(shaderType, codigo_shader);

    se_ha_compilado = i_compile_shader(handle_shader, mensaje_error);

    cypestr_destruye(&codigo_shader);

    glDeleteShaderARB(handle_shader);

    return se_ha_compilado;
}
