
CYBOOL cyoglshader_soporta_shaders(void);

CONSTRUCTOR(struct cyoglshader_t *, cyoglshader_crea_defecto, (const char *id_program));

void cyoglshader_destruye(struct cyoglshader_t **cyoglshader);

void cyoglshader_append_vertex_shader(struct cyoglshader_t *cyoglshader, const char *shader_code);

void cyoglshader_append_fragment_shader(struct cyoglshader_t *cyoglshader, const char *shader_code);

CYBOOL cyoglshader_compila_y_linka(struct cyoglshader_t *cyoglshader, char **mensaje_error);

void cyoglshader_append_variable_uniform(struct cyoglshader_t *cyoglshader, const char *nombre_variable);

CYBOOL cyoglshader_se_puede_definir_variable(struct cyoglshader_t *cyoglshader, const char *nombre_variable);

void cyoglshader_ejecuta_shader(struct cyoglshader_t *cyoglshader);

void cyoglshader_parar_shader(struct cyoglshader_t *cyoglshader);

void cyoglshader_set_variable_uniform_int(struct cyoglshader_t *cyoglshader, const char *nombre_variable, int valor);

void cyoglshader_set_variable_uniform_uint(struct cyoglshader_t *cyoglshader, const char *nombre_variable, unsigned int valor);

void cyoglshader_set_variable_uniform_CYBOOL(struct cyoglshader_t *cyoglshader, const char *nombre_variable, CYBOOL valor);

void cyoglshader_set_variable_uniform_matriz4x4(struct cyoglshader_t *cyoglshader, const char *nombre_variable, const float matriz[16]);

void cyoglshader_set_variable_uniform_float(struct cyoglshader_t *cyoglshader, const char *nombre_variable, float valor);

CYBOOL cyoglshader_compila_codigo_shader(const char *fichero_shader, char **mensaje_error);
