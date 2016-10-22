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

static int createSocket(uint16_t port)
{
    SlSockAddrIn_t sServerAddress;
    int res = 0;

    sockId = sl_Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockId > 0)
    {
        DEBUG("Socket created.\n\r");

        /* make socket non-blocking by setting 1 sec timeout */
        struct SlTimeval_t timeVal;
        timeVal.tv_sec = 5;
        timeVal.tv_usec = 0;
        sl_SetSockOpt(sockId, SL_SOL_SOCKET, SL_SO_RCVTIMEO, (uint8_t *)&timeVal, sizeof(timeVal));

        sServerAddress.sin_family = AF_INET;
        sServerAddress.sin_addr.s_addr = INADDR_ANY;
        sServerAddress.sin_port = sl_Htons(port);

        res = sl_Bind(sockId, (const SlSockAddr_t *)&sServerAddress, sizeof(sServerAddress));
        DEBUG("bind %d\n\r", res);

        res |= sl_Listen (sockId, 1);
        DEBUG("listen %d\n\r", res);

        if (res == SL_SOC_OK)
        {
            DEBUG("listening port %d\n\r", port);
            return MSG_OK;
        }
        sl_Close(sockId);
    }

    return MSG_RESET;
}

static int waitForConnection(void)
{
    SlSocklen_t  in_addrSize;
    sockaddr_in  sClientAddress;

    // accepts a connection from a TCP client, if there is any, otherwise returns SL_EAGAIN
    connSockId = sl_Accept(sockId, (SlSockAddr_t *)&sClientAddress, (SlSocklen_t *)&in_addrSize);
    if (connSockId == SL_EAGAIN)
    {
        return SL_EAGAIN;
    }
    else
    {
        if (connSockId <= 0)
        {
            DEBUG("waitForConnection error %d\n\r", connSockId);
            return MSG_RESET;
        }

        sl_Close(sockId);
    }

    PRINT("TCP Terminal client connected from %d.%d.%d.%d\n\r",
        SL_IPV4_BYTE(sClientAddress.sin_addr.s_addr, 3), SL_IPV4_BYTE(sClientAddress.sin_addr.s_addr, 2),
        SL_IPV4_BYTE(sClientAddress.sin_addr.s_addr, 1), SL_IPV4_BYTE(sClientAddress.sin_addr.s_addr, 0));

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
    else
        return MSG_RESET;
}

static THD_FUNCTION(tcpTermServer, arg)
{
    (void) arg;
    int res;

    while (true)
    {
        res = createSocket(23);
        if (res == MSG_RESET)
        {
            PRINT("Failed to create socket\n\r");
        }
        else
        {
            while ((res = waitForConnection()) == SL_EAGAIN)
            {
                chThdSleepMilliseconds(100);
                if (res == MSG_RESET)
                {
                    PRINT("Failed while waiting for connection\n\r");
                }
            }

            if (res == MSG_OK)
            {
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
                }

                PRINT("Connection closed\n\r");
                sl_Close(connSockId);
            }
        }

        chThdSleepMilliseconds(500);
    }

    chHeapFree(rxBuff);
    chThdExit(MSG_OK);
}

void startTcpTermServer(void)
{
    rxBuff = chHeapAlloc(NULL, BUFSIZE);
    chThdCreateFromHeap(NULL, THD_WORKING_AREA_SIZE(2048), "tcpterm", NORMALPRIO+1, tcpTermServer, NULL);
}
