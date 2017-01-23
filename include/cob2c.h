/**
 * @file
 * @brief Fichero de cabecera para convertir la copy COBOL
 *        en una estructura SDP
 *
 * @author  : Javier Gonzalez
 * @date    : 01/03/15
 * @version : 2.0
 *
 */

#ifndef _COB2C_H_
   #undef _EXT_
   
   #ifdef _COB2C_SRC_
     #define _EXT_
   #else
     #define _EXT_ DLL_LOCAL
   #endif

   /*****************************************************************
    * Posiciones de los campos en la structura COBOL 
    *
    * 	03 TRAP    PIC X(08) VALUE 'SDPTRAPB'
    *   03 NAME    PIC X(32) VALUE  '${MODULE}'
    *   03 MD5ID   PIC X(32)
    *   03 FLG-DATA  PIC X(16)
    *   03 DEPTH BINARY-LONG	
    *   03 SDPCNT
    *      05 NBLK BINARY-LONG VALUE ${BLOCKS}
    *      05 NPAR BINARY-LONG VALUE ${NPARRS}
    *      05 NPRS BINARY-LONG VALUE ${NFILES}	
    *   03 03 COB OCCURS ${BLOCKS} TIMES PIC 9(01)
    *   03 03 PAR OCCURS ${PARRS}  TIMES BINARY-LONG
    *   03 03 PRS OCCURS ${FILES}
    *     05 ACC OCCURS 8 TIMES BINARY-LONG VALUE
	*****************************************************************/
   #define COB_TRAP        0
   #define COB_MODULE      8
   #define COB_MODULE_LEN  MAX_MODULE
   #define COB_FIRMA       COB_MODULE + COB_MODULE_LEN
   #define COB_FIRMA_LEN   HASH_SIZE
   #define COB_FLAGS       COB_FIRMA + COB_FIRMA_LEN   
   #define COB_FLAGS_LEN   16
   #define COB_DEPTH       COB_FLAGS + COB_FLAGS_LEN
   #define COB_DEPTH_LEN   sizeof(long)
   #define COB_BLK         COB_DEPTH + COB_DEPTH_LEN   
   #define COB_BLK_LEN     (3 * sizeof(long))
   #define COB_COVER       COB_BLK + COB_BLK_LEN

   /**
    * Convierte la copy COBOL en una estructura SDP
    * @param sdp    Puntero a la estructura SDP
    * @param data   Puntero a la copy COBOL
    * @param modulo Segundo parametro de invocacion (nombre del modulo o parrafo)
    * @param len    Longitud del campo modulo
    */
   _EXT_ void copyCobolToCStruct(SDP *sdp, char *data, char *modulo, int len);

   /* Se usan en entornos de pruebas */
   _EXT_ char *CStructToCopyCobol(SDP *sdp);
   _EXT_ void initSDP(SDP *sdp);
   _EXT_ void setModuleName(SDP *sdp, char *name);
   _EXT_ void setParagraph(SDP *sdp, char *name);
   _EXT_ void setCoverage(SDP *sdp, char *name);

#endif