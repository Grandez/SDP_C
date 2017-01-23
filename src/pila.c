/**
 * @file
 * @brief Gestor de pilas abstractas por copia o referencia
 *
 * Si se indica que es un puntero, la pila mantiene el puntero al dato
 * y el llamador es responsable de mantener la integridad de los datos
 * Es decir, no liberar esa memoria mientras este en la pila
 *
 * Por defecto la pila mantiene su propia copia de los datos evitando
 * el problema de punteros nulos, y se encarga de gestionar la memoria
 * de manera adecuada
 *
 * Mantiene pilas de objetos
 * Funciona como un objeto:
 *   Primero se crea la pila con createStack, 
 *   la cual devuelve una instancia de pila
 *   El resto de llamadas a la pila se realiza 
 *   pasandole como primer parametro esa instancia
 *
 * @author:  Javier Gonzalez
 * @date:    01/03/15
 * @version: 1.0
 *
 */

#include <stdlib.h>
#include <memory.h>

#include "pila.h"

struct ST_PILA {
    int size;
    unsigned char pointer;
	int numItems;
	int maxItems;
	void **items;
};

typedef struct ST_PILA PILA;


PILA **pilas = 0x0;
int numPilas = 0;
int maxPilas = 0;

/************************************************/
/***           PROTOTYPES                     ***/
/************************************************/

int alocarMemoria() ;
PILA *nuevaPila(int size, unsigned char pointer);
void increaseStack(PILA *stack);

/************************************************/
/***  PUBLIC FUNCTIONS                        ***/
/************************************************/

/**
 * Crea una nueva instancia de pila con elementos de size bytes
 * @param size    Tamaño del objeto a almacenar en la pila
 * @param pointer Flag para indicar si la pila es de copia o referencia
 *                TRUE es una pila de referencia, almacena el puntero al objeto
 *                FALSE es una pila de copia, almacena una copia del objeto
 * @return Manejador de la pila
 */
_EXT_ void * createStack(int size, unsigned char pointer) {
	int pos = alocarMemoria();
	pilas[pos] = nuevaPila(size, pointer);
	return pilas[pos];
}

/**
 * Elimina la instancia de pila pasada como parametro 
 * @param stack  Handle de la pila a eliminar
 * @return Manejador de la pila
 */
_EXT_ void *deleteStack(void *stack) {
    free(stack);
    return 0x0;
}

/**
 * Elimina todas las pilas activas en el sistema
 */

_EXT_ void cleanStacks() {
    int idx = 0;
    free(pilas);
    pilas = 0x0;
    numPilas = 0;
    maxPilas = 0;
}

/**
 * Inserta un elemento en la pila
 * @param stack  Handle de la pila
 * @param data   Objeto a insertar en la pila
 */
_EXT_ void push(void *stack, void *data) {
    PILA *wrk = (PILA *) stack;

	if (wrk->maxItems == wrk->numItems) increaseStack(wrk);
    if (wrk->pointer == 0x0) {
	    memcpy(wrk->items[wrk->numItems++], data, wrk->size);
    }
    else {
        wrk->items[wrk->numItems++] = data;
    }
}

/**
 * Saca un elemento en la pila
 * @param  stack  Handle de la pila
 * @return Objeto en la cima de la pila
 */
_EXT_ void * pop(void *stack) {
      PILA *wrk = (PILA *) stack;
      return (wrk->numItems > 0) ? wrk->items[--wrk->numItems] : NULL;
}

/**
 * Obtiene el elemento de la cima de la pila sin eliminarlo
 * @param  stack  Handle de la pila
 * @return Objeto en la cima de la pila
 */
_EXT_ void *peek(void *stack) {
      return peekPrevious(stack, 0);
}

/**
 * Obtiene un elemento de la de la pila sin eliminarlo
 * @param  stack  Handle de la pila
 * @param  deep   Posicion del elemento en la pila (0 = cima de la pila)
 * @return Objeto en la cima de la pila
 */
_EXT_ void *peekPrevious(void *stack, int deep) {
      PILA *wrk = (PILA *) stack;
      return (wrk->numItems > deep) ? wrk->items[wrk->numItems - deep - 1] : NULL;
}

/**
 * Devuelve un elemento de la pila sin quitarlo
 * from = 0 -> Base
 *        1 -> Cima
 * offset = desplazamiento relativo
 */
_EXT_ void * seek(void *stack, int offset, int from) {
    PILA *wrk = (PILA *) stack;
	int idx = (from == 1) ? wrk->numItems  - 1: 0;
	idx += offset;
	if (idx < 0) return 0x0;
	if (idx >= wrk->numItems) return 0x0;
	return wrk->items[idx];
}

/**
 * Devuelve la profundidad actual de la pila
 * @param  stack  Handle de la pila
 * @return profundidad de la pila
 */
_EXT_ int stackDepth(void *stack) {
    PILA *wrk = (PILA *) stack;
	return wrk->numItems;
}

/************************************************/
/***  PRIVATE FUNCTIONS                       ***/
/************************************************/

int alocarMemoria() {
	// Crear la pila maestra inicial
	if (pilas == 0x0) {
		pilas = (PILA **) malloc(sizeof(PILA *) * 5);
		maxPilas = 5;
		return 0;
	}
	// No hay espacio para una nueva pila
	if (numPilas == maxPilas) {
		maxPilas += 5;
	    pilas = (PILA **) realloc(pilas, sizeof(PILA *) * maxPilas);
	}
	return numPilas;
}

PILA *nuevaPila(int size, unsigned char pointer) {
	PILA *pila = (PILA *) malloc(sizeof(PILA));
    pila->size     = size;
    pila->pointer  = pointer;
	pila->numItems = 0;
	pila->maxItems = 0;
	pila->items    = 0x0;
	return pila;
}

void increaseStack(PILA *stack) {
    int idx = stack->numItems;
    stack->maxItems += 10;
	stack->items = (void **) realloc(stack->items, stack->maxItems * sizeof(void **));
    for (idx = stack->numItems; idx < stack->maxItems; idx++) {
        stack->items[idx] = malloc(stack->size);
        memset(stack->items[idx], 0x0, stack->size);
    }
}