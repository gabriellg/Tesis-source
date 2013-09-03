//opcion.inl

bool opcion_es_opcion(const ArrTipo(opcion_t) *opciones, const char *ejecutar);

void opcion_ejecuta(
				const ArrTipo(opcion_t) *opciones, 
				struct marco_t *marco,
				const char *ejecutar,
				struct opcion_item_datos_t *datos_opcion,
				const char *parametro);

ArrString *opcion_obtener_todo_help(const ArrTipo(opcion_t) *opciones);

ArrString *opcion_obtener_help_comandos(const ArrTipo(opcion_t) *opciones, const ArrString *comandos);

