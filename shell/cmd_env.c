#include <stdlib.h>
#include <string.h>
#include "hal.h"
#include "chprintf.h"
#include "shellcommands.h"
#include "env.h"
#include "helpers.h"

void cmd_env(BaseSequentialStream *chp, int argc, char *argv[])
{
    if (argc == 0)
    {
        int count = 0;
        char *buf;

        buf = (char *)chHeapAlloc(NULL, ENV_PAGE_SIZE);
        if (!buf)
            return;

        (void) envdump(buf, "\n\r", &count);
        chprintf(chp, "%s", buf);

        chHeapFree(buf);
    }
    else
    {
        if (strcmp(argv[0], "erase") == 0)
        {
            chprintf(chp, "Erasing... ");
            chThdSleepMilliseconds(2);

            if (enverase() == MSG_OK)
                chprintf(chp, "done\n\r");
            else
                chprintf(chp, "failed\n\r");
        }

        else if (strcmp(argv[0], "store") == 0)
        {
            int page = 0;
            int count = envstore(&page);

            if (count >= 0)
                chprintf(chp, "Stored %d variables to page %d.\n\r", count, page);
            else
                chprintf(chp, "Failed\n\r");
        }

        else if (strcmp(argv[0], "load") == 0)
        {
            int page = 0;
            int count = envload(&page);

            if (count > 0)
                chprintf(chp, "Loaded %d variables from page %d.\n\r", count, page);
            else
                chprintf(chp, "No variables found.\n\r");
        }

        else if (strcmp(argv[0], "set") == 0 && argc > 2)
        {
            if (setenv(argv[1], argv[2], 1))
                chprintf(chp, "Failed\n\r");
        }

        else if (strcmp(argv[0], "unset") == 0 && argc > 1)
        {
            if (unsetenv(argv[1]))
                chprintf(chp, "Failed\n\r");
        }

        else
        {
            chprintf(chp, "env erase, load, store, set arg value, unset arg\n\r");
        }
        return;
    }
}
