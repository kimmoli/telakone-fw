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
        chEvtBroadcastFlagsI(&wifiEvent, WIFIEVENT_START);
    }
    else if (strcmp(argv[0], "stop") == 0)
    {
        chEvtBroadcastFlagsI(&wifiEvent, WIFIEVENT_STOP);
    }
    else if (strcmp(argv[0], "prog") == 0)
    {
        chEvtBroadcastFlagsI(&wifiEvent, WIFIEVENT_PROG);
    }
    else if (strcmp(argv[0], "ver") == 0)
    {
        chEvtBroadcastFlagsI(&wifiEvent, WIFIEVENT_VERSION);
    }
    else
    {
        usage(chp);
    }
}

void usage(BaseSequentialStream *chp)
{
    chprintf(chp, "wifi command\n\r");
    chprintf(chp, "- start\n\r");
    chprintf(chp, "- stop\n\r");
    chprintf(chp, "- prog\n\r");
}
