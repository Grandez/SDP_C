/*
 * Programa para probar la libreria dinamica SDPTRAPB
 *
 * Author  : Javier Gonzalez
 * Date    : 01/03/15
 * Version : 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#include "jggsal.h"
#include "tstFunctions.h"
#include "SDPTRAPB.h"
#include "trapcodes.h"
#include "sdpconfig.h"

int modo  = 1;
int maxRC = 0;

void printRes(int res) {
    if (res != 0) {
        printf(" - KO\n");
        if (res > maxRC) maxRC = res;
    }
    else {
        printf(" - OK\n");
    }
}

/* Inicio y fin */

void caso000(void) {
    /* BEG, END, PARR, CALL , MOD , COVER, PERS */

    int msgs[][8] = {{1,1,0,0,1,1,1},{1,1,0,0,1,1,1},{1,1,0,0,1,1,1}};
    char nombre[] = "SDP00000"; 
    int res;
    
    printf("Ejecutando CASO000 en modo %d", modo);    
    
    creaCopy(0, nombre, 3, 2, 1);
    call(BEG_MODULE, 0, "SDP00000");

    call(END_MODULE, 0, "SDP00000");
    res = check(msgs[modo - 1]);
    printRes(res);
}

void caso001(void) {
    /* BEG, END, PARR, CALL , MOD , COVER, PERS */

    int msgs[][8] = {{1,1,1,0,1,1,1},{1,1,1,0,1,1,1},{1,1,1,0,1,1,1}};
    int res;

    printf("Ejecutando CASO001 en modo %d", modo);    

    creaCopy(0, "SDP00001", 3, 2, 1);
    call(BEG_MODULE,    0, "SDP00001");
    call(BEG_PARAGRAPH, 0, "PARRAFO1");
    call(END_PARAGRAPH, 0, "PARRAFO1");
    call(END_MODULE,    0, "SDP00001");

    res = check(msgs[modo - 1]);
    printRes(res);
}

void caso002(void) {
    /* BEG, END, PARR, CALL , MOD , COVER, PERS */

    int msgs[][8] = {{1,1,1,0,1,1,1},{1,1,1,0,1,1,1},{1,1,5,0,1,1,1}};
    int res;
    int loop;

    printf("Ejecutando CASO002 en modo %d", modo);    

    creaCopy(0, "SDP00002", 3, 2, 1);
    call(BEG_MODULE, 0, "SDP00002");
    for (loop = 0; loop < 5; loop++) {
      call(BEG_PARAGRAPH, 0, "PARRAFO1");
      call(END_PARAGRAPH, 0, "PARRAFO1");
    }
    call(END_MODULE, 0, "SDP00002");
    res = check(msgs[modo - 1]);
    printRes(res);
}


void caso010(void) {
    /* BEG, END, PARR, CALL , MOD , COVER, PERS */

    int msgs[][8] = {{1,1,2,1,2,2,2},{1,1,2,1,2,2,2},{1,1,2,1,2,2,2}};
    int res;

    printf("Ejecutando CASO010 en modo %d", modo);

    creaCopy(11, "SDP00011", 3, 2, 1);
    creaCopy(10, "SDP00010", 3, 2, 1);

    call(BEG_MODULE, 10,  "SDP00010");

    call(BEG_PARAGRAPH, 10, "PARRAFO1");
    
       call(BEG_CALL,   10,  "SDP00011");
       
          call(BEG_MODULE,    11, "SDP00011" );
          call(BEG_PARAGRAPH, 11, "PARRAFO10");
          call(END_PARAGRAPH, 11, "PARRAFO10");
          call(END_MODULE,    11, "SDP00011" );

      call(END_CALL,  10,  "SDP00011");

    call(END_PARAGRAPH, 10, "PARRAFO1");
    call(END_MODULE, 10,  "SDP00010");
    
    res = check(msgs[modo - 1]);
    printRes(res);
}

void caso011(void) {
    /* BEG, END, PARR, CALL , MOD , COVER, PERS */

    int msgs[][8] = {{1,1,2,1,2,2,2},{1,1,2,1,2,2,2},{1,1,2,1,2,2,2}};
    int res;

    printf("Ejecutando CASO011 en modo %d", modo);

    creaCopy(11, "SDP00011", 3, 2, 1);
    creaCopy(10, "SDP00010", 3, 2, 1);

    call(BEG_MODULE, 10,  "SDP00010");

    call(BEG_PARAGRAPH, 10, "PARRAFO1");
    
       call(BEG_CALL,   10,  "SDP00011");
       
          call(BEG_MODULE,    11, "SDP00011" );
          call(BEG_PARAGRAPH, 11, "PARRAFO10");
          call(END_PARAGRAPH, 11, "PARRAFO10");
          call(END_MODULE,    11, "SDP00011" );

      call(END_CALL,  10,  "SDP00011");

    call(END_PARAGRAPH, 10, "PARRAFO1");
    call(END_MODULE, 10,  "SDP00010");
    
    res = check(msgs[modo - 1]);
    printRes(res);
}

void caso020(void) {
    /* BEG, END, PARR, CALL , MOD , COVER, PERS */

    int msgs[][8] = {{1,1,0,2,3,3,0},{1,1,0,2,3,3,0},{1,1,0,2,3,3,0}};
    int res;

    printf("Ejecutando CASO020 en modo %d", modo);

    creaCopy(20, "SDP00020", 20, 0, 0);
    creaCopy(21, "SDP00021", 21, 0, 0);
    creaCopy(22, "SDP00022", 22, 0, 0);

    call(BEG_MODULE, 20,   "SDP00020");
        call(BEG_CALL, 20,     "SDP00021");

           call(BEG_MODULE, 21,  "SDP00021");
           call(BEG_CALL,   21,  "SDP00022");

               call(BEG_MODULE, 22, "SDP00022");
               call(END_MODULE, 22,  "SDP00022");

           call(END_CALL,    21,  "SDP00022");
           call(END_MODULE,  21,  "SDP00021");

        call(END_CALL, 20,     "SDP00021");
    call(END_MODULE,  20,  "SDP00020");
           
    res = check(msgs[modo - 1]);
    printRes(res);
}

void caso021(void) {
    /* BEG, END, PARR, CALL , MOD , COVER, PERS */

    int msgs[][8] = {{1,1,0,2,3,3,0},{1,1,0,6,11,11,0},{1,1,0,10,11,11,0}};
    int res;
    int loop;

    printf("Ejecutando CASO021 en modo %d", modo);

    creaCopy(20, "SDP00020", 20, 0, 0);
    creaCopy(21, "SDP00021", 21, 0, 0);
    creaCopy(22, "SDP00022", 22, 0, 0);

    call(BEG_MODULE, 20,   "SDP00020");
    for (loop = 0; loop < 5; loop++) {
        call(BEG_CALL, 20,     "SDP00021");

           call(BEG_MODULE, 21,  "SDP00021");
           call(BEG_CALL,   21,  "SDP00022");

               call(BEG_MODULE, 22, "SDP00022");
               call(END_MODULE, 22,  "SDP00022");

           call(END_CALL,    21,  "SDP00022");
           call(END_MODULE,  21,  "SDP00021");

        call(END_CALL, 20,     "SDP00021");
    }
    call(END_MODULE,  20,  "SDP00020");
           
    res = check(msgs[modo - 1]);
    printRes(res);
}

void caso022(void) {
    /* BEG, END, PARR, CALL , MOD , COVER, PERS */

    int msgs[][8] = {{1,1,0,3,3,3,0},{1,1,0,7,15,15,0},{1,1,0,14,15,15,0}};
    int res;
    int loop;

    printf("Ejecutando CASO020 en modo %d", modo);

    creaCopy(20, "SDP00022", 20, 0, 0);
    creaCopy(21, "SDP00021", 21, 0, 0);
    creaCopy(22, "SDP00022", 22, 0, 0);

    call(BEG_MODULE, 20,   "SDP00020");
    for (loop = 0; loop < 5; loop++) {
        call(BEG_CALL, 20,     "SDP00021");

           call(BEG_MODULE, 21,  "SDP00021");
           call(BEG_CALL,   21,  "SDP00022");

               call(BEG_MODULE, 22, "SDP00022");
               call(END_MODULE, 22,  "SDP00022");

           call(END_CALL,    21,  "SDP00022");
           call(END_MODULE,  21,  "SDP00021");

        call(END_CALL, 20,     "SDP00021");
    }
    for (loop = 0; loop < 4; loop++) {
           call(BEG_CALL,   20,  "SDP00022");

               call(BEG_MODULE, 22, "SDP00022");
               call(END_MODULE, 22,  "SDP00022");

           call(END_CALL,    20,  "SDP00022");
    }

    call(END_MODULE,  20,  "SDP00020");
           
    res = check(msgs[modo - 1]);
    printRes(res);
}

void caso023(void) {
    /* BEG, END, PARR, CALL , MOD , COVER, PERS */

    int msgs[][8] = {{1,1,2,1,2,2,0},{1,1,6,1,6,6,0},{1,1,60,5,6,6,0}};
    int res;
    int loop, loop2;

    printf("Ejecutando CASO023 en modo %d", modo);

    creaCopy(20, "SDP00020", 20, 2, 0);
    creaCopy(21, "SDP00021", 21, 2, 0);

    call(BEG_MODULE, 20,   "SDP00020");
        for (loop = 0; loop < 10; loop++) {
          call(BEG_PARAGRAPH, 20, "PARRAFO20");
          call(END_PARAGRAPH, 20, "PARRAFO20");
        }
        for (loop = 0; loop < 5; loop++) {
            call(BEG_CALL, 20,     "SDP00021");

                call(BEG_MODULE, 21,  "SDP00021");
                for (loop2 = 0; loop2 < 10; loop2++) {
                     call(BEG_PARAGRAPH, 21, "PARRAFO21");
                     call(END_PARAGRAPH, 21, "PARRAFO21");
                }
                call(END_MODULE,  21,  "SDP00021");

           call(END_CALL, 20,     "SDP00021");
        }
    call(END_MODULE,  20,  "SDP00020");
           
    res = check(msgs[modo - 1]);
    printRes(res);
}

void caso030(void) {
    /* BEG, END, PARR, CALL , MOD , COVER, PERS */

    int msgs[][8] = {{1,1,1,1,1,1,0},{1,1,1,1,1,1,0},{1,1,1,1,1,1,0}};
    int res;
    int loop, loop2;

    printf("Ejecutando CASO030 en modo %d", modo);

    creaCopy(20, "SDP00020", 20, 2, 0);
    creaCopy(21, "SDP00021", 21, 2, 0);

    call(BEG_MODULE, 20,   "SDP00020");
          call(BEG_PARAGRAPH, 20, "PARRAFO20");
          call(BEG_CALL, 20,     "NOMONITOR");
          call(END_CALL, 20,     "NOMONITOR");
          call(END_PARAGRAPH, 20, "PARRAFO20");
    call(END_MODULE,  20,  "SDP00020");
           
    res = check(msgs[modo - 1]);
    printRes(res);
}

void (*caso[])() = { caso000
                  , 0x00 };

/*
void (*caso[])() = { caso000
                    ,caso001 
                    ,caso010 
                    ,caso011
                    ,caso020
                    ,caso021
                    ,caso022
                    ,caso023
                   , 0x00 };
*/
int main(int argc, char* argv[])
{
    int idx = 0;

    SDPTRAPB_setDebugLevel(DBG_MSG + DBG_SESION);
    while (caso[idx] != 0x00) {
       for (modo = 1 ; modo < 4; modo++) {
          switch(modo) {
             case 3: SDPTRAPB_setModo(4); break;
             case 2: SDPTRAPB_setModo(2); break;
             case 1: SDPTRAPB_setModo(1); break;
          }
          caso[idx]();
       }
       idx++;
    }
    _getch();
	return maxRC;
}

