// Calcula los limites de un dibujo.

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct grafico_t *graflimits_crea(void);
    
struct grafico_t *graflimits_crea_con_margen(void);    

void graflimits_obten_limites(struct grafico_t *grafico, double *ox, double *oy, double *ex, double *ey);

#ifdef __cplusplus
}
#endif /* __cplusplus */

