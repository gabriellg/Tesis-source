// Estructura que nos define una fecha.

struct fecha_t *fecha_crea_ahora(void);

struct fecha_t *fecha_dia_mes_anyo(
                        unsigned long dia, 
                        unsigned long mes, 
                        unsigned long anyo, 
                        unsigned long hora, 
                        unsigned long minuto);

struct fecha_t *fecha_copia(const struct fecha_t *fecha);

void fecha_destruye(struct fecha_t **fecha);

unsigned long fecha_anyo(const struct fecha_t *fecha);

char *fecha_texto_dia(const struct fecha_t *fecha);

char *fecha_texto_hora(const struct fecha_t *fecha);
    
char *fecha_texto_dia_y_hora(const struct fecha_t *fecha);

void fecha_get(const struct fecha_t *fecha, unsigned long *dia, unsigned long *mes, unsigned long *anyo, unsigned long *hora, unsigned long *minutos);
