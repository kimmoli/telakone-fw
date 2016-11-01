#include <stdlib.h>
#include <string.h>
#include "hal.h"
#include "chprintf.h"
#include "shellcommands.h"
#include "tftp.h"
#include "wifi.h"

void cmd_tftp(BaseSequentialStream *chp, int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    if (!wifistatus->connected)
    {
        chprintf(chp, "Wifi is not connected.\n\r");
        return;
    }
    else
    {
        chprintf(chp, "TODO\n\r");
    }

}

