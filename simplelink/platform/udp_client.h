#ifndef TK_UDP_CLIENT_H
#define TK_UDP_CLIENT_H

#include "hal.h"

extern int udpSend(uint32_t destinationIP, char *hostName, uint16_t port, char *data, int len);

#endif

