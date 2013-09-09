//Definición de texturas.

#include "cyogltextura.inl"
#include "cyogltextura.win"

#include "cyassert.h"
#include "cypespy.h"
#include "cambtipo.h"

#include "windows.tlh"
#include "win.h"

static PFNGLACTIVETEXTUREPROC s_glActiveTexture = NULL;

// ---------------------------------------------------------------------------

static void glActiveTexture(GLenum texture)
{
    if (s_glActiveTexture != NULL)
        s_glActiveTexture(texture);
}

// ---------------------------------------------------------------------------

void cyogltextura_hardware_texturas(CYBOOL *con_mips_por_hardware, CYBOOL *con_varias_unidades_texturas) 
{
    const GLubyte *version;
    CYBOOL con_mips_por_hardware_loc;
    GLint units_textures;

    version = glGetString(GL_VERSION);

    if (version != NULL)
    {
        if (version[0] > '1')
            con_mips_por_hardware_loc = CIERTO;
        else
        {
            if (version[2] >= '4')
                con_mips_por_hardware_loc = CIERTO;
            else
                con_mips_por_hardware_loc = FALSO;
        }
    }
    else
        con_mips_por_hardware_loc = FALSO;

    glGetIntegerv(GL_MAX_TEXTURE_UNITS, &units_textures);
    s_glActiveTexture = (PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");

    *con_mips_por_hardware = con_mips_por_hardware_loc;

    if (units_textures > 1 && s_glActiveTexture != NULL)
        *con_varias_unidades_texturas = CIERTO;
    else
        *con_varias_unidades_texturas = FALSO;
}

// ---------------------------------------------------------------------------

unsigned int cyogltextura_crea_textura_profundidad_shadow(unsigned long ancho_sombra, unsigned long alto_sombra)
{
    unsigned int id_textura_profundidad;
    GLfloat border_shadow[4] = {1.f, 1.f, 1.f, 1.f};

    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &id_textura_profundidad);
    glBindTexture(GL_TEXTURE_2D, id_textura_profundidad);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_shadow);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY); 

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, (int)ancho_sombra, (int)alto_sombra, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    return id_textura_profundidad;
}

// ---------------------------------------------------------------------------

unsigned int cyogltextura_crea_textura_datos_resultado(unsigned long ancho_sombra, unsigned long alto_sombra)
{
    unsigned int id_textura_profundidad;
    GLfloat border_shadow[4] = {1.f, 1.f, 1.f, 1.f};

    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &id_textura_profundidad);
    glBindTexture(GL_TEXTURE_2D, id_textura_profundidad);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_shadow);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (int)ancho_sombra, (int)alto_sombra, 0, GL_RGB, GL_FLOAT, 0);

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    return id_textura_profundidad;
}

// ----------------------------------------------------------------------------

static void i_carga_material(CYBOOL con_mips_por_hardware, const unsigned char *bytes_image_rgb, unsigned long xsize, unsigned long ysize)
{
    int xsize_opengl, ysize_opengl;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    xsize_opengl = cambtipo_unsigned_long_a_int(xsize);
    ysize_opengl = cambtipo_unsigned_long_a_int(ysize);

    if (con_mips_por_hardware == CIERTO)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, xsize_opengl, ysize_opengl, 0, GL_BGR, GL_UNSIGNED_BYTE, bytes_image_rgb);
    }
    else
    {
        int ret;

        ret = gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, xsize_opengl, ysize_opengl, GL_BGR, GL_UNSIGNED_BYTE, bytes_image_rgb);
        assert(ret == 0);
    }

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

// ----------------------------------------------------------------------------

unsigned int cyogltextura_crea_textura_imagen_repetida(
                        CYBOOL con_mips_por_hardware, 
                        const unsigned char *bytes_image_rgb, 
                        unsigned long xsize, unsigned long ysize)
{
    unsigned int id_material_opengl;

    id_material_opengl = 0;
    glGenTextures(1, &id_material_opengl);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, id_material_opengl);
    i_carga_material(con_mips_por_hardware, bytes_image_rgb, xsize, ysize);
    assert(glIsTexture(id_material_opengl) == GL_TRUE);
    glDisable(GL_TEXTURE_2D);

    return id_material_opengl;
}

// ----------------------------------------------------------------------------

void cyogltextura_destruye_textura(unsigned int *id_textura)
{
    assert_no_null(id_textura);

    glDeleteTextures(1, id_textura);
    *id_textura = UINT_MAX;
}

// ----------------------------------------------------------------------------

void cyogltextura_destruye_varias_texturas(unsigned long num_texturas, unsigned int **id_texturas)
{
    GLsizei n_opengl;

    assert_no_null(id_texturas);
    assert_no_null(*id_texturas);

    n_opengl = cambtipo_unsigned_long_a_int(num_texturas);
    glDeleteTextures(n_opengl, *id_texturas);

    FREE_PP(id_texturas, unsigned int);
}

// ----------------------------------------------------------------------------

void cyogltextura_activa_textura(unsigned int id_textura, unsigned int unidad_textura)
{
    glActiveTexture(GL_TEXTURE0 + unidad_textura);
    glBindTexture(GL_TEXTURE_2D, id_textura);
}


// ----------------------------------------------------------------------------

void cyogltextura_desactiva_textura(unsigned int unidad_textura)
{
    glActiveTexture(GL_TEXTURE0 + unidad_textura);
    glBindTexture(GL_TEXTURE_2D, 0);
}

// ----------------------------------------------------------------------------

void cyogltextura_dibujo_textura_profundidad_debug(
                        unsigned int id_textura_profundidad,
                        unsigned long ind_ventana,
                        long x_viewport, long y_viewport,
                        unsigned long ancho_viewport, unsigned long alto_viewport)
{
    long dx, sizeX, sizeY;

    sizeX = cambtipo_unsigned_long_a_int(ancho_viewport / 4); 
    sizeY = cambtipo_unsigned_long_a_int(alto_viewport / 4);
    dx = cambtipo_unsigned_long_a_int(ancho_viewport) - (long)ind_ventana * sizeX;

    glPushAttrib(GL_VIEWPORT_BIT);
    glViewport(x_viewport + dx, y_viewport, sizeX, sizeY);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, id_textura_profundidad);

    glEnable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);

    glColor4f(1.f,1.f,1.f,1.f);

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

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glPopAttrib();
}
