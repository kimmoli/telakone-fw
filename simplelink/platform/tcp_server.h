#ifndef TK_TCPSERVER_H
#define TK_TCPSERVER_H

#include "hal.h"

typedef struct
{
    uint16_t port;
} TcpServerConfig;

extern uint32_t tcpClientAddr;

extern int setReceiveTimeout(int sock, int ms);
extern int sendToSocket(void *data, int16_t Len);
extern int receiveFromSocket(void *buff, int16_t Maxlen, int16_t *rxLen);

extern void startTcpTermServer(int port);
extern void stopTcpTermServer(void);

#endif
