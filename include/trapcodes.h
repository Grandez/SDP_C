#ifndef _TRAP_H_
   #define _TRAP_H_

	#define END_MARK     0x01
	
	#define HTRAP       0x100  //  256
	#define HCODE       0x200  //  512
	#define HACCESS     0x400  // 1024
	#define HUNDEF2     0x800
	#define HUNDEF3    0x1000
	#define HUNDEF4    0x2000
    #define HUNDEF5    0x4000
	#define HUNDEF     0x8000

	// Mascara del grupo
	
	#define TRAP         0x01
	#define CODE         0x02
	#define ACCESS       0x04
	#define UNDEF2       0x08
	#define UNDEF3       0x10
	#define UNDEF4       0x20
    #define UNDEF5       0x40
	#define UNDEF        0x80

	// Mascara de TRAP
	
	#define MODULE       0x02
	#define PARAGRAPH    0x04
	#define CALL         0x08
	#define SQL          0x10
	#define CICS         0x20
	#define CUSTOM       0x40
	
	// Usados en la libreria
	#define COVER       0x100
	#define PERS        0x200
	
	// Mascara de CODE
	
	#define BLOQUE       0x02
	#define WORKINGS     0x04
	
	// Mascara de ACCESS
	
    #define OPEN       0x01
    #define CLOSE      0x02
	#define INSERT     0x04
	#define READ       0x08	
	#define UPDATE     0x10
    #undef DELETE
	#define DELETE     0x20
	#define SELECT     0x40
	#define TOTAL      0x80
	

	// Valores de code
	
	#define BLOCK    HCODE + BLOQUE    // 514
	#define WORKING  HCODE + WORKINGS  // 516
	
	// Valores BEG
	
	#define BEG_MODULE    HTRAP + MODULE    // 258
	#define BEG_PARAGRAPH HTRAP + PARAGRAPH // 260
	#define BEG_CALL      HTRAP + CALL
	#define BEG_SQL       HTRAP + SQL
	#define BEG_CICS      HTRAP + CICS
	#define BEG_INST      HTRAP + INST
	#define BEG_CUSTOM    HTRAP + CUSTOM
	
	// Valores END
	
	#define END_MODULE       BEG_MODULE    + END_MARK // 259
	#define END_PARAGRAPH    BEG_PARAGRAPH + END_MARK
	#define END_CALL         BEG_CALL      + END_MARK
	#define END_SQL          BEG_SQL       + END_MARK
	#define END_CICS         BEG_CICS      + END_MARK
	#define END_INST         BEG_INST      + END_MARK
	#define END_CUSTOM       BEG_CUSTOM    + END_MARK
	
    // Valores ACESS 

	#define ACC_OPEN    HACCESS + OPEN   // 1025
	#define ACC_CLOSE   HACCESS + CLOSE  // 1026
	#define ACC_INSERT  HACCESS + INSERT
	#define ACC_READ    HACCESS + READ
	#define ACC_UPDATE  HACCESS + UPDATE
	#define ACC_DELETE  HACCESS + DELETE
	#define ACC_SELECT  HACCESS + SELECT

    #define MSG_BEG      0x00
    #define MSG_END      0x01
    #define MSG_COVER   0x100
    #define MSG_PERS    0x200
    #define MSG_PARRS   0x400

#endif
