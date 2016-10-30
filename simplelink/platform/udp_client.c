#include "udp_client.h"
#include "hal.h"
#include "simplelink.h"

int udpSend(uint32_t destinationIP, char *hostName, uint16_t port, char *data, int len)
{
    int res;
    int sockId;
    SlSockAddrIn_t  addr;
    uint32_t hostIpAddr;

    if (hostName != NULL)
    {
        res = sl_NetAppDnsGetHostByName((signed char*)hostName, strlen(hostName), &hostIpAddr, SL_AF_INET);
        if (res)
        {
            PRINT("Error %d\n\r", res);
            return MSG_RESET;
        }
    }
    else
    {
        hostIpAddr = destinationIP;
    }

    PRINT("[UDP] Sending %d bytes to %d.%d.%d.%d port %d\n\r", len,
        SL_IPV4_BYTE(hostIpAddr, 3), SL_IPV4_BYTE(hostIpAddr, 2),
        SL_IPV4_BYTE(hostIpAddr, 1), SL_IPV4_BYTE(hostIpAddr, 0),
        port);

    addr.sin_family = SL_AF_INET;
    addr.sin_port = sl_Htons(port);
    addr.sin_addr.s_addr = sl_Htonl(hostIpAddr);

    sockId = sl_Socket(SL_AF_INET, SL_SOCK_DGRAM, SL_IPPROTO_UDP);

    int addrSize = sizeof(SlSockAddrIn_t);
    res = sl_SendTo(sockId, data, len, 0, (SlSockAddr_t *)&addr, addrSize);
    if (res <= 0)
    {
        PRINT("Error %d\n\r", res);
        return MSG_RESET;
    }

    sl_Close(sockId);

    return MSG_OK;
}

