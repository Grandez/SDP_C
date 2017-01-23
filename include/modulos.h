/**
 * @file
 * @brief Funciones de gestion de los modulos activos en una sesion
 *
 * @author  : Javier Gonzalez
 * @date    : 01/03/15
 * @version : 2.0
 *
 */

#ifndef _MODULOS_H_
   #define _MODULOS_H_
   
   #ifdef _EXT_
      #undef _EXT_
   #endif
   
   #ifdef _MODULOS_C_
      #define _EXT_
   #else
      #define _EXT_ extern
   #endif

   /**
    * Inserta un modulo en la lista de modulos
    * @param sdp Puntero a la estructura SDP actual
    * @param mode Indicador de disponibilidad de la estructura SDP
    */
   _EXT_ MOD   *addModule(SDP *sdp, int mode);

   /**
    * Inserta una nueva llamada desde ese modulo a otro
    * @param modulo Puntero al TIMER obtenido al entrar en la rutina
    */
   _EXT_ void   addCall(TIMER *modulo);

   /**
    * Inserta una nueva llamada a un parrafo en ese modulo
    * @param modulo Puntero al TIMER obtenido al entrar en la rutina
    */
   _EXT_ void   addParrafo(TIMER *timer);

   /**
    * Inserta la informacion relativa a la cobertura de codigo
    * @param mask Datos de la cobertura
    */
   _EXT_ void   addCoverage(char *mask);

   /**
    * Inserta la informacion relativa al uso global de los parraos del modulo
    * @param tbParras Tabla de contadores de uso de parrafos
    * @param largo    Longitud de la tabla
    */
   _EXT_ void   addUsoParrafos(unsigned long *tbParrs, int largo);

   /**
    * Inserta la informacion relativa al uso de los ficheros del modulo
    * @param tbFiles Tabla de contadores de uso de los ficheros
    * @param largo   Numero de ficheros (Elementos en tbFiles)
    */
   _EXT_ void   addFiles(unsigned long *tbFiles, int largo);

   /**
    * Inicializa el modulo
    * Se utiliza cuando el modo de proceso es MODE_MODULE
    */
   _EXT_ void   resetModule(void);

   /**
    * Establece el modulo pasado como parametro como modulo acivo
    * @param puntero a MOD con el modulo activo
    */
   _EXT_ void   setCurrentModule(MOD *mod);

   /**
    * Devuelve el modulo activo
    * @return Puntero al modulo activo
    */
   _EXT_ MOD   *getCurrentModule(void);

   /**
    * Devuelve el primer modulo de la lista de modulos ejecutados
    * @return Puntero al primer modulo
    */
   _EXT_ MOD   *getFirstModule(void);

   /**
    * Devuelve el siguiente modulo de la lista de modulos ejecutados
    * @return Puntero al siguiente modulo
    */
   _EXT_ MOD   *getNextModule(void);

   /**
    * Realiza los procesos de limpieza de memoria
    */
   _EXT_ void cleanModules(void);
#endif