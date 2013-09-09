
typedef void (*cypeoglframebuffer_FPtr_inicia_variables_shader)(struct cyoglshader_t *shader);

CYBOOL cyoglframebuffer_soporta_FBO(void);

CONSTRUCTOR(struct cyoglframebuffer_t *, cyoglframebuffer_crea_fbo_profundidad, (unsigned long xsize, unsigned long ysize));

CONSTRUCTOR(struct cyoglframebuffer_t *, cyoglframebuffer_crea_fbo_color, (unsigned long xsize, unsigned long ysize));

void cyoglframebuffer_destruye(struct cyoglframebuffer_t **cyoglframebuffer);

void cyoglframebuffer_activa_frame(struct cyoglframebuffer_t *cyoglframebuffer);

void cyoglframebuffer_desactiva_frame(struct cyoglframebuffer_t *cyoglframebuffer);

void cyoglframebuffer_activa_frame_con_shader(struct cyoglframebuffer_t *cyoglframebuffer, struct cyoglshader_t *shader);

void cyoglframebuffer_desactiva_con_shader(struct cyoglframebuffer_t *cyoglframebuffer, struct cyoglshader_t *shader);

void cyoglframebuffer_aplica_textura(const struct cyoglframebuffer_t *cyoglframebuffer, unsigned int unidad_textura);

void cyoglframebuffer_dibuja_framebuffer_debug(
                        const struct cyoglframebuffer_t *cyoglframebuffer, 
                        unsigned long ind_ventana,
                        long x_viewport, long y_viewport,
                        unsigned long ancho_viewport, unsigned long alto_viewport);

void cyoglframebuffer_procesa_buffers(
                        const struct cyoglframebuffer_t *cyoglframebuffer_1, 
                        const struct cyoglframebuffer_t *cyoglframebuffer_2, 
                        struct cyoglframebuffer_t *cyoglframebuffer_resultado, 
                        struct cyoglshader_t *shader,
                        const char *nombre_textura_1, const char *nombre_textura_2,
                        unsigned long x_size_procesamiento, unsigned long y_size_procesamiento,
                        cypeoglframebuffer_FPtr_inicia_variables_shader func_inicia_variables_shader_opc);
