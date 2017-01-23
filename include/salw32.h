/**
 * @file
 * @brief Definicion de funciones especificas para sistemas Windows
 *
 * @author  : Javier Gonzalez
 * @date    : 01/03/15
 * @version : 2.0
 *
 */

#if defined (_WIN32) || defined (__CYGWIN__)
   #ifndef _SALW32_H_
      #define _SALW32_H_

      #include "sdpTypes.h"
      #undef _EXT_
      #ifdef _SALW32_SRC_
         #define _EXT_
      #else
         #define _EXT_ DLL_LOCAL
      #endif
   
      /**
       * Obtiene el nombre de la maquina
       * @param name Buffer donde almacenar el nombre de la maquina
       * @param namelen Longitud del buffer
       * @return Longitud del nombre de la maquina
       */
      _EXT_ int  win32_gethostname(char *name, int namelen);

      /**
       * Devuelve el nombre del usuario activo en la maquina
       * @param name Buffer donde almacenar el nombre de usuario
       * @param len Longitud del buffer
       */
      _EXT_ void win32_getlogin   (char *buff, int len);
   #endif
#endif