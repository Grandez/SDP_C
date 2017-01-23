#include <stdio.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>

#define _SHA256_SRC_

#include "sha256le.h"
#include "sha256be.h"


_EXT_ void SHA256(char *out, const unsigned char *in) {
    BYTE res[33];

    SHA256_CTX    ctxLe;
    SHA256Context ctxBe;

    unsigned int x = 0x76543210;
    char *c = (char*) &x;

    unsigned int len = strlen ((const char *)in);

    // Chequear arquitectura: Big Endian o Little Endian
  if (*c == 0x10)   {  // Little Endian
      sha256_init(&ctxLe);
	  sha256_update(&ctxLe, in, len);
	  sha256_final(&ctxLe, res);
  }
  else   { // Big Endian
     SHA256Reset(&ctxBe);
     SHA256Input(&ctxBe, in, len);
     SHA256Result(&ctxBe, res);
  }


    /* Evitar el bucle para optimizar el rendimiento */
    sprintf(out, 
        "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
        res[0],  res[1],  res[2],  res[3],  res[4],  res[5],  res[6],  res[7], 
        res[8],  res[9],  res[10], res[11], res[12], res[13], res[14], res[15],
        res[16], res[17], res[18], res[19], res[20], res[21], res[22], res[23], 
        res[24], res[25], res[26], res[27], res[28], res[29], res[30], res[31]);

}

