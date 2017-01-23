/**
 * @file                                                              
 * @brief Cliente Websphere MQSeries                                   
 *                                                              
 * La informacion relativa a QManager, Nombre de cola, etc se   
 * obtiene del entorno                                          
 *
 * @author : Javier Gonzalez 
 * @date   : 01/04/2015
 * @version: 1.0
 *
 */

#define _AMQSDP_SRC_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include "cmqc.h"

#include "SDPTRAP.h"
#include "amqsdp.h"
#include "global.h"

MQOD     od = {MQOD_DEFAULT};    /* Object Descriptor             */
MQMD     md = {MQMD_DEFAULT};    /* Message Descriptor            */
MQPMO   pmo = {MQPMO_DEFAULT};   /* put message options           */
MQCNO   cno = {MQCNO_DEFAULT};   /* connection options            */
MQCSP   csp = {MQCSP_DEFAULT};   /* security parameters           */

MQHCONN  Hcon   = 0x0;           /* connection handle             */
MQHOBJ   HQueue = 0x0;           /* object handle                 */
MQLONG   O_options;              /* MQOPEN options                */
MQLONG   C_options;              /* MQCLOSE options               */
MQLONG   mqcc;                   /* completion code               */
MQLONG   mqrc;                   /* reason code                   */

char *qname = DEF_QUEUE;

#ifdef SDP_BUFF_SIZE
   long buffSize = SDP_BUFF_SIZE;
 #else
   long buffSize = 4095;
#endif

int   msgs     = 0;             /* Numero actual de mensajes        */
int   maxGroup = ITEMS_BY_MSG;  /* Numero de mensajes agrupados     */
int   offset   = 0;             /* Puntero a la cola del buffer     */
char *buffer   = 0x0;           /* Buffer de envio de mensajes      */

/* No se por que, no los coge en este modulo */

int inErr;
int dbgLevel;
FILE *traza;


/********************************************************************/
/*                    PROTOTYPES                                    */
/********************************************************************/

void MQFree(void);
int  MQConn();
void MQDisconn();
int  MQOpen();
void MQClose();

/********************************************************************/
/*                    PUBLIC FUNCTIONS                              */
/********************************************************************/

/* No se porque no reconoce los extern */
_EXT_ void initAMQ(int d, FILE *f) {
    dbgLevel = d;
    traza = f;
}

_EXT_ void setBufferSize() {
   if (buffSize < 4095) buffSize = 4095;
   buffer = (char *) malloc(buffSize); 
   offset = 0;
   buffer[0] = 0x0;
}

_EXT_ void unsetBufferSize() {
   if (buffer != 0x0) free(buffer);
}

_EXT_ void setMaxGroup(int group) {
    maxGroup = group;
}

/**
 * Envia el mensaje si force = true o se han alcanzado el
 * el numero de mensajes a agrupar
 *
 * Si no lo almacena en el buffer para evitar envios masivos
 * mensajes
 * Se verifica si el mensaje cabe en el buffer alocado
 * antes de alamacenarlo, seria el caso en el que el numero
 * mensajes a agrupar es excesivo
 *
 */
_EXT_ int enviar(char *szMsg, unsigned char force) {

    if (dbgLevel & DBG_MSG) fprintf(traza, "MESAGE: %s\n", szMsg);

    if ((long) (strlen(buffer) + strlen(szMsg)) >= buffSize) {
        if (dbgLevel & DBG_SEND) fprintf(traza, "SEND  : %s\n", buffer);
        inErr = MQPut(buffer, strlen(buffer));
        msgs = 0;
        buffer[0] = 0x00;
    }

    if (msgs > 0) strcat(buffer, ":");
    strcat(buffer, szMsg);
    msgs++;

    if (force || msgs == maxGroup) {
        if (dbgLevel & DBG_SEND) fprintf(traza, "SEND  : %s\n", buffer);
        inErr = MQPut(buffer, strlen(buffer));
        msgs = 0;
        buffer[0] = 0x0;
    }
    return inErr;
}

_EXT_ int MQConnect() {
    int  res;
    char QMName[50] = ""; 

    char *var = getenv("SDP_QMGR");
    if (var != 0x0) strcpy(QMName, var);
    var = getenv("SDP_QUEUE");
    if (var != 0x0) qname = strdup(var);

     res = MQConn(QMName);
//     if (Hcon == 0x0) return res;
//     return MQOpenOutput();
     return res;
}

_EXT_ void MQDisconnect() {
     MQClose();
     MQDisconn();
}


_EXT_ long MQPut(char *msg, long len) {

   if (HQueue == 0x0) return 0;

   memcpy(md.Format, MQFMT_STRING, (size_t)MQ_FORMAT_LENGTH);
   

   pmo.Options = MQPMO_NO_SYNCPOINT
               | MQPMO_FAIL_IF_QUIESCING
               | MQPMO_NEW_MSG_ID   
               | MQPMO_NEW_CORREL_ID;      

   memcpy(md.MsgId, MQMI_NONE, sizeof(md.MsgId) );
   MQPUT(Hcon, HQueue, &md, &pmo, len, msg, &mqcc, &mqrc);

   if (mqcc == MQCC_FAILED) {
     fprintf(stderr, "SDP%05d Error enviando mensaje\n", mqrc);
     return mqrc;
   }

   return 0;
}

_EXT_ long MQGet(char *buff, long len) {

   MQGMO   gmo = {MQGMO_DEFAULT};  
   MQLONG  messlen;

   if (HQueue == 0x0) return 0;
   
   memcpy(md.MsgId, MQMI_NONE, sizeof(md.MsgId));
   memcpy(md.CorrelId, MQCI_NONE, sizeof(md.CorrelId));

   md.Encoding       = MQENC_NATIVE;
   md.CodedCharSetId = MQCCSI_Q_MGR;

   gmo.Options = MQGMO_WAIT           /* wait for new messages       */
               | MQGMO_NO_SYNCPOINT   /* no transaction              */
               | MQGMO_CONVERT;       /* convert if necessary        */

   gmo.WaitInterval = 1000;          /* 1 second limit for waiting */


   MQGET(Hcon, HQueue, &md, &gmo, len, buff, &messlen, &mqcc, &mqrc);
   if (mqcc == 0) buff[messlen] = 0x0;

   return mqrc; 
}

/********************************************************************/
/*                    PRIVATE FUNCTIONS                             */
/********************************************************************/

int MQConn(char *QMName) {
   
   fprintf(stderr, "Conectando a %s\n", QMName);
   MQCONNX(QMName, &cno,  &Hcon, &mqcc,  &mqrc);

   if (mqcc == MQCC_FAILED) {
     fprintf(stderr, "SDP%05d Error conectando al servidor de colas\n", mqrc);
     return mqrc;
   }
   return 0;
}

void MQDisconn() {
     MQDISC(&Hcon, &mqcc, &mqrc);  
}

int MQOpenOutput() {

     O_options = MQOO_OUTPUT            /* open queue for output     */
               | MQOO_FAIL_IF_QUIESCING /* but not if MQM stopping   */
               ;                   
     return MQOpen();
}

int MQOpenInput() {
     O_options = MQOO_INPUT_AS_Q_DEF    /* open queue for input      */
               | MQOO_FAIL_IF_QUIESCING /* but not if MQM stopping   */
               ;                        /* = 0x2001 = 8193 decimal   */
    return MQOpen();
}

int MQOpen() {
    strncpy(od.ObjectName, qname, (size_t)MQ_Q_NAME_LENGTH);

    MQOPEN(Hcon, &od, O_options, &HQueue, &mqcc, &mqrc); 

   if (mqcc == MQCC_FAILED)    {
     fprintf(stderr, "SDP%05d Error abriendo la cola %s\n", mqrc, qname);
     return mqrc;
   }

   return 0;
}

void MQClose() {
     MQCLOSE(Hcon, &HQueue, C_options, &mqcc, &mqrc);
}
