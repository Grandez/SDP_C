#ifndef _TSTFUNCTIONS_H_
   #define _TSTFUNCTIONS_H_

   #undef _EXT_
   #ifdef _TSTFUNCTIONS_C_
      #define _EXT_
   #else
      #define _EXT_ extern
   #endif

   _EXT_ int check(int* counters);
   _EXT_ void call(int tipo, int copy, char *label);
   _EXT_ void creaCopy(int pos, char *nombre, int numBlocks, int numParrs, int numFiles);

#endif