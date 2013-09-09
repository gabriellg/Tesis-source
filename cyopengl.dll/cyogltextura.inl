//Definición de texturas.

void cyogltextura_hardware_texturas(CYBOOL *con_mips_por_hardware, CYBOOL *con_varias_unidades_texturas);

unsigned int cyogltextura_crea_textura_profundidad_shadow(unsigned long ancho_sombra, unsigned long alto_sombra);

unsigned int cyogltextura_crea_textura_datos_resultado(unsigned long ancho_sombra, unsigned long alto_sombra);

unsigned int cyogltextura_crea_textura_imagen_repetida(CYBOOL con_mips_por_hardware, const unsigned char *bytes_image_rgb, unsigned long xsize, unsigned long ysize);

void cyogltextura_destruye_textura(unsigned int *id_textura);

void cyogltextura_destruye_varias_texturas(unsigned long num_texturas, unsigned int **id_texturas);

void cyogltextura_activa_textura(unsigned int id_textura, unsigned int unidad_textura);

void cyogltextura_desactiva_textura(unsigned int unidad_textura);

void cyogltextura_dibujo_textura_profundidad_debug(
                        unsigned int id_textura_profundidad,
                        unsigned long ind_ventana,
                        long x_viewport, long y_viewport,
                        unsigned long ancho_viewport, unsigned long alto_viewport);
