#include <stdint.h>
#include "hal.h"
#include "tcp_server.h"
#include "simplelink.h"
#include "helpers.h"

#define BUFSIZE               256

static char *rxBuff;

static int16_t sockId = 0;
static int16_t connSockId = 0;

static const char *welcomeMessage = "Welcome to " BOARD_NAME "\n\n\r";
static const char *goodbyeMessage = "Bye!\n\r";

#if 0
static int setNonBlockingMode(int mode)
{
    int res;
    SlSockNonblocking_t nonBlockingOption;

    nonBlockingOption.NonblockingEnabled = mode;
    res = sl_SetSockOpt(sockId, SOL_SOCKET, SL_SO_NONBLOCKING, &nonBlockingOption, sizeof(nonBlockingOption));

    return res;
}
#endif

static int setReceiveTimeout(int ms)
{
    int res;
    struct SlTimeval_t timeVal;

    timeVal.tv_sec = ms / 1000;
    timeVal.tv_usec = (ms % 1000) * 1000;
    res = sl_SetSockOpt(sockId, SL_SOL_SOCKET, SL_SO_RCVTIMEO, (uint8_t *)&timeVal, sizeof(timeVal));

    return res;
}

static int createSocket(uint16_t port)
{
    SlSockAddrIn_t sServerAddress;
    int res = 0;

    sockId = sl_Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockId > 0)
    {
        setReceiveTimeout(1000);

        sServerAddress.sin_family = AF_INET;
        sServerAddress.sin_addr.s_addr = INADDR_ANY;
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

static int waitForConnection(void)
{
    SlSocklen_t in_addrSize;
    SlSockAddrIn_t sClientAddress;

    in_addrSize = sizeof(sClientAddress);
    connSockId = sl_Accept(sockId, (SlSockAddr_t *)&sClientAddress, (SlSocklen_t *)&in_addrSize);

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

    uint32_t clientAddr = sl_Htonl(sClientAddress.sin_addr.s_addr);

    PRINT("TCP Terminal client connected from %d.%d.%d.%d\n\r",
        SL_IPV4_BYTE(clientAddr, 3), SL_IPV4_BYTE(clientAddr, 2),
        SL_IPV4_BYTE(clientAddr, 1), SL_IPV4_BYTE(clientAddr, 0));

    return MSG_OK;
}

static int sendToSocket(void *data, int16_t Len)
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

static int receiveFromSocket(void *buff, _i16 Maxlen, _i16 *rxLen)
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
    (void) arg;
    int res;

    res = createSocket(23);
    if (res == MSG_RESET)
    {
        PRINT("Failed to create socket\n\r");
        chHeapFree(rxBuff);
        chThdExit(MSG_OK);
    }

    while (true)
    {
        //setNonBlockingMode(true);

        do
        {
            chThdSleepMilliseconds(100);

            res = waitForConnection();

            if (res == MSG_RESET)
            {
                PRINT("Failed while waiting for connection\n\r");
            }
        }
        while (res == SL_EAGAIN);

        if (res == MSG_OK)
        {
            //setNonBlockingMode(false);

            sendToSocket((char *) welcomeMessage, strlen(welcomeMessage));

            while (res != MSG_RESET)
            {
                int16_t rxLen;

                chThdSleepMilliseconds(20);

                res = receiveFromSocket(rxBuff, BUFSIZE, &rxLen);

                if (res == MSG_OK)
                {
                    /* Just dump the data out */
                    dump(rxBuff, (int)rxLen);
                    if (strncmp(rxBuff, "exit", 4) == 0)
                    {
                        sendToSocket((char *) goodbyeMessage, strlen(goodbyeMessage));
                        break;
                    }
                }
                else if (res == MSG_RESET)
                {
                    break;
                }
            }

            PRINT("Connection closed\n\r");
        }
        sl_Close(connSockId);
        chThdSleepMilliseconds(100);
    }

    sl_Close(sockId);
    chHeapFree(rxBuff);
    chThdExit(MSG_OK);
}

void startTcpTermServer(void)
{
    rxBuff = chHeapAlloc(NULL, BUFSIZE);
    chThdCreateFromHeap(NULL, THD_WORKING_AREA_SIZE(2048), "tcpterm", NORMALPRIO+1, tcpTermServer, NULL);
}
