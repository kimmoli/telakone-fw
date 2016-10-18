#include <stdlib.h>
#include <string.h>
#include "hal.h"
#include "chprintf.h"
#include "shellcommands.h"
#include "wifi.h"

static void usage(BaseSequentialStream *chp);

void cmd_wifi(BaseSequentialStream *chp, int argc, char *argv[])
{
    if (argc == 0)
    {
        usage(chp);
    }

    else if (strcmp(argv[0], "start") == 0)
    {
        uint32_t wifimode = WIFIEVENT_MODE_AP;

        if (argc == 2)
        {
            if (strcmp(argv[1], "station") == 0)
                wifimode = WIFIEVENT_MODE_STATION;
        }

        chEvtBroadcastFlagsI(&wifiEvent, WIFIEVENT_START | wifimode);
    }

    else if (strcmp(argv[0], "stop") == 0)
    {
        chEvtBroadcastFlagsI(&wifiEvent, WIFIEVENT_STOP);
    }

    else if (strcmp(argv[0], "prog") == 0)
    {
#ifdef TK_CC3100_PROGRAMMING
        chEvtBroadcastFlagsI(&wifiEvent, WIFIEVENT_PROG);
#else
        chprintf(chp, "Programming disabled. Please build with TK_CC3100_PROGRAMMING defined\n\r");
#endif
    }

    else if (strcmp(argv[0], "ver") == 0)
    {
        chEvtBroadcastFlagsI(&wifiEvent, WIFIEVENT_VERSION);
    }

    else if (strcmp(argv[0], "conf") == 0)
    {
        chprintf(chp, "Mode   = %s\n\r", getenv("wifimode"));
        chprintf(chp, "SSID   = %s\n\r", getenv("ssid"));
        chprintf(chp, "IP     = %s\n\r", getenv("ip"));
        chprintf(chp, "Domain = %s\n\r", getenv("domain"));
    }

    else
    {
        usage(chp);
    }
}

void usage(BaseSequentialStream *chp)
{
    chprintf(chp, "wifi commands: start stop prog ver conf\n\r");
}
