/**
 * @file
 * @brief System Abtraction Layer
 *        Version de funciones para Windows 32
 * 
 * Implementa funciones Unix que son diferentes en las plataformas Windows
 *
 * @author:  Javier Gonzalez
 * @date:    01/03/15
 * @version: 1.0
 *
 */

#ifdef _WIN32
#include <windows.h>

#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

#define  _SALW32_SRC_
#include "salw32.h"

/**
 * @brief Devuelve el nombre del host
 *
 * En windows es necesario inicializar el entorno de red antes de
 * utilizar esta funcion
 *
 * @param name    Buffer donde se almacenara el nombre del Host
 * @param namelen Longitud del buffer
 * @return 0 Si se ha obtenido el nombre del Host
 *         Errorcode en caso contrario   
 */
_EXT_ int win32_gethostname(char *name, int namelen) {
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) return -1;
    return gethostname(name, namelen);
}

/**
 * @brief Devuelve el identificador del usuario
 *
 * @param name    Buffer donde se almacenara el nombre del usuario
 * @param namelen Longitud del buffer
 */

_EXT_ void win32_getlogin(char *buff, int len) {
      DWORD sz = len;

     if (!GetUserName ((LPTSTR) buff, &sz)) strcpy(buff, "N/A");
}

#endif