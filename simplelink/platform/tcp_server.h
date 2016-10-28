#ifndef TK_TCPSERVER_H
#define TK_TCPSERVER_H

#include "hal.h"

typedef struct
{
    uint16_t port;
} TcpServerConfig;

extern void startTcpTermServer(int port);
extern uint32_t getTcpMessageCount(void);

#endif
