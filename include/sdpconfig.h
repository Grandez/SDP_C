/**
 * @file
 * @brief Fichero de cabecera de configuracion del sistema
 *        Define las constantes y los valores globales de comportamiento
 *
 * Estas variables se pueden modificar:
 *    1.- Mediante definiciones en el proceso de compilacion (-DVAR=xxx)
 *    2.- A traves de una variable de entorno
 *
 * @author  : Javier Gonzalez
 * @date    : 01/03/15
 * @version : 2.0
 *
 */

#ifndef _SDPCONFIG_H_
   #define _SDPCONFIG_H_

   #define BINARY_LONG   4  /**< Longitud de un BINARY LONG COBOL */ 
   #define ARQ_32        (sizeof(int) == 4)  /**< Longitud de un entero en 32 bits */
   #define ARQ_64        (sizeof(int) == 8)  /**< Longitud de un entero en 64 bits */

   #define MODE_NONE     0  /**< No hara trazas sobre el modulo */
   #define MODE_GLOBAL   1  /**< Acumula todas las trazas de la sesion y las envia al final */
   #define MODE_MODULE   2  /**< Acumula las trazas por modulo y las envia cada vez que acaba un modulo */
   #define MODE_DETAILED 4  /**< Envia las trazas segun se van generando, sin acumular datos  */
   #define MODE_UNLOAD   9  /**< Uso interno  */

   #define MAX_MODULE    32  /**< Longitud del nombre del modulo            */
   #define MAX_PARRAFO   48  /**< Maxima longitud del nombre de un parrafo  */

   #ifndef ITEMS_BY_MSG
       #define ITEMS_BY_MSG 25 /**< Numero de mensajes que se almacenaran antes de enviarlos a la cola */
   #endif

   #undef TRUE
   #define TRUE 0xFF

   #undef FALSE
   #define FALSE 0x00

   #ifndef NULL
     #define NULL 0x0
   #endif

   #define DBG_NONE    0
   #define DBG_CALL    1
   #define DBG_PUSH    2
   #define DBG_POP     4
   #define DBG_SESION  8
   #define DBG_MSG    16
   #define DBG_SEND   32
   #define DBG_TIMER  64
   #define DBG_MOD   128
   
   #define DBG_ALL   255

   #ifndef DEBUG_LEVEL
      #define DEBUG_LEVEL DBG_NONE
   #endif

   #ifndef PROCESS_MODE
      #define PROCESS_MODE MODE_GLOBAL
   #endif

   #ifdef HASH_SHA
       #define HASH_TYPE SHA /**< Utilizar el algoritmo SHA-256 para la generacion de firmas digitales */
       #define HASH_SIZE 64  /* Longitud de la firma digital   */
       #define FIRMA(a, b) SHA(a, b)
   #endif

   #ifdef HASH_MD5
       #define HASH_TYPE MD5  /**< Utilizar el algoritmo MD5 para la generacion de firmas digitales */
       #define HASH_SIZE 32   /* Longitud de la firma digital   */
       #define FIRMA(a, b) MD5(a, b)
   #endif

   #ifndef HASH_TYPE
       #define HASH_TYPE MD5  /**< Utilizar el algoritmo MD5 para la generacion de firmas digitales */
       #define HASH_SIZE  32  /* Longitud de la firma digital   */
       #define FIRMA(a, b) MD5(a, b)
   #endif

   #ifndef DEF_QUEUE
     #define DEF_QUEUE "SDP.TRAPPER"
   #endif

#endif