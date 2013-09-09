// Renderización OpenGL

#include "cypegl3d.inl"
#include "cypegl3d.win"

#include "cplangl.tlh"
#include "cypeplan.tlh"

#include "cyoglframebuffer.inl"
#include "cyoglshader.inl"
#include "cyogltextura.inl"
#include "cyshadow.inl"

#include "idcolmat.h"
#include "idcolor.h"
#include "a_bool.h"
#include "a_ulong.h"
#include "a_pto3d.h"
#include "a_pto2d.h"
#include "a_punter.h"
#include "a_cadena.h"
#include "cambtipo.h"
#include "cginstal.h"
#include "cyassert.h"
#include "cypemath.h"
#include "cypespy.h"
#include "cypewin.h"
#include "ejes2d.h"
#include "enprueba.h"
#include "imagnwin.h"
#include "msidioma.h"
#include "proyecta.h"
#include "standarc.h"
#include "win.h"
#include "widestr.h"
#include "cypestr.h"
#include "apunorde.h"
#include "cypemesh.h"
#include "copiafor.h"
#include "windows.tlh"
#include "defmath.tlh"

#define i_GetAValue(rgb) ((BYTE)((rgb)>>24))

enum i_tipo_luz_ambiente_t
{
    i_LUZ_MATERIALES,
    i_LUZ_AMBIENTE,
    i_LUZ_SOMBRA
};

enum i_tipo_eje_t
{
    i_EJE_X,
    i_EJE_Y,
    i_EJE_Z
};

enum i_tipo_vista_t
{
    i_VISTA_NO_DEFINIDA,
    i_VISTA_ORTOGONAL,
    i_VISTA_PERSPECTIVA
};

struct i_datos_vista_perspectiva_t
{
    double angulo_foco_camara_grados;

    double x_camara, y_camara, z_camara;
    double x_centro_proyeccion, y_centro_proyeccion, z_plano_proyeccion;

    double dx_acumulados_por_avance, dy_acumulados_por_avance;
};

struct i_buffers_puntos_t
{
    ArrEnum(cplan_tipo_primitiva_t) *tipos_primitivas;
    ArrPuntero(ArrULong) *inds_todas_las_caras;

    GLfloat *array_puntos, *array_normales;
    GLfloat *array_texturas;
    GLboolean *array_es_borde;
    unsigned long num_inds_todas_las_caras;
    unsigned int **array_ind_todas_las_caras;
};

struct i_lista_dibujo_t
{
    char *id_lista;

    unsigned long id_lista_detallada;
    unsigned long id_lista_poco_detallada;
    unsigned long id_lista_placas_detallada;
    unsigned long id_lista_placas_poco_detallada;
};

#define NUM_MAXIMOS_PLANOS 6

struct i_font_opengl_t
{
    unsigned long num_letras;
    GLYPHMETRICSFLOAT *agmf;
    DWORD lista_de_letras;
};

struct i_datos_material_t
{
    char *id_material;

    ArrEnum(cplan_material_t) *materiales_asignados;

    unsigned int id_material_opengl;
};

struct i_tipo_dibujo_t
{
    CYBOOL con_texturas;
    enum cplangl_tipo_dibujo_t tipo_dibujo_actual;
    enum cplan_material_t material;
    double factor_transparencia;
    unsigned long color_rgba_actual;
};

struct i_manejador_opengl_t
{
    struct HWND__ *hwnd;
    CYBOOL se_debe_destruir_ventana;

    struct HDC__ *hdc_trabajo;

    struct HDC__ *hdc_destino;
    struct HGLRC__ *hrc;
    struct HPALETTE__ *hPal;
    struct HPALETTE__ *hOldPal;

    char *vendedor_opengl;
    char *version_opengl;

    CYBOOL con_soporte_sombras;
    CYBOOL con_mips_por_hardware;

    CYBOOL es_version_compatible;
    CYBOOL es_shader_compilable;
};

struct i_informe_t
{
    unsigned long num_puntos_con_detalle, num_puntos_sin_detalle;
};

struct cypegl3d_t
{
    struct i_manejador_opengl_t *manejador_opengl;

    unsigned long color_fondo_rgb;

    int ox_destino;
    int oy_destino;
    unsigned long ancho_destino;
    unsigned long alto_destino;

    struct i_font_opengl_t *font_normal;

    struct i_tipo_dibujo_t *tipo_dibujo_actual;
    ArrEstructura(i_tipo_dibujo_t) *pila_tipos_dibujo;

    unsigned long num_planos_adicionales_clipping_activos;

    ArrEstructura(i_datos_material_t) *materiales_cargados;

    struct cyshadow_t *shader_sombra;

    enum cplangl_tipo_lista_dibujo_t tipo_lista_dibujo;
    char *id_lista_activa;
    struct apunorde(i_lista_dibujo_t) *lista_dibujos;

    enum i_tipo_vista_t tipo_vista;
    struct i_datos_vista_perspectiva_t *datos_vista_perspectiva;

    struct i_informe_t informe;
};

static const double i_INFINITO = 1.e10;

static const int i_MERIDIANOS_CILINDRO = 10;
static const int i_PARALELOS_CILINDRO = 5;
static const int i_MERIDIANOS_CILINDRO_DETALLADO = 50;

static unsigned long i_UNIDAD_MATERIALES = 0;

// ----------------------------------------------------------------------------

CONSTRUCTOR(static struct i_buffers_puntos_t *, i_crea_buffers_puntos, (
                        ArrEnum(cplan_tipo_primitiva_t) **tipos_primitivas,
                        ArrPuntero(ArrULong) **inds_todas_las_caras,
                        GLfloat **array_puntos, 
                        GLfloat **array_normales,
                        GLfloat **array_texturas,
                        GLboolean **array_es_borde,
                        unsigned long num_inds_todas_las_caras,
                        unsigned int ***array_ind_todas_las_caras))
{
    struct i_buffers_puntos_t *buffers_puntos;

    buffers_puntos = MALLOC(struct i_buffers_puntos_t);

    buffers_puntos->tipos_primitivas = ASIGNA_PUNTERO_PP_NO_NULL(tipos_primitivas, ArrEnum(cplan_tipo_primitiva_t));
    buffers_puntos->inds_todas_las_caras = ASIGNA_PUNTERO_PP_NO_NULL(inds_todas_las_caras, ArrPuntero(ArrULong));

    buffers_puntos->array_puntos = ASIGNA_PUNTERO_PP_NO_NULL(array_puntos, GLfloat); 
    buffers_puntos->array_normales = ASIGNA_PUNTERO_PP_NO_NULL(array_normales, GLfloat); 
    buffers_puntos->array_texturas = ASIGNA_PUNTERO_PP(array_texturas, GLfloat);

    buffers_puntos->array_es_borde = ASIGNA_PUNTERO_PP_NO_NULL(array_es_borde, GLboolean);

    buffers_puntos->num_inds_todas_las_caras = num_inds_todas_las_caras;
    buffers_puntos->array_ind_todas_las_caras = ASIGNA_PUNTERO_PP_NO_NULL(array_ind_todas_las_caras, unsigned int *);

    return buffers_puntos;
}

// ----------------------------------------------------------------------------

static void i_destruye_array_caras(
                        unsigned long num_inds_todas_las_caras,
                        unsigned int ***array_ind_todas_las_caras)
{
    unsigned long i;
    unsigned int **array_ind_todas_las_caras_loc;

    array_ind_todas_las_caras_loc = *array_ind_todas_las_caras;

    for (i = 0; i < num_inds_todas_las_caras; i++)
        FREE_PP(&array_ind_todas_las_caras_loc[i], unsigned int);

    FREE_PP(array_ind_todas_las_caras, unsigned int *);
}

// ----------------------------------------------------------------------------

static void i_destruye_buffers_puntos(struct i_buffers_puntos_t **buffers_puntos)
{
    assert_no_null(buffers_puntos);
    assert_no_null(*buffers_puntos);

    arr_DestruyeEnum(&(*buffers_puntos)->tipos_primitivas, cplan_tipo_primitiva_t);
    arr_DestruyeEstructurasTD(&(*buffers_puntos)->inds_todas_las_caras, ArrULong, arr_DestruyeULong);

    FREE_PP(&(*buffers_puntos)->array_puntos, GLfloat);
    FREE_PP(&(*buffers_puntos)->array_normales, GLfloat);

    if ((*buffers_puntos)->array_texturas != NULL)
        FREE_PP(&(*buffers_puntos)->array_texturas, GLfloat);

    FREE_PP(&(*buffers_puntos)->array_es_borde, GLboolean);

    i_destruye_array_caras((*buffers_puntos)->num_inds_todas_las_caras, &(*buffers_puntos)->array_ind_todas_las_caras);

    FREE_PP(buffers_puntos, struct i_buffers_puntos_t);
}

// ----------------------------------------------------------------------------

static void i_dibuja_elementos(
                        const ArrEnum(cplan_tipo_primitiva_t) *tipos_primitivas,
                        const ArrPuntero(ArrULong) *inds_todas_las_caras,
                        unsigned long num_inds_todas_las_caras,
                        unsigned int **array_ind_todas_las_caras)
{
    unsigned long i;

    assert(num_inds_todas_las_caras == arr_NumElemsEnum(tipos_primitivas, cplan_tipo_primitiva_t));

    for (i = 0; i < num_inds_todas_las_caras; i++)
    {
        enum cplan_tipo_primitiva_t tipo_primitiva;
        GLsizei num_indices;
        const ArrULong *inds_caras;

        tipo_primitiva = arr_GetEnum(tipos_primitivas, cplan_tipo_primitiva_t, i);
        inds_caras = arr_GetPunteroTD(inds_todas_las_caras, i, ArrULong);
        num_indices = (GLsizei)arr_NumElemsULong(inds_caras);

        glDrawElements((unsigned short)tipo_primitiva, num_indices, GL_UNSIGNED_INT, array_ind_todas_las_caras[i]);
    }
}

// ----------------------------------------------------------------------------

static void i_dibuja_buffer(const struct i_buffers_puntos_t *buffer)
{
    assert_no_null(buffer);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_EDGE_FLAG_ARRAY);

    if (buffer->array_texturas != NULL)
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, buffer->array_puntos);

    if (buffer->array_texturas != NULL)
        glTexCoordPointer(2, GL_FLOAT, 0, buffer->array_texturas);

    glNormalPointer(GL_FLOAT, 0, buffer->array_normales);
    glEdgeFlagPointer(0, buffer->array_es_borde);

    i_dibuja_elementos(buffer->tipos_primitivas, buffer->inds_todas_las_caras, buffer->num_inds_todas_las_caras, buffer->array_ind_todas_las_caras);

    if (buffer->array_texturas != NULL)
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glDisableClientState(GL_EDGE_FLAG_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

// ----------------------------------------------------------------------------

CONSTRUCTOR(static struct i_datos_vista_perspectiva_t *, i_crea_vista_perspectiva, (
                        double angulo_foco_camara_grados,
                        double x_camara, double y_camara, double z_camara,
                        double x_centro_proyeccion, double y_centro_proyeccion, double z_plano_proyeccion,
                        double dx_acumulados_por_avance, double dy_acumulados_por_avance))
{
    struct i_datos_vista_perspectiva_t *vista_perspectiva;

    vista_perspectiva = MALLOC(struct i_datos_vista_perspectiva_t);

    vista_perspectiva->angulo_foco_camara_grados = angulo_foco_camara_grados;

    vista_perspectiva->x_camara = x_camara;
    vista_perspectiva->y_camara = y_camara;
    vista_perspectiva->z_camara = z_camara;

    vista_perspectiva->x_centro_proyeccion = x_centro_proyeccion;
    vista_perspectiva->y_centro_proyeccion = y_centro_proyeccion;
    vista_perspectiva->z_plano_proyeccion = z_plano_proyeccion;

    vista_perspectiva->dx_acumulados_por_avance = dx_acumulados_por_avance;
    vista_perspectiva->dy_acumulados_por_avance = dy_acumulados_por_avance;

    return vista_perspectiva;
}

// ----------------------------------------------------------------------------

CONSTRUCTOR(static struct i_datos_vista_perspectiva_t *, i_crea_vista_perspectiva_por_defecto, (void))
{
    double angulo_foco_camara_grados;
    double x_camara, y_camara, z_camara;
    double x_centro_proyeccion, y_centro_proyeccion, z_plano_proyeccion;
    double dx_acumulados_por_avance, dy_acumulados_por_avance;

    angulo_foco_camara_grados = 0.;

    x_camara = 0.; 
    y_camara = 0.; 
    z_camara = 0.;
    
    x_centro_proyeccion = 0.; 
    y_centro_proyeccion = 0.; 
    z_plano_proyeccion = 0.;
    
    dx_acumulados_por_avance = 0.; 
    dy_acumulados_por_avance = 0.;

    return i_crea_vista_perspectiva(
                        angulo_foco_camara_grados,
                        x_camara, y_camara, z_camara,
                        x_centro_proyeccion, y_centro_proyeccion, z_plano_proyeccion,
                        dx_acumulados_por_avance, dy_acumulados_por_avance);
}

// ----------------------------------------------------------------------------

static void i_destruye_vista_perspectiva(struct i_datos_vista_perspectiva_t **vista_perspectiva)
{
    FREE_PP(vista_perspectiva, struct i_datos_vista_perspectiva_t);
}

// ----------------------------------------------------------------------------

CONSTRUCTOR(static struct i_lista_dibujo_t *, i_crea_lista_dibujo, (
                        const char *id_lista,
                        unsigned long id_lista_detallada,
                        unsigned long id_lista_poco_detallada,
                        unsigned long id_lista_placas_detallada,
                        unsigned long id_lista_placas_poco_detallada))
{
    struct i_lista_dibujo_t *datos_dibujo;

    datos_dibujo = MALLOC(struct i_lista_dibujo_t);
    
    datos_dibujo->id_lista = cad_copia_cadena(id_lista);
    
    datos_dibujo->id_lista_detallada = id_lista_detallada;
    datos_dibujo->id_lista_poco_detallada = id_lista_poco_detallada;
    datos_dibujo->id_lista_placas_detallada = id_lista_placas_detallada;
    datos_dibujo->id_lista_placas_poco_detallada = id_lista_placas_poco_detallada;

    return datos_dibujo;
}

// ----------------------------------------------------------------------------

CONSTRUCTOR(static struct i_lista_dibujo_t *, i_crea_lista_dibujo_defecto, (const char *id_lista))
{
    unsigned long id_lista_detallada, id_lista_poco_detallada;
    unsigned long id_lista_placas_detallada, id_lista_placas_poco_detallada;

    id_lista_detallada = ULONG_MAX;
    id_lista_poco_detallada = ULONG_MAX;
    id_lista_placas_detallada = ULONG_MAX; 
    id_lista_placas_poco_detallada = ULONG_MAX;

    return i_crea_lista_dibujo(id_lista, id_lista_detallada, id_lista_poco_detallada, id_lista_placas_detallada, id_lista_placas_poco_detallada);
}

// ----------------------------------------------------------------------------

static void i_destruye_id_lista_dibujo(unsigned long id_lista_dibujo)
{
    if (glIsList(id_lista_dibujo) == GL_TRUE)
        glDeleteLists(id_lista_dibujo, 1);
}

// ----------------------------------------------------------------------------

static void i_destruye_lista_dibujo(struct i_lista_dibujo_t **lista_dibujo)
{
    assert_no_null(lista_dibujo);
    assert_no_null(*lista_dibujo);

    cypestr_destruye(&(*lista_dibujo)->id_lista);
    
    if ((*lista_dibujo)->id_lista_detallada != ULONG_MAX)
        i_destruye_id_lista_dibujo((*lista_dibujo)->id_lista_detallada);
        
    if ((*lista_dibujo)->id_lista_poco_detallada != ULONG_MAX)
        i_destruye_id_lista_dibujo((*lista_dibujo)->id_lista_poco_detallada);

    if ((*lista_dibujo)->id_lista_placas_detallada != ULONG_MAX)
        i_destruye_id_lista_dibujo((*lista_dibujo)->id_lista_placas_detallada);

    if ((*lista_dibujo)->id_lista_placas_poco_detallada != ULONG_MAX)
        i_destruye_id_lista_dibujo((*lista_dibujo)->id_lista_placas_poco_detallada);

    FREE_PP(lista_dibujo, struct i_lista_dibujo_t);
}

// ----------------------------------------------------------------------------

static enum comparac_t i_existe_lista_dibujo(const struct i_lista_dibujo_t *lista1,  const struct i_lista_dibujo_t *lista2)
{
    assert_no_null(lista1);
    assert_no_null(lista2);
    
    return cad_cadenas_compara(lista1->id_lista, lista2->id_lista);
}

// ----------------------------------------------------------------------------

static enum comparac_t i_busca_lista_dibujo(const struct i_lista_dibujo_t *lista,  const char *id_lista)
{
    assert_no_null(lista);
    
    return cad_cadenas_compara(lista->id_lista, id_lista);
}

// ----------------------------------------------------------------------------

static void i_activa_lista_opengl(struct i_lista_dibujo_t *lista_dibujo, enum cplangl_tipo_lista_dibujo_t tipo_lista_detalles)
{
    unsigned long id_lista_opengl;

    id_lista_opengl = glGenLists(1);
    glNewList(id_lista_opengl, GL_COMPILE);

    switch (tipo_lista_detalles)
    {
        case CPLANGL_LISTA_CON_MUCHO_DETALLE:

            assert(lista_dibujo->id_lista_detallada == ULONG_MAX);
            lista_dibujo->id_lista_detallada = id_lista_opengl;
            break;

        case CPLANGL_LISTA_CON_POCO_DETALLE:

            assert(lista_dibujo->id_lista_poco_detallada == ULONG_MAX);
            lista_dibujo->id_lista_poco_detallada = id_lista_opengl;
            break;

        case CPLANGL_LISTA_PLACAS_CON_MUCHO_DETALLE:

            assert(lista_dibujo->id_lista_placas_detallada == ULONG_MAX);
            lista_dibujo->id_lista_placas_detallada = id_lista_opengl;
            break;

        case CPLANGL_LISTA_PLACAS_CON_POCO_DETALLE:

            assert(lista_dibujo->id_lista_placas_poco_detallada == ULONG_MAX);
            lista_dibujo->id_lista_placas_poco_detallada = id_lista_opengl;
            break;

        default_error();
    }
}

// ----------------------------------------------------------------------------

CONSTRUCTOR(static struct i_tipo_dibujo_t *, i_crea_tipo_dibujo, (
                        CYBOOL con_texturas,
                        enum cplangl_tipo_dibujo_t tipo_dibujo_actual,
                        enum cplan_material_t material,
                        double factor_transparencia,
                        unsigned long color_rgba_actual))
{
    struct i_tipo_dibujo_t *tipo_dibujo;

    tipo_dibujo = MALLOC(struct i_tipo_dibujo_t);

    tipo_dibujo->con_texturas = con_texturas;
    tipo_dibujo->tipo_dibujo_actual = tipo_dibujo_actual;
    tipo_dibujo->material = material;
    tipo_dibujo->factor_transparencia = factor_transparencia;
    tipo_dibujo->color_rgba_actual = color_rgba_actual;

    return tipo_dibujo;
}

// ----------------------------------------------------------------------------

CONSTRUCTOR(static struct i_tipo_dibujo_t *, i_crea_tipo_dibujo_defecto, (
                        CYBOOL con_texturas,
                        enum cplangl_tipo_dibujo_t tipo_dibujo_actual,
                        unsigned char color_rgba))
{
    enum cplan_material_t material;
    double factor_transparencia;

    factor_transparencia = 1.;
    material = (enum cplan_material_t )ULONG_MAX;

    return i_crea_tipo_dibujo(con_texturas, tipo_dibujo_actual, material, factor_transparencia, color_rgba);
}

// ---------------------------------------------------------------------------

static void i_destruye_tipo_dibujo(struct i_tipo_dibujo_t **tipo_dibujo)
{
    FREE_PP(tipo_dibujo, struct i_tipo_dibujo_t);
}

// ----------------------------------------------------------------------------

static CYBOOL i_soporta_version_minima(const char *version, char **vendedor_opc, char **version_opengl_opc)
{
    const GLubyte *version_opengl;

    assert(cad_longitud_cadena(version) == 3);
    version_opengl = glGetString(GL_VERSION);

    if (version_opengl_opc != NULL)
        *version_opengl_opc = cad_copia_cadena((char *)version_opengl);

    if (vendedor_opc != NULL)
    {
        const GLubyte *vendedor, *render;

        vendedor = glGetString(GL_VENDOR);
        render = glGetString(GL_RENDERER);
        *vendedor_opc = copiafor_codigo2("%s - %s", vendedor, render);
    }

    if (version_opengl[0] > version[0])
    {
        return CIERTO;
    }
    else if (version_opengl[0] == version[0])
    {
        if (version_opengl[2] >= version[2])
            return CIERTO;
    }

    return FALSO;
}

// ----------------------------------------------------------------------------

static void i_datos_hardware_tarjeta(
                        CYBOOL *con_mips_por_hardware, 
                        CYBOOL *con_soporte_sombras,
                        char **vendedor_opengl_opc,
                        char **version_opengl_opc,
                        CYBOOL *es_version_compatible_opc,
                        CYBOOL *es_shader_compilable_opc)
{
    CYBOOL con_mips_por_hardware_loc, con_varias_unidades_texturas;
    CYBOOL es_version_compatible_loc, es_shader_compilable_loc;

    con_mips_por_hardware_loc = FALSO;
    con_varias_unidades_texturas = FALSO;
    es_version_compatible_loc = FALSO;
    es_shader_compilable_loc = FALSO;

    __try
    {
        assert_no_null(con_mips_por_hardware);
        assert_no_null(con_soporte_sombras);

        cyogltextura_hardware_texturas(&con_mips_por_hardware_loc, &con_varias_unidades_texturas);

        if (i_soporta_version_minima(
                            "2.1", 
                            vendedor_opengl_opc, version_opengl_opc) == CIERTO
                && con_varias_unidades_texturas == CIERTO
                && con_mips_por_hardware_loc == CIERTO)
        {
            assert(cyoglshader_soporta_shaders() == CIERTO);
            assert(cyoglframebuffer_soporta_FBO() == CIERTO);

            es_shader_compilable_loc = cyshadow_soporta_shaders_sombras();
            es_version_compatible_loc = CIERTO;
        }
        else
        {
            es_version_compatible_loc = FALSO;
            es_shader_compilable_loc = FALSO;
        }
    }
    __except(1)
    {
        if (vendedor_opengl_opc != NULL)
            *vendedor_opengl_opc = cad_copia_cadena("XXXX");

        if (version_opengl_opc != NULL)
            *version_opengl_opc = cad_copia_cadena("0.0");
    }

    if (es_version_compatible_opc != NULL)
        *es_version_compatible_opc = es_version_compatible_loc;

    if (es_shader_compilable_opc != NULL)
        *es_shader_compilable_opc = es_shader_compilable_loc;

    *con_mips_por_hardware = con_mips_por_hardware_loc;

    if (es_version_compatible_loc == CIERTO && es_shader_compilable_loc == CIERTO)
        *con_soporte_sombras = CIERTO;
    else
        *con_soporte_sombras = FALSO;
}

// ----------------------------------------------------------------------------

CONSTRUCTOR(static struct i_datos_material_t *, i_crea_datos_material, (
                        const char *id_material,
                        ArrEnum(cplan_material_t) **materiales_asignados,
                        unsigned int id_material_opengl))
{
    struct i_datos_material_t *datos_material;

    datos_material = MALLOC(struct i_datos_material_t);

    datos_material->id_material = cad_copia_cadena(id_material);
    datos_material->materiales_asignados = ASIGNA_PUNTERO_PP_NO_NULL(materiales_asignados, ArrEnum(cplan_material_t));
    datos_material->id_material_opengl = id_material_opengl;

    return datos_material;
}

// ----------------------------------------------------------------------------

CONSTRUCTOR(static struct i_datos_material_t *, i_crea_datos_material_de_imagen, (
                        CYBOOL con_mips_por_hardware,
                        const char *id_material, 
                        enum cplan_material_t material, 
                        const unsigned char *bytes_image_rgb, 
                        unsigned long xsize, unsigned long ysize))
{
    unsigned int id_material_opengl;
    ArrEnum(cplan_material_t) *materiales_asignados;

    id_material_opengl = cyogltextura_crea_textura_imagen_repetida(con_mips_por_hardware, bytes_image_rgb, xsize, ysize);

    materiales_asignados = arr_CreaEnum(1, cplan_material_t);
    arr_SetEnum(materiales_asignados, cplan_material_t, 0, material);

    return i_crea_datos_material(id_material, &materiales_asignados, id_material_opengl);
}

// ----------------------------------------------------------------------------

static void i_destruye_datos_material(struct i_datos_material_t **datos_material)
{
    assert_no_null(datos_material);
    assert_no_null(datos_material);
    assert((*datos_material)->id_material_opengl == UINT_MAX);

    cypestr_destruye(&(*datos_material)->id_material);
    arr_DestruyeEnum(&(*datos_material)->materiales_asignados, cplan_material_t);

    FREE_PP(datos_material, struct i_datos_material_t);
}

// ----------------------------------------------------------------------------

static CYBOOL i_es_datos_material(const struct i_datos_material_t *datos_material, const enum cplan_material_t *material)
{
    assert_no_null(datos_material);
    assert_no_null(material);

    return arr_ExisteUnicoEnum(datos_material->materiales_asignados, cplan_material_t, *material, NULL);
}

// ----------------------------------------------------------------------------

static CYBOOL i_es_id_material(const struct i_datos_material_t *datos_material, const char *id_material)
{
    return cad_cadenas_iguales(datos_material->id_material, id_material);
}

// ----------------------------------------------------------------------------

CONSTRUCTOR(static struct i_font_opengl_t *, i_crea_font_opengl, (unsigned long num_letras, GLYPHMETRICSFLOAT **agmf, DWORD lista_de_letras))
{
    struct i_font_opengl_t *font_opengl;

    font_opengl = MALLOC(struct i_font_opengl_t);

    font_opengl->num_letras = num_letras;
    font_opengl->agmf = ASIGNA_PUNTERO_PP_NO_NULL(agmf, GLYPHMETRICSFLOAT);
    font_opengl->lista_de_letras = lista_de_letras;

    return font_opengl;
}

// ----------------------------------------------------------------------------

static void i_destruye_font_opengl(struct i_font_opengl_t **font_opengl)
{
    assert_no_null(font_opengl);
    assert_no_null(*font_opengl);

    glDeleteLists((*font_opengl)->lista_de_letras, (GLsizei)(*font_opengl)->num_letras);
    FREE_PP(&(*font_opengl)->agmf, GLYPHMETRICSFLOAT);

    FREE_PP(font_opengl, struct i_font_opengl_t);
}

// ----------------------------------------------------------------------------

static struct i_font_opengl_t *i_lista_de_letras_para_opengl(struct HDC__ * hDC)
{
    DWORD lista_de_letras;
    HFONT hFont;
    HGDIOBJ hFontAnterior;
    GLYPHMETRICSFLOAT *agmf_loc;
    LOGFONT logfont;
    unsigned long num_letras_loc;

    logfont.lfHeight = -14;
    logfont.lfWidth = 0;
    logfont.lfEscapement = 0;
    logfont.lfOrientation = 0;
    logfont.lfWeight = FW_BOLD;
    logfont.lfItalic = FALSE;
    logfont.lfUnderline = FALSE;
    logfont.lfStrikeOut = FALSE;
    logfont.lfCharSet = msidioma_charset_para_fonts_windows_segun_idioma();
    logfont.lfOutPrecision = OUT_TT_ONLY_PRECIS;
    logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    logfont.lfQuality = DEFAULT_QUALITY;
    logfont.lfPitchAndFamily = DEFAULT_PITCH;

    strcpy(logfont.lfFaceName, "Arial");

    hFont = CreateFontIndirect(&logfont);
    hFontAnterior = SelectObject(hDC, hFont); 

    num_letras_loc = 256;
    agmf_loc = CALLOC(num_letras_loc, GLYPHMETRICSFLOAT);
    lista_de_letras = glGenLists((GLsizei)num_letras_loc);
    (void)wglUseFontOutlines(hDC, 0, num_letras_loc, lista_de_letras, 0.5f, 0.0f, WGL_FONT_POLYGONS, agmf_loc);

    SelectObject(hDC, hFontAnterior);
    DeleteObject(hFont);

    return i_crea_font_opengl(num_letras_loc, &agmf_loc, lista_de_letras);
}

// ----------------------------------------------------------------------------

static void i_asigna_puntos(const ArrPunto3D *puntos, GLfloat *array_vertices3D, unsigned long offset)
{
    unsigned long i, num;

    num = arr_NumElemsPunto3D(puntos);

    for (i = 0; i < num; i++)
    {
        unsigned long ind;
        double x, y, z;

        ind = i * 3 + offset;

        arr_GetPunto3D(puntos, i, &x, &y, &z);
        array_vertices3D[ind] = cambtipo_double_a_float(x);
        array_vertices3D[ind+1] = cambtipo_double_a_float(y);
        array_vertices3D[ind+2] = cambtipo_double_a_float(z);
    }
}

// ----------------------------------------------------------------------------

CONSTRUCTOR(static GLfloat *, i_crea_array_vertices, (const ArrPunto3D *puntos))
{
    GLfloat *array_vertices3D;
    unsigned long num;

    num = arr_NumElemsPunto3D(puntos);
    assert(num > 0);

    array_vertices3D = CALLOC(3 * num, GLfloat);
    i_asigna_puntos(puntos, array_vertices3D, 0);

    return array_vertices3D;
}

// ----------------------------------------------------------------------------

CONSTRUCTOR(static GLboolean *, i_crea_es_borde, (const ArrBool *es_borde))
{
    GLboolean *array_es_borde;
    unsigned long i, num;

    num = arr_NumElemsBOOL(es_borde);

    array_es_borde = CALLOC(num, GLboolean);

    for (i = 0; i < num; i++)
        array_es_borde[i] = (GLboolean)arr_GetBOOL(es_borde, i);

    return array_es_borde;
}

// ----------------------------------------------------------------------------

CONSTRUCTOR(static unsigned int *, i_crea_array_inds_caras, (unsigned long num_puntos, const ArrULong *inds_caras))
{
    unsigned int *array_inds_caras;
    unsigned long i, num;

    num = arr_NumElemsULong(inds_caras);

    array_inds_caras = CALLOC(num, unsigned int);

    for (i = 0; i < num; i++)
    {
        unsigned long ind;

        ind = arr_GetULong(inds_caras, i);
        assert(ind < num_puntos);

        array_inds_caras[i] = arr_GetULong(inds_caras, i);
    }

    return array_inds_caras;
}

// ----------------------------------------------------------------------------

CONSTRUCTOR(static unsigned int **, i_crea_array_inds_todas_caras, (
                        unsigned long num_puntos,
                        const ArrPuntero(ArrULong) *inds_todas_las_caras,
                        unsigned long *num_inds_todas_las_caras))
{
    unsigned int **array_inds_todas_las_caras_loc;
    unsigned long i, num_inds_todas_las_caras_loc;

    assert_no_null(num_inds_todas_las_caras);

    num_inds_todas_las_caras_loc = arr_NumElemsPunteroTD(inds_todas_las_caras, ArrULong);
    array_inds_todas_las_caras_loc = CALLOC(num_inds_todas_las_caras_loc, unsigned int *);

    for (i = 0; i < num_inds_todas_las_caras_loc; i++)
    {
        const ArrULong *inds_caras;

        inds_caras = arr_GetPunteroConstTD(inds_todas_las_caras, i, ArrULong);
        array_inds_todas_las_caras_loc[i] = i_crea_array_inds_caras(num_puntos, inds_caras);
    }

    *num_inds_todas_las_caras = num_inds_todas_las_caras_loc;

    return array_inds_todas_las_caras_loc;
}

// ----------------------------------------------------------------------------

CONSTRUCTOR(static GLfloat *, i_crea_array_texturas2D, (const ArrPunto2D *coord_texturas))
{
    unsigned long i, num_puntos;
    GLfloat *array_texturas_2D;

    num_puntos = arr_NumElemsPunto2D(coord_texturas);

    array_texturas_2D = CALLOC(num_puntos * 2, GLfloat);

    for (i = 0; i < num_puntos; i++)
    {
        unsigned long ind;
        double x, y;

        ind = i * 2;
        arr_GetPunto2D(coord_texturas, i, &x, &y);

        array_texturas_2D[ind] = cambtipo_double_a_float(x);
        array_texturas_2D[ind+1] = cambtipo_double_a_float(y);
    }

    return array_texturas_2D;
}

// ----------------------------------------------------------------------------

CONSTRUCTOR(static struct i_buffers_puntos_t *, i_crea_buffers_puntos_defecto, (
                        const ArrEnum(cplan_tipo_primitiva_t) *tipos_primitivas,
                        const ArrPuntero(ArrULong) *inds_todas_las_caras,
                        const ArrPunto3D *puntos,
                        const ArrPunto3D *normales,
                        const ArrPunto2D *coord_texturas_opc,
                        const ArrBool *es_borde,
                        unsigned long *num_puntos_dibujo))
{
    unsigned long num_puntos;
    ArrEnum(cplan_tipo_primitiva_t) *tipos_primitivas_buffer;
    ArrPuntero(ArrULong) *inds_todas_las_caras_buffer;
    GLfloat *array_puntos, *array_normales, *array_coord_textura;
    GLboolean *array_es_borde;
    unsigned long num_inds_todas_las_caras;
    unsigned int **array_ind_todas_las_caras;

    num_puntos = arr_NumElemsPunto3D(puntos);
    if (coord_texturas_opc != NULL)
        assert(num_puntos == arr_NumElemsPunto2D(coord_texturas_opc));

    array_puntos = i_crea_array_vertices(puntos);
    array_normales = i_crea_array_vertices(normales);

    if (num_puntos_dibujo != NULL)
        (*num_puntos_dibujo) += num_puntos;

    if (coord_texturas_opc != NULL)
        array_coord_textura = i_crea_array_texturas2D(coord_texturas_opc);
    else
        array_coord_textura = NULL;

    array_es_borde = i_crea_es_borde(es_borde);
    array_ind_todas_las_caras = i_crea_array_inds_todas_caras(num_puntos, inds_todas_las_caras, &num_inds_todas_las_caras);

    tipos_primitivas_buffer = arr_CopiaEnum(tipos_primitivas, cplan_tipo_primitiva_t);
    inds_todas_las_caras_buffer = arr_CopiaEstructurasTD(inds_todas_las_caras, ArrULong, arr_CopiaULong);

    return i_crea_buffers_puntos(
                        &tipos_primitivas_buffer, &inds_todas_las_caras_buffer,
                        &array_puntos, &array_normales, &array_coord_textura, &array_es_borde,
                        num_inds_todas_las_caras, &array_ind_todas_las_caras);
}

// ----------------------------------------------------------------------------

static void i_integridad(const struct cypegl3d_t *cypegl3d)
{
    assert_no_null(cypegl3d);
    assert_no_null(cypegl3d->tipo_dibujo_actual);
    assert_no_null(cypegl3d->manejador_opengl);
    assert_no_null(cypegl3d->manejador_opengl->hrc);
    assert_no_null(cypegl3d->manejador_opengl->hdc_trabajo);
    assert_no_null(cypegl3d->manejador_opengl->hdc_destino);
    assert_no_null(cypegl3d->font_normal);
    assert_no_null(cypegl3d->lista_dibujos);
}

// ----------------------------------------------------------------------------

static void i_make_current(struct i_manejador_opengl_t *manejador_opengl)
{
    assert_no_null(manejador_opengl);
    
    if (manejador_opengl->hPal != NULL)
    {
        (void)SelectPalette(manejador_opengl->hdc_trabajo, manejador_opengl->hPal, 0);
        (void)RealizePalette(manejador_opengl->hdc_trabajo);
    }

    if (manejador_opengl->hrc != wglGetCurrentContext())
    {
        BOOL res;

        res = wglMakeCurrent(manejador_opengl->hdc_trabajo, manejador_opengl->hrc);
        if (enprueba_esta_definido() == CIERTO)
            assert(res);
    }
}

// ----------------------------------------------------------------------------

static void i_inicializa_opengl(void)
{
    static CYBOOL opengl_inicializado = FALSO;

    if (opengl_inicializado == FALSO)
    {
        unsigned long handle;
        handle = cginstal_cargar_opengl();
        cginstal_valida_handle("opengl", handle);

        opengl_inicializado = CIERTO;
    }
}

// ----------------------------------------------------------------------------

CONSTRUCTOR(static struct i_manejador_opengl_t *, i_crea_manejador_opengl, (
                            struct HWND__ *hwnd,
                            CYBOOL se_debe_destruir_ventana,
                            struct HDC__ *hdc_trabajo,
                            struct HDC__ **hdc_destino,
                            struct HGLRC__ **hrc,
                            struct HPALETTE__ **hPal,
                            struct HPALETTE__ **hOldPal,
                            char **vendedor_opengl,
                            char **version_opengl,
                            CYBOOL con_soporte_sombras,
                            CYBOOL con_mips_por_hardware,
                            CYBOOL es_version_compatible,
                            CYBOOL es_shader_compilable))
{
    struct i_manejador_opengl_t *manejador_opengl;

    manejador_opengl = MALLOC(struct i_manejador_opengl_t);

    manejador_opengl->hwnd = hwnd;
    manejador_opengl->se_debe_destruir_ventana = se_debe_destruir_ventana;
    manejador_opengl->hdc_trabajo = hdc_trabajo;
    manejador_opengl->hdc_destino = ASIGNA_PUNTERO_PP_NO_NULL(hdc_destino, struct HDC__);

    manejador_opengl->hrc = ASIGNA_PUNTERO_PP_NO_NULL(hrc, struct HGLRC__);
    manejador_opengl->hPal = ASIGNA_PUNTERO_PP(hPal, struct HPALETTE__);
    manejador_opengl->hOldPal = ASIGNA_PUNTERO_PP(hOldPal, struct HPALETTE__);

    manejador_opengl->vendedor_opengl = ASIGNA_PUNTERO_PP_NO_NULL(vendedor_opengl, char);
    manejador_opengl->version_opengl = ASIGNA_PUNTERO_PP_NO_NULL(version_opengl, char);

    manejador_opengl->con_soporte_sombras = con_soporte_sombras;
    manejador_opengl->con_mips_por_hardware = con_mips_por_hardware;
    manejador_opengl->es_version_compatible = es_version_compatible;
    manejador_opengl->es_shader_compilable = es_shader_compilable;

    return manejador_opengl;
}

// ----------------------------------------------------------------------------

static void i_destruye_hdc_bitmap(struct HDC__ **hdc_trabajo)
{
    struct HBITMAP__ * hBitmap;

    assert_no_null(hdc_trabajo);
    assert_no_null(*hdc_trabajo);

    hBitmap = GetCurrentBitmap(*hdc_trabajo);
    DeleteDC(*hdc_trabajo);
    DeleteBitmap(hBitmap);

    *hdc_trabajo = NULL;
}

// ----------------------------------------------------------------------------

static void i_destruye_manejador_opengl(struct i_manejador_opengl_t **manejador_opengl)
{
    assert_no_null(manejador_opengl);
    assert_no_null(*manejador_opengl);

    cypestr_destruye(&(*manejador_opengl)->vendedor_opengl);
    cypestr_destruye(&(*manejador_opengl)->version_opengl);

    if ((*manejador_opengl)->hPal != NULL)
    {
        assert_no_null((*manejador_opengl)->hOldPal);
        (void)SelectPalette((*manejador_opengl)->hdc_trabajo, (*manejador_opengl)->hOldPal, 0); 

        DeletePalette((*manejador_opengl)->hPal);
    }
    else
        assert((*manejador_opengl)->hOldPal == NULL);

    if ((*manejador_opengl)->hrc != NULL) 
    {
        if ((*manejador_opengl)->hrc == wglGetCurrentContext())
            (void)wglMakeCurrent((*manejador_opengl)->hdc_trabajo, NULL); 

        (void)wglDeleteContext((*manejador_opengl)->hrc);
    }       

    if ((*manejador_opengl)->hdc_trabajo != (*manejador_opengl)->hdc_destino)
        i_destruye_hdc_bitmap(&(*manejador_opengl)->hdc_trabajo);

    if ((*manejador_opengl)->hwnd != NULL)
    {
        ReleaseDC((*manejador_opengl)->hwnd, (*manejador_opengl)->hdc_destino);
        if ((*manejador_opengl)->se_debe_destruir_ventana == CIERTO)
            DestroyWindow((*manejador_opengl)->hwnd);
    }
    else
        assert((*manejador_opengl)->se_debe_destruir_ventana == FALSO);

    FREE_PP(manejador_opengl, struct i_manejador_opengl_t);
}

// ----------------------------------------------------------------------------

CONSTRUCTOR(static struct cypegl3d_t *, i_crea, (
                            struct i_manejador_opengl_t **manejador_opengl,
                            unsigned long color_fondo_rgb,
                            int ox_destino, int oy_destino,
                            unsigned long ancho_destino, unsigned long alto_destino,
                            struct i_font_opengl_t **font_normal,
                            ArrEstructura(i_datos_material_t) **materiales_cargados,
                            struct i_tipo_dibujo_t **tipo_dibujo_actual,
                            ArrEstructura(i_tipo_dibujo_t) **pila_tipos_dibujo,
                            struct cyshadow_t **shader_sombra,
                            enum cplangl_tipo_lista_dibujo_t tipo_lista_dibujo,
                            const char *id_lista_activa,
                            struct apunorde(i_lista_dibujo_t) **lista_dibujos,
                            enum i_tipo_vista_t tipo_vista,
                            struct i_datos_vista_perspectiva_t **datos_vista_perspectiva,
                            unsigned long num_puntos_con_detalle,
                            unsigned long num_puntos_sin_detalle))
{
    struct cypegl3d_t *cypegl3d;

    cypegl3d = MALLOC(struct cypegl3d_t);

    cypegl3d->manejador_opengl = ASIGNA_PUNTERO_PP_NO_NULL(manejador_opengl, struct i_manejador_opengl_t);

    cypegl3d->ox_destino = ox_destino;
    cypegl3d->oy_destino = oy_destino;
    cypegl3d->ancho_destino = ancho_destino;
    cypegl3d->alto_destino = alto_destino;

    cypegl3d->color_fondo_rgb = color_fondo_rgb;

    cypegl3d->font_normal = ASIGNA_PUNTERO_PP_NO_NULL(font_normal, struct i_font_opengl_t);

    cypegl3d->num_planos_adicionales_clipping_activos = 0;

    cypegl3d->materiales_cargados = ASIGNA_PUNTERO_PP_NO_NULL(materiales_cargados, ArrEstructura(i_datos_material_t));

    cypegl3d->tipo_dibujo_actual = ASIGNA_PUNTERO_PP_NO_NULL(tipo_dibujo_actual, struct i_tipo_dibujo_t);
    cypegl3d->pila_tipos_dibujo = ASIGNA_PUNTERO_PP_NO_NULL(pila_tipos_dibujo, ArrEstructura(i_tipo_dibujo_t));

    cypegl3d->shader_sombra = ASIGNA_PUNTERO_PP(shader_sombra, struct cyshadow_t);

    cypegl3d->tipo_lista_dibujo = tipo_lista_dibujo;
    cypegl3d->id_lista_activa = COPIA_PUNTERO_OPCIONAL(id_lista_activa, char, cad_copia_cadena);
    cypegl3d->lista_dibujos = ASIGNA_PUNTERO_PP_NO_NULL(lista_dibujos, struct apunorde(i_lista_dibujo_t));

    cypegl3d->tipo_vista = tipo_vista;
    cypegl3d->datos_vista_perspectiva = ASIGNA_PUNTERO_PP_NO_NULL(datos_vista_perspectiva, struct i_datos_vista_perspectiva_t); 

    cypegl3d->informe.num_puntos_con_detalle = num_puntos_con_detalle;
    cypegl3d->informe.num_puntos_sin_detalle = num_puntos_sin_detalle;

    i_integridad(cypegl3d);

    return cypegl3d;
}

// ----------------------------------------------------------------------------

CONSTRUCTOR(static struct cypegl3d_t *, i_crea_con_camara_y_ventana_iniciada, (
                            struct i_manejador_opengl_t **manejador_opengl, 
                            unsigned long color_fondo_rgb))
                            
{
    int ox_destino, oy_destino;
    unsigned long ancho_destino, alto_destino;
    struct i_font_opengl_t *font_normal;
    struct i_tipo_dibujo_t *tipo_dibujo_actual;
    ArrEstructura(i_tipo_dibujo_t) *pila_tipos_dibujo;
    ArrEstructura(i_datos_material_t) *materiales_cargados;
    char *id_lista_activa;
    struct apunorde(i_lista_dibujo_t) *lista_dibujos;
    enum i_tipo_vista_t tipo_vista;
    struct i_datos_vista_perspectiva_t *datos_vista_perspectiva;
    struct cyshadow_t *shader_sombra;
    unsigned long num_puntos_con_detalle, num_puntos_sin_detalle;

    assert_no_null(manejador_opengl);
    assert_no_null(*manejador_opengl);

    ox_destino = 0;
    oy_destino = 0;
    ancho_destino = 0;
    alto_destino = 0;

    font_normal = i_lista_de_letras_para_opengl((*manejador_opengl)->hdc_trabajo);

    tipo_dibujo_actual = i_crea_tipo_dibujo_defecto(FALSO, CPLANGL_DIBUJO_CON_MUCHO_DETALLE, 0);
    pila_tipos_dibujo = arr_CreaPunteroST(0, i_tipo_dibujo_t);
    materiales_cargados = arr_CreaPunteroST(0, i_datos_material_t);

    id_lista_activa = NULL;
    lista_dibujos = apunorde_crea_sin_copia(i_existe_lista_dibujo, i_destruye_lista_dibujo, i_lista_dibujo_t);

    tipo_vista = i_VISTA_NO_DEFINIDA;
    datos_vista_perspectiva = i_crea_vista_perspectiva_por_defecto();

    if ((*manejador_opengl)->con_soporte_sombras == CIERTO)
        shader_sombra = cyshadow_crea_shadow();
    else
        shader_sombra = NULL;

    num_puntos_con_detalle = 0;
    num_puntos_sin_detalle = 0;

    return i_crea(manejador_opengl, color_fondo_rgb,
                            ox_destino, oy_destino, ancho_destino, alto_destino,
                            &font_normal, 
                            &materiales_cargados,
                            &tipo_dibujo_actual, &pila_tipos_dibujo, &shader_sombra,
                            CPLANGL_LISTA_CON_MUCHO_DETALLE, id_lista_activa, &lista_dibujos,
                            tipo_vista, &datos_vista_perspectiva,
                            num_puntos_con_detalle, num_puntos_sin_detalle);
}

// ----------------------------------------------------------------------------

static void i_destruye_ids_opengl(const ArrEstructura(i_datos_material_t) *materiales_cargados)
{
    unsigned int *id_texturas;
    unsigned long i, num;

    num = arr_NumElemsPunteroST(materiales_cargados, i_datos_material_t);
    id_texturas = CALLOC(num, unsigned int);

    for (i = 0; i < num; i++)
    {
        struct i_datos_material_t *datos_material;

        datos_material = arr_GetPunteroST(materiales_cargados, i, i_datos_material_t);
        assert_no_null(datos_material);

        id_texturas[i] = datos_material->id_material_opengl;
        datos_material->id_material_opengl = UINT_MAX;
    }

    cyogltextura_destruye_varias_texturas(num, &id_texturas);
}

// ----------------------------------------------------------------------------

void cypegl3d_destruye(struct cypegl3d_t **cypegl3d)
{   
    assert_no_null(cypegl3d);
    i_integridad(*cypegl3d);

    i_make_current((*cypegl3d)->manejador_opengl);

    assert(arr_NumElemsPunteroST((*cypegl3d)->pila_tipos_dibujo, i_tipo_dibujo_t) == 0);
    assert((*cypegl3d)->id_lista_activa == NULL);

    if (arr_NumElemsPunteroST((*cypegl3d)->materiales_cargados, i_datos_material_t) > 0)
        i_destruye_ids_opengl((*cypegl3d)->materiales_cargados);

    if ((*cypegl3d)->shader_sombra != NULL)
        cyshadow_destruye(&(*cypegl3d)->shader_sombra);

    i_destruye_tipo_dibujo(&(*cypegl3d)->tipo_dibujo_actual);
    arr_DestruyeEstructurasST(&(*cypegl3d)->pila_tipos_dibujo, i_tipo_dibujo_t, i_destruye_tipo_dibujo);

    i_destruye_font_opengl(&(*cypegl3d)->font_normal);

    arr_DestruyeEstructurasST(&(*cypegl3d)->materiales_cargados, i_datos_material_t, i_destruye_datos_material);
    apunorde_destruye(&(*cypegl3d)->lista_dibujos, i_lista_dibujo_t);

    i_destruye_vista_perspectiva(&(*cypegl3d)->datos_vista_perspectiva);

    i_destruye_manejador_opengl(&(*cypegl3d)->manejador_opengl);

    FREE_PP(cypegl3d, struct cypegl3d_t);
}

// ----------------------------------------------------------------------------

void cypegl3d_set_color_fondo(struct cypegl3d_t *cypegl3d, unsigned long color_fondo_rgb)
{
    assert_no_null(cypegl3d);
    cypegl3d->color_fondo_rgb = color_fondo_rgb;
}

// ----------------------------------------------------------------------------

static CYBOOL i_es_descripcion_correcta(
                        const PIXELFORMATDESCRIPTOR *descriptor_formato_pixel,
                        CYBOOL es_para_pantalla)
{
    assert_no_null(descriptor_formato_pixel);

    if (es_para_pantalla == CIERTO)
    {
        if((descriptor_formato_pixel->dwFlags & PFD_DOUBLEBUFFER) == 0)
            return FALSO;
    }
    else
    {
        if((descriptor_formato_pixel->dwFlags & PFD_DOUBLEBUFFER) != 0)
            return FALSO;
    }

    if((descriptor_formato_pixel->dwFlags & PFD_SUPPORT_OPENGL) == 0)
        return FALSO;

    if((descriptor_formato_pixel->dwFlags & PFD_GENERIC_FORMAT) == 0)
        return FALSO;

    if(descriptor_formato_pixel->iPixelType != PFD_TYPE_RGBA)
        return FALSO;

    return CIERTO;
}

// ----------------------------------------------------------------------------

static CYBOOL i_seleccionada_formato(struct HDC__ * hdc_trabajo, CYBOOL es_para_pantalla)
{
    BOOL res;
    int indPixelFormat;
    PIXELFORMATDESCRIPTOR descriptor_formato_pixel;

	memset(&descriptor_formato_pixel, 0, sizeof(PIXELFORMATDESCRIPTOR));

    descriptor_formato_pixel.nSize      = sizeof(PIXELFORMATDESCRIPTOR);
    descriptor_formato_pixel.nVersion   = 1;
    descriptor_formato_pixel.iPixelType = PFD_TYPE_RGBA;
    descriptor_formato_pixel.cColorBits = 32;
    descriptor_formato_pixel.cDepthBits = 16;

    if (es_para_pantalla == CIERTO)
        descriptor_formato_pixel.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    else
        descriptor_formato_pixel.dwFlags = PFD_DRAW_TO_BITMAP | PFD_SUPPORT_OPENGL | PFD_SUPPORT_GDI;

	indPixelFormat = ChoosePixelFormat(hdc_trabajo, &descriptor_formato_pixel);
    assert(indPixelFormat != 0);

    res = SetPixelFormat(hdc_trabajo, indPixelFormat, &descriptor_formato_pixel);

    if (res != 0)
        return CIERTO;
    else
        return FALSO;
}

// ----------------------------------------------------------------------------

static CYBOOL i_selecciona_formato_pixel(struct HDC__ * hdc_trabajo, CYBOOL es_para_pantalla)
{
    const char *valor_formato_pixel;

    valor_formato_pixel = getenv("FORMATO_PIXEL");

    if (valor_formato_pixel == NULL)
    {
        if (i_seleccionada_formato(hdc_trabajo, es_para_pantalla) == FALSO)
        {
            int i, num_formatos_pixels;
            PIXELFORMATDESCRIPTOR descriptor_formato_pixel;

            num_formatos_pixels = DescribePixelFormat(hdc_trabajo, 1, 0, NULL);

            for (i = 1; i <= num_formatos_pixels; i++)
            {
                int respuesta;

                respuesta = DescribePixelFormat(hdc_trabajo, i, sizeof(PIXELFORMATDESCRIPTOR), &descriptor_formato_pixel);

                if (respuesta != 0)
                {
                    if (i_es_descripcion_correcta(&descriptor_formato_pixel, es_para_pantalla) == CIERTO)
                    {
                        if (es_para_pantalla == CIERTO)
                            descriptor_formato_pixel.dwFlags |= PFD_DRAW_TO_WINDOW;
                        else
                            descriptor_formato_pixel.dwFlags |= PFD_DRAW_TO_BITMAP;

                        respuesta = SetPixelFormat(hdc_trabajo, i, &descriptor_formato_pixel);
                        if (respuesta != 0)
                            return CIERTO;
                    }
                }
            }

            return FALSO;
        }
        else
            return CIERTO;
    }
    else
    {
        int resp, valor;

        resp = sscanf(valor_formato_pixel, "%d", &valor);
        assert(resp == 1);

        resp = SetPixelFormat(hdc_trabajo, valor, NULL);
        assert_msg(resp != 0, "Error al seleccionar valor de formato de pixel");

        return CIERTO;
    }
}

// ----------------------------------------------------------------------------

static BYTE i_calcula_rango(BYTE bitsColor)
{
    int rango;
    unsigned long mascara;

    mascara = cambtipo_mascara_desplaza_bit(bitsColor);
    rango = cambtipo_unsigned_long_a_long(mascara) - 1;

    return cambtipo_int_a_unsigned_char(rango);
}

// ----------------------------------------------------------------------------

static void i_crea_paleta_si_es_necesario(struct HDC__ * hdc_trabajo, struct HPALETTE__ * *hPal, struct HPALETTE__ * *hOldPal)
{
    int nPixelFormat;
    PIXELFORMATDESCRIPTOR pfd;

    assert_no_null(hPal);
    assert_no_null(hOldPal);

    nPixelFormat = GetPixelFormat(hdc_trabajo);
    (void)DescribePixelFormat(hdc_trabajo, nPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

    if (!(pfd.dwFlags & PFD_NEED_PALETTE))
    {
        *hPal = NULL;
        *hOldPal = NULL;
    }
    else
    {
        LOGPALETTE *pPal;
        unsigned long nColors;
        unsigned int i;
        BYTE RedRange, GreenRange, BlueRange;

        nColors = cambtipo_mascara_desplaza_bit(pfd.cColorBits);

        pPal = (LOGPALETTE*)spy_malloc(sizeof(LOGPALETTE) + (unsigned int) nColors*sizeof(PALETTEENTRY));

        pPal->palVersion = 0x300;
        pPal->palNumEntries = (unsigned short) nColors;

        RedRange = i_calcula_rango(pfd.cRedBits);
        GreenRange = i_calcula_rango(pfd.cGreenBits);
        BlueRange = i_calcula_rango(pfd.cBlueBits);

        for (i = 0; i < nColors; i++)
        {
            pPal->palPalEntry[i].peRed = (i >> pfd.cRedShift) & RedRange;
            pPal->palPalEntry[i].peRed = (unsigned char)((double) pPal->palPalEntry[i].peRed * 255.0 / RedRange);

            pPal->palPalEntry[i].peGreen = (i >> pfd.cGreenShift) & GreenRange;
            pPal->palPalEntry[i].peGreen = (unsigned char)((double)pPal->palPalEntry[i].peGreen * 255.0 / GreenRange);

            pPal->palPalEntry[i].peBlue = (i >> pfd.cBlueShift) & BlueRange;
            pPal->palPalEntry[i].peBlue = (unsigned char)((double)pPal->palPalEntry[i].peBlue * 255.0 / BlueRange);

            pPal->palPalEntry[i].peFlags = (unsigned char)NULL;
        }
        
        *hPal = CreatePalette(pPal);
        *hOldPal = SelectPalette(hdc_trabajo, *hPal, FALSE);
        (void)RealizePalette(hdc_trabajo);

        FREE_PP(&pPal, LOGPALETTE);
    }
}

// ----------------------------------------------------------------------------

static CYBOOL i_se_puede_crear_manejador_opengl(
                        CYBOOL es_para_pantalla, 
                        struct HWND__ *hwnd, 
                        CYBOOL se_debe_destruir_ventana,
                        struct HDC__ *hdc_destino,
                        struct HDC__ *hdc_trabajo,
                        struct i_manejador_opengl_t **manejador_opengl)
{
    CYBOOL se_puede_crear_manejador_opengl;

    assert_no_null(hdc_destino);
    assert_no_null(manejador_opengl);

    i_inicializa_opengl();

    if (i_selecciona_formato_pixel(hdc_trabajo, es_para_pantalla) == CIERTO)
    {
        struct HGLRC__ *hrc;

        hrc = wglCreateContext(hdc_trabajo);

        if (hrc != NULL)
        {
            int resp;
            struct HPALETTE__ *hPal;
            struct HPALETTE__ *hOldPal;
            char *vendedor_opengl, *version_opengl;
            CYBOOL con_mips_por_hardware, con_soporte_sombras;
            CYBOOL es_version_compatible, es_shader_compilable;

            resp = wglMakeCurrent(hdc_trabajo, hrc);
            assert(resp != 0);

            i_crea_paleta_si_es_necesario(hdc_trabajo, &hPal, &hOldPal);

            vendedor_opengl = NULL;
            version_opengl = NULL;
            i_datos_hardware_tarjeta(
                        &con_mips_por_hardware,  &con_soporte_sombras,
                        &vendedor_opengl, &version_opengl,
                        &es_version_compatible, &es_shader_compilable);

            *manejador_opengl = i_crea_manejador_opengl(
                        hwnd, 
                        se_debe_destruir_ventana,
                        hdc_trabajo, 
                        &hdc_destino, &hrc, &hPal, &hOldPal,
                        &vendedor_opengl, &version_opengl,
                        con_soporte_sombras, con_mips_por_hardware,
                        es_version_compatible, es_shader_compilable);

            se_puede_crear_manejador_opengl = CIERTO;
        }
        else
            se_puede_crear_manejador_opengl = FALSO;
    }
    else
        se_puede_crear_manejador_opengl = FALSO;

    return se_puede_crear_manejador_opengl;
}   

// ----------------------------------------------------------------------------

static CYBOOL i_se_puede_crea_ventana_ficticia_base(struct i_manejador_opengl_t **manejador_opengl)
{
    CYBOOL se_puede_crear_manejador;
    struct HWND__ *hwnd_ficticia_loc;
    struct HDC__ *hdc_destino_loc;
    struct HINSTANCE__ *hInstance;
    CYBOOL es_para_pantalla, se_debe_destruir_ventana;

    hInstance = GetModuleHandle(NULL);

	hwnd_ficticia_loc = CreateWindow(
								"STATIC",
								"Título no mostrado",
								0,
                                0, 0, 800, 600, NULL, NULL, hInstance, NULL);
    assert_no_null(hwnd_ficticia_loc);
    hdc_destino_loc = GetDC(hwnd_ficticia_loc);

    es_para_pantalla = CIERTO;
    se_debe_destruir_ventana = CIERTO;

    se_puede_crear_manejador = i_se_puede_crear_manejador_opengl(
                        es_para_pantalla, 
                        hwnd_ficticia_loc, se_debe_destruir_ventana, 
                        hdc_destino_loc, hdc_destino_loc,
                        manejador_opengl);

    if (se_puede_crear_manejador == FALSO)
    {
        ReleaseDC(hwnd_ficticia_loc, hdc_destino_loc);
        DestroyWindow(hwnd_ficticia_loc);
    }

    return se_puede_crear_manejador;
}

// ----------------------------------------------------------------------------

static CYBOOL i_se_puede_crea_ventana_ficticia(struct i_manejador_opengl_t **manejador_opengl)
{
    CYBOOL se_puede_crear_primer_intento;

    se_puede_crear_primer_intento = i_se_puede_crea_ventana_ficticia_base(manejador_opengl);

    if (se_puede_crear_primer_intento == FALSO)
        return i_se_puede_crea_ventana_ficticia_base(manejador_opengl);
    else
        return se_puede_crear_primer_intento;
}

// ----------------------------------------------------------------------------

CYBOOL cypegl3d_se_puede_usar_opengl(void)
{
    static CYBOOL s_primera_vez = CIERTO;
    static CYBOOL s_se_puede_usar;

    if (s_primera_vez == CIERTO)
    {
        struct i_manejador_opengl_t *manejador_opengl;

        s_se_puede_usar = i_se_puede_crea_ventana_ficticia(&manejador_opengl);

        if (s_se_puede_usar == CIERTO)
            i_destruye_manejador_opengl(&manejador_opengl);

        s_primera_vez = FALSO;
    }

    return s_se_puede_usar;
}

// ----------------------------------------------------------------------------

CYBOOL cypegl3d_hardware_soporta_sombras(
                    char **vendedor_opengl,
                    char **version_opengl,
                    CYBOOL *con_mips_por_hardware,
                    CYBOOL *es_version_compatible,
                    CYBOOL *es_shader_compilable)
{
    CYBOOL con_soporte_sombras;
    struct i_manejador_opengl_t *manejador_opengl;
    CYBOOL hay_opengl;

    assert_no_null(vendedor_opengl);
    assert_no_null(version_opengl);
    assert_no_null(con_mips_por_hardware);
    assert_no_null(es_version_compatible);
    assert_no_null(es_shader_compilable);

    hay_opengl = i_se_puede_crea_ventana_ficticia(&manejador_opengl);

    if (hay_opengl == CIERTO)
    {
        con_soporte_sombras = manejador_opengl->con_soporte_sombras;

        *vendedor_opengl = cad_copia_cadena(manejador_opengl->vendedor_opengl);
        *version_opengl = cad_copia_cadena(manejador_opengl->version_opengl);
        *con_mips_por_hardware = manejador_opengl->con_mips_por_hardware;
        *es_version_compatible = manejador_opengl->es_version_compatible;
        *es_shader_compilable = manejador_opengl->es_shader_compilable;

        i_destruye_manejador_opengl(&manejador_opengl);
    }
    else
    {
        con_soporte_sombras = FALSO;

        *vendedor_opengl = cad_copia_cadena("XXXXX");
        *version_opengl = cad_copia_cadena("XXXXX");
        *con_mips_por_hardware = FALSO;
        *es_version_compatible = FALSO;
        *es_shader_compilable = FALSO;
    }

    return con_soporte_sombras;
}

// ----------------------------------------------------------------------------

CYBOOL cypegl3d_compila_codigo_shader(const char *fichero_shader, char **mensaje_error)
{
    CYBOOL se_ha_compilado;
    CYBOOL se_puede_compilar_shaders;
    struct i_manejador_opengl_t *manejador_opengl;

    se_puede_compilar_shaders = i_se_puede_crea_ventana_ficticia(&manejador_opengl);
    assert(se_puede_compilar_shaders == CIERTO);

    if (cyoglshader_soporta_shaders() == CIERTO)
        se_ha_compilado = cyoglshader_compila_codigo_shader(fichero_shader, mensaje_error);
    else
    {
        se_ha_compilado = FALSO;
        cypestr_copia_si_no_null(mensaje_error, "La tarjeta no soporta shaders");
    }

    i_destruye_manejador_opengl(&manejador_opengl);

    return se_ha_compilado;
}

// ----------------------------------------------------------------------------

struct cypegl3d_t *cypegl3d_crea_para_ventana(struct HWND__ * hwnd, unsigned long color_fondo_rgb)
{
    CYBOOL se_puede_crear_manejador;
    CYBOOL es_para_pantalla, se_debe_destruir_ventana;
    
    struct HDC__ * hdc_destino;
    struct i_manejador_opengl_t *manejador_opengl;
       
    es_para_pantalla = CIERTO;
    se_debe_destruir_ventana = FALSO;
    hdc_destino = GetDC(hwnd);

    se_puede_crear_manejador = i_se_puede_crear_manejador_opengl(
                        es_para_pantalla, 
                        hwnd, se_debe_destruir_ventana, hdc_destino, hdc_destino, 
                        &manejador_opengl);
    assert(se_puede_crear_manejador == CIERTO);

    return i_crea_con_camara_y_ventana_iniciada(&manejador_opengl, color_fondo_rgb);
}

// ----------------------------------------------------------------------------

static void i_set_dib_section_32_bits(struct HDC__ *hdc_trabajo, int ancho, int alto)
{
    BITMAPINFOHEADER bitmapInfo;
    unsigned long size;
    void *pBits;
    struct HBITMAP__ *hDibSection;
    HGDIOBJ hOldObject;

	 // Initialize the bitmapinfo header
	size = sizeof(BITMAPINFOHEADER);
	memset(&bitmapInfo, 0, size);

	bitmapInfo.biSize = size;
	bitmapInfo.biWidth = ancho;
	bitmapInfo.biHeight = alto;
	bitmapInfo.biPlanes = 1;
	bitmapInfo.biBitCount = 32;
	bitmapInfo.biCompression = BI_RGB;

	hDibSection = CreateDIBSection( 
                        hdc_trabajo,
						(BITMAPINFO*)&bitmapInfo,
						DIB_RGB_COLORS, &pBits, NULL, 0);
						
    hOldObject = SelectObject(hdc_trabajo, hDibSection);
    DeleteObject(hOldObject);
}

// ----------------------------------------------------------------------------

struct cypegl3d_t *cypegl3d_crea_para_dispositivo(
                        struct HDC__ *hdc_destino,
                        unsigned long color_fondo_rgb)
{
    CYBOOL es_para_pantalla, se_debe_destruir_ventana;
    struct HWND__ * hwnd;
    CYBOOL se_puede_crear_manejador;
    struct i_manejador_opengl_t *manejador_opengl;
    struct HDC__ *hdc_trabajo;

    es_para_pantalla = FALSO;
    se_debe_destruir_ventana = FALSO;
    hwnd = NULL;

    hdc_trabajo = CreateCompatibleDC(hdc_destino);
    i_set_dib_section_32_bits(hdc_trabajo, 1, 1);

    se_puede_crear_manejador = i_se_puede_crear_manejador_opengl(
                        es_para_pantalla, hwnd, se_debe_destruir_ventana, 
                        hdc_destino, hdc_trabajo, &manejador_opengl);
    assert(se_puede_crear_manejador == CIERTO);

    return i_crea_con_camara_y_ventana_iniciada(&manejador_opengl, color_fondo_rgb);
}

// ----------------------------------------------------------------------------

static void i_actualiza_viewport(
                        struct i_manejador_opengl_t *manejador_opengl,
                        int ancho_viewport_para_impresora, int alto_viewport_para_impresora,
                        unsigned long *ancho_viewport, 
                        unsigned long *alto_viewport)
{
    unsigned long ancho_viewport_loc, alto_viewport_loc;
    int ancho_trabajo, alto_trabajo;

    assert_no_null(ancho_viewport); 
    assert_no_null(alto_viewport);

    if (manejador_opengl->hwnd != NULL)
    {
        cypewin_dimensiones_pixels_hwnd_incluyendo_scroll_si_hay(
                            manejador_opengl->hwnd, 
                            &ancho_viewport_loc, &alto_viewport_loc);
    }
    else
    {
        assert_no_null(manejador_opengl->hdc_destino);
        ancho_viewport_loc = cambtipo_int_a_unsigned_long(ancho_viewport_para_impresora);
        alto_viewport_loc = cambtipo_int_a_unsigned_long(alto_viewport_para_impresora);
    }

    ancho_trabajo = cambtipo_unsigned_long_a_int(ancho_viewport_loc);
    alto_trabajo  = cambtipo_unsigned_long_a_int(alto_viewport_loc);

    if (manejador_opengl->hdc_trabajo != manejador_opengl->hdc_destino)
    {
        i_set_dib_section_32_bits(manejador_opengl->hdc_trabajo, ancho_trabajo, alto_trabajo);
    }

    glViewport(0, 0, ancho_trabajo, alto_trabajo);

    *ancho_viewport = ancho_viewport_loc, 
    *alto_viewport = alto_viewport_loc;
}

// ----------------------------------------------------------------------------

static void i_cambia_ejes_cype(void)
{
    double rotacion_para_ejes_de_cype;

    rotacion_para_ejes_de_cype = -90.;
    glRotated(rotacion_para_ejes_de_cype, 1., 0., 0.);
}

// ----------------------------------------------------------------------------

static void i_aplica_rotacion(double giro_x_grados, double giro_y_grados, double giro_z_grados)
{
    glRotated(giro_x_grados, 1., 0., 0.);
    glRotated(giro_y_grados, 0., 1., 0.);
    glRotated(giro_z_grados, 0., 0., 1.);
}

// ----------------------------------------------------------------------------

static void i_aplica_vista_ortogonal(
                        double x_min, double y_min,
                        double x_max, double y_max,
                        double giro_x_grados, double giro_y_grados, double giro_z_grados)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(x_min, x_max, y_min, y_max, -1000., 1000.);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    i_cambia_ejes_cype();
    i_aplica_rotacion(giro_x_grados, giro_y_grados, giro_z_grados);
}

// ----------------------------------------------------------------------------

static void i_aplica_vista_perspectiva(
                        double angulo_camara_grd,
                        double x_camara, double y_camara, double z_camara,
                        double x_objeto, double y_objeto,
                        double z_plano_proyeccion,
                        double giro_x_grados, double giro_y_grados, double giro_z_grados,
                        unsigned long ancho_destino, unsigned long alto_destino)
{
    double factor_deformacion;
    static double s_zNear = 0.1, s_zFar = 10000.;

    factor_deformacion = (double)ancho_destino / (double)alto_destino;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(angulo_camara_grd, factor_deformacion, s_zNear, s_zFar);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(x_camara, y_camara, z_camara, x_objeto, y_objeto, z_plano_proyeccion, 0., 1., 0.);

    i_cambia_ejes_cype();
    i_aplica_rotacion(giro_x_grados, giro_y_grados, giro_z_grados);
}

// ----------------------------------------------------------------------------

void cypegl3d_define_vista(
                        struct cypegl3d_t *cypegl3d, 
                        double x_min_orto, double y_min_orto, double x_max_orto, double y_max_orto,
                        double giro_x_grados, double giro_y_grados, double giro_z_grados)
{
    i_integridad(cypegl3d);
    i_make_current(cypegl3d->manejador_opengl);

    switch(cypegl3d->tipo_vista)
    {
        case i_VISTA_ORTOGONAL:
        {
            i_aplica_vista_ortogonal(
                        x_min_orto, y_min_orto,
                        x_max_orto, y_max_orto,
                        giro_x_grados, giro_y_grados, giro_z_grados);
            break;
        }
        case i_VISTA_PERSPECTIVA:
        {
            i_aplica_vista_perspectiva(
                        cypegl3d->datos_vista_perspectiva->angulo_foco_camara_grados,
                        cypegl3d->datos_vista_perspectiva->x_camara, 
                        cypegl3d->datos_vista_perspectiva->y_camara, 
                        cypegl3d->datos_vista_perspectiva->z_camara,
                        cypegl3d->datos_vista_perspectiva->x_centro_proyeccion, 
                        cypegl3d->datos_vista_perspectiva->y_centro_proyeccion,
                        cypegl3d->datos_vista_perspectiva->z_plano_proyeccion,
                        giro_x_grados, giro_y_grados, giro_z_grados,
                        cypegl3d->ancho_destino, cypegl3d->alto_destino);
            break;
        }
        case i_VISTA_NO_DEFINIDA:
        default_error();
    }
}

// ----------------------------------------------------------------------------

static void i_define_luz_materiales(void)
{
    static GLfloat ambientLight0[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    static GLfloat diffuseLight0[] = { 0.7f, 0.7f, 0.7f, 1.0f };

    static GLfloat ambientLight1[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    static GLfloat diffuseLight1[] = { 0.7f, 0.7f, 0.7f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight0);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, .005f);

    glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight1);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, .005f);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
}

// ----------------------------------------------------------------------------

static void i_define_luz_color(void)
{
    static GLfloat ambientLight0[] = { 0.45f, 0.45f, 0.45f, 1.0f };
    static GLfloat diffuseLight0[] = { 1.f, 1.f, 1.f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight0);

    glEnable(GL_LIGHT0);
    glDisable(GL_LIGHT1);
}

// ----------------------------------------------------------------------------

static void i_define_luz_dia(void)
{
    static GLfloat ambientLight0[] = { 0.55f, 0.55f, 0.55f, 1.0f };
    static GLfloat diffuseLight0[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight0);

    glEnable(GL_LIGHT0);
    glDisable(GL_LIGHT1);
}

// ----------------------------------------------------------------------------

static void i_define_luz_noche(void)
{
    static GLfloat ambientLight0[] = { 0.35f, 0.35f, 0.35f, 0.35f };
    static GLfloat diffuseLight0[] = { 0.0f, 0.0f, 0.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight0);

    glEnable(GL_LIGHT0);
    glDisable(GL_LIGHT1);
}

// ----------------------------------------------------------------------------

static void i_pon_luz_por_defecto(enum i_tipo_luz_ambiente_t tipo_luz_ambiente)
{
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    switch(tipo_luz_ambiente)
    {
        case i_LUZ_MATERIALES:
        
            i_define_luz_materiales();
            break;
        
        case i_LUZ_AMBIENTE:
        case i_LUZ_SOMBRA:

            i_define_luz_color();
            break;

        default_error();
    }
}

// ----------------------------------------------------------------------------

static void i_luces_activas(enum i_tipo_luz_ambiente_t tipo_luz, CYBOOL *activa_luz_1, CYBOOL *activa_luz_2)
{
    CYBOOL activa_luz_1_loc, activa_luz_2_loc;

    assert_no_null(activa_luz_1);
    assert_no_null(activa_luz_2);

    switch(tipo_luz)
    {
        case i_LUZ_MATERIALES:
        
            activa_luz_1_loc = CIERTO;
            activa_luz_2_loc = CIERTO;
            break;
        
        case i_LUZ_AMBIENTE:
        case i_LUZ_SOMBRA:

            activa_luz_1_loc = CIERTO;
            activa_luz_2_loc = FALSO;
            break;

        default_error();
    }

    *activa_luz_1 = activa_luz_1_loc;
    *activa_luz_2 = activa_luz_2_loc;
}

// ----------------------------------------------------------------------------

static enum i_tipo_luz_ambiente_t i_tipo_luz_ambiente(const struct i_tipo_dibujo_t *tipo_dibujo)
{
    assert_no_null(tipo_dibujo);

    switch(tipo_dibujo->tipo_dibujo_actual)
    {
        case CPLANGL_DIBUJO_CON_MUCHO_DETALLE:
        case CPLANGL_DIBUJO_CON_POCO_DETALLE:
        case CPLANGL_DIBUJO_LINEAS:

            if (tipo_dibujo->con_texturas == CIERTO)
                return i_LUZ_MATERIALES;
            else
                return i_LUZ_AMBIENTE;

        case CPLANGL_DIBUJO_SOMBRAS_SHADER:
        case CPLANGL_GENERA_SOMBRAS_SHADER:

            return i_LUZ_AMBIENTE;

        default_error();
    }
}

// ----------------------------------------------------------------------------

static void i_set_atributos_opengl_defecto(void)
{
    glCullFace(GL_BACK);
    glPolygonMode(GL_FRONT, GL_FILL);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}

// ----------------------------------------------------------------------------

static void i_prepara_tipo_dibujo(
                        const struct i_tipo_dibujo_t *tipo_dibujo, 
                        struct cyshadow_t *shadow)
{
    enum i_tipo_luz_ambiente_t tipo_luz;

    assert_no_null(tipo_dibujo);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    tipo_luz = i_tipo_luz_ambiente(tipo_dibujo);
    i_pon_luz_por_defecto(tipo_luz);

    switch (tipo_dibujo->tipo_dibujo_actual) 
    { 
        case CPLANGL_DIBUJO_CON_MUCHO_DETALLE:
        {
            i_set_atributos_opengl_defecto();
            glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
            break;
        }
        case CPLANGL_DIBUJO_CON_POCO_DETALLE:
        {
            i_set_atributos_opengl_defecto();
            glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_DONT_CARE);
            break;
        }
        case CPLANGL_DIBUJO_LINEAS:
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDisable(GL_CULL_FACE);
            glDisable(GL_LIGHTING);

            glShadeModel(GL_FLAT);
            glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
            glLineWidth(1.f);
            glColor3ub(0, 0, 0);
            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
            break;
        }
        case CPLANGL_GENERA_SOMBRAS_SHADER:

            cyshadow_inicia_generacion_sombra(shadow);
            //glCullFace(GL_FRONT);
            break;

        case CPLANGL_DIBUJO_SOMBRAS_SHADER:
        {
            CYBOOL activa_luz_1, activa_luz_2;

            i_luces_activas(tipo_luz, &activa_luz_1, &activa_luz_2);
            i_set_atributos_opengl_defecto();
            glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
            cyshadow_inicia_dibujo_sombra(shadow, i_UNIDAD_MATERIALES, activa_luz_1, activa_luz_2);
            break;
        }
        default_error(); 
    }
}

// ----------------------------------------------------------------------------

static void i_inicio_dibujo_segun_modelo(const struct i_tipo_dibujo_t *tipo_dibujo, struct cyshadow_t *shadow, unsigned long color_fondo_rgb)
{
    float red, green, blue;

    red = GetRValue(color_fondo_rgb) / 255.f;
    green = GetGValue(color_fondo_rgb) / 255.f;
    blue = GetBValue(color_fondo_rgb) / 255.f;

    glClearColor(red, green, blue, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    i_prepara_tipo_dibujo(tipo_dibujo, shadow);
}

// ----------------------------------------------------------------------------

static void i_desactiva_todos_planos_adicionales_de_clipping(void)
{
    unsigned int i;

    for (i = 0; i < NUM_MAXIMOS_PLANOS; i++)
    {
        GLenum plane;

        plane = GL_CLIP_PLANE0 + i;
        glDisable(plane);
    }
}

// ----------------------------------------------------------------------------

void cypegl3d_cambia_vista_ortogonal(struct cypegl3d_t *cypegl3d)
{
    i_integridad(cypegl3d);
    cypegl3d->tipo_vista = i_VISTA_ORTOGONAL;
}

// ----------------------------------------------------------------------------

static void i_calcula_datos_camara(
                        double x_min_ventana, double y_min_ventana,
                        double x_max_ventana, double y_max_ventana,
                        double *angulo_camara_grd,
                        double *x_camara,
                        double *y_camara)
{
    assert_no_null(angulo_camara_grd);
    assert_no_null(x_camara);
    assert_no_null(y_camara);

    *angulo_camara_grd = 45.;
    *x_camara = (x_min_ventana + x_max_ventana) / 2.;
    *y_camara = (y_min_ventana + y_max_ventana) / 2.;
}

// ---------------------------------------------------------------------------

static double i_calcula_coord_z_camara_para_ver_coord(
                        double coord_objeto, double coord_z_objeto,
                        double coord_camara, double tg_angulo_camara, double factor_aspecto)
{
    double coord_z_de_camara_para_ver_coord;
    double coord_desde_centro_camara, distancia_camara_para_que_se_vea_objeto;

    coord_desde_centro_camara = coord_objeto - coord_camara;
    distancia_camara_para_que_se_vea_objeto = fabs(coord_desde_centro_camara) / tg_angulo_camara / factor_aspecto;
    coord_z_de_camara_para_ver_coord = distancia_camara_para_que_se_vea_objeto + coord_z_objeto;

    return coord_z_de_camara_para_ver_coord;
}

// ---------------------------------------------------------------------------

static double i_calcula_z_camara_para_ver_puntos(
                        const ArrPunto3D *nube_de_puntos,
                        double x_min_ventana, double y_min_ventana,
                        double x_max_ventana, double y_max_ventana,
                        const struct proyeccion_t *proyeccion,
                        double factor_aspecto,
                        double tg_angulo_camara,
                        double x_camara, double y_camara)
{
    unsigned long i, num;
    double z_camara;

    z_camara = -i_INFINITO;

    num = arr_NumElemsPunto3D(nube_de_puntos);

    for (i = 0; i < num; i++)
    {
        double x, y, z, x_proy, y_proy, z_proy;
        double z_camara_para_ver_punto;

        arr_GetPunto3D(nube_de_puntos, i, &x, &y, &z);

        proyecta_punto(proyeccion, x, y, z, &x_proy, &y_proy, &z_proy);

        if (x_min_ventana < x_proy && x_proy < x_max_ventana)
        {
            z_camara_para_ver_punto = i_calcula_coord_z_camara_para_ver_coord(
                        x_proy, z_proy, 
                        x_camara, tg_angulo_camara, factor_aspecto);

            z_camara = MAX(z_camara_para_ver_punto, z_camara);
        }

        if (y_min_ventana < y_proy && y_proy < y_max_ventana)
        {
            z_camara_para_ver_punto = i_calcula_coord_z_camara_para_ver_coord(
                        y_proy, z_proy, 
                        y_camara, tg_angulo_camara, 1.);
            z_camara = MAX(z_camara_para_ver_punto, z_camara);
        }
    }

    return z_camara;
}

// ---------------------------------------------------------------------------

static double i_calcula_z_camara_sombra(
                        const ArrPuntero(ArrPunto3D) *puntos_recorrido_dias,
                        double x_min_ventana, double y_min_ventana,
                        double x_max_ventana, double y_max_ventana,
                        const struct proyeccion_t *proyeccion,
                        double factor_aspecto,
                        double tg_angulo_camara, double x_camara, double y_camara)
{
    double z_camara_sombra;
    unsigned long i, num;

    num = arr_NumElemsPunteroTD(puntos_recorrido_dias, ArrPunto3D);
    z_camara_sombra = -i_INFINITO;

    for (i = 0; i < num; i++)
    {
        const ArrPunto3D *puntos_recorrido_dia;
        double z_camara_recorrido_dia;

        puntos_recorrido_dia = arr_GetPunteroConstTD(puntos_recorrido_dias, i, ArrPunto3D);

        z_camara_recorrido_dia = i_calcula_z_camara_para_ver_puntos(
                        puntos_recorrido_dia, 
                        x_min_ventana, y_min_ventana,
                        x_max_ventana, y_max_ventana,
                        proyeccion,
                        factor_aspecto, 
                        tg_angulo_camara, x_camara, y_camara);

        z_camara_sombra = MAX(z_camara_recorrido_dia, z_camara_sombra);
    }

    return z_camara_sombra;
}

// ---------------------------------------------------------------------------

static double i_obtener_z_camara(
                        const ArrPunto3D *nube_de_puntos,
                        double x_min_ventana, double y_min_ventana,
                        double x_max_ventana, double y_max_ventana,
                        CYBOOL con_sombra, 
                        const ArrPuntero(ArrPunto3D) *puntos_recorrido_dias,
                        const struct proyeccion_t *proyeccion,
                        double factor_aspecto,
                        double tg_angulo_camara,
                        double x_camara, double y_camara)
{
    double z_camara;

    z_camara = i_calcula_z_camara_para_ver_puntos(
                        nube_de_puntos, 
                        x_min_ventana, y_min_ventana,
                        x_max_ventana, y_max_ventana,
                        proyeccion,
                        factor_aspecto, tg_angulo_camara, x_camara, y_camara);

    if (con_sombra == CIERTO)
    {
        double z_camara_sombra;

        z_camara_sombra = i_calcula_z_camara_sombra(
                        puntos_recorrido_dias, 
                        x_min_ventana, y_min_ventana,
                        x_max_ventana, y_max_ventana,
                        proyeccion,
                        factor_aspecto,
                        tg_angulo_camara, x_camara, y_camara);

        z_camara = MAX(z_camara_sombra, z_camara);
    }

    return z_camara;
}

// ----------------------------------------------------------------------------

static void i_calcula_max_min_perspectiva(
                        const ArrPunto3D *nube_de_puntos,
                        double x_min_ventana, double y_min_ventana,
                        double x_max_ventana, double y_max_ventana,
                        const struct proyeccion_t *proyeccion,
                        double x_camara, double y_camara, double z_camara,
                        double dz_proyeccion,
                        double *x_min_pers, double *y_min_pers,
                        double *x_max_pers, double *y_max_pers)
{
    unsigned long i, num;

    assert_no_null(x_min_pers);
    assert_no_null(y_min_pers);
    assert_no_null(x_max_pers);
    assert_no_null(y_max_pers);

    num = arr_NumElemsPunto3D(nube_de_puntos);

    for (i = 0; i < num; i++)
    {
        double x, y, z, x_proy, y_proy, z_proy, dz;

        arr_GetPunto3D(nube_de_puntos, i, &x, &y, &z);
        proyecta_punto(proyeccion, x, y, z, &x_proy, &y_proy, &z_proy);

        dz = z_camara - z_proy;

        if (x_min_ventana < x_proy && x_proy < x_max_ventana)
        {
            double dx, x_pers;

            dx = x_proy - x_camara;
            x_pers = x_camara + dx * dz_proyeccion / dz;

            *x_min_pers = MIN(*x_min_pers, x_pers);
            *x_max_pers = MAX(*x_max_pers, x_pers);
        }

        if (y_min_ventana < y_proy && y_proy < y_max_ventana)
        {
            double dy, y_pers;

            dy = y_proy - y_camara;
            y_pers = y_camara + dy * dz_proyeccion / dz;

            *y_min_pers = MIN(*y_min_pers, y_pers);
            *y_max_pers = MAX(*y_max_pers, y_pers);
        }    
    }
}

// ----------------------------------------------------------------------------

static void i_calcula_centro_desde_posicion_camara(
                        const ArrPunto3D *nube_de_puntos,
                        double x_min_ventana, double y_min_ventana,
                        double x_max_ventana, double y_max_ventana,
                        CYBOOL con_sombra,
                        const ArrPuntero(ArrPunto3D) *puntos_recorrido_dias,
                        const struct proyeccion_t *proyeccion,
                        double x_camara, double y_camara, double z_camara,
                        double z_plano_proyeccion,
                        double *x_centro, double *y_centro)
{
    unsigned long num;
    double dz_proyeccion;
    double x_min_pers, y_min_pers, x_max_pers, y_max_pers;

    num = arr_NumElemsPunto3D(nube_de_puntos);

    if (num > 0)
    {
        dz_proyeccion = z_camara - z_plano_proyeccion;
        x_min_pers = y_min_pers = 1e100;
        x_max_pers = y_max_pers = -1e100;

        i_calcula_max_min_perspectiva(
                        nube_de_puntos, 
                        x_min_ventana, y_min_ventana,
                        x_max_ventana, y_max_ventana,
                        proyeccion,
                        x_camara, y_camara, z_camara, dz_proyeccion,
                        &x_min_pers, &y_min_pers, &x_max_pers, &y_max_pers);

        if (con_sombra == CIERTO)
        {
            unsigned long i;

            num = arr_NumElemsPunteroTD(puntos_recorrido_dias, ArrPunto3D);

            for (i = 0; i < num; i++)
            {
                const ArrPunto3D *puntos_recorrido_dia;

                puntos_recorrido_dia = arr_GetPunteroConstTD(puntos_recorrido_dias, i, ArrPunto3D);
                i_calcula_max_min_perspectiva(
                            puntos_recorrido_dia, 
                            x_min_ventana, y_min_ventana,
                            x_max_ventana, y_max_ventana,
                            proyeccion,
                            x_camara, y_camara, z_camara, dz_proyeccion,
                            &x_min_pers, &y_min_pers, &x_max_pers, &y_max_pers);
            }
        }

        *x_centro = (x_max_pers + x_min_pers) / 2.;
        *y_centro = (y_max_pers + y_min_pers) / 2.;
    }
    else
    {
        *x_centro = 0.;
        *y_centro = 0.;
    }
}

// ---------------------------------------------------------------------------

static void i_calcula_datos_vista_perspectiva(
                        const ArrPunto3D *nube_de_puntos_sin_proyectar,
                        CYBOOL con_sombra,
                        const ArrPuntero(ArrPunto3D) *puntos_recorrido_dias,
                        const struct proyeccion_t *proyeccion,
                        double x_min_ventana, double y_min_ventana, 
                        double x_max_ventana, double y_max_ventana,
                        double *angulo_camara_grd,
                        double *x_camara, double *y_camara, double *z_camara,
                        double *x_objeto, double *y_objeto, double *z_plano_proyeccion,
                        double *dx, double *dy)
{
    double x_camara_loc, y_camara_loc, z_camara_loc, x_objeto_loc, y_objeto_loc;
    double angulo_camara_grd_loc;
    double tg_angulo_camara, dz, z_plano_proyeccion_loc;
    double x_centro, y_centro;
    double dx_loc, dy_loc;
    double factor_aspecto;

    assert_no_null(angulo_camara_grd);
    assert_no_null(x_camara);
    assert_no_null(y_camara);
    assert_no_null(z_camara);
    assert_no_null(x_objeto);
    assert_no_null(y_objeto);
    assert_no_null(z_plano_proyeccion);
    assert_no_null(dx);
    assert_no_null(dy);

    assert(y_min_ventana < y_max_ventana);
    factor_aspecto = (x_max_ventana - x_min_ventana) / (y_max_ventana - y_min_ventana);

    i_calcula_datos_camara(
                        x_min_ventana, y_min_ventana,
                        x_max_ventana, y_max_ventana,
                        &angulo_camara_grd_loc, 
                        &x_camara_loc, &y_camara_loc);

    tg_angulo_camara = tan(angulo_camara_grd_loc / 2. * PI / 180.);

    z_camara_loc = i_obtener_z_camara(
                        nube_de_puntos_sin_proyectar,
                        x_min_ventana, y_min_ventana,
                        x_max_ventana, y_max_ventana,
                        con_sombra, puntos_recorrido_dias,
                        proyeccion, factor_aspecto,
                        tg_angulo_camara, x_camara_loc, y_camara_loc);

    dz = (y_max_ventana - y_min_ventana) / 2. / tg_angulo_camara;
    z_plano_proyeccion_loc = z_camara_loc - dz;

    i_calcula_centro_desde_posicion_camara(
                        nube_de_puntos_sin_proyectar, 
                        x_min_ventana, y_min_ventana,
                        x_max_ventana, y_max_ventana,
                        con_sombra, puntos_recorrido_dias,
                        proyeccion,
                        x_camara_loc, y_camara_loc, z_camara_loc,
                        z_plano_proyeccion_loc, 
                        &x_centro, &y_centro);

    dx_loc = x_centro - x_camara_loc;
    dy_loc = y_centro - y_camara_loc;
    x_objeto_loc = x_camara_loc = x_centro;
    y_objeto_loc = y_camara_loc = y_centro;

    *angulo_camara_grd = angulo_camara_grd_loc;
    *x_camara = x_camara_loc; 
    *y_camara = y_camara_loc;
    *z_camara = z_camara_loc;
    *x_objeto = x_objeto_loc; 
    *y_objeto = y_objeto_loc; 
    *z_plano_proyeccion = z_plano_proyeccion_loc;
    *dx = dx_loc;
    *dy = dy_loc;
}

// ----------------------------------------------------------------------------

void cypegl3d_cambia_vista_perspectiva_vc(
                        struct cypegl3d_t *cypegl3d,
                        const ArrPunto3D *nube_de_puntos_sin_proyectar,
                        CYBOOL con_sombra,
                        const ArrPuntero(ArrPunto3D) *puntos_recorrido_dias,
                        const struct proyeccion_t *proyeccion,
                        double x_min_ventana, double y_min_ventana, 
                        double x_max_ventana, double y_max_ventana)
{
    i_integridad(cypegl3d);

    cypegl3d->tipo_vista = i_VISTA_PERSPECTIVA;

    i_calcula_datos_vista_perspectiva(
                        nube_de_puntos_sin_proyectar,
                        con_sombra, puntos_recorrido_dias,
                        proyeccion,
                        x_min_ventana, y_min_ventana, 
                        x_max_ventana, y_max_ventana,
                        &cypegl3d->datos_vista_perspectiva->angulo_foco_camara_grados, 
                        &cypegl3d->datos_vista_perspectiva->x_camara, &cypegl3d->datos_vista_perspectiva->y_camara, &cypegl3d->datos_vista_perspectiva->z_camara,
                        &cypegl3d->datos_vista_perspectiva->x_centro_proyeccion, &cypegl3d->datos_vista_perspectiva->y_centro_proyeccion, &cypegl3d->datos_vista_perspectiva->z_plano_proyeccion,
                        &cypegl3d->datos_vista_perspectiva->dx_acumulados_por_avance, &cypegl3d->datos_vista_perspectiva->dy_acumulados_por_avance);
}

// ----------------------------------------------------------------------------

static double i_calcula_angulo_foco_camara_grados(
                        double y_min, double y_max, 
                        double distancia_camara)
{
    double alto_ventana_mts, angulo_en_radianes, tangente;

    assert(distancia_camara > 0.00001);

    alto_ventana_mts = y_max - y_min;
    assert(alto_ventana_mts > 0.00001);

    tangente = alto_ventana_mts / 2. / distancia_camara;

    angulo_en_radianes = atan(tangente);

    return 2. * angulo_en_radianes * 180. / PI;
}

// ----------------------------------------------------------------------------

static void i_incrementos_xy_por_avance_en_z(
                            struct i_datos_vista_perspectiva_t *datos_vista_perspectiva, double dz, 
                            double *dx, double *dy)
{
    double cose_eje_x, seno_eje_x;
    double cose_eje_y, seno_eje_y;
    double dx_loc, dy_loc;

    assert_no_null(datos_vista_perspectiva);

    // Se desplaza X según la dirección del avance de la camara hacia el objeto
    mt_parametros_segmento(
                    -datos_vista_perspectiva->z_camara, datos_vista_perspectiva->x_camara,
                    -datos_vista_perspectiva->z_plano_proyeccion, datos_vista_perspectiva->x_centro_proyeccion, 
                    NULL, &cose_eje_x, &seno_eje_x);
    dx_loc = -dz / cose_eje_x * seno_eje_x;

    // Se desplaza Y según la dirección del avance de la camara hacia el objeto
    mt_parametros_segmento(
                    -datos_vista_perspectiva->z_camara, datos_vista_perspectiva->y_camara, 
                    -datos_vista_perspectiva->z_plano_proyeccion, datos_vista_perspectiva->y_centro_proyeccion, 
                    NULL, &cose_eje_y, &seno_eje_y);
    dy_loc = -dz / cose_eje_y * seno_eje_y;

    *dx = dx_loc;
    *dy = dy_loc;
}

// ---------------------------------------------------------------------------

static void i_calcula_posicion_camara(
                        double x_min, double y_min, double x_max, double y_max,
                        double z_camara, double z_plano_proyeccion,
                        double dx_acumulados_por_avance, double dy_acumulados_por_avance,
                        double *angulo_foco_camara_grados,
                        double *x_centro_proyeccion,
                        double *y_centro_proyeccion)
{
    double distancia_camara;

    distancia_camara = z_camara - z_plano_proyeccion;

    *angulo_foco_camara_grados = i_calcula_angulo_foco_camara_grados(y_min, y_max, distancia_camara);

    *x_centro_proyeccion = (x_max + x_min) / 2. + dx_acumulados_por_avance;
    *y_centro_proyeccion = (y_max + y_min) / 2. + dy_acumulados_por_avance;
}

// ---------------------------------------------------------------------------

void cypegl3d_zoom(struct cypegl3d_t *cypegl3d, double x_min, double y_min, double x_max, double y_max)
{
    assert_no_null(cypegl3d);
    assert_no_null(cypegl3d->datos_vista_perspectiva);

    switch(cypegl3d->tipo_vista)
    {
        case i_VISTA_ORTOGONAL:
        
            break;
        
        case i_VISTA_PERSPECTIVA:
        {
            i_calcula_posicion_camara(
                        x_min, y_min, x_max, y_max,
                        cypegl3d->datos_vista_perspectiva->z_camara, 
                        cypegl3d->datos_vista_perspectiva->z_plano_proyeccion,
                        cypegl3d->datos_vista_perspectiva->dx_acumulados_por_avance,
                        cypegl3d->datos_vista_perspectiva->dy_acumulados_por_avance,
                        &cypegl3d->datos_vista_perspectiva->angulo_foco_camara_grados,
                        &cypegl3d->datos_vista_perspectiva->x_centro_proyeccion,
                        &cypegl3d->datos_vista_perspectiva->y_centro_proyeccion);
            break;
        }
        case i_VISTA_NO_DEFINIDA:

            break;

        default_error();
    }
}

// ----------------------------------------------------------------------------

CYBOOL cypegl3d_se_permite_zoom_animado(struct cypegl3d_t *cypegl3d)
{
    i_integridad(cypegl3d);

    switch(cypegl3d->tipo_vista)
    {
        case i_VISTA_ORTOGONAL:
        {
            return FALSO;
        }
        case i_VISTA_PERSPECTIVA:
        {
            return CIERTO;
        }
        case i_VISTA_NO_DEFINIDA:
        default_error();
    }
}

// ----------------------------------------------------------------------------

void cypegl3d_set_posicion_camara(
                        struct cypegl3d_t *cypegl3d,
                        double angulo_foco_camara_grados,
                        double x_centro_proyeccion, double y_centro_proyeccion)
{
    i_integridad(cypegl3d);
    assert_no_null(cypegl3d->datos_vista_perspectiva);

    cypegl3d->datos_vista_perspectiva->angulo_foco_camara_grados = angulo_foco_camara_grados;

    cypegl3d->datos_vista_perspectiva->x_centro_proyeccion = x_centro_proyeccion;
    cypegl3d->datos_vista_perspectiva->y_centro_proyeccion = y_centro_proyeccion;
}

// ----------------------------------------------------------------------------

void cypegl3d_get_posicion_camara(
                        struct cypegl3d_t *cypegl3d,
                        double *angulo_foco_camara_grados, double *x_centro_proyeccion, double *y_centro_proyeccion)
{
    i_integridad(cypegl3d);
    assert_no_null(angulo_foco_camara_grados);
    assert_no_null(x_centro_proyeccion);
    assert_no_null(y_centro_proyeccion);
    assert_no_null(cypegl3d->datos_vista_perspectiva);

    *angulo_foco_camara_grados = cypegl3d->datos_vista_perspectiva->angulo_foco_camara_grados;
    *x_centro_proyeccion = cypegl3d->datos_vista_perspectiva->x_centro_proyeccion;
    *y_centro_proyeccion = cypegl3d->datos_vista_perspectiva->y_centro_proyeccion;
}

// ----------------------------------------------------------------------------

void cypegl3d_calcula_datos_camara(
                        struct cypegl3d_t *cypegl3d,
                        double x_min, double y_min, double x_max, double y_max,
                        double *angulo_foco_camara_grados,
                        double *x_centro_proyeccion,
                        double *y_centro_proyeccion)
{
    i_integridad(cypegl3d);
    assert_no_null(cypegl3d->datos_vista_perspectiva);

    i_calcula_posicion_camara(
                        x_min, y_min, x_max, y_max,
                        cypegl3d->datos_vista_perspectiva->z_camara,
                        cypegl3d->datos_vista_perspectiva->z_plano_proyeccion,
                        cypegl3d->datos_vista_perspectiva->dx_acumulados_por_avance,
                        cypegl3d->datos_vista_perspectiva->dy_acumulados_por_avance,
                        angulo_foco_camara_grados, x_centro_proyeccion, y_centro_proyeccion);
}

// ----------------------------------------------------------------------------

void cypegl3d_avanza(struct cypegl3d_t *cypegl3d, double dx, double dy, double dz)
{
    i_integridad(cypegl3d);
    i_make_current(cypegl3d->manejador_opengl);
    assert_no_null(cypegl3d->datos_vista_perspectiva);

    switch(cypegl3d->tipo_vista)
    {
        case i_VISTA_PERSPECTIVA:
        {
            double dx_debido_a_dz, dy_debido_a_dz;

            i_incrementos_xy_por_avance_en_z(cypegl3d->datos_vista_perspectiva, dz, &dx_debido_a_dz, &dy_debido_a_dz);

            dx += dx_debido_a_dz;
            dy += dy_debido_a_dz;

            cypegl3d->datos_vista_perspectiva->x_centro_proyeccion += dx;
            cypegl3d->datos_vista_perspectiva->x_camara += dx;

            cypegl3d->datos_vista_perspectiva->y_centro_proyeccion += dy;
            cypegl3d->datos_vista_perspectiva->y_camara += dy;

            cypegl3d->datos_vista_perspectiva->z_plano_proyeccion += dz;
            cypegl3d->datos_vista_perspectiva->z_camara += dz;

            cypegl3d->datos_vista_perspectiva->dx_acumulados_por_avance += dx;
            cypegl3d->datos_vista_perspectiva->dy_acumulados_por_avance += dy;
            break;
        }
        case i_VISTA_NO_DEFINIDA:
        case i_VISTA_ORTOGONAL:

            break;

        default_error();
    }
}

// ----------------------------------------------------------------------------

static void i_calcula_coseno_seno_giro(double incr_giro_alrededor_eje_x_grados, double *coseno, double *seno)
{
    double incr_giro_alrededor_eje_x_rad;

    assert_no_null(coseno);
    assert_no_null(seno);

    incr_giro_alrededor_eje_x_rad = incr_giro_alrededor_eje_x_grados * PI / 180.;

    *coseno = cos(incr_giro_alrededor_eje_x_rad);
    *seno = sin(incr_giro_alrededor_eje_x_rad);
}

// ---------------------------------------------------------------------------

static void i_gira_posicion_camara(
                            double x_camara, double y_camara, double z_camara,
                            double x_centro, double y_centro, double z_centro,
                            double Ux, double Uy, double Uz,
                            double Vx, double Vy, double Vz,
                            double Wx, double Wy, double Wz,
                            double *x_centro_girado, double *y_centro_girado, double *z_centro_girado)
{
    double dx, dy, dz;
    double dx_girado, dy_girado, dz_girado;

    assert_no_null(x_centro_girado);
    assert_no_null(y_centro_girado);
    assert_no_null(z_centro_girado);

    dx = x_centro - x_camara;
    dy = y_centro - y_camara;
    dz = z_centro - z_camara;

    dx_girado = Ux * dx + Uy * dy + Uz * dz;
    dy_girado = Vx * dx + Vy * dy + Vz * dz;
    dz_girado = Wx * dx + Wy * dy + Wz * dz;

    *x_centro_girado = x_camara + dx_girado;
    *y_centro_girado = y_camara + dy_girado;
    *z_centro_girado = z_camara + dz_girado;
}

// ---------------------------------------------------------------------------

void cypegl3d_gira_camara_eje_x(struct cypegl3d_t *cypegl3d, double incr_giro_alrededor_eje_x_grados)
{
    double coseno, seno;

    i_integridad(cypegl3d);
    assert_no_null(cypegl3d->datos_vista_perspectiva);

    i_calcula_coseno_seno_giro(incr_giro_alrededor_eje_x_grados, &coseno, &seno);

    i_gira_posicion_camara(
                        cypegl3d->datos_vista_perspectiva->x_camara, 
                        cypegl3d->datos_vista_perspectiva->y_camara, 
                        cypegl3d->datos_vista_perspectiva->z_camara,
                        cypegl3d->datos_vista_perspectiva->x_centro_proyeccion, 
                        cypegl3d->datos_vista_perspectiva->y_centro_proyeccion, 
                        cypegl3d->datos_vista_perspectiva->z_plano_proyeccion,
                        1., 0., 0.,
                        0., coseno, -seno,
                        0., seno, coseno,
                        &cypegl3d->datos_vista_perspectiva->x_centro_proyeccion, 
                        &cypegl3d->datos_vista_perspectiva->y_centro_proyeccion, 
                        &cypegl3d->datos_vista_perspectiva->z_plano_proyeccion);
}

// ----------------------------------------------------------------------------

void cypegl3d_gira_camara_eje_y(struct cypegl3d_t *cypegl3d, double incr_giro_alrededor_eje_y_grados)
{
    double coseno, seno;

    i_integridad(cypegl3d);
    assert_no_null(cypegl3d->datos_vista_perspectiva);

    i_calcula_coseno_seno_giro(incr_giro_alrededor_eje_y_grados, &coseno, &seno);

    i_gira_posicion_camara(
                        cypegl3d->datos_vista_perspectiva->x_camara, 
                        cypegl3d->datos_vista_perspectiva->y_camara, 
                        cypegl3d->datos_vista_perspectiva->z_camara,
                        cypegl3d->datos_vista_perspectiva->x_centro_proyeccion, 
                        cypegl3d->datos_vista_perspectiva->y_centro_proyeccion, 
                        cypegl3d->datos_vista_perspectiva->z_plano_proyeccion,
                        coseno, 0., -seno,
                        0., 1., 0.,
                        seno, 0., coseno,
                        &cypegl3d->datos_vista_perspectiva->x_centro_proyeccion, 
                        &cypegl3d->datos_vista_perspectiva->y_centro_proyeccion, 
                        &cypegl3d->datos_vista_perspectiva->z_plano_proyeccion);
}

// ----------------------------------------------------------------------------

void cypegl3d_inicio_dibujo_3D(struct cypegl3d_t *cypegl3d, enum cplangl_tipo_dibujo_t tipo_dibujo, int ancho_viewport_para_impresora, int alto_viewport_para_impresora) 
{
    unsigned long ancho_viewport, alto_viewport;

    i_integridad(cypegl3d);
    i_make_current(cypegl3d->manejador_opengl);

    i_actualiza_viewport(
                    cypegl3d->manejador_opengl, 
                    ancho_viewport_para_impresora, alto_viewport_para_impresora, 
                    &ancho_viewport, &alto_viewport);

    assert(arr_NumElemsPunteroST(cypegl3d->pila_tipos_dibujo, i_tipo_dibujo_t) == 0);

    cypegl3d->num_planos_adicionales_clipping_activos = 0;
    i_desactiva_todos_planos_adicionales_de_clipping();

    cypegl3d->tipo_dibujo_actual->tipo_dibujo_actual = tipo_dibujo;
    i_inicio_dibujo_segun_modelo(cypegl3d->tipo_dibujo_actual, NULL, cypegl3d->color_fondo_rgb);

    cypegl3d->ox_destino    = 0;
    cypegl3d->oy_destino    = 0;
    cypegl3d->ancho_destino = ancho_viewport;
    cypegl3d->alto_destino  = alto_viewport;
}

// ----------------------------------------------------------------------------

void cypegl3d_fin_dibujo(struct cypegl3d_t *cypegl3d)
{
    i_integridad(cypegl3d);
    i_make_current(cypegl3d->manejador_opengl);

    assert(arr_NumElemsPunteroST(cypegl3d->pila_tipos_dibujo, i_tipo_dibujo_t) == 0);

    glFlush();
    (void)GdiFlush();
    (void)SwapBuffers(cypegl3d->manejador_opengl->hdc_trabajo);

    if (cypegl3d->manejador_opengl->hdc_trabajo != cypegl3d->manejador_opengl->hdc_destino)
    {
        imagnwin_volcar_a_dispositivo(
                        cypegl3d->manejador_opengl->hdc_trabajo,
                        cypegl3d->manejador_opengl->hdc_destino,
                        cypegl3d->ox_destino, cypegl3d->oy_destino,
                        cypegl3d->ancho_destino, cypegl3d->alto_destino);
    }
}

// ----------------------------------------------------------------------------

static void i_pon_color_rgba(unsigned long color_rgba, double factor_transparencia)
{
    GLfloat color_ambient_and_diffuse[4];

    color_ambient_and_diffuse[0] = GetRValue(color_rgba) / 255.f;
    color_ambient_and_diffuse[1] = GetGValue(color_rgba) / 255.f;
    color_ambient_and_diffuse[2] = GetBValue(color_rgba) / 255.f;
    color_ambient_and_diffuse[3] = (float)(factor_transparencia * i_GetAValue(color_rgba) / 255.f);

    glColor4f(color_ambient_and_diffuse[0], color_ambient_and_diffuse[1], color_ambient_and_diffuse[2], color_ambient_and_diffuse[3]);
}

// ----------------------------------------------------------------------------
// Primitivas de dibujo
// ----------------------------------------------------------------------------

void cypegl3d_guardar(struct cypegl3d_t *cypegl3d)
{
    i_make_current(cypegl3d->manejador_opengl);

    glPushAttrib(GL_ALL_ATTRIB_BITS);
}

// ----------------------------------------------------------------------------

void cypegl3d_reponer(struct cypegl3d_t *cypegl3d)
{
    i_make_current(cypegl3d->manejador_opengl);

    glPopAttrib();
}

// ----------------------------------------------------------------------------

static void i_set_rgba(struct i_tipo_dibujo_t *tipo_dibujo, unsigned long color_rgba)
{
    assert_no_null(tipo_dibujo);

    switch(tipo_dibujo->tipo_dibujo_actual)
    {
        case CPLANGL_DIBUJO_CON_MUCHO_DETALLE:
        case CPLANGL_DIBUJO_CON_POCO_DETALLE:
        case CPLANGL_DIBUJO_SOMBRAS_SHADER:

            tipo_dibujo->color_rgba_actual = color_rgba;
            i_pon_color_rgba(tipo_dibujo->color_rgba_actual, tipo_dibujo->factor_transparencia);
            break;

        case CPLANGL_DIBUJO_LINEAS:
        case CPLANGL_GENERA_SOMBRAS_SHADER:

            break;

        default_error();
    }
}

// ----------------------------------------------------------------------------

void cypegl3d_set_color(struct cypegl3d_t *cypegl3d, unsigned char color)
{
    unsigned long color_rgb, color_rgba;

    i_integridad(cypegl3d);
    i_make_current(cypegl3d->manejador_opengl);

    color_rgb = idcolor_colorRGBSegunFondo(color, cypegl3d->color_fondo_rgb);
    color_rgba = color_rgb | (((unsigned long)255)<<24);

    i_set_rgba(cypegl3d->tipo_dibujo_actual, color_rgba);
}

// ----------------------------------------------------------------------------

void cypegl3d_set_color_rgba(struct cypegl3d_t *cypegl3d, unsigned long color_rgba)
{
    i_integridad(cypegl3d);
    i_make_current(cypegl3d->manejador_opengl);

    i_set_rgba(cypegl3d->tipo_dibujo_actual, color_rgba);
}

// ----------------------------------------------------------------------------

void cypegl3d_set_tipo_linea(struct cypegl3d_t *cypegl3d, const unsigned char tipo)
{
    i_make_current(cypegl3d->manejador_opengl);

    switch (tipo)
    {
        case 0:

            glDisable(GL_LINE_STIPPLE);
            break;

        case 1:

            glEnable(GL_LINE_STIPPLE);
            glLineStipple(2, 0x0C3F);
            break;

        case 2:

            glEnable(GL_LINE_STIPPLE);
            glLineStipple(2, 0x0FFF);
            break;

        case 3:

            glEnable(GL_LINE_STIPPLE);
            glLineStipple(1, 0x0F0F);
            break;

        case 4:

            glEnable(GL_LINE_STIPPLE);
            glLineStipple(2, 0x0FFF);
            break;

        case 5:

            glEnable(GL_LINE_STIPPLE);
            glLineStipple(2, 0x333F);
            break;

        case 6:

            glEnable(GL_LINE_STIPPLE);
            glLineStipple(2, 0x0C3F);
            break;

        case 7:

            glEnable(GL_LINE_STIPPLE);
            glLineStipple(2, 0x0F0F);
            break;

        default_error();
    }
}

// ----------------------------------------------------------------------------

void cypegl3d_linea3D(struct cypegl3d_t *cypegl3d, double x1, double y1, double z1, double x2, double y2, double z2)
{
    i_make_current(cypegl3d->manejador_opengl);

    glPushAttrib(GL_LIGHTING);
    glDisable(GL_LIGHTING);
    
    glBegin(GL_LINES);
   
    glVertex3d(x1, y1, z1);
    glVertex3d(x2, y2, z2);

    glEnd();

    cypegl3d->informe.num_puntos_con_detalle += 2;

    glPopAttrib();
}

// ----------------------------------------------------------------------------

static void i_setVertexs(const ArrPunto3D *puntos, const ArrULong *rgba, unsigned long color_fondo_rgb, unsigned long *num_puntos_totales)
{
    unsigned long i, num_puntos;

    num_puntos = arr_NumElemsPunto3D(puntos);
    if (rgba != NULL)
        assert(num_puntos == arr_NumElemsULong(rgba));

    for (i = 0; i < num_puntos; i++)
    {
        double x, y, z;

        if (rgba != NULL)
        {
            unsigned long color_rgba, color_rgba_final;

            color_rgba = arr_GetULong(rgba, i);

            if (color_rgba == 0xFFFFFFFF && color_fondo_rgb == 0x00FFFFFF)
                color_rgba_final = idcolor_rgba(0, 0, 0, 255);
            else
                color_rgba_final = color_rgba;

            i_pon_color_rgba(color_rgba_final, 1.);
        }

        arr_GetPunto3D(puntos, i, &x, &y, &z);
        glVertex3d(x, y, z);
    }

    if (num_puntos_totales != NULL)
        (*num_puntos_totales) += num_puntos;
}

// ----------------------------------------------------------------------------

void cypegl3d_polilinea_con_cambio_de_color(struct cypegl3d_t *cypegl3d, const ArrPunto3D *polilinea, const ArrULong *rgba)
{
    i_make_current(cypegl3d->manejador_opengl);
  
    glBegin(GL_LINE_STRIP);

    i_setVertexs(polilinea, rgba, cypegl3d->color_fondo_rgb, &cypegl3d->informe.num_puntos_con_detalle);

    glEnd();
}

// ----------------------------------------------------------------------------

void cypegl3d_polilinea3D(struct cypegl3d_t *cypegl3d, const ArrPunto3D *polilinea)
{
    i_make_current(cypegl3d->manejador_opengl);

    glPushAttrib(GL_LIGHTING);
    glDisable(GL_LIGHTING);
    
    glBegin(GL_LINE_STRIP);

    i_setVertexs(polilinea, NULL, cypegl3d->color_fondo_rgb, &cypegl3d->informe.num_puntos_con_detalle);

    glEnd();

    glPopAttrib();
}

// ----------------------------------------------------------------------------

void cypegl3d_poligono3D(struct cypegl3d_t *cypegl3d, const ArrPunto3D *poligono)
{
    i_make_current(cypegl3d->manejador_opengl);

    glPushAttrib(GL_LIGHTING);
    glPushAttrib(GL_CULL_FACE);
    glPushAttrib(GL_FRONT_AND_BACK);

    glDisable(GL_LIGHTING);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_CULL_FACE);

    glBegin(GL_POLYGON);
    i_setVertexs(poligono, NULL, cypegl3d->color_fondo_rgb, &cypegl3d->informe.num_puntos_con_detalle);
    glEnd();
    
    glPopAttrib();
    glPopAttrib();
    glPopAttrib();
}

// ----------------------------------------------------------------------------

static void i_set_flag_edge(CYBOOL es_visible)
{
    if (es_visible == CIERTO)
        glEdgeFlag(GL_TRUE);
    else
        glEdgeFlag(GL_FALSE);
}

// ----------------------------------------------------------------------------

static CYBOOL i_calcula_normal(
                        double x1, double y1, double z1,
                        double x2, double y2, double z2,
                        double x3, double y3, double z3,
                        double *Nx, double *Ny, double *Nz)
{
    CYBOOL es_triangulo;

    mt_producto_vectorial_3ptos(x1, y1, z1, x2, y2, z2, x3, y3, z3, Nx, Ny, Nz);

    if (mt_modulo_vector_3D(*Nx, *Ny, *Nz) > 0.00001)
    {
        es_triangulo = CIERTO;
        mt_pasar_a_unitario_3D(Nx, Ny, Nz);
    }
    else
        es_triangulo = FALSO;

    return es_triangulo;
}

// ----------------------------------------------------------------------------

void cypegl3d_quad(struct cypegl3d_t *cypegl3d, 
                        double x1, double y1, double z1, CYBOOL es_visible_arista1,
                        double x2, double y2, double z2, CYBOOL es_visible_arista2,
                        double x3, double y3, double z3, CYBOOL es_visible_arista3,
                        double x4, double y4, double z4, CYBOOL es_visible_arista4)
{
    double Nx1, Ny1, Nz1, Nx2, Ny2, Nz2;
    CYBOOL es_triangulo1, es_triangulo2;

    i_integridad(cypegl3d);
    i_make_current(cypegl3d->manejador_opengl);

    es_triangulo1 = i_calcula_normal(x1, y1, z1, x2, y2, z2, x3, y3, z3, &Nx1, &Ny1, &Nz1);
    es_triangulo2 = i_calcula_normal(x3, y3, z3, x4, y4, z4, x1, y1, z1, &Nx2, &Ny2, &Nz2);
    //assert(es_triangulo1 == CIERTO || es_triangulo2 == CIERTO);

    glBegin(GL_TRIANGLES);

    
    if (es_triangulo1 == CIERTO)
        glNormal3d(Nx1, Ny1, Nz1); 

    i_set_flag_edge(es_visible_arista1);
    glVertex3d(x1, y1, z1);

    i_set_flag_edge(es_visible_arista2);
    glVertex3d(x2, y2, z2);

    i_set_flag_edge(es_visible_arista3);
    glVertex3d(x3, y3, z3);

    if (es_triangulo2 == CIERTO)
        glNormal3d(Nx2, Ny2, Nz2); 

    i_set_flag_edge(es_visible_arista3);
    glVertex3d(x3, y3, z3);

    i_set_flag_edge(es_visible_arista4);
    glVertex3d(x4, y4, z4);

    i_set_flag_edge(es_visible_arista4);
    glVertex3d(x1, y1, z1);
    
    glEnd();
}

// ----------------------------------------------------------------------------

static void i_dibuja_texto(DWORD lista_de_letras, const WCHAR *texto)
{
    unsigned long long_texto;

    long_texto = widestr_longitud(texto);

    glListBase(lista_de_letras);
    glCallLists((int)long_texto, GL_UNSIGNED_SHORT, texto);  
}

// ----------------------------------------------------------------------------

static void i_calcula_posicion(
                        double longitud, double alto, unsigned char just,
                        double x, double y,
                        double *x_texto, double *y_texto)
{
    CYBOOL con_linea_base;

    assert_no_null(x_texto);
    assert_no_null(y_texto);
    assert(just >= 2 && just <= 15);

    if (just >= 2 && just <= 9)
        con_linea_base = FALSO;
    else
        con_linea_base = CIERTO;

    switch (just) 
    { 
        case 11:
        case 2:
        {
            *x_texto = x - longitud / 2.;
            *y_texto = y;
            break;
        }
        case 12:
        case 3:
        {
            *x_texto = x - longitud;
            *y_texto = y;
            break;
        }
        case 13:
        case 4:
        {
            *x_texto = x;
            *y_texto = y - alto / 2.;
            break;
        }
        case 14:
        case 5:
        {
            *x_texto = x - longitud / 2.;
            *y_texto = y - alto / 2.;
            break;
        }
        case 15:
        case 6:
        {
            *x_texto = x - longitud;
            *y_texto = y - alto / 2.;
            break;
        }
        case 7:
        {
            *x_texto = x;
            *y_texto = y - alto;
            break;
        }
        case 8:
        {
            *x_texto = x - longitud / 2.;
            *y_texto = y - alto;
            break;
        }
        case 9:
        {
            *x_texto = x - longitud;
            *y_texto = y - alto;
            break;
        }
        case 10:
        {
            *x_texto = x;
            *y_texto = y;
            break;
        }
        case 1:
        default_error(); 
    }

    if (con_linea_base == CIERTO)
        *y_texto -= alto * .2;
}

// ----------------------------------------------------------------------------

static double i_calcula_altura(struct i_font_opengl_t *font_normal, const WCHAR *texto, double alto)
{
    unsigned long i, num_car;
    double alto_maximo;

    assert_no_null(font_normal);

    alto_maximo = 0.;
    num_car = widestr_longitud(texto);

    for (i = 0; i < num_car; i++)
    {
        double alto_letra;
        GLYPHMETRICSFLOAT *agmf_letra;

        assert(texto[i] <= 255);

        agmf_letra = &(font_normal->agmf[texto[i]]);
        alto_letra = alto * agmf_letra->gmfBlackBoxY;
        alto_maximo = MAX(alto_letra, alto_maximo);
    }

    return alto_maximo;
}

// ----------------------------------------------------------------------------

void cypegl3d_dibuja_texto3D(
                        struct cypegl3d_t *cypegl3d,
                        const WCHAR *texto,
                        double x, double y, double z,
                        unsigned char just,
                        double alto,
                        double Ux, double Uy,
                        double longitud)
{
    double angulo_giro;
    double x_texto, y_texto;

    i_make_current(cypegl3d->manejador_opengl);
    assert_no_null(cypegl3d->font_normal);

    if (just == 1)
    {
        x_texto = x;
        y_texto = y;
    }
    else
    {
        double altura_texto;

        altura_texto = i_calcula_altura(cypegl3d->font_normal, texto, alto);
        i_calcula_posicion(longitud, altura_texto, just, x, y, &x_texto, &y_texto);
    }

    angulo_giro = mt_angulo(0., 0., Ux, Uy);

    glPushAttrib(GL_LIGHTING);
    glPushAttrib(GL_FRONT);

    glDisable(GL_LIGHTING);
    glPolygonMode(GL_FRONT, GL_FILL);

    glPushMatrix();

        glTranslated(x_texto, y_texto, z);
        glRotated(angulo_giro, 0., 0., 1.);

        glScaled(1.1 * alto, 1.1 * alto, 1.);

        i_dibuja_texto(cypegl3d->font_normal->lista_de_letras, texto);

    glPopMatrix();

    glPopAttrib();
    glPopAttrib();
}

// ----------------------------------------------------------------------------

void cypegl3d_longitud_trozo_texto(
                        struct cypegl3d_t *cypegl3d, 
                        const WCHAR *texto, 
                        unsigned short comienzo, unsigned short final, 
                        double alto, double *longitud_texto)
{
    unsigned long i;
    double longitud_loc;

    assert_no_null(cypegl3d);
    assert_no_null(cypegl3d->font_normal);
    assert_no_null(longitud_texto);

    longitud_loc = 0.;

    for (i = comienzo; i <= final; i++)
    {
        GLYPHMETRICSFLOAT *agmf_letra;

        assert(texto[i] < 255);
        agmf_letra = &(cypegl3d->font_normal->agmf[texto[i]]);
        longitud_loc += alto * agmf_letra->gmfCellIncX;
    }

    *longitud_texto = longitud_loc;
}

// ----------------------------------------------------------------------------

static GLUquadricObj *i_crea_quadric(enum cplangl_tipo_dibujo_t tipo_dibujo)
{
    GLUquadricObj *quadric;

    quadric = gluNewQuadric();

    switch (tipo_dibujo)
    {
        case CPLANGL_DIBUJO_CON_MUCHO_DETALLE:
        case CPLANGL_DIBUJO_SOMBRAS_SHADER:
        
            gluQuadricDrawStyle(quadric, GLU_FILL);
            gluQuadricNormals(quadric, GLU_SMOOTH);
            gluQuadricOrientation(quadric, GLU_OUTSIDE);
            gluQuadricTexture(quadric, GLU_FALSE);
            break;

        case CPLANGL_DIBUJO_CON_POCO_DETALLE:
        case CPLANGL_GENERA_SOMBRAS_SHADER:

            gluQuadricDrawStyle(quadric, GLU_FILL);
            gluQuadricNormals(quadric, GLU_FLAT);
            gluQuadricOrientation(quadric, GLU_OUTSIDE);
            gluQuadricTexture(quadric, GLU_FALSE);
            break;

        case CPLANGL_DIBUJO_LINEAS:

            gluQuadricDrawStyle(quadric, GLU_SILHOUETTE);
            gluQuadricNormals(quadric, GLU_FLAT);
            gluQuadricOrientation(quadric, GLU_OUTSIDE);
            gluQuadricTexture(quadric, GLU_FALSE);
            break;

        default_error();
    }

    return quadric;
}

// ----------------------------------------------------------------------------

static void i_punto_origen(GLUquadricObj *quadric)
{
    double radio_punto;

    radio_punto = 0.15;

    glColor3d(0.5, 0.5, 0.5);
    gluSphere(quadric, radio_punto, 10, 10);
}

// ----------------------------------------------------------------------------

static void i_color_eje(enum i_tipo_eje_t tipo_eje, double *r_color, double *g_color, double *b_color)
{
    assert_no_null(r_color);
    assert_no_null(g_color);
    assert_no_null(b_color);

    switch (tipo_eje) 
    { 
        case i_EJE_X:

            *r_color = 0.8;
            *g_color = 0.;
            *b_color = 0.;
            break;

        case i_EJE_Y:

            *r_color = 0.;
            *g_color = 0.;
            *b_color = 1.;
            break;

        case i_EJE_Z:

            *r_color = 0.;
            *g_color = 0.5;
            *b_color = 0;
            break;

        default_error(); 
    }
}

// ----------------------------------------------------------------------------

static void i_datos_eje(
                        enum i_tipo_eje_t tipo_eje, 
                        double *angulo_giro, double *Ux_giro, double *Uy_giro, double *Uz_giro,
                        double *r_color, double *g_color, double *b_color)
{
    assert_no_null(angulo_giro);

    assert_no_null(Ux_giro);
    assert_no_null(Uy_giro);
    assert_no_null(Uz_giro);

    assert_no_null(r_color);
    assert_no_null(g_color);
    assert_no_null(b_color);

    switch (tipo_eje) 
    { 
        case i_EJE_X:

            *angulo_giro = 90.;
            *Ux_giro = 0.;
            *Uy_giro = 1.;
            *Uz_giro = 0.;
            break;

        case i_EJE_Y:

            *angulo_giro = -90.;
            *Ux_giro = 1.;
            *Uy_giro = 0.;
            *Uz_giro = 0.;
            break;

        case i_EJE_Z:

            *angulo_giro = 0.;
            *Ux_giro = 0.;
            *Uy_giro = 0.;
            *Uz_giro = 1.;
            break;

        default_error(); 
    }

    i_color_eje(tipo_eje, r_color, g_color, b_color);
}

// ----------------------------------------------------------------------------

static void i_dibuja_tapa(
                        GLUquadricObj *quadric, 
                        unsigned long num_meridianos, unsigned long num_paralelos,
                        double radio_interior, double radio_exterior, 
                        CYBOOL es_tapa_base)
{
    glPushMatrix();

        if (es_tapa_base == CIERTO)
            glRotated(180., 1., 0., 0.);

        gluDisk(quadric, radio_interior, radio_exterior, (int)num_meridianos, (int)num_paralelos);

    glPopMatrix();
}

// ----------------------------------------------------------------------------

static void i_dibujo_eje(
                        GLUquadricObj *quadric,
                        double longitud_ejes, double longitud_punta)
{
    double radio_eje, radio_punta;

    radio_eje = 0.05;
    radio_punta = 0.15;

    gluCylinder(quadric, radio_eje, radio_eje, longitud_ejes, i_MERIDIANOS_CILINDRO, i_PARALELOS_CILINDRO);

    glTranslated(0., 0., longitud_ejes);
    gluCylinder(quadric, radio_punta, 0., longitud_punta, i_MERIDIANOS_CILINDRO, i_PARALELOS_CILINDRO);

    i_dibuja_tapa(quadric, i_MERIDIANOS_CILINDRO, 2, radio_eje, radio_punta, CIERTO);
}

// ----------------------------------------------------------------------------

static void i_dibuja_y_posiciona_eje(
                        GLUquadricObj *quadric, 
                        enum i_tipo_eje_t tipo_eje, 
                        double longitud_ejes, double longitud_punta)
{
    double angulo_giro, Ux_giro, Uy_giro, Uz_giro;
    double r_color, g_color, b_color;
     
    i_datos_eje(tipo_eje, &angulo_giro, &Ux_giro, &Uy_giro, &Uz_giro, &r_color, &g_color, &b_color);

    glPushMatrix();

        glRotated(angulo_giro, Ux_giro, Uy_giro, Uz_giro);
            
            glColor4f((float)r_color, (float)g_color, (float)b_color, (float)0.5);
            i_dibujo_eje(quadric, longitud_ejes, longitud_punta);
 
    glPopMatrix();
}

// ----------------------------------------------------------------------------

static void i_dibuja_ejes3d(enum cplangl_tipo_dibujo_t tipo_dibujo, double longitud_ejes, double longitud_punta)
{
    GLUquadricObj *quadric;

    quadric = i_crea_quadric(tipo_dibujo);

    i_punto_origen(quadric);

    i_dibuja_y_posiciona_eje(quadric, i_EJE_X, longitud_ejes, longitud_punta);
    i_dibuja_y_posiciona_eje(quadric, i_EJE_Y, longitud_ejes, longitud_punta);
    i_dibuja_y_posiciona_eje(quadric, i_EJE_Z, longitud_ejes, longitud_punta);

    gluDeleteQuadric(quadric);
}

// ----------------------------------------------------------------------------

static void i_posiciona_luces_sin_sombras(
                        double xmin, double ymin, double zmin,
                        double xmax, double ymax, double zmax)
{
    double tamx, tamy, tamz;
    GLfloat positionLight1[4], positionLight2[4];

    tamx = 1.2 * (xmax - xmin);
    tamy = 1.2 * (ymax - ymin); 
    tamz = 1.2 * (zmax - zmin);

    positionLight1[0] = (float)-(0.5 * tamx);
    positionLight1[1] = (float)(-0.6 * tamy);
    positionLight1[2] = (float)(1.2 * tamz);
    positionLight1[3] = (float)1.;

    positionLight2[0] = (float)(-0.2 * tamx);
    positionLight2[1] = (float)(-0.6 * tamy);
    positionLight2[2] = (float)(1.2 * tamz);
    positionLight2[3] = (float)1.;

    glPushMatrix();
    glLoadIdentity();

    i_cambia_ejes_cype();

    glLightfv(GL_LIGHT0, GL_POSITION, positionLight1);
    glLightfv(GL_LIGHT1, GL_POSITION, positionLight2);

    glPopMatrix();
}

// ----------------------------------------------------------------------------

static void i_posiciona_luces_con_sombras(
                        double Ux_luz, double Uy_luz, double Uz_luz,
                        double xmin, double ymin, double zmin,
                        double xmax, double ymax, double zmax)
{
    GLfloat positionLight[4];
    double x_pos_luz, y_pos_luz, z_pos_luz;

    x_pos_luz = (xmin + xmax) / 2. + Ux_luz * 1000.;
    y_pos_luz = (ymin + ymax) / 2. + Uy_luz * 1000.;
    z_pos_luz = (zmin + zmax) / 2. + Uz_luz * 1000.;

    positionLight[0] = (float)x_pos_luz;
    positionLight[1] = (float)y_pos_luz;
    positionLight[2] = (float)z_pos_luz;
    positionLight[3] = (float)0.;

    glLightfv(GL_LIGHT0, GL_POSITION, positionLight);
}

// ----------------------------------------------------------------------------

void cypegl3d_posiciona_luces(
                        struct cypegl3d_t *cypegl3d,
                        double xmin, double ymin, double zmin,
                        double xmax, double ymax, double zmax)
{
    assert_no_null(cypegl3d);
    assert_no_null(cypegl3d->tipo_dibujo_actual);

    i_make_current(cypegl3d->manejador_opengl);

    switch(cypegl3d->tipo_dibujo_actual->tipo_dibujo_actual)
    {
        case CPLANGL_DIBUJO_CON_MUCHO_DETALLE:
        case CPLANGL_DIBUJO_CON_POCO_DETALLE:
        case CPLANGL_DIBUJO_LINEAS:

            i_posiciona_luces_sin_sombras(xmin, ymin, zmin, xmax, ymax, zmax);
            break;

        case CPLANGL_DIBUJO_SOMBRAS_SHADER:
        case CPLANGL_GENERA_SOMBRAS_SHADER:
        default_error();
    }
}

// ----------------------------------------------------------------------------

void cypegl3d_posiciona_luces_dia(
                        struct cypegl3d_t *cypegl3d,
                        CYBOOL es_de_dia,
                        double Ux_luz, double Uy_luz, double Uz_luz,
                        double xmin, double ymin, double zmin,
                        double xmax, double ymax, double zmax)
{
    assert_no_null(cypegl3d);
    assert_no_null(cypegl3d->tipo_dibujo_actual);

    i_make_current(cypegl3d->manejador_opengl);

    switch(cypegl3d->tipo_dibujo_actual->tipo_dibujo_actual)
    {
        case CPLANGL_DIBUJO_SOMBRAS_SHADER:
        {
            if (es_de_dia == CIERTO)
                i_define_luz_dia();
            else
                i_define_luz_noche();

            cyshadow_es_de_dia_o_de_noche(cypegl3d->shader_sombra, es_de_dia);
            i_posiciona_luces_con_sombras(Ux_luz, Uy_luz, Uz_luz, xmin, ymin, zmin, xmax, ymax, zmax);
            break;
        }
        case CPLANGL_GENERA_SOMBRAS_SHADER:
        case CPLANGL_DIBUJO_CON_MUCHO_DETALLE:
        case CPLANGL_DIBUJO_CON_POCO_DETALLE:
        case CPLANGL_DIBUJO_LINEAS:
        default_error();
    }
}

// ----------------------------------------------------------------------------

static void i_set_relleno(CYBOOL relleno)
{
    if (relleno == CIERTO)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

// ----------------------------------------------------------------------------

void cypegl3d_circulo(struct cypegl3d_t *cypegl3d, double xc, double yc, double zc, double radio, CYBOOL relleno)
{
    GLUquadricObj *circulo;

    i_integridad(cypegl3d);
    i_make_current(cypegl3d->manejador_opengl);

    glPushMatrix();

        glTranslated(xc, yc, zc);

        circulo = i_crea_quadric(cypegl3d->tipo_dibujo_actual->tipo_dibujo_actual);
        i_set_relleno(relleno);

        gluDisk(circulo, 0., radio, 10, 10);

        gluDeleteQuadric(circulo);
 
    glPopMatrix();
}

// ----------------------------------------------------------------------------

static void i_parametros_arco(
                        double xc, double yc, double zc, 
                        double x1, double y1, double z1, 
                        double x2, double y2, double z2, 
                        double *radio, double *angulo_inicial, double *angulo_final)
{
    double Xo, Yo, Zo;
    double Ux, Uy, Uz;
    double Vx, Vy, Vz;
    double xc2d, yc2d, x12d, y12d, x22d, y22d;
    double precision, radio1, radio2;

    assert_no_null(radio);
    assert_no_null(angulo_inicial);
    assert_no_null(angulo_final);

    precision = .0000001;

    ejes2d_ejes_con_tres_puntos(
                        xc, yc, zc,
                        x1, y1, z1,
                        x2, y2, z2,
                        &Xo, &Yo, &Zo,
                        &Ux, &Uy, &Uz,
                        &Vx, &Vy, &Vz);

    ejes2d_coordenadas_3D_a_2D(
                        Xo, Yo, Zo, Ux, Uy, Uz, Vx, Vy, Vz,
                        xc, yc, zc, precision, &xc2d, &yc2d);

    ejes2d_coordenadas_3D_a_2D(
                        Xo, Yo, Zo, Ux, Uy, Uz, Vx, Vy, Vz,
                        x1, y1, z1, precision, &x12d, &y12d);

    ejes2d_coordenadas_3D_a_2D(
                        Xo, Yo, Zo, Ux, Uy, Uz, Vx, Vy, Vz,
                        x2, y2, z2, precision, &x22d, &y22d);

    radio1 = mt_distancia(xc2d, yc2d, x12d, y12d);
    radio2 = mt_distancia(xc2d, yc2d, x22d, y22d);

    assert(fabs(radio1 - radio2) < precision);

    *radio = radio1;
    *angulo_inicial = mt_angulo(xc2d, yc2d, x12d, y12d);
    *angulo_final = mt_angulo(xc2d, yc2d, x22d, y22d);
}

// ----------------------------------------------------------------------------

void cypegl3d_arco3D(
                    struct cypegl3d_t *cypegl3d, 
                    double xc, double yc, double zc, 
                    double x1, double y1, double z1, 
                    double x2, double y2, double z2, 
                    CYBOOL relleno)
{
    GLUquadricObj *arco;
    double radio, angulo_inicial, angulo_final, diff_angulo;

    i_integridad(cypegl3d);
    i_make_current(cypegl3d->manejador_opengl);

    glPushMatrix();

    glTranslated(xc, yc, zc);

        arco = i_crea_quadric(cypegl3d->tipo_dibujo_actual->tipo_dibujo_actual);

        i_parametros_arco(xc, yc, zc, x1, y1, z1, x2, y2, z2, &radio, &angulo_inicial, &angulo_final);

        diff_angulo = angulo_final - angulo_inicial;

        i_set_relleno(relleno);

        gluPartialDisk(
                arco,
                0.,
                radio,
                10, 10,
                angulo_inicial * 180. / PI,
                diff_angulo * 180. / PI);

        gluDeleteQuadric(arco);

    glPopMatrix();
}

// ----------------------------------------------------------------------------

void cypegl3d_elipse(
                    struct cypegl3d_t *cypegl3d, 
                    double xc, double yc, double zc, 
                    double ra, double rb, 
                    double angulo_rotacion, CYBOOL relleno)
{
    GLUquadricObj *circulo;

    i_integridad(cypegl3d);
    i_make_current(cypegl3d->manejador_opengl);

    glPushMatrix();

        glTranslated(xc, yc, zc);

        i_set_relleno(relleno);

        glRotated(angulo_rotacion * 180. / PI, 0., 0., 1.);
        glScaled(ra, rb, 1.);

        circulo = i_crea_quadric(cypegl3d->tipo_dibujo_actual->tipo_dibujo_actual);
        gluDisk(circulo, 0., 1., 10, 10);
        gluDeleteQuadric(circulo);

    glPopMatrix();
}

// ----------------------------------------------------------------------------

void cypegl3d_triangulo3D_orientado(
                    struct cypegl3d_t *cypegl3d,
                    double x1, double y1, double z1, double Wx1, double Wy1, double Wz1,
                    double x2, double y2, double z2, double Wx2, double Wy2, double Wz2,
                    double x3, double y3, double z3, double Wx3, double Wy3, double Wz3)
{
    i_make_current(cypegl3d->manejador_opengl);

    glBegin(GL_TRIANGLES);

    glNormal3d(Wx1, Wy1, Wz1); 
    glVertex3d(x1, y1, z1);

    glNormal3d(Wx2, Wy2, Wz2); 
    glVertex3d(x2, y2, z2);

    glNormal3d(Wx3, Wy3, Wz3); 
    glVertex3d(x3, y3, z3);

    cypegl3d->informe.num_puntos_con_detalle += 3;

    glEnd();
}


// ----------------------------------------------------------------------------

static CYBOOL i_es_color_transparente(enum cplangl_tipo_dibujo_t tipo_dibujo_actual, unsigned long rgba)
{
    CYBOOL es_color_transparente;

    switch(tipo_dibujo_actual)
    {
        case CPLANGL_DIBUJO_CON_MUCHO_DETALLE:
        case CPLANGL_DIBUJO_CON_POCO_DETALLE:
        case CPLANGL_DIBUJO_SOMBRAS_SHADER:
        {
            unsigned char alfa;

            alfa = i_GetAValue(rgba);

            if (alfa == 255)
                es_color_transparente = FALSO;
            else
                es_color_transparente = CIERTO;
            break;
        }
        case CPLANGL_DIBUJO_LINEAS:
        case CPLANGL_GENERA_SOMBRAS_SHADER:

            es_color_transparente = FALSO;
            break;

        default_error();
    }
    
    return es_color_transparente;
}

// ----------------------------------------------------------------------------

static void i_dibuja_mesh(
                        const ArrPunto3D *puntos,
                        const ArrPunto3D *normales,
                        const ArrPunto2D *coord_texturas_opc,
                        const ArrBool *es_borde,
                        const ArrEnum(cplan_tipo_primitiva_t) *tipos_primitivas,
                        const ArrPuntero(ArrULong) *inds_todas_las_caras,
                        unsigned long *num_puntos_dibujo)
{
    struct i_buffers_puntos_t *buffer;

    buffer = i_crea_buffers_puntos_defecto(
                        tipos_primitivas, inds_todas_las_caras,
                        puntos, normales, coord_texturas_opc, es_borde, num_puntos_dibujo);

    i_dibuja_buffer(buffer);

    i_destruye_buffers_puntos(&buffer);
}

// ----------------------------------------------------------------------------

static void i_append_buffer_destruyendo(
                        ArrPunto3D **puntos,
                        ArrPunto3D **normales,
                        ArrPunto2D **coord_texturas_opc,
                        ArrBool **es_borde,
                        ArrEnum(cplan_tipo_primitiva_t) **tipos_primitivas,
                        ArrPuntero(ArrULong) **inds_todas_las_caras,
                        unsigned long *num_puntos_dibujo)
{
    assert_no_null(coord_texturas_opc);
    assert_no_null(puntos);
    assert_no_null(normales);
    assert_no_null(es_borde);
    assert_no_null(tipos_primitivas);
    assert_no_null(inds_todas_las_caras);

    i_dibuja_mesh(
                *puntos, *normales, *coord_texturas_opc,
                *es_borde, *tipos_primitivas, *inds_todas_las_caras, num_puntos_dibujo);

    arr_DestruyePunto3D(puntos);
    arr_DestruyePunto3D(normales);

    if (*coord_texturas_opc != NULL)
        arr_DestruyePunto2D(coord_texturas_opc);

    arr_DestruyeBOOL(es_borde);
    
    arr_DestruyeEnum(tipos_primitivas, cplan_tipo_primitiva_t);
    arr_DestruyeEstructurasTD(inds_todas_las_caras, ArrULong, arr_DestruyeULong);
}

// ----------------------------------------------------------------------------

void cypegl3d_bandas_triangulos(
                    struct cypegl3d_t *cypegl3d,
                    const ArrPunto3D *puntos,
                    const ArrPunto3D *normales,
                    const ArrPunto2D *coord_texturas,
                    const ArrBool *es_borde,
                    const ArrEnum(cplan_tipo_primitiva_t) *tipos_primitivas,
                    const ArrPuntero(ArrULong) *inds_todas_las_caras)
{
    unsigned long *num_puntos_primitivas;

    i_make_current(cypegl3d->manejador_opengl);

    switch(cypegl3d->tipo_lista_dibujo)
    {
        case CPLANGL_LISTA_CON_MUCHO_DETALLE:
        case CPLANGL_LISTA_PLACAS_CON_MUCHO_DETALLE:

            num_puntos_primitivas = &cypegl3d->informe.num_puntos_con_detalle;
            break;

        case CPLANGL_LISTA_CON_POCO_DETALLE:
        case CPLANGL_LISTA_PLACAS_CON_POCO_DETALLE:

            num_puntos_primitivas = &cypegl3d->informe.num_puntos_sin_detalle;
            break;

        default_error();
    }

    i_dibuja_mesh(puntos, normales, coord_texturas, es_borde, tipos_primitivas, inds_todas_las_caras, num_puntos_primitivas);
}

// ----------------------------------------------------------------------------

static void i_aplica_vector_plano(double Nx, double Ny, double Nz)
{
    double Ux, Uy, Uz, Vx, Vy, Vz;

    cypemath_calcula_matriz_transformacion_autocad(Nx, Ny, Nz, &Ux, &Uy, &Uz, &Vx, &Vy, &Vz);
    cypegl3d_set_matriz_transformacion(Ux, Uy, Uz, Vx, Vy, Vz, Nx, Ny, Nz, 0., 0., 0.);
}

// ----------------------------------------------------------------------------

static enum cplangl_tipo_dibujo_t i_tipo_dibujo_segun_tipo_lista(enum cplangl_tipo_lista_dibujo_t tipo_lista_dibujo)
{
    switch(tipo_lista_dibujo)
    {
        case CPLANGL_LISTA_CON_MUCHO_DETALLE:
        case CPLANGL_LISTA_PLACAS_CON_MUCHO_DETALLE:

            return CPLANGL_DIBUJO_CON_MUCHO_DETALLE;

        case CPLANGL_LISTA_CON_POCO_DETALLE:
        case CPLANGL_LISTA_PLACAS_CON_POCO_DETALLE:

            return CPLANGL_DIBUJO_CON_POCO_DETALLE;

        default_error();
    }
}

// ----------------------------------------------------------------------------

static enum cplangl_tipo_dibujo_t i_calcula_tipo_dibujo(
                        enum cplangl_tipo_dibujo_t tipo_dibujo,
                        char *id_lista_activa,
                        enum cplangl_tipo_lista_dibujo_t tipo_lista_dibujo)
{
    if (id_lista_activa != NULL)
        return i_tipo_dibujo_segun_tipo_lista(tipo_lista_dibujo);
    else
        return tipo_dibujo;
}

// ----------------------------------------------------------------------------

static void i_dibuja_tapa_circular(
                        GLUquadricObj *quadric,
                        double x_centro, double y_centro, double z_centro, double radio,
                        double Nx, double Ny, double Nz, 
                        unsigned long num_puntos_circulo,
                        CYBOOL es_tapa_base)
{
    glPushMatrix();

    glTranslated(x_centro, y_centro, z_centro);
    i_aplica_vector_plano(Nx, Ny, Nz);

    glRotated(90., 0., 0., 1.);

    i_dibuja_tapa(quadric, num_puntos_circulo, 2, 0., radio, es_tapa_base);

    glPopMatrix();
}

// ----------------------------------------------------------------------------

static void i_dibuja_tapa_tubo(GLUquadricObj *quadric, const ArrPunto3D *puntos, double radio, unsigned long num_puntos_circulo, CYBOOL es_inicio)
{
    double x1, y1, z1, x2, y2, z2;
    double x_centro, y_centro, z_centro;
    double Nx, Ny, Nz;
    unsigned long num_puntos;

    num_puntos = arr_NumElemsPunto3D(puntos);
    assert(num_puntos > 1);

    if (es_inicio == CIERTO)
    {
        arr_GetPunto3D(puntos, 0, &x1, &y1, &z1);
        arr_GetPunto3D(puntos, 1, &x2, &y2, &z2);

        x_centro = x1;
        y_centro = y1;
        z_centro = z1;
    }
    else
    {
        arr_GetPunto3D(puntos, num_puntos - 2, &x1, &y1, &z1);
        arr_GetPunto3D(puntos, num_puntos - 1, &x2, &y2, &z2);

        x_centro = x2;
        y_centro = y2;
        z_centro = z2;
    }

    mt_vector_director_de_dos_puntos_3D(x1, y1, z1, x2, y2, z2, &Nx, &Ny, &Nz);
    i_dibuja_tapa_circular(quadric, x_centro, y_centro, z_centro, radio, Nx, Ny, Nz, num_puntos_circulo, es_inicio);
}

// ----------------------------------------------------------------------------

static unsigned long i_calcula_num_puntos_circulo(enum cplangl_tipo_dibujo_t tipo_dibujo, double radio)
{
    switch (tipo_dibujo)
    { 
        case CPLANGL_DIBUJO_CON_MUCHO_DETALLE:
        case CPLANGL_DIBUJO_SOMBRAS_SHADER:

            if (radio < 0.01)
                return i_MERIDIANOS_CILINDRO;
            else
                return i_MERIDIANOS_CILINDRO_DETALLADO;

        case CPLANGL_DIBUJO_CON_POCO_DETALLE:
        case CPLANGL_DIBUJO_LINEAS:
        case CPLANGL_GENERA_SOMBRAS_SHADER:

            return i_MERIDIANOS_CILINDRO;

        default_error(); 
    }
}

// ----------------------------------------------------------------------------

static void i_tubos_en_detalle(const ArrPunto3D *puntos, double diametro, unsigned long *num_puntos_dibujo)
{
    enum cplangl_tipo_dibujo_t tipo_dibujo;
    GLUquadricObj *quadric;
    double radio;
    unsigned long num_puntos_circulo;
    ArrPunto3D *puntos_prisma;
    ArrPunto3D *normales_prisma;
    ArrPunto2D *coord_texturas_opc;
    ArrBool *es_borde_prisma;
    ArrEnum(cplan_tipo_primitiva_t) *tipos_primitivas_prisma;
    ArrPuntero(ArrULong) *inds_caras_prisma;

    tipo_dibujo = CPLANGL_DIBUJO_CON_MUCHO_DETALLE;
    radio = diametro / 2.;

    num_puntos_circulo = 10;

    cypemesh_genera_prisma_tubos(
                        puntos, radio, num_puntos_circulo, 
                        &puntos_prisma, &normales_prisma, &es_borde_prisma,
                        &tipos_primitivas_prisma, &inds_caras_prisma);

    coord_texturas_opc = NULL;
    i_append_buffer_destruyendo(
                        &puntos_prisma, &normales_prisma, &coord_texturas_opc, &es_borde_prisma,
                        &tipos_primitivas_prisma, &inds_caras_prisma, num_puntos_dibujo);

    quadric = i_crea_quadric(tipo_dibujo);

    i_dibuja_tapa_tubo(quadric, puntos, radio, num_puntos_circulo, CIERTO);
    i_dibuja_tapa_tubo(quadric, puntos, radio, num_puntos_circulo, FALSO);

    gluDeleteQuadric(quadric);
}

// ----------------------------------------------------------------------------

static void i_tubos_simples(const ArrPunto3D *puntos, unsigned long *num_puntos_sin_detalle)
{
    glBegin(GL_LINE_STRIP);
    i_setVertexs(puntos, NULL, ULONG_MAX, num_puntos_sin_detalle);
    glEnd();
}

// ----------------------------------------------------------------------------

void cypegl3d_dibuja_tubos(struct cypegl3d_t *cypegl3d, const ArrPunto3D *puntos, double diametro)
{
    enum cplangl_tipo_dibujo_t tipo_dibujo;

    i_integridad(cypegl3d);
    i_make_current(cypegl3d->manejador_opengl);
    assert_no_null(cypegl3d->tipo_dibujo_actual);

    tipo_dibujo = i_calcula_tipo_dibujo(
                        cypegl3d->tipo_dibujo_actual->tipo_dibujo_actual, 
                        cypegl3d->id_lista_activa,
                        cypegl3d->tipo_lista_dibujo);

    switch (tipo_dibujo) 
    { 
        case CPLANGL_DIBUJO_CON_MUCHO_DETALLE:
        case CPLANGL_DIBUJO_SOMBRAS_SHADER:
        
            i_tubos_en_detalle(puntos, diametro, &cypegl3d->informe.num_puntos_con_detalle);
            break;
        
        case CPLANGL_DIBUJO_CON_POCO_DETALLE:
        
            i_tubos_simples(puntos, &cypegl3d->informe.num_puntos_sin_detalle);
            break;

        case CPLANGL_GENERA_SOMBRAS_SHADER:

            break;

        case CPLANGL_DIBUJO_LINEAS:
        default_error(); 
    }
}

// ----------------------------------------------------------------------------

void cypegl3d_dibuja_prisma_conico(
                        struct cypegl3d_t *cypegl3d,
                        double x_centro_inf, double y_centro_inf, double z_centro_inf,
                        double diametro_inferior,
                        double x_centro_sup, double y_centro_sup, double z_centro_sup,
                        double diametro_superior,
                        double Nx, double Ny, double Nz)
{
    GLUquadricObj *quadric;
    enum cplangl_tipo_dibujo_t tipo_dibujo;
    ArrPunto3D *puntos, *normales;
    ArrPunto2D *coord_texturas;
    ArrBool *es_borde;
    ArrEnum(cplan_tipo_primitiva_t) *tipos_primitivas;
    ArrPuntero(ArrULong) *inds_caras;
    unsigned long num_puntos_circulo;
    double radio_inf, radio_sup, radio_maximo;

    i_integridad(cypegl3d);
    i_make_current(cypegl3d->manejador_opengl);

    tipo_dibujo = i_calcula_tipo_dibujo(
                        cypegl3d->tipo_dibujo_actual->tipo_dibujo_actual,
                        cypegl3d->id_lista_activa,
                        cypegl3d->tipo_lista_dibujo);

    radio_inf = diametro_inferior / 2.;
    radio_sup = diametro_superior / 2.;

    radio_maximo = MAX(radio_inf, radio_sup);
    num_puntos_circulo = i_calcula_num_puntos_circulo(tipo_dibujo, radio_maximo);

    cypemesh_genera_prisma_conico(
                        x_centro_inf, y_centro_inf, z_centro_inf, radio_inf,
                        x_centro_sup, y_centro_sup, z_centro_sup, radio_sup,
                        Nx, Ny, Nz,
                        num_puntos_circulo,
                        &puntos, &normales, &coord_texturas, &es_borde, &tipos_primitivas, &inds_caras);

    i_append_buffer_destruyendo(
                        &puntos, &normales, &coord_texturas, &es_borde, &tipos_primitivas, &inds_caras, 
                        &cypegl3d->informe.num_puntos_con_detalle);

    quadric = i_crea_quadric(tipo_dibujo);

    if (radio_inf > 0.000001)
        i_dibuja_tapa_circular(quadric, x_centro_inf, y_centro_inf, z_centro_inf, radio_inf, Nx, Ny, Nz, num_puntos_circulo, CIERTO);

    if (radio_sup > 0.000001)
        i_dibuja_tapa_circular(quadric, x_centro_sup, y_centro_sup, z_centro_sup, radio_sup, Nx, Ny, Nz, num_puntos_circulo, FALSO);

    gluDeleteQuadric(quadric);
}

// ----------------------------------------------------------------------------

void cypegl3d_triangulo_gradiente_3D(
                    struct cypegl3d_t *cypegl3d,
                    double x1, double y1, double z1, unsigned long rgba1,
                    double x2, double y2, double z2, unsigned long rgba2,
                    double x3, double y3, double z3, unsigned long rgba3)
{
    i_make_current(cypegl3d->manejador_opengl);

    glPushAttrib(GL_LIGHTING);
    glPushAttrib(GL_CULL_FACE);
    glPushAttrib(GL_FRONT_AND_BACK);

    glDisable(GL_LIGHTING);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_CULL_FACE);

    glBegin(GL_TRIANGLES);

    i_pon_color_rgba(rgba1, 1.);
    glVertex3d(x1, y1, z1);
    i_pon_color_rgba(rgba2, 1.);
    glVertex3d(x2, y2, z2);
    i_pon_color_rgba(rgba3, 1.);
    glVertex3d(x3, y3, z3);

    cypegl3d->informe.num_puntos_con_detalle += 3;                

    glEnd();

    glPopAttrib();
    glPopAttrib();
    glPopAttrib();
}

// ----------------------------------------------------------------------------

void cypegl3d_inicia_lista_dibujo(
                    struct cypegl3d_t *cypegl3d, 
                    enum cplangl_tipo_lista_dibujo_t tipo_lista_dibujo,
                    const char *id_lista)
{
    struct i_lista_dibujo_t *lista_dibujo;
    CYBOOL existe;
    unsigned long ind_encontrado;

    i_integridad(cypegl3d);
    i_make_current(cypegl3d->manejador_opengl);

    assert(cypegl3d->id_lista_activa == NULL);

    existe = apunorde_existe(cypegl3d->lista_dibujos, i_lista_dibujo_t, id_lista, char, i_busca_lista_dibujo, &ind_encontrado);
    
    if (existe == CIERTO)
        lista_dibujo = apunorde_get(cypegl3d->lista_dibujos, ind_encontrado, i_lista_dibujo_t);
    else
    {
        lista_dibujo = i_crea_lista_dibujo_defecto(id_lista);
        apunorde_inserta_original(cypegl3d->lista_dibujos, lista_dibujo, i_lista_dibujo_t);
    }
    
    cypegl3d->tipo_lista_dibujo = tipo_lista_dibujo;
    i_activa_lista_opengl(lista_dibujo, tipo_lista_dibujo);
    
    cypegl3d->id_lista_activa = cad_copia_cadena(id_lista);
}

// ----------------------------------------------------------------------------

void cypegl3d_fin_lista_dibujo_activo(struct cypegl3d_t *cypegl3d, const char *id_lista)
{
    i_integridad(cypegl3d);
    i_make_current(cypegl3d->manejador_opengl);
    assert_no_null(cypegl3d->id_lista_activa);
    assert(cad_cadenas_iguales(cypegl3d->id_lista_activa, id_lista) == CIERTO);

    glEndList();

    cypestr_destruye(&cypegl3d->id_lista_activa);
}

// ----------------------------------------------------------------------------

static void i_set_tipo_modelado(enum cplangl_tipo_modelado_t tipo_modelado)
{
    switch (tipo_modelado)
    { 
        case CPLANGL_MODELADO_SOLIDO:

            glPolygonMode(GL_FRONT, GL_FILL);
            break;

        case CPLANGL_MODELADO_ALAMBRICO:

            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;

        default_error(); 
    }
}

// ----------------------------------------------------------------------------

static void i_set_tipo_modelado_placa(enum cplangl_tipo_modelado_t tipo_modelado)
{
    switch(tipo_modelado)
    {
        case CPLANGL_MODELADO_SOLIDO:

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;

        case CPLANGL_MODELADO_ALAMBRICO:

            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;

        default_error();
    }
}

// ----------------------------------------------------------------------------

static void i_dibuja_lista(unsigned long id_lista, GLenum tipo_offset, float factor_offset, float units)
{
    glPushAttrib(tipo_offset);
    glEnable(tipo_offset);
    glPolygonOffset(factor_offset, units);

    glCallList(id_lista);

    glPopAttrib();
}

// ----------------------------------------------------------------------------

static void i_dibuja_lista_ambas_caras(
                        unsigned long id_lista, 
                        GLenum tipo_offset, float factor_offset, float units)
{
    glPushAttrib(GL_CULL_FACE);
    glDisable(GL_CULL_FACE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    i_dibuja_lista(id_lista, tipo_offset, factor_offset, units);
    
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
    glPopAttrib();
}

// ----------------------------------------------------------------------------

static void i_dibuja_placas_mucho_detalle(
                        enum cplangl_tipo_modelado_t tipo_modelado,
                        CYBOOL con_transparencia,
                        unsigned long id_lista_placas,
                        float factor_offset, float units)
{
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    if (con_transparencia == CIERTO)
        glDepthMask(GL_FALSE);

    i_set_tipo_modelado_placa(tipo_modelado);

    i_dibuja_lista_ambas_caras(id_lista_placas, GL_POLYGON_OFFSET_FILL, factor_offset, units);

    if (con_transparencia == CIERTO)
    {
        glDepthMask(GL_TRUE);
        glDisable(GL_LIGHTING);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glCallList(id_lista_placas);
    }

    i_set_tipo_modelado(tipo_modelado);

    glPopAttrib();
}

// ----------------------------------------------------------------------------

static void i_calcula_offset_opengl(
                        enum cplangl_tipo_dibujo_t tipo_dibujo_actual,
                        enum cplangl_offset_zbuffer_t offset_zbuffer,
                        float *factor_offset, float *units)
{
    double factor_offset_loc, units_loc;
    double coef_nivel, distancia_entre_niveles;

    assert_no_null(factor_offset);
    assert_no_null(units);

    distancia_entre_niveles = 0.5;

    switch(offset_zbuffer)
    {
        case CPLANGL_NIVEL_0:

            coef_nivel = 0.;
            break;

        case CPLANGL_NIVEL_1:

            coef_nivel = 1.;
            break;

        case CPLANGL_NIVEL_2:

            coef_nivel = 2.;
            break;

        case CPLANGL_NIVEL_3:

            coef_nivel = 3.;
            break;

        default_error();
    }

    switch (tipo_dibujo_actual) 
    {
        case CPLANGL_DIBUJO_CON_MUCHO_DETALLE:
        case CPLANGL_DIBUJO_CON_POCO_DETALLE:
        case CPLANGL_DIBUJO_SOMBRAS_SHADER:
        {
            factor_offset_loc = coef_nivel * distancia_entre_niveles + .1;
            units_loc = factor_offset_loc;
            break;
        }
        case CPLANGL_DIBUJO_LINEAS:
        {
            factor_offset_loc = coef_nivel * distancia_entre_niveles + .1 - .5 * distancia_entre_niveles;
            units_loc = factor_offset_loc;
            break;
        }
        case CPLANGL_GENERA_SOMBRAS_SHADER:
        {
            factor_offset_loc = 1.1;
            units_loc = 4.;
            break;
        }
        default_error();
    }

    *factor_offset = cambtipo_double_a_float(factor_offset_loc);
    *units = cambtipo_double_a_float(units_loc);
}

// ----------------------------------------------------------------------------

static void i_dibuja_lista_dibujo(
                        enum cplangl_tipo_dibujo_t tipo_dibujo_actual,
                        enum cplangl_tipo_modelado_t tipo_modelado, 
                        enum cplangl_offset_zbuffer_t offset_zbuffer,
                        CYBOOL con_transparencia,
                        const struct i_lista_dibujo_t *lista_dibujo)
{
    float factor_offset, units;
    unsigned long id_lista_poco_detallado;

    assert_no_null(lista_dibujo);

    i_calcula_offset_opengl(tipo_dibujo_actual, offset_zbuffer, &factor_offset, &units);

    if (lista_dibujo->id_lista_poco_detallada != ULONG_MAX)
        id_lista_poco_detallado = lista_dibujo->id_lista_poco_detallada;
    else
        id_lista_poco_detallado = lista_dibujo->id_lista_detallada;

    switch (tipo_dibujo_actual) 
    { 
        case CPLANGL_DIBUJO_CON_MUCHO_DETALLE:
        case CPLANGL_DIBUJO_SOMBRAS_SHADER:
        case CPLANGL_GENERA_SOMBRAS_SHADER:
        {
            i_set_tipo_modelado(tipo_modelado);

            i_dibuja_lista(
                        lista_dibujo->id_lista_detallada, 
                        GL_POLYGON_OFFSET_FILL, factor_offset, units);

            if (lista_dibujo->id_lista_placas_detallada != ULONG_MAX)
                i_dibuja_placas_mucho_detalle(
                        tipo_modelado, con_transparencia,
                        lista_dibujo->id_lista_placas_detallada,
                        factor_offset, units);
            break;
        }
        case CPLANGL_DIBUJO_CON_POCO_DETALLE:
        {
            i_set_tipo_modelado(tipo_modelado);

            i_dibuja_lista(id_lista_poco_detallado, GL_POLYGON_OFFSET_FILL, factor_offset, units);

            if (lista_dibujo->id_lista_placas_poco_detallada != ULONG_MAX)
            {
                enum cplangl_tipo_modelado_t tipo_modelado_poco_detalle;

                if (con_transparencia == CIERTO)
                    tipo_modelado_poco_detalle = CPLANGL_MODELADO_ALAMBRICO;
                else
                    tipo_modelado_poco_detalle = tipo_modelado;

                i_dibuja_placas_mucho_detalle(
                        tipo_modelado_poco_detalle, con_transparencia,
                        lista_dibujo->id_lista_placas_poco_detallada,
                        factor_offset, units);
            }
            break;
        }
        case CPLANGL_DIBUJO_LINEAS:
        {
            i_dibuja_lista(id_lista_poco_detallado, GL_POLYGON_OFFSET_LINE, factor_offset, units);
            break;
        }
        default_error(); 
    }
}

// ----------------------------------------------------------------------------

static unsigned long i_id_texture(const ArrEstructura(i_datos_material_t) *datos_materiales, enum cplan_material_t material)
{
    const struct i_datos_material_t *datos_material;

    datos_material = arr_DameUnicaEstructuraST(datos_materiales, i_datos_material_t, &material, enum cplan_material_t, i_es_datos_material);
    return datos_material->id_material_opengl;
}

// ----------------------------------------------------------------------------

static void i_puntos_suelo(
                        double factor, 
                        double xmin, double ymin, double xmax, double ymax,
                        double *xmin_suelo, double *ymin_suelo, double *xmax_suelo, double *ymax_suelo)
{
    double xmin_suelo_loc, ymin_suelo_loc, xmax_suelo_loc, ymax_suelo_loc;
    double margen, margen_x, margen_y;

    assert_no_null(xmin_suelo);
    assert_no_null(ymin_suelo);
    assert_no_null(xmax_suelo);
    assert_no_null(ymax_suelo);

    margen_x = factor * (xmax - xmin);
    margen_y = factor * (ymax - ymin);

    margen = MAX(margen_x, margen_y);
    margen = MAX(margen, 100.);

    xmin_suelo_loc = xmin - margen;
    ymin_suelo_loc = ymin - margen;
    xmax_suelo_loc = xmax + margen;
    ymax_suelo_loc = ymax + margen;

    *xmin_suelo = xmin_suelo_loc;
    *ymin_suelo = ymin_suelo_loc;
    *xmax_suelo = xmax_suelo_loc;
    *ymax_suelo = ymax_suelo_loc;
}

// ----------------------------------------------------------------------------

static void i_dibuja_lineas(double factor, double xmin, double ymin, double xmax, double ymax)
{
    double x, y, incr;
    double xmin_suelo, ymin_suelo, xmax_suelo, ymax_suelo;

    i_puntos_suelo(factor, xmin, ymin, xmax, ymax, &xmin_suelo, &ymin_suelo, &xmax_suelo, &ymax_suelo);

    incr = 1.;
    glColor3ub(0xCC, 0xB2, 0x66);

    glBegin(GL_LINES);

    for (x = 0.; x < xmax_suelo; x += incr)
    {
        glVertex2d(x, ymin_suelo);
        glVertex2d(x, ymax_suelo);
    }

    for (x = -incr; x > xmin_suelo; x -= incr)
    {
        glVertex2d(x, ymin_suelo);
        glVertex2d(x, ymax_suelo);
    }

    for (y = 0.; y < ymax_suelo; y += incr)
    {
        glVertex2d(xmin_suelo, y);
        glVertex2d(xmax_suelo, y);
    }

    for (y = -incr; y > ymin_suelo; y -= incr)
    {
        glVertex2d(xmin_suelo, y);
        glVertex2d(xmax_suelo, y);
    }

    glEnd();
}

// ----------------------------------------------------------------------------

static void i_dibuja_cielo(double factor, double xmin, double ymin, double xmax, double ymax)
{
    double xmin_suelo, ymin_suelo, xmax_suelo, ymax_suelo;
    double altura;
    static unsigned char r_cielo = 50, g_cielo = 150, b_cielo = 250;
    static unsigned char r_horizonte = 230, g_horizonte = 230, b_horizonte = 255;

    i_puntos_suelo(factor, xmin, ymin, xmax, ymax, &xmin_suelo, &ymin_suelo, &xmax_suelo, &ymax_suelo);
    altura = (ymax_suelo - ymin_suelo) / 2.;

    glBegin(GL_TRIANGLE_STRIP);
    
    glColor3ub(r_cielo, g_cielo, b_cielo);
    glVertex3d(xmin_suelo, ymax_suelo, altura);

    glColor3ub(r_horizonte, g_horizonte, b_horizonte);
    glVertex3d(xmin_suelo, ymax_suelo, 0.);

    glColor3ub(r_cielo, g_cielo, b_cielo);
    glVertex3d(xmax_suelo, ymax_suelo, altura);

    glColor3ub(r_horizonte, g_horizonte, b_horizonte);
    glVertex3d(xmax_suelo, ymax_suelo, 0.);

    glColor3ub(r_cielo, g_cielo, b_cielo);
    glVertex3d(xmax_suelo, ymin_suelo, altura);

    glColor3ub(r_horizonte, g_horizonte, b_horizonte);
    glVertex3d(xmax_suelo, ymin_suelo, 0.);

    glColor3ub(r_cielo, g_cielo, b_cielo);
    glVertex3d(xmin_suelo, ymin_suelo, altura);

    glColor3ub(r_horizonte, g_horizonte, b_horizonte);
    glVertex3d(xmin_suelo, ymin_suelo, 0.);

    glColor3ub(r_cielo, g_cielo, b_cielo);
    glVertex3d(xmin_suelo, ymax_suelo, altura);

    glColor3ub(r_horizonte, g_horizonte, b_horizonte);
    glVertex3d(xmin_suelo, ymax_suelo, 0.);

    glEnd();

    glColor3ub(r_cielo, g_cielo, b_cielo);

    glBegin(GL_TRIANGLE_STRIP);

    glNormal3d(0., 0., -1.);

    glVertex3d(xmin_suelo, ymax_suelo, altura);
    glVertex3d(xmin_suelo, ymin_suelo, altura);
    glVertex3d(xmax_suelo, ymax_suelo, altura);
    glVertex3d(xmax_suelo, ymin_suelo, altura);

    glEnd();
}

// ----------------------------------------------------------------------------

static void i_dibuja_triangulos_suelo(
                        double factor, 
                        double xmin, double ymin, double xmax, double ymax, 
                        unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    double factor_textura = 0.2;
    double xmin_textura, ymin_textura, xmax_textura, ymax_textura;
    double xmitad, ymitad, xmitad_textura, ymitad_textura;
    double xmin_suelo, ymin_suelo, xmax_suelo, ymax_suelo;
    
    i_puntos_suelo(factor, xmin, ymin, xmax, ymax, &xmin_suelo, &ymin_suelo, &xmax_suelo, &ymax_suelo);

    xmin_textura = xmin_suelo * factor_textura;
    ymin_textura = ymin_suelo * factor_textura;
    xmax_textura = xmax_suelo * factor_textura;
    ymax_textura = ymax_suelo * factor_textura;

    xmitad = .5 * (xmin_suelo + xmax_suelo);
    ymitad = .5 * (ymin_suelo + ymax_suelo);

    xmitad_textura = .5 * (xmin_textura + xmax_textura);
    ymitad_textura = .5 * (ymin_textura + ymax_textura);

    glColor4ub(r, g, b, a);
    glNormal3d(0., 0., 1.); 

    glBegin(GL_TRIANGLE_FAN);

    glTexCoord2d(xmitad_textura, ymitad_textura);
    glVertex3d(xmitad, ymitad, 0.);

    glTexCoord2d(xmin_textura, ymin_textura);
    glVertex3d(xmin_suelo, ymin_suelo, 0.);

    glTexCoord2d(xmax_textura, ymin_textura);
    glVertex3d(xmax_suelo, ymin_suelo, 0.);

    glTexCoord2d(xmax_textura, ymax_textura);
    glVertex3d(xmax_suelo, ymax_suelo, 0.);

    glTexCoord2d(xmin_textura, ymax_textura);
    glVertex3d(xmin_suelo, ymax_suelo, 0.);

    glTexCoord2d(xmin_textura, ymin_textura);
    glVertex3d(xmin_suelo, ymin_suelo, 0.);

    glEnd();
}

// ----------------------------------------------------------------------------

static void i_se_aplican_texturas(
                        const struct i_tipo_dibujo_t *tipo_dibujo,
                        struct cyshadow_t *shader_sombra,
                        CYBOOL se_aplican_materiales,
                        unsigned long id_material)
{
    assert_no_null(tipo_dibujo);

    if (se_aplican_materiales == CIERTO)
    {
        glEnable(GL_TEXTURE_2D);
        cyogltextura_activa_textura(id_material, 0);
    }
    else
        glDisable(GL_TEXTURE_2D);

    switch(tipo_dibujo->tipo_dibujo_actual)
    {
        case CPLANGL_DIBUJO_CON_MUCHO_DETALLE:
        case CPLANGL_DIBUJO_CON_POCO_DETALLE:
        case CPLANGL_DIBUJO_LINEAS:
        case CPLANGL_GENERA_SOMBRAS_SHADER:

            break;

        case CPLANGL_DIBUJO_SOMBRAS_SHADER:

            cyshadow_se_aplican_materiales(shader_sombra, se_aplican_materiales);
            break;

        default_error();
    }
}

// ----------------------------------------------------------------------------

static void i_dibuja_suelo_con_texturas(
                        const struct i_tipo_dibujo_t *tipo_dibujo,
                        struct cyshadow_t *shader_sombra,
                        double factor,
                        const ArrEstructura(i_datos_material_t) *materiales_cargados, 
                        double xmin, double ymin, double xmax, double ymax)
{
    unsigned long id_material_opengl_cesped;

    id_material_opengl_cesped = i_id_texture(materiales_cargados, CPLAN_MATERIAL_SUELO_CESPED_VISTA3D);

    i_se_aplican_texturas(tipo_dibujo, shader_sombra, CIERTO, id_material_opengl_cesped);

    glPolygonOffset(2.f, 2.f);
    i_dibuja_triangulos_suelo(factor, xmin, ymin, xmax, ymax, 150, 150, 150, 255);

    i_se_aplican_texturas(tipo_dibujo, shader_sombra, FALSO, 0);
}

// ----------------------------------------------------------------------------

static void i_datos_para_dibujar_suelo(enum i_tipo_vista_t tipo_vista, double *factor, CYBOOL *con_cielo)
{
    assert_no_null(factor);
    assert_no_null(con_cielo);

    switch(tipo_vista)
    {
        case i_VISTA_PERSPECTIVA:
        {
            *factor = 50.;
            *con_cielo = CIERTO;
            break;
        }
        case i_VISTA_ORTOGONAL: 
        {
            *factor = .2;
            *con_cielo = FALSO;
            break;
        }
        case i_VISTA_NO_DEFINIDA:
        default_error();
    }
}

// ---------------------------------------------------------------------------

static void i_dibuja_suelo(
                        const struct i_tipo_dibujo_t *tipo_dibujo,
                        struct cyshadow_t *shader_sombra,
                        const ArrEstructura(i_datos_material_t) *materiales_cargados,
                        enum i_tipo_vista_t tipo_vista,
                        CYBOOL con_mucho_detalle, CYBOOL con_sombras,
                        double xmin, double ymin, double xmax, double ymax)
{
    unsigned long color_rgba;
    double factor;
    CYBOOL con_cielo;

    assert_no_null(tipo_dibujo);

    glPushAttrib(GL_ALL_ATTRIB_BITS);

    if (con_sombras == FALSO)
        glDisable(GL_LIGHTING);

    glDisable(GL_CULL_FACE);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glEnable(GL_POLYGON_OFFSET_FILL);

    i_datos_para_dibujar_suelo(tipo_vista, &factor, &con_cielo);

    if (tipo_dibujo->con_texturas == CIERTO)
        i_dibuja_suelo_con_texturas(tipo_dibujo, shader_sombra, factor, materiales_cargados, xmin, ymin, xmax, ymax);
    else
    {
        unsigned char r, g, b, a;

        color_rgba = idcolmat_rgba_material(CPLAN_MATERIAL_SUELO_VISTA3D, 1.);

        r = GetRValue(color_rgba);
        g = GetGValue(color_rgba);
        b = GetBValue(color_rgba);

        if (con_mucho_detalle == CIERTO)
            a = i_GetAValue(color_rgba);
        else
            a = 255;

        glPolygonOffset(2.f, 2.f);
        i_dibuja_triangulos_suelo(factor, xmin, ymin, xmax, ymax, r, g, b, a);

        if (con_mucho_detalle == CIERTO)
        {
            glPolygonOffset(1.f, 1.f);
            i_dibuja_lineas(factor, xmin, ymin, xmax, ymax);
        }
    }

    if (con_cielo == CIERTO)
    {
        glShadeModel(GL_SMOOTH);
        i_dibuja_cielo(factor / 2., xmin, ymin, xmax, ymax);
    }

    if (con_sombras == FALSO)
        glEnable(GL_LIGHTING);

    glPopAttrib();
}

// ---------------------------------------------------------------------------

void cypegl3d_dibuja_suelo(struct cypegl3d_t *cypegl3d, double xmin, double ymin, double xmax, double ymax)
{
    i_integridad(cypegl3d);
    i_make_current(cypegl3d->manejador_opengl);

    switch(cypegl3d->tipo_dibujo_actual->tipo_dibujo_actual)
    {
        case CPLANGL_DIBUJO_CON_MUCHO_DETALLE:

            i_dibuja_suelo(
                        cypegl3d->tipo_dibujo_actual,
                        cypegl3d->shader_sombra,
                        cypegl3d->materiales_cargados,
                        cypegl3d->tipo_vista,
                        CIERTO, //con_mucho_detalle,
                        FALSO, //con_sombra,
                        xmin, ymin, xmax, ymax);
            break;

        case CPLANGL_DIBUJO_SOMBRAS_SHADER:

            i_dibuja_suelo(
                        cypegl3d->tipo_dibujo_actual,
                        cypegl3d->shader_sombra,
                        cypegl3d->materiales_cargados,
                        cypegl3d->tipo_vista,
                        CIERTO, //con_mucho_detalle,
                        CIERTO, //con_sombra,
                        xmin, ymin, xmax, ymax);
            break;

        case CPLANGL_DIBUJO_CON_POCO_DETALLE:
        
            i_dibuja_suelo(
                        cypegl3d->tipo_dibujo_actual,
                        cypegl3d->shader_sombra,
                        cypegl3d->materiales_cargados,
                        cypegl3d->tipo_vista,
                        FALSO, //con_mucho_detalle,
                        FALSO, //con_sombra,
                        xmin, ymin, xmax, ymax);
            break;
        
        case CPLANGL_DIBUJO_LINEAS:
        case CPLANGL_GENERA_SOMBRAS_SHADER:

            break;

        default_error();
    }
}

// ----------------------------------------------------------------------------

static CYBOOL i_se_puede_dibujar_en_modo(enum cplangl_tipo_dibujo_t tipo_dibujo_actual, CYBOOL arroja_sombra)
{
    switch(tipo_dibujo_actual)
    {
        case CPLANGL_DIBUJO_CON_MUCHO_DETALLE:
        case CPLANGL_DIBUJO_CON_POCO_DETALLE:
        case CPLANGL_DIBUJO_LINEAS:
        case CPLANGL_DIBUJO_SOMBRAS_SHADER:

            return CIERTO;

        case CPLANGL_GENERA_SOMBRAS_SHADER:

            return arroja_sombra;

        default_error();
    }

}

// ----------------------------------------------------------------------------

void cypegl3d_dibuja_lista_dibujo(
                        struct cypegl3d_t *cypegl3d, 
                        enum cplangl_tipo_modelado_t tipo_modelado,
                        enum cplangl_offset_zbuffer_t offset_zbuffer,
                        CYBOOL arroja_sombra,
                        const char *id_lista)
{
    CYBOOL se_dibuja;
    unsigned long ind_encontrado;
    const struct i_lista_dibujo_t *lista_dibujo;

    i_integridad(cypegl3d);
    i_make_current(cypegl3d->manejador_opengl);
    assert(cypegl3d->id_lista_activa == NULL);

    if (i_se_puede_dibujar_en_modo(cypegl3d->tipo_dibujo_actual->tipo_dibujo_actual, arroja_sombra) == CIERTO)
    {
        CYBOOL con_transparencia;

        se_dibuja = apunorde_existe(cypegl3d->lista_dibujos, i_lista_dibujo_t, id_lista, char, i_busca_lista_dibujo, &ind_encontrado);
        assert(se_dibuja == CIERTO);

        lista_dibujo = apunorde_get_const(cypegl3d->lista_dibujos, ind_encontrado, i_lista_dibujo_t);

        con_transparencia = i_es_color_transparente(cypegl3d->tipo_dibujo_actual->tipo_dibujo_actual, cypegl3d->tipo_dibujo_actual->color_rgba_actual);

        i_dibuja_lista_dibujo(
                    cypegl3d->tipo_dibujo_actual->tipo_dibujo_actual, 
                    tipo_modelado, offset_zbuffer, con_transparencia,
                    lista_dibujo);
    }
}

// ----------------------------------------------------------------------------

static CYBOOL i_se_aplica_color(enum cplangl_tipo_dibujo_t tipo_dibujo_actual)
{
    switch(tipo_dibujo_actual)
    {
        case CPLANGL_DIBUJO_CON_MUCHO_DETALLE:
        case CPLANGL_DIBUJO_CON_POCO_DETALLE:
        case CPLANGL_DIBUJO_SOMBRAS_SHADER:

            return CIERTO;

        case CPLANGL_DIBUJO_LINEAS:
        case CPLANGL_GENERA_SOMBRAS_SHADER:

            return FALSO;

        default_error();
    }
}

// ----------------------------------------------------------------------------

static CYBOOL i_se_debe_aplicar_textura(
                        const struct i_tipo_dibujo_t *tipo_dibujo_actual,
                        const ArrEstructura(i_datos_material_t) *datos_materiales,
                        enum cplan_material_t material,
                        unsigned long *ind_material)
{
    assert_no_null(tipo_dibujo_actual);

    if (tipo_dibujo_actual->con_texturas == CIERTO)
    {
        return arr_ExisteUnicaEstructuraST(
                        datos_materiales, i_datos_material_t, &material, enum cplan_material_t, 
                        i_es_datos_material, ind_material);
    }

    return FALSO;
}


// ----------------------------------------------------------------------------

static void i_aplica_color_material(
                        struct i_tipo_dibujo_t *tipo_dibujo_actual,
                        struct cyshadow_t *shader_sombra,
                        const ArrEstructura(i_datos_material_t) *datos_materiales,
                        enum cplan_material_t material,
                        double factor_transparencia)
{
    CYBOOL se_activa_texturas_loc;
    unsigned long id_material_loc;
    unsigned long rgba_material_loc;

    assert_no_null(tipo_dibujo_actual);

    tipo_dibujo_actual->material = material;
    tipo_dibujo_actual->factor_transparencia = factor_transparencia;

    if (i_se_aplica_color(tipo_dibujo_actual->tipo_dibujo_actual) == CIERTO)
    {
        unsigned long ind_material;

        if (i_se_debe_aplicar_textura(tipo_dibujo_actual, datos_materiales, material, &ind_material) == CIERTO)
        {
            const struct i_datos_material_t *datos_material;

            datos_material = arr_GetPunteroConstST(datos_materiales, ind_material, i_datos_material_t);
            assert_no_null(datos_material);

            se_activa_texturas_loc = CIERTO;
            id_material_loc = datos_material->id_material_opengl;
            rgba_material_loc = idcolor_rgba(150, 150, 150, 255);
        }
        else
        {
            se_activa_texturas_loc = FALSO;
            id_material_loc = 0;
            rgba_material_loc = idcolmat_rgba_material(material, 1.);
        }

        i_set_rgba(tipo_dibujo_actual, rgba_material_loc);

        i_se_aplican_texturas(tipo_dibujo_actual, shader_sombra, se_activa_texturas_loc, id_material_loc);
    }
}

// ----------------------------------------------------------------------------

void cypegl3d_destruye_listas_dibujo(struct cypegl3d_t *cypegl3d)
{
    i_integridad(cypegl3d);
    i_make_current(cypegl3d->manejador_opengl);

    assert(cypegl3d->id_lista_activa == NULL);

    apunorde_destruye(&cypegl3d->lista_dibujos, i_lista_dibujo_t);
    cypegl3d->lista_dibujos = apunorde_crea_sin_copia(i_existe_lista_dibujo, i_destruye_lista_dibujo, i_lista_dibujo_t);
    cypegl3d->informe.num_puntos_con_detalle = 0;
    cypegl3d->informe.num_puntos_sin_detalle = 0;
}

// ----------------------------------------------------------------------------

static void i_define_vista_para_ejes(
                        double longitud_ejes,
                        unsigned long ancho_viewport,
                        unsigned long alto_viewport,
                        double giro_x_grados, double giro_y_grados, double giro_z_grados)
{
    double longitud_ejes_con_margen;
    double aspect_ratio;
    double xmin, ymin, xmax, ymax;

    aspect_ratio = (double)ancho_viewport / (double)alto_viewport;
    longitud_ejes_con_margen = 1.05 * longitud_ejes;

    if (ancho_viewport <= alto_viewport)
    {
        xmin = -longitud_ejes_con_margen;
        ymin = -longitud_ejes_con_margen;

        xmax = longitud_ejes_con_margen;
        ymax = longitud_ejes_con_margen / aspect_ratio;
    }
    else
    {
        xmin = -longitud_ejes_con_margen;
        ymin = -longitud_ejes_con_margen;

        xmax = longitud_ejes_con_margen * aspect_ratio;
        ymax = longitud_ejes_con_margen;
    }

    i_aplica_vista_ortogonal(
                    xmin, ymin,
                    xmax, ymax,
                    giro_x_grados, giro_y_grados, giro_z_grados);
}

// ----------------------------------------------------------------------------

static const WCHAR *i_texto_eje(enum i_tipo_eje_t tipo_eje)
{
    switch (tipo_eje) 
    { 
        case i_EJE_X:

            return L"X";

        case i_EJE_Y:

            return L"Y";

        case i_EJE_Z:

            return L"Z";

        default_error(); 
    }
}

// ----------------------------------------------------------------------------

static void i_dibuja_texto_eje(DWORD lista_de_letras, enum i_tipo_eje_t tipo_eje, double Vx, double Vy) 
{
    if (mt_modulo_vector_2D(Vx, Vy) > .001)
    {
        double r_color, g_color, b_color;
        const WCHAR *texto_eje;

        texto_eje = i_texto_eje(tipo_eje);
        i_color_eje(tipo_eje, &r_color, &g_color, &b_color);

        glPushMatrix();
            glTranslated(Vx, Vy, 0.);

                glScaled(0.5, 0.5, 1.);
                glColor3d(r_color, g_color, b_color);
                i_dibuja_texto(lista_de_letras, texto_eje);
        
        glPopMatrix();
    }
}

// ----------------------------------------------------------------------------

static void i_dibuja_textos(
                        DWORD lista_de_letras, 
                        double desplazamiento_texto_en_el_eje, 
                        const struct proyeccion_t *proyeccion)
{
    double Vx, Vy;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    proyecta_punto(proyeccion, desplazamiento_texto_en_el_eje, 0., 0., &Vx, &Vy, NULL);
    i_dibuja_texto_eje(lista_de_letras, i_EJE_X, Vx, Vy);
                        
    proyecta_punto(proyeccion, 0., desplazamiento_texto_en_el_eje, 0., &Vx, &Vy, NULL);
    i_dibuja_texto_eje(lista_de_letras, i_EJE_Y, Vx, Vy);

    proyecta_punto(proyeccion, 0., 0., desplazamiento_texto_en_el_eje, &Vx, &Vy, NULL);
    i_dibuja_texto_eje(lista_de_letras, i_EJE_Z, Vx, Vy);
}

// ----------------------------------------------------------------------------

void cypegl3d_dibuja_ejes_en_esq_inf_izq(struct cypegl3d_t *cypegl3d, const struct proyeccion_t *proyeccion)
{
    i_integridad(cypegl3d);
    i_make_current(cypegl3d->manejador_opengl);

    if (cypegl3d->manejador_opengl->hwnd != NULL)
    {
        long x_viewport, y_viewport;
        unsigned long ancho_viewport, alto_viewport;
        int ancho_trabajo, alto_trabajo;
        double longitud_ejes = 1., longitud_punta;
        double giro_x_grados, giro_y_grados, giro_z_grados;

        longitud_ejes = 1.;
        longitud_punta = 0.4;

        proyecta_obtener_proyeccion(proyeccion, &giro_x_grados, &giro_y_grados, &giro_z_grados);

        cypewin_dimensiones_pixels_hwnd_incluyendo_scroll_si_hay(
                            cypegl3d->manejador_opengl->hwnd,
                            &ancho_viewport, &alto_viewport);

        x_viewport = 0;
        y_viewport = 0; 
        ancho_trabajo = cambtipo_unsigned_long_a_int(ancho_viewport);
        alto_trabajo  = cambtipo_unsigned_long_a_int(alto_viewport);

        i_pon_luz_por_defecto(i_LUZ_AMBIENTE);
        glViewport(x_viewport, y_viewport, ancho_trabajo / 7, alto_trabajo / 7);

        i_define_vista_para_ejes(longitud_ejes + longitud_punta + 0.5, ancho_viewport, alto_viewport, giro_x_grados, giro_y_grados, giro_z_grados);

        i_dibuja_ejes3d(cypegl3d->tipo_dibujo_actual->tipo_dibujo_actual, longitud_ejes, longitud_punta);
        i_dibuja_textos(cypegl3d->font_normal->lista_de_letras, longitud_ejes + longitud_punta, proyeccion);
    }
}

// ----------------------------------------------------------------------------

void cypegl3d_define_viewport_2D(
                        struct cypegl3d_t *cypegl3d, 
                        int x_origen, int y_origen, unsigned long xsize, unsigned long ysize,
                        double x_min, double y_min, double x_max, double y_max)
{
    i_integridad(cypegl3d);
    i_make_current(cypegl3d->manejador_opengl);

    if (cypegl3d->manejador_opengl->hdc_trabajo != cypegl3d->manejador_opengl->hdc_destino)
    {
        int ancho_trabajo, alto_trabajo;

        ancho_trabajo = cambtipo_unsigned_long_a_int(xsize);
        alto_trabajo = cambtipo_unsigned_long_a_int(ysize);

        glViewport(x_origen, y_origen, (GLsizei)ancho_trabajo, (GLsizei)alto_trabajo);
    }
    else
        glViewport(x_origen, y_origen, (GLsizei)xsize, (GLsizei)ysize);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(x_min, x_max, y_min, y_max, -1000., 1000.);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDepthFunc(GL_ALWAYS);
    glShadeModel(GL_SMOOTH);
}

// ----------------------------------------------------------------------------

CYBOOL cypegl3d_se_puede_definir_lista_de_dibujos(const struct cypegl3d_t *cypegl3d)
{
    assert_no_null(cypegl3d);

    if (cypegl3d->manejador_opengl->hwnd != NULL)
        return CIERTO;
    else
        return FALSO;
}

// ----------------------------------------------------------------------------

unsigned long cypegl3d_resolucion_ppp(const struct cypegl3d_t *cypegl3d)
{
    unsigned long resolucion, res_x, res_y;

    res_x = cypewin_GetDeviceCaps(cypegl3d->manejador_opengl->hdc_trabajo, LOGPIXELSX);
    res_y = cypewin_GetDeviceCaps(cypegl3d->manejador_opengl->hdc_trabajo, LOGPIXELSY);

    resolucion = (res_x + res_y) / 2;

    return resolucion;
}

// ----------------------------------------------------------------------------

void cypegl3d_anyadir_plano_clipping(struct cypegl3d_t *cypegl3d, double A, double B, double C, double D)
{
    GLenum plane;
    GLdouble equation[4];

    assert_no_null(cypegl3d);

    equation[0] = A;
    equation[1] = B;
    equation[2] = C;
    equation[3] = D;

    plane = GL_CLIP_PLANE0 + cypegl3d->num_planos_adicionales_clipping_activos;
    glClipPlane(plane, equation);
    glEnable(plane);
    cypegl3d->num_planos_adicionales_clipping_activos++;
}

// ----------------------------------------------------------------------------

static void i_apila_modo_dibujo(
                    ArrEstructura(i_tipo_dibujo_t) *pila_tipos_dibujo,
                    struct cyshadow_t *shadow,
                    struct i_tipo_dibujo_t **tipo_dibujo_actual,
                    CYBOOL con_texturas,
                    enum cplangl_tipo_dibujo_t tipo_dibujo)
{
    struct i_tipo_dibujo_t *tipo_dibujo_nuevo;

    assert_no_null(tipo_dibujo_actual);
    assert_no_null(*tipo_dibujo_actual);

    arr_AppendPunteroST(pila_tipos_dibujo, *tipo_dibujo_actual, i_tipo_dibujo_t);
    tipo_dibujo_nuevo = i_crea_tipo_dibujo(
                    con_texturas, tipo_dibujo, 
                    (*tipo_dibujo_actual)->material, 
                    (*tipo_dibujo_actual)->factor_transparencia, 
                    (*tipo_dibujo_actual)->color_rgba_actual);
    i_prepara_tipo_dibujo(tipo_dibujo_nuevo, shadow);

    *tipo_dibujo_actual = tipo_dibujo_nuevo;
}

// ---------------------------------------------------------------------------

void cypegl3d_inicia_modo_dibujo(struct cypegl3d_t *cypegl3d, CYBOOL con_texturas, enum cplangl_tipo_dibujo_t tipo_dibujo)
{
    i_integridad(cypegl3d);

    i_make_current(cypegl3d->manejador_opengl);

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    i_apila_modo_dibujo(
                    cypegl3d->pila_tipos_dibujo,
                    cypegl3d->shader_sombra,
                    &cypegl3d->tipo_dibujo_actual, con_texturas,
                    tipo_dibujo);
}

// ----------------------------------------------------------------------------

static void i_desapila_modo_dibujo(
                    ArrEstructura(i_tipo_dibujo_t) *pila_tipos_dibujo, 
                    struct i_tipo_dibujo_t **tipo_dibujo_actual)
{
    unsigned long num;

    assert_no_null(tipo_dibujo_actual);

    num = arr_NumElemsPunteroST(pila_tipos_dibujo, i_tipo_dibujo_t);
    assert(num > 0);

    i_destruye_tipo_dibujo(tipo_dibujo_actual);

    *tipo_dibujo_actual = arr_GetPunteroST(pila_tipos_dibujo, num - 1, i_tipo_dibujo_t);
    arr_BorrarPunteroST(pila_tipos_dibujo, num - 1, i_tipo_dibujo_t);

    i_prepara_tipo_dibujo(*tipo_dibujo_actual, NULL);
}

// ----------------------------------------------------------------------------

void cypegl3d_fin_modo_dibujo(struct cypegl3d_t *cypegl3d)
{
    i_integridad(cypegl3d);
    i_make_current(cypegl3d->manejador_opengl);

    switch(cypegl3d->tipo_dibujo_actual->tipo_dibujo_actual)
    {
        case CPLANGL_DIBUJO_CON_MUCHO_DETALLE:
        case CPLANGL_DIBUJO_CON_POCO_DETALLE:
        case CPLANGL_DIBUJO_LINEAS:
        case CPLANGL_GENERA_SOMBRAS_SHADER:

            break;

        case CPLANGL_DIBUJO_SOMBRAS_SHADER:
        {
            cyshadow_fin_dibujo_sombra(cypegl3d->shader_sombra);
            /*{
                long x_viewport, y_viewport;
                unsigned long ancho_viewport, alto_viewport;

                cypewin_dimensiones_pixels_hwnd_incluyendo_scroll_si_hay(cypegl3d->manejador_opengl->hwnd, &ancho_viewport, &alto_viewport);
                x_viewport = 0;
                y_viewport = 0;
                cyshadow_dibuja_mapa_sombras_debug(cypegl3d->shader_sombra, x_viewport, y_viewport, ancho_viewport, alto_viewport);
            }*/
            break;
        }
        default_error();
    }

    glPopAttrib();

    i_desapila_modo_dibujo(cypegl3d->pila_tipos_dibujo, &cypegl3d->tipo_dibujo_actual);
}

// ----------------------------------------------------------------------------

static void i_posiciona_camara_desde_luz(
                        double giro_x_grados, double giro_y_grados, double giro_z_grados,
                        double x_min, double y_min, double z_min, double x_max, double y_max, double z_max)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(x_min, x_max, y_min, y_max, -z_min, -z_max);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    i_cambia_ejes_cype();
    i_aplica_rotacion(giro_x_grados, giro_y_grados, giro_z_grados);
}

// ----------------------------------------------------------------------------

void cypegl3d_inicia_modo_dibujo_sombras_shaders(
                        struct cypegl3d_t *cypegl3d,
                        double giro_x_grados, double giro_y_grados, double giro_z_grados,
                        double x_min, double y_min, double z_min, double x_max, double y_max, double z_max)
{
    assert_no_null(cypegl3d);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    i_apila_modo_dibujo(
                        cypegl3d->pila_tipos_dibujo, 
                        cypegl3d->shader_sombra, 
                        &cypegl3d->tipo_dibujo_actual, FALSO, 
                        CPLANGL_GENERA_SOMBRAS_SHADER);

    i_posiciona_camara_desde_luz(
                        giro_x_grados, giro_y_grados, giro_z_grados,
                        x_min, y_min, z_min, x_max, y_max, z_max);
}

// ----------------------------------------------------------------------------

void cypegl3d_fin_modo_dibujo_sombras_shaders(struct cypegl3d_t *cypegl3d)
{
    cyshadow_fin_generacion_sombra(cypegl3d->shader_sombra);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    i_desapila_modo_dibujo(cypegl3d->pila_tipos_dibujo, &cypegl3d->tipo_dibujo_actual);
}

// ----------------------------------------------------------------------------

void cypegl3d_inicia_solo_solidos(struct cypegl3d_t *cypegl3d)
{
    i_integridad(cypegl3d);
    i_make_current(cypegl3d->manejador_opengl);

    switch(cypegl3d->tipo_dibujo_actual->tipo_dibujo_actual)
    {
        case CPLANGL_DIBUJO_CON_MUCHO_DETALLE:
        case CPLANGL_DIBUJO_SOMBRAS_SHADER:

            glEnable(GL_ALPHA_TEST);
            glAlphaFunc(GL_GREATER, .98f);
            break;

        case CPLANGL_DIBUJO_CON_POCO_DETALLE:
        case CPLANGL_DIBUJO_LINEAS:
        case CPLANGL_GENERA_SOMBRAS_SHADER:

            break;

        default_error();
    }
}

// ----------------------------------------------------------------------------

void cypegl3d_fin_solo_solidos(struct cypegl3d_t *cypegl3d)
{
    i_make_current(cypegl3d->manejador_opengl);

    switch(cypegl3d->tipo_dibujo_actual->tipo_dibujo_actual)
    {
        case CPLANGL_DIBUJO_CON_MUCHO_DETALLE:
        case CPLANGL_DIBUJO_SOMBRAS_SHADER:
        
            glDisable(GL_ALPHA_TEST);
            break;
        
        case CPLANGL_DIBUJO_CON_POCO_DETALLE:
        case CPLANGL_DIBUJO_LINEAS:
        case CPLANGL_GENERA_SOMBRAS_SHADER:

            break;

        default_error();
    }
}

// ----------------------------------------------------------------------------

void cypegl3d_inicia_solo_transparencias(struct cypegl3d_t *cypegl3d)
{
    i_integridad(cypegl3d);
    i_make_current(cypegl3d->manejador_opengl);

    switch(cypegl3d->tipo_dibujo_actual->tipo_dibujo_actual)
    {
        case CPLANGL_DIBUJO_CON_MUCHO_DETALLE:
        case CPLANGL_DIBUJO_SOMBRAS_SHADER:

            glEnable(GL_ALPHA_TEST);
            glAlphaFunc(GL_LESS, .98f);
            glEnable(GL_BLEND);
            break;

        case CPLANGL_DIBUJO_CON_POCO_DETALLE:
        case CPLANGL_DIBUJO_LINEAS:
        case CPLANGL_GENERA_SOMBRAS_SHADER:

            break;

        default_error();
    }
}

// ----------------------------------------------------------------------------

void cypegl3d_fin_solo_transparencias(struct cypegl3d_t *cypegl3d)
{
    i_make_current(cypegl3d->manejador_opengl);

    switch(cypegl3d->tipo_dibujo_actual->tipo_dibujo_actual)
    {
        case CPLANGL_DIBUJO_CON_MUCHO_DETALLE:
        case CPLANGL_DIBUJO_SOMBRAS_SHADER:
        
            glDisable(GL_BLEND);
            glDisable(GL_ALPHA_TEST);
            break;
        
        case CPLANGL_DIBUJO_CON_POCO_DETALLE:
        case CPLANGL_DIBUJO_LINEAS:
        case CPLANGL_GENERA_SOMBRAS_SHADER:

            break;

        default_error();
    }
}

// ----------------------------------------------------------------------------

void cypegl3d_inicia_con_transparencias(struct cypegl3d_t *cypegl3d)
{
    i_integridad(cypegl3d);
    i_make_current(cypegl3d->manejador_opengl);

    switch(cypegl3d->tipo_dibujo_actual->tipo_dibujo_actual)
    {
        case CPLANGL_DIBUJO_CON_MUCHO_DETALLE:
        case CPLANGL_DIBUJO_SOMBRAS_SHADER:

            glEnable(GL_BLEND);
            break;

        case CPLANGL_DIBUJO_CON_POCO_DETALLE:
        case CPLANGL_DIBUJO_LINEAS:
        case CPLANGL_GENERA_SOMBRAS_SHADER:

            break;

        default_error();
    }
}

// ----------------------------------------------------------------------------

void cypegl3d_fin_con_transparencias(struct cypegl3d_t *cypegl3d)
{
    i_integridad(cypegl3d);
    i_make_current(cypegl3d->manejador_opengl);

    switch(cypegl3d->tipo_dibujo_actual->tipo_dibujo_actual)
    {
        case CPLANGL_DIBUJO_CON_MUCHO_DETALLE:
        case CPLANGL_DIBUJO_SOMBRAS_SHADER:

            glDisable(GL_BLEND);
            break;

        case CPLANGL_DIBUJO_CON_POCO_DETALLE:
        case CPLANGL_DIBUJO_LINEAS:
        case CPLANGL_GENERA_SOMBRAS_SHADER:

            break;

        default_error();
    }
}

// ----------------------------------------------------------------------------

CYBOOL cypegl3d_tiene_mucho_detalle(const struct cypegl3d_t *cypegl3d)
{
    assert_no_null(cypegl3d);

    switch(cypegl3d->tipo_dibujo_actual->tipo_dibujo_actual)
    {
        case CPLANGL_DIBUJO_CON_MUCHO_DETALLE:
        case CPLANGL_DIBUJO_SOMBRAS_SHADER:

            return CIERTO;

        case CPLANGL_DIBUJO_CON_POCO_DETALLE:
        case CPLANGL_DIBUJO_LINEAS:
        case CPLANGL_GENERA_SOMBRAS_SHADER:

            return FALSO;

        default_error();
    }
}

// ----------------------------------------------------------------------------

void cypegl3d_desplaza_ventana_de_dibujo(struct cypegl3d_t *cypegl3d, int dx_origen, int dy_origen)
{
    int ancho_trabajo, alto_trabajo;

    assert_no_null(cypegl3d);
    i_make_current(cypegl3d->manejador_opengl);

    cypegl3d->ox_destino += dx_origen;
    cypegl3d->oy_destino += dy_origen;

    ancho_trabajo = cambtipo_unsigned_long_a_int(cypegl3d->ancho_destino);
    alto_trabajo  = cambtipo_unsigned_long_a_int(cypegl3d->alto_destino);

    glViewport(cypegl3d->ox_destino, cypegl3d->oy_destino, ancho_trabajo, alto_trabajo);
}

// ----------------------------------------------------------------------------

void cypegl3d_append_transformaciones(struct cypegl3d_t *cypegl3d)
{
    assert_no_null(cypegl3d);

    i_make_current(cypegl3d->manejador_opengl);
    glPushMatrix();
}

// ----------------------------------------------------------------------------

void cypegl3d_pop_transformaciones(struct cypegl3d_t *cypegl3d)
{
    assert_no_null(cypegl3d);

    i_make_current(cypegl3d->manejador_opengl);
    glPopMatrix();
}

// ----------------------------------------------------------------------------

void cypegl3d_set_matriz_transformacion(
                        double Ux, double Uy, double Uz,
                        double Vx, double Vy, double Vz,
                        double Wx, double Wy, double Wz,
                        double Dx, double Dy, double Dz)
{
    GLdouble matriz[4][4];

    matriz[0][0] = Ux; matriz[0][1] = Uy; matriz[0][2] = Uz; matriz[0][3] = 0.;
    matriz[1][0] = Vx; matriz[1][1] = Vy; matriz[1][2] = Vz; matriz[1][3] = 0.;
    matriz[2][0] = Wx; matriz[2][1] = Wy; matriz[2][2] = Wz; matriz[2][3] = 0.;
    matriz[3][0] = Dx; matriz[3][1] = Dy; matriz[3][2] = Dz; matriz[3][3] = 1.;

    glMultMatrixd((GLdouble *)matriz);
}

// ----------------------------------------------------------------------------

void cypegl3d_set_desplazamientos(struct cypegl3d_t *cypegl3d, double dx, double dy, double dz)
{
    assert_no_null(cypegl3d);

    i_make_current(cypegl3d->manejador_opengl);

    glTranslated(dx, dy, dz);
}

// ----------------------------------------------------------------------------

void cypegl3d_set_rotacion(struct cypegl3d_t *cypegl3d, double angulo_grados, double Wx, double Wy, double Wz)
{
    assert_no_null(cypegl3d);

    i_make_current(cypegl3d->manejador_opengl);

    glRotated(angulo_grados, Wx, Wy, Wz);
}

// ----------------------------------------------------------------------------

void cypegl3d_inicio_transparencia_envoltorios(struct cypegl3d_t *cypegl3d)
{
    i_integridad(cypegl3d);
    /*static GLfloat zPlane[] = {0., 0., 1., 0.};
    GLenum error;

    assert_no_null(cypegl3d);

    i_make_current(cypegl3d->manejador_opengl);
    glEnable(GL_TEXTURE_1D);
    glEnable(GL_TEXTURE_GEN_S);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);*/
    //glTexGenfv(GL_S, GL_EYE_PLANE, zPlane);
    //error = glGetError();
//    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
//    glTexGenfv(GL_S, GL_OBJECT_LINEAR, zPlane);
//    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
//    glTexGenfv(GL_S, GL_OBJECT_PLANE, zPlane);
}

// ----------------------------------------------------------------------------

void cypegl3d_fin_transparencia_envoltorios(struct cypegl3d_t *cypegl3d)
{
    i_integridad(cypegl3d);

    /*i_make_current(cypegl3d->manejador_opengl);
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_1D);*/
}

// ----------------------------------------------------------------------------

void cypegl3d_activa_profundidad(struct cypegl3d_t *cypegl3d)
{
    i_integridad(cypegl3d);
    i_make_current(cypegl3d->manejador_opengl);
    glDepthMask(GL_TRUE);
}

// ----------------------------------------------------------------------------

void cypegl3d_desactiva_profundidad(struct cypegl3d_t *cypegl3d)
{
    i_integridad(cypegl3d);
    i_make_current(cypegl3d->manejador_opengl);
    glDepthMask(GL_FALSE);
}

// ----------------------------------------------------------------------------

void cypegl3d_append_material_si_es_necesario(
                        struct cypegl3d_t *cypegl3d,
                        const char *id_material,
                        enum cplan_material_t material,
                        const unsigned char *bytes_image_rgb,
                        unsigned long xsize, unsigned long ysize)
{
    assert_no_null(cypegl3d);
                     
    i_integridad(cypegl3d);
    i_make_current(cypegl3d->manejador_opengl);
    
    if (arr_ExisteUnicaEstructuraST(
                        cypegl3d->materiales_cargados, i_datos_material_t, 
                        &material, enum cplan_material_t, i_es_datos_material, NULL) == FALSO)
    {
        unsigned long ind_id_material;
        struct i_datos_material_t *datos_material;

        if (arr_ExisteUnicaEstructuraST(
                        cypegl3d->materiales_cargados, i_datos_material_t, 
                        id_material, char, i_es_id_material, &ind_id_material) == CIERTO)
        {
            datos_material = arr_GetPunteroST(cypegl3d->materiales_cargados, ind_id_material, i_datos_material_t);
            arr_AppendEnum(datos_material->materiales_asignados, cplan_material_t, material);
        }
        else
        {
            datos_material = i_crea_datos_material_de_imagen(cypegl3d->manejador_opengl->con_mips_por_hardware, id_material, material, bytes_image_rgb, xsize, ysize);
            arr_AppendPunteroST(cypegl3d->materiales_cargados, datos_material, i_datos_material_t);
        }
    }
}

// ----------------------------------------------------------------------------

void cypegl3d_set_material(struct cypegl3d_t *cypegl3d, enum cplan_material_t material, double factor_transparencia)
{
    i_integridad(cypegl3d);
    i_make_current(cypegl3d->manejador_opengl);

    i_aplica_color_material(
                    cypegl3d->tipo_dibujo_actual,
                    cypegl3d->shader_sombra,
                    cypegl3d->materiales_cargados,
                    material, factor_transparencia);
}

// ----------------------------------------------------------------------------

CYBOOL cypegl3d_rehacer_bloques(const struct cypegl3d_t *cypegl3d)
{
    assert_no_null(cypegl3d);

    if (apunorde_numelems(cypegl3d->lista_dibujos, i_lista_dibujo_t) > 0)
        return FALSO;
    else
        return CIERTO;
}

// ----------------------------------------------------------------------------

CYBOOL cypegl3d_soporta_sombras(const struct cypegl3d_t *cypegl3d)
{
    assert_no_null(cypegl3d);

    if (cypegl3d->shader_sombra != NULL)
        return CIERTO;
    else
        return FALSO;
}

// ----------------------------------------------------------------------------

void cypegl3d_activa_luces(struct cypegl3d_t *cypegl3d)
{
    i_integridad(cypegl3d);
    i_make_current(cypegl3d->manejador_opengl);

    switch(cypegl3d->tipo_dibujo_actual->tipo_dibujo_actual)
    {
        case CPLANGL_DIBUJO_CON_MUCHO_DETALLE:
        case CPLANGL_DIBUJO_CON_POCO_DETALLE:
        case CPLANGL_DIBUJO_LINEAS:

            glEnable(GL_LIGHTING);
            break;

        case CPLANGL_GENERA_SOMBRAS_SHADER:

            break;

        case CPLANGL_DIBUJO_SOMBRAS_SHADER:
        default_error();
    }
}

// ----------------------------------------------------------------------------

void cypegl3d_desactiva_luces(struct cypegl3d_t *cypegl3d)
{
    i_integridad(cypegl3d);
    i_make_current(cypegl3d->manejador_opengl);

    switch(cypegl3d->tipo_dibujo_actual->tipo_dibujo_actual)
    {
        case CPLANGL_DIBUJO_CON_MUCHO_DETALLE:
        case CPLANGL_DIBUJO_CON_POCO_DETALLE:
        case CPLANGL_DIBUJO_LINEAS:

            glDisable(GL_LIGHTING);
            break;

        case CPLANGL_GENERA_SOMBRAS_SHADER:

            break;

        case CPLANGL_DIBUJO_SOMBRAS_SHADER:
        default_error();
    }
}

// ----------------------------------------------------------------------------

void cypegl3d_dibuja_esfera(struct cypegl3d_t *cypegl3d, double radio_esfera)
{
    GLUquadricObj *quadric;
    long num_puntos;

    i_integridad(cypegl3d);
    assert_no_null(cypegl3d->tipo_dibujo_actual);
    i_make_current(cypegl3d->manejador_opengl);

    quadric = i_crea_quadric(cypegl3d->tipo_dibujo_actual->tipo_dibujo_actual);

    num_puntos = (long)i_calcula_num_puntos_circulo(cypegl3d->tipo_dibujo_actual->tipo_dibujo_actual, radio_esfera);

    gluSphere(quadric, radio_esfera, num_puntos, num_puntos);

    gluDeleteQuadric(quadric);
}

// ----------------------------------------------------------------------------

ArrCadena *cypeg3d_informe_debug(struct cypegl3d_t *cypegl3d)
{
    ArrCadena *txt_debug;
    char *txt_puntos;

    txt_debug = arr_CreaCADENA(0);

    txt_puntos = copiafor_codigo1("Puntos con detalle: %ld", cypegl3d->informe.num_puntos_con_detalle);
    arr_AppendCADENAOriginal(txt_debug, &txt_puntos);

    txt_puntos = copiafor_codigo1("Puntos sin detalle: %ld", cypegl3d->informe.num_puntos_sin_detalle);
    arr_AppendCADENAOriginal(txt_debug, &txt_puntos);

    return txt_debug;
}
