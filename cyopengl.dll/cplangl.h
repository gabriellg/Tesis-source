/*
@doc PANELES CPLANGL
@modulo CplanGl.h |
Abre un cypeplan para dibujar en una ventana con funciones de OpenGL

@head3 Funciones |
@index func | CPLANGL
*/

DLL_CYOPENGL CYBOOL cplangl_se_puede_usar_opengl(void);

DLL_CYOPENGL CYBOOL cplangl_hardware_soporta_sombras(
                    char **vendedor_opengl,
                    char **version_opengl,
                    CYBOOL *con_mips_por_hardware,
                    CYBOOL *es_version_compatible,
                    CYBOOL *es_shader_compilable);

DLL_CYOPENGL CYBOOL cplangl_compila_codigo_shader(const char *fichero_shader, char **mensaje_error);

DLL_CYOPENGL CONSTRUCTOR(struct cypeplan_t *, cplangl_crea_salida_3D_imagen, (unsigned long ancho_imagen, unsigned long alto_imagen));

DLL_CYOPENGL CONSTRUCTOR(struct cypeplan_t *, cplangl_crea_salida_3D_vista, (void));

DLL_CYOPENGL void cplangl_abre_salida_para_ventana(struct cypeplan_t *salida, const struct Escalas_t *escalas, struct HWND__ * hwnd, unsigned long color_fondo_rgb);

DLL_CYOPENGL void cplangl_abre_salida_para_dispositivo(struct cypeplan_t *salida, const struct Escalas_t *escala, struct HDC__ * hdc_destino, unsigned long color_fondo_rgb);

DLL_CYOPENGL void cplangl_cierra_salida(struct cypeplan_t *salida);

DLL_CYOPENGL void cplangl_set_color_fondo(struct cypeplan_t *salida, unsigned long color_fondo_rgb);

DLL_CYOPENGL void cplangl_inicio_dibujo_3D(struct cypeplan_t *salida, enum cplangl_tipo_dibujo_t tipo_dibujo);

DLL_CYOPENGL void cplangl_cambia_vista_ortogonal(struct cypeplan_t *salida);

DLL_CYOPENGL void cplangl_cambia_vista_perspectiva_vc(
                        struct cypeplan_t *salida,
                        const ArrPunto3D *nube_de_puntos_sin_proyectar,
                        CYBOOL con_sombra,
                        const ArrPuntero(ArrPunto3D) *puntos_recorrido_dias,
                        const struct proyeccion_t *proyeccion,
                        double x_min_ventana, double y_min_ventana, 
                        double x_max_ventana, double y_max_ventana);

DLL_CYOPENGL void cplangl_avanza(struct cypeplan_t *salida, double dx, double dy, double dz);

DLL_CYOPENGL void cplangl_gira_camara_eje_x(struct cypeplan_t *salida, double incr_giro_alrededor_eje_x_grados);

DLL_CYOPENGL void cplangl_gira_camara_eje_y(struct cypeplan_t *salida, double incr_giro_alrededor_eje_y_grados);

DLL_CYOPENGL void cplangl_fin_dibujo_3D(struct cypeplan_t *salida);

DLL_CYOPENGL void cplangl_inicia_lista_dibujo_con_detalle(struct cypeplan_t *salida, enum cplangl_tipo_lista_dibujo_t tipo_lista_detalles, const char *id_lista);

DLL_CYOPENGL void cplangl_inicia_lista_dibujo(struct cypeplan_t *salida, const char *id_lista);

DLL_CYOPENGL void cplangl_fin_lista_dibujo_activo(struct cypeplan_t *salida, const char *id_lista);

DLL_CYOPENGL void cplangl_dibuja_suelo(struct cypeplan_t *salida, double xmin, double ymin, double xmax, double ymax);

DLL_CYOPENGL void cplangl_dibuja_lista_dibujo(struct cypeplan_t *salida, enum cplangl_tipo_modelado_t tipo_modelado, enum cplangl_offset_zbuffer_t offset_zbuffer, CYBOOL arroja_sombra, const char *id_lista);

DLL_CYOPENGL void cplangl_destruye_listas_dibujo(struct cypeplan_t *salida);

DLL_CYOPENGL void cplangl_define_tipo_vista(struct cypeplan_t *salida, const struct proyeccion_t *proyeccion);

DLL_CYOPENGL void cplangl_dibuja_ejes(struct cypeplan_t *salida, const struct proyeccion_t *proyeccion);

DLL_CYOPENGL void cplangl_define_viewport_2D(
                        struct cypeplan_t *salida, 
                        int x_origen, int y_origen, 
                        unsigned long xsize, unsigned long ysize,
                        double x_min, double y_min, double x_max, double y_max);

DLL_CYOPENGL CYBOOL cplangl_se_puede_definir_lista_de_dibujos(struct cypeplan_t *salida);

DLL_CYOPENGL void cplangl_anyadir_plano_clipping(struct cypeplan_t *salida, double coefA, double coefB, double coefC, double coefD);

DLL_CYOPENGL CYBOOL cplangl_es_salida_opengl(const struct cypeplan_t *salida);

DLL_CYOPENGL CYBOOL cplangl_es_salida_opengl_con_mucho_detalle(const struct cypeplan_t *salida);

DLL_CYOPENGL CYBOOL cplangl_se_permite_zoom_animado(struct cypeplan_t *salida);

DLL_CYOPENGL void cplangl_set_posicion_camara(
                        struct cypeplan_t *salida,
                        double angulo_foco_camara_grados,
                        double x_centro_proyeccion, double y_centro_proyeccion);

DLL_CYOPENGL void cplangl_get_posicion_camara(
                        struct cypeplan_t *salida,
                        double *angulo_foco_camara_grados,
                        double *x_centro_proyeccion, double *y_centro_proyeccion);

DLL_CYOPENGL void cplangl_calcula_datos_camara(
                        struct cypeplan_t *salida,
                        double x_min, double y_min, double x_max, double y_max,
                        double *angulo_foco_camara_grados,
                        double *x_centro_proyeccion,
                        double *y_centro_proyeccion);

DLL_CYOPENGL void cplangl_tamanyo_imagen(const struct cypeplan_t *salida, unsigned long *tam_x_imagen, unsigned long *tam_y_imagen);

DLL_CYOPENGL void cplangl_posiciona_luces(
                        const struct cypeplan_t *salida,
                        double xmin, double ymin, double zmin,
                        double xmax, double ymax, double zmax);

DLL_CYOPENGL void cplangl_posiciona_luces_dia(
                        struct cypeplan_t *salida, 
                        CYBOOL es_de_dia,
                        double Ux_luz, double Uy_luz, double Uz_luz,
                        double xmin, double ymin, double zmin,
                        double xmax, double ymax, double zmax);

DLL_CYOPENGL void cplangl_activa_luces(struct cypeplan_t *salida);

DLL_CYOPENGL void cplangl_desactiva_luces(struct cypeplan_t *salida);

DLL_CYOPENGL void cplangl_inicia_modo_dibujo_sombras_shaders(
                        struct cypeplan_t *salida,
                        double giro_x_grados, double giro_y_grados, double giro_z_grados,
                        double x_min, double y_min, double z_min, double x_max, double y_max, double z_max);

DLL_CYOPENGL void cplangl_fin_modo_dibujo_sombras_shaders(struct cypeplan_t *salida);

DLL_CYOPENGL void cplangl_inicia_modo_dibujo(const struct cypeplan_t *salida, CYBOOL con_texturas, enum cplangl_tipo_dibujo_t tipo_dibujo);

DLL_CYOPENGL void cplangl_fin_modo_dibujo(const struct cypeplan_t *salida);

DLL_CYOPENGL void cplangl_inicia_solo_solidos(struct cypeplan_t *salida);

DLL_CYOPENGL void cplangl_fin_solo_solidos(struct cypeplan_t *salida);

DLL_CYOPENGL void cplangl_inicia_solo_transparencias(struct cypeplan_t *salida);

DLL_CYOPENGL void cplangl_fin_solo_transparencias(struct cypeplan_t *salida);

DLL_CYOPENGL void cplangl_inicia_con_transparencias(struct cypeplan_t *salida);

DLL_CYOPENGL void cplangl_fin_con_transparencias(struct cypeplan_t *salida);

DLL_CYOPENGL void cplangl_desplaza_ventana_de_dibujo(const struct cypeplan_t *salida, int dx_origen, int dy_origen);

DLL_CYOPENGL void cplangl_append_transformaciones(struct cypeplan_t *salida);

DLL_CYOPENGL void cplangl_append_sistema_coordenadas(
                        struct cypeplan_t *salida,
                        double Ux, double Uy, double Uz,
                        double Vx, double Vy, double Vz,
                        double Wx, double Wy, double Wz,
                        double Dx, double Dy, double Dz);

DLL_CYOPENGL void cplangl_pop_transformaciones(struct cypeplan_t *salida);

DLL_CYOPENGL void cplangl_set_desplazamiento(struct cypeplan_t *salida, double dx, double dy, double dz);

DLL_CYOPENGL void cplangl_set_rotacion(struct cypeplan_t *salida, double angulo_grados, double Wx, double Wy, double Wz);

DLL_CYOPENGL void cplangl_inicio_transparencia_envoltorio(struct cypeplan_t *salida);

DLL_CYOPENGL void cplangl_fin_transparencia_envoltorios(struct cypeplan_t *salida);

DLL_CYOPENGL void cplangl_activa_profundidad(struct cypeplan_t *salida);

DLL_CYOPENGL void cplangl_desactiva_profundidad(struct cypeplan_t *salida);

DLL_CYOPENGL void cplangl_append_material_si_es_necesario(
                        struct cypeplan_t *salida,
                        const char *id_material,
                        enum cplan_material_t material,
                        const unsigned char *bytes_image_rgb, 
                        unsigned long xsize, unsigned long ysize);

DLL_CYOPENGL void cplangl_set_material(struct cypeplan_t *salida, enum cplan_material_t material, double factor_transparencia);

DLL_CYOPENGL CYBOOL cplangl_rehacer_bloques(struct cypeplan_t *salida);

DLL_CYOPENGL CYBOOL cplangl_soporta_sombras(struct cypeplan_t *salida);

DLL_CYOPENGL void cplangl_dibuja_esfera(struct cypeplan_t *salida, double radio_esfera);

DLL_CYOPENGL CONSTRUCTOR(ArrCadena *, cplangl_informe_debug, (struct cypeplan_t *salida));
