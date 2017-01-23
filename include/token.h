/*
 * Estructuras de datos para almacenar la informacion de consumo
 * 
 * TIMER almacena los contadores en microsegundos
 * TOKEN almacena el TIMER asociado a un componente
 * 
 * Author:  Javier Gonzalez
 * Date:    01/03/15
 * Version: 1.0
 */

#ifndef _TOKEN_
   #define _TOKEN_
   
   typedef unsigned long long mlong;

   /* Contadores de tiempo en microsegundos */
   struct ST_TIMER {
	   mlong stime;  /* Tiempo actual del sistema              */
	   mlong utime;  /* Tiempo de CPU consumido en modo User   */
	   mlong ktime;  /* Tiempo de CPU consumido en modo Kernel */
   };

   typedef struct ST_TIMER TIMER;

   struct ST_TOKEN {
	   char *name;
	   char *id;
	   TIMER tms;
   };

   typedef struct ST_TOKEN TOKEN;
#endif
