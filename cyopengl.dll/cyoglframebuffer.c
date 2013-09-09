// Definición de FBO.

#include "cyoglframebuffer.inl"
#include "cyoglframebuffer.win"

#include "cyogltextura.inl"
#include "cyoglshader.inl"

#include "cypespy.h"
#include "cyassert.h"
#include "win.h"
#include "windows.tlh"

static PFNGLGENFRAMEBUFFERSEXTPROC          s_glGenFramebuffersEXT = NULL;
static PFNGLDELETEFRAMEBUFFERSEXTPROC       s_glDeleteFramebuffersEXT = NULL;
static PFNGLBINDFRAMEBUFFEREXTPROC          s_glBindFramebufferEXT = NULL;
static PFNGLFRAMEBUFFERTEXTURE2DEXTPROC     s_glFramebufferTexture2DEXT = NULL;
static PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC   s_glCheckFramebufferStatusEXT = NULL;
static PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC  s_glFramebufferRenderbufferEXT = NULL;

static PFNGLGENRENDERBUFFERSEXTPROC s_glGenRenderbuffersEXT = NULL;
static PFNGLDELETERENDERBUFFERSEXTPROC s_glDeleteRenderbuffersEXT = NULL;
static PFNGLBINDRENDERBUFFEREXTPROC s_glBindRenderbufferEXT = NULL;
static PFNGLRENDERBUFFERSTORAGEEXTPROC s_glRenderbufferStorageEXT = NULL;

enum i_tipo_buffer_t
{
    i_BUFFER_PROFUNDIDAD,
    i_BUFFER_COLOR
};

struct cyoglframebuffer_t
{
    enum i_tipo_buffer_t tipo_buffer;

    GLuint id_textura;
    GLuint id_frame;
    GLuint id_render;

    CYBOOL activado;
};

// ---------------------------------------------------------------------------

static void glGenFramebuffersEXT(GLsizei n, GLuint *framebuffers)
{
    assert_no_null(s_glGenFramebuffersEXT);
    s_glGenFramebuffersEXT(n, framebuffers);
}

// ---------------------------------------------------------------------------

static void glDeleteFramebuffersEXT(GLsizei n, const GLuint *framebuffers)
{
    assert_no_null(s_glDeleteFramebuffersEXT);
    s_glDeleteFramebuffersEXT(n, framebuffers);
}

// ---------------------------------------------------------------------------

static void glBindFramebufferEXT(GLenum target, GLuint framebuffer)
{
    assert_no_null(s_glBindFramebufferEXT);
    s_glBindFramebufferEXT(target, framebuffer);
}

// ---------------------------------------------------------------------------

static void glFramebufferTexture2DEXT(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
    assert_no_null(s_glFramebufferTexture2DEXT);
    s_glFramebufferTexture2DEXT(target, attachment, textarget, texture, level);
}

// ---------------------------------------------------------------------------

static GLenum glCheckFramebufferStatusEXT(GLenum target)
{
    assert_no_null(s_glCheckFramebufferStatusEXT);
    return s_glCheckFramebufferStatusEXT(target);
}

// ---------------------------------------------------------------------------

static void glFramebufferRenderbufferEXT(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{
    assert_no_null(s_glFramebufferRenderbufferEXT);
    s_glFramebufferRenderbufferEXT(target, attachment, renderbuffertarget, renderbuffer);
}


// ---------------------------------------------------------------------------

static void glGenRenderbuffersEXT(GLsizei n, GLuint *renderbuffers)
{
    assert_no_null(s_glGenRenderbuffersEXT);
    s_glGenRenderbuffersEXT(n, renderbuffers);
}

// ---------------------------------------------------------------------------

static void glDeleteRenderbuffersEXT(GLsizei n, const GLuint *renderbuffers)
{
    assert_no_null(s_glDeleteRenderbuffersEXT);
    s_glDeleteRenderbuffersEXT(n, renderbuffers);
}

// ---------------------------------------------------------------------------

static void glBindRenderbufferEXT(GLenum target, GLuint renderbuffer)
{
    assert_no_null(s_glBindRenderbufferEXT);
    s_glBindRenderbufferEXT(target, renderbuffer);
}

// ---------------------------------------------------------------------------

static void glRenderbufferStorageEXT(GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
{
    assert_no_null(s_glRenderbufferStorageEXT);
    s_glRenderbufferStorageEXT(target, internalformat, width, height);
}

// ---------------------------------------------------------------------------

static CYBOOL i_soporta_fbo(void)
{
    if (s_glGenFramebuffersEXT == NULL)
        return FALSO;

    if (s_glDeleteFramebuffersEXT == NULL)
        return FALSO;

    if (s_glBindFramebufferEXT == NULL)
        return FALSO;

    if (s_glFramebufferTexture2DEXT == NULL)
        return FALSO;

    if (s_glCheckFramebufferStatusEXT == NULL)
        return FALSO;

    if (s_glFramebufferRenderbufferEXT == NULL)
        return FALSO;

    if (s_glGenRenderbuffersEXT == NULL)
        return FALSO;

    if (s_glDeleteRenderbuffersEXT == NULL)
        return FALSO;

    if (s_glBindRenderbufferEXT == NULL)
        return FALSO;

    if (s_glRenderbufferStorageEXT == NULL)
        return FALSO;

    return CIERTO;
}

// ---------------------------------------------------------------------------

CYBOOL cyoglframebuffer_soporta_FBO(void)
{
    static CYBOOL s_cargados_fbo = FALSO;
    static CYBOOL s_fbo_soportados = FALSO;

    if (s_cargados_fbo == FALSO)
    {
        s_glGenFramebuffersEXT		= (PFNGLGENFRAMEBUFFERSEXTPROC)wglGetProcAddress("glGenFramebuffersEXT");
        s_glDeleteFramebuffersEXT   = (PFNGLDELETEFRAMEBUFFERSEXTPROC)wglGetProcAddress("glDeleteFramebuffersEXT");
        s_glBindFramebufferEXT		= (PFNGLBINDFRAMEBUFFEREXTPROC)wglGetProcAddress("glBindFramebufferEXT");
        s_glFramebufferTexture2DEXT	= (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)wglGetProcAddress("glFramebufferTexture2DEXT");
        s_glCheckFramebufferStatusEXT = (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC)wglGetProcAddress("glCheckFramebufferStatusEXT");
        s_glFramebufferRenderbufferEXT = (PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC)wglGetProcAddress("glFramebufferRenderbufferEXT");

        s_glGenRenderbuffersEXT = (PFNGLGENRENDERBUFFERSEXTPROC)wglGetProcAddress("glGenRenderbuffersEXT");
        s_glDeleteRenderbuffersEXT = (PFNGLDELETERENDERBUFFERSEXTPROC)wglGetProcAddress("glDeleteRenderbuffersEXT");
        s_glBindRenderbufferEXT = (PFNGLBINDRENDERBUFFEREXTPROC)wglGetProcAddress("glBindRenderbufferEXT");
        s_glRenderbufferStorageEXT = (PFNGLRENDERBUFFERSTORAGEEXTPROC)wglGetProcAddress("glRenderbufferStorageEXT");

        s_fbo_soportados = i_soporta_fbo();
        s_cargados_fbo = CIERTO;
    }

    return s_fbo_soportados;
}

// ---------------------------------------------------------------------------

CONSTRUCTOR(static struct cyoglframebuffer_t *, i_crea, (
                        enum i_tipo_buffer_t tipo_buffer, 
                        GLuint id_textura, GLuint id_frame, GLuint id_render, 
                        CYBOOL activado))
{
    struct cyoglframebuffer_t *cyoglframebuffer;

    cyoglframebuffer = MALLOC(struct cyoglframebuffer_t);

    cyoglframebuffer->tipo_buffer = tipo_buffer;

    cyoglframebuffer->id_textura = id_textura;
    cyoglframebuffer->id_frame = id_frame;
    cyoglframebuffer->id_render = id_render;

    cyoglframebuffer->activado = activado;

    return cyoglframebuffer;
}

// ---------------------------------------------------------------------------

static void i_check_status(void)
{
    GLenum statusFBO;

	statusFBO = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    assert(statusFBO == GL_FRAMEBUFFER_COMPLETE_EXT);
}

// ---------------------------------------------------------------------------

struct cyoglframebuffer_t *cyoglframebuffer_crea_fbo_profundidad(unsigned long xsize, unsigned long ysize)
{
    GLuint id_textura, id_frame, id_render;
    CYBOOL activado;

    id_textura = cyogltextura_crea_textura_profundidad_shadow(xsize, ysize);
    id_frame = ULONG_MAX;
	glGenFramebuffersEXT(1, &id_frame);
    activado = FALSO;

    id_render = ULONG_MAX;

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, id_frame);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, id_textura, 0);

    i_check_status();

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

    return i_crea(i_BUFFER_PROFUNDIDAD, id_textura, id_frame, id_render, activado);
}

// ---------------------------------------------------------------------------

struct cyoglframebuffer_t *cyoglframebuffer_crea_fbo_color(unsigned long xsize, unsigned long ysize)
{
    GLuint id_textura, id_frame, id_render;
    CYBOOL activado;

    id_textura = cyogltextura_crea_textura_datos_resultado(xsize, ysize);

    id_frame = ULONG_MAX;
	glGenFramebuffersEXT(1, &id_frame);
    activado = FALSO;

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, id_frame);

    glGenRenderbuffersEXT(1, &id_render);
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, id_render);
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, (GLsizei)xsize, (GLsizei)ysize);

    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, id_render);

    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, id_textura, 0);

    glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    i_check_status();

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

    return i_crea(i_BUFFER_COLOR, id_textura, id_frame, id_render, activado);
}

// ---------------------------------------------------------------------------

void cyoglframebuffer_destruye(struct cyoglframebuffer_t **cyoglframebuffer)
{
    assert_no_null(cyoglframebuffer);
    assert_no_null(*cyoglframebuffer);
    assert((*cyoglframebuffer)->activado == FALSO);

    cyogltextura_destruye_textura(&(*cyoglframebuffer)->id_textura);
    glDeleteFramebuffersEXT(1, &(*cyoglframebuffer)->id_frame);

    if ((*cyoglframebuffer)->id_render != ULONG_MAX)
    {
        glDeleteRenderbuffersEXT(1, &(*cyoglframebuffer)->id_render);
        (*cyoglframebuffer)->id_render = ULONG_MAX;
    }

    FREE_PP(cyoglframebuffer, struct cyoglframebuffer_t);
}

// ---------------------------------------------------------------------------

static void i_activa_frame(unsigned int id_frame, CYBOOL *activado)
{
    assert_no_null(activado);

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, id_frame);
    *activado = CIERTO;
}

// ---------------------------------------------------------------------------

static void i_desactiva_frame(CYBOOL *activado)
{
    assert_no_null(activado);

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    *activado = FALSO;
}

// ---------------------------------------------------------------------------

void cyoglframebuffer_activa_frame(struct cyoglframebuffer_t *cyoglframebuffer)
{
    assert_no_null(cyoglframebuffer);
    assert(cyoglframebuffer->activado == FALSO);

    i_activa_frame(cyoglframebuffer->id_frame, &cyoglframebuffer->activado);
}

// ---------------------------------------------------------------------------

void cyoglframebuffer_desactiva_frame(struct cyoglframebuffer_t *cyoglframebuffer)
{
    assert_no_null(cyoglframebuffer);
    assert(cyoglframebuffer->activado == CIERTO);

    i_desactiva_frame(&cyoglframebuffer->activado);
}

// ---------------------------------------------------------------------------

void cyoglframebuffer_activa_frame_con_shader(struct cyoglframebuffer_t *cyoglframebuffer, struct cyoglshader_t *shader)
{
    assert_no_null(cyoglframebuffer);
    assert(cyoglframebuffer->activado == FALSO);

    i_activa_frame(cyoglframebuffer->id_frame, &cyoglframebuffer->activado);
    cyoglshader_ejecuta_shader(shader);
}

// ---------------------------------------------------------------------------

void cyoglframebuffer_desactiva_con_shader(struct cyoglframebuffer_t *cyoglframebuffer, struct cyoglshader_t *shader)
{
    assert_no_null(cyoglframebuffer);
    assert(cyoglframebuffer->activado == CIERTO);

    i_desactiva_frame(&cyoglframebuffer->activado);
    cyoglshader_parar_shader(shader);
}

// ---------------------------------------------------------------------------

void cyoglframebuffer_aplica_textura(const struct cyoglframebuffer_t *cyoglframebuffer, unsigned int unidad_textura)
{
    assert_no_null(cyoglframebuffer);
    cyogltextura_activa_textura(cyoglframebuffer->id_textura, unidad_textura);
}

// ---------------------------------------------------------------------------

void cyoglframebuffer_dibuja_framebuffer_debug(
                        const struct cyoglframebuffer_t *cyoglframebuffer, 
                        unsigned long ind_ventana,
                        long x_viewport, long y_viewport,
                        unsigned long ancho_viewport, unsigned long alto_viewport)
{
    assert_no_null(cyoglframebuffer);
    cyogltextura_dibujo_textura_profundidad_debug(cyoglframebuffer->id_textura, ind_ventana, x_viewport, y_viewport, ancho_viewport, alto_viewport);
}

// ---------------------------------------------------------------------------

void cyoglframebuffer_procesa_buffers(
                        const struct cyoglframebuffer_t *cyoglframebuffer_1, 
                        const struct cyoglframebuffer_t *cyoglframebuffer_2, 
                        struct cyoglframebuffer_t *cyoglframebuffer_resultado, 
                        struct cyoglshader_t *shader,
                        const char *nombre_textura_1, const char *nombre_textura_2,
                        unsigned long x_size_procesamiento, unsigned long y_size_procesamiento,
                        cypeoglframebuffer_FPtr_inicia_variables_shader func_inicia_variables_shader_opc)
{
    assert_no_null(cyoglframebuffer_1);
    assert_no_null(cyoglframebuffer_2);
    assert_no_null(cyoglframebuffer_resultado);
    assert(cyoglframebuffer_1->activado == FALSO);
    assert(cyoglframebuffer_2->activado == FALSO);
    assert(cyoglframebuffer_resultado->activado == FALSO);

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, cyoglframebuffer_resultado->id_frame);

    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0,(int) x_size_procesamiento, (int)y_size_procesamiento);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    cyoglshader_ejecuta_shader(shader);

    cyogltextura_activa_textura(cyoglframebuffer_1->id_textura, 0);
    cyogltextura_activa_textura(cyoglframebuffer_2->id_textura, 1);

    cyoglshader_set_variable_uniform_uint(shader, nombre_textura_1, 0);
    cyoglshader_set_variable_uniform_uint(shader, nombre_textura_2, 1);

    if (func_inicia_variables_shader_opc != NULL)
        func_inicia_variables_shader_opc(shader);

    glBegin(GL_QUADS);

        glTexCoord2d(0., 0.);
        glVertex2d(-1., -1.);

        glTexCoord2d(1., 0.);
        glVertex2d(1., -1.);

        glTexCoord2d(1., 1.);
        glVertex2d(1., 1.);

        glTexCoord2d(0., 1.);
        glVertex2d(-1., 1.);

    glEnd();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    cyogltextura_desactiva_textura(1);
    cyogltextura_desactiva_textura(0);

    cyoglshader_parar_shader(shader);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}
