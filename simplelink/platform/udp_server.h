#ifndef TK_UDP_SERVER_H
#define TK_UDP_SERVER_H

#include "hal.h"

typedef struct
{
    uint16_t port;
} UdpServerConfig;

extern void startUdpServer(int port);
extern void stopUdpServer(void);

#endif
