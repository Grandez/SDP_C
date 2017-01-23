/**
 * @file
 * @brief Obtiene los tiempos del sistema (CPU, System, etc)
 *        en sistemas Windows 32 bits
 * 
 * @author:  Javier Gonzalez
 * @date:    01/03/15
 * @version: 1.0
 *
 */

#ifdef _WIN32

#define _TIMERWIN_SRC_

#include <Windows.h>
#include <stdlib.h>

#include "SDPTRAP.h"
#include "timer.h"


ULARGE_INTEGER beginTime;
ULONG          beginCpu;

int begin = 0; /* Flag para fbegin */

/************************************************/
/***           PROTOTYPES                     ***/
/************************************************/

void FT2ULI(FILETIME *from, ULARGE_INTEGER *to);

/************************************************/
/***  PUBLIC FUNCTIONS                        ***/
/************************************************/

/**
 * Obtiene los tiempos del sistema en una estructura TIMER
 * @param Puntero a la estructura TIMER donde se alamcenaran los tiempos
 *
 */
_EXT_ void win32_getTimers(TIMER *timer) {
	FILETIME systemTime;
	FILETIME createTime;
	FILETIME exitTime;
	FILETIME kernelTime;
	FILETIME userTime;
//    ULARGE_INTEGER sTime;
    ULARGE_INTEGER diff;

    /*
     * Es importante que los tiempos de proceso
     * se tomen antes de obtener el tiempo del sistema
     * Si no es posible que diera mas tiempo de CPU
     * que el tiempo del sistema
     */
	if (GetProcessTimes( GetCurrentProcess(), &createTime, 
		                                      &exitTime, 
											  &kernelTime, 
											  &userTime ) != -1 ) {
        timer->cpuTime = kernelTime.dwLowDateTime 
                       + userTime.dwLowDateTime;
	}

	GetSystemTimeAsFileTime(&systemTime);

    /* Guardar como tiempo del sistema no el tiempo actual
     * si no la diferencia respecto al momento de creacion
     * Como se toma el tiempo una vez iniciado hay que restar
     * el posible tiempo ya consumido en Kernel y User
     */

    if (begin == 0) {
        begin = 1;
        FT2ULI(&createTime, &beginTime);
        beginCpu = timer->cpuTime;
        // DEBUG
        beginCpu = 0;

    }

    FT2ULI(&systemTime, &diff);
    timer->systemTime = diff.LowPart - beginTime.LowPart;
    timer->cpuTime   -= beginCpu;

}

/**
 * Devuelve el timestamp del sistema
 * La estructura FILETIME se emula en Unix
 */
_EXT_ void win32_getTimestamp(FILETIME *ft) {
	GetSystemTimeAsFileTime(ft);
}

/*
_EXT_ ULONG ULI2UL(ULI *uli) {
    return uli->HighPart;
}
*/
_EXT_ TIMER *newTimer(char *nombre) {
    TIMER *t = (TIMER *) malloc(sizeof(TIMER));
    memset(t, 0x0, sizeof(TIMER));
    strcpy(t->label, nombre);
    return t;
}

/************************************************/
/***  PRIVATE FUNCTIONS                       ***/
/************************************************/

/* Convierte FILETIME from to ULARGE to */
void FT2ULI(FILETIME *from, ULARGE_INTEGER *to) {
    to->LowPart  = from->dwLowDateTime;
    to->HighPart = from->dwHighDateTime;
}

#endif