/**
 * @file
 * @brief Funciones publicas del archivo timerWin
 *        Se incluye en la plataforma windows
 *
 * @author  : Javier Gonzalez
 * @date    : 01/03/15
 * @version : 1.0
 */

#ifdef _WIN32
#ifndef _TIMERWIN_H_
   #define _TIMERWIN_H_

   #undef _EXT_
   #ifdef _TIMERWIN_SRC_
      #define _EXT_
   #else
      #define _EXT_ DLL_LOCAL
   #endif

   typedef unsigned long long mlong;
   typedef ULARGE_INTEGER ULI;



   /**
    * Obtiene los tiempos consumidos hasta el momento 
    */
   _EXT_ void    win32_getTimers(TIMER *timer);

   /**
    * Devuelve el timestamp del sistema en microsegundos 
    */
   _EXT_ void  win32_getTimestamp(FILETIME *tms);
   _EXT_ TIMER *newTimer(char *label);
#endif
#endif