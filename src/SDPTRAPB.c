/**
 * @file
 * @brief SDPTRAPB DLL de captura de datos de ejecucion del programa COBOL
 *        Utiliza un unico punto de entrada: SDPTRAP
 * 
 * Modo 0: Acumula toda la informacion en memoria y la envia al final del proceso
 * Modo 1: Acumula la informacion en memoria y la envia cada modulo
 * Modo 2: Va enviando la informacion segun se va produciendo
 *
 * @author:  Javier Gonzalez
 * @date:    01/03/15
 * @version: 1.0
 *
 */

#define _MAIN_MODULE_
#define _SDPTRAPB_C_

#define NO_STDIO_REDIRECT

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "jggsal.h"

#include "SDPTRAPB.h"
#include "SDPTRAP.h" 

#include "cob2c.h"
#include "modulos.h"
#include "timer.h"
#include "messages.h"
#include "md5.h"
#include "amqsdp.h"

#include "global.h"

TIMER  current;                           /* Datos en el momento actual  */
TIMER  ultimo = {0,"",0,0,0,0,0,0x0} ;    /* Ultimos datos resumidos     */
FILE  *traza = 0x0;

/************************************************/
/***           PROTOTYPES                     ***/
/************************************************/

void startProcess      (int option);
void endProcess        (int option, char *sdpData);
void calculateTimers   (TIMER *timer);

void initSystem        (void);
void cleanSystem       (void);

void nuevoModulo       (void);

/************************************************/
/***  PUBLIC FUNCTIONS (DLL)                  ***/
/************************************************/

/**
 * Usado para pruebas unitarias y de integracion
 */
SDPTRAP_DLL void SDPTRAPB_setDebugLevel(int level) {
    dbgLevel = level;
}

/**
 * Usado para pruebas unitarias y de integracion
 */
SDPTRAP_DLL void SDPTRAPB_setModo(int mode) {
    modo = mode;
}

/**
  * @brief Punto de entrada a la libreria SDPTRAPB
  *
  * @param option  Una combinacion de los valores definidos en MSG_xxx
  * @param len     Longitud de la etiqueta, 0 si no es relevante
  * @param sdpData Puntero a la estructura SDP
  * @param modulo  Etiqueta (parrafo, rutina, etc.)
  */
SDPTRAP_DLL void SDPTRAPB(int *option, int *len, char *sdpData, char *modulo) {
//int idx;
    int tipo = *option & 0xFF; /* Quitar el byte superior */
 
	if (inErr != 0) return;

    if (dbgLevel > 0 && traza == 0x0) {
        traza = fopen("sdptrab.log", "at");
        setvbuf(traza, NULL, _IONBF, 0);
    }

    if (dbgLevel & DBG_CALL) fprintf(traza, "CALL  : %d-%d-%s\n", tipo, *option, modulo);

    // Captura los tiempos actuales en current
    getTimers(&current);
    if (dbgLevel & DBG_TIMER) {
        fprintf(traza, "TIMER : %ld-%ld\n", current.systemTime, current.cpuTime);
    }
    
    /* Actualiza la variabel global SDP sdp */
	copyCobolToCStruct(&sdp, sdpData, modulo, *len);

	/* Las opciones acabadas es 0 son inicio, si no son finales */

	if (tipo & END_MARK) {
		endProcess(tipo, sdpData);
	}
	else {
        startProcess(tipo);
	}
}

/************************************************/
/***          PRIVATE FUNCTIONS               ***/
/************************************************/

/*
 * Mensajes de inicio
 *
 * La primera vez se detecta por que las pilas son nulas
 * Entonces crea el identificador de la sesion
 */

void startProcess(int tipo) {
//    char data[1024];

    if (pilaTimers == 0x0)           initSystem();
    if (tipo & MODULE)               nuevoModulo();

    strcpy(current.label,sdp.label);
    current.tipo = tipo;

    if (stackDepth(pilaTimers) == 0) sendBeginSession(&current);

    if (dbgLevel & DBG_PUSH) {
        fprintf(traza, "PUSH  : %d-%s-%ld-%ld\n", 
                       current.tipo, current.label, current.systemTime, current.cpuTime);
    }
    push(pilaTimers, &current);
}

/*
 * Mensaje de fin
 *
 * En el caso de fin de modulo, puede que haya parrafos pendientes
 * por lo que tambien hay que quitarlos
 * Asi que de manera general, se quitan todos los timers
 * hasta que cuadran por nombre de etiqueta
 */
void endProcess(int tipo, char *sdpData) {

    TIMER *timer;
//    TIMER  internal;
    int    flag = 0xFF;

    tipo &= (0xFF ^ END_MARK); /* Quitar la marca de fin de proceso */

    timer = (TIMER *) pop(pilaTimers);
    if (dbgLevel & DBG_POP) {
        fprintf(traza, "POPTRP: %d-%s-%s-%ld-%ld\n", 
                timer->tipo, timer->label, sdp.label, timer->systemTime, timer->cpuTime);  
    }

    /* 
     * En los casos en que se produzca un salto anormal de la pila
     * por ejemplo, un goback
     * Quitar elementos de la pila hasta encontrar su pareja
     */
    while (strcmp(timer->label, sdp.label) && timer->tipo != tipo) {
          calculateTimers(timer);
          processMessage(timer->tipo, timer);
          timer = (TIMER *) pop(pilaTimers);
          if (dbgLevel & DBG_POP) {
              fprintf(traza, "POPTRP: %d-%s-%s-%ld-%ld\n", 
                      timer->tipo, timer->label, sdp.label, timer->systemTime, timer->cpuTime);  
          }
    }

    // Hacer el proceso previo a fin de modulo
    if (tipo & MODULE) processEndModule(sdpData);

    calculateTimers(timer); // Actualiza el timer

    processMessage(tipo & flag, timer);

    if (stackDepth(pilaTimers) == 0) {
        sendPendingData();
        sendEndSession(&current);
        cleanSystem();
    }
}

/* 
 * Calcula los tiempos que hay que registrar 
 * Deja los tiempos como ultimo
 */

void calculateTimers(TIMER *timer) {
    timer->systemTime  = current.systemTime - timer->systemTime;
    timer->cpuTime     = current.cpuTime    - timer->cpuTime;     

     /*
      * Si no hay anterior todo se ha consumido aqui
      * Si no aqui se ha consumido la diferencia entre el actual y ultimo
     */

     if (ultimo.systemTime == 0) {
         timer->intSystemTime = timer->systemTime;
         timer->intCpuTime    = timer->cpuTime;
     }
     else {
        timer->intSystemTime = current.systemTime - ultimo.systemTime;
        timer->intCpuTime    = current.cpuTime    - ultimo.cpuTime;
     }
     timer->veces = 1;
     ultimo = current;
}


void nuevoModulo(void) {
    MOD *modulo = addModule(&sdp, modo);
    modulo->monitor = 0xFF;
    if (dbgLevel > DBG_MOD) fprintf(traza, "PUSHM : %s\n", modulo->nombre);
    push(pilaModulos, modulo);
}

/*
 * Crea el hash SHA de inicio de la sesion
 * Prepara el buffer de envio
 * Inicia las pilas
 * Envia el mensaje de inicio de sesion
 */

void initSystem() {
   FILETIME systemTime;
   char hostname[128];
   char szMsg[512];

   /* Verificar la agrupacion de mensajes */
   char *var = getenv("SDP_GROUP");
   if (var != 0x0)  setMaxGroup(atoi(var));

   /* Verificar el modo de trabajo */
   var = getenv("SDP_MODE");
   if (var != 0x0)  modo = atoi(var);

   setBufferSize();

   /* Crear las pilas */
   pilaTimers  = (void *) createStack(sizeof(TIMER), FALSE);
   pilaModulos = (void *) createStack(sizeof(MOD *), TRUE);

   /* Generar el identificador de la sesion */
   hostname[127] = 0x0;
   gethostname(hostname, 127);
   getlogin(hostname, sizeof(hostname));
   usuario = strdup(hostname);

   getTimestamp(&systemTime);
   idSession = (char *) malloc(HASH_SIZE + 1);
   
   sprintf(szMsg, "%s%lu%lu%d%s", sdp.firma, systemTime.dwHighDateTime, systemTime.dwLowDateTime, getpid(), hostname);
   
   FIRMA(idSession, szMsg);

   if (dbgLevel & DBG_SESION) fprintf(traza, "SESION: %s-%s\n", sdp.nombre, idSession);

   /* Conectar el gestor de colas */
   initAMQ(dbgLevel, traza);

   inErr = MQConnect();
  if (inErr == 0) inErr = MQOpenOutput();

}

/*
 * Libera la memoria y cierra las conexiones
 */
void cleanSystem() {
    MQDisconnect();

    pilaTimers = deleteStack(pilaTimers);
    cleanStacks();
    cleanModules();
    
    pilaTimers  = NULL;
    pilaModulos = NULL;

    unsetBufferSize();

    free(idSession);

    if (traza != NULL) {
        fclose(traza);
        traza = 0x0;
    }

}
