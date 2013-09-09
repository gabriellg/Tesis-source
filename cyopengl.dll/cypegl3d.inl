// Renderización OpenGL

CYBOOL cypegl3d_se_puede_usar_opengl(void);

CYBOOL cypegl3d_hardware_soporta_sombras(
                    char **vendedor_opengl,
                    char **version_opengl,
                    CYBOOL *con_mips_por_hardware,
                    CYBOOL *es_version_compatible,
                    CYBOOL *es_shader_compilable);

CYBOOL cypegl3d_compila_codigo_shader(const char *fichero_shader, char **mensaje_error);

CONSTRUCTOR(struct cypegl3d_t *, cypegl3d_crea_para_ventana, (struct HWND__ * hwnd, unsigned long color_fondo_rgb));

CONSTRUCTOR(struct cypegl3d_t *, cypegl3d_crea_para_dispositivo, (struct HDC__ * hdc_destino, unsigned long color_fondo_rgb));

void cypegl3d_destruye(struct cypegl3d_t **cypegl3d);

void cypegl3d_set_color_fondo(struct cypegl3d_t *cypegl3d, unsigned long color_fondo_rgb);

// Prepración dibujo...

void cypegl3d_define_vista(
                        struct cypegl3d_t *cypegl3d, 
                        double x_min_orto, double y_min_orto, double x_max_orto, double y_max_orto,
                        double giro_x_grados, double giro_y_grados, double giro_z_grados);

void cypegl3d_cambia_vista_ortogonal(struct cypegl3d_t *cypegl3d);

void cypegl3d_cambia_vista_perspectiva_vc(
                        struct cypegl3d_t *cypegl3d,
                        const ArrPunto3D *nube_de_puntos_sin_proyectar,
                        CYBOOL con_sombra,
                        const ArrPuntero(ArrPunto3D) *puntos_recorrido_dias,
                        const struct proyeccion_t *proyeccion,
                        double x_min_ventana, double y_min_ventana, 
                        double x_max_ventana, double y_max_ventana);

void cypegl3d_zoom(struct cypegl3d_t *cypegl3d, double x_min, double y_min, double x_max, double y_max);

CYBOOL cypegl3d_se_permite_zoom_animado(struct cypegl3d_t *cypegl3d);

void cypegl3d_set_posicion_camara(struct cypegl3d_t *cypegl3d, double angulo_foco_camara_grados, double x_centro_proyeccion, double y_centro_proyeccion);

void cypegl3d_get_posicion_camara(struct cypegl3d_t *cypegl3d, double *angulo_foco_camara_grados, double *x_centro_proyeccion, double *y_centro_proyeccion);

void cypegl3d_calcula_datos_camara(
                        struct cypegl3d_t *cypegl3d,
                        double x_min, double y_min, double x_max, double y_max,
                        double *angulo_foco_camara_grados, double *x_centro_proyeccion, double *y_centro_proyeccion);

void cypegl3d_avanza(struct cypegl3d_t *cypegl3d, double dx, double dy, double dz);

void cypegl3d_gira_camara_eje_x(struct cypegl3d_t *cypegl3d, double incr_giro_alrededor_eje_x_grados);

void cypegl3d_gira_camara_eje_y(struct cypegl3d_t *cypegl3d, double incr_giro_alrededor_eje_y_grados);

void cypegl3d_inicio_dibujo_3D(struct cypegl3d_t *cypegl3d, enum cplangl_tipo_dibujo_t tipo_dibujo, int ancho_viewport_para_impresora, int alto_viewport_para_impresora);


void cypegl3d_fin_dibujo(struct cypegl3d_t *cypegl3d);

// Primitivas de dibujo...

void cypegl3d_guardar(struct cypegl3d_t *cypegl3d);

void cypegl3d_reponer(struct cypegl3d_t *cypegl3d);

void cypegl3d_set_color(struct cypegl3d_t *cypegl3d, unsigned char color);

void cypegl3d_set_color_rgba(struct cypegl3d_t *cypegl3d, unsigned long color_rgba);

void cypegl3d_set_tipo_linea(struct cypegl3d_t *cypegl3d, const unsigned char tipo);

void cypegl3d_linea3D(struct cypegl3d_t *cypegl3d, double x1, double y1, double z1, double x2, double y2, double z2);

void cypegl3d_polilinea_con_cambio_de_color(struct cypegl3d_t *cypegl3d, const ArrPunto3D *polilinea, const ArrULong *rgba);

void cypegl3d_polilinea3D(struct cypegl3d_t *cypegl3d, const ArrPunto3D *polilinea);

void cypegl3d_poligono3D(struct cypegl3d_t *cypegl3d, const ArrPunto3D *poligono);

void cypegl3d_quad(struct cypegl3d_t *cypegl3d, 
                        double x1, double y1, double z1, CYBOOL es_visible_arista1,
                        double x2, double y2, double z2, CYBOOL es_visible_arista2,
                        double x3, double y3, double z3, CYBOOL es_visible_arista3,
                        double x4, double y4, double z4, CYBOOL es_visible_arista4);

void cypegl3d_dibuja_texto3D(
                        struct cypegl3d_t *cypegl3d,
                        const WCHAR *texto,
                        double x, double y, double z,
                        unsigned char just,
                        double alto,
                        double Ux, double Uy,
                        double longitud);

void cypegl3d_longitud_trozo_texto(
                        struct cypegl3d_t *cypegl3d, 
                        const WCHAR *texto, 
                        unsigned short comienzo, unsigned short final, 
                        double alto, double *longitud_texto);

void cypegl3d_posiciona_luces(
                        struct cypegl3d_t *cypegl3d,
                        double xmin, double ymin, double zmin,
                        double xmax, double ymax, double zmax);

void cypegl3d_posiciona_luces_dia(
                        struct cypegl3d_t *cypegl3d,
                        CYBOOL es_de_dia,
                        double Ux_luz, double Uy_luz, double Uz_luz,
                        double xmin, double ymin, double zmin,
                        double xmax, double ymax, double zmax);

void cypegl3d_circulo(struct cypegl3d_t *cypegl3d, double xc, double yc, double zc, double radio, CYBOOL relleno);

void cypegl3d_arco3D(
                    struct cypegl3d_t *cypegl3d, 
                    double xc, double yc, double zc, 
                    double x1, double y1, double z1, 
                    double x2, double y2, double z2, 
                    CYBOOL relleno);

void cypegl3d_elipse(
                    struct cypegl3d_t *cypegl3d, 
                    double xc, double yc, double zc, 
                    double ra, double rb, 
                    double angulo_rotacion, CYBOOL relleno);

void cypegl3d_triangulo3D_orientado(
                    struct cypegl3d_t *cypegl3d,
                    double x1, double y1, double z1, double Wx1, double Wy1, double Wz1, 
                    double x2, double y2, double z2, double Wx2, double Wy2, double Wz2, 
                    double x3, double y3, double z3, double Wx3, double Wy3, double Wz3);

void cypegl3d_bandas_triangulos(
                    struct cypegl3d_t *cypegl3d,
                    const ArrPunto3D *puntos,
                    const ArrPunto3D *normales,
                    const ArrPunto2D *coord_texturas,
                    const ArrBool *es_borde,
                    const ArrEnum(cplan_tipo_primitiva_t) *tipo_primitiva,
                    const ArrPuntero(ArrULong) *inds_todas_las_caras);

void cypegl3d_dibuja_tubos(struct cypegl3d_t *cypegl3d, const ArrPunto3D *puntos, double diametro);

void cypegl3d_dibuja_prisma_conico(
                        struct cypegl3d_t *cypegl3d,
                        double x_centro_inf, double y_centro_inf, double z_centro_inf,
                        double diametro_inferior,
                        double x_centro_sup, double y_centro_sup, double z_centro_sup,
                        double diametro_superior,
                        double Nx, double Ny, double Nz);

void cypegl3d_triangulo_gradiente_3D(
                        struct cypegl3d_t *cypegl3d,
                        double x1, double y1, double z1, unsigned long rgba1,
                        double x2, double y2, double z2, unsigned long rgba2,
                        double x3, double y3, double z3, unsigned long rgba3);

void cypegl3d_inicia_lista_dibujo(
                        struct cypegl3d_t *cypegl3d, 
                        enum cplangl_tipo_lista_dibujo_t tipo_lista_dibujo,
                        const char *id_lista);

void cypegl3d_fin_lista_dibujo_activo(struct cypegl3d_t *cypegl3d, const char *id_lista);

void cypegl3d_dibuja_suelo(struct cypegl3d_t *cypegl3d, double xmin, double ymin, double xmax, double ymax);

void cypegl3d_dibuja_lista_dibujo(
                        struct cypegl3d_t *cypegl3d, 
                        enum cplangl_tipo_modelado_t tipo_modelado,
                        enum cplangl_offset_zbuffer_t offset_zbuffer,
                        CYBOOL arroja_sombra,
                        const char *id_lista);

void cypegl3d_dibuja_lista_dibujo_con_material(
                        struct cypegl3d_t *cypegl3d, 
                        enum cplangl_tipo_modelado_t tipo_modelado,
                        enum cplan_material_t material,
                        double factor_transparencia,
                        enum cplangl_offset_zbuffer_t offset_zbuffer,
                        const char *id_lista);

void cypegl3d_destruye_listas_dibujo(struct cypegl3d_t *cypegl3d);

void cypegl3d_dibuja_ejes_en_esq_inf_izq(struct cypegl3d_t *cypegl3d, const struct proyeccion_t *proyeccion);

void cypegl3d_define_viewport_2D(
                        struct cypegl3d_t *cypegl3d, 
                        int x_origen, int y_origen, unsigned long xsize, unsigned long ysize,
                        double x_min, double y_min, double x_max, double y_max);

CYBOOL cypegl3d_se_puede_definir_lista_de_dibujos(const struct cypegl3d_t *cypegl3d);

unsigned long cypegl3d_resolucion_ppp(const struct cypegl3d_t *cypegl3d);

void cypegl3d_anyadir_plano_clipping(struct cypegl3d_t *cypegl3d, double A, double B, double C, double D);

void cypegl3d_inicia_modo_dibujo_sombras_shaders(
                        struct cypegl3d_t *cypegl3d,
                        double giro_x_grados, double giro_y_grados, double giro_z_grados,
                        double x_min, double y_min, double z_min, double x_max, double y_max, double z_max);

void cypegl3d_fin_modo_dibujo_sombras_shaders(struct cypegl3d_t *cypegl3d);

void cypegl3d_inicia_modo_dibujo(struct cypegl3d_t *cypegl3d, CYBOOL con_texturas, enum cplangl_tipo_dibujo_t tipo_dibujo);

void cypegl3d_fin_modo_dibujo(struct cypegl3d_t *cypegl3d);

void cypegl3d_inicia_solo_solidos(struct cypegl3d_t *cypegl3d);

void cypegl3d_fin_solo_solidos(struct cypegl3d_t *cypegl3d);

void cypegl3d_inicia_solo_transparencias(struct cypegl3d_t *cypegl3d); 

void cypegl3d_fin_solo_transparencias(struct cypegl3d_t *cypegl3d);

void cypegl3d_inicia_con_transparencias(struct cypegl3d_t *cypegl3d);

void cypegl3d_fin_con_transparencias(struct cypegl3d_t *cypegl3d);

CYBOOL cypegl3d_tiene_mucho_detalle(const struct cypegl3d_t *cypegl3d);

void cypegl3d_desplaza_ventana_de_dibujo(struct cypegl3d_t *cypegl3d, int dx_origen, int dy_origen);

void cypegl3d_append_transformaciones(struct cypegl3d_t *cypegl3d);

void cypegl3d_pop_transformaciones(struct cypegl3d_t *cypegl3d);

void cypegl3d_set_matriz_transformacion(
                        double Ux, double Uy, double Uz,
                        double Vx, double Vy, double Vz,
                        double Wx, double Wy, double Wz,
                        double Dx, double Dy, double Dz);

void cypegl3d_set_desplazamientos(struct cypegl3d_t *cypegl3d, double dx, double dy, double dz);

void cypegl3d_set_rotacion(struct cypegl3d_t *cypegl3d, double angulo_grados, double Wx, double Wy, double Wz);

void cypegl3d_inicio_transparencia_envoltorios(struct cypegl3d_t *cypegl3d);

void cypegl3d_fin_transparencia_envoltorios(struct cypegl3d_t *cypegl3d);

void cypegl3d_activa_profundidad(struct cypegl3d_t *cypegl3d);

void cypegl3d_desactiva_profundidad(struct cypegl3d_t *cypegl3d);

void cypegl3d_append_material_si_es_necesario(
                        struct cypegl3d_t *cypegl3d,
                        const char *id_material,
                        enum cplan_material_t material,
                        const unsigned char *bytes_image_rgb,
                        unsigned long xsize, unsigned long ysize);

void cypegl3d_set_material(struct cypegl3d_t *cypegl3d, enum cplan_material_t material, double factor_transparencia);

CYBOOL cypegl3d_rehacer_bloques(const struct cypegl3d_t *cypegl3d);

CYBOOL cypegl3d_soporta_sombras(const struct cypegl3d_t *cypegl3d);

void cypegl3d_activa_luces(struct cypegl3d_t *cypegl3d);

void cypegl3d_desactiva_luces(struct cypegl3d_t *cypegl3d);

void cypegl3d_dibuja_esfera(struct cypegl3d_t *cypegl3d, double radio_esfera);

ArrCadena *cypeg3d_informe_debug(struct cypegl3d_t *cypegl3d);
