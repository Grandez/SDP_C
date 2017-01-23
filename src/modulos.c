/**
 * @file
 * @brief Mantiene la lista de modulos activos con sus ID
 *                                                              
 * @author : Javier Gonzalez 
 * @date   : 01/04/2015
 * @version: 1.0
 *
 */

#define _MODULOS_C_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "SDPTRAP.h"
#include "modulos.h"
#include "timer.h"
#include "md5.h"
#include "sha256.h"

MOD *lstMods = 0x0;  /**< Lista de modulos invocados durante la sesion */
MOD *currMod = 0x0;  /**< Modulo actual                                */
extern FILE *traza;

/************************************************/
/***           PROTOTYPES                     ***/
/************************************************/

MOD   *newModule(SDP *sdp, char *name);
MOD   *findModuleBySDP  (SDP *sdp);
MOD   *findModuleByName (char *name);
MOD   *findModule       (SDP *sdp, char *name);

MOD   *findCall    (char *nombre);
TIMER *findParrafo (char *nombre);

void   addCounter    (TIMER *tgt, TIMER *timer);
void   resetCounters (MOD *mod);
void   resetTimer    (TIMER *timer);

MOD *duplicateModule(MOD *mod);

/*******************************************************************/
/***                PUBLIC FUNCTIONS                             ***/
/*******************************************************************/

/**
 * Inserta un modulo en la lista de modulos
 * @param sdp Puntero a la estructura SDP actual
 * @param mode Indicador de disponibilidad de la estructura SDP
 */
_EXT_ MOD *addModule(SDP *sdp, int mode) {
    MOD *current;
//    MOD *aux;
//    int pos;

    /* Primera vez */
    if (lstMods == NULL) {
        current = newModule(sdp, sdp->nombre);
        lstMods = current;
        currMod = current;
        return current;
    }
    
    current = findModuleBySDP(sdp);
    if (mode == MODE_MODULE) resetCounters(current);
    currMod = current;
    return currMod;
}

/**
 * Inserta una nueva llamada desde ese modulo a otro
 * @param timer Puntero al TIMER obtenido al entrar en la rutina
 */
_EXT_ void addCall(TIMER *timer) {
    MOD *called = NULL;
    called = findCall(timer->label);
    addCounter(&(called->timers), timer);
}

/**
 * Inserta una nueva llamada a un parrafo en ese modulo
 * @param timer Puntero al TIMER obtenido al entrar en la rutina
 */
_EXT_ void addParrafo(TIMER *timer) {
    addCounter(findParrafo(timer->label), timer);
}

/**
 * Inserta la informacion relativa a la cobertura de codigo
 * @param mask Datos de la cobertura
 */
_EXT_ void addCoverage(char *mask) {
    unsigned int idx;
    for (idx = 0; idx < strlen(mask); idx++) {
        currMod->cobertura[idx] |= mask[idx];
    }
}

/**
 * Inserta la informacion relativa al uso global de los parraos del modulo
 * @param tbParrs Tabla de contadores de uso de parrafos
 * @param largo    Longitud de la tabla
 */
_EXT_ void addUsoParrafos(unsigned long *tbParrs, int largo) {
    int iParr;
    unsigned long *p = currMod->parrs;

    for (iParr = 0; iParr < largo; iParr++) {
         p[iParr] += tbParrs[iParr];
    }

}

/**
 * Inserta la informacion relativa al uso de los ficheros del modulo
 * @param tbFiles Tabla de contadores de uso de los ficheros
 * @param largo   Numero de ficheros (Elementos en tbFiles)
 */
_EXT_ void addFiles(unsigned long *tbFiles, int largo) {
    int iFile;
    int iAcc;
    int idx;

    unsigned long *f = currMod->files;

    for (iFile = 0; iFile < largo; iFile++) {
        for (iAcc = 0; iAcc < 8; iAcc++) {
            idx = iFile * 8 + iAcc;
             f[idx] += tbFiles[idx];
        }
    }
}

/**
 * Inicializa el modulo
 * Se utiliza cuando el modo de proceso es MODE_MODULE
 */
_EXT_ void resetModule() {
    unsigned int idx;

    for (idx = 0; idx < strlen(currMod->cobertura); idx++) {
        currMod->cobertura[idx] = '0';
    }
    memset(currMod->files, 0x0, sizeof(long) * 8 * currMod->numFiles);
    resetTimer(&(currMod->timers));
    resetCounters(currMod);
}

/**
 * Establece el modulo pasado como parametro como modulo activo
 * Se sabe que existe
 * @param mod puntero a MOD con el modulo activo
 */
_EXT_ void setCurrentModule(MOD *mod) {
    currMod = mod;
}

/**
 * Devuelve el modulo activo
 * @return Puntero al modulo activo
 */
_EXT_ MOD *getCurrentModule() {
    return currMod;
}

/**
 * Devuelve el primer modulo de la lista de modulos ejecutados
 * @return Puntero al primer modulo
 */
_EXT_ MOD *getFirstModule() {
    currMod = lstMods;
    return currMod;
}

/**
 * Devuelve el siguiente modulo de la lista de modulos ejecutados
 * @return Puntero al siguiente modulo
 */
_EXT_ MOD *getNextModule() {
    currMod = currMod->next;
    return currMod;
}

/**
 * Realiza los procesos de limpieza de memoria
 */
_EXT_ void cleanModules() {
    /*
    MOD *tmp = lstMods;
    MOD *aux;
    while (tmp != NULL) {
        free(tmp->lstParrs);
        free(tmp->cobertura);
        free(tmp->files);
        aux = tmp->next;
        free(tmp);
        tmp = aux;
    }
    */
    lstMods = NULL;
    currMod = NULL;
}

/*******************************************************************/
/***                PRIVATE FUNCTIONS                            ***/
/*******************************************************************/

void addCounter(TIMER *tgt, TIMER *timer) {
    tgt->veces++;
    tgt->systemTime    += timer->systemTime;
    tgt->cpuTime       += timer->cpuTime;
    tgt->intSystemTime += timer->intSystemTime;
    tgt->intCpuTime    += timer->intCpuTime;
}

/*
 * Busca el modulo en la lista de modulos
 * Si no existe lo crea
 */

MOD *findModuleBySDP(SDP *sdp) {
    return findModule(sdp, sdp->nombre);
}

/* 
 * Busca el modulo por nombre. Existe
 */
MOD *findModuleByName(char *name) {
    return findModule(NULL, name);
}
/*
 * Se intoducen los modulos al inicio
 * en orden inverso de llamada, asi
 * el ultimo en procesarse es el modulo raiz
 */
MOD *findModule(SDP *sdp, char *name) {
    MOD *tmp  = lstMods; 

    while (tmp != NULL) {
        if (strcmp(tmp->nombre, name) == 0) return tmp;
        tmp = tmp->next;
    }

    tmp = newModule(sdp, name);
    tmp->next = lstMods;
    lstMods = tmp;
    return tmp;
}

/*
 * Busca la llamada en la lista de modulos llamados
 * Si no lo encuentra lo crea pero debe ser copiado
 * por que un valor es el numero de veces que se ha ejecutado
 * el modulo y otro las veces que este modulo lo ha llamado
 */

MOD *findCall(char *nombre) {
    MOD *last = currMod->lstMods;
    MOD *call = currMod->lstMods;

    /* No hay ninguno */
    if (call == NULL) {
        call = findModuleByName(nombre);
        currMod->lstMods = duplicateModule(call);
        return currMod->lstMods;
    }

    while (call != NULL) {
        if (strcmp(call->nombre, nombre) == 0) return call;
        last = call;
        call = call->next;
    }
    call = findModuleByName(nombre);
    last->next = duplicateModule(call);
    return last->next;
}

MOD *duplicateModule(MOD *mod) {
    MOD *tmp = (MOD *) malloc(sizeof(MOD));
    memset(tmp, 0x0, sizeof(MOD));
    strcpy(tmp->nombre, mod->nombre);
    strcpy(tmp->firma, mod->firma);
    return tmp;
}

TIMER *findParrafo(char *nombre) {
    int idx = 0;
    MOD *modulo = currMod;
    /*
     * Algunos compiladores manejan el puntero a cadena
     * Pero sabemos que si exsite el parrafo, al menos se ha ejecutado una vez
     * con lo que veces >= 1
     */
    while (modulo->lstParrs[idx].veces != 0) {
        if (strcmp(modulo->lstParrs[idx].label, nombre) == 0) {
            return &(modulo->lstParrs[idx]);
        }
        idx++;
    }
    strcpy(modulo->lstParrs[idx].label, nombre);
    return &(modulo->lstParrs[idx]);
}

void resetCounters(MOD *mod) {
    int  idx = 0;
    MOD *aux = mod->lstMods;

    while (aux != NULL) {
        resetTimer(&(aux->timers));
        aux = aux->next;
    }

    while (mod->lstParrs[idx].veces != 0) {
        resetTimer(&(mod->lstParrs[idx]));
        idx++;
    }
}

void resetTimer(TIMER *timer) {
     timer->cpuTime       ^= timer->cpuTime;
     timer->systemTime    ^= timer->systemTime;
     timer->intSystemTime ^= timer->intSystemTime;
     timer->intCpuTime    ^= timer->intCpuTime;
     timer->veces         ^= timer->veces;
}

/*
 * Crea una nueva structura MOD
 */

MOD *newModule (SDP *sdp, char *name) {
    int idx;
    int nParrs  = 0;
    int nBlocks = 0;
    int nPers   = 0;
    MOD *curr = (MOD *) malloc(sizeof(MOD));
    memset(curr, 0x0, sizeof(MOD));

    if (sdp == NULL) {
        strcpy(curr->nombre, name);
        strcpy(curr->timers.label, name);
        curr->monitor = 0x0;
        FIRMA(curr->firma, name);
    }
    else {
        strcpy(curr->nombre, sdp->nombre);
        strcpy(curr->firma, sdp->firma);
        strcpy(curr->timers.label, sdp->nombre);
        curr->numFiles = sdp->persistencia;
        curr->numParrs = sdp->parrafos;
        curr->monitor = 0xFF;
        nParrs  = sdp->parrafos;
        nBlocks = sdp->bloques;
        nPers   = sdp->persistencia;
    }

    /* Se valida el fin de la lista por el numero de veces invocado
       asi que se crea un parrafo virtual mas
    */
    curr->lstParrs = (TIMER *) malloc(sizeof(TIMER) * (nParrs + 1));
    for (idx = 0; idx < (nParrs + 1); idx++) {
       memset(&(curr->lstParrs[idx]), 0x0, sizeof(TIMER));
    }
    curr->cobertura = (char *) malloc(nBlocks + 1);
    memset(curr->cobertura, '0', nBlocks);
    curr->cobertura[nBlocks] = 0x0;

    if (nParrs > 0) {
        curr->parrs = (unsigned long *) malloc(nParrs * sizeof(unsigned long));
        memset(curr->parrs, 0x0, nParrs * sizeof(unsigned long));
    }

    if (nPers > 0) {
        curr->files = (unsigned long *) malloc(nPers * 8 * sizeof(unsigned long));
        memset(curr->files, 0x0, nPers * 8 * sizeof(unsigned long));
    }

    return curr;
}

