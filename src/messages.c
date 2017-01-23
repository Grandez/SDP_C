/**
 * @file
 * @brief Gestiona la generacion de los diferentes mensajes que se deben enviar
 *                                                              
 * Los acumula o los envia en funcion del tipo de proceso
 * y del estado
 *
 * @author : Javier Gonzalez 
 * @date   : 01/04/2015
 * @version: 1.0
 *
 */

#define _MESSAGES_C_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "SDPTRAP.h"
#include "messages.h"
#include "modulos.h"
#include "amqsdp.h"
#include "cob2c.h"
#include "global.h"
#include "md5.h"

/**
 * @typedef CNT
 * @brief Mantiene la informacion de los timers 
 */
typedef struct ST_CNT {
       ULONG systemTime;        /**< Tiempo del sistema                     */
	   ULONG cpuTime;           /**< Tiempo de CPU consumido                */
       ULONG intSystemTime;     /**< Tiempo del sistema                     */
	   ULONG intCpuTime;        /**< Tiempo de CPU consumido                */
} CNT;

char *firma;
char  szMsg[4095];    /**< Buffer para un unico mensaje */

unsigned long sequence = 0L;

MOD *lastMod   = NULL; /**< Ultimo modulo ejecutado */
MOD *modActual = NULL; /**< Modulo actual           */
CNT cnt;

/********************************************************************/
/*                    PROTOTYPES                                    */
/********************************************************************/

void processMessageCall      (TIMER *timer);
void processMessageParagraph (TIMER *timer);
void processMessageModule    (TIMER *timer);

void sendCoverage   (char *data, long size, MOD *mod);
void sendUsoParrafos(char *data, long size, MOD *mod);
void sendFiles      (char *data, long size, MOD *mod);
void sendParrafos   (MOD *mod);
void sendCalls      (MOD *mod);

void sendSessionControl(int type, TIMER *current);

void processParrafos(MOD *mod);

void updateModulo(MOD *mod, TIMER *timer);
void ajustaTimers(TIMER *timer);

/**
 * Prepara el mensaje a enviar
 * Si es un mensaje de fin de llamada, ajusta la pila de modulos
 * @param tipo tipo del mensaje segun codigos MSG_xxx
 * @param timer Puntero al TIMER calculado an entrar en la libreria
 */
_EXT_ void processMessage (int tipo, TIMER *timer) {

    modActual = (MOD *) peek(pilaModulos);

    switch (tipo) {
       case CALL:      processMessageCall      (timer); break;
       case PARAGRAPH: processMessageParagraph (timer); break;
       case MODULE:    processMessageModule    (timer); break;
    }
}

/**
 * Procesa el mensaje de fin de llamada
 * Si el modulo estaba monitorizado, se ha quitado de la pila al hacer END_MODULE
 * 
 * Puede ser que el modulo llamado no este monitorizado
 * En ese caso se envia una firma vacia
 * Hay que verificar que no es una llamada recursiva; es decir
 * Si el modulo en la cima de la pila es el mismo que el actual
 * el anterior puede ser el mismo y seria recursivo
 *
 * @param timer Puntero al TIMER calculado an entrar en la libreria
 */
void processMessageCall(TIMER *timer) {
    char called[65];

    if (modo < MODE_DETAILED) {
        addCall(timer);
        return;
    }
    ajustaTimers(timer);

    if (lastMod == NULL || strcmp(sdp.label,lastMod->nombre) != 0) {
        MD5(called, sdp.label);
    }
    else {
        strcpy(called, lastMod->firma);
    }

    sprintf(szMsg, "%d;%ld;%s;%s;%s;%s;%ld;%lu;%lu;%lu;%lu",
                   CALL, ++sequence, idSession,
                   modActual->firma, // Calling
                   called, // Called
                   timer->label,  timer->veces,
                   cnt.systemTime, cnt.cpuTime, cnt.intSystemTime, cnt.intCpuTime);

    inErr = enviar(szMsg, FALSE);

}

/**
 * Procesa un mensaje de tipo parrafo
 *
 * @param timer Puntero al TIMER calculado an entrar en la libreria
 */

void processMessageParagraph(TIMER *timer) {
     if (modo < MODE_DETAILED) {
         addParrafo(timer);
         return;
     }
     
     ajustaTimers(timer);
     sprintf(szMsg, "%d;%ld;%s;%s;%s;%ld;%lu;%lu;%lu;%lu",
            PARAGRAPH,    ++sequence,  idSession,  modActual->firma,
            timer->label, timer->veces,
            cnt.systemTime, cnt.cpuTime, cnt.intSystemTime, cnt.intCpuTime);

    inErr = enviar(szMsg, FALSE);
}

/**
 * Procesa un mensaje de inicio de modulo
 *
 * @param timer Puntero al TIMER calculado an entrar en la libreria
 */

void processMessageModule(TIMER *timer) {
    if (modo < MODE_UNLOAD) {
        lastMod = (MOD *) pop(pilaModulos);
        modActual = (MOD *)peek(pilaModulos);
        if (modActual == NULL) modActual = lastMod;
        if (dbgLevel & DBG_MOD) fprintf(traza, "POPMOD: %s\n", lastMod->nombre);
    }

    if (modo < MODE_MODULE) {
        updateModulo(lastMod, timer);
        setCurrentModule((MOD *)peek(pilaModulos));
        modActual = (MOD *)peek(pilaModulos);
        return;
    }
    ajustaTimers(timer);
    sprintf(szMsg, "%d;%ld;%s;%s;%s;%ld;%lu;%lu;%lu;%lu",
            MODULE, ++sequence, idSession, modActual->firma,
            timer->label, timer->veces, 
            cnt.systemTime, cnt.cpuTime, cnt.intSystemTime, cnt.intCpuTime);

    inErr = enviar(szMsg, FALSE);

    if (modo == MODE_UNLOAD) return;

    if (modo == MODE_MODULE) resetModule();        
    
    setCurrentModule(modActual);
}

/**
 * Envia los mensajes asociados al final de un modulo en funcion
 * del modo de proceso
 *
 *   Contadores de uso de parrafos
 *   Contadores de las llamadas realizadas en ese modulo
 *   Contadores de los ficheros
 *   Informacion de la cobertura
 *
 * @param sdpData Direccion de la COPY COBOL
 */
void processEndModule(char *sdpData) {
    unsigned long bloques[4];
    uint32        tbAux[4];
    int begin;

    if (modo == MODE_MODULE) {
       sendParrafos(getCurrentModule());
       sendCalls(getCurrentModule());
    }

    memcpy(&tbAux, &(sdpData[COB_BLK]), 3 * BINARY_LONG);
    for (begin = 0; begin < 4; begin++) {
        bloques[begin] = tbAux[begin];
    }

    sendCoverage(&(sdpData[COB_COVER]), bloques[0], (MOD *) peek(pilaModulos));

    // La cobertura ocupa un byte por bloque y esta indicado en bloques[0]
    begin = COB_COVER + bloques[0];
    sendUsoParrafos(&(sdpData[begin]), bloques[1], (MOD *) peek(pilaModulos));

    begin += ((bloques[1] + 1) * BINARY_LONG);
    sendFiles(&sdpData[begin], bloques[2], (MOD *) peek(pilaModulos));
}

/**
 * Envia el mensaje con la cobertura de ese modulo
 *
 * @param data Puntero a los datos de cobertura
 * @param size Longitud de los datos de cobertura
 * @param mod  Puntero al MOD actual
 */

void sendCoverage(char *data, long size, MOD *mod) {
    int idx = 0;
    char *tmp = (char *) malloc(size + 1);

    strncpy(tmp, data, size);
    tmp[size] = 0x0;

    if (modo == MODE_GLOBAL) {
        addCoverage(tmp);
        return;
    }

    sprintf(szMsg, "%d;%ld;%s;%s;%s",MSG_COVER,++sequence,idSession,mod->firma,tmp);

    inErr = enviar(szMsg, FALSE);
    free(tmp);
    /*
               fprintf(traza, "********************************** COVER ***************\n");
        if (dbgLevel & DBG_ALL) {
           fprintf(traza, "INT son : %d\n", sizeof(int));
           for (idx = 0; idx < 22; idx++) {
              fprintf(traza, "Byte %d: %X - %c\n", idx, (unsigned char) &(data[idx]), (unsigned char) &(data[idx]));
           }
           fprintf(traza, "********************************** COVER ***************\n");
        }
        */
}

/**
 * Envia el mensaje con los contadores de parrafos
 *
 * @param mod  Puntero al MOD actual
 */

void sendParrafos(MOD *mod) {
    int idx = 0;

    TIMER tmp = mod->lstParrs[idx++];
    while (tmp.veces != 0) {
        ajustaTimers(&tmp);
        sprintf(szMsg, "%d;%ld;%s;%s;%s;%ld;%lu;%lu;%lu;%lu",
                PARAGRAPH,  ++sequence, idSession, mod->firma,
                tmp.label,  tmp.veces, 
                cnt.systemTime, cnt.cpuTime, cnt.intSystemTime, cnt.intCpuTime);
        
        inErr = enviar(szMsg, FALSE);
        tmp = mod->lstParrs[idx++];
    }
}

/**
 * Envia losmensajes asociados a las sentencias CALL
 *
 * @param mod  Puntero al MOD actual
 */

void sendCalls(MOD *mod) {
    MOD *tmp = mod->lstMods;

    while(tmp != NULL) {
       ajustaTimers(&(tmp->timers));
       sprintf(szMsg, "%d;%ld;%s;%s;%s;%s;%ld;%lu;%lu;%lu;%lu",
                       CALL,
                       ++sequence,
                       idSession,
                       mod->firma, // Calling
                       tmp->firma,
                       tmp->nombre,
                       tmp->timers.veces,
	                   cnt.systemTime, cnt.cpuTime, cnt.intSystemTime, cnt.intCpuTime);
 
      inErr = enviar(szMsg, FALSE);
      tmp = tmp->next;
    }
}

/**
 * Enviar el mensaje con el contador de uso de cada parrafo
 *
 * @param sdpData Puntero a los datos de uso de parrafos 
 * @param size Longitud de los datos
 * @param mod  Puntero al MOD actual
 */
void sendUsoParrafos(char *sdpData, long size, MOD *mod) {
    unsigned long *tbParrs;
    uint32        *tbAux;

    char *beg = &(sdpData[-20]);
    char tmp[12];
    int idx = 0;
    int  largo = size * sizeof(long);

    if (size == 0) return;

    tbParrs = (unsigned long *) malloc(largo);
    tbAux   = (uint32 *)        malloc(size * BINARY_LONG);
    memcpy(tbAux, sdpData, size * BINARY_LONG);

    if (ARQ_32) {
        tbAux   = (uint32 *)        malloc(size * BINARY_LONG);
        memcpy(tbAux, sdpData, size * BINARY_LONG);
        for (idx = 0; idx < size; idx++) {
            tbParrs[idx] = tbAux[idx];
        }
        free(tbAux);
    }
    else {
        memcpy(tbParrs, sdpData, largo);
    }

    if (modo == MODE_GLOBAL) {
        addUsoParrafos(tbParrs, size);
        return;
    }

    sprintf(szMsg,"%d;%ld;%s;%s;", MSG_PARRS,++sequence,idSession,mod->firma);

    for (idx = 0; idx < size; idx++) {
         sprintf(tmp, "%ldX", tbParrs[idx]);
         strcat(szMsg, tmp);
    }

    inErr = enviar(szMsg, FALSE);

    // Poner los contadores a cero
    memset(sdpData, 0x0, size * BINARY_LONG); 
}

/**
 * Envia el mensaje con los accesos a los ficheros y tablas
 * Cada fichero tiene 8 tipos de acceso
 *
 * @param sdpData Puntero a los datos de uso de parrafos 
 * @param size Longitud de los datos
 * @param mod  Puntero al MOD actual
 */
void sendFiles(char *sdpData, long size, MOD *mod) {
    int idx;
    int acc;
    char tmp[12];
    unsigned long *tbFiles;
    uint32   *tbAux;

    int  largo = size * 8 * sizeof(long);

    if (size == 0) return;

    tbFiles = (unsigned long *) malloc(largo);

    if (ARQ_32) {
        tbAux   = (uint32 *)        malloc(size * 8 * BINARY_LONG);
        memcpy(tbAux, sdpData, (size * 8 * BINARY_LONG));
        for (idx = 0; idx < (size * 8) ; idx++) {
            tbFiles[idx] = tbAux[idx];
        }
        free(tbAux);
    }
    else {
        memcpy(tbFiles, sdpData, largo);
    }

    if (modo == MODE_GLOBAL) {
        addFiles(tbFiles, size);
        return;
    }

    sprintf(szMsg,"%d;%ld;%s;%s;", MSG_PERS,++sequence,idSession,mod->firma);

    for (idx = 0; idx < size; idx++) {
        for (acc = 0; acc < 8; acc++) {
           sprintf(tmp, "%ldX", tbFiles[(idx * 8) + acc]);
           strcat(szMsg, tmp);
        }
    }

    inErr = enviar(szMsg, FALSE);

    // Poner los contadores a cero
    memset(sdpData, 0x0, size * 8 * BINARY_LONG); 
}

/**
 * Envia el mensaje de inicio de sesion
 * 
 * Este mensaje se envia siempre y de manera aislada
 *
 * @param current Puntero al TIMER obtenido al entrar en la libreria
 */
void sendBeginSession(TIMER *current) {
    sendSessionControl(MSG_BEG, current);
}

/**
 * Envia el mensaje de fin de sesion
 * 
 * @param current Puntero al TIMER obtenido al entrar en la libreria
 */
void sendEndSession(TIMER *current) {
    sendSessionControl(MSG_END, current);
}

void sendSessionControl(int type, TIMER *current) {
    time_t tt;
    ULONG auxTime;
    /* En Windows time_t es uint_64 y eso causa problemas
       forzamos a que sea ULONG copiandolo a auxTime
    */
    time(&tt);
    auxTime = (ULONG) tt - 0;

    ajustaTimers(current);
    
    sprintf(szMsg, "%d;%ld;%s;%s;%s;%lu;%lu;%lu", 
                   type, ++sequence, idSession, sdp.firma, 
                    usuario, auxTime,
                    cnt.systemTime, cnt.cpuTime);

    inErr = enviar(szMsg, TRUE);
}

/**
 * En caso de trabajar en modo global envia todos los datos
 * antes de enviar el mensaje de fin de sesion
 */ 
void sendPendingData() {
    MOD     *mod;
    int      oldMode = modo;

    if (modo != MODE_GLOBAL) return;

    oldMode = modo;
    modo = MODE_UNLOAD;

    mod = getFirstModule();
    while (mod != NULL) {
        if (mod->monitor != 0x0) { // Esta monitorizado
           modActual = mod;
           lastMod = mod;

           sendParrafos(mod);
           sendCalls(mod);
           sendCoverage(mod->cobertura, strlen(mod->cobertura), mod);
           sendUsoParrafos((char *) mod->parrs, mod->numParrs, mod);
           sendFiles      ((char *) mod->files, mod->numFiles, mod);

           processMessageModule(&mod->timers);
        }
        mod = getNextModule();
    }
    modo = oldMode;

}

void processParrafos(MOD *mod) {
    int idx = 0;
    TIMER *tmp = mod->lstParrs;

    while (tmp[idx].veces != 0) {
        processMessageParagraph(&(tmp[idx])); 
        idx++;
    }
}

void updateModulo(MOD *mod, TIMER *timer) {
    mod->timers.veces++;
    mod->timers.systemTime    += timer->systemTime;
    mod->timers.cpuTime       += timer->cpuTime;
    mod->timers.intSystemTime += timer->intSystemTime;
    mod->timers.intCpuTime    += timer->intCpuTime;
}

/**
 * Caso especial para entornos Windows
 * En Windows los tiempos se dan en decenas de nanosegundos
 * por lo que hay que pasarlos a microsegundos redondeando
 */
void ajustaTimers(TIMER *timer) {
    #ifdef _WIN32
       cnt.systemTime    = (timer->systemTime    + 5) / 10;
       cnt.cpuTime       = (timer->cpuTime       + 5) / 10;
       cnt.intSystemTime = (timer->intSystemTime + 5) / 10;
       cnt.intCpuTime    = (timer->intCpuTime    + 5) / 10;
   #else
       cnt.systemTime    = timer->systemTime;
       cnt.cpuTime       = timer->cpuTime;
       cnt.intSystemTime = timer->intSystemTime;
       cnt.intCpuTime    = timer->intCpuTime;
   #endif
}
