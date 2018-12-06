#include <stdlib.h>
#include <string.h>
#include "wifi_ping.h"
#include "hal.h"
#include "wifi.h"
#include "helpers.h"

static void SimpleLinkPingReport(SlPingReport_t *pPingReport);

void SimpleLinkPingReport(SlPingReport_t *pPingReport)
{
    PRINT("%d packets transmitter, %d received, time %d\n\r",
        pPingReport->PacketsSent, pPingReport->PacketsReceived, pPingReport->TestTime);
    PRINT("rtt min/avg/max = %d %d %d\n\r",
          pPingReport->MinRoundTime, pPingReport->AvgRoundTime, pPingReport->MaxRoundTime);
}

msg_t slPing(uint32_t destinationIP, char *hostName)
{
    int res;
    SlPingStartCommand_t pingParams = {0};
    SlPingReport_t pingReport = {0};

    if (hostName != NULL)
    {
        uint32_t hostIpAddr;
        res = sl_NetAppDnsGetHostByName((signed char*)hostName, strlen(hostName), &hostIpAddr, SL_AF_INET);
        if (res)
        {
            PRINT("Error %d\n\r", res);
            return MSG_RESET;
        }
        pingParams.Ip = hostIpAddr;
    }
    else
    {
        pingParams.Ip = destinationIP;
    }

    // Set the ping parameters
    pingParams.PingIntervalTime = 500;
    pingParams.PingSize = 32;
    pingParams.PingRequestTimeout = 1000;
    pingParams.TotalNumberOfAttempts = 3;
    pingParams.Flags = 0;

    PRINT("Pinging %d.%d.%d.%d with %d packets. %d bytes each.\n\r",
        SL_IPV4_BYTE(pingParams.Ip, 3), SL_IPV4_BYTE(pingParams.Ip, 2),
        SL_IPV4_BYTE(pingParams.Ip, 1), SL_IPV4_BYTE(pingParams.Ip, 0),
        pingParams.TotalNumberOfAttempts, pingParams.PingSize);


    // Check for LAN connection
    res = sl_NetAppPingStart((SlPingStartCommand_t*)&pingParams, SL_AF_INET, (SlPingReport_t*)&pingReport, SimpleLinkPingReport);

    if (res)
    {
        PRINT("Failed to start ping. Error %d\n\r", res);
        return MSG_RESET;
    }

    return MSG_OK;
}
