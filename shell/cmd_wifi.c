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
    else
    {
        usage(chp);
    }
}

void usage(BaseSequentialStream *chp)
{
    chprintf(chp, "wifi commands: start stop prog ver\n\r");
}
