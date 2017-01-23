/**
 * @file
 * @brief Cliente Websphere MQSeries                                   
 *
 * La informacion relativa a QManager, Nombre de cola, etc se   
 * obtiene del entorno                                          
 *
 * @author  : Javier Gonzalez
 * @date    : 01/03/15
 * @version : 2.0
 *
 */

 #ifndef _AMQSDP_H_
   #include "jggsal.h"
   
   #undef _EXT_
   
   #ifdef _AMQSDP_SRC_
     #define _EXT_
   #else
     #define _EXT_ DLL_LOCAL
   #endif

   /**
    * Inicia el entorno MQ
    * Para depuracion es necesario pasarlo el nivel de debug y el fichero a utilizar
    */
   _EXT_ void initAMQ(int dbg, FILE *f);

   /**
    * Crea el buffer de mensajes
    */
   _EXT_ void setBufferSize();

   /**
    * Libera el buffer de mensajes
    */
   _EXT_ void unsetBufferSize();

   /**
    * Establece el numero de mensajes que se enviaran en un unico mensaje de la cola
    */
   _EXT_ void setMaxGroup(int group);

   /**
    * Escribe el mensaje en la cola
    */
   _EXT_ int  enviar(char *szMsg, unsigned char force);

   /**
    * Establece la conexion con el gestor de colas
    */
   _EXT_ int  MQConnect(void);

   /**
    * Libera la conexion con el gestor de colas
    */
   _EXT_ void MQDisconnect(void);  

   /**
    * Abre la cola de escritura
    */
   _EXT_ int  MQOpenOutput(void);

   /**
    * Abre la cola de lectura
    */
   _EXT_ int  MQOpenInput(void);

   /**
    * Escribe el mensaje en la cola
    */
   _EXT_ long MQPut(char *msg, long len);

   /**
    * Lee un mensaje de la cola
    */
   _EXT_ long MQGet(char *msg, long len);


#endif
