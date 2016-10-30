#include "hal.h"
#include "simplelink.h"
#include "wifi_gettime.h"

msg_t slGetSNTPTime(uint32_t ipAddr, char *hostName, RTCDateTime *timespec, int tz)
{
    SlSockAddrIn_t localAddr;
    SlSockAddrIn_t addr;
    uint32_t serverIpAddr;
    char dataBuf[48];
    int res = MSG_RESET;
    int addrSize = 0;
    int sockId;

    time_t transmitTime;

    if (hostName != NULL)
    {
        res = sl_NetAppDnsGetHostByName((signed char*)hostName, strlen(hostName), &serverIpAddr, SL_AF_INET);
        if (res)
        {
            PRINT("Error %d\n\r", res);
            return MSG_RESET;
        }
    }
    else
    {
        serverIpAddr = ipAddr;
    }

    memset(dataBuf, 0, sizeof(dataBuf));
    dataBuf[0] = '\x1b';

    PRINT("Connecting to NTP server at %d.%d.%d.%d\n\r",
        SL_IPV4_BYTE(serverIpAddr, 3), SL_IPV4_BYTE(serverIpAddr, 2),
        SL_IPV4_BYTE(serverIpAddr, 1), SL_IPV4_BYTE(serverIpAddr, 0));

    chThdSleepMilliseconds(10);

    addr.sin_family = SL_AF_INET;
    addr.sin_port = sl_Htons(123);
    addr.sin_addr.s_addr = sl_Htonl(serverIpAddr);

    sockId = sl_Socket(SL_AF_INET, SL_SOCK_DGRAM, SL_IPPROTO_UDP);

    res = sl_SendTo(sockId, dataBuf, sizeof(dataBuf), 0, (SlSockAddr_t *)&addr, sizeof(addr));
    if (res != sizeof(dataBuf))
    {
        /* could not send SNTP request */
        PRINT("Device couldn't send SNTP request\n\r");
        return MSG_RESET;
    }

    addrSize = sizeof(SlSockAddrIn_t);
    localAddr.sin_family = SL_AF_INET;
    localAddr.sin_port = 0;
    localAddr.sin_addr.s_addr = SL_INADDR_ANY;

    res = sl_Bind(sockId,(SlSockAddr_t *)&localAddr, addrSize);
    if(res < 0)
    {
        PRINT("Bind error\n\r");
        return MSG_RESET;
    }

    res = sl_RecvFrom(sockId, dataBuf, sizeof(dataBuf), 0, (SlSockAddr_t *)&localAddr,  (SlSocklen_t*)&addrSize);
    if (res <= 0)
    {
        PRINT("Device couldn't receive time information \n\r");
        return MSG_RESET;
    }

    if ((dataBuf[0] & 0x7) != 4)    /* expect only server response */
    {
        /* MODE is not server, abort */
        PRINT("Device is expecting response from server only!\n\r");
        return MSG_RESET;
    }
    else
    {
        /* Transmit Timestamp integer part (seconds) */
        transmitTime = (dataBuf[40] << 24) | (dataBuf[41] << 16) | (dataBuf[42] << 8) | (dataBuf[43]);

        /* Offset correction of 70 years: (70*365 + 17)*86400 */
        transmitTime -= 2208988800;

        /* Adjust timezone */
        transmitTime += tz * 3600;

        struct tm tim;
        struct tm *canary;

        /* If the conversion is successful the function returns a pointer to the object the result was written into.*/
        canary = localtime_r(&transmitTime, &tim);
        osalDbgCheck(&tim == canary);

        rtcConvertStructTmToDateTime(&tim, 0, timespec);
        rtcSetTime(&RTCD1, timespec);

        PRINT("NTP time returned is %s\r", asctime(&tim));
    }

    return MSG_OK;
}
