/**
 * @file
 * @brief Define las variables de uso global para todo el sistema
 *
 * Se definen para evitar el uso de estructuras y el paso de 
 * parametros excesivos
 *
 * @author  : Javier Gonzalez
 * @date    : 01/03/15
 * @version : 2.0
 *
 */

#ifndef _GLOBAL_H_
   #define _GLOBAL_H_
   
   #include <stdio.h>
   #include "sdpconfig.h"
   #include "sdpTypes.h"


   #ifdef _MAIN_MODULE_
      SDP    sdp;                  /**< Estructura COBOL en formato C  */

      int modo = PROCESS_MODE;     /**< Control de modo de trabajo     */
      
      /* Control de debug */
      #ifdef DEBUG_LEVEL
         int dbgLevel = DEBUG_LEVEL;
      #else
         int dbgLevel = DBG_NONE;
      #endif

      int   inErr    = 0;        /**< Flag de error en MQ        */
      void *pilaTimers = 0x0;    /**< Pila de timers             */
      void *pilaModulos = 0x0;   /**< Pila de modulos            */
      char *idSession;           /**< Identificador de la sesion */
      char  *usuario;            /**< Usuario                    */
   #else    
       extern SDP   sdp;
       extern FILE *traza;
       extern int   modo;
       extern int   dbgLevel;
       extern int   inErr;
       extern void *pilaTimers;
       extern void *pilaModulos;
       extern char *idSession;
       extern char *usuario;

   #endif
#endif