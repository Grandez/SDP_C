/*********************************************************************
* Filename:   sha256.h
* Author:     Brad Conte (brad AT bradconte.com)
* Copyright:
* Disclaimer: This code is presented "as is" without any guarantees.
* Details:    Defines the API for the corresponding SHA1 implementation.
*********************************************************************/

#ifndef SHA256_LE_H_
#define SHA256_LE_H_

/*************************** HEADER FILES ***************************/
#include <stddef.h>

/****************************** MACROS ******************************/
#define SHA256_BLOCK_SIZE 32            // SHA256 outputs a 32 byte digest

/**************************** DATA TYPES ****************************/
typedef unsigned char BYTE;             // 8-bit byte
typedef unsigned int  WORD;             // 32-bit word, change to "long" for 16-bit machines

typedef struct {
	BYTE data[64];
	WORD datalen;
	unsigned long long bitlen;
	WORD state[8];
} SHA256_CTX;

#undef _EXT_
#ifdef _SHA256LE_C_
  #define _EXT_
#else
  #define _EXT_ extern
#endif

/*********************** FUNCTION DECLARATIONS **********************/
_EXT_ void sha256_init(SHA256_CTX *ctx);
_EXT_ void sha256_update(SHA256_CTX *ctx, const BYTE data[], size_t len);
_EXT_ void sha256_final(SHA256_CTX *ctx, BYTE hash[]);

#endif   // SHA256_H

