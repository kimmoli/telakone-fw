#include <stdlib.h>
#include <string.h>
#include "udp_server.h"
#include "hal.h"
#include "wifi.h"
#include "messaging.h"
#include "helpers.h"

UdpServerConfig udpserverconf =
{
    4554
};

static thread_t *udpServerTp = NULL;

static THD_FUNCTION(udpServer, arg)
{
    UdpServerConfig *config = arg;

    SlSockAddrIn_t addr;
    SlSockAddrIn_t localAddr;
    uint16_t addrSize = 0;
    int16_t sockID = 0;
    int16_t res = 0;

    char *rxBuf;

    rxBuf = chHeapAlloc(NULL, MSGBUFSIZE * sizeof(char));

    localAddr.sin_family = SL_AF_INET;
    localAddr.sin_port = sl_Htons((uint16_t)config->port);
    localAddr.sin_addr.s_addr = 0;


    sockID = sl_Socket(SL_AF_INET,SL_SOCK_DGRAM, 0);
    if( sockID < 0 )
    {
        PRINT("[UDP] socket error %d\n\r", sockID);
        chHeapFree(rxBuf);
        chThdExit(MSG_RESET);
    }

    /* make socket non-blocking by setting 1 sec timeout */
    struct SlTimeval_t timeVal;
    timeVal.tv_sec = 5;
    timeVal.tv_usec = 0;
    sl_SetSockOpt(sockID, SL_SOL_SOCKET, SL_SO_RCVTIMEO, (uint8_t *)&timeVal, sizeof(timeVal));

    addrSize = sizeof(SlSockAddrIn_t);

    res = sl_Bind(sockID, (SlSockAddr_t *)&localAddr, addrSize);
    if( res < 0 )
    {
        PRINT("[UDP] bind error %d\n\r", res);
        sl_Close(sockID);
        chHeapFree(rxBuf);
        chThdExit(MSG_RESET);
    }

    PRINT("[UDP] server listening port %d\n\r", config->port);

    while (!chThdShouldTerminateX())
    {
        res = sl_RecvFrom(sockID, rxBuf, MSGBUFSIZE, 0, (SlSockAddr_t *)&addr, (SlSocklen_t*)&addrSize );

        if (res == SL_EAGAIN)
            continue;

        if (res < 0)
        {
            sl_Close(sockID);
            PRINT("[UDP] receive error %d\n\r", res);
            chHeapFree(rxBuf);
            chThdTerminate(chThdGetSelfX());
        }
        else
        {
            PRINT("[UDP] from %d.%d.%d.%d\n\r",
                    SL_IPV4_BYTE(addr.sin_addr.s_addr, 0), SL_IPV4_BYTE(addr.sin_addr.s_addr, 1),
                    SL_IPV4_BYTE(addr.sin_addr.s_addr, 2), SL_IPV4_BYTE(addr.sin_addr.s_addr, 3));

            dump(rxBuf, res);

            chBSemWait(&messagingReceiceSem);
            memcpy(messagingReceiveBuffer, rxBuf, res);
            chBSemSignal(&messagingReceiceSem);

            chEvtBroadcastFlagsI(&messagingEvent, MESSAGING_EVENT_SEND | (MIN(res, 0x3FF)));
        }
    }

    sl_Close(sockID);
    chHeapFree(rxBuf);
    chThdExit(MSG_OK);
}

void startUdpServer(int port)
{
    if (port == -1)
    {
        if (udpServerTp)
        {
            chThdTerminate(udpServerTp);
            chThdWait(udpServerTp);
            udpServerTp = NULL;

            PRINT("[UDP] server stopped\n\r");
            return;
        }
        else
        {
            PRINT("[UDP] server not running\n\r");
            return;
        }
    }
    else if (!udpServerTp)
    {
        if (port > 0)
            udpserverconf.port = port;

        udpServerTp = chThdCreateFromHeap(NULL, THD_WORKING_AREA_SIZE(256), "udpserver", NORMALPRIO+1, udpServer, (void *) &udpserverconf);
    }
    else
    {
        PRINT("[UDP] server already running\n\r");
    }
}
