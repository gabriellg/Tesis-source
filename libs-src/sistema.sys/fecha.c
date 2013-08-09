// Estructura que nos define una fecha.

#include "fecha.h"

#include "time.h"
#include "memory.h"
#include "asrtbas.h"
#include "strbas.h"

struct fecha_t
{
    time_t tiempo;
};

//---------------------------------------------------------

static void i_integridad(const struct fecha_t *fecha)
{
    assert_no_null(fecha);
}

//---------------------------------------------------------

static struct fecha_t *i_crea(time_t tiempo)
{
    struct fecha_t *fecha;

    fecha = MALLOC(struct fecha_t);
        
    fecha->tiempo = tiempo;
    
    return fecha;
}

//---------------------------------------------------------

struct fecha_t *fecha_crea_ahora(void)
{
    time_t ahora;

    ahora = time(NULL);
    assert(ahora >= 0);
    
    return i_crea(ahora);
}

//---------------------------------------------------------

struct fecha_t *fecha_copia(const struct fecha_t *fecha)
{
    i_integridad(fecha);
    return i_crea(fecha->tiempo);
}

//---------------------------------------------------------

struct fecha_t *fecha_dia_mes_anyo(
                        unsigned long dia, 
                        unsigned long mes, 
                        unsigned long anyo, 
                        unsigned long hora, 
                        unsigned long minuto)
{
    struct tm tiempo_descompuesto;
    time_t tiempo;

    assert(anyo >= 1900);
    assert(mes > 0);
    assert(mes <= 12);
    
    tiempo_descompuesto.tm_sec = 0;
    tiempo_descompuesto.tm_min = minuto;
    tiempo_descompuesto.tm_hour = hora;
    tiempo_descompuesto.tm_mday = dia;
    tiempo_descompuesto.tm_mon = mes - 1;
    tiempo_descompuesto.tm_year = anyo - 1900;
    
    tiempo = mktime(&tiempo_descompuesto);
    assert(tiempo != -1);
    
    return i_crea(tiempo);
}

//---------------------------------------------------------

void fecha_destruye(struct fecha_t **fecha)
{
    assert_no_null(fecha);
    i_integridad(*fecha);
    
    FREE_T(fecha, struct fecha_t);
}

//---------------------------------------------------------

static struct tm *i_tiempo_descompuesto_que_no_hay_que_destruir(const time_t *tiempo)
{
    assert_no_null(tiempo);
    return localtime(tiempo);
}

//---------------------------------------------------------

static char *i_texto_fecha_con_formato(const time_t *tiempo, const char *formato)
{
    char buffer[250];
    struct tm *tiempo_descompuesto;
    size_t size;

    tiempo_descompuesto = i_tiempo_descompuesto_que_no_hay_que_destruir(tiempo);
    
    size = strftime(buffer, 250, formato, tiempo_descompuesto);
    assert(size > 0);
    
    return strbas_copyString(buffer);
}

//---------------------------------------------------------

unsigned long i_interpreta_anyo(int anyo)
{
    return anyo + 1900;
}

//---------------------------------------------------------

unsigned long fecha_anyo(const struct fecha_t *fecha)
{
    struct tm *tiempo_descompuesto;
    
    i_integridad(fecha);
    
    tiempo_descompuesto = i_tiempo_descompuesto_que_no_hay_que_destruir(&fecha->tiempo);
    assert_no_null(tiempo_descompuesto);
    
    return i_interpreta_anyo(tiempo_descompuesto->tm_year);
}

//---------------------------------------------------------

char *fecha_texto_dia(const struct fecha_t *fecha)
{
    i_integridad(fecha);
    return i_texto_fecha_con_formato(&fecha->tiempo, "%d/%m/%Y");
}

//---------------------------------------------------------

char *fecha_texto_hora(const struct fecha_t *fecha)
{
    i_integridad(fecha);
    return i_texto_fecha_con_formato(&fecha->tiempo, "%R");
}

//---------------------------------------------------------

char *fecha_texto_dia_y_hora(const struct fecha_t *fecha)
{
    i_integridad(fecha);
    return i_texto_fecha_con_formato(&fecha->tiempo, "%d/%m/%Y %R");
}

//---------------------------------------------------------

void fecha_get(
            const struct fecha_t *fecha, 
            unsigned long *dia, unsigned long *mes, unsigned long *anyo,
            unsigned long *hora, unsigned long *minutos)
{
    struct tm *tiempo_descompuesto;
        
    i_integridad(fecha);
    
    tiempo_descompuesto = i_tiempo_descompuesto_que_no_hay_que_destruir(&fecha->tiempo);
    assert_no_null(tiempo_descompuesto);

    if (dia != NULL)
        *dia = tiempo_descompuesto->tm_mday;

    if (mes != NULL)
        *mes = tiempo_descompuesto->tm_mon + 1;

    if (anyo != NULL)
        *anyo = i_interpreta_anyo(tiempo_descompuesto->tm_year);

    if (hora != NULL)
        *hora = tiempo_descompuesto->tm_hour;
    
    if (minutos != NULL)
        *minutos = tiempo_descompuesto->tm_min;
}
