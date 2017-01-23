/**
 * @file
 * @brief Obtiene los tiempos del sistema en una estructura TIMER
 *        Version Linux
 *
 * @author:  Javier Gonzalez
 * @date:    01/03/15
 * @version: 1.0
 */

#ifdef __unix__

#define _TIMERLNX_SRC_

#include <stdio.h>
#include <sys/resource.h>

#include "jggsal.h"

#include "SDPTRAP.h"
#include "timer.h"


ULONG  beginTime;
ULONG  beginCpu;

int begin = 0; /* Flag para fbegin */

/**
 * Obtiene los tiempos del sistema en una estructura TIMER
 * @param Puntero a la estructura TIMER donde se alamcenaran los tiempos
 *
 */
_EXT_ void linux_getTimers(TIMER *timer) {

	struct rusage  rusage;
    struct timeval tv;

    /*
     * Es importante que los tiempos de proceso
     * se tomen antes de obtener el tiempo del sistema
     * Si no es posible que diera mas tiempo de CPU
     * que el tiempo del sistema
     */

	if ( getrusage( RUSAGE_CHILDREN, &rusage ) != -1 ) {
         timer->cpuTime =  (rusage.ru_utime.tv_sec  + rusage.ru_stime.tv_sec) *  1000000;
         timer->cpuTime += (rusage.ru_utime.tv_usec + rusage.ru_stime.tv_usec);
	}

    /* Guardar como tiempo del sistema no el tiempo actual
     * si no la diferencia respecto al momento de creacion
     * Como se toma el tiempo una vez iniciado hay que restar
     * el posible tiempo ya consumido en Kernel y User
     */

    if (begin == 0) {
        begin = 1;
        beginTime = 0;
        beginCpu = 0;

    }

    gettimeofday(&tv, NULL);
    timer->systemTime = (tv.tv_sec * 1000000) + tv.tv_usec;

}

/**
 * Devuelve el timestamp del sistema
 * La estructura FILETIME se emula en Unix
 */

_EXT_ void linux_getTimestamp(FILETIME *ft) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    ft->dwHighDateTime = 0;
    ft->dwLowDateTime = (tv.tv_sec * 1000000) + tv.tv_usec;
}
#endif