// Funciones para usar con tiempo de procesamiento.

#include "unistd.h"
#include "time.h"

//---------------------------------------------------------------

void timesys_sleep(unsigned long milliseconds)
{
    struct timespec ts;
    
    ts.tv_sec = (time_t)(milliseconds / 1000);
    ts.tv_nsec = (milliseconds % 1000) * 1000 * 1000;
    
    nanosleep (&ts, NULL);
}
