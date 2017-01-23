/**
 * @file
 * @brief Define las estructrus y tipos de datos usados en la libreria
 *
 * @author  : Javier Gonzalez
 * @date    : 01/03/15
 * @version : 2.0
 *
 */

#ifndef _SDPTYPES_H_
   #define _SDPTYPES_H_

   #include "sdpconfig.h"

   typedef unsigned long ULONG;

   /**
    * @typedef SDP
    * @brief Estructura C equivalente a la COPY COBOL 
    * 
    * La copy COBOL es una secuencia de bytes raw
    */
    
   typedef struct ST_SDP {
       char   nombre[MAX_MODULE + 1];  /**< Nombre del modulo asciiz */
       char   firma[HASH_SIZE + 1];    /**< Firma del modulo         */
       char  *coverage;                /**< tabla de bloques asciiz  */
	   char   label[MAX_PARRAFO + 1];  /**< Rutina o parrafo         */
       unsigned long bloques;          /**< Numero de bloques        */ 
       unsigned long parrafos;         /**< Numero de parrafos       */
       unsigned long persistencia;     /**< Numero de ficheros       */
   } SDP;

   /**
    * @typedef TIMER
    * @brief Contadores de tiempo en microsegundos 
    *
    * Se mantiene un acumulador por cada parrafo o modulo
    * Los tiempos van en mirosegundos
    */

   struct ST_TIMER {
       int   tipo;                   /**< Tipo del mensaje que genera el timer  */  
       char  label[MAX_PARRAFO + 1]; /**< Nombre                                */
       ULONG veces;                  /** Numero de veces que se ha utilizado    */
       ULONG systemTime;             /**< Tiempo del sistema                    */
	   ULONG cpuTime;                /**< Tiempo de CPU consumido               */
       ULONG intSystemTime;          /**< Tiempo del sistema                    */
	   ULONG intCpuTime;             /**< Tiempo de CPU consumido               */
       struct ST_TIMER *next;
   };

   typedef struct ST_TIMER TIMER;

   /*
    * @typedef MOD
    * @brief Structura para mantener la informacion de los modulos llamados
    */

   struct ST_MOD {
       char   nombre[MAX_MODULE + 1];  /**< Nombre del modulo                       */
       char   firma[HASH_SIZE + 1];    /**< Firma del modulo                        */
       char  *cobertura;               /**< Cadena de cobertura                     */
       char   monitor;                 /**< Es un modulo monitorizado o no          */
       unsigned int   numParrs;        /**< Numero de parrafos                      */
       unsigned long *parrs;           /**< Lista de acceso a parrafos              */
       unsigned int   numFiles;        /**< Numero de ficheros                      */
       unsigned long *files;           /**< Lista de acceso a ficheros              */
       TIMER  timers;                  /**< Timers del modulo                       */
       TIMER *lstParrs;                /**< Tabla de parrafos del modulo            */
       struct ST_MOD *lstMods;         /**< Lista de modulos llamados por el modulo */
       struct ST_MOD *next;
   };

   typedef struct ST_MOD MOD;

#endif