#include <stdlib.h>
#include <string.h>
#include "hal.h"
#include "chprintf.h"
#include "shellcommands.h"
#include "tftp.h"
#include "wifi.h"
#include "filesystem.h"

void cmd_tftp(BaseSequentialStream *chp, int argc, char *argv[])
{
    (void) argc;
    (void) argv;
    uint8_t* buf = NULL;
    uint32_t len;

    if (!wifistatus->connected)
    {
        chprintf(chp, "Wifi is not connected.\n\r");
        return;
    }
    else
    {
        if (argc == 3)
        {
            buf = tftpc(0, argv[0], argv[1], &len);
            if (len > 0 && buf)
            {
                slFileWrite(argv[2], buf, len);
            }

            if (buf)
                chHeapFree(buf);
        }
        else
        {
            chprintf(chp, "tftp host hostfile localfile\n\r");
        }
    }
}

