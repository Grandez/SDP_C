/**
 * @file
 * @brief Funciones de gestion de pilas abtracta
 *
 * @author  : Javier Gonzalez
 * @date    : 01/03/15
 * @version : 2.0
 *
 */

#ifndef _PILA_H_
   #define _PILA_H_

   #include "jggsal.h"

   #undef _EXT_
   #ifdef _PILA_C_
      #define _EXT_
   #else
     #define _EXT_ DLL_LOCAL
   #endif

 /**
  * Crea una nueva instancia de pila con elementos de size bytes
  * @param size    Tamaño del objeto a almacenar en la pila
  * @param pointer Flag para indicar si la pila es de copia o referencia
  *                TRUE es una pila de referencia, almacena el puntero al objeto
  *                FALSE es una pila de copia, almacena una copia del objeto
  * @return Manejador de la pila
  */
  _EXT_ void *createStack(int size, unsigned char pointer);

  /**
   * Elimina la instancia de pila pasada como parametro 
   * @param stack  Handle de la pila a eliminar
   * @return Manejador de la pila
   */
  _EXT_ void *deleteStack(void *stack);

  /**
   * Inserta un elemento en la pila
   * @param stack  Handle de la pila
   * @param data   Objeto a insertar en la pila
   */
  _EXT_ void  push(void *stack, void *data);

  /**
   * Saca un elemento en la pila
   * @param  stack  Handle de la pila
   * @return Objeto en la cima de la pila
   */
  _EXT_ void *pop(void *stack);

  /**
   * Obtiene el elemento de la cima de la pila sin eliminarlo
   * @param  stack  Handle de la pila
   * @return Objeto en la cima de la pila
   */
  _EXT_ void *peek(void *stack);

  /**
   * Obtiene un elemento de la de la pila sin eliminarlo
   * @param  stack  Handle de la pila
   * @param  deep   Posicion del elemento en la pila (0 = cima de la pila)
   * @return Objeto en la cima de la pila
   */
  _EXT_ void *peekPrevious(void *stack, int deep);

  /**
   * Devuelve la profundidad actual de la pila
   * @param  stack  Handle de la pila
   * @return profundidad de la pila
   */
  _EXT_ int   stackDepth(void *stack);

  /**
   * Elimina todas las pilas activas en el sistema
   */
  _EXT_ void  cleanStacks(void);

#endif