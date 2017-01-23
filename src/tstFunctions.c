
#include <stdio.h>
#include <stdlib.h>

#include "tstFunctions.h"
#include "amqsdp.h"
#include "trapcodes.h"
#include "SDPTRAPB.h"
#include "MD5.h"

long mqrc = 0;

char *copys[1000];
int numCopys = -1;

_EXT_ int check(int *expected) {
    int res[10];
    int idx = 0;
    char *tok;
    char buffer[32767];

    memset(res, 0x0, sizeof(res));
    mqrc = MQConnect();
    if (mqrc != 0) return 32;
    
    mqrc = MQOpenInput();
    if (mqrc != 0) return 32;

    mqrc = MQGet(buffer, sizeof(buffer) - 1);
    while (mqrc == 0) {
        tok = strtok(buffer, ";");
        switch (atoi(tok)) {
           case MSG_BEG:   res[0]++; break;
           case MSG_END:   res[1]++; break;
           case PARAGRAPH: res[2]++; break;
	       case CALL:      res[3]++; break;
           case MODULE:    res[4]++; break;
           case MSG_COVER: res[5]++; break;
           case MSG_PERS:  res[6]++; break;
           default: return 16;
        }
        mqrc = MQGet(buffer, sizeof(buffer) - 1);
    }
    MQDisconnect();
 
    for (idx = 0; idx < 7; idx++) {
        if (res[idx] != expected[idx]) return -1;
    }
    return 0;
}

_EXT_ void call(int tipo, int cpy, char *label) {
    int opcion = tipo;
    int largo = strlen(label);

    SDPTRAPB(&opcion, &largo , copys[cpy], label);
}

_EXT_ void creaCopy(int pos, char *nombre, int numBlocks, int numParrs, int numFiles) {
    char *data;
    long blk[4];
    char md5[33];
    char name[MAX_MODULE + 1];
    int i = 0;
    int j = 0;
    char acc[10];
    int end = 0;
    int offset;
    int idx;
    long val;
    char aux[12];

    copys[pos] = (char *) malloc(1024);
    data = copys[pos];

    /* Nombre de la rutina */
    strcpy(data, "SDPTRAPB");

    /* Nombre del modulo */
    strcpy(name, nombre);
    while(strlen(name) < MAX_MODULE) strcat(name, " ");
    strcat(data, name);

    /* Firma digital */
    MD5(md5, nombre);
    strcat(data, md5);


    /* Tipos de profiling */
//    strcat(data, "11111111");

    offset = strlen(data);

    blk[0] = numBlocks;
    blk[1] = numParrs;
    blk[2] = numFiles;
    blk[3] = 0x0;

    memcpy(&(data[72]), blk, 3 * sizeof(long));

    /* Cobertura */
    offset = 72 + (3 * sizeof(long));
    for (idx = 0; idx < numBlocks; idx++) {
        sprintf(aux, "%d", (idx % 2));
        memcpy(&(data[offset]), aux, 1);
        offset++;
    }

    /* Parrafos */
    for (idx = 0; idx <= numParrs; idx++) {
            val = idx + 1;
            memcpy(&(data[offset]), &val, sizeof(long));
            offset += sizeof(long);
    }

    /* Ficheros */
    for (idx = 0; idx <= numFiles; idx++) {
        for (j = 0; j < 8; j++) {
            val = idx + j;
            memcpy(&(data[offset]), &val, sizeof(long));
            offset += sizeof(long);
        }
    }

    data[strlen(nombre) + 8] = 0x0;
}

