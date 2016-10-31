#include <stdint.h>
#include "hal.h"
#include "tcp_server.h"
#include "simplelink.h"
#include "messaging.h"
#include "helpers.h"
#include "tcp_stream.h"
#include "shell.h"
#include "shellcommands.h"

#define BUFSIZE               256

TcpServerConfig tcpserverconf =
{
    23
};

static int16_t sockId = 0;
static int16_t connSockId = 0;

static int setReceiveTimeout(int ms);
static int createSocket(uint16_t port);
static int waitForConnection(void);

uint32_t tcpClientAddr;

int setReceiveTimeout(int ms)
{
    int res;
    struct SlTimeval_t timeVal;

    timeVal.tv_sec = ms / 1000;
    timeVal.tv_usec = (ms % 1000) * 1000;
    res = sl_SetSockOpt(sockId, SL_SOL_SOCKET, SL_SO_RCVTIMEO, (uint8_t *)&timeVal, sizeof(timeVal));

    return res;
}

int createSocket(uint16_t port)
{
    SlSockAddrIn_t sServerAddress;
    int res = 0;

    sockId = sl_Socket(SL_AF_INET, SL_SOCK_STREAM, SL_IPPROTO_TCP);
    if (sockId > 0)
    {
        setReceiveTimeout(1000);

        sServerAddress.sin_family = SL_AF_INET;
        sServerAddress.sin_addr.s_addr = SL_INADDR_ANY;
        sServerAddress.sin_port = sl_Htons(port);

        res = sl_Bind(sockId, (const SlSockAddr_t *)&sServerAddress, sizeof(sServerAddress));

        res |= sl_Listen (sockId, 1);

        if (res == SL_SOC_OK)
        {
            PRINT("TCP Terminal listening on port %d\n\r", port);
            return MSG_OK;
        }
        else
        {
            sl_Close(sockId);
            return MSG_RESET;
        }
    }

    return MSG_RESET;
}

int waitForConnection(void)
{
    int res;
    SlFdSet_t rfds;
    struct SlTimeval_t tv;
    SlSocklen_t in_addrSize;
    SlSockAddrIn_t stcpClientAddress;

    SL_FD_ZERO(&rfds);
    SL_FD_SET(sockId, &rfds);

    tv.tv_sec = 1;
    tv.tv_usec = 0;

    res = sl_Select(sockId, &rfds, (SlFdSet_t *) 0, (SlFdSet_t *) 0, &tv);
    if(res == 0)
    {
        return SL_EAGAIN;
    }
    else if (res < 0)
    {
        DEBUG("error %d\n\r", res);
        return MSG_RESET;
    }

    in_addrSize = sizeof(stcpClientAddress);
    connSockId = sl_Accept(sockId, (SlSockAddr_t *)&stcpClientAddress, (SlSocklen_t *)&in_addrSize);

    if (connSockId == SL_EAGAIN)
    {
        return SL_EAGAIN;
    }
    else
    {
        if (connSockId <= 0)
        {
            DEBUG("error %d\n\r", connSockId);
            return MSG_RESET;
        }
    }

    tcpClientAddr = sl_Htonl(stcpClientAddress.sin_addr.s_addr);

    PRINT("TCP Terminal client connected from %d.%d.%d.%d\n\r",
        SL_IPV4_BYTE(tcpClientAddr, 3), SL_IPV4_BYTE(tcpClientAddr, 2),
        SL_IPV4_BYTE(tcpClientAddr, 1), SL_IPV4_BYTE(tcpClientAddr, 0));

    chThdSleepMilliseconds(10);

    return MSG_OK;
}

int sendToSocket(void *data, int16_t Len)
{
    volatile int8_t retries;

    int res = sl_Send(connSockId, data, Len, 0);

    if (res > 0)
    {
        retries = 0;
        return MSG_OK;
    }
    else if ((retries++ > 10) && (res == SL_EAGAIN))
    {
        return SL_EAGAIN;
    }
    else
    {
        DEBUG("error %d\n\r", res);
        retries = 0;
        return MSG_RESET;
    }
}

int receiveFromSocket(void *buff, int16_t Maxlen, int16_t *rxLen)
{
    *rxLen = sl_Recv(connSockId, buff, Maxlen, 0);

    if (*rxLen == SL_EAGAIN)
        return SL_EAGAIN;
    else if (*rxLen > 0)
        return MSG_OK;

    return MSG_RESET;
}

static THD_FUNCTION(tcpTermServer, arg)
{
    TcpServerConfig *config = arg;

    int res;
    thread_t *stp;

    res = createSocket(config->port);
    if (res == MSG_RESET)
    {
        PRINT("Failed to create socket\n\r");
        chThdExit(MSG_OK);
    }

    while (!chThdShouldTerminateX())
    {
        do
        {
            chThdSleepMilliseconds(100);

            res = waitForConnection();

            if (res == MSG_RESET)
            {
                PRINT("Failed while waiting for connection\n\r");
            }

            if (chThdShouldTerminateX())
                res = MSG_RESET;
        }
        while (res == SL_EAGAIN);

        if (res == MSG_OK)
        {
            tcpStreamInit(&TCPD1);
            consoleStream = (BaseSequentialStream *) &TCPD1;

            PRINT("Welcome to " BOARD_NAME "\n\n\r");

            stp = chThdCreateFromHeap(NULL, SHELL_WA_SIZE, "tcpshell", NORMALPRIO + 1, shellThread, (void *)&shell_cfg_tcp);

            while (!chThdTerminatedX(stp))
            {
                chThdSleepMilliseconds(200);
            }

            tcpStreamStop(&TCPD1);
            consoleStream = (BaseSequentialStream *) &SD3;
            tcpClientAddr = 0;
            PRINT("Connection closed\n\r");
        }

        sl_Close(connSockId);
        chThdSleepMilliseconds(100);
    }

    sl_Close(sockId);
    chThdExit(MSG_OK);
}

void startTcpTermServer(int port)
{
    thread_t *tp;
    tp = chRegFindThreadByName("tcpterm");

    if (!tp)
    {
        if (port > 0)
            tcpserverconf.port = port;

        chThdCreateFromHeap(NULL, THD_WORKING_AREA_SIZE(512), "tcpterm", NORMALPRIO+2, tcpTermServer, (void *) &tcpserverconf);
    }
    else
    {
        PRINT("TCP Terminal already running.\n\r");
    }
}

void stopTcpTermServer(void)
{
    thread_t *tp;
    tp = chRegFindThreadByName("tcpterm");

    if (tp)
    {
        chThdTerminate(tp);
        chThdWait(tp);

        PRINT("TCP Server stopped\n\r");
        return;
    }
    else
    {
        PRINT("TCP Server not running\n\r");
        return;
    }
}
