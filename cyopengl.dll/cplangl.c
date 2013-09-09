// Abre un cypeplan para dibujar con OpenGL

#include "cplangl.h"
#include "cplangl.tlh"

#include "cypegl3d.inl"

#include "cypespy.h"
#include "cypeplan.h"
#include "escal.h"
#include "proyecta.h"
#include "cyassert.h"
#include "gestcapa.h"
#include "arraydxf.h"
#include "idcolor.h"
#include "win.h"
#include "cambtipo.h"

#include "cypeplan.tlh"
#include "gestcapa.tlh"
#include "defmath.tlh"

struct i_datos_dibujo_3D_t
{
    struct cypegl3d_t *cypegl3d;

    struct GestionCapas_t *gestionCapas;
    struct Escalas_t *escala;

    unsigned long ancho_imagen, alto_imagen;
};

// ----------------------------------------------------------------------------

CONSTRUCTOR(static struct i_datos_dibujo_3D_t *, i_creadatos_dibujo, (
                        struct cypegl3d_t **cypegl3d,
                        struct GestionCapas_t **gestionCapas,
                        struct Escalas_t **escala,
                        unsigned long ancho_imagen, unsigned long alto_imagen))
{
    struct i_datos_dibujo_3D_t *datos_dibujo;

    datos_dibujo = MALLOC(struct i_datos_dibujo_3D_t);

    datos_dibujo->cypegl3d = ASIGNA_PUNTERO_PP(cypegl3d, struct cypegl3d_t);

    datos_dibujo->gestionCapas = ASIGNA_PUNTERO_PP_NO_NULL(gestionCapas, struct GestionCapas_t);
    datos_dibujo->escala = ASIGNA_PUNTERO_PP(escala, struct Escalas_t);
    
    datos_dibujo->ancho_imagen = ancho_imagen;
    datos_dibujo->alto_imagen = alto_imagen;
    
    return datos_dibujo;
}

// ----------------------------------------------------------------------------

CONSTRUCTOR(static struct i_datos_dibujo_3D_t *, i_creadatos_dibujo_defecto, (unsigned long ancho_imagen, unsigned long alto_imagen))
{
    struct cypegl3d_t *cypegl3d;
    struct GestionCapas_t *gestionCapas;
    struct Escalas_t *escala;

    cypegl3d = NULL;

    gestionCapas = gestcapa_crea();
    escala = NULL;
    
    return i_creadatos_dibujo(&cypegl3d, &gestionCapas, &escala, ancho_imagen, alto_imagen);
}

// ----------------------------------------------------------------------------

static void i_destruye_datos_dibujo(struct i_datos_dibujo_3D_t **datos_dibujo)
{
    assert_no_null(datos_dibujo);
    assert_no_null(*datos_dibujo);
    
    if ((*datos_dibujo)->cypegl3d != NULL)
        cypegl3d_destruye(&(*datos_dibujo)->cypegl3d);

    gestcapa_destruye(&(*datos_dibujo)->gestionCapas);

    if ((*datos_dibujo)->escala != NULL)
        escal_destruye(&(*datos_dibujo)->escala);
        
    FREE_PP(datos_dibujo, struct i_datos_dibujo_3D_t);
}

// ----------------------------------------------------------------------------

static void i_guardar(struct i_datos_dibujo_3D_t *datos_dibujo)
{
    assert_no_null(datos_dibujo);

    cypegl3d_guardar(datos_dibujo->cypegl3d);
    arraydxf_guardar_atributos_actuales(datos_dibujo->gestionCapas->datosCapa);
}

// ----------------------------------------------------------------------------

static void i_escr_color_CypeGL(struct cypegl3d_t *cypegl3d, const struct idcolor_t *color)
{
    idcolor_escr_color_con_paleta_cype(cypegl3d, color, cypegl3d_set_color, cypegl3d_set_color_rgba, cypegl3d_t);
}

// ----------------------------------------------------------------------------

static void i_reponer(struct i_datos_dibujo_3D_t *datos_dibujo)
{
    struct idcolor_t *color_actual;
    CYBOOL ha_cambiado_el_color;
                                
    assert_no_null(datos_dibujo);

    cypegl3d_reponer(datos_dibujo->cypegl3d);
    arraydxf_reponer_atributos(datos_dibujo->gestionCapas->datosCapa,
                               NULL, NULL, NULL,
                               &ha_cambiado_el_color, &color_actual,
                               NULL, NULL, NULL);
    
    if (ha_cambiado_el_color == CIERTO)
        i_escr_color_CypeGL(datos_dibujo->cypegl3d, color_actual);
    idcolor_destruye(&color_actual);
}

// ----------------------------------------------------------------------------

static unsigned char i_define_capa(
                                struct i_datos_dibujo_3D_t *datos_dibujo,
                                const unsigned char capa,
                                const char *nombre,
                                const enum capadxf_pluma_t pluma,
                                const double grosor_pluma,
                                const struct idcolor_t *idcolor,
                                const unsigned char linea,
                                const char *nombre_tipo_linea_definida,
                                const unsigned char texto)
{
    assert_no_null(datos_dibujo);
    return arraydxf_define_capas(
                                datos_dibujo->gestionCapas->datosCapa, 
                                capa, nombre, pluma, grosor_pluma, idcolor,
                                linea, nombre_tipo_linea_definida, texto);
}

// ----------------------------------------------------------------------------

static void i_escr_capa(struct i_datos_dibujo_3D_t *datos_dibujo, const unsigned char capadxf)
{
    struct idcolor_t *color_actual;
    CYBOOL ha_cambiado_el_color;

    assert_no_null(datos_dibujo);

    arraydxf_cambiar_capa(datos_dibujo->gestionCapas->datosCapa, capadxf,
                          NULL, NULL, NULL,
                          &ha_cambiado_el_color, &color_actual,
                          NULL, NULL, NULL);

    if (ha_cambiado_el_color && datos_dibujo->cypegl3d != NULL)
        i_escr_color_CypeGL(datos_dibujo->cypegl3d, color_actual);
    idcolor_destruye(&color_actual);
}

// ----------------------------------------------------------------------------

static void i_escr_color(struct i_datos_dibujo_3D_t *datos_dibujo, const struct idcolor_t *idcolor)
{
    struct idcolor_t *color_actual;

    assert_no_null(datos_dibujo);

    arraydxf_cambiar_idcolor(datos_dibujo->gestionCapas->datosCapa, idcolor, &color_actual, NULL);
    i_escr_color_CypeGL(datos_dibujo->cypegl3d, color_actual);
    idcolor_destruye(&color_actual);
}

// ----------------------------------------------------------------------------

static void i_escr_tipo_linea(struct i_datos_dibujo_3D_t *datos_dibujo, const unsigned char tipo)
{
    assert_no_null(datos_dibujo);
    cypegl3d_set_tipo_linea(datos_dibujo->cypegl3d, tipo);
}

// ----------------------------------------------------------------------------

static void i_dibuja_circulo3D(
                        struct i_datos_dibujo_3D_t *datos_dibujo,
                        const double xc, const double yc, double zc,
                        const double radio,
                        CYBOOL relleno)
{
    assert_no_null(datos_dibujo);
    assert(relleno == FALSO);

    cypegl3d_circulo(datos_dibujo->cypegl3d, xc, yc, zc, radio, relleno);
}

// ----------------------------------------------------------------------------

static void i_dibuja_arco3D(
                        struct i_datos_dibujo_3D_t *datos_dibujo,
                        double xc, double yc, double zc, 
                        double x1, double y1, double z1, 
                        double x2, double y2, double z2, 
                        CYBOOL relleno)
{
    assert_no_null(datos_dibujo);
    assert(relleno == FALSO);

    cypegl3d_arco3D(datos_dibujo->cypegl3d, xc, yc, zc, x1, y1, z1, x2, y2, z2, relleno);
}

// ----------------------------------------------------------------------------

static void i_dibuja_elipse3D(
                        struct i_datos_dibujo_3D_t *datos_dibujo,
                        const double xc, const double yc, double zc,
                        const double ra, const double rb,
                        const double angulo,
                        CYBOOL relleno)
{
    assert_no_null(datos_dibujo);
    assert(relleno == FALSO);

    cypegl3d_elipse(datos_dibujo->cypegl3d, xc, yc, zc, ra, rb, angulo, relleno);
}

// ----------------------------------------------------------------------------

static void i_dibuja_linea3D(
                        struct i_datos_dibujo_3D_t *datos_dibujo,
                        const double x1, const double y1, const double z1,
                        const double x2, const double y2, const double z2)
{
    assert_no_null(datos_dibujo);
    cypegl3d_linea3D(datos_dibujo->cypegl3d, x1, y1, z1, x2, y2, z2);
}

// ----------------------------------------------------------------------------

static void i_dibuja_polilinea3D(
                        struct i_datos_dibujo_3D_t *datos_dibujo,
                        const ArrPunto3D *polilinea,
                        CYBOOL relleno)
{
    assert_no_null(datos_dibujo);

    if (relleno == FALSO)   
        cypegl3d_polilinea3D(datos_dibujo->cypegl3d, polilinea);
    else
        cypegl3d_poligono3D(datos_dibujo->cypegl3d, polilinea);
}

// ----------------------------------------------------------------------------

static void i_dibuja_cara3D(
                        struct i_datos_dibujo_3D_t *datos_dibujo,
                        double x1, double y1, double z1, CYBOOL es_visible_arista1,
                        double x2, double y2, double z2, CYBOOL es_visible_arista2,
                        double x3, double y3, double z3, CYBOOL es_visible_arista3,
                        double x4, double y4, double z4, CYBOOL es_visible_arista4)
{
    assert_no_null(datos_dibujo);
    cypegl3d_quad(
            datos_dibujo->cypegl3d,
            x1, y1, z1, es_visible_arista1, x2, y2, z2, es_visible_arista2,
            x3, y3, z3, es_visible_arista3, x4, y4, z4, es_visible_arista4);
}

// ----------------------------------------------------------------------------

static void i_dibuja_solid3D(
                        struct i_datos_dibujo_3D_t *datos_dibujo,
                        double x1, double y1, double z1,
                        double x2, double y2, double z2,
                        double x3, double y3, double z3,
                        double x4, double y4, double z4)
{
    assert_no_null(datos_dibujo);
    cypegl3d_quad(
            datos_dibujo->cypegl3d,
            x1, y1, z1, CIERTO, x2, y2, z2, CIERTO,
            x4, y4, z4, CIERTO, x3, y3, z3, CIERTO);
}

// ----------------------------------------------------------------------------

static void i_dibuja_polilinea_con_cambio_de_color(
                        struct i_datos_dibujo_3D_t *datos_dibujo,
                        const ArrPunto3D *polilinea, 
                        const ArrULong *rgba)
{
    assert_no_null(datos_dibujo);
    cypegl3d_polilinea_con_cambio_de_color(datos_dibujo->cypegl3d, polilinea, rgba);
}

// ----------------------------------------------------------------------------

static void i_dibuja_texto3D(
                        struct i_datos_dibujo_3D_t *datos_dibujo,
                        const WCHAR *texto, 
                        double x, double y, double z, 
                        unsigned char just, 
                        double alto, double ancho, 
                        double cos, double sen, 
                        double longitud, 
                        unsigned char estilo_texto)
{
    UNREFERENCED(ancho);
    UNREFERENCED(estilo_texto);
    assert_no_null(datos_dibujo);
    cypegl3d_dibuja_texto3D(
                        datos_dibujo->cypegl3d, texto,
                        x, y, z, just,
                        alto, cos, sen, longitud);
}

// ----------------------------------------------------------------------------

static void i_dibuja_triangulo_con_normal_por_vertice3D(
                        struct i_datos_dibujo_3D_t *datos_dibujo,
                        double x1, double y1, double z1, double Wx1, double Wy1, double Wz1, 
                        double x2, double y2, double z2, double Wx2, double Wy2, double Wz2,
                        double x3, double y3, double z3, double Wx3, double Wy3, double Wz3)
{
    assert_no_null(datos_dibujo);
    cypegl3d_triangulo3D_orientado(
                                datos_dibujo->cypegl3d,
                                x1, y1, z1, Wx1, Wy1, Wz1,
                                x2, y2, z2, Wx2, Wy2, Wz2,
                                x3, y3, z3, Wx3, Wy3, Wz3);
}

// ----------------------------------------------------------------------------

static void i_dibuja_mesh(
                        struct i_datos_dibujo_3D_t *datos_dibujo,
                        const ArrPunto3D *puntos,
                        const ArrPunto3D *normales,
                        const ArrPunto2D *coord_texturas,
                        const ArrBool *es_borde,
                        const ArrEnum(cplan_tipo_primitiva_t) *tipo_primitiva,
                        const ArrPuntero(ArrULong) *inds_caras)
{
    assert_no_null(datos_dibujo);
    cypegl3d_bandas_triangulos(
                        datos_dibujo->cypegl3d,
                        puntos, normales, coord_texturas, es_borde,
                        tipo_primitiva, inds_caras);
}

// ----------------------------------------------------------------------------

static void i_dibuja_tubo3D(
                        struct i_datos_dibujo_3D_t *datos_dibujo, 
                        const ArrPunto3D *puntos,
                        double diametro)
{
    assert_no_null(datos_dibujo);
    cypegl3d_dibuja_tubos(datos_dibujo->cypegl3d, puntos, diametro);
}

// ----------------------------------------------------------------------------

static void i_dibuja_prisma_conico(
                        struct i_datos_dibujo_3D_t *datos_dibujo, 
                        double x_centro_inf, double y_centro_inf, double z_centro_inf,
                        double diametro_inferior,
                        double x_centro_sup, double y_centro_sup, double z_centro_sup,
                        double diametro_superior,
                        double Nx, double Ny, double Nz)
{
    assert_no_null(datos_dibujo);
    cypegl3d_dibuja_prisma_conico(
                        datos_dibujo->cypegl3d,
                        x_centro_inf, y_centro_inf, z_centro_inf, diametro_inferior,
                        x_centro_sup, y_centro_sup, z_centro_sup, diametro_superior,
                        Nx, Ny, Nz);
}

// ----------------------------------------------------------------------------

static void i_dibuja_triangulo_gradiente3D(
                        struct i_datos_dibujo_3D_t *datos_dibujo,
                        const double x1, const double y1, const double z1, unsigned long rgba1,
                        const double x2, const double y2, const double z2, unsigned long rgba2,
                        const double x3, const double y3, const double z3, unsigned long rgba3)
{
    assert_no_null(datos_dibujo);
    cypegl3d_triangulo_gradiente_3D(datos_dibujo->cypegl3d, x1, y1, z1, rgba1, x2, y2, z2, rgba2, x3, y3, z3, rgba3);
}

// ----------------------------------------------------------------------------

static const struct Escalas_t *i_get_escalas(struct i_datos_dibujo_3D_t *datos_dibujo)
{
    assert_no_null(datos_dibujo);
    assert_no_null(datos_dibujo->escala);

    return datos_dibujo->escala;
}

// ----------------------------------------------------------------------------

static void i_set_escalas(struct i_datos_dibujo_3D_t *datos_dibujo, const struct Escalas_t *escala)
{
    double x_min, y_min, x_max, y_max;

    assert_no_null(datos_dibujo);
    assert_no_null(escala);
    
    if (datos_dibujo->escala != NULL)
        escal_destruye(&(datos_dibujo->escala));

    datos_dibujo->escala = escal_copia(escala);

    escal_getRectMetros(escala, &x_min, &y_min, &x_max, &y_max);
    cypegl3d_zoom(datos_dibujo->cypegl3d, x_min, y_min, x_max, y_max);
}

// ----------------------------------------------------------------------------

static void i_obtener_limites(struct i_datos_dibujo_3D_t *datos_dibujo, double *ox, double *oy, double *ex, double *ey)
{
    assert_no_null(datos_dibujo);
    escal_getLimitesVentana(datos_dibujo->escala, ox, oy, ex, ey);
}

// ----------------------------------------------------------------------

static void i_ajusta_tamanyo_segun_resolucion(struct cypegl3d_t *cypegl3d, double *pixel_m)
{
    unsigned long resolucion_ppp;

    resolucion_ppp = cypegl3d_resolucion_ppp(cypegl3d);

    if (resolucion_ppp > 120)
        *pixel_m = (*pixel_m) * (((resolucion_ppp / 96.) / 2.) + 1.);
}

// ----------------------------------------------------------------------

static double i_tamanyo_punto_visible(struct i_datos_dibujo_3D_t *datos_dibujo)
{
    double pixel_m;

    assert_no_null(datos_dibujo);

    pixel_m = escal_pixelsAMetros(datos_dibujo->escala, 1);
    i_ajusta_tamanyo_segun_resolucion(datos_dibujo->cypegl3d, &pixel_m);

    return pixel_m;
}

// ----------------------------------------------------------------------------

CYBOOL cplangl_se_puede_usar_opengl(void)
{
    return cypegl3d_se_puede_usar_opengl();
}

// ----------------------------------------------------------------------

CYBOOL cplangl_hardware_soporta_sombras(
                    char **vendedor_opengl,
                    char **version_opengl,
                    CYBOOL *con_mips_por_hardware,
                    CYBOOL *es_version_compatible,
                    CYBOOL *es_shader_compilable)
{
    return cypegl3d_hardware_soporta_sombras(
                    vendedor_opengl, version_opengl,
                    con_mips_por_hardware, es_version_compatible, es_shader_compilable);
}

// ----------------------------------------------------------------------

CYBOOL cplangl_compila_codigo_shader(const char *fichero_shader, char **mensaje_error)
{
    return cypegl3d_compila_codigo_shader(fichero_shader, mensaje_error);
}

// ----------------------------------------------------------------------

static void i_tamanyo_pantalla(unsigned long *ancho, unsigned long *alto)
{
    assert_no_null(ancho);
    assert_no_null(alto);

    *ancho = cambtipo_int_a_unsigned_long(GetSystemMetrics(SM_CXSCREEN));
    *alto  = cambtipo_int_a_unsigned_long(GetSystemMetrics(SM_CYSCREEN));
}

// ----------------------------------------------------------------------

static double i_separacion_visible_entre_lineas(struct i_datos_dibujo_3D_t *datos_dibujo)
{
    int pixels;
    double pixels_mts;
    unsigned long ancho, alto;

    assert_no_null(datos_dibujo);

    i_tamanyo_pantalla(&ancho, &alto);

    if(ancho > 1000 && alto > 700)
        pixels = 3;
    else
        pixels = 2;

    pixels_mts = escal_pixelsAMetros(datos_dibujo->escala, pixels);

    return MAX(pixels_mts, 0.01);
}

// ----------------------------------------------------------------------------

static void i_guardar_capas(struct i_datos_dibujo_3D_t *datos_dibujo)
{
    assert_no_null(datos_dibujo);
    gestcapa_guardar_capas(datos_dibujo->gestionCapas);
}

// ----------------------------------------------------------------------
static void i_fin_capas(struct i_datos_dibujo_3D_t *datos_dibujo)
{
    const struct idcolor_t *color_defecto;

    assert_no_null(datos_dibujo);
    gestcapa_fin_grupos_capas(datos_dibujo->gestionCapas);

    arraydxf_obtener_atributos_actuales(datos_dibujo->gestionCapas->datosCapa, NULL, NULL, &color_defecto, NULL, NULL);
    i_escr_color_CypeGL(datos_dibujo->cypegl3d, color_defecto);
}

// ----------------------------------------------------------------------------

static void i_longitud_trozo_texto(
                        struct i_datos_dibujo_3D_t *datos_dibujo,
                        const WCHAR *texto,
                        unsigned short comienzo, unsigned short final,
                        double alto, double ancho,
                        unsigned short estilo, double *longitud_texto)
{
    UNREFERENCED(ancho);
    UNREFERENCED(estilo);
    assert_no_null(datos_dibujo);
    cypegl3d_longitud_trozo_texto(datos_dibujo->cypegl3d, texto, comienzo, final, alto, longitud_texto);
}

// ----------------------------------------------------------------------------

CONSTRUCTOR(static struct cypeplan_t *, i_crea_salida, (
                        enum cplan_salida_t tipo_salida, 
                        unsigned ancho_imagen, unsigned long alto_imagen))
{
    struct cypeplan_t *salida;
    unsigned char capa;
    struct i_datos_dibujo_3D_t *datos_dibujo;

    datos_dibujo = i_creadatos_dibujo_defecto(ancho_imagen, alto_imagen);

    salida = cypeplan_abre_disposicion_generico(
                            tipo_salida,
                            NULL, &datos_dibujo, i_datos_dibujo_3D_t,
                            i_destruye_datos_dibujo,
                            i_define_capa,
                            NULL,  //OnSetClave,
                            i_escr_capa,
                            NULL,  //OnSetPluma,
                            i_escr_color,
                            NULL,
                            NULL,
                            i_escr_tipo_linea,
                            NULL,  //OnSetEstiloTexto,
                            i_guardar_capas,
                            i_fin_capas,
                            i_guardar,
                            i_reponer,
                            NULL,  //OnSetXOR
                            NULL,  //OnSetBorrar
                            NULL,  //OnBorraFondo
                            i_get_escalas,
                            i_set_escalas,
                            i_obtener_limites,
                            i_separacion_visible_entre_lineas,
                            NULL, //i_tamanyo_texto_visible,
                            i_tamanyo_punto_visible,
                            NULL, //cypeplan_FPtr_append_extrusion OnAppendExtrusion
                            NULL); //cypeplan_FPtr_borra_extrusion OnBorraExtrusion

    cypeplan_set_funciones2D(
                            salida, i_datos_dibujo_3D_t,
                            NULL, //OnDibujaLinea, 
                            NULL, //OnDibujaCirculo, 
                            NULL, //OnDibujaArco, 
                            NULL, //OnDibujaPoligono, 
                            NULL, //OnDibujaRegiones, 
                            NULL, //OnDrawRegionesConGradiente
                            NULL, //OnDrawHatch,
                            NULL, //OnDibujaArandela, 
                            NULL, //OnDibujaTexto, 
                            NULL, //OnDibujaImagen,
                            NULL, //OnElipse, 
                            NULL, //OnPunto, 
                            NULL, //OnArcoElipse, 
                            i_longitud_trozo_texto,
                            NULL, //OnRayo, 
                            NULL, //OnLineaReferencia, 
                            NULL, //OnDrawPolilinea,
                            NULL, //OnDibujaGrid, 
                            NULL); //OnDibujaPuntoClave);

    cypeplan_set_funciones3D(
                            salida, i_datos_dibujo_3D_t,
                            i_dibuja_linea3D, 
                            i_dibuja_circulo3D,
                            i_dibuja_arco3D,
                            i_dibuja_polilinea3D,
                            i_dibuja_cara3D,
                            i_dibuja_solid3D,
                            i_dibuja_polilinea_con_cambio_de_color,
                            i_dibuja_texto3D,
                            NULL, //OnSetPlanoTexto
                            i_dibuja_elipse3D,
                            NULL, //OnPunto3D, 
                            NULL, //OnArcoElipse3D,
                            NULL, //OnRayo3D, 
                            NULL, //OnLineaReferencia3D, 
                            i_dibuja_triangulo_gradiente3D,
                            i_dibuja_triangulo_con_normal_por_vertice3D,
                            i_dibuja_mesh,
                            NULL, //i_dibuja_grid3D,
                            i_dibuja_tubo3D,
                            i_dibuja_prisma_conico,
                            NULL); // OnDibujaXRef

    capa = cplan_define_capa_dxf(salida, 0, "DEFECTO", 1, 7, 0, 0);    // Color Negro 
    cplan_escr_capadxf(salida, capa);

    return salida;
}

// ----------------------------------------------------------------------------

struct cypeplan_t *cplangl_crea_salida_3D_imagen(unsigned long ancho_imagen, unsigned long alto_imagen)
{
    enum cplan_salida_t tipo_salida;

    tipo_salida = SALIDA_IMAGEN;
    return i_crea_salida(tipo_salida, ancho_imagen, alto_imagen);
}

// ----------------------------------------------------------------------------

struct cypeplan_t *cplangl_crea_salida_3D_vista(void)
{
    enum cplan_salida_t tipo_salida;
    unsigned long ancho_imagen;
    unsigned long alto_imagen;

    tipo_salida = SALIDA_PANEL;
    ancho_imagen = 0;
    alto_imagen = 0;
    return i_crea_salida(tipo_salida, ancho_imagen, alto_imagen);
}

// ----------------------------------------------------------------------------

static void i_ancho_alto_para_impresora(
                        const struct Escalas_t *escala, 
                        int *ancho_viewport_para_impresora, int *alto_viewport_para_impresora)
{
    escal_getDatosPixels(escala, NULL, NULL, ancho_viewport_para_impresora, alto_viewport_para_impresora);
}

// ----------------------------------------------------------------------------

void cplangl_abre_salida_para_ventana(
                        struct cypeplan_t *salida, 
                        const struct Escalas_t *escala,
                        struct HWND__ * hwnd,
                        unsigned long color_fondo_rgb)
{
    struct i_datos_dibujo_3D_t *datos_dibujo;
    const struct idcolor_t *color_defecto;

    datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);

    assert(datos_dibujo->cypegl3d == NULL);

    datos_dibujo->cypegl3d = cypegl3d_crea_para_ventana(hwnd, color_fondo_rgb);
    datos_dibujo->escala = escal_copia(escala);

    arraydxf_obtener_atributos_actuales(datos_dibujo->gestionCapas->datosCapa, NULL, NULL, &color_defecto, NULL, NULL);
    i_escr_color_CypeGL(datos_dibujo->cypegl3d, color_defecto);
}

// ----------------------------------------------------------------------------

void cplangl_abre_salida_para_dispositivo(
                        struct cypeplan_t *salida,
                        const struct Escalas_t *escala,
                        struct HDC__ * hdc_destino,
                        unsigned long color_fondo_rgb)
{
    struct i_datos_dibujo_3D_t *datos_dibujo;
    const struct idcolor_t *color_defecto;

    datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);

    assert(datos_dibujo->cypegl3d == NULL);

    datos_dibujo->cypegl3d = cypegl3d_crea_para_dispositivo(hdc_destino, color_fondo_rgb);
    datos_dibujo->escala = escal_copia(escala);

    arraydxf_obtener_atributos_actuales(datos_dibujo->gestionCapas->datosCapa, NULL, NULL, &color_defecto, NULL, NULL);
    i_escr_color_CypeGL(datos_dibujo->cypegl3d, color_defecto);
}

// ---------------------------------------------------------------------------

void cplangl_cierra_salida(struct cypeplan_t *salida)
{
    struct i_datos_dibujo_3D_t *datos_dibujo;

    datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);

    cypegl3d_destruye(&datos_dibujo->cypegl3d);
    escal_destruye(&datos_dibujo->escala);
}

// ---------------------------------------------------------------------------

void cplangl_set_color_fondo(struct cypeplan_t *salida, unsigned long color_fondo_rgb)
{
    struct i_datos_dibujo_3D_t *datos_dibujo;

    datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
    assert_no_null(datos_dibujo);

    if (datos_dibujo->cypegl3d != NULL)
        cypegl3d_set_color_fondo(datos_dibujo->cypegl3d, color_fondo_rgb);
}

// ---------------------------------------------------------------------------

void cplangl_inicio_dibujo_3D(struct cypeplan_t *salida, enum cplangl_tipo_dibujo_t tipo_dibujo)
{
    struct i_datos_dibujo_3D_t *datos_dibujo;
    int ancho_viewport_para_impresora, alto_viewport_para_impresora;

    datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
    assert_no_null(datos_dibujo);

    i_ancho_alto_para_impresora(datos_dibujo->escala, &ancho_viewport_para_impresora, &alto_viewport_para_impresora);
    cypegl3d_inicio_dibujo_3D(datos_dibujo->cypegl3d, tipo_dibujo, ancho_viewport_para_impresora, alto_viewport_para_impresora);
}

// ----------------------------------------------------------------------------

void cplangl_cambia_vista_ortogonal(struct cypeplan_t *salida)
{
    struct i_datos_dibujo_3D_t *datos_dibujo;

    datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
    assert_no_null(datos_dibujo);

    cypegl3d_cambia_vista_ortogonal(datos_dibujo->cypegl3d);
}

// ----------------------------------------------------------------------------

void cplangl_cambia_vista_perspectiva_vc(
                        struct cypeplan_t *salida,
                        const ArrPunto3D *nube_de_puntos_sin_proyectar,
                        CYBOOL con_sombra,
                        const ArrPuntero(ArrPunto3D) *puntos_recorrido_dias,
                        const struct proyeccion_t *proyeccion,
                        double x_min_ventana, double y_min_ventana, 
                        double x_max_ventana, double y_max_ventana)
{
    struct i_datos_dibujo_3D_t *datos_dibujo;

    datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
    assert_no_null(datos_dibujo);

    cypegl3d_cambia_vista_perspectiva_vc(
                        datos_dibujo->cypegl3d,
                        nube_de_puntos_sin_proyectar, con_sombra,
                        puntos_recorrido_dias,
                        proyeccion,
                        x_min_ventana, y_min_ventana, 
                        x_max_ventana, y_max_ventana);
}

// ---------------------------------------------------------------------------

void cplangl_avanza(struct cypeplan_t *salida, double dx, double dy, double dz)
{
    struct i_datos_dibujo_3D_t *datos_dibujo;

    datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
    assert_no_null(datos_dibujo);

    cypegl3d_avanza(datos_dibujo->cypegl3d, dx, dy, dz);
}

// ---------------------------------------------------------------------------

void cplangl_gira_camara_eje_x(struct cypeplan_t *salida, double incr_giro_alrededor_eje_x_grados)
{
    struct i_datos_dibujo_3D_t *datos_dibujo;

    datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
    assert_no_null(datos_dibujo);

    cypegl3d_gira_camara_eje_x(datos_dibujo->cypegl3d, incr_giro_alrededor_eje_x_grados);
}

// ---------------------------------------------------------------------------

void cplangl_gira_camara_eje_y(struct cypeplan_t *salida, double incr_giro_alrededor_eje_y_grados)
{
    struct i_datos_dibujo_3D_t *datos_dibujo;

    datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
    assert_no_null(datos_dibujo);

    cypegl3d_gira_camara_eje_y(datos_dibujo->cypegl3d, incr_giro_alrededor_eje_y_grados);
}

// ---------------------------------------------------------------------------

void cplangl_fin_dibujo_3D(struct cypeplan_t *salida)
{
    struct i_datos_dibujo_3D_t *datos_dibujo;

    datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
    assert_no_null(datos_dibujo);
    cypegl3d_fin_dibujo(datos_dibujo->cypegl3d);
}

// ----------------------------------------------------------------------------

void cplangl_inicia_lista_dibujo_con_detalle(
                    struct cypeplan_t *salida, 
                    enum cplangl_tipo_lista_dibujo_t tipo_lista_detalles,
                    const char *id_lista)
{

    struct i_datos_dibujo_3D_t *datos_dibujo;

    datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
    assert_no_null(datos_dibujo);

    cypegl3d_inicia_lista_dibujo(datos_dibujo->cypegl3d, tipo_lista_detalles, id_lista);
}

// ----------------------------------------------------------------------------

void cplangl_inicia_lista_dibujo(
                    struct cypeplan_t *salida, 
                    const char *id_lista)
{

    struct i_datos_dibujo_3D_t *datos_dibujo;

    datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
    assert_no_null(datos_dibujo);

    cypegl3d_inicia_lista_dibujo(datos_dibujo->cypegl3d, CPLANGL_LISTA_CON_MUCHO_DETALLE, id_lista);
}

// ----------------------------------------------------------------------------

void cplangl_fin_lista_dibujo_activo(struct cypeplan_t *salida, const char *id_lista)
{
    struct i_datos_dibujo_3D_t *datos_dibujo;

    datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
    assert_no_null(datos_dibujo);

    cypegl3d_fin_lista_dibujo_activo(datos_dibujo->cypegl3d, id_lista);
}

// ----------------------------------------------------------------------------

void cplangl_dibuja_suelo(struct cypeplan_t *salida, double xmin, double ymin, double xmax, double ymax)
{
    struct i_datos_dibujo_3D_t *datos_dibujo;

    datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
    assert_no_null(datos_dibujo);
    
    cypegl3d_dibuja_suelo(datos_dibujo->cypegl3d, xmin, ymin, xmax, ymax);
}

// ----------------------------------------------------------------------------

void cplangl_dibuja_lista_dibujo(
                        struct cypeplan_t *salida,
                        enum cplangl_tipo_modelado_t tipo_modelado,
                        enum cplangl_offset_zbuffer_t offset_zbuffer,
                        CYBOOL arroja_sombra, const char *id_lista)
{
    struct i_datos_dibujo_3D_t *datos_dibujo;

    datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
    assert_no_null(datos_dibujo);

    cypegl3d_dibuja_lista_dibujo(datos_dibujo->cypegl3d, tipo_modelado, offset_zbuffer, arroja_sombra, id_lista);
}

// ----------------------------------------------------------------------------

void cplangl_destruye_listas_dibujo(struct cypeplan_t *salida)
{
    struct i_datos_dibujo_3D_t *datos_dibujo;

    datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
    assert_no_null(datos_dibujo);

    cypegl3d_destruye_listas_dibujo(datos_dibujo->cypegl3d);
}

// ----------------------------------------------------------------------------

void cplangl_define_tipo_vista(struct cypeplan_t *salida, const struct proyeccion_t *proyeccion)
{
    struct i_datos_dibujo_3D_t *datos_dibujo;
    double giro_x_grados, giro_y_grados, giro_z_grados;
    double x_min, y_min, x_max, y_max;

    datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
    assert_no_null(datos_dibujo);

    proyecta_obtener_proyeccion(proyeccion, &giro_x_grados, &giro_y_grados, &giro_z_grados);
    escal_getRectMetros(datos_dibujo->escala, &x_min, &y_min, &x_max, &y_max);

    cypegl3d_define_vista(datos_dibujo->cypegl3d, x_min, y_min, x_max, y_max, giro_x_grados, giro_y_grados, giro_z_grados);
}

// ----------------------------------------------------------------------------

void cplangl_dibuja_ejes(struct cypeplan_t *salida, const struct proyeccion_t *proyeccion)
{
    struct i_datos_dibujo_3D_t *datos_dibujo;

    datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
    assert_no_null(datos_dibujo);

    cypegl3d_dibuja_ejes_en_esq_inf_izq(datos_dibujo->cypegl3d, proyeccion);
}

// ----------------------------------------------------------------------------

void cplangl_define_viewport_2D(
                        struct cypeplan_t *salida, 
                        int x_origen, int y_origen, 
                        unsigned long xsize, unsigned long ysize,
                        double x_min, double y_min, double x_max, double y_max)
{
    struct i_datos_dibujo_3D_t *datos_dibujo;

    datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
    assert_no_null(datos_dibujo);

    cypegl3d_define_viewport_2D(
                        datos_dibujo->cypegl3d, x_origen, y_origen, xsize, ysize,
                        x_min, y_min, x_max, y_max);
}

// ----------------------------------------------------------------------------

CYBOOL cplangl_se_puede_definir_lista_de_dibujos(struct cypeplan_t *salida)
{
    struct i_datos_dibujo_3D_t *datos_dibujo;

    if (cypeplan_es_datos_dispositivo_base(salida, i_datos_dibujo_3D_t) == CIERTO)
    {
        datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
        assert_no_null(datos_dibujo);

        return cypegl3d_se_puede_definir_lista_de_dibujos(datos_dibujo->cypegl3d);
    }
    else
        return FALSO;
}

// ----------------------------------------------------------------------------

void cplangl_anyadir_plano_clipping(struct cypeplan_t *salida, double coefA, double coefB, double coefC, double coefD)
{
    struct i_datos_dibujo_3D_t *datos_dibujo;

    datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
    assert_no_null(datos_dibujo);

    cypegl3d_anyadir_plano_clipping(datos_dibujo->cypegl3d, coefA, coefB, coefC, coefD);
}

// ----------------------------------------------------------------------------

CYBOOL cplangl_es_salida_opengl(const struct cypeplan_t *salida)
{
    return cypeplan_es_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
}

// ----------------------------------------------------------------------------

CYBOOL cplangl_es_salida_opengl_con_mucho_detalle(const struct cypeplan_t *salida)
{
    if (cypeplan_es_datos_dispositivo_base(salida, i_datos_dibujo_3D_t) == CIERTO)
    {
        struct i_datos_dibujo_3D_t *datos_dibujo;

        datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
        assert_no_null(datos_dibujo);
        
        return cypegl3d_tiene_mucho_detalle(datos_dibujo->cypegl3d);
    }
    else
        return FALSO;
}

// ----------------------------------------------------------------------------

CYBOOL cplangl_se_permite_zoom_animado(struct cypeplan_t *salida)
{
    struct i_datos_dibujo_3D_t *datos_dibujo;

    datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
    assert_no_null(datos_dibujo);

    return cypegl3d_se_permite_zoom_animado(datos_dibujo->cypegl3d);
}

// ----------------------------------------------------------------------------

void cplangl_set_posicion_camara(
                        struct cypeplan_t *salida,
                        double angulo_foco_camara_grados,
                        double x_centro_proyeccion, double y_centro_proyeccion)
{
    struct i_datos_dibujo_3D_t *datos_dibujo;

    datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
    assert_no_null(datos_dibujo);

    cypegl3d_set_posicion_camara(datos_dibujo->cypegl3d, angulo_foco_camara_grados, x_centro_proyeccion, y_centro_proyeccion);
}

// ----------------------------------------------------------------------------

void cplangl_get_posicion_camara(
                        struct cypeplan_t *salida,
                        double *angulo_foco_camara_grados,
                        double *x_centro_proyeccion, double *y_centro_proyeccion)
{
    struct i_datos_dibujo_3D_t *datos_dibujo;

    datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
    assert_no_null(datos_dibujo);

    cypegl3d_get_posicion_camara(datos_dibujo->cypegl3d, angulo_foco_camara_grados, x_centro_proyeccion, y_centro_proyeccion);
}
                        
// ----------------------------------------------------------------------------

void cplangl_calcula_datos_camara(
                        struct cypeplan_t *salida,
                        double x_min, double y_min, double x_max, double y_max,
                        double *angulo_foco_camara_grados,
                        double *x_centro_proyeccion,
                        double *y_centro_proyeccion)
{
    struct i_datos_dibujo_3D_t *datos_dibujo;

    datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
    assert_no_null(datos_dibujo);

    cypegl3d_calcula_datos_camara(
                        datos_dibujo->cypegl3d, x_min, y_min, x_max, y_max,
                        angulo_foco_camara_grados, x_centro_proyeccion, y_centro_proyeccion);
}

// ----------------------------------------------------------------------------

void cplangl_tamanyo_imagen(const struct cypeplan_t *salida, unsigned long *ancho_imagen, unsigned long *alto_imagen)
{
    struct i_datos_dibujo_3D_t *datos_dibujo;

    assert_no_null(ancho_imagen);
    assert_no_null(alto_imagen);
    assert(cplan_salida(salida) == SALIDA_IMAGEN);

    datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
    assert_no_null(datos_dibujo);

    *ancho_imagen = datos_dibujo->ancho_imagen;
    *alto_imagen = datos_dibujo->alto_imagen;
}

// ----------------------------------------------------------------------------

void cplangl_posiciona_luces(
                        const struct cypeplan_t *salida, 
                        double xmin, double ymin, double zmin,
                        double xmax, double ymax, double zmax)
{
    struct i_datos_dibujo_3D_t *datos_dibujo;

    datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
    assert_no_null(datos_dibujo);

    cypegl3d_posiciona_luces(datos_dibujo->cypegl3d, xmin, ymin, zmin, xmax, ymax, zmax);
}

// ----------------------------------------------------------------------------

void cplangl_posiciona_luces_dia(
                        struct cypeplan_t *salida, 
                        CYBOOL es_de_dia,
                        double Ux_luz, double Uy_luz, double Uz_luz,
                        double xmin, double ymin, double zmin,
                        double xmax, double ymax, double zmax)
{
    struct i_datos_dibujo_3D_t *datos_dibujo;

    datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
    assert_no_null(datos_dibujo);

    cypegl3d_posiciona_luces_dia(
                        datos_dibujo->cypegl3d, es_de_dia,
                        Ux_luz, Uy_luz, Uz_luz,
                        xmin, ymin, zmin, xmax, ymax, zmax);
}

// ----------------------------------------------------------------------------

void cplangl_activa_luces(struct cypeplan_t *salida)
{
    struct i_datos_dibujo_3D_t *datos_dibujo;

    datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
    assert_no_null(datos_dibujo);

    cypegl3d_activa_luces(datos_dibujo->cypegl3d);
}

// ----------------------------------------------------------------------------

void cplangl_desactiva_luces(struct cypeplan_t *salida)
{
    struct i_datos_dibujo_3D_t *datos_dibujo;

    datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
    assert_no_null(datos_dibujo);

    cypegl3d_desactiva_luces(datos_dibujo->cypegl3d);
}

// ----------------------------------------------------------------------------

void cplangl_inicia_modo_dibujo_sombras_shaders(
                        struct cypeplan_t *salida,
                        double giro_x_grados, double giro_y_grados, double giro_z_grados,
                        double x_min, double y_min, double z_min, double x_max, double y_max, double z_max)
{
    struct i_datos_dibujo_3D_t *datos_dibujo;

    datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
    assert_no_null(datos_dibujo);

    cypegl3d_inicia_modo_dibujo_sombras_shaders(
                        datos_dibujo->cypegl3d,
                        giro_x_grados, giro_y_grados, giro_z_grados,
                        x_min, y_min, z_min, x_max, y_max, z_max);
}
                        
// ----------------------------------------------------------------------------

void cplangl_fin_modo_dibujo_sombras_shaders(struct cypeplan_t *salida)
{
    struct i_datos_dibujo_3D_t *datos_dibujo;

    datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
    assert_no_null(datos_dibujo);

    cypegl3d_fin_modo_dibujo_sombras_shaders(datos_dibujo->cypegl3d);
}

// ----------------------------------------------------------------------------

void cplangl_inicia_modo_dibujo(const struct cypeplan_t *salida, CYBOOL con_texturas, enum cplangl_tipo_dibujo_t tipo_dibujo)
{
    struct i_datos_dibujo_3D_t *datos_dibujo;

    datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
    assert_no_null(datos_dibujo);

    cypegl3d_inicia_modo_dibujo(datos_dibujo->cypegl3d, con_texturas, tipo_dibujo);
}

// ----------------------------------------------------------------------------

void cplangl_fin_modo_dibujo(const struct cypeplan_t *salida)
{
    struct i_datos_dibujo_3D_t *datos_dibujo;

    datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
    assert_no_null(datos_dibujo);

    cypegl3d_fin_modo_dibujo(datos_dibujo->cypegl3d);
}

// ----------------------------------------------------------------------------

void cplangl_inicia_solo_solidos(struct cypeplan_t *salida)
{
    if (cypeplan_es_datos_dispositivo_base(salida, i_datos_dibujo_3D_t) == CIERTO)
    {
        struct i_datos_dibujo_3D_t *datos_dibujo;

        datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
        assert_no_null(datos_dibujo);

        cypegl3d_inicia_solo_solidos(datos_dibujo->cypegl3d);
    }
}

// ----------------------------------------------------------------------------

void cplangl_fin_solo_solidos(struct cypeplan_t *salida)
{
    if (cypeplan_es_datos_dispositivo_base(salida, i_datos_dibujo_3D_t) == CIERTO)
    {
        struct i_datos_dibujo_3D_t *datos_dibujo;

        datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
        assert_no_null(datos_dibujo);
        cypegl3d_fin_solo_solidos(datos_dibujo->cypegl3d);
    }
}

// ----------------------------------------------------------------------------

void cplangl_inicia_solo_transparencias(struct cypeplan_t *salida)
{
    if (cypeplan_es_datos_dispositivo_base(salida, i_datos_dibujo_3D_t) == CIERTO)
    {
        struct i_datos_dibujo_3D_t *datos_dibujo;

        datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
        assert_no_null(datos_dibujo);

        cypegl3d_inicia_solo_transparencias(datos_dibujo->cypegl3d);
    }
}

// ----------------------------------------------------------------------------

void cplangl_fin_solo_transparencias(struct cypeplan_t *salida)
{
    if (cypeplan_es_datos_dispositivo_base(salida, i_datos_dibujo_3D_t) == CIERTO)
    {
        struct i_datos_dibujo_3D_t *datos_dibujo;

        datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
        assert_no_null(datos_dibujo);
        cypegl3d_fin_solo_transparencias(datos_dibujo->cypegl3d);
    }
}

// ----------------------------------------------------------------------------

void cplangl_inicia_con_transparencias(struct cypeplan_t *salida)
{
    if (cypeplan_es_datos_dispositivo_base(salida, i_datos_dibujo_3D_t) == CIERTO)
    {
        struct i_datos_dibujo_3D_t *datos_dibujo;

        datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
        assert_no_null(datos_dibujo);

        cypegl3d_inicia_con_transparencias(datos_dibujo->cypegl3d);
    }
}

// ----------------------------------------------------------------------------

void cplangl_fin_con_transparencias(struct cypeplan_t *salida)
{
    if (cypeplan_es_datos_dispositivo_base(salida, i_datos_dibujo_3D_t) == CIERTO)
    {
        struct i_datos_dibujo_3D_t *datos_dibujo;

        datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
        assert_no_null(datos_dibujo);
        cypegl3d_fin_con_transparencias(datos_dibujo->cypegl3d);
    }
}

// ----------------------------------------------------------------------------

void cplangl_desplaza_ventana_de_dibujo(const struct cypeplan_t *salida, int dx_origen, int dy_origen)
{
    struct i_datos_dibujo_3D_t *datos_dibujo;

    datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
    assert_no_null(datos_dibujo);

    cypegl3d_desplaza_ventana_de_dibujo(datos_dibujo->cypegl3d, dx_origen, dy_origen);
}

// ----------------------------------------------------------------------------

void cplangl_append_transformaciones(struct cypeplan_t *salida)
{
    struct i_datos_dibujo_3D_t *datos_dibujo;

    datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
    assert_no_null(datos_dibujo);

    cypegl3d_append_transformaciones(datos_dibujo->cypegl3d);
}

// ----------------------------------------------------------------------------

void cplangl_append_sistema_coordenadas(
                        struct cypeplan_t *salida,
                        double Ux, double Uy, double Uz,
                        double Vx, double Vy, double Vz,
                        double Wx, double Wy, double Wz,
                        double Dx, double Dy, double Dz)
{
    struct i_datos_dibujo_3D_t *datos_dibujo;

    datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
    assert_no_null(datos_dibujo);

    cypegl3d_append_transformaciones(datos_dibujo->cypegl3d);
    cypegl3d_set_matriz_transformacion(Ux, Uy, Uz, Vx, Vy, Vz, Wx, Wy, Wz, Dx, Dy, Dz);
}

// ----------------------------------------------------------------------------

void cplangl_pop_transformaciones(struct cypeplan_t *salida)
{
    struct i_datos_dibujo_3D_t *datos_dibujo;

    datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
    assert_no_null(datos_dibujo);

    cypegl3d_pop_transformaciones(datos_dibujo->cypegl3d);
}

// ----------------------------------------------------------------------------

void cplangl_set_desplazamiento(struct cypeplan_t *salida, double dx, double dy, double dz)
{
    struct i_datos_dibujo_3D_t *datos_dibujo;

    datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
    assert_no_null(datos_dibujo);

    cypegl3d_set_desplazamientos(datos_dibujo->cypegl3d, dx, dy, dz);
}

// ----------------------------------------------------------------------------

void cplangl_set_rotacion(struct cypeplan_t *salida, double angulo_grados, double Wx, double Wy, double Wz)
{
    struct i_datos_dibujo_3D_t *datos_dibujo;

    datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
    assert_no_null(datos_dibujo);

    cypegl3d_set_rotacion(datos_dibujo->cypegl3d, angulo_grados, Wx, Wy, Wz);
}

// ----------------------------------------------------------------------------

void cplangl_inicio_transparencia_envoltorio(struct cypeplan_t *salida)
{
    struct i_datos_dibujo_3D_t *datos_dibujo;

    datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
    assert_no_null(datos_dibujo);

    cypegl3d_inicio_transparencia_envoltorios(datos_dibujo->cypegl3d);
}

// ----------------------------------------------------------------------------

void cplangl_fin_transparencia_envoltorios(struct cypeplan_t *salida)
{
    struct i_datos_dibujo_3D_t *datos_dibujo;

    datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
    assert_no_null(datos_dibujo);

    cypegl3d_fin_transparencia_envoltorios(datos_dibujo->cypegl3d);
}

// ----------------------------------------------------------------------------

void cplangl_activa_profundidad(struct cypeplan_t *salida)
{
    struct i_datos_dibujo_3D_t *datos_dibujo;

    datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
    assert_no_null(datos_dibujo);

    cypegl3d_activa_profundidad(datos_dibujo->cypegl3d);
}

// ----------------------------------------------------------------------------

void cplangl_desactiva_profundidad(struct cypeplan_t *salida)
{
    struct i_datos_dibujo_3D_t *datos_dibujo;

    datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
    assert_no_null(datos_dibujo);

    cypegl3d_desactiva_profundidad(datos_dibujo->cypegl3d);
}

// ----------------------------------------------------------------------------

void cplangl_append_material_si_es_necesario(
                        struct cypeplan_t *salida,
                        const char *id_material,
                        enum cplan_material_t material,
                        const unsigned char *bytes_image_rgb,
                        unsigned long xsize, unsigned long ysize)
{
    struct i_datos_dibujo_3D_t *datos_dibujo;

    datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
    assert_no_null(datos_dibujo);

    cypegl3d_append_material_si_es_necesario(datos_dibujo->cypegl3d, id_material, material, bytes_image_rgb, xsize, ysize);
}

// ----------------------------------------------------------------------------

void cplangl_set_material(struct cypeplan_t *salida, enum cplan_material_t material, double factor_transparencia)
{
    if (cypeplan_es_datos_dispositivo(salida, i_datos_dibujo_3D_t) == CIERTO)
    {
        struct i_datos_dibujo_3D_t *datos_dibujo;

        datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
        assert_no_null(datos_dibujo);
        cypegl3d_set_material(datos_dibujo->cypegl3d, material, factor_transparencia);
    }
}

// ----------------------------------------------------------------------------

CYBOOL cplangl_rehacer_bloques(struct cypeplan_t *salida)
{
    struct i_datos_dibujo_3D_t *datos_dibujo;

    datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
    assert_no_null(datos_dibujo);

    return cypegl3d_rehacer_bloques(datos_dibujo->cypegl3d);
}

// ----------------------------------------------------------------------------

CYBOOL cplangl_soporta_sombras(struct cypeplan_t *salida)
{
    if (cypeplan_es_datos_dispositivo_base(salida, i_datos_dibujo_3D_t) == CIERTO)
    {
        struct i_datos_dibujo_3D_t *datos_dibujo;

        datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
        assert_no_null(datos_dibujo);
        
        return cypegl3d_soporta_sombras(datos_dibujo->cypegl3d);
    }
    else
        return FALSO;
}

// ----------------------------------------------------------------------------

void cplangl_dibuja_esfera(struct cypeplan_t *salida, double radio_esfera)
{
    struct i_datos_dibujo_3D_t *datos_dibujo;

    datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
    assert_no_null(datos_dibujo);

    cypegl3d_dibuja_esfera(datos_dibujo->cypegl3d, radio_esfera);
}

// ----------------------------------------------------------------------------

ArrCadena *cplangl_informe_debug(struct cypeplan_t *salida)
{
    struct i_datos_dibujo_3D_t *datos_dibujo;

    datos_dibujo = cypeplan_obtener_datos_dispositivo_base(salida, i_datos_dibujo_3D_t);
    assert_no_null(datos_dibujo);

    return cypeg3d_informe_debug(datos_dibujo->cypegl3d);
}
