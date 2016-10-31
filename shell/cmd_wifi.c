#include <stdlib.h>
#include <string.h>
#include "hal.h"
#include "chprintf.h"
#include "shellcommands.h"
#include "wifi.h"
#include "udp_server.h"
#include "udp_client.h"
#include "tcp_server.h"
#include "wifi_spawn.h"

static void usage(BaseSequentialStream *chp);

void cmd_wifi(BaseSequentialStream *chp, int argc, char *argv[])
{
    if (argc == 0)
    {
        usage(chp);
    }

    else if (strcmp(argv[0], "start") == 0)
    {
        chEvtBroadcastFlags(&wifiEvent, WIFIEVENT_START);
    }

    else if (strcmp(argv[0], "stop") == 0)
    {
        chEvtBroadcastFlags(&wifiEvent, WIFIEVENT_STOP);
    }

    else if (strcmp(argv[0], "scan") == 0)
    {
        chEvtBroadcastFlags(&wifiEvent, WIFIEVENT_SCAN);
    }

    else if (strncmp(argv[0], "connect", 4) == 0)
    {
        chEvtBroadcastFlags(&wifiEvent, WIFIEVENT_CONNECT);
    }

    else if (strncmp(argv[0], "disconnect", 4) == 0)
    {
        chEvtBroadcastFlags(&wifiEvent, WIFIEVENT_DISCONNECT);
    }

    else if (strcmp(argv[0], "ping") == 0)
    {
        if (hostToPing)
        {
            chHeapFree(hostToPing);
            hostToPing = NULL;
        }

        if (argc == 2)
        {
            int len = strlen(argv[1]) +1;
            hostToPing = chHeapAlloc(NULL, sizeof(char) * len);
            memcpy(hostToPing, argv[1], len);
        }

        chEvtBroadcastFlags(&wifiEvent, WIFIEVENT_PING);
    }

    else if (strcmp(argv[0], "udpserver") == 0)
    {
        if (argc == 2 && strncmp(argv[1], "stop", 4) == 0)
            stopUdpServer();
        else if (argc == 2)
            startUdpServer(strtol(argv[1], NULL, 10));
        else
            startUdpServer(0);
    }

    else if (strcmp(argv[0], "udpclient") == 0)
    {
        if (argc == 4)
        {
            udpSend(0, argv[1], strtol(argv[2], NULL, 10), argv[3], strlen(argv[3]));
        }
        else
        {
            chprintf(chp, "udpclient destination port data");
        }
    }

    else if (strcmp(argv[0], "udpstat") == 0)
    {
        if (argc == 2 && strncmp(argv[1], "stop", 4) == 0)
            stopUdpStatusSend();
        else if (argc == 2)
            startUdpStatusSend(argv[1], 0);
        else
            PRINT("destination url?");
    }

    else if (strcmp(argv[0], "term") == 0)
    {
        if (argc == 2 && strncmp(argv[1], "stop", 4) == 0)
            stopTcpTermServer();
        else if (argc == 2)
            startTcpTermServer(strtol(argv[1], NULL, 10));
        else
            startTcpTermServer(0);
    }

    else if (strcmp(argv[0], "prog") == 0)
    {
#ifdef TK_CC3100_PROGRAMMING
        chEvtBroadcastFlags(&wifiEvent, WIFIEVENT_PROG);
#else
        chprintf(chp, "Programming disabled. Please build with TK_CC3100_PROGRAMMING defined\n\r");
#endif
    }

    else if (strcmp(argv[0], "ver") == 0)
    {
        chEvtBroadcastFlags(&wifiEvent, WIFIEVENT_VERSION);
    }

    else if (strcmp(argv[0], "stat") == 0)
    {

        chprintf(chp, "-- Configration --\n\r");
        chprintf(chp, "Mode:              %s\n\r", getenv("wifimode"));
        chprintf(chp, "-- Access point --\n\r");
        chprintf(chp, "SSID:              %s\n\r", getenv("myssid"));
        chprintf(chp, "IP:                %s\n\r", getenv("ip"));
        chprintf(chp, "Domain:            %s\n\r", getenv("domain"));
        chprintf(chp, "-- Station --\n\r");
        chprintf(chp, "Connect to SSID:   %s\n\r", getenv("ssid"));
        chprintf(chp, "Key:               %s\n\r", getenv("key"));
        chprintf(chp, "Sec mode:          %s\n\r", secNames[strtol(getenv("sec"), NULL, 10)]);
        chprintf(chp, "-- Other --\n\r");
        chprintf(chp, "NTP server:        %s\n\r", getenv("ntp"));
        chprintf(chp, "Timezone:          %s\n\r", getenv("tz"));
        chprintf(chp, "-- Current status --\n\r");
        chprintf(chp, "Running:           %s\n\r", wifistatus->running ? "YES" : "NO");
        chprintf(chp, "Connected:         %s\n\r", wifistatus->connected ? "YES" : "NO");
        chprintf(chp, "IP Acquired:       %s\n\r", wifistatus->ipAcquired ? "YES" : "NO");
        chprintf(chp, "IP Leased:         %s\n\r", wifistatus->ipLeased ? "YES" : "NO");
        if (wifistatus->ipAcquired)
            chprintf(chp, "Own IP:            %d.%d.%d.%d\n\r",
                     SL_IPV4_BYTE(wifistatus->ownIpAddress, 3),
                     SL_IPV4_BYTE(wifistatus->ownIpAddress, 2),
                     SL_IPV4_BYTE(wifistatus->ownIpAddress, 1),
                     SL_IPV4_BYTE(wifistatus->ownIpAddress, 0));
        if (wifistatus->ipAcquired)
            chprintf(chp, "Gateway IP:        %d.%d.%d.%d\n\r",
                     SL_IPV4_BYTE(wifistatus->gatewayIpAddress, 3),
                     SL_IPV4_BYTE(wifistatus->gatewayIpAddress, 2),
                     SL_IPV4_BYTE(wifistatus->gatewayIpAddress, 1),
                     SL_IPV4_BYTE(wifistatus->gatewayIpAddress, 0));
        if (wifistatus->ipLeased)
            chprintf(chp, "Leased IP:         %d.%d.%d.%d\n\r",
                     SL_IPV4_BYTE(wifistatus->leasedIpAddress, 3),
                     SL_IPV4_BYTE(wifistatus->leasedIpAddress, 2),
                     SL_IPV4_BYTE(wifistatus->leasedIpAddress, 1),
                     SL_IPV4_BYTE(wifistatus->leasedIpAddress, 0));
    }

    else if (strcmp(argv[0], "ntp") == 0)
    {
        if (getenv("ntp") == NULL || getenv("tz") == NULL)
        {
            chprintf(chp, "No NTP server or timezone set.\n\r");
        }
        else
        {
            chprintf(chp, "Requesting time from %s\n\r", getenv("ntp"));
            chEvtBroadcastFlags(&wifiEvent, WIFIEVENT_GETTIME);
        }
    }

    else if (strcmp(argv[0], "http") == 0)
    {
        chEvtBroadcastFlags(&wifiEvent, WIFIEVENT_HTTPSERVER);
    }

    else
    {
        usage(chp);
    }
}

void usage(BaseSequentialStream *chp)
{
    chprintf(chp, "wifi commands: start stop prog ver show connect disconnect\n\r");
}
