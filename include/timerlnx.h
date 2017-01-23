/**
 * @file
 * @brief Funciones publicas del archivo timerLnx
 *        Se incluye en la plataforma Linux
 *
 * @author  : Javier Gonzalez
 * @date    : 01/03/15
 * @version : 1.0
 */

#ifdef __unix__
#ifndef _TIMERLNX_H_
   #define _TIMERLNX_H_

   #undef _EXT_
   #ifdef _TIMERLNX_SRC_
      #define _EXT_
   #else
      #define _EXT_ DLL_LOCAL
   #endif

   typedef unsigned long long mlong;

   /**
    * Obtiene los tiempos consumidos hasta el momento 
    */
   _EXT_ void    linux_getTimers(TIMER *timer);

   /**
    * Obtiene el timestamp del sistema
    */
   _EXT_ void    linux_getTimestamp(FILETIME *ft);
#endif
#endif