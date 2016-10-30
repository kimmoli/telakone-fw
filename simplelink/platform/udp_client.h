#ifndef TK_UDP_CLIENT_H
#define TK_UDP_CLIENT_H

#include "hal.h"

typedef struct
{
    char *hostname;
    uint16_t port;
} UdpStatusSendConfig;

extern int udpSend(uint32_t destinationIP, char *hostName, uint16_t port, char *data, int len);

extern void startUdpStatusSend(char *hostname, int port);
extern void stopUdpStatusSend(void);

#endif

