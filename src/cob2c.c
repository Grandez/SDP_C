/**
 * @file
 * @brief Utilidad para pasar los datos de la Copy COBOL a la estructura C
 *
 * copyCobolToCStruct se utiliza en el flujo normal
 * CStructToCopyCobol Solo tiene sentido para pruebas
 *
 * @author  : Javier Gonzalez
 * @date    : 01/03/15
 * @version : 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _COB2C_SRC_

#include "jggsal.h"
#include "sdpTypes.h"
#include "cob2c.h"

/************************************************/
/***           PROTOTYPES                     ***/
/************************************************/

void removeSpaces(char *data, int left);

/************************************************/
/***       PUBLIC FUNCTIONS                   ***/
/************************************************/

_EXT_ FILE *traza;

/**
 * Copia los datos de la COPY a una estructura C para agilizar 
 * el proceso
 *
 * @param sdp Puntero a la estructura SDP
 * @param data Puntero a la COPY COBOL
 * @param modulo Parametro recibido con una etiqueta
 * @param len Longitud de la etiqueta
 */
_EXT_ void copyCobolToCStruct(SDP *sdp, char *data, char *modulo, int len) {
    int idx = 0;
    memset(sdp, 0x0, sizeof(SDP));

    memcpy(sdp->nombre , &(data[COB_MODULE]), COB_MODULE_LEN);
    while (sdp->nombre[idx] != ' ' && idx < COB_MODULE_LEN) idx++;
    if (idx < COB_MODULE_LEN) sdp->nombre[idx] = 0x0;

    memcpy(sdp->firma  , &(data[COB_FIRMA]),  COB_FIRMA_LEN);

	if (len == 0) {
		strcpy(sdp->label, sdp->nombre);
	}
	else {
		strncpy(sdp->label, modulo, len);
	}

    memcpy(&(sdp->bloques),      &(data[COB_BLK]),  BINARY_LONG);
    memcpy(&(sdp->parrafos),     &(data[COB_BLK  +  BINARY_LONG]), BINARY_LONG);
    memcpy(&(sdp->persistencia), &(data[COB_BLK  + (BINARY_LONG * 2)]), BINARY_LONG);

}	



_EXT_ void initSDP(SDP *sdp) {
    char coverage[] = "111111";
    memset(sdp, ' ', sizeof(SDP));
    sdp->coverage = strdup("111111");
}

_EXT_ void setModuleName(SDP *sdp, char *name) {
    strcpy(sdp->nombre, name);
}

_EXT_ void setCoverage(SDP *sdp, char *name) {
    sdp->coverage = strdup(name);
}

/************************************************/
/***        PRIVATE FUNCTIONS                 ***/
/************************************************/

void removeSpaces(char *data, int left) {
    int idx = left - 1;
    while (idx >= 0 && data[idx] == ' ') idx--;
    data[++idx] = 0x0;
}