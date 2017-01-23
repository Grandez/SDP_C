/**
 * @file
 * @brief Fichero de cabecera para incluir los modulos de calculo de timers
 *
 * Incluira el modulo correspondiente en funcion del Sistema Operativo
 *
 * @author  : Javier Gonzalez
 * @date    : 01/03/15
 * @version : 2.0
 *
 */

#ifndef _TIMER_H_

   #define _TIMER_H_
   
   #ifdef _WIN32
      #include "timerwin.h"
   #endif

   #ifdef __unix__
      #include "timerlnx.h"
   #endif
#endif