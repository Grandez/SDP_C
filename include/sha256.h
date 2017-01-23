/*
 * Fichero de cabecera para el calculo de la firma SHA de 256 bits  de un texto
 * Modificado a partir del algoritmo desarrollado por:
 * 
 * US Secure Hash Algorithms (RFC 6234).
 *
 * Homepage:
 * https://tools.ietf.org/html/rfc6234#page-91
 *
 */

#ifndef _SHA256_H_
  #define _SHA256_H_
  
  #undef _EXT_
  #ifdef _SHA256_SRC_
     #define _EXT_
  #else
     #define _EXT_ extern
  #endif

  /* 
   * Devuelve la firma MD5 asociada a in
   * out debe ser un array de 32 bytes
   */
  _EXT_ void SHA256 (char *out , const char *in);
#endif