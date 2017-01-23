/**
 * @file
 * @brief Fichero de cabecera para el calculo de la firma MD5 de un texto
 * Modificado a partir del algoritmo desarrollado por:
 * 
 * MD5 Message-Digest Algorithm (RFC 1321).
 *
 * Homepage:
 * http://openwall.info/wiki/people/solar/software/public-domain-source-code/md5
 *
 * Author:
 * Alexander Peslyak, better known as Solar Designer <solar at openwall.com>
 *
 * See md5.c for more information.
 *
 * @author  : Javier Gonzalez
 * @date    : 01/03/15
 * @version : 2.0
 *
 */

#ifndef _MD5_H_
  #define _MD5_H_
  
  #undef _EXT_
  #ifdef _MD5_SRC_
     #define _EXT_
  #else
     #define _EXT_ DLL_LOCAL
  #endif

  /** 
   * Devuelve la firma MD5 asociada a in
   * out debe ser un array de 32 bytes
   */
  _EXT_ void MD5 (char *out , const char *in);
#endif