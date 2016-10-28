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
        int port = 0;

        if (argc == 2)
        {
            if (strncmp(argv[1], "stop", 4) == 0)
                port = -1;
            else
                port = strtol(argv[1], NULL, 10);
        }

        startUdpServer(port);
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

    else if (strcmp(argv[0], "term") == 0)
    {
        startTcpTermServer(0);
    }

    else if (strcmp(argv[0], "stat") == 0)
    {
        uint32_t u, t;
        u = getUdpMessageCount();
        t = getTcpMessageCount();

        PRINT("udp %d tcp %d\n\r", u, t);
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

    else if (strcmp(argv[0], "show") == 0)
    {
        chprintf(chp, "Mode   = %s\n\r", getenv("wifimode"));
        chprintf(chp, "AP config:\n\r");
        chprintf(chp, "SSID   = %s\n\r", getenv("myssid"));
        chprintf(chp, "IP     = %s\n\r", getenv("ip"));
        chprintf(chp, "Domain = %s\n\r", getenv("domain"));
        chprintf(chp, "Station config:\n\r");
        chprintf(chp, "SSID   = %s\n\r", getenv("ssid"));
        chprintf(chp, "Key    = %s\n\r", getenv("key"));
        chprintf(chp, "Sec    = %s\n\r", secNames[strtol(getenv("sec"), NULL, 10)]);
    }

    else if (strcmp(argv[0], "test") == 0)
    {
        wifiSpawn((void *)wsptesti, NULL, 0);
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
