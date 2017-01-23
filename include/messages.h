/**
 * @file
 * @brief Funciones de gestion del envio de mensajes al sistema de colas
 *
 * @author  : Javier Gonzalez
 * @date    : 01/03/15
 * @version : 2.0
 *
 */

#ifndef _MESSAGES_H_
   #define _MESSAGES_H_
   
   #include "sdpTypes.h"

   #undef _EXT_
   #ifdef _MESSAGES_C_
      #define _EXT_
   #else
      #define _EXT_ DLL_LOCAL
   #endif   

  /**
   * Procesa un mensaje recibido del programa
   * @param tipo  tipo del mensaje
   * @param timer Puntero al TIMER obtenido al entrar en SDPTRAB
   */
  _EXT_ void processMessage   (int tipo, TIMER *timer);

  /**
   * Realiza el proceso asociado al final de la ejecucion de un modulo
   * @param data  Copy COBOL del programa
   */
  _EXT_ void processEndModule (char *data);

  /**
   * Realiza el proceso asociado al inicio de la sesion
   * @param current Puntero al TIMER obtenido al entrar en SDPTRAB
   */
  _EXT_ void sendBeginSession (TIMER *current);

  /**
   * Realiza el proceso asociado al final de la sesion
   * @param current Puntero al TIMER obtenido al entrar en SDPTRAB
   */
  _EXT_ void sendEndSession   (TIMER *current);

  /**
   * Procesa la informacion pendiente
   */
  _EXT_ void sendPendingData  (void);


#endif