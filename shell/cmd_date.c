#include <stdlib.h>
#include <string.h>
#include "hal.h"
#include "env.h"
#include "chprintf.h"
#include "shellcommands.h"
#include "helpers.h"

void cmd_date(BaseSequentialStream *chp, int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    struct tm timp = {0};
    RTCDateTime timespec;

    rtcGetTime(&RTCD1, &timespec);
    rtcConvertDateTimeToStructTm(&timespec, &timp, NULL);

    chprintf(chp, "RTC time is %s\r", asctime(&timp));
}
