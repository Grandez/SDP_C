#ifndef _PILAPTR_H_
   #define _PILAPTR_H_

   #ifdef _EXT_
      #undef _EXT_
   #endif
   #ifdef _PILAPTR_C_
      #define _EXT_
   #else
     #define _EXT_ extern
   #endif

  _EXT_ void *createStackPtr(void);
  _EXT_ void *deleteStackPtr(void *stack);
  _EXT_ void  pushPtr(void *stack, void *data);
  _EXT_ void *popPtr(void *stack);
  _EXT_ void *peekPtr(void *stack);
  _EXT_ void *peekPtrPrevious(void *stack, int deep);
  _EXT_ int   stackDepthPtr(void *stack);


#endif