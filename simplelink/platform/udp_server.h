#ifndef TK_UDP_SERVER_H
#define TK_UDP_SERVER_H

#include "hal.h"

#define BUF_SIZE 1400

typedef struct
{
    uint16_t port;
} UdpServerConfig;

extern void startUdpServer(uint16_t port);

#endif
