#include <stdlib.h>
#include <string.h>
#include "hal.h"
#include "chprintf.h"
#include "shellcommands.h"
#include "blinker.h"

void cmd_blink(BaseSequentialStream *chp, int argc, char *argv[])
{
    if (argc == 1 && strncmp(argv[0], "off", 2) == 0)
        chEvtBroadcastFlags(&blinkEvent, BLINKEVENT_BLINK_OFF);
    else if (argc == 1 && strncmp(argv[0], "slow", 2) == 0)
        chEvtBroadcastFlags(&blinkEvent, BLINKEVENT_SLOW_BLINK_ON);
    else if (argc == 1 && strncmp(argv[0], "fast", 2) == 0)
        chEvtBroadcastFlags(&blinkEvent, BLINKEVENT_FAST_BLINK_ON);
    else if (argc == 1 && strncmp(argv[0], "breathe", 2) == 0)
        chEvtBroadcastFlags(&blinkEvent, BLINKEVENT_BREATHE_BLINK_ON);
    else
        chprintf(chp, "blink style (off, slow, fast, breathe\n\r");
}

