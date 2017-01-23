/**
 * @file
 * @brief Fichero de cabecera de la libreria SDPTRAPB
 *        Define las constantes y los valores globales de comportamiento
 *
 * Define:
 *    El unico punto de entrada a la DLL
 *    La estructura C equivalente a la COPY COBOL
 *    Las constantes para la gestion de los diferentes tipos de mensajes
 *
 * En determinados entornos COBOL no se permite que las rutinas C
 * tengan varios puntos de entrada
 * La carga de la libreria se realiza por el mismo nombre de la rutina
 * y el punto de entrada en una funcion con el mismo nombre
 *
 * @author  : Javier Gonzalez
 * @date    : 01/03/15
 * @version : 2.0
 *
 */

#ifndef _SDPTRAPB_H_
   #define _SDPTRAPB_H_


   #ifdef _SDPTRAPB_C_
      #define SDPTRAP_DLL DLL_EXPORT 
    #else
       #define SDPTRAP_DLL DLL_IMPORT 
    #endif
   
   #undef _EXT_
   #ifdef _SDPTRAPB_C_
      #define _EXT_
   #else
      #define _EXT_ DLL_LOCAL
   #endif

   /**
    * @brief Punto de entrada a la libreria SDPTRAPB
    *
    * @param option  = una combinacion de los valores definidos en MSG_xxx
    * @param len     = Longitud de la etiqueta, 0 si no es relevante
    * @param sdpData = Puntero a la estructura SDP
    * @param label   = Etiqueta (parrafo, rutina, etc.)
    */

   SDPTRAP_DLL void SDPTRAPB     (int *option, int *len, char *sdpData, char *label);

   /**
    * Usado para pruebas unitarias y de integracion
    */
   SDPTRAP_DLL void SDPTRAPB_setDebugLevel(int level);

   /**
    * Usado para pruebas unitarias y de integracion
    */
   SDPTRAP_DLL void SDPTRAPB_setModo      (int mode);

#endif