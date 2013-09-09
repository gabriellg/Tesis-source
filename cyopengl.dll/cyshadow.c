//Definición de shader para sombras.

#include "cyshadow.inl"
#include "cyshadow.win"

#include "cyoglframebuffer.inl"
#include "cyoglshader.inl"

#include "cyassert.h"
#include "cypespy.h"
#include "standarc.h"
#include "win.h"
#include "windows.hxx"

struct cyshadow_t
{
    float matriz_sombra[16];

    struct cyoglframebuffer_t *frame_buffer_profundidad;
    struct cyoglshader_t *shader_aplicar_sombra;
};

static unsigned long i_SIZEX_SOMBRA = 1024;
static unsigned long i_SIZEY_SOMBRA = 1024;

static int i_NUM_KERNEL = 4;
static float i_SHADOW_INTESIDAD = 0.25;

static const char *i_U_SHADOW_MAP = "u_ShadowMap";
static const char *i_U_MATRIZ_LUZ = "u_MPLuz";
static const char *i_U_NUM_KERNEL = "u_NumKernel";
static const char *i_U_SHADOW_INTENSIDAD = "u_ShadowIntensidad";

static const char *i_U_LUCES_ACTIVADAS = "u_LucesActivadas";
static const char *i_VARIABLE_LUZ_1 = "u_luces_activadas[0]";
static const char *i_VARIABLE_LUZ_2 = "u_luces_activadas[1]";

static const char *i_U_MATERIAL_ELEMENTOS = "u_MaterialElementos";
static const char *i_U_SE_APLICA_MATERIAL = "u_SeAplicaMaterial";
static const char *i_U_SOMBRAS_ACTIVADAS = "u_SombrasActivadas";

static const char *i_SHADER_FRAGMENT_LUZOPENGL =
{
	"// Define los efectos de luz de opengl.\n"\
	"\n"\
	"// ---------------------------------------------------------------------------\n"\
	"\n"\
	"vec4 luzopengl_color_luz(const in int ind_luz, const in vec3 vertex, const in vec3 normal)\n"\
	"{\n"\
	"    vec4 intensidadAmbiente, intesidadDifusa;\n"\
	"    vec3 L;\n"\
	"    float NdotL;\n"\
	"    float dist, atenuacion;\n"\
	"\n"\
	"    L = gl_LightSource[ind_luz].position.xyz - vertex;\n"\
	"    dist = length(L);\n"\
	"    L = normalize(L);\n"\
	"\n"\
	"    NdotL = max(dot(normal, L), 0.);\n"\
	"\n"\
	"    if (gl_LightSource[ind_luz].position.w != 0.)\n"\
	"        atenuacion = gl_LightSource[ind_luz].constantAttenuation \n"\
	"                        + gl_LightSource[ind_luz].linearAttenuation * dist \n"\
	"                        + gl_LightSource[ind_luz].quadraticAttenuation * dist * dist;\n"\
	"    else\n"\
	"        atenuacion = 1.;\n"\
	"\n"\
	"    if (atenuacion != 0.)\n"\
	"        atenuacion = 1. / atenuacion;\n"\
	"\n"\
	"    intensidadAmbiente = gl_Color * gl_LightSource[ind_luz].ambient;\n"\
	"    intesidadDifusa = gl_Color * gl_LightSource[ind_luz].diffuse * NdotL;\n"\
	"\n"\
	"    return atenuacion * (intensidadAmbiente + intesidadDifusa);\n"\
	"}\n"\
};

static const char *i_SHADER_FRAGMENT_VARIAS_LUCES =
{
	"// Definición de varias luces.\n"\
	"// ---------------------------------------------------------------------------\n"\
	"\n"\
	"uniform bool u_luces_activadas[2];\n"\
	"\n"\
	"vec4 luzopengl_color_luz(const in int ind_luz, const in vec3 vertex, const in vec3 normal);\n"\
	"\n"\
	"vec4 variasluces_color_varias_luces(vec3 vertex, vec3 normal_vertex)\n"\
	"{\n"\
	"    vec4 intensidadTotal;\n"\
	"    vec3 normal;\n"\
	"\n"\
	"    normal = normalize(normal_vertex);\n"\
	"\n"\
	"    intensidadTotal = vec4(0.);\n"\
	"\n"\
	"    for (int i = 0; i < 2; i++)\n"\
	"    {\n"\
	"        if (u_luces_activadas[i] == true)\n"\
	"        {\n"\
	"            vec4 intensidadLuz;\n"\
	"\n"\
	"            intensidadLuz = luzopengl_color_luz(i, vertex, normal_vertex);\n"\
	"            intensidadTotal += intensidadLuz;\n"\
	"        }\n"\
	"    }\n"\
	"\n"\
	"    return gl_Color * gl_LightModel.ambient + intensidadTotal;\n"\
	"}\n"\
};

static const char *i_SHADOW_VERTEX =
{
	"uniform mat4 u_MPLuz;\n"\
	"\n"\
	"varying vec4 outvsh_shadow_coord;\n"\
	"varying vec3 outvsh_vertex, outvsh_normal;\n"\
	"\n"\
	"// ---------------------------------------------------------------------------\n"\
	"\n"\
	"void main()\n"\
	"{\n"\
	"    outvsh_vertex = vec3(gl_ModelViewMatrix * gl_Vertex);\n"\
	"    outvsh_normal = normalize(gl_NormalMatrix * gl_Normal);\n"\
	"\n"\
	"    outvsh_shadow_coord = u_MPLuz * gl_Vertex;\n"\
	"\n"\
	"    gl_FrontColor = gl_Color;\n"\
	"    gl_BackColor = gl_Color;\n"\
	"\n"\
	"    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;\n"\
	"	gl_Position = ftransform();\n"\
	"}\n"\
};

static const char *i_SHADER_FRAGMENT_COLOR_TEXTURA =
{
	"// Define la función para calcular el color según si se aplica la textura o no.\n"\
	"// ---------------------------------------------------------------------------\n"\
	"\n"\
	"vec4 color_con_textura_opcional(in sampler2D id_textura, in vec4 color_pixel, in bool se_aplica_material, in vec4 coord_textura)\n"\
	"{\n"\
	"    vec4 color;\n"\
	"\n"\
	"    if (se_aplica_material == true)\n"\
	"        color = color_pixel * texture2D(id_textura, coord_textura.st);\n"\
	"    else\n"\
	"        color = color_pixel;\n"\
	"\n"\
	"    return color;\n"\
	"}\n"\
};

static const char *i_SHADER_FRAGMENT_SHADOW_TEXTURE =
{
	"// Función que define las texturas.\n"\
	"// ---------------------------------------------------------------------------\n"\
	"\n"\
	"float i_lookup(\n"\
	"            in sampler2DShadow shadowMap,\n"\
	"            in vec4 shadowCoord, \n"\
	"            in float dx, in float dy, \n"\
	"            in float ratioToPixelX, in float ratioToPixelY)\n"\
	"{\n"\
	"    float sombra;\n"\
	"    bool isShadow;\n"\
	"    vec4 posSample;\n"\
	"\n"\
	"    posSample = shadowCoord + vec4(dx * ratioToPixelX * shadowCoord.w, dy * ratioToPixelY * shadowCoord.w, 0.0, 0.0);\n"\
	"\n"\
	"    if (shadow2DProj(shadowMap, posSample).x != 1.)\n"\
	"        sombra = 1.;\n"\
	"    else\n"\
	"        sombra = 0.;\n"\
	"\n"\
	"    return sombra;\n"\
	"}\n"\
	"\n"\
	"// ---------------------------------------------------------------------------\n"\
	"\n"\
	"float calculaSombra(\n"\
	"                in sampler2DShadow shadowMap, \n"\
	"                in vec4 shadowCoord, \n"\
	"                in int numKernel, \n"\
	"                in float ratioToPixelX, in float ratioToPixelY)\n"\
	"{\n"\
	"    float sombra;\n"\
	"    float sum, numKernelFloat;\n"\
	"    vec2 offset;\n"\
	"    float dxConst, dyConst;\n"\
	"\n"\
	"    numKernelFloat = float(numKernel);\n"\
	"\n"\
	"    sum = 0.;\n"\
	"    dxConst = -numKernelFloat / 2.;\n"\
	"    dyConst = -numKernelFloat / 2.;\n"\
	"\n"\
	"    for (float i = 0.; i < numKernelFloat; i+=1.)\n"\
	"    {\n"\
	"        for (float j = 0.; j < numKernelFloat; j+=1.)\n"\
	"        {\n"\
	"            float dx, dy;\n"\
	"\n"\
	"            dx = i + dxConst;\n"\
	"            dy = j + dyConst;\n"\
	"\n"\
	"            sum += i_lookup(shadowMap, shadowCoord, dx, dy, ratioToPixelX, ratioToPixelY);\n"\
	"        }\n"\
	"    }\n"\
	"        \n"\
	"    sombra = sum / (numKernelFloat * numKernelFloat);\n"\
	"\n"\
	"    return sombra;\n"\
	"}\n"\
};

static const char *i_SHADER_FRAGMENT_MAIN_SHADOW_TEXTURE =
{
	"uniform sampler2DShadow u_ShadowMap;\n"\
	"uniform int u_NumKernel;\n"\
	"uniform float u_ShadowIntensidad;\n"\
	"\n"\
	"uniform sampler2D u_MaterialElementos;\n"\
	"uniform bool u_SeAplicaMaterial;\n"\
	"\n"\
	"uniform bool u_LucesActivadas;\n"\
	"uniform bool u_SombrasActivadas;\n"\
	"\n"\
	"varying vec4 outvsh_shadow_coord;\n"\
	"varying vec3 outvsh_vertex, outvsh_normal;\n"\
	"\n"\
	"float calculaSombra(in sampler2DShadow shadowMap, in vec4 shadowCoord, in int numKernel, in float ratioToPixelX, in float ratioToPixelY);\n"\
	"vec4 color_con_textura_opcional(in sampler2D id_textura, in vec4 color_pixel, in bool se_aplica_material, in vec4 coord_textura);\n"\
	"vec4 variasluces_color_varias_luces(vec3 vertex, vec3 normal_vertex);\n"\
	"\n"\
	"// ---------------------------------------------------------------------------\n"\
	"\n"\
	"void main()\n"\
	"{\n"\
	"    float iluminacion, sombra;\n"\
	"    vec4 color_elemento, color_final;\n"\
	"    vec4 coord_sombra_con_precision;\n"\
	"    float rationPixelShadow;\n"\
	"\n"\
	"    rationPixelShadow = 1. / 1024.;\n"\
	"\n"\
	"    if (u_SombrasActivadas == true)\n"\
	"        sombra = calculaSombra(u_ShadowMap, outvsh_shadow_coord, u_NumKernel, rationPixelShadow, rationPixelShadow);\n"\
	"    else\n"\
	"        sombra = 0.;\n"\
	"\n"\
	"    if (u_LucesActivadas == true)\n"\
	"        color_elemento = variasluces_color_varias_luces(outvsh_vertex, outvsh_normal);\n"\
	"    else\n"\
	"        color_elemento = gl_Color;\n"\
	"\n"\
	"    if (sombra != 0.)\n"\
	"    {\n"\
	"        iluminacion = 1. - u_ShadowIntensidad * sombra;\n"\
	"        color_elemento = vec4(iluminacion * color_elemento.rgb, color_elemento.a);\n"\
	"    }\n"\
	"\n"\
	"    color_final = color_con_textura_opcional(u_MaterialElementos, color_elemento, u_SeAplicaMaterial, gl_TexCoord[0]);\n"\
	"\n"\
	"    gl_FragColor = vec4(color_final.rgb, gl_Color.a);\n"\
	"}\n"\
};

// ---------------------------------------------------------------------------

CONSTRUCTOR(static struct cyshadow_t *, i_crea, (
                        float matriz_sombra[16],
                        struct cyoglframebuffer_t **frame_buffer_profundidad,
                        struct cyoglshader_t **shader_aplicar_sombra))
{
    struct cyshadow_t *shadow;

    shadow = MALLOC(struct cyshadow_t);

    memcpy(shadow->matriz_sombra, matriz_sombra, 16 * sizeof(float));

    shadow->frame_buffer_profundidad = ASIGNA_PUNTERO_PP_NO_NULL(frame_buffer_profundidad, struct cyoglframebuffer_t);
    shadow->shader_aplicar_sombra = ASIGNA_PUNTERO_PP_NO_NULL(shader_aplicar_sombra, struct cyoglshader_t);

    return shadow;
}

// ---------------------------------------------------------------------------

CONSTRUCTOR(static struct cyoglshader_t *, i_crea_shader_shadow, (void))
{
    CYBOOL se_puede_crear_shader;
    struct cyoglshader_t *shader_aplicar_sombra_loc;

    shader_aplicar_sombra_loc = cyoglshader_crea_defecto("shader");

    cyoglshader_append_vertex_shader(shader_aplicar_sombra_loc, i_SHADOW_VERTEX);

    cyoglshader_append_fragment_shader(shader_aplicar_sombra_loc, i_SHADER_FRAGMENT_LUZOPENGL);
    cyoglshader_append_fragment_shader(shader_aplicar_sombra_loc, i_SHADER_FRAGMENT_VARIAS_LUCES);
    cyoglshader_append_fragment_shader(shader_aplicar_sombra_loc, i_SHADER_FRAGMENT_COLOR_TEXTURA);
    cyoglshader_append_fragment_shader(shader_aplicar_sombra_loc, i_SHADER_FRAGMENT_SHADOW_TEXTURE);
    cyoglshader_append_fragment_shader(shader_aplicar_sombra_loc, i_SHADER_FRAGMENT_MAIN_SHADOW_TEXTURE);

    se_puede_crear_shader = cyoglshader_compila_y_linka(shader_aplicar_sombra_loc, NULL);
    assert(se_puede_crear_shader == CIERTO);

    cyoglshader_append_variable_uniform(shader_aplicar_sombra_loc, i_U_SHADOW_MAP);
    cyoglshader_append_variable_uniform(shader_aplicar_sombra_loc, i_U_MATRIZ_LUZ);
    cyoglshader_append_variable_uniform(shader_aplicar_sombra_loc, i_U_NUM_KERNEL);
    cyoglshader_append_variable_uniform(shader_aplicar_sombra_loc, i_U_SHADOW_INTENSIDAD);

    cyoglshader_append_variable_uniform(shader_aplicar_sombra_loc, i_U_MATERIAL_ELEMENTOS);
    cyoglshader_append_variable_uniform(shader_aplicar_sombra_loc, i_U_SE_APLICA_MATERIAL);
    cyoglshader_append_variable_uniform(shader_aplicar_sombra_loc, i_U_SOMBRAS_ACTIVADAS);

    cyoglshader_append_variable_uniform(shader_aplicar_sombra_loc, i_U_LUCES_ACTIVADAS);
    cyoglshader_append_variable_uniform(shader_aplicar_sombra_loc, i_VARIABLE_LUZ_1);
    cyoglshader_append_variable_uniform(shader_aplicar_sombra_loc, i_VARIABLE_LUZ_2);

    return shader_aplicar_sombra_loc;
}

// ---------------------------------------------------------------------------

CYBOOL cyshadow_soporta_shaders_sombras(void)
{
    CYBOOL se_puede_crear_shader;
    struct cyoglshader_t *shader_aplicar_sombra;

    shader_aplicar_sombra = cyoglshader_crea_defecto("shader");

    cyoglshader_append_vertex_shader(shader_aplicar_sombra, i_SHADOW_VERTEX);

    cyoglshader_append_fragment_shader(shader_aplicar_sombra, i_SHADER_FRAGMENT_LUZOPENGL);
    cyoglshader_append_fragment_shader(shader_aplicar_sombra, i_SHADER_FRAGMENT_VARIAS_LUCES);
    cyoglshader_append_fragment_shader(shader_aplicar_sombra, i_SHADER_FRAGMENT_COLOR_TEXTURA);
    cyoglshader_append_fragment_shader(shader_aplicar_sombra, i_SHADER_FRAGMENT_SHADOW_TEXTURE);
    cyoglshader_append_fragment_shader(shader_aplicar_sombra, i_SHADER_FRAGMENT_MAIN_SHADOW_TEXTURE);

    se_puede_crear_shader = cyoglshader_compila_y_linka(shader_aplicar_sombra, NULL);

    if (se_puede_crear_shader == CIERTO)
    {
        if (cyoglshader_se_puede_definir_variable(shader_aplicar_sombra, i_U_SHADOW_MAP) == FALSO)
            se_puede_crear_shader = FALSO;
        else if (cyoglshader_se_puede_definir_variable(shader_aplicar_sombra, i_U_MATRIZ_LUZ) == FALSO)
            se_puede_crear_shader = FALSO;
        else if (cyoglshader_se_puede_definir_variable(shader_aplicar_sombra, i_U_NUM_KERNEL) == FALSO)
            se_puede_crear_shader = FALSO;
        else if (cyoglshader_se_puede_definir_variable(shader_aplicar_sombra, i_U_SHADOW_INTENSIDAD) == FALSO)
            se_puede_crear_shader = FALSO;
        else if (cyoglshader_se_puede_definir_variable(shader_aplicar_sombra, i_U_MATERIAL_ELEMENTOS) == FALSO)
            se_puede_crear_shader = FALSO;
        else if (cyoglshader_se_puede_definir_variable(shader_aplicar_sombra, i_U_SE_APLICA_MATERIAL) == FALSO)
            se_puede_crear_shader = FALSO;
        else if (cyoglshader_se_puede_definir_variable(shader_aplicar_sombra, i_U_SOMBRAS_ACTIVADAS) == FALSO)
            se_puede_crear_shader = FALSO;
        else if (cyoglshader_se_puede_definir_variable(shader_aplicar_sombra, i_U_LUCES_ACTIVADAS) == FALSO)
            se_puede_crear_shader = FALSO;
        else if (cyoglshader_se_puede_definir_variable(shader_aplicar_sombra, i_VARIABLE_LUZ_1) == FALSO)
            se_puede_crear_shader = FALSO;
        else
            se_puede_crear_shader = cyoglshader_se_puede_definir_variable(shader_aplicar_sombra, i_VARIABLE_LUZ_2);
    }
    
    cyoglshader_destruye(&shader_aplicar_sombra);

    return se_puede_crear_shader;
}

// ---------------------------------------------------------------------------

struct cyshadow_t *cyshadow_crea_shadow()
{
    float matriz_sombra[16];
    struct cyoglframebuffer_t *frame_buffer_profundidad;
    struct cyoglshader_t *shader_aplicar_sombra;

    frame_buffer_profundidad = cyoglframebuffer_crea_fbo_profundidad(i_SIZEX_SOMBRA, i_SIZEY_SOMBRA);
    shader_aplicar_sombra = i_crea_shader_shadow();
    memset(matriz_sombra, 0, 16 * sizeof(float));

    return i_crea(matriz_sombra, &frame_buffer_profundidad, &shader_aplicar_sombra);
}

// ---------------------------------------------------------------------------

void cyshadow_destruye(struct cyshadow_t **shadow)
{
    assert_no_null(shadow);
    assert_no_null(*shadow);

    cyoglframebuffer_destruye(&(*shadow)->frame_buffer_profundidad);
    cyoglshader_destruye(&(*shadow)->shader_aplicar_sombra);

    FREE_PP(shadow, struct cyshadow_t);
}

// ---------------------------------------------------------------------------

static void i_prepara_escena_para_calcula_mapa_profundidad(void)
{
    glPolygonMode(GL_FRONT, GL_FILL);
    glShadeModel(GL_FLAT);
    glDisable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);

    glViewport(0, 0, (int)i_SIZEX_SOMBRA, (int)i_SIZEY_SOMBRA);

    glClearColor(1.f, 1.f, 1.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// ---------------------------------------------------------------------------

void cyshadow_inicia_generacion_sombra(struct cyshadow_t *shadow)
{
    assert_no_null(shadow);

    glPushAttrib(GL_VIEWPORT_BIT);

    cyoglframebuffer_activa_frame(shadow->frame_buffer_profundidad);
    i_prepara_escena_para_calcula_mapa_profundidad();
}

// ----------------------------------------------------------------------------

static void i_calcula_proyeccion_sombras(float matriz_sombra[16])
{
    float lightProjection[16], lightModelview[16];

    glGetFloatv(GL_PROJECTION_MATRIX, lightProjection);
    glGetFloatv(GL_MODELVIEW_MATRIX, lightModelview);

    glMatrixMode(GL_TEXTURE);

    glPushMatrix();
    glLoadIdentity();

    glTranslatef(0.5f, 0.5f, 0.5f);
    glScalef(0.5f, 0.5f, 0.5f);
    glMultMatrixf(lightProjection);
    glMultMatrixf(lightModelview);
    glGetFloatv(GL_TEXTURE_MATRIX, matriz_sombra);

    glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
}

// ---------------------------------------------------------------------------

void cyshadow_fin_generacion_sombra(struct cyshadow_t *shadow)
{
    assert_no_null(shadow);

    i_calcula_proyeccion_sombras(shadow->matriz_sombra);

    cyoglframebuffer_desactiva_frame(shadow->frame_buffer_profundidad);
    glPopAttrib();
}

// ---------------------------------------------------------------------------

void cyshadow_inicia_dibujo_sombra(
                        struct cyshadow_t *shadow,
                        unsigned long unidad_texturas_materiales, 
                        CYBOOL activa_luz_1, CYBOOL activa_luz_2)
{
    int unidad_textura_sombra;

    assert_no_null(shadow);

    unidad_textura_sombra = 1;
    cyoglframebuffer_aplica_textura(shadow->frame_buffer_profundidad, (unsigned int)unidad_textura_sombra);

    cyoglshader_ejecuta_shader(shadow->shader_aplicar_sombra);

    cyoglshader_set_variable_uniform_int(shadow->shader_aplicar_sombra, i_U_SHADOW_MAP, unidad_textura_sombra);
    cyoglshader_set_variable_uniform_matriz4x4(shadow->shader_aplicar_sombra, i_U_MATRIZ_LUZ, shadow->matriz_sombra);

    cyoglshader_set_variable_uniform_int(shadow->shader_aplicar_sombra, i_U_NUM_KERNEL, i_NUM_KERNEL);
    cyoglshader_set_variable_uniform_float(shadow->shader_aplicar_sombra, i_U_SHADOW_INTENSIDAD, i_SHADOW_INTESIDAD);

    cyoglshader_set_variable_uniform_CYBOOL(shadow->shader_aplicar_sombra, i_U_SE_APLICA_MATERIAL, FALSO);
    cyoglshader_set_variable_uniform_int(shadow->shader_aplicar_sombra, i_U_MATERIAL_ELEMENTOS, (int)unidad_texturas_materiales);

    cyoglshader_set_variable_uniform_CYBOOL(shadow->shader_aplicar_sombra, i_U_SOMBRAS_ACTIVADAS, CIERTO);

    cyoglshader_set_variable_uniform_CYBOOL(shadow->shader_aplicar_sombra, i_U_LUCES_ACTIVADAS, CIERTO);
    cyoglshader_set_variable_uniform_CYBOOL(shadow->shader_aplicar_sombra, i_VARIABLE_LUZ_1, activa_luz_1);
    cyoglshader_set_variable_uniform_CYBOOL(shadow->shader_aplicar_sombra, i_VARIABLE_LUZ_2, activa_luz_2);
}

// ---------------------------------------------------------------------------

void cyshadow_fin_dibujo_sombra(struct cyshadow_t *shadow)
{
    assert_no_null(shadow);
    cyoglshader_parar_shader(shadow->shader_aplicar_sombra);
}

// ---------------------------------------------------------------------------

void cyshadow_se_aplican_materiales(struct cyshadow_t *shadow, CYBOOL se_aplica_materiales)
{
    assert_no_null(shadow);
    cyoglshader_set_variable_uniform_CYBOOL(shadow->shader_aplicar_sombra, i_U_SE_APLICA_MATERIAL, se_aplica_materiales);
}

// ---------------------------------------------------------------------------

void cyshadow_dibuja_mapa_sombras_debug(
                        const struct cyshadow_t *shadow,
                        long x_viewport, long y_viewport,
                        unsigned long ancho_viewport, unsigned long alto_viewport)
{
    assert_no_null(shadow);

    cyoglframebuffer_dibuja_framebuffer_debug(
                        shadow->frame_buffer_profundidad, 
                        1, x_viewport, y_viewport, ancho_viewport, alto_viewport);
}

// ---------------------------------------------------------------------------

void cyshadow_es_de_dia_o_de_noche(struct cyshadow_t *shadow, CYBOOL es_de_dia)
{
    cyoglshader_set_variable_uniform_CYBOOL(shadow->shader_aplicar_sombra, i_U_SOMBRAS_ACTIVADAS, es_de_dia);
}