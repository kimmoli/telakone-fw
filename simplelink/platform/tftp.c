#include "hal.h"
#include "tftp.h"
#include "simplelink.h"
#include "filesystem.h"
#include "wifi.h"
#include "tcp_server.h"
#include "helpers.h"


#define TFTP_SERVER_PORT     69

#define OPCODE_RRQ           1
#define OPCODE_WRQ           2
#define OPCODE_DATA          3
#define OPCODE_ACK           4
#define OPCODE_ERROR         5

#define ERROR_FILE_NOT_FOUND 1

struct tftp_hdr
{
    uint16_t opcode;
    union
    {
        char filename[2];   /* With RRQ */
        uint16_t blockno;   /* With DATA and ACK */
        uint16_t errorcode;
    };
    char data[512];
};

static uint8_t* tftpExitFail(uint8_t *rebuf, int sockId)
{
    if (rebuf)
        chHeapFree(rebuf);

    sl_Close(sockId);
    return NULL;
}

uint8_t* tftpc(uint32_t hostIP, char *hostName, char *filename, uint32_t *rxLen)
{
    int res, len, retrycount, blocksize, expectblock, progress;
    static uint8_t buf[1500];
    struct tftp_hdr *t;
    uint32_t received;
    uint8_t* rebuf = NULL;
    uint32_t rebufSize = 10000;

    int sockId;
    uint32_t hostIpAddr = 0;
    SlSockAddrIn_t localAddr;
    SlSockAddrIn_t inAddr;
    SlSockAddrIn_t addr;
    int addrSize;

    if (hostName != NULL)
    {
        res = sl_NetAppDnsGetHostByName((signed char*)hostName, strlen(hostName), &hostIpAddr, SL_AF_INET);
        if (res)
        {
            PRINT("Failed to resolve host name %s. Error %d.\n\r", hostName, res);
            return NULL;
        }
    }
    else
    {
        hostIpAddr = hostIP;
    }

    PRINT("Get \"%s\" from %d.%d.%d.%d\n\r", filename,
          SL_IPV4_BYTE(hostIpAddr, 3), SL_IPV4_BYTE(hostIpAddr, 2),
          SL_IPV4_BYTE(hostIpAddr, 1), SL_IPV4_BYTE(hostIpAddr, 0));

    sockId = sl_Socket(SL_AF_INET, SL_SOCK_DGRAM, SL_IPPROTO_UDP);

    t = (struct tftp_hdr *) buf;
    blocksize = 512;
    received = 0;

    /*
     * Bind to an ephemeral port and send Read Request (RRQ) to the TFTP
     * server port 69.
     */

    localAddr.sin_family = SL_AF_INET;
    localAddr.sin_addr.s_addr = SL_INADDR_ANY;
    localAddr.sin_port = 0;

    addrSize = sizeof(SlSockAddrIn_t);

    res = sl_Bind(sockId, (SlSockAddr_t *)&localAddr, addrSize);
    if (res < 0)
    {
        DEBUG("Failed bind\n\r");
        return tftpExitFail(rebuf, sockId);
    }

    t->opcode = sl_Htons(OPCODE_RRQ);
    strcpy(t->filename, filename);
    len = strlen(filename);
    strcpy(t->filename + len + 1, "octet");

    /*
     * The TFTP server responds from a newly allocated ephemeral port to our old port
     * number which initiated the connection. Figure out where the reply came from
     * and use that as the destination port for the rest of the communication.
     */

    inAddr.sin_family = SL_AF_INET;
    inAddr.sin_addr.s_addr = SL_INADDR_ANY;
    inAddr.sin_port = 0;

    addr.sin_family = SL_AF_INET;
    addr.sin_addr.s_addr = sl_Htonl(hostIpAddr);
    addr.sin_port = sl_Htons(TFTP_SERVER_PORT);

    int inaddrSize = sizeof(inAddr);
    retrycount = 3;
    do
    {
        addrSize = sizeof(SlSockAddrIn_t);
        res = sl_SendTo(sockId, buf, 2 + len + 1 + 5 + 1, 0, (SlSockAddr_t *)&addr, addrSize);
        if (res < 0)
        {
            DEBUG("Failed send RRQ %d %d %d\n\r", retrycount, res, len);
            return tftpExitFail(rebuf, sockId);
        }

        setReceiveTimeout(sockId, 2000);

        len = sl_RecvFrom(sockId, buf, 1450, 0, (SlSockAddr_t *)&inAddr, (SlSocklen_t *) &inaddrSize);

        retrycount--;
    }
    while (retrycount > 0 && len == SL_EAGAIN);

    if (retrycount == 0 && len < 0)
    {
        PRINT("No response from server %d\n\r", len);
        return tftpExitFail(rebuf, sockId);
    }

    res = sl_Connect(sockId, (const SlSockAddr_t *)&addr, sizeof(SlSockAddr_t));
    if (res < 0)
    {
        DEBUG("Failed connect socket\n\r");
        return tftpExitFail(rebuf, sockId);
    }

    expectblock = 1;

    /*
     * Receive DATA frames until the length of the data is less than the maximum
     * block size
     */

    rebuf = chHeapAlloc(NULL, rebufSize);

    do  /* while (len == 512+4) */
    {
        progress = 0;
        retrycount = 3;
        do
        {  /* while (retrycount >= 0 && progress == 0) */

            if (expectblock != 1)
            {
                len = sl_RecvFrom(sockId, buf, 1450, 0, (SlSockAddr_t *)&inAddr, (SlSocklen_t *) &inaddrSize);
            }

            if (len >= 4)
            {
                if (t->opcode == sl_Htons(OPCODE_DATA))
                {
                    if (t->blockno == sl_Htons(expectblock))
                    {
                        t->opcode = sl_Htons(OPCODE_ACK);
                        t->blockno = sl_Htons(expectblock);
                        sl_SendTo(sockId, buf, 4, 0, (SlSockAddr_t *)&inAddr, inaddrSize);

                        if (received + len > rebufSize)
                        {
                            rebufSize += 10000;
                            uint8_t *reabuf = chHeapAlloc(NULL, rebufSize);
                            memcpy(reabuf, rebuf, received);
                            rebuf = reabuf;
                            chHeapFree(reabuf);
                        }

                        memcpy(rebuf + (expectblock-1)*blocksize, t->data, len-4);

                        expectblock++;
                        progress = 1;
                        received = received + len - 4;
                    }
                    else
                        DEBUG("Expected block %d but got %d\n\r", expectblock, sl_Htons(t->blockno));
                }
                if (t->opcode == sl_Htons(OPCODE_ERROR))
                {
                    PRINT("Received error %d\n\r", sl_Htons(t->errorcode));
                    return tftpExitFail(rebuf, sockId);
                }
            }

            if (len < 0)
            {
                /* no DATA packet within timeout, resend previous ACK */
                t->opcode = sl_Htons(OPCODE_ACK);
                t->blockno = sl_Htons(expectblock-1);
                sl_SendTo(sockId, buf, 4, 0, (SlSockAddr_t *)&inAddr, inaddrSize);
            }
            retrycount--;

        }
        while (retrycount > 0 && progress == 0);

        if (retrycount == 0 && progress == 0)
        {
            PRINT("No response from server\n\r");
            return tftpExitFail(rebuf, sockId);
        }

        PRINT(".");
        chThdSleepMicroseconds(100);
    }
    while (len == 512+4);

    PRINT("\n\rReceived %ld bytes.\n\r", received);

    sl_Close(sockId);

    *rxLen = received;

    return rebuf;
}
