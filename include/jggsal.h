/**
 * @file
 * @brief Software Abstraction Layer
 *
 * Componente de compatibilidad entre sistemas operativos
 * Redefine las funciones del runtime en funcion del Sistema Operativo
 * Cuando es necesario, jggsal.c actua como wrapper de la
 * funcion objetivo. 
 *
 * De esta manera se eliminan los bloques #ifdef _OS_ del codigo fuente
 *
 * OJO: NO PUEDE LLAMARSE SAL PORQUE LO UTILIZA WINDOWS
 *
 * @author  : Javier Gonzalez
 * @date    : 01/03/15
 * @version : 1.0
 */

#ifndef _JGGSAL_H_
   #define _JGGSAL_H_

   #if defined (_WIN32) || defined (__CYGWIN__)
       #include <Windows.h>
       #include <process.h>

       #define DLL_LOCAL  extern
       #define DLL_EXPORT __declspec(dllexport) 
       #define DLL_IMPORT __declspec(dllimport)

       #define uint32 UINT32
       #define strdup(a) _strdup(a)
       #define getpid()  _getpid()
       
       #define gethostname(a, b) win32_gethostname(a, b)
       #define getTimers(a)      win32_getTimers(a)
       #define getTimestamp(a)   win32_getTimestamp(a)
       #define getlogin(a, b)    win32_getlogin(a,b) 

       #include "salw32.h" 
   #endif

   #if defined (__unix__)
       #include <stdint.h>
       
       typedef int32_t uint32;
       
       typedef unsigned long DWORD;
       typedef struct _FILETIME {
              DWORD dwLowDateTime;
              DWORD dwHighDateTime;
      } FILETIME;
      
      #define DLL_LOCAL  extern __attribute__ ((visibility ("hidden")))
      #define DLL_EXPORT        __attribute__ ((visibility ("default")))
      #define DLL_IMPORT extern

      #define getTimers(a)    linux_getTimers(a)
      #define getTimestamp(a) linux_getTimestamp(a)
   #endif

#endif